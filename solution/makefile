CC=gcc
CFLAGS=-c -Wall -g -Wno-unused-variable
LDFLAGS=-lm

EXECUTABLE=trace

INC=-Iinc -Iunits/inc
SRCDIR=src
OBJDIR=obj
SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(addprefix $(OBJDIR)/,$(notdir $(SOURCES:.c=.o)))

TESTSOURCES=$(wildcard units/$(SRCDIR)/*.c)
TESTOBJECTS=$(addprefix units/$(OBJDIR)/,$(notdir $(TESTSOURCES:.c=.o)))

ifeq ($(OS),Windows_NT)
	SUPPRESS= >nul
	SUPPRESS_ALL= >nul 2>&1
	SUPPRESS_ERRORS= 2>nul
else
	SUPPRESS= >/dev/null
	SUPPRESS_ALL= >/dev/null 2>&1
	SUPPRESS_ERRORS= 2>/dev/null

    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
    endif
    ifeq ($(UNAME_S),Darwin)
    endif
endif

all: before $(SOURCES) $(EXECUTABLE)

before:
	-@mkdir obj $(SUPPRESS_ALL) ||: 
	-@mkdir units/obj $(SUPPRESS_ALL) ||: 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	-@rm $(OBJECTS) $(SUPPRESS_ALL) ||:
	-@rm $(TESTOBJECTS) $(SUPPRESS_ALL) ||:
	-@rm $(EXECUTABLE) $(SUPPRESS_ALL) ||:
	-@rm unittest $(SUPPRESS_ALL) ||:
	@echo "Binaries removed!"

unittest: before $(TESTSOURCES) $(TESTOBJECTS) $(filter-out obj/main.o, $(OBJECTS))
	$(CC) $(filter-out obj/main.o, $(OBJECTS)) $(TESTOBJECTS) $(LDFLAGS) -o $@

units/$(OBJDIR)/%.o: units/$(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

count:
	@find . -name '*.c' -or -name '*.h' | xargs wc -l