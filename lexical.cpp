#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

// TODO
// fazer a parte dos erros (contagem de linhas e colunas também)
// fazer a parte dos comentarios
// fazer o negocio do \n e \r

// matriz de estados
int state_matrix[17][22] = {
    //   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20    21
    // a-z 0-9   )   ;   =   -   +   [   ]   .   ,   >   :   <   {   (   *   }  \r  \n  ' '\t resto
    {    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}, // estado 0 erro
    {    2,  3,  4,  4,  4,  4,  4,  4,  4,  4,  4,  5,  5,  7, 10,  9, 11, 12, 13, 15,  16,   0}, // estado 1 inicial
    {    2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}, // estado 2 identificador
    {    0,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}, // estado 3 numero
    {    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}, // estado 4 simbolo simples[1]
    {    0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}, // estado 5 simbolo simples[2]
    {    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}, // estado 6 simbolo composto[1]
    {    0,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}, // estado 7 simbolo simples[3]
    {    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}, // estado 8 simbolo composto[2]
    {    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,  0,  0,  0,   0,   0}, // estado 9 simbolo simples[4]
    {    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}, // estado 10 abre comentarios
    {    0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}, // estado 11 simbolo simples
    {    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}, // estado 12 fecha comentarios
    {    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,   0,   0}, // estado 13 kreba linha
    {    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}, // estado 14 kreba linha
    {    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  0,   0,   0}, // estado 15 kreba linha
    {    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0}  // estado 16 espaco e \t
};

// retorna um vetor com as palavras reservadas
map<string,int> reservedWords();

// retorna o valor do caractere para a matriz de transicao
int getSymbolValue(char symbol);

// retorna um valor booleano indicando se e estado final ou nao
bool isFinalState(int state);

// reconhece o token e retorna o numero que o representa
int recognizeToken(int state, string token);

// retorna o valor do token dos caracteres como (, ), : e +
int recognizeSimpleSymbol(string token);

// retorna o valor do token dos strings <>, <=, :=
int recognizeCompostSymbol(string token);

/* start of main */
int main(int argc, char **argv) {
    // caso o numero de parametros nao esteja correto
    if (argc != 3) {
        cerr << "Uso: \tlexical arquivo_de_entrada arquivo_de_saida" << endl;
        return 1;
    }

    // arquivos de entrada e saida
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1]);
    output_file.open(argv[2]);

    // declaracao de variaveis iniciais
    char next_char;
    int last_state = 0, current_state = 1;
    int lines = 1, columns = 0, symbol = 21;
    bool reading_comments = false, error = false;
    int token_value;
    string token;

    // inicio do automato
    while (input_file >> noskipws >> next_char) {
        next_char = tolower(next_char);
        symbol = getSymbolValue(next_char);
        token.push_back(next_char);

        if (!error) columns++;

        current_state = state_matrix[current_state][symbol];

        // definir se está lendo comentarios

        // define o ultimo estado final
        if (isFinalState(current_state)) {
            last_state = current_state;
        }

        if (current_state == 0) {
            token.pop_back();
            input_file.unget();
            if (last_state < 13) {
                token_value = recognizeToken(last_state, token);
                cout << "token[" << last_state << "]: " << token << "[" << token_value << "]" << endl;
                // checar os erros
            }

            last_state = 0;
            current_state = 1;
            token.clear();
        }
    }

    input_file.close();
    output_file.close();
    return 0;
}
/* end of main */

/* start of reservedWords */
map<string,int> reservedWords() {
    // adiciona todas as palavras reservadas
    map<string,int> reserved;
    reserved["program"] = 0;
    reserved["label"] = 1;
    reserved["type"] = 2;
    reserved["array"] = 3;
    reserved["of"] = 4;
    reserved["var"] = 5;
    reserved["procedure"] = 6;
    reserved["function"] = 7;
    reserved["integer"] = 8;
    reserved["begin"] = 9;
    reserved["end"] = 10;
    reserved["if"] = 11;
    reserved["then"] = 12;
    reserved["else"] = 13;
    reserved["while"] = 14;
    reserved["do"];
    reserved["or"] = 15;
    reserved["and"] = 16;
    reserved["div"] = 17;
    reserved["not"] = 18;

    return reserved;
}
/* end of reservedWords */

/* start of getSymbolValue */
int getSymbolValue(char symbol) {
    if (symbol >= 97 && symbol <= 122) return 0;
    else if (symbol >= 48 && symbol <= 57) return 1;
    else if (symbol == ')') return 2;
    else if (symbol == ';') return 3;
    else if (symbol == '=') return 4;
    else if (symbol == '-') return 5;
    else if (symbol == '+') return 6;
    else if (symbol == '[') return 7;
    else if (symbol == ']') return 8;
    else if (symbol == '.') return 9;
    else if (symbol == ',') return 10;
    else if (symbol == '>') return 11;
    else if (symbol == ':') return 12;
    else if (symbol == '<') return 13;
    else if (symbol == '{') return 14;
    else if (symbol == '(') return 15;
    else if (symbol == '*') return 16;
    else if (symbol == '}') return 17;
    else if (symbol == '\r') return 18;
    else if (symbol == '\n') return 19;
    else if (symbol == ' ' || symbol == '\t') return 20;
    else return 21;
}
/* end of getSymbolValue */

/*  start of isFinalState */
bool isFinalState(int state) {
    if (state >= 2 && state <= 16) return true;
    else return false;
}
/* end of isFinalState */

/* start of recognizeToken */
int recognizeToken(int state, string token) {
    map<string,int> reserved_words = reservedWords();
    cout << state << endl;

    if (state == 2) {
        map<string,int>::iterator search = reserved_words.find(token);
        if (search != reserved_words.end()) return reserved_words[token];
        else return 36;
    } else if (state == 3) {
        return 37;
    } else if (state == 6 or state == 8) {
        return recognizeCompostSymbol(token);
    } else if (state == 9) {
        return recognizeSimpleSymbol(token);
    }

    return -1;
}
/* end of recoginzeToken */

/* start of recognizeTokenChar */
int recognizeSimpleSymbol(string token) {
    map<string,int> charMap;
    charMap[";"] = 18;
    charMap["."] = 19;
    charMap[","] = 20;
    charMap[":"] = 21;
    charMap["("] = 22;
    charMap[")"] = 23;
    charMap["="] = 24;
    charMap["<"] = 25;
    charMap[">"] = 26;
    charMap["+"] = 27;
    charMap["-"] = 28;
    charMap["*"] = 29;
    charMap["["] = 30;
    charMap["]"] = 31;

    return charMap[token];
}
/* end of recognizeTokenChar */

/* start of recognizeCompostSymbol */
int recognizeCompostSymbol(string token) {
    map<string,int> charMap;
    charMap[":="] = 32;
    charMap["<>"] = 33;
    charMap["<="] = 34;
    charMap[">="] = 35;

    return charMap[token];
}
/* end of recognizeCompostSymbol */
