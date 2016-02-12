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
//	S+=env->dump();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++) S+=(*it)->dump(depth+1);
	return S; }

Sim* Sim::eval() {
	Sim* E = env->lookup(val); if (E) return E;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it)=(*it)->eval();
	return this;
}

Sim* Sim::eq(Sim*o) {
	env->next->iron[val]=o;
	Sim*op=new Op("="); op->push(this); op->push(o); return op; }
Sim* Sim::at(Sim*o) {
	Sim*op=new Op("@"); op->push(this); op->push(o); return op; }
Sim* Sim::add(Sim*o) {
	Sim*op=new Op("+"); op->push(this); op->push(o); return op; }

Str::Str(string V):Sim("str",V) {}
string Str::tagval() { return tagstr(); }

List::List():Sim("[","]") {}

Fn::Fn(string V, FN F):Sim("fn",V) { fn=F; }
Sim* Fn::at(Sim*o) { return fn(o); }

Op::Op(string V):Sim("op",V) {}
Sim* Op::eval() {
	if (val=="~") return nest[0]; // quote
	Sim::eval();
	if (nest.size()==2) { // binops
		if (val=="=") return nest[0]->eq(nest[1]);
		if (val=="@") return nest[0]->at(nest[1]);
		if (val=="+") return nest[0]->add(nest[1]);
	}
	return this;
}

File::File(string V):Sim("file",V) {}
string File::tagval() { return tagstr(); }
Sim* file(Sim*o) { return new File(o->val); }

Dir::Dir(string V):Sim("dir",V) {}
string Dir::tagval() { return tagstr(); }
Sim* dir(Sim*o) { return new Dir(o->val); }
Sim* Dir::add(Sim*o) { File*F = new File(val+"/"+o->val);
	push(F); return F; }

Env* glob = new Env(NULL);
Env::Env(Env*e) { next=e; }
Sim* Env::lookup(string V) {
	if (iron.find(V)!=iron.end()) return iron[V];	// lookup local ok
	if (next) return next->lookup(V);				// recursive lookup
	return NULL;									// not found
}
string Env::dump() { string S;
	for (auto it=iron.begin(),e=iron.end();it!=e;it++)
		S += ","+it->first;
	if (next) S+=next->dump();
	return S; }
void env_init() {
	// metainfo
	glob->iron["MODULE"]	= new Str(MODULE);
	// fileio
	glob->iron["dir"]		= new Fn("dir",dir);
	glob->iron["file"]		= new Fn("file",file);
}
