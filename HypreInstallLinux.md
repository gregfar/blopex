# Introduction #

Installing and running HYPRE 2.6.0b with our modifications on linux.

Tested on:
  * AK's Fedora 12 64bit, Two AMD Opteron CPUs, 16GB RAM.
  * PZ's Intel(R) Pentium(R) ,4 CPU, 3.06GHz, 3.24 GB, IntelMKL 10.1.1.019
  * xvia  Fedora 10 OS, 4 Quad Core Opteron 2.0 Ghz CPUs, 64 GB RAM mpich2, ACML 4.4.0.
  * game1 Fedora 12 64bit, Quad Core Intel i7 965, 12GB RAM

THIS INFO NEEDS TO BE UPDATED.

# Details #

## Preliminaries ##

First download the compressed hypre file:
You can download from the official HYPRE web site, or you can download directly version 2.6.0b with the following command:

```
$ wget https://computation.llnl.gov/casc/hypre/download/hypre-2.6.0b.tar.gz
```

Unzip the compressed file:

```
$ tar xzvf hypre-2.6.0b.tar.gz 
```

You can rename the HYPRE directory at this point to reflect the expected configuration, e.g.,

```
$ mv hypre-2.6.0b hypre-2.6.0b_openmpi
```

Go to the new HYPRE directory source:

```
$ cd hypre-2.6.0b_openmpi\src
```

and download our modifications

```
$ wget http://blopex.googlecode.com/files/hypre_lobpcg_modifications.tar.gz
```

You should now be in hypre-2.6.0b\_openmpi/src - double-check with pwd.
Uncompress:

```
$ tar xzvf hypre_lobpcg_modifications.tar.gz
```

The above applies our modification to hypre 2.6.0b

## Configuring and compiling ##

Hypre configure is not too smart and relies on a user to tell where all the compilers are and what flags to use. This is easy if you plan to use mpiich2 or openmpi, see
http://code.google.com/p/blopex/wiki/openmpiandmpich2switchinFedora12

If you do not do anything and the mpich2 RPM is installed (check with rpm -q mpich2) it is the default (check with which mpirun)

Suppose that you use openmpi on 64bit, make sure to load its environment by

```
$ module load openmpi-x86_64
```

Type

```
$ env | grep MPI
$ which mpicc 
```

to double-check which C compiler is being used. For openmpi, the latter should point to
/usr/lib64/openmpi/bin/mpicc

At this point, you also need to decide on which BLAS/LAPACK library
you want to use. If you just run

```
$ ./configure
```

it would use the source of all related BLAS/LAPACK functions provided within hypre
and compile this source during the next step, make.

If you want to use some precompiled BLAS/LAPACK libraries, you need to tell
hypre configure where they are located, e.g.,

```
$ ./configure --with-lapack-libs="acml acml_mv m" --with-lapack-lib-dirs="/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib"  
```

will use acml-4-4-0-gfortran-64bit BLAS/LAPACK functions from the given location, i.e., opt/acml-4-4-0-gfortran-64bit/gfortran64/lib. Openmpi uses GNU compilers and hypre 1.6.0b does not support long integers, which determines our choice of the ACML library from many available, cf. http://code.google.com/p/blopex/wiki/PetscInstallLinuxACML .

For Intel 64 bit CPUs,

```
$ ./configure  --with-lapack-libs="mkl  mkl_lapack guide pthread" --with-lapack-lib-dirs="/opt/intel/mkl/10.1.1.019/lib/em64t" --with-blas-libs="mkl  mkl_em64t  guide pthread m" --with-blas-lib-dirs="/opt/intel/mkl/10.1.1.019/lib/em64" 
```

will use 64bit Intel's MKL rev. 10.1.1.019 library for BLAS/LAPACK.

If you want to use precompiled BLAS/LAPACK libraries from the distribution provider, in Fedora, check if you have them installed and determine their locations by

```
$ rpm -ql blas   | grep lib
$ rpm -ql lapack | grep lib
```

In Fedora 12 64-bit, the location is /usr/lib64/ , so you run

```
$ ./configure --with-lapack-libs="lapack blas" --with-lapack-lib-dirs="/usr/lib64"  
```

Now, the following makes HYPRE

```
$ make 
```

There MUST NOT BE ANY ERRORS. If you do get errors, there is something wrong with your setup of compilers. This setup is controlled by relevant parameters in env, as well as configure options. To see the latter, run

```
$ ./configure --help 
```

Having no errors, try to be adventurous and run

```
$ make test
```

which complies and links all the drivers in the test directory. If you get errors at this point, you have problems with linking to the external headers and/or the libraries. These are, again, set up by relevant parameters in env, as well as configure options.

## Running ##

There are several BLOPEX-related drivers the test directory, e.g., the simplest tests to run are:

```
$ ./ij -lobpcg
$ ./struct -lobpcg
```

and, the same, using mpi:

```
$ mpirun -np 2 ./ij -lobpcg
$ mpirun -np 2 ./struct -lobpcg
```

If you get errors here like "error while loading shared libraries: libacml.so: cannot open shared object file", this means that the executable has been compiled using shared libraries, so you need to provide the location of the shared libraries in the execution line using LD\_LIBRARY\_PATH, cf. http://code.google.com/p/blopex/wiki/PetscInstallLinuxACML , e.g., for OPENMPI,

```
$ mpirun -np 2 -x LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib ./ij -lobpcg
$ mpirun -np 2 -x LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib ./struct -lobpcg
```

or, for MPICH2,
```
$ mpirun -np 2 -env LD_LIBRARY_PATH /usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib ./ij -lobpcg
$ mpirun -np 2 -env LD_LIBRARY_PATH /usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib ./struct -lobpcg
```

Alternatively, set the LD\_LIBRARY\_PATH in your environment by (bash shell), e.g.,

```
$ export LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib
```

for AMD's ACML, or

```
$ export LD_LIBRARY_PATH=/opt/intel/mkl/10.1.1.019/lib/em64t
```

for Intel's MKL, then you do not need to specify "-x LD\_LIBRARY\_PATH=..." in the
execution command line.

In addition to BLOPEX-related drivers the test directory, there is
ex11 driver in the examples directory. To make the executable ex11
run in the examples directory

```
$ make ex11
```

It may give errors even though there have been no errors making the test drivers. E.g., "could not find lg2c."

This is because the examples/Makefile ignores the configure parameters and variables in env, but rather uses hard-coded values. So you have to edit the examples/Makefile by hand.
E.g., to remove the "could not find lg2c" error, you can try the following.
On line 27 of the Makefile you see a reference to lg2c. Replace the word `-lg2c` with `-lgfortran`. If you have test drivers made without errors, try to reproduce the parameters used there for the ex11 Makefile.

Having no errors, run the ex11 executable in the usual way:

```
$ ./ex11
$ mpirun -np 2 ./ex11
```

or, if shared libraries have been used, something like

```
$ ./ex11 -x LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib
$ mpirun -np 2 -x LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib ./ex11
```

to see the familiar output coming from the LOBPCG function.

For details on hypre Linux testing see http://code.google.com/p/blopex/wiki/HypreTestingLinux

## Complete examples ##

### ACML 4.4.0 and the default mpich2 ###

```
$ tar xzvf hypre-2.6.0b.tar.gz
$ mv hypre-2.6.0b hypre1
$ cd hypre1/src
$ ./configure --with-lapack-libs="acml acml_mv m" --with-lapack-lib-dirs="/tmp/bryan/acml4.4.0/gfortran64/lib"
$ make all
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib
$ make test
$ cd test
$ mpirun -np 16 ./ij -lobpcg

...

Eigenvalue lambda   2.43042158313016e-01
Residual   3.93340679298185e-09

11 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 0.030000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 0.040000 seconds
  cpu MFLOPS      = 0.000000

$
```

HOW DOES THE ABOVE USE hypre\_lobpcg\_modifications.tar.gz ?


### Precompiled BLAS/LAPACK libraries from Fedora ###

```
$ tar xzvf hypre-2.6.0b.tar.gz
$ mv hypre-2.6.0b hypre8
$ cd hypre8/src
$ wget http://blopex.googlecode.com/files/hypre_lobpcg_modifications.tar.gz
$ tar xzvf hypre_lobpcg_modifications.tar.gz
$ ./configure --with-lapack-libs="lapack blas" --with-lapack-lib-dirs="/usr/lib64"
$ make all
$ make test
$ cd test
$ mpirun -np 16 ./ij -lobpcg

...

Eigenvalue lambda       Residual              
  2.43042158313016e-01    2.74167648768121e-09

12 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 0.020000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 0.030000 seconds
  cpu MFLOPS      = 0.000000

$
```

### ACML gfortran64\_mp libraries ###

Configure:
```
$ ./configure --with-lapack-libs="acml_mp acml_mv m" --with-lapack-lib-dirs="/tmp/bryan/acml4.4.0/gfortran64_mp/lib"
```

Run:
```
$ export LD_LIBRARY_PATH=/usr/lib64/mpich2/lib:/tmp/bryan/acml4.4.0/gfortran64_mp/lib
$ ./ij -lobpcg
$ ./ij -lobpcg -n 64 64 64 -pcgitr 0 -vrand 20 -seed 1
```

The mp libraries already should use all 16 CPUs. If you in addition run it with mpirun -np 16, you effectively use 16x16=256 CPUs (in virtual mode). That should be roughly similar to running mpirun -np 256 with non-mp libraries, so not advisable.

PLEASE REPORT THE TIMING DIFFERENCE COMPARED TO NON-MP!

**Note:** This install is running on only one processor. Will report times when we figure out how to get it to run on all processors. (It is only running on one processor with PETSc as well).


# Still needs checking #

LOBPCG is running fine, but I (BS) am still getting some errors in some of the test examples. For example, compiling ex5.c, I am getting an error that says:

```
../hypre/lib/libHYPRE.a(par_gsmg.o): In function `hypre_BoomerAMGFitVectors':
par_gsmg.c:(.text+0x1a40): undefined reference to `dgels_'
```