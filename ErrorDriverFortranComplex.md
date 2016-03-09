# Introduction #

Donald has made a possible fix,
http://blopex.googlecode.com/svn/trunk/blopex_petsc/driver_fortran/blopex_c.c
It has been submitted to PETSc and will appear in petsc-3.1-p4.

# Fix for C++ Issues #

I have encountered two errors connected to C++.

The first is type conversion errors in assigning the void function pointers that appear in the parameters of petsc\_lobpcg\_solve\_c_in blopex\_c.c .  C++ is much more strict about this than C.  I added explict type casting to conversion and this fixed that problem._

The second problem occurs in the link step which gives error "ex2f\_blopex.F:391: undefined reference to `petsc\_lobpcg\_solve\_c_".
Using command "objdump blopex\_c.o -t" I discovered the object name in blopex\_c.o is "_Z21petsc\_lobpcg\_solve\_c\_PP6\_p\_VecPiS2\_S2\_PdS3\_S3\_PFvPvS4\_S4\_ES6\_S6\_PFvS4\_ES8\_S2_" which sure isn't "petsc\_lobpcg\_solve\_c_".  After a lot of time searching the internet I found the following.

C++ generates names in the object files that include codes for parameters.  Fortran does not and C does not.   To compile with C++ and force object names to be fortran compatible we need to surround the C++ function with  extern "C" { .,.....}.  And so we don't mess things up for the normal C compiles we only do this if PETSC\_CLANGUAGE\_CXX is set.

I've checked out in XVIA using g++ and gcc compilers and it works well there.

I have moved the new code for blopex\_c.c in driver\_fortran to google source.
That's the only thing that has changes.

To test:
(1) install PETSc as normal
(2) download blopex\_c.c from the google source code and move to $PETSC\_DIR/src/contrib/blopex/driver\_fortran/.
(3) complete test as normal doing make and execute ex2f\_blopex

# In cygwin #

## Setup Petsc ##

Start the configuration process as follows to configure for complex scalars and 64bit integers.
```
>> ./config/configure.py  --with-cc=gcc --with-fc=gfortran --with-cxx=g++ --with-f-blas-lapack=/cygdrive/c/cygwin/lib --download-mpich=1 --download-blopex=1 --with-scalar-type=complex --with-clanguage=cxx --with-64-bit-indices
```

(AK:
http://code.google.com/p/blopex/wiki/PetscTestingCygwinDouble64
uses
```
>>  ./configure --with-f-blas-lapack=/cygdrive/c/cygwin/lib --with –mpich-dir=/cygdrive/c/cygwin/usr/local/bin/mpich2-install  --download-blopex=1 --with-64-bit-indices
```
and driver\_fortran compiles just fine. Why do you need to specify --with-clanguage=cxx ?
For complex version in cygwin, it always said 'C Compiler provided doest not support C99 complex' without using cxx or mpicxx compilers.


I guess that it forces compilation of C codes using a C++ compiler, and blopex\_c.c
is not written careful enough to handle it.)

One is not supposed to compile blopex\_c.c directly in
driver\_fortran. Only the ex2f\_blopex executable must be complied by
```
>> make ex2f_blopex
```

## making ex2f\_blopex ##
Using the latest blopex\_c.c, ex2f\_blopex works well in Cygwin and 64 bit or 32 bit.
The test results have been posted on the PetscTestingCygwinComplex64 Wiki for 64bit.
For 32bit see PetscTestingCygwinDoubleAndComplex32 Wiki.

## previous error making ex2f\_blopex ##
Using the fixed blopex\_c.c , I still get the errors as following (running make ex2f\_blopex):

(This looks like that error previous error, already removed, using the C++ compiler. I am not sure that this is complex-related. Please compare closely your configure for double (which works) and complex and make sure that the only difference is --with-scalar-type=complex AK)
```
$ make ex2f_blopex
/cygdrive/c/petsc/petsc_blopex_1/petsc-3.1-p3/cygwin-cxx-debug/bin/mpif90 -c  -W
all -Wno-unused-variable -g  -I/cygdrive/c/petsc/petsc_blopex_1/petsc-3.1-p3/cyg
win-cxx-debug/include -I/cygdrive/c/petsc/petsc_blopex_1/petsc-3.1-p3/include -I
/cygdrive/c/petsc/petsc_blopex_1/petsc-3.1-p3/cygwin-cxx-debug/include   -I/cygd
rive/c/petsc/petsc_blopex_1/petsc-3.1-p3/cygwin-cxx-debug/include -I/cygdrive/c/
petsc/petsc_blopex_1/petsc-3.1-p3/cygwin-cxx-debug/include    -o ex2f_blopex.o e
x2f_blopex.F
/cygdrive/c/petsc/petsc_blopex_1/petsc-3.1-p3/cygwin-cxx-debug/bin/mpicxx -o blo
pex_c.o -c -Wall -Wwrite-strings -Wno-strict-aliasing -g -I/cygdrive/c/petsc/pet
sc_blopex_1/petsc-3.1-p3/cygwin-cxx-debug/include -I/cygdrive/c/petsc/petsc_blop
ex_1/petsc-3.1-p3/cygwin-cxx-debug/include -I/cygdrive/c/petsc/petsc_blopex_1/pe
tsc-3.1-p3/include -I/cygdrive/c/petsc/petsc_blopex_1/petsc-3.1-p3/cygwin-cxx-de
bug/include -D__INSDIR__=src/contrib/blopex/driver_fortran/ blopex_c.c
blopex_c.c: In function `void petsc_lobpcg_solve_c_(_p_Vec**, int*, int*, int*,
double*, double*, double*, void*, void*, void*, void*, void*, int*)':
blopex_c.c:169: error: invalid conversion from `void*' to `void (*)(void*, void*
, void*)'
blopex_c.c:170: error: invalid conversion from `void*' to `void (*)(void*, void*
, void*)'
blopex_c.c:171: error: invalid conversion from `void*' to `void (*)(void*, void*
, void*)'
blopex_c.c:172: error: invalid conversion from `void*' to `void (*)(void*)'
blopex_c.c:173: error: invalid conversion from `void*' to `void (*)(void*)'
blopex_c.c:287: warning: cannot pass objects of non-POD type `struct std::comple
x<double>' through `...'; call will abort at runtime
blopex_c.c:156: warning: unused variable `j'
make: [blopex_c.o] Error 1 (ignored)
/cygdrive/c/petsc/petsc_blopex_1/petsc-3.1-p3/cygwin-cxx-debug/bin/mpif90 -Wall
-Wno-unused-variable -g  -o ex2f_blopex ex2f_blopex.o blopex_c.o -Wl,-rpath,/cyg
drive/c/petsc/petsc_blopex_1/petsc-3.1-p3/cygwin-cxx-debug/lib -L/cygdrive/c/pet
sc/petsc_blopex_1/petsc-3.1-p3/cygwin-cxx-debug/lib -lpetsc   -Wl,-rpath,/cygdri
ve/c/petsc/petsc_blopex_1/petsc-3.1-p3/cygwin-cxx-debug/lib -L/cygdrive/c/petsc/
petsc_blopex_1/petsc-3.1-p3/cygwin-cxx-debug/lib -lBLOPEX -llapack -lblas -lrt -
L/cygdrive/c/petsc/petsc_blopex_1/petsc-3.1-p3/cygwin-cxx-debug/lib -L/usr/lib/g
cc/i686-pc-cygwin/4.3.4 -ldl -lpmpich -lmpich -lgcc_s -lgcc_eh -luser32 -ladvapi
32 -lshell32 -lmpichf90 -lgfortran -L/usr/lib/gcc/i686-pc-cygwin -L/usr/i686-pc-
cygwin/bin -lmpichcxx -lstdc++ -lgdi32 -luser32 -ladvapi32 -lkernel32 -lmpichcxx
 -lstdc++ -ldl -lpmpich -lmpich -lgcc_s -lgcc_eh -luser32 -ladvapi32 -lshell32 -
ldl
gfortran: blopex_c.o: No such file or directory
make: [ex2f_blopex] Error 1 (ignored)
/usr/bin/rm -f ex2f_blopex.o

```

# In cygwin 32 bit #

## Setup Petsc ##

Start the configuration process as follows to configure for complex scalars and 32bit integers.
```
>> ./config/configure.py --with-cc=mpicc --with-fc=mpif90 --with-cxx=mpicxx 
--with-f-blas-lapack=/lib --with-mpich-dir=/mpich2-install --download-blopex=1 
--with-scalar-type=complex --with-clanguage=cxx
```

If the c compilers aren't specified in this way and Petsc is configured the same as for the double 32 bit version, only with '--with-scalar-type=complex' as follows:

```
>> ./configure --with-f-blas-lapack=/lib --with –mpich-dir=/mpich2-install --download-blopex=1 --with-scalar-type=complex
```

Or with:

```
>> ./configure --with-f-blas-lapack=/lib --download-mpich=1 --download-blopex=1 --with-scalar-type=complex
```

Then Petsc fails to configure and the following error is output:

```
C Compiler provided doest not support C99 complex
```

## Error of making ex2f\_blopex ##

Once configuration is complete, running make on the fortran driver in the fortran\_driver subdirectory produces the following error:

```
$ make ex2f_blopex
mpif90 -c  -Wall -Wno-unused-variable -g   -I/petsc_complex/petsc-3.1-p3/cygwin-cxx-debug/include -I/petsc_complex/petsc-3.1-p3/inclu
de -I/petsc_complex/petsc-3.1-p3/cygwin-cxx-debug/include -I/mpich2-install/include   -I/petsc_complex/petsc-3.1-p3/cygwin-cxx-debug/
include -I/petsc_complex/petsc-3.1-p3/cygwin-cxx-debug/include -I/mpich2-install/include    -o ex2f_blopex.o ex2f_blopex.F
mpicxx -o blopex_c.o -c -Wall -Wwrite-strings -Wno-strict-aliasing -g -I/petsc_complex/petsc-3.1-p3/cygwin-cxx-debug/include -I/mpich
2-install/include -I/petsc_complex/petsc-3.1-p3/cygwin-cxx-debug/include -I/petsc_complex/petsc-3.1-p3/include -I/petsc_complex/petsc
-3.1-p3/cygwin-cxx-debug/include -I/mpich2-install/include -D__INSDIR__=src/contrib/blopex/driver_fortran/ blopex_c.c
blopex_c.c: In function `void petsc_lobpcg_solve_c_(_p_Vec**, int*, int*, int*, double*, double*, double*, void*, void*, void*, void*
, void*, int*)':
blopex_c.c:166: error: invalid conversion from `void*' to `void (*)(void*, void*, void*)'
blopex_c.c:167: error: invalid conversion from `void*' to `void (*)(void*, void*, void*)'
blopex_c.c:168: error: invalid conversion from `void*' to `void (*)(void*, void*, void*)'
blopex_c.c:169: error: invalid conversion from `void*' to `void (*)(void*)'
blopex_c.c:170: error: invalid conversion from `void*' to `void (*)(void*)'
blopex_c.c:153: warning: unused variable `j'
make: [blopex_c.o] Error 1 (ignored)
mpif90 -Wall -Wno-unused-variable -g   -o ex2f_blopex ex2f_blopex.o blopex_c.o -Wl,-rpath,/petsc_complex/petsc-3.1-p3/cygwin-cxx-debu
g/lib -L/petsc_complex/petsc-3.1-p3/cygwin-cxx-debug/lib -lpetsc   -lX11 -Wl,-rpath,/petsc_complex/petsc-3.1-p3/cygwin-cxx-debug/lib
-L/petsc_complex/petsc-3.1-p3/cygwin-cxx-debug/lib -lBLOPEX -llapack -lblas -lrt -lrt -L/mpich2-install/lib -L/usr/lib/gcc/i686-pc-cy
gwin/4.3.4 -ldl -lpmpich -lmpich -lopa -lpthread -lgcc_s -lgcc_eh -luser32 -ladvapi32 -lshell32 -lmpichf90 -lgfortran -L/usr/lib/gcc/
i686-pc-cygwin -L/usr/i686-pc-cygwin/bin -lmpichcxx -lstdc++ -lgdi32 -luser32 -ladvapi32 -lkernel32 -lmpichcxx -lstdc++ -lrt -ldl -lp
mpich -lmpich -lopa -lpthread -lgcc_s -lgcc_eh -luser32 -ladvapi32 -lshell32 -ldl
gfortran: blopex_c.o: No such file or directory
make: [ex2f_blopex] Error 1 (ignored)
/usr/bin/rm -f ex2f_blopex.o
```

UPDATE: Using the modified blopex\_c.c produces a very similar error to the previous version. The only difference is the line numbers the errors occur on and one new error on line 287:

```
blopex_c.c:169: error: invalid conversion from `void*' to `void (*)(void*, void*, void*)'
blopex_c.c:170: error: invalid conversion from `void*' to `void (*)(void*, void*, void*)'
blopex_c.c:171: error: invalid conversion from `void*' to `void (*)(void*, void*, void*)'
blopex_c.c:172: error: invalid conversion from `void*' to `void (*)(void*)'
blopex_c.c:173: error: invalid conversion from `void*' to `void (*)(void*)'
blopex_c.c:287: warning: cannot pass objects of non-POD type `struct std::complex<double>' through `...'; call will abort at runtime
blopex_c.c:156: warning: unused variable `j'
```

UPDATE 2: The second revision to the blopex\_c.c file has fixed the problem. The fortran driver now compiles and runs in Cygwin 32bit. The test results have been posted on the PetscTestingCygwinDoubleAndComplex32 Wiki.

# In Linux #

## complex scalars and 32bit integers (ACML and OPENMPI) ##

### PETSc configuration ###

Start the configuration process as follows to configure for complex scalars and 32bit integers.
```
>> module load openmpi-x86_64
>> LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib; export LD_LIBRARY_PATH
>> PETSC_DIR=$PWD; export PETSC_DIR
>> ./configure --with-blas-lapack-dir=/opt/acml-4-4-0-gfortran-64bit/gfortran64 --download-blopex=1 --with-scalar-type=complex
```


### Making ex2f\_blopex OK ###

```
>> make ex2f_blopex
mpif90 -c  -Wall -Wno-unused-variable -g   -I/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/linux-gnu-c-debug/include -I/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/include -I/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/linux-gnu-c-debug/include -I/usr/include/openmpi-x86_64 -I/usr/lib64/openmpi/lib   -I/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/linux-gnu-c-debug/include -I/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/linux-gnu-c-debug/include -I/usr/include/openmpi-x86_64 -I/usr/lib64/openmpi/lib    -o ex2f_blopex.o ex2f_blopex.F
mpicc -o blopex_c.o -c -Wall -Wwrite-strings -Wno-strict-aliasing -g3 -I/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/linux-gnu-c-debug/include -I/usr/include/openmpi-x86_64 -I/usr/lib64/openmpi/lib -I/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/linux-gnu-c-debug/include -I/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/include -I/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/linux-gnu-c-debug/include -I/usr/include/openmpi-x86_64 -I/usr/lib64/openmpi/lib -D__INSDIR__=src/contrib/blopex/driver_fortran/ blopex_c.c
blopex_c.c: In function ‘petsc_lobpcg_solve_c_’:
blopex_c.c:156: warning: unused variable ‘j’
mpif90 -Wall -Wno-unused-variable -g   -o ex2f_blopex ex2f_blopex.o blopex_c.o -Wl,-rpath,/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/linux-gnu-c-debug/lib -L/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/linux-gnu-c-debug/lib -lpetsc   -lX11 -Wl,-rpath,/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/linux-gnu-c-debug/lib -L/home/aknyazev/work/petsc/petsc-3.1-p3_ACML_OPENMPI_complex/linux-gnu-c-debug/lib -lBLOPEX -Wl,-rpath,/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib -L/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib -lacml -L/usr/lib64/openmpi/lib -L/usr/lib/gcc/x86_64-redhat-linux/4.4.3 -ldl -lmpi -lopen-rte -lopen-pal -lnsl -lutil -lgcc_s -lpthread -lmpi_f90 -lmpi_f77 -lgfortran -lm -lm -L/usr/libexec/gcc/x86_64-redhat-linux/4.4.3 -L/usr/libexec/gcc/x86_64-redhat-linux -L/usr/lib/gcc/x86_64-redhat-linux -lm -lm -ldl -lmpi -lopen-rte -lopen-pal -lnsl -lutil -lgcc_s -lpthread -ldl 
/bin/rm -f ex2f_blopex.o

```

### Running ex2f\_blopex with fixed blopex\_c.c ###

Somehow, using the normal
```
>> module load openmpi-x86_64 
>> LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib; export LD_LIBRARY_PATH
```
is not enough:
```
>> ./ex2f_blopex 
./ex2f_blopex: error while loading shared libraries: libmpichf90.so.1.2: cannot open shared object file: No such file or directory
```
while using
```
>> LD_LIBRARY_PATH=/usr/lib64/mpich2/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib; export LD_LIBRARY_PATH
```
gives
```
>> ./ex2f_blopex 
./ex2f_blopex: error while loading shared libraries: libmpi.so.0: cannot open shared object file: No such file or directory
```

Only using both openmpi and mpich2 libs (which one should never be doing), actually works:
```
>> LD_LIBRARY_PATH=/usr/lib64/mpich2/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib:/usr/lib64/openmpi/lib; export LD_LIBRARY_PATH
>> ./ex2f_blopex 
BLOPEX complete in  13 iterations
eval   1 is 0.205227064E-01 residual 0.3433E-07
eval   2 is 0.512014707E-01 residual 0.1366E-06
eval   3 is 0.512014707E-01 residual 0.1546E-06
eval   4 is 0.818802350E-01 residual 0.6034E-06
eval   5 is 0.101982840E+00 residual 0.7529E-06
>> mpirun -np 2 ./ex2f_blopex
BLOPEX complete in  13 iterations
eval   1 is 0.205227064E-01 residual 0.2583E-07
eval   2 is 0.512014707E-01 residual 0.2347E-06
eval   3 is 0.512014707E-01 residual 0.2949E-06
eval   4 is 0.818802350E-01 residual 0.5673E-06
eval   5 is 0.101982840E+00 residual 0.4896E-06
```

## complex scalars and 64bit integers (ACML and MPICH2) ##

### PETSc configuration ###

Start the configuration process as follows to configure for complex scalars and 64bit integers.
```
>> module load mpich2-x86_64
>> mpd&
>> LD_LIBRARY_PATH=/usr/lib64/mpich2/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib; export LD_LIBRARY_PATH
>> PETSC_DIR=$PWD; export PETSC_DIR
>> ./configure --with-blas-lapack-dir=/opt/acml-4-4-0-gfortran-64bit/gfortran64 --download-blopex=1 --with-64-bit-indices --with-scalar-type=complex
```


### Making ex2f\_blopex OK ###

```
>> make ex2f_blopex
mpif90 -c  -Wall -Wno-unused-variable -g   -I/home/aknyazev/work/petsc/petsc-3.1/linux-gnu-c-debug/include -I/home/aknyazev/work/petsc/petsc-3.1/include -I/home/aknyazev/work/petsc/petsc-3.1/linux-gnu-c-debug/include -I/usr/include/mpich2-x86_64   -I/home/aknyazev/work/petsc/petsc-3.1/linux-gnu-c-debug/include -I/home/aknyazev/work/petsc/petsc-3.1/linux-gnu-c-debug/include -I/usr/include/mpich2-x86_64    -o ex2f_blopex.o ex2f_blopex.F
mpicc -o blopex_c.o -c -Wall -Wwrite-strings -Wno-strict-aliasing -g3 -I/home/aknyazev/work/petsc/petsc-3.1/linux-gnu-c-debug/include -I/usr/include/mpich2-x86_64 -I/home/aknyazev/work/petsc/petsc-3.1/linux-gnu-c-debug/include -I/home/aknyazev/work/petsc/petsc-3.1/include -I/home/aknyazev/work/petsc/petsc-3.1/linux-gnu-c-debug/include -I/usr/include/mpich2-x86_64 -D__INSDIR__=src/contrib/blopex/driver_fortran/ blopex_c.c
blopex_c.c: In function ‘petsc_lobpcg_solve_c_’:
blopex_c.c:155: warning: unused variable ‘j’
mpif90 -Wall -Wno-unused-variable -g   -o ex2f_blopex ex2f_blopex.o blopex_c.o -Wl,-rpath,/home/aknyazev/work/petsc/petsc-3.1/linux-gnu-c-debug/lib -L/home/aknyazev/work/petsc/petsc-3.1/linux-gnu-c-debug/lib -lpetsc   -Wl,-rpath,/home/aknyazev/work/petsc/petsc-3.1/linux-gnu-c-debug/lib -L/home/aknyazev/work/petsc/petsc-3.1/linux-gnu-c-debug/lib -lBLOPEX -Wl,-rpath,/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib -L/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib -lacml -lm -L/usr/lib64/mpich2/lib -L/usr/lib/gcc/x86_64-redhat-linux/4.4.3 -ldl -lmpich -lopa -lpthread -lrt -lgcc_s -lmpichf90 -lgfortran -lm -L/usr/libexec/gcc/x86_64-redhat-linux/4.4.3 -L/usr/libexec/gcc/x86_64-redhat-linux -L/usr/lib/gcc/x86_64-redhat-linux -lm -ldl -lmpich -lopa -lpthread -lrt -lgcc_s -ldl 
/bin/rm -f ex2f_blopex.o
```

### Running ex2f\_blopex with the fixed blopex\_c.c ###

```
>> 
```