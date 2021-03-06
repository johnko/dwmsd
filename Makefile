# dwmsd - dynamic window manager status daemon

CONFIGMK = nonexistant.mk

ifeq ($(OS),Windows_NT)
    #CCFLAGS += -D WIN32
    ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        #CCFLAGS += -D AMD64
    endif
    ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        #CCFLAGS += -D IA32
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        #CCFLAGS += -D LINUX
    endif
    ifeq ($(UNAME_S),FreeBSD)
        MAKE = gmake
        CONFIGMK = config.mk.freebsd
    endif
    ifeq ($(UNAME_S),Darwin)
        CONFIGMK = config.mk.darwin
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        #CCFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        #CCFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        #CCFLAGS += -D ARM
    endif
endif

include ${CONFIGMK}

SRC = dwmsd.c dwmsc.c
OBJ = ${SRC:.c=.o}

all: options dwmsd dwmsc

options:
	@echo dwmsd build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC -c $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: ${CONFIGMK}

dwmsd: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ dwmsd.o ${LDFLAGS}

dwmsc: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ dwmsc.o ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f dwmsd dwmsc ${OBJ} dwmsd-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p dwmsd-${VERSION}
	@cp -R Makefile ${CONFIGMK} \
		${SRC} dwmsd-${VERSION}
	@tar -cf dwmsd-${VERSION}.tar dwmsd-${VERSION}
	@gzip dwmsd-${VERSION}.tar
	@rm -rf dwmsd-${VERSION}

install: all
	@echo installing executables to ${DESTDIR}${PREFIX}/bin
	@install -d -m 0755 ${DESTDIR}${PREFIX}/bin
	@install -m 0755 dwmsd      ${DESTDIR}${PREFIX}/bin/
	@install -m 0755 dwmsc      ${DESTDIR}${PREFIX}/bin/

uninstall:
	@echo removing executables from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/dwmsd
	@rm -f ${DESTDIR}${PREFIX}/bin/dwmsc

.PHONY: all options clean dist install uninstall
