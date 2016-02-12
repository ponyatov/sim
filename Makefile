MODULE = $(notdir $(CURDIR))
sim.log: sim.sim ./exe.exe
	./exe.exe < $< > $@ && tail $(TAIL) $@
C = cpp.cpp ypp.tab.cpp lex.yy.c
H = hpp.hpp ypp.tab.hpp
F = -std=gnu++11 -DMODULE=\"$(MODULE)\"
./exe.exe: $(C) $(H) Makefile
	$(CXX) $(F) -o $@ $(C)
ypp.tab.cpp: ypp.ypp
	bison $<
lex.yy.c: lpp.lpp
	flex $<
