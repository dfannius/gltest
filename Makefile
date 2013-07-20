CC = gcc
CXX = g++
CXXFLAGS = -Wall -g -std=c++11

CSRCS = gl3w.c
CXXSRCS = Camera.cpp Math.cpp MathTest.cpp Mesh.cpp Pipeline.cpp Renderer.cpp ShaderProgram.cpp TransformState.cpp Utl.cpp gcctest.cpp

DEPDIR = deps
OBJDIR = objs

COBJS = $(CSRCS:%.c=$(OBJDIR)/%.o)
CXXOBJS = $(CXXSRCS:%.cpp=$(OBJDIR)/%.o)

define makedeps
@sed -e "s|.*: |$(OBJDIR)/$*.o: |" < $(OBJDIR)/$*.d > $(DEPDIR)/$*.P
@sed -e "s/.*: //" -e "s/\\\\$$//" < $(OBJDIR)/$*.d | fmt -1 | sed -e "s/^ *//" -e "s/$$/:/" >> $(DEPDIR)/$*.P
@rm -f $(OBJDIR)/$*.d
endef

$(OBJDIR)/%.o : %.c
	$(CC) -c -MMD -o $@ $<
	$(makedeps)

$(OBJDIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<
	$(makedeps)

# Disassembly
%.lst : %.cpp FORCE
#	$(CXX) -g -std=c++11 -O2 -c -masm=intel -fverbose-asm -fno-asynchronous-unwind-tables -Wa,-adhln $< > $@
	$(CXX) -std=c++11 -O2 -c -masm=intel -Wa,-adln $< > $@

all: gltest.exe

clean:
	rm -f gltest.exe $(OBJDIR)/*.o $(OBJDIR)/*.d $(DEPDIR)/*.P *.lst

gltest.exe: $(COBJS) $(CXXOBJS)
	g++ $(COBJS) $(CXXOBJS) -o gltest -lfreeglut -lopengl32 -lgdi32 -lwinmm -L"c:/prj/freeglut/lib"

FORCE:

-include $(CSRCS:%.c=$(DEPDIR)/%.P)
-include $(CXXSRCS:%.cpp=$(DEPDIR)/%.P)

