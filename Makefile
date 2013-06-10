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

SRV = dwmsd.c
OBJ = ${SRV:.c=.o}
SRC = dwmsc.c
OBJC = ${SRC:.c=.o}

all: options dwmsd dwmsc

options:
	@echo dwmsd build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: ${CONFIGMK}
${OBJC}: ${CONFIGMK}

dwmsd: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

dwmsc: ${OBJC}
	@echo CC -o $@
	@${CC} -o $@ ${OBJC} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f dwmsd dwmsc ${OBJ} ${OBJC} dwmsd-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p dwmsd-${VERSION}
	@cp -R Makefile ${CONFIGMK} \
		${SRV} ${SRC} dwmsd-${VERSION}
	@tar -cf dwmsd-${VERSION}.tar dwmsd-${VERSION}
	@gzip dwmsd-${VERSION}.tar
	@rm -rf dwmsd-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f dwmsd ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/dwmsd
	@cp -f dwmsc ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/dwmsc

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/dwmsd
	@rm -f ${DESTDIR}${PREFIX}/bin/dwmsc

.PHONY: all options clean dist install uninstall
