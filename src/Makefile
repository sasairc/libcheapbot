#
#    Makefile for libcheapbot
#

LIBNAME		= libcheapbot
VERSION		= 0
PATCHLEVEL	= 0
SUBLEVEL	= 1
VERSIONSTR	= $(VERSION).$(PATCHLEVEL).$(SUBLEVEL)

SRCS		= $(wildcard *.c)
OBJS		= $(SRCS:.c=.o)
DEFCFLAGS	= -fPIC
ifeq ($(shell uname), Linux)
LINKERNAME	= $(LIBNAME).so
DEFLDFLAGS	= -shared		\
		  -Wl,-soname=$(LINKERNAME).$(VERSION)
# for Linux
endif
ifeq ($(shell uname), Darwin)
LINKERNAME	= $(LIBNAME).dylib
DEFLDFLAGS 	= -dynamiclib -undefined suppress -flat_namespace 	\
		   -install_name $(LINKERNAME)				\
		   -current_version $(VERSIONSTR)			\
		   -compatibility_version $(VERSION).$(PATCHLEVEL)
# for Darwin
endif
ifndef	LINKERNAME
LINKERNAME	= $(LIBNAME).so
DEFLDFLAGS	= -shared						\
		  -Wl,-soname=$(LINKERNAME).$(VERSION)
# for Other System
endif
# with oniguruma
DEFLDFLAGS	+= -lonig

all: $(LINKERNAME).$(VERSIONSTR)

$(LINKERNAME).$(VERSIONSTR): $(OBJS)
ifeq ($(CMDLINE), 0)
	@echo "  CCLD    $@"
	@$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)
else
	$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)
endif

%.o: %.c
ifeq ($(CMDLINE), 0)
	@echo "  CC      $@"
	@$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)
else
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)
endif

install-lib: $(LINKERNAME).$(VERSIONSTR)
	install -pd $(LIBDIR)
	install -pm 755 $(LINKERNAME).$(VERSIONSTR) $(LIBDIR)/
	ln -sf $(LIBDIR)/$(LINKERNAME).$(VERSIONSTR) $(LIBDIR)/$(LINKERNAME).$(VERSION)
	ln -sf $(LIBDIR)/$(LINKERNAME).$(VERSIONSTR) $(LIBDIR)/$(LINKERNAME)
ifeq ($(shell uname), Darwin)
	install_name_tool -change "$(LINKERNAME)" "$(LIBDIR)/$(LINKERNAME)" $(LIBDIR)/$(LINKERNAME)
# for Darwin
endif

install-header: cheapbot.h
	install -pd $(INCLUDEDIR)
	install -pm 644 $< $(INCLUDEDIR)/

clean:
	-$(RM) -f $(LINKERNAME).$(VERSIONSTR)
	-$(RM) -f $(OBJS)

.PHONY: all		\
	install-lib	\
	install-header	\
	clean
