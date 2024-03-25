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
            variableOffset -= 4;
            if (i + 1 < tokens.size() && tokens[i+1].type == TokenType::IDENTIFIER) {
                varMap[tokens[i+1].value] = variableOffset;
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
                // 简单算术表达式处理，例如 a = b + c;
                cout << "    lw $v0, " << varMap[tokens[i+2].value] << "($fp)  # 加载第一个操作数\n";
                cout << "    sw $v0, " <<  "0($sp)  # 存储第一个操作数\n";
                
                cout << "    addiu $sp, $sp, -4\n"; //进栈，移动指针
                
                cout << "    lw $v0, " << varMap[tokens[i+4].value] << "($fp)  # 加载第二个操作数\n";
                cout << "    sw $v0, " <<  "0($sp)  # 存储第二个操作数\n";
                
                cout << "    addiu $sp, $sp, -4\n";  //进栈，移动指针
                
                cout << "    lw $t1, " <<  "4($sp)  # 加载第一个操作数\n";
                cout << "    lw $t0, " <<  "8($sp)  # 加载第二个操作数\n";
                
                

                if (tokens[i+3].value == "+") {
                    cout << "    add $t0, $t0, $t1  # 执行加法\n";
                } else if (tokens[i+3].value == "-") {
                    cout << "    sub $t0, $t0, $t1  # 执行减法\n";
                } else if (tokens[i+3].value == "*") {
                    cout << "    mul $t0, $t0, $t1  # 执行乘法\n";
                } else if (tokens[i+3].value == "/") {
                    cout << "    div $t0, $t1  # 执行除法，注意除法后结果在lo寄存器\n";
                    cout << "    mflo $t0  # 从lo寄存器移动结果到$t0\n";
                }
                
                cout << "sw $t0, 8($sp)\n";
                cout << "addiu $sp, $sp, 4\n";
                cout << "lw $v0, 4($sp)\n";

            }
        } else if (tokens[i].type == TokenType::RETURN_KEYWORD) {
//cout << "    move $v0, " << varMap[tokens[i+1].value] << "$t0  加载返回值\n";
            cout << "    sw $v0, "<<varMap[tokens[i+1].value]<<"($fp)\n";
            cout << "    addiu $sp, $sp, 4\n";
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
