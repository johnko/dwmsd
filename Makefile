# dwmsd - dynamic window manager status daemon

include config.mk

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

${OBJ}: config.mk

${OBJC}: config.mk

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
	@cp -R Makefile config.mk \
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
