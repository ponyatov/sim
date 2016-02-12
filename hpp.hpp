#ifndef _H_SIM
#define _H_SIM

#include <iostream>
#include <cstdlib>
//#include <cstdio>
//#include <cassert>
#include <vector>
#include <map>
using namespace std;

struct Sim;
struct Env { Env* next; Sim* lookup(string); map<string,Sim*> iron; };
extern Env *env;
extern void env_init();
struct Sim {
	string tag,val;
	Sim(string,string); Sim(string);
	vector<Sim*> nest; void push(Sim*);
	virtual string dump(int depth=0); string pad(int);
	virtual Sim* eval(Env*);
};
extern void W(Sim*);
extern void W(string);

struct List: Sim { List(); };

typedef Sim*(*FN)(Sim*);
struct Fn: Sim { Fn(string,FN); FN fn; };

struct Dir: Sim { Dir(string); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_SIM
