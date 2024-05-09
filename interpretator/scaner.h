#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <algorithm>

enum LexicalType {
    N_NULL,
    AND, BEGIN, STRINGT, ELSE, IF, INT,GOTO, BOOLEAN,
    NOT, OR, PROGRAM, READ,TRUE,FALSE,  WHILE, WRITE,
    FIN,
    SEMICOLON,DDOT, COMMA, ASSIGN, LBRACE, RBRACE, LPAREN, RPAREN, EQ, LSS,
    GTR, PLUS, MINUS, MUL, DIV, LEQ, NEQ, GEQ,INC,DEC,
    NUM,
    BOOL,
    STRING,
    LABEL,
    ID,

    POLIZ_LABEL,                                                                                
    POLIZ_ADDRESS,                                                                              
    POLIZ_GO,                                                                                   
    POLIZ_FGO,
    UN_MINUS,
};

class Token {
    LexicalType type;
    int value;
    std::string string;
public:
    Token(LexicalType t = N_NULL, int v = 0, std::string s = ""): type(t), value(v), string(s) {}
    LexicalType get_type() const { 
        return type;
    }
    int get_value() const { 
        return value; 
    }
    std::string get_string() const { 
        return string; 
    }
  friend std::ostream& operator<< (std::ostream &s, Token t);
};

class Identifier {
    std::string name;
    LexicalType type;
    bool isDeclared,isAssigned,isLabel;
    int val;
    std::string value;
public:
    Identifier() { 
        isDeclared = false; 
        isAssigned = false;
        isLabel = false; 
    }
    bool operator== (const std::string& s) const { 
        return name == s; 
    }
    Identifier(const std::string n) {
        name = n;
        isDeclared = false; 
        isAssigned = false;
        isLabel = false;
    }
    std::string get_name() const { 
      return name; 
    }
    std::string get_string() const { 
      return value; 
    }
    void put_string(const std::string & s) { 
      value = s; 
    }
    bool get_isLabel() const { 
      return isLabel; 
    }
    void put_isLabel(){ 
      isLabel = true;
    }
    bool get_isDeclared() const { 
      return isDeclared; 
    }
    void put_isDeclared() { 
      isDeclared = true; 
    }
    LexicalType get_type() const { 
      return type; 
    }
    void put_type(LexicalType t) { 
      type = t; 
    }
    bool get_isAssigned() const { 
      return isAssigned; 
    }
    void put_isAssigned() { 
      isAssigned = true; 
    }
    int  get_val() const { 
      return val; 
    }
    void put_val(int v) { 
      val = v; 
    }
};
// class Label{
//     std::string name;
//     bool isDeclared;
// public:
//     Label(std::string label = ""):name(label){}

//     std::string get_name()const{return name;}

//     bool operator== (const std::string& s) const { 
//         return name == s; 
//     }
// };

// std::vector<Label> LabelTable;

// int put_Label(const std::string & buf) {
//     std::vector<Label>::iterator it;

//     if ((it = find(LabelTable.begin(), LabelTable.end(), buf)) != LabelTable.end())
//         return it - LabelTable.begin();
//     LabelTable.push_back(Label(buf));
//     return LabelTable.size() - 1;
// }
std::vector<Identifier> IdentifierTable;

// int put_identifier(const std::string & buf) {
//     std::vector<Identifier>::iterator it;

//     if ((it = find(IdentifierTable.begin(), IdentifierTable.end(), buf)) != IdentifierTable.end())
//         return it - IdentifierTable.begin();
//     IdentifierTable.push_back(Identifier(buf));
//     return IdentifierTable.size() - 1;}
std::vector<Identifier> TLD;
std::vector<Identifier> TUD;

int put_identifier(std::vector<Identifier>& Table, const std::string& buf) {
    std::vector<Identifier>::iterator k;
    if ((k = find(Table.begin(), Table.end(), buf)) != Table.end()) return k - Table.begin();
    Table.push_back(Identifier(buf));
    return Table.size() - 1;
}



class Scanner {
    FILE * file;
    char   current_char;
    int look(const std::string& buf, const char ** list) {
        for(int i = 0; list[i]; ++i) {
            if (buf == list[i])
                return i;
        }
        return -1;
    }
    void read_next_char() {
        current_char = fgetc(file);
    }
public:
    static const char * Keywords[], * SpecialSymbols[], * Boolean[];
    Scanner(const char * program) {
        file = fopen(program, "r");
    }
    Token get_token();
};

const char *
Scanner::Keywords[] = { "", "and", "begin","string",  "else",  "if",  "int","goto","boolean", "not", "or", "program",
                      "read", "true","false", "while", "write",NULL };
const char *
Scanner::SpecialSymbols[] = { ";",":", ",", "=","{","}","(", ")", "==", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", "+=","-=",NULL };

Token Scanner::get_token() {
    enum    State { H, IDENT,LG,SLASH,RCOM, NUMB,COM, EQ, NEQ, QT};
    int     d, j;
    std::string  buf;
    State   current_state = H;
    do {
        read_next_char();
        switch (current_state) {
            case H:
                if (current_char == ' ' || current_char == '\n' || current_char == '\r' || current_char == '\t'); 
                    else if (isalpha(current_char)) {
                        buf.push_back(current_char);
                        current_state = IDENT;
                    }
                    else if (current_char == '"') {
                        current_state = QT;
                    }
                    else if (isdigit(current_char)) {
                        d = current_char - '0';
                        current_state = NUMB;
                    } 
                    else if (current_char == EOF)
                        return Token(FIN);
                    else if (current_char == '!') {
                        buf.push_back(current_char);
                        current_state = NEQ;
                    }
                    else if(current_char == '/') {
                        current_state = SLASH;
                    }
                    else if (current_char == '=') {
                        buf.push_back(current_char);
                        current_state = EQ;
                    }
                    
                    else if ( current_char == '+') {
                        buf.push_back(current_char);
                        current_state = EQ;
                    }
                    else if ( current_char == '<' || current_char == '>') {
                        buf.push_back(current_char);
                        current_state = LG;
                    }
                    else if ( current_char == '-') {
                        buf.push_back(current_char);
                        current_state = EQ;
                    }
                    else {
                        buf.push_back(current_char);
                        if (((j = look(buf, SpecialSymbols)))>=0) {
                            return Token((LexicalType)(j + (int)SEMICOLON), j);
                        }
                        else
                            throw current_char;
                    }
                    break;
            case IDENT:
                // if (isalpha(current_char) || isdigit(current_char)) {
                //     buf.push_back(current_char); 
                // }
                // else {
                //     if(current_char != ':'){
                //         ungetc(current_char, file);
                //         if (((j = look(buf, Keywords))>=0)) {
                //             return Token((LexicalType)j, j);
                //         }
                //         else {
                //             j = put_identifier(IdentifierTable, buf);
                //             return Token(ID, j);
                //         }
                //     }
                //     else{
                //         j = put_identifier(IdentifierTable,buf);
                //        return Token(ID, j); 
                //     }
                // }
                if (isalpha(current_char) || isdigit(current_char)) {
                    buf.push_back(current_char);
                }
                else if (current_char == ':') {
                    if ((j = look(buf, Keywords)>=0)) {
                        throw current_char;
                    }
                    else {
                        j = put_identifier(TLD, buf);
                        return Token((LexicalType)LABEL, j);
                    }
                }
                else {
                    ungetc(current_char, file);
                    if (((j = look(buf, Keywords))>=0)) {
                            return Token((LexicalType)j, j);
                    }
                    else {
                        j = put_identifier(IdentifierTable, buf);
                        return Token((LexicalType)ID, j);
                    }
                }
                ///
                break;
            case SLASH:
                if (current_char == '*') {
                    current_state = COM;
                }
                else {
                    ungetc(current_char, file);
                    buf.push_back(current_char);
                    j = look(buf, SpecialSymbols);
                    return Token(DIV, j);
                }
                break;
            case COM:
                if (current_char == '*') {
                    current_state = RCOM;
                }
                break;
            case RCOM:
                if (current_char == '/') {
                    current_state = H;
                }
                else {
                    ungetc(current_char, file);
                    current_state = COM;
                }
                break;  
            case QT:
                if (current_char == '"') {
                    return Token(STRING, 0, buf);
                }
                buf.push_back(current_char); 
                break;
            case NUMB:
                if (isdigit(current_char)) {
                    d = d * 10 + (current_char - '0');
                }
                else {
                    ungetc(current_char, file);
                    return Token(NUM, d);
                }
                break;
            case EQ:
                if (current_char == '=') {
                    buf.push_back(current_char);
                    j = look(buf, SpecialSymbols);
                    return Token((LexicalType)(j + (int)SEMICOLON), j);
                }
                else {
                    ungetc(current_char, file);
                    j = look(buf, SpecialSymbols);
                    return Token((LexicalType)(j + (int)SEMICOLON), j);
                }
                break;
            case LG:
                if (current_char == '=') {
                    buf.push_back(current_char);
                    j = look(buf, SpecialSymbols);
                    return Token((LexicalType)(j + (int)SEMICOLON), j);
                }
                else {
                    ungetc(current_char, file);
                    j = look(buf, SpecialSymbols);
                    return Token((LexicalType)(j + (int)SEMICOLON), j);
                }
                break;
            case NEQ:
                if (current_char == '=') {
                    buf.push_back(current_char);
                    j = look(buf, SpecialSymbols);
                    
                    return Token((LexicalType)(j + (int)SEMICOLON), j);
                }
                else
                    throw buf[0];
                break;

    }
  }
  while (true);
}
std::ostream & operator<< ( std::ostream &s, Token l ) {
    std::string t;
    if ( l.get_type() <= WRITE )
        t = Scanner::Keywords[l.get_type()];
    else if ( l.get_type() > FIN && l.get_type() <= GEQ )
        t = Scanner::SpecialSymbols[ l.get_type() - SEMICOLON];
    else if (l.get_type() == FIN){
        s<< "FIN" << std::endl;
        return s;
    }
    else if ( l.get_type() == STRING ){
        t = "STRING";
        s << '(' << t << ',' << l.get_string() <<',' << l.get_type() << ");" << std::endl;
        return s;
    }
    else if ( l.get_type() == NUM )
        t = "NUM";
    else if ( l.get_type() == ID )
        t = IdentifierTable[l.get_value()].get_name ();
    else if(l.get_type() == POLIZ_LABEL)
        t = "POLIZ_LABEL";
    else if(l.get_type() == POLIZ_ADDRESS)
        t = "POLIZ_ADDRESS";
    else if(l.get_type() == POLIZ_GO)
        t = "POLIZ_GO";
    else if(l.get_type() == POLIZ_FGO)
        t = "POLIZ_FGO";
    else if(l.get_type() == UN_MINUS)
        t = "UN_MINUS";
    else {
        s << '('  << l.get_value() << ',' << l.get_type() << ");" << std::endl;
        return s;
    }
    s << '(' << t << ',' << l.get_value() << ',' << l.get_type() << ");" << std::endl;
    return s;
}
