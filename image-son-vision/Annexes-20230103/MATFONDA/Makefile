CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -Wpedantic
LDFLAGS=$(CFLAGS)
LDLIBS=-lm
RM=rm -f

OBJECTS=limace.o salade.o erreurs.o fondam.o exam.o

EXE=exam

.PHONY: all
all: $(EXE)

OBJ=limace.o salade.o erreurs.o fondam.o
exam: $(OBJ) exam.o 

limace.o: limace.h
salade.o: limace.h salade.h
erreurs.o: erreurs.h
fondam.o: limace.h salade.h erreurs.h
exam.o: limace.h erreurs.h fondam.h

clean:
	$(RM) $(OBJECTS) $(EXE)
