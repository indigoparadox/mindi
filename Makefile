
# vim: ft=make noexpandtab

BINDIR := bin
OBJDIR := obj

OBJECTS := $(OBJDIR)/src/mindifil.o $(OBJDIR)/src/mindievt.o
OBJECTS_TESTS := \
	$(OBJDIR)/tests/check.o \
	$(OBJDIR)/tests/chkmdfil.o \
	$(OBJDIR)/tests/chksetup.o \
	$(OBJDIR)/tests/chkmdevt.o

MD := mkdir -v -p

CFLAGS := -Wall -Werror -fpic

test_mindi: LDFLAGS += $(shell pkg-config --libs check) -L$(BINDIR)/static -lmindi
test_mindi: CFLAGS += -DCHECK -g

all: $(BINDIR)/static/libmindi.a $(BINDIR)/shared/libmindi.so test_mindi mididump

mididump: $(OBJDIR)/src/main.o | $(BINDIR)/static/libmindi.a
	$(CC) -o $@ $^ -L$(BINDIR)/static -lmindi $(LDFLAGS)

test_mindi: $(OBJECTS_TESTS) | $(BINDIR)/static/libmindi.a
	$(MD) $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BINDIR)/static/libmindi.a: $(OBJECTS)
	$(MD) $(dir $@)
	$(AR) rcs $@ $^

$(BINDIR)/shared/libmindi.so: $(OBJECTS)
	$(MD) $(dir $@)
	$(CC) -shared -o $@ $^

$(OBJDIR)/%.o: %.c
	$(MD) $(dir $@)
	$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean

clean:
	rm -rf $(OBJDIR); \
	rm -f test_mindi; \
	rm -rf $(BINDIR); \
	rm -f mididump

