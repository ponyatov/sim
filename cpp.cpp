#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { env_init(); return yyparse(); }

void W(Sim*o) { cout << o->dump(); }
void W(string s) { cout << s; }

Sim::Sim(string T,string V) { tag=T; val=V; env=new Env(glob); }
Sim::Sim(string V):Sim("",V) {}
void Sim::push(Sim*o) { nest.push_back(o); }

string Sim::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sim::tagval() { return "<"+tag+":"+val+">"; }
string Sim::tagstr() { return "<"+tag+":'"+val+"'>"; }
string Sim::dump(int depth) { string S = "\n"+pad(depth)+tagval();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++) S+=(*it)->dump(depth+1);
	return S; }

Sim* Sim::eval() {
	Sim* E = env->lookup(val); if (E) return E;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it)=(*it)->eval();
	return this;
}

Sim* Sim::at(Sim*o) {
	Sim*op=new Op("@"); op->push(this); op->push(o); return op; }

Str::Str(string V):Sim("str",V) {}
string Str::tagval() { return tagstr(); }

List::List():Sim("[","]") {}

Fn::Fn(string V, FN F):Sim("fn",V) { fn=F; }
Sim* Fn::at(Sim*o) { return fn(o); }

Op::Op(string V):Sim("op",V) {}
/*
Sim* Op::eval() {
	if (val=="~") return nest[0]; // quote
	Sim::eval();
	if (nest.size()==2) { // binops
		if (val=="@") return nest[0]->at(nest[1]);
	}
	return this;
}
*/

Dir::Dir(string V):Sim("dir",V) {}
Sim* dir(Sim*o) { return new Dir(o->val); }

Env* glob = new Env(NULL);
Env::Env(Env*e) { next=e; }
Sim* Env::lookup(string V) {
	Sim*E = iron[V]; if (E) return E;	// lookup local
	if (next) return next->lookup(V);	// lookup nested
	else return NULL;					// not found
}
void env_init() {
	// metainfo
	glob->iron["MODULE"] = new Str(MODULE);
	// fileio
	glob->iron["dir"] = new Fn("dir",dir);
}
