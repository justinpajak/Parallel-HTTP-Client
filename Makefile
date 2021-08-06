CC=		gcc
CFLAGS=		-g -Wall -std=gnu99 -Iinclude
LD=		gcc
LDFLAGS=	-Llib
AR=		ar
ARFLAGS=	rcs
TARGETS=	bin/thor

all:		$(TARGETS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

lib/libthor.a: src/hammer.o src/http.o src/socket.o src/timestamp.o src/url.o
	$(AR) $(ARFLAGS) $@ $^

bin/thor: src/thor.o lib/libthor.a
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) lib/*.a src/*.o tests/*.o *.log *.input

.PHONY:		all test clean
