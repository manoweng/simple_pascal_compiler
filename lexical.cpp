#include <iostream>
#include <fstream>
// #include <locale>
#include <string>
#include <vector>
// #include <cstdlib>
using namespace std;

int getSymbol(char c);
bool isFinalState(int num);
int printToken(int token, string value, string value1);

int matrix[15][19] = {
    //-   +   =   /   ;   ,   {   }   *  (   )   :  <  > 0-9 a-z err ' '  \n
    { 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0}, // estado 0
    {13, 13, 13, 13, 13, 13, 13, 13, 11, 9, 13,  4, 6, 4,  3,  2,  0, 14, 14}, // estado 1
    { 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0, 0, 0, 0,  2,  2,  0,  0,  0},  // estado 2
    { 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0, 0, 0,  3,  0,  0,  0,  0}, // estado 3
    { 0,  0,  5,  0,  0,  0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0}, // estado 4
    { 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0}, // estado 5
    { 0,  0,  7,  0,  0,  0,  0,  0,  0, 0,  0,  0, 0, 8,  0,  0,  0,  0,  0}, // estado 6
    { 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0}, // estado 7
    { 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0}, // estado 8
    { 0,  0,  0,  0,  0,  0,  0,  0, 10, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0}, // estado 9
    { 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0}, // estado 10
    { 0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 12,  0, 0, 0,  0,  0,  0,  0,  0}, // estado 11
    { 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0}, // estado 12
    { 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0}, // estado 13
    { 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0, 0, 0,  0,  0,  0, 14, 14}, // estado 14
};

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "Uso: lexico [Nome do Arquivo de Entrada] [Nome do Arquivo de Saida]" << endl;
        return(1);
    }

    int lastState = 0, currentState = 1, column = 0, line = 1;
    int symbol = -1, previousSymbol = -1;
    bool readingComments = false, error = false;
    fstream file;
    fstream output;
    file.open(argv[1], fstream::in);
    output.open(argv[2], fstream::out);
    char ch, chl, invalidSymbol;
    string value, valuel;

    while (file >> noskipws >> ch) {
        chl = tolower(ch);

        if (!error) column++;

        symbol = getSymbol(chl);
        value.push_back(ch);
        valuel.push_back(chl);

        if (symbol == -1) {
            if (!readingComments) {
                error = true;
                invalidSymbol = ch;
                symbol = previousSymbol;
            } else
                symbol = 16;
        }

        currentState = matrix[currentState][symbol];

        if (isFinalState(currentState)) {
            lastState = currentState;
        }

        if (currentState == 14) {
            lastState = 0;
            currentState = 1;
            value.clear();
            valuel.clear();
            if (symbol == 18) {
                column = 0;
                line++;
            }
        }

        if (currentState == 0) {
            file.unget();

            if (error && !readingComments) {
                output << "Simbolo inválido: " << invalidSymbol << endl;
                value.erase(value.begin() + (value.size() - 1));
                output << "Palavra: " << value << endl;
                output << "Erro na linha: " << line << " coluna: " << column << endl;
                return(1);
            }

            if (lastState == 12) readingComments = false;

            if (!readingComments) {
                value.erase(value.begin() + (value.size() - 1));
                valuel.erase(valuel.begin() + (valuel.size() - 1));
                output << printToken(lastState, valuel, value) << endl;

                if (lastState == 10) readingComments = true;
            }

            lastState = 0;
            currentState = 1;
            value.clear();
        }

        previousSymbol = symbol;
    }

    if (readingComments) output << "Arquivo terminou de forma incorreta!!! :( " << endl;

    file.close();
    output.close();
}

int getSymbol(char c) {
    if (c == '-') return 0;
    else if (c == '+') return 1;
    else if (c == '=') return 2;
    else if (c == '/') return 3;
    else if (c == ';') return 4;
    else if (c == ',') return 5;
    else if (c == '[') return 6;
    else if (c == ']') return 7;
    else if (c == '*') return 8;
    else if (c == '(') return 9;
    else if (c == ')') return 10;
    else if (c == ':') return 11;
    else if (c == '<') return 12;
    else if (c == '>') return 13;
    else if (c >= 97 && c <= 122) return 15;
    else if (c >= 48 && c <= 57) return 14;
    else if (c == ' ' || c == '\t') return 17;
    else if (c == '\n') return 18;
    else return -1;
}

bool isFinalState(int num) {
    if (num >= 2 && num <= 13) return true;
    else return false;
}

int printToken(int token, string value, string value1){
    vector<string> symbols;
    symbols.push_back("program");
    symbols.push_back("label");
    symbols.push_back("type");
    symbols.push_back("array");
    symbols.push_back("of");
    symbols.push_back("var");
    symbols.push_back("integer");
    symbols.push_back("procedure");
    symbols.push_back("function");
    symbols.push_back("begin");
    symbols.push_back("end");
    symbols.push_back("if");
    symbols.push_back("then");
    symbols.push_back("else");
    symbols.push_back("while");
    symbols.push_back("do");
    symbols.push_back("or");
    symbols.push_back("and");
    symbols.push_back("div");
    symbols.push_back("not");

    switch (token) {
        case 2:
        for (std::vector<string>::iterator it = symbols.begin(); it != symbols.end(); ++it) {
            if ((*it).compare(value) == 0)
            {
                if(value1 == "program")
                    return 1;
                else if(value1 == "label")
                    return 2;
                else if(value1 == "type")
                    return 3;
                else if(value1 == "array")
                    return 27;
                else if(value1 == "of")
                    return 32;
                else if(value1 == "var")
                    return 4;
                else if(value1 == "integer")
                    return 3;
                else if(value1 == "procedure")
                    return 5;
                else if(value1 == "function")
                    return 6;
                else if(value1 == "begin")
                    return 7;
                else if(value1 == "end")
                    return 31;
                else if(value1 == "if")
                    return 29;
                else if(value1 == "then")
                    return 30;
                else if(value1 == "else")
                    return 20;
                else if(value1 == "while")
                    return 8;
                else if(value1 == "do")
                    return 34;
                else if(value1 == "or")
                    return 42;
                else if(value1 == "and")
                    return 22;
                else if(value1 == "div")
                    return 21;
                else if(value1 == "not")
                    return 9;
                else
                    return 0;

            }
        }
        return 25;
        break;
        case 3:
        return 26;
        break;
        case 4:
        case 6:
        case 9:
        case 11:
        case 13:
            if(value1 == "(")
                return 10;
            else if(value1 == ")")
                return 11;
            else if(value1 == ";")
                return 12;
            else if(value1 == ":")
                return 16;
            else if(value1 == "+")
                return 40;
            else if(value1 == "-")
                return 41;
            else if(value1 == "/")
                return 21;
            else if(value1 == "[")
                return 17;
            else if(value1 == "]")
                return 18;
            else if(value1 == ".")
                return 13;
            else if(value1 == ",")
                return 14;
            else if(value1 == ">")
                return 39;
            else if(value1 == "<")
                return 36;
            else if(value1 == "*")
                return 23;
            else if(value1 == "=")
                return 15;
            else
                return 0;
            break;
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if(value1 == ">=")
                return 38;
            else if(value1 == "<=")
                return 37;
            else if(value1 == ":=")
                return 33;
            else if(value1 == "<>")
                return 35;
            else if(value1 == "(*")
                return 0; 
            else if(value1 == "*)")
                return 0;
            else if(value1 == "..")
                return 19;
            else
                return ("SIMBOLO COMPOSTO NAO IDENTIFICADO");
            break;
    }

    return "";
}
