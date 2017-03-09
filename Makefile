CC ?= cc
CFLAGS = -std=c11 -O2 -lpthread
DBGFLAGS = -W -Wall -Werror -Wextra -Wno-unused-parameter -pedantic -pedantic-errors -ggdb3 -std=c11 -lpthread
CFLAGS = $(DBGFLAGS) # debug

%: %.c
	$(CC) -o $@ $(CFLAGS) $<

all: client_udp server_udp client_tcp server_tcp
