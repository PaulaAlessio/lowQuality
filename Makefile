# project name 
TARGET	= lowQuality

# compiler
#CC	= g++
#C	= gcc -H 
C	= gcc 

# compiling flags
CFLAGS   = -Wall -O3 -march=native -std=c11  -Iinclude/  
LFLAGS   = -Wall   


LINKER   = gcc -o

# change these to set the proper directories where each files should be
SRCDIR   = src
INCDIR   = include
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f




$(BINDIR)/$(TARGET): $(OBJECTS)
	@ echo "Linking..."
	@$(LINKER) $@  $(LFLAGS)   $(OBJECTS)
	@echo $(LINKER) $@  $(LFLAGS)   $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo "Compiling ..."
	@$(C) $(CFLAGS)  -c $< -o $@
	@echo $(C) $(CFLAGS)  "-c" $< "-o" $@
	@echo "Compiled "$<" successfully!"


.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
 
