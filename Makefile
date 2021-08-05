CC=		gcc
CFLAGS=		-g -Wall -std=gnu99 -Iinclude
LD=		gcc
LDFLAGS=	-Llib
AR=		ar
ARFLAGS=	rcs
TARGETS=	bin/thor

all:		$(TARGETS)

# TODO: Add rules for lib/libthor.a, bin/unit_{url,socket,http,timestamp}
# bin/thor and any objects they require

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

tests/%.o: tests/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

lib/libthor.a: src/hammer.o src/http.o src/socket.o src/timestamp.o src/url.o
	$(AR) $(ARFLAGS) $@ $^

bin/unit_%: tests/unit_%.o lib/libthor.a
	$(LD) $(LDFLAGS) -o $@ $^

bin/thor: src/thor.o lib/libthor.a
	$(LD) $(LDFLAGS) -o $@ $^

test:
	@$(MAKE) -sk test-all

test-all:               test-url test-socket test-http test-timestamp test-thor

test-url:		bin/unit_url
	@bin/test_url.sh

test-socket:		bin/unit_socket
	@bin/test_socket.sh

test-http:		bin/unit_http
	@bin/test_http.sh

test-timestamp:		bin/unit_timestamp
	@bin/test_timestamp.sh

test-thor:		bin/thor
	@bin/test_thor.sh

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) bin/unit_* lib/*.a src/*.o tests/*.o *.log *.input

.PHONY:		all test clean
