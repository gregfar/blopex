

# Introduction #

Trials and Tribulations of testing blopex\_serial\_complex with acml library.
This was done on math.cudenver.edu on 5/6/2009.

This is included as an example of the kinds of errors that can occur and how to resolve them.  In particular the location of and modifications for the acml libraries.  If you are only concerned about the end result just review the summary section.  If you encounter problem review the section `How you get to the summary`.

# Summary #

Change directories to the blopex\_serial\_complex directory.

Replace LAPACKLIB in Makefile.inc
```
[dmccuan@math blopex_serial_complex]$ cat Makefile.inc
LOBPCG_ROOT_DIR =  ../../blopex_abstract
CFLAGS = -Wall -g
#LAPACKLIB = -llapack
LAPACKLIB = -L/opt/acml3.1.0/gnu64/lib -lacml -lacml_mv -lm  -L/fc6/usr/lib/gcc/x86_64-redhat-linux/3.4.6 -lg2c
```

Set LD\_LIBRARY\_PATH
```
> export LD_LIBRARY_PATH=/opt/acml3.1.0/gnu64/lib:/fc6/usr/lib64
```

Execute the test.
```
> cd driver
> ./serial_driver L test2a.txt test2x.txt
```

# Testing Results #

Eigenvalues Computes
```
Eigenvalue lambda       Residual
-1.1080244951277193e+01  8.7715925997380589e-07
-1.0619031628691275e+01  8.3381011189059162e-07
-1.0091481158767698e+01  9.3748814865258268e-07
-9.9117793865262627e+00  8.9289211201220377e-07
-9.7514313021142627e+00  9.7493815218842427e-07
-9.3207970589077078e+00  9.8447435796019496e-07
-8.8124520006214393e+00  6.4165543396311134e-07
-8.5728211942248826e+00  7.6010710998841121e-07
-8.3669043128529790e+00  9.8321054529518632e-07
-7.9116588717464165e+00  7.6052601198572822e-07

76 iterations
CPU time used = 4.6999999999999997e-01
```


# How you get to the summary #

First find the acml library:
```
> cd ../blopex_serial_complex
> locate libacml.so
....
/opt/acml3.1.0/gnu64/lib/libacml.so
....
```
Replace LAPACKLIB in Makefile.inc
```
[dmccuan@math blopex_serial_complex]$ cat Makefile.inc
LOBPCG_ROOT_DIR =  ../../blopex_abstract
CFLAGS = -Wall -g
#LAPACKLIB = -llapack
LAPACKLIB = -L/opt/acml3.1.0/gnu64/lib -lacml
```
Make gets error:
```
lobpcg.c:(.text+0x660): undefined reference to `sqrt'
../../blopex_abstract/lib/libBLOPEX.a(fortran_matrix.o): In function `utilities_FortranMatrixFNorm':
```

I saw this with Bluefire testing using IBM routines.
The -lm to pickup C math routines is explicitly needed.
This is not true when using lapack library.
I assume they are included as part of the lapack libs.

Change LAPACKLIB to:
```
LAPACKLIB = -L/opt/acml3.1.0/gnu64/lib -lacml -lm
```
Make gets error:
```
/usr/bin/ld: warning: libacml_mv.so, needed by /opt/acml3.1.0/gnu64/lib/libacml.so, not found (try using -rpath or -rpath-link)
```

Note: libacml\_mv.so is in same directory as libacml.so

Change LAPACKLIB to:
```
LAPACKLIB = -L/opt/acml3.1.0/gnu64/lib -lacml -lacml_mv -lm
```
Make gets error:
```
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `e_wsfi'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `s_stop'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `s_wsfe'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `e_rsfi'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `s_cmp'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `e_wsfe'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `z_abs'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `do_lio'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `s_wsle'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `z_sqrt'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `s_cat'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `c_sqrt'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `s_copy'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `s_rsfi'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `do_fio'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `z_exp'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `c_abs'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `e_wsle'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `c_exp'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `s_wsfi'
/opt/acml3.1.0/gnu64/lib/libacml.so: undefined reference to `i_indx'
```
Had to look for this error on the internet. Found:
```
Linking your program to the SHTOOLS, LAPACK, BLAS, and FFTW libraries might result in link errors similar to:

/usr/lib///liblapack.so: undefined reference to `e_wsfe'
/usr/lib///liblapack.so: undefined reference to `z_abs'
/usr/lib///liblapack.so: undefined reference to `c_sqrt'
/usr/lib///liblapack.so: undefined reference to `s_cmp'
/usr/lib///liblapack.so: undefined reference to `z_exp'
/usr/lib///liblapack.so: undefined reference to `c_exp'
/usr/lib///liblapack.so: undefined reference to `do_fio'
/usr/lib///liblapack.so: undefined reference to `z_sqrt'
/usr/lib///liblapack.so: undefined reference to `s_cat'
/usr/lib///liblapack.so: undefined reference to `s_stop'
/usr/lib///liblapack.so: undefined reference to `c_abs'
/usr/lib///liblapack.so: undefined reference to `s_wsfe'
/usr/lib///liblapack.so: undefined reference to `s_copy'

This can arise when the offending libraries were built using the g77 compiler. In order to rectify this, it should only be necessary to link to the additional library libg2c.a (i.e., g77 to c). Assuming that this library can be found by the linker, just append -lg2c to the list of libraries passed to the linker. If the g2c libary can not be found by the linker, an easy way to find its location is by using either the "locate" or "find" shell commands

locate libg2c.a
find /usr -name libg2c.a

where the find command searches the directory /usr. This pathname can then be added to the linker's search path by using the option -Ldirname, by, for example,

-L/usr/lib/gcc/i586-mandrake-linux-gnu/3.4.3 -lg2c
```

_Note: I would have though that supplying location of libg2c.so would be sufficient, but this is not true.  For some reason it wants libg2c.a, although later during execution we will have to supply location of libg2c.so._

Locate libg2c.a in /fc6/usr/lib/gcc/x86\_64-redhat-linux/3.4.6

Change LAPACKLIB to:
```
LAPACKLIB = -L/opt/acml3.1.0/gnu64/lib -lacml -lacml_mv -lm  -L/fc6/usr/lib/gcc/x86_64-redhat-linux/3.4.6 -lg2c
```
Make is finally successful.

Execute the test.
```
>cd driver
>./serial_driver L test2a.txt test2x.txt
```
Get error:
```
./serial_driver: error while loading shared libraries: libacml.so: cannot open shared object file: No such file or directory
```
Locate libacml.so
```
> locate libacml.so
...
/opt/acml3.1.0/gnu64/lib/libacml.so
...
```
Set LD\_LIBRARY\_PATH
```
>export LD_LIBRARY_PATH=/opt/acml3.1.0/gnu64/lib
```
Execute the test again.

Get error:
```
./serial_driver: error while loading shared libraries: libg2c.so.0: cannot open shared object file: No such file or directory
```
Locate libacml.so
```
> locate libg2c.so.0
...
/fc6/usr/lib/libg2c.so.0
...
```
Set LD\_LIBRARY\_PATH
```
> export LD_LIBRARY_PATH=/opt/acml3.1.0/gnu64/lib:/fc6/usr/lib64
```
Execute the test again.

Now have success:
```
.....
Eigenvalue lambda       Residual
-1.1080244951277193e+01  8.7715925997380589e-07
-1.0619031628691275e+01  8.3381011189059162e-07
-1.0091481158767698e+01  9.3748814865258268e-07
-9.9117793865262627e+00  8.9289211201220377e-07
-9.7514313021142627e+00  9.7493815218842427e-07
-9.3207970589077078e+00  9.8447435796019496e-07
-8.8124520006214393e+00  6.4165543396311134e-07
-8.5728211942248826e+00  7.6010710998841121e-07
-8.3669043128529790e+00  9.8321054529518632e-07
-7.9116588717464165e+00  7.6052601198572822e-07

76 iterations
CPU time used = 4.6999999999999997e-01
```