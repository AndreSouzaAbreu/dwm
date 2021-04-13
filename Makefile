include config.mk
SRC=drw.c dwm.c util.c
OBJ=${SRC:.c=.o}

all: options install clean

options:
	@echo dwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

dwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f dwm ${OBJ} dwm-${VERSION}.tar.gz *.orig *.rej

dist: clean
	mkdir -p dwm-${VERSION}
	cp -R LICENSE Makefile README config.mk\
		dwm.1 drw.h util.h ${SRC} transient.c dwm-${VERSION}
	tar -cf dwm-${VERSION}.tar dwm-${VERSION}
	gzip dwm-${VERSION}.tar
	rm -rf dwm-${VERSION}

install: dwm
	mkdir -p ${DESTDIR}
	cp -f dwm ${DESTDIR}
	chmod 755 ${DESTDIR}/dwm

uninstall:
	rm -f ${DESTDIR}/dwm

.PHONY: all options clean dist install uninstall
