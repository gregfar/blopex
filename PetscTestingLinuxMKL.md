

# Introduction #

How to test BLOPEX under version 3.1 of Petsc with MKL library on Linux , e.g., configured for 64bit integers and complex.


System configuration:

  * Intel(R) Pentium(R) ,4 CPU, 3.06GHz, 3.24 GB of RAM
  * Intel® Math Kernel Library (Intel® MKL) 10.1.1.019

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

Start the configuration process as follows to configure for complex scalars and 64bit integers:
```
>> ./config/configure.py  --with-blas-lapack-dir=/opt/intel/mkl/10.1.1.019/lib/em64t --with-mpi-dir=/usr/include/openmpi --download-blopex=1 --with-scalar-type=complex --with-64-bit-indices

```

Or use the following to configure for real scalars:
```
>> ./config/configure.py  --with-blas-lapack-dir=/opt/intel/mkl/10.1.1.019/lib/em64t --with-mpi-dir=/usr/include/openmpi --download-blopex=1

```

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

We document here tests for configuration for double and complex scalars.

## Configure for complex 64bit ##

### driver ###

mpirun -np 2 ./driver -n\_eigs 3 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

2.43042158313016e-01    2.33732859352094e-09

4.79521039879634e-01    9.33723494113888e-09

4.79521039879651e-01    7.39604636900795e-09



15 iterations

Solution process, seconds: 1.702193e+00

```

### driver\_fiedler  L-Matrix-complex\_64.petsc ###
driver\_fiedler accepts as input matrices in Petsc format. These matrices must be setup for either 32bit or 64bit arithematic, ral or complex. The test matrices are setup for double (real) or complex and for 32bit or 64bit. The 64bit version have a file name containing "64"`. All our matrices for testing can be downloaded from http://code.google.com/p/blopex/downloads/detail?name=blopex_petsc_test.tar.gz

./driver\_fiedler -matrix L-matrix-complex\_64.petsc -n\_eigs 3 -itr 20


Eigenvalues computed
```
Eigenvalue lambda       Residual              

3.24057229853955e-06    4.72681154227631e-09

9.46022689448146e-06    6.10483552187108e-09

2.01380536737721e-05    5.14827915351930e-09



15 iterations

Solution process, seconds: 2.423247e+03

Final eigenvalues:

3.240572e-06

9.460227e-06

2.013805e-05

```

### driver\_fiedler  DL-matrix-complex\_64.petsc ###

./driver\_fiedler -matrix DL-matrix-complex\_64.petsc -n\_eigs 3 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

4.01981784031632e-05    1.09799600946737e-05

4.02631352199169e-05    1.10746737154000e-05

4.08036982852300e-05    8.67316193448469e-05


20 iterations

Solution process, seconds: 8.508270e+00

Final eigenvalues:

4.019818e-05

4.026314e-05

4.080370e-05

```

### driver\_fiedler test\_complex\_64.petsc ###

./driver\_fiedler -matrix test\_complex1\_64.petsc -n\_eigs 3 -tol 1e-5 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

9.31730863870398e-01    2.57142017316005e-03

9.43321850680357e-01    1.68270918339033e-03

9.46569432391360e-01    4.37718561208011e-03

20 iterations

Solution process, seconds: 5.865788e-02

Final eigenvalues:

9.317309e-01

9.433219e-01

9.465694e-01

```

### driver\_diag ###

mpirun -np 2 ./driver\_diag

Eigenvalues computed
```
Preconditioner setup, seconds: 0.001771

Solving standard eigenvalue problem with preconditioning

block size 1

1 constraint

Initial Max. Residual   2.92990913220037e+01

Eigenvalue lambda       Residual              

4.80896806117342e+01    2.92990913220037e+01

0 iterations

Solution process, seconds: 7.721996e-02

Final eigenvalues:

4.808968e+01

```



## Configure for real ##

### driver ###

./driver -n\_eigs 3  -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

2.43042158313016e-01    2.06075488366290e-09

4.79521039879647e-01    3.30745572674208e-09

4.79521039879651e-01    3.11239055623217e-09

16 iterations

Solution process, seconds: 3.289809e-01
```

### driver\_fiedler L-matrix-double.petsc ###

./driver\_fiedler -matrix L-matrix-double.petsc -n\_eigs 3 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

3.24057229834814e-06    1.15727838536469e-09

9.46022689450812e-06    2.24542351464985e-09

2.01380536839295e-05    4.27241897671768e-09


14 iterations

Solution process, seconds: 3.973206e+02

Final eigenvalues:

3.240572e-06

9.460227e-06

2.013805e-05
```

### driver\_fiedler DL-matrix-double.petsc ###

./driver\_fiedler -matrix DL-matrix-double.petsc -n\_eigs 3 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

4.02544378185199e-05    6.39413460637352e-06

4.03050265240882e-05    2.16914565474854e-05

4.06041171481783e-05    4.82871052755024e-05

20 iterations

Solution process, seconds: 2.432976e+00

Final eigenvalues:

4.025444e-05

4.030503e-05

4.060412e-05

```

### driver\_diag ###

./driver\_diag n\_eigs 3  -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

1.23770732546427e+00    2.52745994211848e+00


20 iterations

Solution process, seconds: 1.091599e-02

Final eigenvalues:

1.237707e+00
```

### driver\_fortran ###

./ex2f\_blopex

Solution
```
BLOPEX complete in  12 iterations

eval   1 is 0.205227064E-01 residual 0.1767E-07

eval   2 is 0.512014707E-01 residual 0.4714E-07

eval   3 is 0.512014707E-01 residual 0.2178E-06

eval   4 is 0.818802350E-01 residual 0.8697E-06

eval   5 is 0.101982840E+00 residual 0.4591E-06

```