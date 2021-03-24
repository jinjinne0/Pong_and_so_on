#Makefile

#compiler
CXX=g++
#compile flags
CFLAGS=
#target file(executable)
TARGET=Main
#source code files
SRCS=Main.cpp Game.cpp
#object files
OBJS= $(SRCS:.cpp=.o)
#include directories
INCDIR=-I ../External/SDL/include
#additional library directories
LIBDIR=-L ../External/SDL/lib
#additional library
LIBS=-lSDL2-2.0.0

$(TARGET):$(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBDIR) $(LIBS)
#TODO
#これでは、install nameが意図しないものになる可能性あり。どうする？

.cpp.o:
	$(CXX) -c $< $(INCDIR)

Main.o:Game.hpp
Game.o:Game.hpp

all:clean $(TARGET)

.PHONY:clean
clean:
	rm -f $(OBJS) $(TARGET)