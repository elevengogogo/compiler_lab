#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>

using namespace std;

enum class TokenType {
    INT_KEYWORD,//整型关键字
    RETURN_KEYWORD,//返回关键字
    IDENTIFIER,//标识符
    INTEGER_LITERAL,//整数字面值
    ASSIGN_OP,//赋值操作符
    ARITH_OP, // 包括+, -, *, /
    SEMICOLON,//分号
    UNKNOWN//未知类型
};

struct Token {
    TokenType type;
    string value;
};

vector<Token> tokenize(const string& src) {
    vector<Token> tokens;
    regex tokenPatterns("\\bint\\b|\\breturn\\b|[a-zA-Z]|[0-9]+|[=+\\-*/;]");//正则表达式
    auto words_begin = sregex_iterator(src.begin(), src.end(), tokenPatterns);
    auto words_end = sregex_iterator();

    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        smatch match = *i;//std::smatch 类型是用来保存正则表达式匹配结果的类型
        string matchStr = match.str();
        Token token;
        token.value = matchStr;

        if (matchStr == "int") token.type = TokenType::INT_KEYWORD;
        else if (matchStr == "return") token.type = TokenType::RETURN_KEYWORD;
        else if (regex_match(matchStr, regex("[a-zA-Z]"))) token.type = TokenType::IDENTIFIER;
        else if (regex_match(matchStr, regex("[0-9]+"))) token.type = TokenType::INTEGER_LITERAL;
        else if (matchStr == "=") token.type = TokenType::ASSIGN_OP;
        else if (regex_match(matchStr, regex("[+\\-*/]"))) token.type = TokenType::ARITH_OP;
        else if (matchStr == ";") token.type = TokenType::SEMICOLON;
        else token.type = TokenType::UNKNOWN;

        tokens.push_back(token);
    }

    return tokens;
}

void generateMIPS(const vector<Token>& tokens) {
    int variableOffset = 0;
    map<string, int> varMap;

    for (size_t i = 0; i < tokens.size(); ++i) { //===========
        if (tokens[i].type == TokenType::INT_KEYWORD) {
            variableOffset -= 4;
            if (i + 1 < tokens.size() && tokens[i+1].type == TokenType::IDENTIFIER) {
                varMap[tokens[i+1].value] = variableOffset;
                if (i + 2 < tokens.size() && tokens[i+2].type == TokenType::ASSIGN_OP) {
                    // 处理带初始化的情况
                    if (i + 3 < tokens.size() && tokens[i+3].type == TokenType::INTEGER_LITERAL) {
                        // 带初始化值为整数的情况
                        cout << "    li $t0, " << tokens[i+3].value << " # load integer value\n";
                    } else {
                        // 带初始化值为变量的情况
                        cout << "    lw $t0, " << varMap[tokens[i+3].value] << "($fp)  # load variable value\n";
                    }
                    cout << "    sw $t0, " << variableOffset << "($fp) # int " << tokens[i+1].value << "\n";
                    i += 4; // Skip the identifier, '=', and initialization value
                } else {
                    // 处理不带初始化的情况
                    cout << "    sw $zero, " << variableOffset << "($fp) # int " << tokens[i+1].value << "\n";
                    i += 2; // Skip the identifier and ';'
                }
            }
}

         else if (tokens[i].type == TokenType::IDENTIFIER && i + 2 < tokens.size() && tokens[i+1].type == TokenType::ASSIGN_OP) {
            string varName = tokens[i].value;
            if (tokens[i+2].type == TokenType::INTEGER_LITERAL && tokens[i+3].type==TokenType::SEMICOLON)//a = 2;
            {
                cout << "    li $v0, " << tokens[i+2].value << " # " << varName << " = " << tokens[i+2].value << "\n";
                cout << "    sw $v0, " << varMap[varName] << "($fp)\n";
            } else if (i + 4 < tokens.size() && tokens[i+3].type == TokenType::ARITH_OP) {
                string varName = tokens[i].value;
                int offset = 2; // 跳过赋值符号
                bool isFirstOperand = true;

                cout << "    # 开始处理表达式 " << varName << "\n";
                while (i + offset < tokens.size() && tokens[i+offset].type != TokenType::SEMICOLON) {
                    if (tokens[i+offset].type == TokenType::INTEGER_LITERAL || tokens[i+offset].type == TokenType::IDENTIFIER) {
                        if (isFirstOperand) {
                            if (tokens[i+offset].type == TokenType::INTEGER_LITERAL) {
                                cout << "    li $t0, " << tokens[i+offset].value << " # 加载立即数\n";
                            } else {
                                cout << "    lw $t0, " << varMap[tokens[i+offset].value] << "($fp)  # 加载变量\n";
                            }
                            isFirstOperand = false;
                        } else {
                            if (tokens[i+offset].type == TokenType::INTEGER_LITERAL) {
                                cout << "    li $t1, " << tokens[i+offset].value << " # 加载立即数\n";
                            } else {
                                cout << "    lw $t1, " << varMap[tokens[i+offset].value] << "($fp)  # 加载变量\n";
                            }
                            // 根据前一个操作符执行操作
                            if (tokens[i+offset-1].type == TokenType::ARITH_OP) {
                                string op = tokens[i+offset-1].value;
                                if (op == "+") {
                                    cout << "    add $t0, $t0, $t1\n";
                                } else if (op == "-") {
                                    cout << "    sub $t0, $t0, $t1\n";
                                } else if (op == "*") {
                                    cout << "    mult $t0, $t1\n  mflo $t0\n";
                                } else if (op == "/") {
                                    cout << "    div $t0, $t1\n";
                                    cout << "    mflo $t0\n";
                                }
                            }
                        }
                    }
                    offset++;
                }
                cout << "    sw $t0, " << varMap[varName] << "($fp)  # 保存结果到 " << varName << "\n";
                i += offset; // 跳过处理过的表达式
            }

        } else if (tokens[i].type == TokenType::RETURN_KEYWORD) {

            //cout << "    sw $v0, "<<varMap[tokens[i+1].value]<<"($fp)\n";
            //cout << "    addiu $sp, $sp, 4\n";
            cout << "    lw $v0, " << varMap[tokens[i+1].value]<<"($fp)\n";
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <source_file>" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string src = buffer.str();
    file.close();

    vector<Token> tokens = tokenize(src);

    generateMIPS(tokens);

    return 0;
}
