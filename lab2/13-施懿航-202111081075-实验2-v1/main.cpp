#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <regex>
#include <fstream>

using namespace std;

enum class TokenType {
    UNKNOWN,
    MAIN_KEYWORD,
    PRINTLN_INT,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    RETURN_KEYWORD,
    INT_KEYWORD,
    IDENTIFIER,
    ASSIGN_OP,
    INTEGER_LITERAL,
    PLUS_OP, MINUS_OP, MULTIPLY_OP, DIVIDE_OP, MODULO_OP,
    AND_OP, OR_OP, XOR_OP, LOGIC_OP,
    SEMICOLON, LESS_THAN, LESS_THAN_EQUAL,
    GREATER_THAN, GREATER_THAN_EQUAL, EQUAL, NOT_EQUAL
};

struct Token {
    TokenType type;
    string value;
};

vector<Token> tokenize(const string& src) {
    vector<Token> tokens;

    regex tokenPatterns("\\bprintln_int\\b|\\bint\\b|\\breturn\\b|[A-Za-z_][0-9A-Za-z_]*|[0-9]+|(==)|(!=)|(<=)|(>=)|[=+\\-*/%;<>|&^]|\\bmain\\b|[()]|[{}]");



    auto words_begin = sregex_iterator(src.begin(), src.end(), tokenPatterns);
    auto words_end = sregex_iterator();

    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        smatch match = *i;
        string matchStr = match.str();
        Token token;
        token.value = matchStr;

        if (matchStr == "println_int") token.type = TokenType::PRINTLN_INT;
    else if (matchStr == "int") token.type = TokenType::INT_KEYWORD;
        else if (matchStr == "return") token.type = TokenType::RETURN_KEYWORD;
        else if (matchStr == "main") token.type = TokenType::MAIN_KEYWORD;
        else if (regex_match(matchStr, regex("[A-Za-z_][0-9A-Za-z_]*"))) token.type = TokenType::IDENTIFIER;//修改识别标识符的正则表达式
        else if (regex_match(matchStr, regex("[0-9]+"))) token.type = TokenType::INTEGER_LITERAL;
        else if (matchStr == "=") token.type = TokenType::ASSIGN_OP;
        else if (matchStr == ";") token.type = TokenType::SEMICOLON;
        else if (matchStr == "(") token.type = TokenType::LEFT_PAREN;
        else if (matchStr == ")") token.type = TokenType::RIGHT_PAREN;
        else if (matchStr == "{") token.type = TokenType::LEFT_BRACE;
        else if (matchStr == "}") token.type = TokenType::RIGHT_BRACE;
        else if (matchStr == "<") token.type = TokenType::LESS_THAN;
        else if (matchStr == "<=") token.type = TokenType::LESS_THAN_EQUAL;
        else if (matchStr == ">") token.type = TokenType::GREATER_THAN;
        else if (matchStr == ">=") token.type = TokenType::GREATER_THAN_EQUAL;
        else if (matchStr == "==") token.type = TokenType::EQUAL;
        else if (matchStr == "!=") token.type = TokenType::NOT_EQUAL;
		// 更新对应的条件分支，以识别新的运算符类型
        else if (matchStr == "+") token.type = TokenType::PLUS_OP;
        else if (matchStr == "-") token.type = TokenType::MINUS_OP;
        else if (matchStr == "*") token.type = TokenType::MULTIPLY_OP;
        else if (matchStr == "/") token.type = TokenType::DIVIDE_OP;
        else if (matchStr == "%") token.type = TokenType::MODULO_OP;
        else if (matchStr == "&") token.type = TokenType::AND_OP;
        else if (matchStr == "|") token.type = TokenType::OR_OP;
        else if (matchStr == "^") token.type = TokenType::XOR_OP;
        else token.type = TokenType::UNKNOWN;

        tokens.push_back(token);
    }

    return tokens;
}
// 判断是否为运算符
bool isOperator(TokenType type) {
    		 //type == TokenType::ARITH_OP || type == TokenType::LOGIC_OP ||
   return  type == TokenType::LESS_THAN || type == TokenType::LESS_THAN_EQUAL ||
           type == TokenType::GREATER_THAN || type == TokenType::GREATER_THAN_EQUAL ||
           type == TokenType::EQUAL || type == TokenType::NOT_EQUAL||
               //更新
           type == TokenType::PLUS_OP || type == TokenType::MINUS_OP ||
           type == TokenType::MULTIPLY_OP || type == TokenType::DIVIDE_OP ||
           type == TokenType::MODULO_OP || type == TokenType::AND_OP ||
           type == TokenType::OR_OP || type == TokenType::XOR_OP ;
 
}

// 运算符优先级
int getPrecedence(TokenType type) {
    switch (type) {
        case TokenType::MULTIPLY_OP:
        case TokenType::DIVIDE_OP:
        case TokenType::MODULO_OP:
            return 7;
        case TokenType::PLUS_OP:
        case TokenType::MINUS_OP:
            return 6;
        case TokenType::LESS_THAN:
        case TokenType::LESS_THAN_EQUAL:
        case TokenType::GREATER_THAN:
        case TokenType::GREATER_THAN_EQUAL:
            return 5;
        case TokenType::EQUAL:
        case TokenType::NOT_EQUAL:
            return 4;
        case TokenType::AND_OP:
            return 3;
        case TokenType::XOR_OP:
            return 2;
        case TokenType::OR_OP:
            return 1;
        default:
            return 0; // 对于未知类型或不适用的类型，返回最低优先级
    }
}


// 将中缀表达式转换为后缀表达式
vector<Token> infixToPostfix(const vector<Token>& infix) {
    stack<Token> opStack;
    vector<Token> postfix;
    for (const Token& token : infix) {
        if (token.type == TokenType::IDENTIFIER || token.type == TokenType::INTEGER_LITERAL) {
            postfix.push_back(token);
        } else if (token.type == TokenType::LEFT_PAREN) {
            opStack.push(token);
        } else if (token.type == TokenType::RIGHT_PAREN) {
            while (!opStack.empty() && opStack.top().type != TokenType::LEFT_PAREN) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop(); // 弹出左括号
        } else if (isOperator(token.type)) {
            while (!opStack.empty() && getPrecedence(opStack.top().type) >= getPrecedence(token.type)) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
    }
    while (!opStack.empty()) {
        postfix.push_back(opStack.top());
        opStack.pop();
    }
        cout << "# Postfix Expression: ";
    for (const Token& token : postfix) {
        cout << token.value << " ";
    }
    cout << endl;
    return postfix;
}


// 根据后缀表达式生成MIPS代码
void generateMIPSFromPostfix(const vector<Token>& postfix, const map<string, int>& varMap, const string& resultVar) {

    for (const Token& token : postfix) {
        if (token.type == TokenType::IDENTIFIER) {
            cout << "    lw $t0, " << varMap.at(token.value) << "($fp)" << endl; // 加载变量到$t0寄存器
            cout << "    sw $t0, 0($sp)" << endl; // 将$t0寄存器中的值存入内存栈
            cout << "addiu $sp, $sp, -4" << endl; // 栈指针下移4字节
        } else if (token.type == TokenType::INTEGER_LITERAL) {
            cout << "    li $t0, " << token.value << endl; // 将立即数加载到$t0寄存器
            cout << "    sw $t0, 0($sp)" << endl; // 将$t0寄存器中的值存入内存栈
            cout << "addiu $sp, $sp, -4" << endl; // 栈指针下移4字节
        } else if (isOperator(token.type)) {
            // 从内存栈中弹出两个操作数
            cout << "    lw $t1, 4($sp)" << endl;
            cout << "    lw $t2, 8($sp)" << endl;

            // 根据操作符生成相应的MIPS指令
            switch (token.type) {
                case TokenType::MULTIPLY_OP:
                    cout << "    mul $t0, $t1, $t2" << endl;
                    break;
                case TokenType::DIVIDE_OP:
                    cout << "    div $t1, $t2" << endl;
                    cout << "    mflo $t0" << endl;
                    break;
                case TokenType::MODULO_OP:
                    cout << "    div $t1, $t2" << endl;
                    cout << "    mfhi $t0" << endl;
                    break;
                case TokenType::PLUS_OP:
                    cout << "    add $t0, $t1, $t2" << endl;
                    break;
                case TokenType::MINUS_OP:
                    cout << "    sub $t0, $t1, $t2" << endl;
                    break;
                case TokenType::LESS_THAN:
                    cout << "    slt $t0, $t1, $t2" << endl;
                    break;
                case TokenType::LESS_THAN_EQUAL:
                    cout << "    slt $t0, $t2, $t1" << endl;
                    cout << "    xori $t0, $t0, 1" << endl;
                    break;
                case TokenType::GREATER_THAN:
                    cout << "    slt $t0, $t2, $t1" << endl;
                    break;
                case TokenType::GREATER_THAN_EQUAL:
                    cout << "    slt $t0, $t1, $t2" << endl;
                    cout << "    xori $t0, $t0, 1" << endl;
                    break;
                case TokenType::EQUAL:
                    cout << "    seq $t0, $t1, $t2" << endl;
                    break;
                case TokenType::NOT_EQUAL:
                    cout << "    sne $t0, $t1, $t2" << endl;
                    break;
                case TokenType::AND_OP:
                    cout << "    and $t0, $t1, $t2" << endl;
                    break;
                case TokenType::XOR_OP:
                    cout << "    xor $t0, $t1, $t2" << endl;
                    break;
                case TokenType::OR_OP:
                    cout << "    or $t0, $t1, $t2" << endl;
                    break;
                default:
                    // 操作符未实现
                    break;
            }
            // 将计算结果存入内存栈
            cout << "    sw $t0, 8($sp)" << endl;
            cout << "addiu $sp, $sp, 4" << endl; // 栈指针下移4字节
        }
    }

    // 将最终结果存储到目标变量
    cout << "    lw $t0, 4($sp)" << endl;
    cout << "    sw $t0, " << varMap.at(resultVar) << "($fp)" << endl;
    cout << "    addiu $sp, $sp, 4" << endl; // 恢复栈指针
}





            
// 完整的generateMIPS函数（包含中缀到后缀的转换和后缀表达式求值）
void generateMIPS(const vector<Token>& tokens) {
    map<string, int> varMap;
    int variableOffset = -4; // 假设栈帧从$fp向下增长
    vector<Token> expression; // 用于收集变量赋值的表达式
    int flag=1;
    int argv=0;
    
    //框架
    cout << ".data" << endl;
    cout << "newline: .asciiz \"\\n\"" << endl;
    cout << ".text" << endl;
    cout << ".globl main" << endl;
    
    for (size_t i = 0; i < tokens.size(); ++i) {
        
        //处理main函数
        if(tokens[i].type == TokenType::INT_KEYWORD && tokens[i+1].type == TokenType::MAIN_KEYWORD && tokens[i+2].type == TokenType::LEFT_PAREN && tokens[i+3].type == TokenType::RIGHT_PAREN){
                    cout << "main:" << endl;
                    cout << "move $fp, $sp # 设置帧指针" << endl;
                    cout << "addiu $sp, $sp, -0x100 # 为局部变量分配栈空间" << endl;
                    i += 4;
                    continue;
                } else if(tokens[i].type == TokenType::INT_KEYWORD && tokens[i+1].type == TokenType::MAIN_KEYWORD && tokens[i+2].type == TokenType::LEFT_PAREN && tokens[i+3].type == TokenType::INT_KEYWORD){
                    cout << "main:" << endl;
                    cout << "move $fp, $sp # 设置帧指针" << endl;
                    cout << "addiu $sp, $sp, -0x100 # 为局部变量分配栈空间" << endl;
                    i += 8;
                    argv = 1;
                    continue;
                }
        
        else if (tokens[i].type == TokenType::INT_KEYWORD) {
            // 处理变量声明
            cout << "    sw $zero, " << variableOffset << "($fp) # int " << tokens[i+1].value << endl;
            string varName = tokens[i+1].value;
            varMap[varName] = variableOffset;
            variableOffset -= 4; // 为下一个变量预留空间
            i += 2; // 跳过变量声明
        } else if (tokens[i].type == TokenType::IDENTIFIER && tokens[i+1].type == TokenType::ASSIGN_OP) {
    // 开始收集赋值表达式
    cout << "    # 开始处理表达式 " << tokens[i].value << endl;
    string varName = tokens[i].value;
    i += 2; // 跳过标识符和赋值操作符

    // 如果标识符后面只有一个立即数，则直接输出赋值语句
    if (tokens[i].type == TokenType::INTEGER_LITERAL && (i + 1 >= tokens.size() || tokens[i + 1].type == TokenType::SEMICOLON)) {
        cout << "    li $t0, " << tokens[i].value << endl;
        cout << "    sw $t0, " << varMap.at(varName) << "($fp)" << endl;
        continue; // 继续下一次循环
    }

    while (i < tokens.size() && tokens[i].type != TokenType::SEMICOLON) {
        expression.push_back(tokens[i]);
        ++i;
    }
    // 将中缀表达式转换为后缀表达式
    vector<Token> postfix = infixToPostfix(expression);
    // 根据后缀表达式生成MIPS代码
    generateMIPSFromPostfix(postfix, varMap, varName);
    expression.clear(); // 清空表达式以便下次使用
}

        //处理printf函数和return语句
        else if (tokens[i].type == TokenType::PRINTLN_INT){
        if (tokens[i+2].type == TokenType::IDENTIFIER) {
            cout << "    lw $a0, " << varMap[tokens[i+2].value] << "($fp)" << endl;
        }
        cout << "    li $v0, 1" << endl;
        cout << "    syscall" << endl;
        if (flag != 0) {
            cout << "    li $v0, 4" << endl;
            cout << "    la $a0, newline" << endl;
            cout << "    syscall" << endl;
        }
        flag++;
        i += 4;
        continue;
        }
        else if (tokens[i].type == TokenType::RETURN_KEYWORD) {
            if(tokens[i+1].type == TokenType::IDENTIFIER){
            cout << "    lw $v0, " << varMap[tokens[i+1].value] << "($fp)" << endl;
            break;}
            else if(tokens[i+1].type == TokenType::INTEGER_LITERAL){
                cout<<" li $v0, 0"<<"\n";
                cout<<"move $v1, $v0"<<"\n";
                cout<<"li $v0, 10"<<"\n";
                cout<<"syscall"<<"\n";
                break;
            }
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

