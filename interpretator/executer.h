#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include "scaner.h"
#include "parser.h"
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::stack;
using std::string;
class Arg{
    int i;
    string s;
    bool is_string;
    bool is_bool;
public:
    Arg(int num = 0, string str = "", bool b = false){
        i = num;
        if(str != "")
            is_string = true;
        else
            is_string = false;
        s = str;
        is_bool = b;
    }
    int getInt(){
        return i;
    }
    string getString(){
        return s;
    }
    bool isString(){
        return is_string;
    }
    bool isBool(){
        return is_bool;
    }
    void putInt(int num){
        i = num;
    }
    void putString(string& str){
        s = str;
        is_string = true;
    }
};
class Executer {
public:
    void execute ( vector<Token> & poliz );
};
void Executer::execute ( vector<Token> & poliz ) {
    Token pc_el;
    stack <Arg> args;
    int index = 0, size = poliz.size();
    Arg i, j;
    while ( index < size ) {
        pc_el = poliz [ index ];
        switch ( pc_el.get_type () ) {
            case  TRUE: case  FALSE:  case BOOL: 
                args.push ( Arg(pc_el.get_value (),"",true) );
                break;
            case  NUM:case POLIZ_ADDRESS: case POLIZ_LABEL:
                args.push ( Arg(pc_el.get_value ()) );
                break;
            case STRING:
                args.push( Arg(0,pc_el.get_string ()));
                break;
            case  ID:
                i = pc_el.get_value ();
                if ( IdentifierTable[i.getInt()].get_isAssigned () ) {
                    if (IdentifierTable[poliz[index].get_value()].get_type() == STRING) {
                        std::string v = IdentifierTable[poliz[index].get_value()].get_string();
                        args.push(Arg(0,v));
                    }
                    else args.push ( Arg( IdentifierTable[i.getInt()].get_val ()) );
                    break;
                }
                else{
                    cout << "POLIZ: indefinite identifier "<< IdentifierTable[i.getInt()].get_name() << endl;
                    throw '~';
                }
                break;
            case  NOT:
                from_st ( args, i );
                args.push( Arg(!i.getInt(),"",true) );
                break;
            case UN_MINUS:
                from_st ( args, i );
                args.push( Arg(-i.getInt()) );
                break;
            case  OR:
                from_st ( args, i ); 
                from_st ( args, j );
                args.push (Arg(j.getInt() || i.getInt(),"",true)  );
                break;
 
            case  AND:
                from_st ( args, i );
                from_st ( args, j );
                args.push (Arg(j.getInt() && i.getInt(),"",true)  );
                break;
 
            case POLIZ_GO:
                from_st ( args, i );
                index = i.getInt() - 1;
                break;
 
            case POLIZ_FGO:
                from_st ( args, i );
                from_st ( args, j );
                if ( !j.getInt() ) index = i.getInt() - 1;
                break;
 
            case  WRITE:
                while (!args.empty()){
                    from_st ( args, j );
                    if(j.isString()){
                        cout << j.getString() << endl;
                    }
                    else if(j.isBool()){
                        if (j.getInt()==1) cout << "true";
                        else cout << "false";
                    }
                    else cout << j.getInt(); 
                }
                break;
            case  READ:
                int k;
                from_st ( args, i );
                if (  IdentifierTable[i.getInt()].get_type () ==  INT ) {
                    cin >> k;
                    IdentifierTable[i.getInt()].put_val (k);
                    IdentifierTable[i.getInt()].put_isAssigned ();
                }
                else if(IdentifierTable[i.getInt()].get_type () ==  STRING){
                    string j;
                    cin >> j;
                    IdentifierTable[i.getInt()].put_string(j);
                    IdentifierTable[i.getInt()].put_isAssigned ();
                    break;
                }
                break;
 
            case  PLUS:
                from_st ( args, i );
                from_st ( args, j );
                if(i.isString()){
                   args.push (Arg(0,i.getString() + j.getString())  ); 
                }
                else args.push (Arg(i.getInt() + j.getInt()) );
                break;
 
            case  MUL:
                from_st ( args, i );
                from_st ( args, j );
                args.push (Arg(i.getInt() * j.getInt())  );
                break;
 
            case  MINUS:
                from_st ( args, i );
                from_st ( args, j );
                args.push (Arg(j.getInt() - i.getInt())  );
                break;
            case SEMICOLON:
                while(!args.empty()) args.pop();
                break;
            case  DIV:
                from_st ( args, i );
                from_st ( args, j );
                if (i.getInt()) {
                    args.push (Arg(j.getInt() / i.getInt())  );
                    break;
                }
                else{
                    cout << "POLIZ:divide by zero" << endl;
                    throw '~';
                }
            case  EQ:
                from_st ( args, i );
                from_st ( args, j );
                if(i.isString()){
                   args.push (Arg((int)(i.getString() == j.getString()),"",true)  ); 
                }
                else args.push (Arg((int)(i.getInt() == j.getInt()),"",true) );
                break;
 
            case  LSS:
                from_st ( args, i );
                from_st ( args, j );
                if(i.isString()){
                   args.push (Arg((int)(i.getString() > j.getString()),"",true)  ); 
                }
                else args.push (Arg((int)(i.getInt() > j.getInt()),"",true) );
                break;
 
            case  GTR:
                from_st ( args, i );
                from_st ( args, j );
                if(i.isString()){
                   args.push (Arg((int)(i.getString() < j.getString()),"",true)  ); 
                }
                else args.push (Arg((int)(i.getInt() < j.getInt()),"",true) );
                break;
 
            case  LEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push (Arg((int)(i.getInt() >= j.getInt()),"",true) );
                break;
 
            case  GEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push (Arg((int)(i.getInt() <= j.getInt()),"",true) );
                break;
 
            case  NEQ:
                from_st ( args, i );
                from_st ( args, j );
                if(i.isString()){
                   args.push (Arg((int)(i.getString() != j.getString()),"",true)  ); 
                }
                else args.push (Arg((int)(i.getInt() != j.getInt()),"",true) );
                break;
 
            case  ASSIGN:
                from_st ( args, i );
                from_st ( args, j );
                if ( IdentifierTable[j.getInt()].get_type () ==  INT )
                    IdentifierTable[j.getInt()].put_val (i.getInt());
                else if(IdentifierTable[j.getInt()].get_type () ==  STRING)
                    IdentifierTable[j.getInt()].put_string(i.getString());    
                IdentifierTable[j.getInt()].put_isAssigned (); 
                break;
 
            default:{
                cout << "POLIZ: unexpected elem" << pc_el <<  endl;
                throw '~';
            }
        }//end of switch
        ++index;
    };//end of while
}
 