#ifndef _H_SIM
#define _H_SIM

#include <iostream>
#include <cstdlib>
//#include <cstdio>
#include <cassert>
#include <vector>
#include <map>
using namespace std;

struct Sim;
struct Env { Env(Env*); Env* next; Sim* lookup(string); map<string,Sim*> iron; };
extern Env *glob;
extern void env_init();
struct Sim {
	string tag,val;
	Sim(string,string); Sim(string);
	Env*env;
	vector<Sim*> nest; void push(Sim*);
	virtual string dump(int depth=0); string pad(int);
	virtual string tagval(); string tagstr();
	virtual Sim* eval();
	virtual Sim* at(Sim*);
};
extern void W(Sim*);
extern void W(string);

struct Str: Sim { Str(string); string tagval(); };

struct List: Sim { List(); };

typedef Sim*(*FN)(Sim*);
struct Fn: Sim { Fn(string,FN); FN fn; Sim*at(Sim*); };

struct Op: Sim { Op(string); };//Sim*eval(); };

struct Dir: Sim { Dir(string); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_SIM
