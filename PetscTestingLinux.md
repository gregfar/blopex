

# Introduction #

How to test BLOPEX under version 3.1-p3 or above of Petsc on Linux (including 64bit integers and/or complex).

# Setup Petsc #

Remove all existing versions of petsc and BLOPEX before starting a new configure.
Make sure that you download Petsc 3.1-p3 or above

Download  [ftp://info.mcs.anl.gov/pub/petsc/release-snapshots/petsc-lite-3.1.tar.gz](ftp://info.mcs.anl.gov/pub/petsc/release-snapshots/petsc-lite-3.1.tar.gz), which excludes documentation and is a lot smaller distribution tar.

We assume you are in your home directory, $HOME at this point.

To unpack Petsc files issue command:
```
>> tar -zxvf petsc-lite-3.1.tar.gz
```
This creates a directory, e.g., petsc-3.1-p3.
Rename it, if you plan to test different combinations, e.g., to
petsc-3.1-p3-int64-complex

Change directory to it, e.g.,
```
>> cd petsc-3.1-p3
```

The PETSC\_DIR environment variable must be setup:
```
>> export PETSC_DIR=$PWD
```
Another environment variable, PETSC\_ARCH, is not needed here.

If you test different combinations of configuration at the same time,
do it in separate terminal windows, and setup  PETSC\_DIR in every window separately
as needed for different directories.

This test configures Petsc for both complex and 64bit:
```
>> ./config/configure.py --with-shared --with-debugging=1 --with-scalar-type=complex --download-blopex=1 --with-64-bit-indices
```

In petsc-3.1-p3 and above,
--download-blopex=1 will downloaded BLOPEX from a Petsc internet site, which is version 1.1 of Blopex with complex or 64bit support.

Petsc runs with either double or complex objects but not both.
To test the blopex routines for real (the default) configure Petsc without the
option --with-scalar-type=complex.

After configuration is finished and the environment variables are setup issue the command
```
>> make all test
```

You may need to start an MPI daemon prior to this. E.g., in Fedora 12  and mpich2:
```
>> module load mpich2-x86_64
>> mpd&
```
See http://code.google.com/p/blopex/wiki/openmpiandmpich2switchinFedora12

# Compile blopex\_petsc drivers #

Now create executables for every of 4 BLOPEX drivers in PETSc:
```
>> cd src/contrib/blopex/driver
>> make driver
>> cd ../driver_diag
>> make driver_diag
>> cd ../driver_fiedler
>> make driver_fiedler
>> cd ../driver_fortran
>> make ex2f_blopex
```

# Execution of Tests #

There are 4 test drivers located in subdirectories of blopex\_petsc:
driver,  driver\_diag,  driver\_fiedler and driver\_fortran.

## Driver "driver" ##

driver builds a 7pt laplacian for solution and calls either lobpcg\_solve\_complex
if Petsc is configured for complex (this is controlled by the PETSC\_USE\_COMPLEX preprocessor variable) or lobpcg\_solve\_double if Petsc is configured for real (double).

To execute driver:
```
>> mpirun -np 2 ./driver -n_eigs 3 -itr 20 
or 
>> ./driver -n_eigs 3 -itr 20
```

This gives the following results (actual numbers may vary!):
```
Partitioning: 1 1 1
Preconditioner setup, seconds: 0.000369

Solving standard eigenvalue problem with preconditioning

block size 10

No constraints


Initial Max. Residual   2.41101765193731e+00
Iteration 1     bsize 10        maxres   1.47485155706695e+00
Iteration 2     bsize 10        maxres   5.76499070872537e-01
Iteration 3     bsize 10        maxres   3.06659255730571e-01
Iteration 4     bsize 10        maxres   2.00635934430889e-01
Iteration 5     bsize 10        maxres   6.38690964763992e-02
Iteration 6     bsize 10        maxres   2.04031509081778e-02
Iteration 7     bsize 10        maxres   1.02958167235834e-02
Iteration 8     bsize 9         maxres   3.57149267532509e-03
Iteration 9     bsize 8         maxres   1.13980591096821e-03
Iteration 10    bsize 6         maxres   4.15729900972210e-04
Iteration 11    bsize 6         maxres   1.64977271895793e-04
Iteration 12    bsize 5         maxres   5.68823837427636e-05
Iteration 13    bsize 3         maxres   2.12576562467706e-05
Iteration 14    bsize 3         maxres   7.60612793341467e-06
Iteration 15    bsize 3         maxres   3.27324391450688e-06
Iteration 16    bsize 2         maxres   1.73049728975516e-06
Iteration 17    bsize 1         maxres   9.63758146551376e-07

Eigenvalue lambda       Residual              
  2.43042158313016e-01    7.85460552428888e-08
  4.79521039879645e-01    5.38143072677682e-07
  4.79521039879654e-01    4.18818869908062e-07
  4.79521039879684e-01    4.23232402359408e-07
  7.15999921446316e-01    8.92081907089266e-07
  7.15999921446375e-01    7.47752020201913e-07
  7.15999921446505e-01    8.79103766081576e-07
  8.52306637651430e-01    6.91831240708695e-07
  8.52306637651604e-01    5.47008725220135e-07
  8.52306637652249e-01    9.63758146551376e-07

17 iterations
Solution process, seconds: 2.897029e-01
```

If PETSc was compiled with the --download-hypre=1 option
(this only works without using --with-64-bit-indices or --with-scalar-type=complex
since hypre does not support any of these options), one can run the driver
using hypre preconditioner, e.g.

```
./driver -ksp_type preonly -pc_type hypre -pc_hypre_type boomeramg 

Partitioning: 1 1 1
Preconditioner setup, seconds: 0.006566

Solving standard eigenvalue problem with preconditioning

block size 1

No constraints


Initial Max. Residual   2.39770129154100e+00
Iteration 1 	bsize 1 	maxres   1.24191384284174e+00
Iteration 2 	bsize 1 	maxres   1.44195944071613e-01
Iteration 3 	bsize 1 	maxres   3.16282446481776e-02
Iteration 4 	bsize 1 	maxres   6.92778009970852e-03
Iteration 5 	bsize 1 	maxres   1.09875006356576e-03
Iteration 6 	bsize 1 	maxres   1.17458938039469e-04
Iteration 7 	bsize 1 	maxres   1.06516470504257e-05
Iteration 8 	bsize 1 	maxres   1.34756419128339e-06
Iteration 9 	bsize 1 	maxres   3.10899995292549e-07
Iteration 10 	bsize 1 	maxres   6.06378675481750e-08
Iteration 11 	bsize 1 	maxres   4.27013783896497e-09

Eigenvalue lambda       Residual              
  2.43042158313016e-01    4.27013783896497e-09

11 iterations
Solution process, seconds: 1.088190e-02
```

## Driver "driver\_diag" ##

driver\_diag solves ann eigenvalue problem for a diagonal matrix.

DONALD, PLEASE ADD THE DESCRIPTION HERE!

A typical run example:

```
>> mpirun -np 2 ./driver_diag
Preconditioner setup, seconds: 0.000669

Solving standard eigenvalue problem with preconditioning

block size 1

1 constraint


Initial Max. Residual   2.97427246688596e+01

Eigenvalue lambda       Residual              
  4.89206836167546e+01    2.97427246688596e+01

0 iterations
Solution process, seconds: 2.892017e-03
Final eigenvalues:
4.892068e+01
```

## Driver "driver\_fiedler" ##

driver\_fiedler accepts as input matrices in Petsc format.  These matrices must be setup for either 32bit or 64bit arithematic, ral or complex.  The test matrices are setup for double (real) or complex and for 32bit or 64bit.  The 64bit version have a file name containing "_64"`. All our matrices for testing can be downloaded from
http://code.google.com/p/blopex/downloads/detail?name=blopex_petsc_test.tar.gz_

For example:
```
>> ./driver_fiedler -matrix DL-matrix-complex_64.petsc -n_eigs 3 -itr 200
```

This gives the following results:
```
Preconditioner setup, seconds: 0.002955

Solving standard eigenvalue problem with preconditioning

block size 3

1 constraint


Initial Max. Residual   9.29703885936090e-01
Iteration 1     bsize 3         maxres   8.08364838024446e-03
Iteration 2     bsize 3         maxres   3.54278978281592e-03
Iteration 3     bsize 3         maxres   5.26702389967756e-04
Iteration 4     bsize 3         maxres   3.87008818770373e-04
.................
Iteration 197   bsize 3         maxres   3.34025396810235e-06
Iteration 198   bsize 3         maxres   3.35960348089173e-06
Iteration 199   bsize 3         maxres   3.39105815089645e-06
Iteration 200   bsize 3         maxres   3.38848721913166e-06

Eigenvalue lambda       Residual              
  4.01462700990605e-05    3.38848721913166e-06
  4.01893266524972e-05    2.96381572768123e-06
  4.02216637895209e-05    3.25730371476605e-06

200 iterations
Solution process, seconds: 2.747978e+00
```

The option -matrix specifies the matrix to solve.
The initial eigenvectors are generated randomly.

The matrix file is in a Petsc format.
These can be setup via some Matlab programs in the PETSc socket interface to Matlab;
`PetscBinaryRead.m` and `PetscBinaryWrite.m`.
These programs read and write Matlab matrices and vectors to files formated for Petsc.
The version from Petsc only supports double.
We have modified these programs to  also support complex and 64bit integers.
Our versions are included in the .../blopex\_petsc directory along with
`PetscWriteReadExample.m` to illustrate how to use them.

The **complex files** are L-matrix-complex\_64.petsc (65536x65536 diagonal values 0-4)
and DL-matrix-complex\_64.petsc (65536x65536 tridiagonal values 0-4).

The **complex files** are complex versions of the L and DL double matrices (with imag part zero)
plus test\_complex1\_64.petsc (40x40 134 nz hpd random),
test\_complex2\_64.petsc (1000x1000 50786 nz hpd random) and
test\_complex3\_64 (10876 nz hpd random).

Driver\_fiedler accepts as input matrices in Petsc format.

For example:
```
>> ./driver_fiedler -matrix DL-matrix-complex.petsc -n_eigs 3 -itr 20
```
The option -matrix specifies the matrix to solve.
The initial eigenvectors are generated randomly.

The matrix file is in a Petsc format.
These can be setup via some Matlab programs in the PETSc socket interface to Matlab;
`PetscBinaryRead.m` and `PetscBinaryWrite.m`.
These programs read and write Matlab matrices and vectors to files formated for Petsc.
The version from Petsc only supports double.
We have modified these programs to  also support complex.
The complex versions are included in the .../blopex\_petsc directory along with
`PetscWriteReadExample.m` to illustrate how to use them.

The **double files** are L-matrix-double.petsc (65536x65536 diagonal values 0-4)
and DL-matrix-double.petsc (65536x65536 tridiagonal values 0-4).

The **complex files** are complex versions of the L and DL double matrices (with imag part zero)
plus test\_complex1.petsc (40x40 134 nz hpd random),
test\_complex2.petsc (1000x1000 50786 nz hpd random) and
test\_complex3 (10876 nz hpd random).

If PETSc was compiled with the --download-hypre=1 option (this only works without using --with-64-bit-indices or --with-scalar-type=complex since hypre does not support any of these options), one can run the driver\_fiedler using hypre preconditioner, e.g.

```
>> ./driver_fiedler -matrix DL-matrix-double.petsc -ksp_type preonly -pc_type hypre -pc_hypre_type boomeramg 
```

In this case, the matrix itself, DL-matrix-double.petsc, is used by hypre to construct
a boomeramg preconditioner. Since the matrix is singular, which hypre does not support, the results may be unpredictable.

## Driver "driver\_fortran" ##

The directory driver\_fortran contains a driver, which is actually called
"ex2f\_blopex". This is a modified version of the Fortran example ex2f.F from the PETSc
source distribution (Version 2.3.3-p4) that illustrates a way of using
BLOPEX with PETSc from Fortran.  A typical run:
```
>> ./ex2f_blopex 
BLOPEX complete in  12 iterations
eval   1 is 0.205227064E-01 residual 0.1767E-07
eval   2 is 0.512014707E-01 residual 0.4739E-07
eval   3 is 0.512014707E-01 residual 0.2178E-06
eval   4 is 0.818802350E-01 residual 0.8697E-06
eval   5 is 0.101982840E+00 residual 0.4592E-06
```

This driver DOES NOT WORK in complex AND 64-bit integers.
DOES it work with complex OR 64-bit integers?
Do we want to update it so that it always works?