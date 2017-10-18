#
#    Makefile for libcheapbot
#

PREFIX     := /usr/local
LIBDIR     := $(PREFIX)/lib
INCLUDEDIR := $(PREFIX)/include
MAKE       := make
CC         := cc
RM         := rm
CFLAGS     := -O2 -g -Wall -Wextra
LDFLAGS    :=
CMDLINE    := 0
export

all clean:
	@$(MAKE) -C ./src	$@

install-lib install-header:
	@$(MAKE) -C ./src	$@

install: install-lib	\
 	 install-header

.PHONY: all		\
 	install		\
	install-lib	\
	install-header	\
	clean
