CC = gcc
CXX = g++
CXXFLAGS = -Wall -g -std=c++11

COBJS = gl3w.o
CXXOBJS = Math.o Mesh.o Pipeline.o Renderer.o Utl.o gcctest.o

%.o : %.c
	$(CC) -c -MMD -o $@ $<
	@sed -e "s/.*: /$*.o: /" < $*.d > $*.P
	@sed -e "s/.*: //" -e "s/\\$$//" < $*.d | fmt -1 | sed -e "s/^ *//" -e "s/$$/:/" >> $*.P
	@rm -f $*.d

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<
	@sed -e "s/.*:/$*.o:/" < $*.d > $*.P
	@sed -e "s/.*: //" -e "s/\\$$//" < $*.d | fmt -1 | sed -e "s/^ *//" -e "s/$$/:/" >> $*.P
	@rm -f $*.d

# Disassembly
%.lst : %.cpp FORCE
#	$(CXX) -g -std=c++11 -O2 -c -masm=intel -fverbose-asm -fno-asynchronous-unwind-tables -Wa,-adhln $< > $@
	$(CXX) -std=c++11 -O2 -c -masm=intel -Wa,-adln $< > $@

all: gltest.exe

clean:
	rm -f gltest.exe *.o *.d *.P *.lst

gltest.exe: $(COBJS) $(CXXOBJS)
	g++ $(COBJS) $(CXXOBJS) -o gltest -lfreeglut -lopengl32 -lgdi32 -lwinmm -L"c:/prj/freeglut/lib"

FORCE:

-include $(COBJS:.o=.P)
-include $(CXXOBJS:.o=.P)

