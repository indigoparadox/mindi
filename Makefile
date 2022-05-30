
# vim: ft=make noexpandtab

OBJECTS := src/mindifil.o
OBJECTS_TESTS := tests/check.o tests/chkmdfil.o tests/chksetup.o

CC := gcc
MD := mkdir -v -p

CFLAGS := -g -pg -Wall -Werror

BINDIR := bin
OBJDIR := obj

$(BINDIR)/libmindi.a: OBJDIR := obj/linux

test_mindi: LDFLAGS += $(shell pkg-config --libs check) -L$(BINDIR)/static -lmindi
test_mindi: CFLAGS += -DCHECK -g

all: $(BINDIR)/static/libmindi.a $(BINDIR)/shared/libmindi.so test_mindi

test_mindi: $(BINDIR)/static/libmindi.a $(addprefix $(OBJDIR)/,$(OBJECTS_TESTS))
	$(MD) $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BINDIR)/static/libmindi.a: $(addprefix $(OBJDIR)/,$(OBJECTS))
	$(MD) $(dir $@)
	$(AR) rcs $@ $^

$(BINDIR)/shared/libmindi.so: $(addprefix $(OBJDIR)/,$(OBJECTS))
	$(MD) $(dir $@)
	$(CC) -shared -o $@ $^

$(OBJDIR)/%.o: %.c
	$(MD) $(dir $@)
	$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean

clean:
	rm -rf obj; \
	rm -f test_mindi; \
	rm -rf bin

