# Define compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -g

# Define directories
SRCDIR := ..
TESTDIR := .
BUILDDIR := ../build

# Source files
SRCS := $(wildcard $(SRCDIR)/*.c)
TESTSRCS := $(wildcard $(TESTDIR)/*.c)

# Headers
HEADERS := $(wildcard $(SRCDIR)/*.h) $(wildcard $(TESTDIR)/*.h) 

# Object file, one per source
OBJS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))
TESTOBJS := $(patsubst $(TESTDIR)/%.c,$(BUILDDIR)/%.o,$(TESTSRCS))

# Define executable
EXEC := $(BUILDDIR)/test.out

# Include directories
INCLUDES := -I$(SRCDIR) -I$(TESTDIR)

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS) $(TESTOBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o:  $(SRCDIR)/%.c $(HEADERS) | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILDDIR)/%.o: $(TESTDIR)/%.c $(HEADERS) | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -r $(BUILDDIR)

# Additional dependency to ensure that the build directory is created before
# any .o files are created
$(OBJS): | $(BUILDDIR)
$(TESTOBJS): | $(BUILDDIR)
