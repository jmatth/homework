CC = /usr/bin/gcc
CFLAGS = -g

OBJs = parse.tab.o symtab.o attr.o instrutil.o lex.yy.o deptest.o

default: codegen

codegen: ${OBJs}
	${CC} ${CFLAGS} ${OBJs} -o codegen

lex.yy.c: scan.l parse.tab.h attr.h deptest.h
	flex -i scan.l

parse.tab.c: parse.y attr.h symtab.h instrutil.h deptest.h
	bison -dv parse.y

parse.tab.h: parse.tab.c

clean:
	rm -f codegen lex.yy.c *.o parse.tab.[ch] parse.output iloc.out stats.log

depend:
	makedepend -I. *.c

# DO NOT DELETE THIS LINE -- make depend depends on it.

attr.o: attr.h
deptest.o: deptest.h
instrutil.o: /usr/include/stdio.h /usr/include/features.h
instrutil.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
instrutil.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
instrutil.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
instrutil.o: /usr/include/libio.h /usr/include/_G_config.h
instrutil.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
instrutil.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
instrutil.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
instrutil.o: /usr/include/endian.h /usr/include/bits/endian.h
instrutil.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
instrutil.o: /usr/include/time.h /usr/include/sys/select.h
instrutil.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
instrutil.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
instrutil.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
instrutil.o: instrutil.h
symtab.o: /usr/include/stdio.h /usr/include/features.h
symtab.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
symtab.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
symtab.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
symtab.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
symtab.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
symtab.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
symtab.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
symtab.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
symtab.o: /usr/include/sys/types.h /usr/include/time.h
symtab.o: /usr/include/sys/select.h /usr/include/bits/select.h
symtab.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
symtab.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
symtab.o: /usr/include/alloca.h /usr/include/string.h /usr/include/xlocale.h
symtab.o: symtab.h attr.h
