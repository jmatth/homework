CC = gcc 
CFLAGS = -g 

OBJs = parse.tab.o symtab.o attr.o instrutil.o lex.yy.o 

default: codegen 

codegen: ${OBJs}
	${CC} ${CFLAGS} ${OBJs} -o codegen 

lex.yy.c: scan.l parse.tab.h attr.h
	flex -i scan.l

parse.tab.c: parse.y attr.h symtab.h instrutil.h
	bison -dv parse.y

parse.tab.h: parse.tab.c

clean:
	rm -f codegen lex.yy.c *.o parse.tab.[ch] parse.output iloc.out

depend:
	makedepend -I. *.c

# DO NOT DELETE THIS LINE -- make depend depends on it.

attr.o: attr.h
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
lex.yy.o: /usr/include/stdio.h /usr/include/features.h
lex.yy.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
lex.yy.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
lex.yy.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
lex.yy.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
lex.yy.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
lex.yy.o: /usr/include/string.h /usr/include/xlocale.h /usr/include/errno.h
lex.yy.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
lex.yy.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
lex.yy.o: /usr/include/asm-generic/errno-base.h /usr/include/stdlib.h
lex.yy.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
lex.yy.o: /usr/include/endian.h /usr/include/bits/endian.h
lex.yy.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
lex.yy.o: /usr/include/time.h /usr/include/sys/select.h
lex.yy.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
lex.yy.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
lex.yy.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h attr.h
lex.yy.o: parse.tab.h /usr/include/unistd.h /usr/include/bits/posix_opt.h
lex.yy.o: /usr/include/bits/environments.h /usr/include/bits/confname.h
lex.yy.o: /usr/include/getopt.h
parse.tab.o: /usr/include/stdio.h /usr/include/features.h
parse.tab.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
parse.tab.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
parse.tab.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
parse.tab.o: /usr/include/libio.h /usr/include/_G_config.h
parse.tab.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
parse.tab.o: /usr/include/bits/sys_errlist.h attr.h instrutil.h symtab.h
parse.tab.o: /usr/include/string.h /usr/include/xlocale.h
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
symtab.o: symtab.h
