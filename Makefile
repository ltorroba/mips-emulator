CC				:= g++

SRCDIR		:= src
BUILDDIR	:= build
TARGET		:= bin/emulator

SRCEXT		:= cpp
SOURCES		:= $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS		:= $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
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

tests: test/test.cpp
	$(CC) $(CFLAGS) test/test.cpp $(INC) $(LIB) -o bin/test

.PHONY: clean
