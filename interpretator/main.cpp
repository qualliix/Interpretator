#include "scaner.h"
#include "parser.h"
#include "executer.h"

class Interpretator {
    Parser   pars;
    Executer E;
public:
    Interpretator ( const char* program ): pars (program) {}
    void     interpretation ();
};
 
void Interpretator::interpretation () {
    pars.analyze ();
    E.execute ( pars.poliz );
}

int main(){
        try {
            cout << Token(NUM) << endl;
            Interpretator I ( "program.txt" );
            I.interpretation ();
            return 0;
        }
        catch(char c){
            if(c!= '~')std::cout << "unexpected symbol" << c << std::endl;
        }
        catch ( Token l ) {
            std::cout << "unexpected lexeme" << l << std::endl;
        }
    
    
}