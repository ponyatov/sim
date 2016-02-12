#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { env_init(); return yyparse(); }

void W(Sim*o) { cout << o->dump(); }
void W(string s) { cout << s; }

Sim::Sim(string T,string V) { tag=T; val=V; }
Sim::Sim(string V):Sim("",V) {}
void Sim::push(Sim*o) { nest.push_back(o); }

string Sim::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sim::dump(int depth) { string S = "\n"+pad(depth)+"<"+tag+":"+val+">";
	for (auto it=nest.begin(),e=nest.end();it!=e;it++) S+=(*it)->dump(depth+1);
	return S; }

Sim* Sim::eval(Env*env) {
	Sim* E = env->lookup(val); if (E) return E;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it)=(*it)->eval(env);
	return this;
}

List::List():Sim("[","]") {}

Fn::Fn(string V, FN F):Sim("fn",V) { fn=F; }

Op::Op(string V):Sim("op",V) {}

Env* env;
Sim* Env::lookup(string V) { return iron[V]; }

Dir::Dir(string V):Sim("dir",V) {}
Sim* dir(Sim*o) { return new Dir(o->val); }

void env_init() {
	env = new Env;
	// metainfo
	env->iron["MODULE"] = new Sim(MODULE);
	// fileio
	env->iron["dir"] = new Fn("dir",dir);
}
