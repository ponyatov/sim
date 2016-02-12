#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { return yyparse(); }

void W(Sim*o) { cout << o->dump(); }
void W(string s) { cout << s; }

Sim::Sim(string V) { val=V; }
void Sim::push(Sim*o) { nest.push_back(o); }

string Sim::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sim::dump(int depth) { string S = "\n"+pad(depth)+"<"+val+">";
	for (auto it=nest.begin(),e=nest.end();it!=e;it++) S+=(*it)->dump(depth+1);
	return S; }

List::List():Sim("[]") {}

