#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>

using namespace std;

enum class TokenType {
    INT_KEYWORD,
    RETURN_KEYWORD,
    IDENTIFIER,
    INTEGER_LITERAL,
    ASSIGN_OP,
    ARITH_OP, // 包括+, -, *, /
    SEMICOLON,
    UNKNOWN
};

struct Token {
    TokenType type;
    string value;
};

vector<Token> tokenize(const string& src) {
    vector<Token> tokens;
    regex tokenPatterns("\\bint\\b|\\breturn\\b|[a-zA-Z]|[0-9]+|[=+\\-*/;]");
    auto words_begin = sregex_iterator(src.begin(), src.end(), tokenPatterns);
    auto words_end = sregex_iterator();

    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        smatch match = *i;
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
    
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].type == TokenType::INT_KEYWORD) {
            variableOffset -= 4; //标识符对应类型的字节，因为本次实验只有INT，所以每次-4
            if (i + 1 < tokens.size() && tokens[i+1].type == TokenType::IDENTIFIER) {
                varMap[tokens[i+1].value] = variableOffset; //varMap用于存储标识符对应偏移量
                cout << "    sw $zero, " << variableOffset << "($fp) # int " << tokens[i+1].value << "\n";
            }
            i += 2; // 跳过声明语句中的标识符和分号
        } else if (tokens[i].type == TokenType::IDENTIFIER && i + 2 < tokens.size() && tokens[i+1].type == TokenType::ASSIGN_OP) {
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
            else {
                string varName = tokens[i].value;
                int offset = 2; // 跳过赋值符号
                
                cout << "    # 开始处理表达式 " << varName << "\n";
                if (tokens[i+offset].type == TokenType::INTEGER_LITERAL || tokens[i+offset].type == TokenType::IDENTIFIER) {
                    
                    if (tokens[i+offset].type == TokenType::INTEGER_LITERAL) {
                        cout << "    li $t0, " << tokens[i+offset].value << " # 加载立即数\n";
                    } else {
                        cout << "    lw $t0, " << varMap[tokens[i+offset].value] << "($fp)  # 加载变量\n";
                    }
                    cout <<"sw $t0," <<varMap[tokens[i].value]<<"($fp)  # 赋值\n";
                }
                
                //i += offset; // 跳过处理过的表达式
            }
        }
        else if (tokens[i].type == TokenType::RETURN_KEYWORD) {
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
