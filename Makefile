
CC=gcc
CXX=g++


TARGET=lcdboardd
#LIBS=  lib/liblcdgfx.a

CFLAGS= $(IDIR) -W -Wall -D __ARM_LINUX__ -std=c++17

#OBJ+=main.o Lcdboard.o Mqtt.o Pugiparam.o

IDIR= -I ../lcdgfx/src/
IDIR+= -I /home/user/Work/lcdgfx/src/
IDIR+= -I inc
OBJDIR=obj
OUTDIR=/home/user
LDIR= -L lib
LIBS= -l mosquittopp -l pthread -l pugixml -l stdc++fs
LIBLCD=  lib/liblcdgfx.a
SRC=src

CFLAGS= $(IDIR) -W -Wall -D __ARM_LINUX__ -std=c++17

#	$(OBJDIR)/U8g2lib.cpp.o\
#OBJ+=$(OBJDIR)/main.o $(OBJDIR)/Lcdboard.o $(OBJDIR)/Mqtt.o $(OBJDIR)/Pugiparam.o

OBJ+=$(patsubst $(SRC)/%.cpp,$(OBJDIR)/%.o, $(wildcard $(SRC)/*.cpp))

all: directories $(TARGET) 

directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OUTDIR)

$(TARGET):$(OBJ)
	@echo Generating $(TARGET) ...
	@$(CXX) -o $(OUTDIR)/$@ $(OBJ) $(LDIR) $(LIBS) $(LIBLCD)

$(OBJDIR)/main.o: src/main.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(LIBS)
$(OBJDIR)/Mqtt.o: src/Mqtt.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(LIBS)
$(OBJDIR)/Lcdboard.o: src/Lcdboard.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(LIBS)
$(OBJDIR)/Pugiparam.o: src/Pugiparam.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(LIBS)

clean:
	@echo RM -rf *.o
	rm *.o
	rm $(OBJDIR)/*.o
