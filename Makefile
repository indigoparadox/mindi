
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

CFLAGS := -Wall -Werror -fpic -DUSE_ALSA -DUSE_STDINT

test_mindi: LDFLAGS += $(shell pkg-config --libs check) -L$(BINDIR)/static -lmindi
test_mindi: CFLAGS += -DCHECK -g

midibeep.exe: CFLAGS := -mm -os -hw -d0 -0 -DUSE_DOS -i=./src
midibeep.exe: CC := wcc
midibeep.exe: LD := wcl
midibeep.exe: LDFLAGS :=

all: $(BINDIR)/static/libmindi.a $(BINDIR)/shared/libmindi.so test_mindi mididump midibeep

mididump: $(OBJDIR)/src/mididump.o | $(BINDIR)/static/libmindi.a
	$(CC) -o $@ $^ -L$(BINDIR)/static -lmindi $(LDFLAGS)

midibeep: $(OBJDIR)/src/midibeep.o | $(BINDIR)/static/libmindi.a
	$(CC) -o $@ $^ -L$(BINDIR)/static -lmindi $(LDFLAGS) -lasound

midibeep.exe: objdos/src/midibeep.o $(subst $(OBJDIR),objdos,$(OBJECTS))
	$(LD) $(LDFLAGS) -fe=$@ $^

test_mindi: $(OBJECTS_TESTS) | $(BINDIR)/static/libmindi.a
	$(MD) $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BINDIR)/static/libmindi.a: $(OBJECTS)
	$(MD) $(dir $@)
	$(AR) rcs $@ $^

$(BINDIR)/shared/libmindi.so: $(OBJECTS)
	$(MD) $(dir $@)
	$(CC) -shared -o $@ $^

objdos/%.o: %.c
	$(MD) $(dir $@)
	$(CC) $(CFLAGS) -fo=$@ $(<:%.c=%)

$(OBJDIR)/%.o: %.c
	$(MD) $(dir $@)
	$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean

clean:
	rm -rf $(OBJDIR); \
	rm -rf objdos; \
	rm -f test_mindi; \
	rm -rf $(BINDIR); \
	rm -f mididump; \
	rm -f midibeep; \
	rm -f midibeep.exe

