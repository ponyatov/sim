#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { return yyparse(); }

string Sim::dump(int) { return "<"+val+">"; }

void W(Sim*o) { cout << o->dump(); }
void W(string s) { cout << s; }
