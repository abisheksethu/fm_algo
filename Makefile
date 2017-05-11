#/******************************************************************************
#	File name: Makefile
#	Description: Makefile for FM algorithm implementation
#	Author: Sethupandi Abishek
#******************************************************************************/

#Define all variables
CC=g++

#Flags
OBJCFLAGS+=-c -fPIC -Wall
CFLAGS+=-Wall
LDFLAGS+=

#Source Files
SRC=common.c
#Object Files
OBJECTS=$(SRC:.c=.o)

#Libraries required
LIBPATH=

#Executables
MODULE=FM

INCLUDE=-I./

#FM Application
TESTFILE=fmalgorithm
TESTAPP=$(TESTFILE)
TESTSRC=$(TESTFILE).cpp

.c.o:
	@echo 'Generating Object Files'
	$(CC) $(OBJCFLAGS) $(INCLUDE) $< -o $@

all: $(SRC) $(OBJECTS)
	@echo 'Generating $(MODULE) object file'
	@echo ''
	@echo 'Generating $(MODULE) Shared Library'
	@echo ''
	$(CC) -shared -Wl,-soname,lib$(MODULE).so -o lib$(MODULE).so *.o 

tiny: $(SRC) $(OBJECTS)
	@echo 'Generating $(MODULE) object file'
	@echo ''
	@echo 'Generating $(MODULE) Shared Library'
	@echo ''
	$(CC) -shared -Os -Wl,-soname,lib$(MODULE).so -o lib$(MODULE).so *.o 

fmapp:
	$(CC) $(CFLAGS) $(TESTSRC) $(INCLUDE) lib$(MODULE).so -o $(TESTAPP) 

version:
	nm lib$(MODULE).so
	
clean:
	rm *.o lib$(MODULE).so $(TESTAPP)
	
