#ifndef _H_SIM
#define _H_SIM

#include <iostream>
#include <cstdlib>
//#include <cstdio>
//#include <cassert>
//#include <vector>
//#include <map>
using namespace std;

struct Sim {
	string val;
	Sim(string);
	virtual string dump(int depth=0);
};

extern void W(Sim*);
extern void W(string);

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_SIM