# Introduction #

BLOPEX has been moved from PETSc to SLEPc. The SLEPc BLOPEX install needs to be tested.

Currently, the development version of SLEPc and development or 3.2 versions of PETSc need to be used to test the latest BLOPEX in SLEPc. In these instructions, we use the development versions of both SLEPc and PETSc

It is easiest to test the development versions, if source control packages "MERCURIAL" and "SVN" have been installed on the test system. In my instructions below, I assume that they are.

# Installing the development versions of PETSc (now without BLOPEX) #

Follow http://www.mcs.anl.gov/petsc/petsc-as/developers/index.html#Obtaining

One must set up PETSC\_DIR, e.g., in my case
```
export PETSC_DIR=/home/aknyazev/work/petsc/petsc-dev
```

Once Mercurial is installed, obtain petsc-dev with the following:
```
hg clone http://petsc.cs.iit.edu/petsc/petsc-dev
cd  $PETSC_DIR
hg clone http://petsc.cs.iit.edu/petsc/BuildSystem config/BuildSystem
```

This needs to be done only once. After this initial setup, all changes in potsc-dev source can be obtained by
```
cd  $PETSC_DIR
hg pull -u
cd config/BuildSystem
hg pull -u
```

During the PETSc installation, you must have set up both PETSC\_DIR and PETSC\_ARCH enviroment variables. PETSC\_ARCH must be used to compile PETSc with different configure options uising a single source directory.  PETSC\_DIR and PETSC\_ARCH are also needed in the next step, the SLEPc installation.

To double-check your environment variables, run
```
env | grep PETSC
env | grep SLEPC
```

To test different installation options on only one source directory pets-dev, open multiple terminal windows and run in separately in an individual window one of the following:

```
cd $PETSC_DIR 
export PETSC_ARCH=arch-linux2-c-debug-real-int32-float64
./config/configure.py --download-hypre=1
```
(hypre is not available in 64-bit or complex)
or
```
cd $PETSC_DIR
export PETSC_ARCH=arch-linux2-c-debug-real-int64-float64
./config/configure.py --with-64-bit-indices
```
or
```
cd $PETSC_DIR 
export PETSC_ARCH=arch-linux2-c-debug-complex-int32-float64
./config/configure.py --with-scalar-type=complex
```
or
```
cd $PETSC_DIR
export PETSC_ARCH=arch-linux2-c-debug-complex-int64-float64
./config/configure.py --with-scalar-type=complex --with-64-bit-indices
```

Note that there is also a PETSc option `--with-precision=single`, which is supported by SLEPc, but but yet by BLOPEX.

For configure, add the other usual options that need to be tested. DO NOT attempt to install BLOPEX within PETSc (BLOPEX has already been removed from the latest PETSc version). After configure, compile as usual.

**It is NOT safe to run PETSc configure and make in different PETSC\_ARCH's in separate terminal windows at the same time! Do it one-by-one.**

# Installing the development versions of SLEPc with BLOPEX #

According to http://www.grycap.upv.es/slepc/download/download.htm, run
```
svn checkout http://www.grycap.upv.es/slepc/svn/trunk slepc-dev
```
e.g., in the same directory where the newly created petsc-dev directory is located. At a later time, the repository can be refreshed simply by:
```
svn update
```

As suggested at http://www.grycap.upv.es/slepc/documentation/instal.htm execute
```
cd slepc-dev/
export SLEPC_DIR=$PWD
```

In my case, I simply use
```
export SLEPC_DIR=/home/aknyazev/work/petsc/slepc-dev
```

Then configure and make SLEPc with BLOPEX:
```
cd $SLEPC_DIR
./configure --download-blopex 
make
make test
make alltest
```

**It is NOT safe to run SLEPc configure and make in different PETSC\_ARCH's in separate terminal windows at the same time! Do it one-by-one.**

The SLEPc configure reads PETSc configure parameters using PETSC\_DIR, so PETSc configure must be executed first, as in the previous step.

SLEPCs's `make test ` and `make alltest` pick up from the SLEPSc configure if `--download-blopex` has been used, and if so, run BLOPEX tests. If everything runs correctly, `make test ` should produce no errors, while `make alltest` should not produce any output at all, ideally.


# Installation and Compiling shell script example #

```
 #!/bin/bash  
export PETSC_DIR=/home/aknyazev/work/petsc/petsc-dev
export SLEPC_DIR=/home/aknyazev/work/petsc/slepc-dev
cd  $PETSC_DIR
hg pull -u
cd config/BuildSystem
hg pull -u
cd $SLEPC_DIR
svn update
cd  $PETSC_DIR
export PETSC_ARCH=arch-linux2-c-debug-real-int32-float64
./config/configure.py --download-hypre=1
make
make test
cd $SLEPC_DIR
./configure --download-blopex 
make
make test
make alltest
```


# Running SLEPc examples with BLOPEX #

BLOPEX-compatible SLEPc examples are listed at
http://www.grycap.upv.es/slepc/documentation/current/src/examples/index.html
and are located in
```
cd $SLEPC_DIR/src/eps/examples/tutorials
```
BLOPEX-compatible SLEPc examples are 1-4, 7, 11, and 19. To use BLOPEX in any BLOPEX-compatible SLEPc example, simply add "-eps\_type blopex" to the command line. In order to call hypre preconditioning, add also -"st\_pc\_type hypre -st\_pc\_hypre\_type boomeramg" assuming that PETSc has been configured with hypre.

## ex4 - reading a PETSc matrix from a file ##



## ex7 - generalized, reading PETSc matrices from files ##

```
./ex7 -help
```
shows the example-specific command line options:

```
Solves a generalized eigensystem Ax=kBx with matrices loaded from a file.
This example works for both real and complex numbers.

The command line options are:
  -f1 <filename>, where <filename> = matrix (A) file in PETSc binary form.
  -f2 <filename>, where <filename> = matrix (B) file in PETSc binary form.
  -ninitial <nini>, number of user-provided initial guesses.
  -finitial <filename>, where <filename> contains <nini> vectors (binary).
  -nconstr <ncon>, number of user-provided constraints.
  -fconstr <filename>, where <filename> contains <ncon> vectors (binary).
```

In order to shift the pencil {A,B} into {A+alpha B,B} the convention in SLEPc is to do A-sigma B, so you need to use a negative value of sigma. This is done for instance (to set alpha=1.0):
```
./ex7 -f1 fileA -f2 file B -eps_gen_hermitian -eps_type blopex -st_shift -1.0
```

A comprehensive collection of tests matrices is at
$PETSC\_DIR/share/petsc/datafiles/matrices/

## ex19 - 3D Laplacian ##
The option "-st\_ksp\_type preonly" is the default in the following:

```
./ex19 -da_grid_x 100 -da_grid_y 100 -da_grid_z 100 -eps_type blopex -eps_nev 2 -eps_tol 1e-11 -eps_max_it 10 -st_pc_type hypre -st_pc_hypre_type boomeramg  -showtimes -eps_view
```

For large problems, the code may run out of memory and crash. In this case, add the following extra options
```
-st_pc_hypre_boomeramg_agg_nl 1 -st_pc_hypre_boomeramg_P_max 4 -st_pc_hypre_boomeramg_interp_type standard 
```

so that you run

```
./ex19 -eps_type blopex -st_pc_type hypre -st_pc_hypre_type boomeramg -st_pc_hypre_boomeramg_agg_nl 1 -st_pc_hypre_boomeramg_P_max 4 -st_pc_hypre_boomeramg_interp_type standard -showtimes -eps_view
```


In the following example, the preconditioner for the BLOPEX eigensolver is the PCG iterative method, preconditioned with hypre's boomeramg:

```
./ex19 -eps_type blopex -st_ksp_type cg -st_pc_type hypre -st_pc_hypre_type boomeramg -st_pc_hypre_boomeramg_agg_nl 1 -st_pc_hypre_boomeramg_P_max 4 -st_pc_hypre_boomeramg_interp_type standard -showtimes -eps_view
```

For a high quality preconditioner, such as boomeramg in this case, using PCG on the top of preconditioning is an overkill - the increase in complexity needed to setup the PCG solver does not lead to enough convergence acceleration. This is reversed if the preconditioner is cheap and poor. The direct application of Jacobi below makes lobpcg to converge in ~350 iterations:

```
 ./ex19 -da_grid_x 100 -da_grid_y 100 -da_grid_z 100 -eps_type blopex -eps_max_it 1000 -st_pc_type jacobi  -showtimes -eps_view
```

while the use of Jacobi-preconditioned PCG with 7 inner iterations makes lobpcg to converge in ~50 iterations:

```
./ex19 -da_grid_x 100 -da_grid_y 100 -da_grid_z 100 -eps_type blopex -st_ksp_type cg -st_pc_type jacobi -st_ksp_max_it 7  -showtimes -eps_view
```

The total number of inner-outer iterations is the same ~350 in both runs, but the latter is almost 3 times faster in CPU time, since a single PCG iteration is cheaper than a single LOBPCG iteration.

PETSc command-line options supported, e.g., "-log\_summary"

You can compile and run multiple examples and redirect the screen output to a text file run running a script like the following:

```
 #!/bin/bash
cd $SLEPC_DIR/src/eps/examples/tutorials
mkdir $PETSC_ARCH
make ex7
mv ex7 $PETSC_ARCH
make ex19
mv ex19 $PETSC_ARCH
cd $PETSC_ARCH
./ex7 -f1 $PETSC_DIR/share/petsc/datafiles/matrices/spd-real-int32-float64 -eps_gen_hermitian -eps_type blopex -st_pc_type hypre -st_pc_hypre_type boomeramg > output.txt
./ex19 -eps_type blopex -st_pc_type hypre -st_pc_hypre_type boomeramg >> output.txt
```