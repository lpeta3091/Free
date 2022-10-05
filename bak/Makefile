CC=gcc
CXX=g++

TARGET=lcdboardd
IDIR= -I ../../Engine/lcdgfx/src/
IDIR+=/home/user/Engine/lcdgfx/src/
IDIR+= -I inc
OBJDIR=obj
OUTDIR=/home/user
LDIR= -L lib
LIBS=  lib/liblcdgfx.a

CFLAGS= $(IDIR) -W -Wall -D __ARM_LINUX__ -std=c++17

OBJ+=main.o Lcdboard.o Mqtt.o Pugiparam.o

#OBJ+=$(patsubst $(CSRCDIR)/%.c,$(OBJDIR)/%.o, $(wildcard $(CSRCDIR)/*.c))

all: directories $(TARGET)

directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OUTDIR)

$(TARGET):$(OBJ)
	@echo Generating $(TARGET) ...
	@$(CXX) -o $(OUTDIR)/$@ $(OBJ) $(LIBS) -l mosquittopp -l pthread -l pugixml -l stdc++fs

main.o: main.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(LIBS)
Mqtt.o: Mqtt.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(LIBS)
Lcdboard.o: Lcdboard.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(LIBS)
Mqttthread.o: Mqttthread.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(LIBS)
Pugiparam.o: Pugiparam.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(LIBS)


	#sudo systemctl daemon-reload
	#sudo systemctl restart $(TARGET)
	#echo $(shell sudo ./afterbuild.sh)
	#$(shell sudo ./afterbuild.sh)

clean:
	@echo RM -rf *.o
	rm *.o
#$(OBJDIR)/
#	@rm -rf $(OBJ)
#	@rm -rf $(OBJDIR)

#	@echo RM -rf $(OUTDIR)/
#	@rm -rf $(OUTDIR)

upload:
	scp $(OUTDIR)/$(TARGET) $(PI):~/

run:
	ssh $(PI)
