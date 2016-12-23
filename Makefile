CC				:= g++

SRCDIR		:= src
TESTDIR		:= test
BUILDDIR	:= build
TARGET		:= bin/emulator
T_TARGET	:= bin/tests

SRCEXT		:= cpp
SOURCES		:= $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS		:= $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
T_SOURCES	:= $(shell find $(TESTDIR) -type f -name *.$(SRCEXT))
T_OBJECTS	:= $(patsubst $(TESTDIR)/%,$(BUILDDIR)/test/%,$(T_SOURCES:.$(SRCEXT)=.o))
R_OBJECTS := $(shell find $(BUILDDIR) -type f -not -name main.o)
CFLAGS		:= -g #-Wall

LIB				:= # none yet
INC				:= -I include

$(TARGET): $(OBJECTS)
	@echo "	Linking..."
	@echo "	$(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo "	Compiling..."
	@mkdir -p $(BUILDDIR)
	@echo "	$(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "	Cleaning..."
	@echo "	$(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

tests: $(T_OBJECTS) $(R_OBJECTS)
	@echo "	Linking tests..."
	@echo "	$(CC) $^ -o $(T_TARGET) $(LIB)"; $(CC) $^ -o $(T_TARGET) $(LIB)

$(BUILDDIR)/test/%.o: $(TESTDIR)/%.$(SRCEXT)
	@echo "	Compiling tests..."
	@mkdir -p $(BUILDDIR)/test
	@echo "	$(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: clean
