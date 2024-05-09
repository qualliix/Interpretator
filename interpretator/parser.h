#pragma once

#include <iostream>

#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
using std::cout;
using std::endl;
using std::stack;

template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
}

class Parser {
    Token          curr_lex;
    LexicalType  c_type;
    int          c_val;
    Scanner      scan;
    stack < int >           st_int;
    stack < LexicalType >   st_lex;

    void P();
    void D1();
    void D();
    void O();
    void O1();
    void E();
    void E_OR();
    void E_AND();
    void E_B();
    void E_PS();
    void E_MD();
    void F();

    void ass(LexicalType type, int  value);
    void  dec (LexicalType type);
    void  check_id ();
    void  check_op ();
    void  check_not ();
    void check_m ();
    void  eq_type ();
    void  eq_bool ();
    void  check_id_in_read ();
    void check_label();
    void enumerate_labels();

    void  gl () {
        curr_lex  = scan.get_token ();
        c_type    = curr_lex.get_type ();
        c_val     = curr_lex.get_value ();
        cout << curr_lex << endl;
    }
public:
    Parser ( const char *program ) : scan (program) { }
    void  analyze();
    std::vector <Token> poliz;
};
 
void Parser::analyze () {
    gl ();
    P ();
    if (c_type !=  FIN)
        throw curr_lex;
    int i = 0;
    for (Token l : poliz) {
        cout << i << l;
        i++;
    }
    cout << endl << "End" << endl;
}
 
void Parser::P () {
    if ( c_type ==  PROGRAM ) {
        gl ();
    }
    else throw curr_lex;
    if(c_type ==   LBRACE){
        gl();
    }else throw curr_lex;

    D (); 
    
    O ();

    if(c_type !=   RBRACE) throw curr_lex;

    gl(); 
    check_label();
    enumerate_labels();
    
}
 
void Parser::D () {
    while(c_type ==  INT || c_type ==  BOOLEAN || c_type ==  STRINGT){
        D1();
        if(c_type ==  SEMICOLON) gl();
        else
            throw curr_lex;
    }
}
 
void Parser::D1 () {

    LexicalType t = c_type;
    gl(); 

    if(c_type !=  ID) throw curr_lex;
    else{
        st_int.push ( c_val );
        gl();
        if(c_type ==  ASSIGN){ 
                    gl();
                    if(t ==  STRINGT){
                        if(c_type !=   STRING) throw curr_lex;
                        else ass ( STRING,0); 
                    }else if (t == BOOLEAN){
                        if(c_type !=  BOOL)
                            throw curr_lex;
                        else{
                            if(c_type == FALSE) ass(BOOL,0);
                            else ass(BOOL,1);
                        }
                    }
                    else {
                        if(c_type ==  MINUS){
                                gl();
                                if(c_type !=  NUM) throw curr_lex;
                                else ass ( INT , - (curr_lex.get_value()));
                        }
                        else{
                            if(c_type == PLUS) gl();
                            if(c_type !=  NUM) throw curr_lex;
                            else ass ( INT ,curr_lex.get_value());
                        }
                    }
                    gl();
        }
        else {
            if(t ==  STRINGT) dec ( STRING );
            else if(t == BOOLEAN) dec ( BOOL );
            else dec ( INT );
        }
        while ( c_type ==  COMMA ){
            gl();
            if ( c_type !=  ID )
                throw curr_lex;
            else {
                st_int.push ( c_val );
                gl ();
                if(c_type ==  ASSIGN){ 
                    gl();
                    if(t ==  STRINGT){
                        if(c_type !=   STRING) throw curr_lex;
                        else ass ( STRING,0); 
                    }else if (t == BOOLEAN){
                        if(c_type !=  BOOL)
                            throw curr_lex;
                        else{
                            if(c_type == FALSE) ass(BOOL,0);
                            else ass(BOOL,1);
                        }
                    }
                    else {
                        if(c_type ==  MINUS){
                                gl();
                                if(c_type !=  NUM) throw curr_lex;
                                else ass ( INT , - (curr_lex.get_value()));
                        }
                        else{
                            if(c_type == PLUS) gl();
                            if(c_type !=  NUM) throw curr_lex;
                            else ass ( INT ,curr_lex.get_value());
                        }
                    }
                    gl();
                }
            }
        }
    }
}
 
void Parser::O() { 
    while(c_type !=   RBRACE){
        O1();
    }
}
void Parser::O1 () {
    int pl0, pl1, pl2, pl3;
    if(c_type == LABEL){
        // IdentifierTable[c_val].put_val(poliz.size());
        // if(IdentifierTable[c_val].get_isDeclared()){
        //     cout << "twice declared" << endl;
        //     throw '~';
        // }
        // else IdentifierTable[c_val].put_isDeclared();
        TLD[curr_lex.get_value()].put_val(poliz.size());
        gl();
        O1();
        return;
    }
    else if ( c_type ==  IF ) {
        gl ();
        if (c_type !=   LPAREN)
            throw curr_lex;
        else{
            gl();
            E ();
            eq_bool ();
            pl2 = poliz.size();
            poliz.push_back ( Token() );
            poliz.push_back ( Token(POLIZ_FGO) ); 
            if (c_type ==   RPAREN) {
                gl();
                O1();
                pl3 = poliz.size ();
                poliz.push_back ( Token () );
 
                poliz.push_back ( Token ( POLIZ_GO ) );
                poliz[pl2] = Token ( POLIZ_LABEL, poliz.size() );
                if (c_type ==  ELSE) {
                    gl();
                    O1();
                    poliz[pl3] = Token ( POLIZ_LABEL, poliz.size() );
                }
            } else
                throw curr_lex;
        }
    }//end if
    else if ( c_type ==  WHILE ) {
        pl0 = poliz.size ();
        gl ();
        if (c_type !=   LPAREN) {
            throw curr_lex;
        }else{
            gl();
            E();
            eq_bool ();
            pl1 = poliz.size (); 
            poliz.push_back ( Token () );
            poliz.push_back ( Token (POLIZ_FGO) );
            if (c_type !=   RPAREN)
                throw curr_lex;
            else{
                gl();
                O1();
                poliz.push_back ( Token ( POLIZ_LABEL, pl0 ) );
                poliz.push_back ( Token ( POLIZ_GO) );
                poliz[pl1] = Token ( POLIZ_LABEL, poliz.size() );
            }
        }
    }//end while
    // else if(c_type == GOTO){
    //     gl();
    //     if(c_type != ID) throw curr_lex;
    //     else{
    //         // pl4 = IdentifierTable[c_val].get_val();
    //         // poliz.push_back ( Token ( POLIZ_LABEL, pl4 ) );
    //         // poliz.push_back ( Token ( POLIZ_GO ) );
    //         // gl();
    //         int j = put_identifier(TUD, IdentifierTable[IdentifierTable.size() - 1].get_name());
    //         poliz.push_back(Token(POLIZ_LABEL, j));
    //         poliz.push_back(Token(POLIZ_GO));
            
    //     }
    // }
    else if (c_type == GOTO) {
        gl();
        if (c_type == ID) {
            int j = put_identifier(TUD, IdentifierTable[IdentifierTable.size() - 1].get_name());
            poliz.push_back(Token(POLIZ_LABEL, j));
            poliz.push_back(Token(POLIZ_GO));
            gl();
            if (c_type == SEMICOLON) gl();
            else throw curr_lex;
        }
        else throw curr_lex;
    }
    else if ( c_type ==  READ ) {
        gl ();
        if ( c_type !=   LPAREN )
            throw curr_lex;
        else{
            gl ();

            if (c_type !=  ID)
                throw curr_lex;
            else{
                check_id_in_read ();
                poliz.push_back( Token ( POLIZ_ADDRESS, c_val ) );
                gl();
            }
            if( c_type !=   RPAREN )
                throw curr_lex;
            else{
                gl ();
                poliz.push_back ( Token (READ) );
            }
            if(c_type !=  SEMICOLON)
                throw curr_lex;
            else 
                gl();
        }
    }//end read
    else if ( c_type ==  WRITE ) {
        gl ();
        if ( c_type ==   LPAREN ) {
            do {
                gl();
                E();
            }
            while ( c_type ==  COMMA );
            if ( c_type ==   RPAREN ){
                gl ();
                poliz.push_back ( Token ( WRITE ) );
            }
            else
                throw curr_lex;
            
            if(c_type !=  SEMICOLON)
                throw curr_lex;
            else
                gl();   
        }
        else
            throw curr_lex;
    }//end write
    else if (c_type ==  ID ) {
        check_id ();
        poliz.push_back (Token ( POLIZ_ADDRESS, c_val ) );
        gl();
        if ( c_type == ASSIGN ) {
            gl ();
            E ();
            eq_type ();
            poliz.push_back ( Token ( ASSIGN ) );
            poliz.push_back ( Token ( SEMICOLON ) ); 
        }
        else
            throw curr_lex;
    }// Оператор - выражение
    else if(c_type ==   LBRACE){ //составной оператор
        gl();
        O();
        if(c_type !=   RBRACE) 
            throw curr_lex;
        else gl();
    }else if(c_type ==  SEMICOLON){ //Пустой оператор?
        gl(); 
    }else
        throw curr_lex;
}
 
void Parser::E () {
    E_OR ();
    while ( c_type ==  ASSIGN ) {
        st_lex.push ( c_type );
        gl (); 
        E_OR (); 
        check_op ();
    }
}
 
void Parser::E_OR () {
    E_AND ();
    while ( c_type ==  OR) {
        st_lex.push ( c_type );
        gl ();
        E_AND ();
        check_op ();
    }
}
 
void Parser::E_AND () {
    E_B();
    while ( c_type ==  AND) {
        st_lex.push ( c_type );
        gl ();
        E_B ();
        check_op ();
    }
}

void Parser::E_B () {
    E_PS ();
    while ( c_type ==  GTR || c_type ==  GEQ || c_type ==  LSS || c_type ==  LEQ || c_type ==  NEQ || c_type ==  EQ) {
        st_lex.push ( c_type );
        gl ();
        E_PS ();
        check_op ();
    }
}
void Parser::E_PS () {
    E_MD ();
    while (c_type ==  PLUS || c_type ==  MINUS) {
        st_lex.push ( c_type );
        gl ();
        E_MD ();
        check_op ();
    }
}

void Parser::E_MD () {
    F ();
    while (c_type ==  MUL || c_type ==  DIV) {
        st_lex.push(c_type);
        gl ();
        F ();
        check_op ();
    }
}
 
void Parser::F () {
    if ( c_type ==  ID ) {
        check_id ();
        poliz.push_back ( Token ( ID, c_val ) );
        gl ();
    }
    else if ( c_type ==  NUM ) {
        st_lex.push ( INT );
        poliz.push_back ( curr_lex );
        gl ();
    }
    else if(c_type ==   STRING){
        st_lex.push ( STRING );
        poliz.push_back ( curr_lex );
        gl();
    }
    else if ( c_type ==  TRUE ) {
        st_lex.push (  BOOL );
        poliz.push_back ( Token ( TRUE, 1) );
        gl ();
    }
    else if ( c_type ==  FALSE) {
        st_lex.push (  BOOL );
        poliz.push_back ( Token ( FALSE, 0) );
        gl ();
    }
    else if(c_type == PLUS){
        gl();
        F();
    }
    else if ( c_type ==  MINUS) {
        gl (); 
        F ();
        check_m(); 
    }
    else if ( c_type ==  NOT) {
        gl (); 
        F ();
        check_not (); 
    }
    else if ( c_type ==   LPAREN ) {
        gl (); 
        E ();
        if ( c_type ==   RPAREN)
            gl ();
        else 
            throw curr_lex;
    }else 
        throw curr_lex;
}

void Parser::check_label() {
    if (TLD.size() != TUD.size()) throw "LABEL TABLES";
    for (int i = 0; i < (int)TLD.size(); i++)
    {
        int j = 0;
        for (; j < (int)TUD.size(); j++)
        {
            if (TLD[i].get_name() == TUD[j].get_name()) {
                TUD[j].put_val(TLD[j].get_val());
                break;
            }
        }
        if (j == (int)TUD.size()) throw "USED UNITIALIZED LABEL";
        
    }
}

void Parser::enumerate_labels() {
    for (int i = 0; i < (int)poliz.size(); i++)
    {
        if (poliz[i].get_type() == POLIZ_LABEL) {
            int adr = TUD[poliz[i].get_value()].get_val();
            poliz[i] = Token(POLIZ_LABEL, adr);
        }
    }
}
void Parser::dec ( LexicalType type ) {
    int i;
    while ( !st_int.empty () ) {
        from_st ( st_int, i );
        if ( IdentifierTable[i].get_isDeclared () )
        {
            cout << "twice declared " <<  IdentifierTable[i].get_name()<< endl;
            throw '~'; 
        } 
           
        else {
            IdentifierTable[i].put_isDeclared ();
            IdentifierTable[i].put_type ( type );
        }
    }
}
void Parser::ass ( LexicalType type, int value) {
    int i;
    while ( !st_int.empty () ) {
        from_st ( st_int, i );
        if ( IdentifierTable[i].get_isDeclared () )
        {
            cout << "twice declared " <<  IdentifierTable[i].get_name()<< endl;
            throw '~'; 
        } 
           
        else {
            IdentifierTable[i].put_isDeclared ();
            IdentifierTable[i].put_isAssigned ();
            IdentifierTable[i].put_type ( type );
            IdentifierTable[i].put_val(value);
            IdentifierTable[i].put_string(curr_lex.get_string());
        }
    }
}
 
void Parser::check_id () {
    if ( IdentifierTable[c_val].get_isDeclared())
        st_lex.push ( IdentifierTable[c_val].get_type () );
    else if(!IdentifierTable[c_val].get_isLabel())
        {
            cout << "not declared " <<  IdentifierTable[c_val].get_name()<< endl;
            throw '~'; 
        }
}
void Parser::check_op () {
    LexicalType t1, t2, op, t =  INT, r =  BOOL;
 
    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );
    if(t1 == STRING || t2 == STRING){
        if (op == EQ || op ==  LSS || op ==  GTR || op ==  LEQ || op ==  GEQ || op ==  NEQ)
            r =  BOOL;
        else if(op == PLUS)
            r = STRING;
        else{
            cout << "wrong operation" << op <<  endl;
            throw '~'; 
        }
        st_lex.push(r);
        poliz.push_back ( Token ( op ) );
        return;
    }
    else if ( op ==  PLUS || op ==  MINUS || op ==  MUL || op ==  DIV )
        r =  INT;
    if ( op ==  OR || op ==  AND)
        t =  BOOL;
    if (op == EQ || op ==  LSS || op ==  GTR || op ==  LEQ || op ==  GEQ || op ==  NEQ)
        r =  BOOL;
    if ( t1 == t2  &&  t1 == t ) 
        st_lex.push (r);
    else
        {
            cout << "wrong types in operation" <<   endl;
            throw '~'; 
        }
    poliz.push_back (Token (op) );
}
 
void Parser::check_not () {
    if (st_lex.top() !=  BOOL)
        {
            cout << "not boolean type in not" <<   endl;
            throw '~';  
        }
    else  
        poliz.push_back ( Token ( NOT) );
}
void Parser::check_m () {
    if (st_lex.top() !=  INT)
        {
            cout << "not int type in unary +-" <<   endl;
            throw '~';  
        }
    else{
        poliz.push_back ( Token(UN_MINUS) );
    }  
}
 
void Parser::eq_type () {
    LexicalType t;
    from_st ( st_lex, t );
    if ( t != st_lex.top () )
        {
            cout << "wrong types in assign" <<   endl;
            throw '~';  
        }
    st_lex.pop();
}
 
void Parser::eq_bool () {
    if ( st_lex.top () !=  BOOL )
        {
            cout << "not boolean expression" <<   endl;
            throw '~';  
        }
    st_lex.pop ();
  }
 
void Parser::check_id_in_read () {
    if ( !IdentifierTable[c_val].get_isDeclared() )
        {
            cout << "not declared" <<  endl;
            throw '~';  
        };
}