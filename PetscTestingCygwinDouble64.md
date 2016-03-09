

# Introduction #
How to test BLOPEX under version 3.1 of Petsc on Cygwin in Windows 7, e.g., configured for 64bit integers and double.
# Setup Petsc #
To use blopex\_solve\_double or blopex\_solve\_complex using Petsc first download Petsc at http://ftp.mcs.anl.gov/pub/petsc/release-snapshots/petsc-lite-3.1-p3.tar.gz
To install files issue command:
```
>> tar -zxvf petsc-lite-3.1-p3.tar.gz
```
This creates files in directory petsc-3.1-p3.
Switch to the Petsc directory and set PETSC\_DIR environment variable.
```
>> cd petsc-3.1-p3
>> export PETSC_DIR=$PWD
```
Start the configuration process as follows to configure for double and 64bit integers.  The installation of mpich2 on windows under cygwin is the same as in Linux, please see http://www.mcs.anl.gov/research/projects/mpich2/documentation/files/mpich2-1.2.1-installguide.pdf
```
>> ./config/configure. --with-f-blas-lapack=/cygdrive/c/cygwin/lib --with –mpich-dir=/cygdrive/c/cygwin/usr/local/bin/mpich2-install  --download-blopex=1 --with-64-bit-indices
```
Or  if you haven’t installed mpich on Windows yet,  you can use option  --download-mpich=1
```
>> ./config/configure. --with-f-blas-lapack=/cygdrive/c/cygwin/lib --download-mpich=1 --download-blopex=1 --with-64-bit-indices
```
The option --download-mpich=1 takes very long time. If you do not need MPI, replace it the option --with-mpi=0

After configuration is finished issue the command
```
>> make all
>> make test
```

# Setup blopex petsc drivers #
Now create executables for the blopex drivers
```
>> cd src/contrib/blopex/driver
>> make driver
>> cd ../driver_fiedler
>> make driver_fiedler
>> cd ../driver_diag
>> make driver_diag
>> cd ../driver_fortran
>> make ex2f_blopex
```
# Execution of Tests #
See wiki PetscTestingLinux for descriptions of test programs.
There are 4 test drivers located in subdirectories of ../blopex: driver,  driver\_fiedler driver\_diag, and driver\_fortran.
# Test Results #
We document here tests for configuration double and 64bit
## Configure for double  64bit ##

### driver ###

./driver -n\_eigs 3 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual
  2.43042158313016e-01    2.06075489797594e-09
  4.79521039879647e-01    4.41011822871539e-09
  4.79521039879649e-01    7.25866549368105e-09
16 iterations
Solution process, seconds: 2.569786e-01
```
### driver\_fiedler  L-matrix-double\_64.petsc ###
driver\_fiedler accepts as input matrices in Petsc format. These matrices must be setup for either 32bit or 64bit arithematic, ral or complex. The test matrices are setup for double (real) or complex and for 32bit or 64bit. The 64bit version have a file name containing "64"`. All our matrices for testing can be downloaded from http://code.google.com/p/blopex/downloads/detail?name=blopex_petsc_test.tar.gz

./driver\_fiedler -matrix L-matrix-double\_64.petsc -n\_eigs 3 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual
  3.24057229835491e-06    1.08379498003101e-09
  9.46022689455523e-06    2.33567983484665e-09
  2.01380536843911e-05    4.39095965249420e-09

14 iterations
Solution process, seconds: 5.487032e+02
Final eigenvalues:
3.240572e-06
9.460227e-06
2.013805e-05
```
### driver\_fiedler  DL-matrix-double\_64.petsc ###

./driver\_fiedler -matrix DL-matrix-double\_64.petsc -n\_eigs 3 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual
  4.02544378184729e-05    6.39420026735611e-06
  4.03050265235268e-05    2.16914540699202e-05
  4.06041171454049e-05    4.82871036680101e-05
20 iterations
Solution process, seconds: 2.433026e+00
Final eigenvalues:
4.025444e-05
4.030503e-05
4.060412e-05
```

### driver\_diag ###

./driver\_diag -n\_eigs 3 -tol 1e-6 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual
  1.20073961755945e+00    2.12082900660306e+00
  2.23691202511820e+00    2.42856881925111e+00
  3.26199055636483e+00    2.63494491986278e+00
20 iterations
Solution process, seconds: 2.580865e-02
Final eigenvalues:
-9.290899e+260
-9.290899e+260
-9.290899e+260
```

### driver\_fortran ###

./ex2f\_blopex

Solution:
```
BLOPEX complete in  12 iterations
eval   1 is 0.205227064E-01 residual 0.1767E-07
eval   2 is 0.512014707E-01 residual 0.4715E-07
eval   3 is 0.512014707E-01 residual 0.2178E-06
eval   4 is 0.818802350E-01 residual 0.8697E-06
eval   5 is 0.101982840E+00 residual 0.4591E-06
```