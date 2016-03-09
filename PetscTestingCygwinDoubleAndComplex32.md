

# Introduction #

How to test BLOPEX under version 3.1 of Petsc on Cygwin in Windows Vista , e.g., configured for 32bit integers and complex.

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

Start the configuration process as follows to configure for complex scalars and 32bit integers:
```
>> ./config/configure.py  --with-cc=gcc --with-fc=gfortran --with-cxx=g++ --with-f-blas-lapack=/lib --download-mpich=1 --download-blopex=1 --with-scalar-type=complex --with-clanguage=cxx
```

The option --download-mpich=1 takes very long time. If you do not need MPI, replace it the option  --with-mpi=0

Or you can install mpich2 on Windows under Cygwin. The installation of mpich2 under Cygwin is the same as installation on Linux. In this case configure as follows:
```
>> ./config/configure.py --with-cc=mpicc --with-fc=mpif90 --with-cxx=mpicxx --with-f-blas-lapack=/lib 
--with-mpich-dir=/mpich2-install --download-blopex=1 --with-scalar-type=complex --with-clanguage=cxx
```

To configure for real scalars, remove the option --with-scalar-type=complex .

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

## Configure for complex 32bit ##

### driver ###

./driver -n\_eigs 3 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

2.43042158313016e-01    4.05315093619489e-09

4.79521039879644e-01    3.53297058589270e-09

4.79521039879666e-01    2.73360846146462e-09



16 iterations

Solution process, seconds: 2.653677e+00

```

### driver\_fiedler  L-Matrix-complex.petsc ###
driver\_fiedler accepts as input matrices in Petsc format. These matrices must be setup for either 32bit or 64bit arithmetic, real or complex. The test matrices are setup for double (real) or complex and for 32bit or 64bit. The 64bit version have a file name containing "64"`. All our matrices for testing can be downloaded from http://code.google.com/p/blopex/downloads/detail?name=blopex_petsc_test.tar.gz

./driver\_fiedler -matrix L-matrix-complex.petsc -n\_eigs 2 -itr 10


Eigenvalues computed
```
Eigenvalue lambda       Residual              

3.24057229830860e-06    4.81233203447475e-10

9.46022689880392e-06    5.55107151898088e-09



8 iterations

Solution process, seconds: 1.096357e+04

Final eigenvalues:

3.240572e-06

9.460227e-06


```

### driver\_fiedler  DL-matrix-complex.petsc ###

./driver\_fiedler -matrix DL-matrix-complex.petsc -n\_eigs 3 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

4.01981784120551e-05    1.09799496270908e-05

4.02631352413017e-05    1.10746798948064e-05

4.08036982755562e-05    8.67316255262878e-05



20 iterations

Solution process, seconds: 3.206345e+01

Final eigenvalues:

4.019818e-05

4.026314e-05

4.080370e-05

```

### driver\_fiedler test\_complex1.petsc ###

./driver\_fiedler -matrix test\_complex1.petsc -n\_eigs 3 -tol 1e-5 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

9.31730863870398e-01    2.57142017316024e-03

9.43321850680357e-01    1.68270918339062e-03

9.46569432391362e-01    4.37718561208272e-03



20 iterations

Solution process, seconds: 1.824983e-01

Final eigenvalues:

9.317309e-01

9.433219e-01

9.465694e-01

```

### driver\_fiedler test\_complex2.petsc ###

./driver\_fiedler -matrix test\_complex2.petsc -n\_eigs 3 -tol 1e-5 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

5.83900226796456e-01    6.86502217585543e-03

5.92824487218932e-01    4.60860300516518e-03

5.95744059452787e-01    8.66023275371204e-03



20 iterations

Solution process, seconds: 5.299701e+00

Final eigenvalues:

5.839002e-01

5.928245e-01

5.957441e-01

```

### driver\_fiedler test\_complex3.petsc ###

./driver\_fiedler -matrix test\_complex3.petsc -n\_eigs 3 -tol 1e-5 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

9.61926171742241e+01    4.60108796113616e-02

9.62485999275871e+01    1.38214270890162e-01

9.62891435698207e+01    4.42759548368067e-02



20 iterations

Solution process, seconds: 1.231342e+00

Final eigenvalues:

9.619262e+01

9.624860e+01

9.628914e+01

```

### driver\_diag ###

./driver\_diag -n\_eigs 3 -tol 1e-6 -itr 20

Eigenvalues computed
```
Preconditioner setup, seconds: 0.002168

Solving standard eigenvalue problem with preconditioning

block size 3

1 constraint

Initial Max. Residual   3.02086358779961e+01

Eigenvalue lambda       Residual              

1.20380409373538e+00    2.15412601685457e+00

2.24153368254557e+00    2.47605811026978e+00

3.26833543336572e+00    2.70123706781517e+00



20 iterations

Solution process, seconds: 1.636976e-01

Final eigenvalues:

1.203804e+00

2.241534e+00

3.268335e+00

```

### driver\_fortran ###


./ex2f\_blopex

Solution
```
BLOPEX complete in  13 iterations

eval   1 is 0.205227064E-01 residual 0.3433E-07

eval   2 is 0.512014707E-01 residual 0.1338E-06

eval   3 is 0.512014707E-01 residual 0.1571E-06

eval   4 is 0.818802350E-01 residual 0.6035E-06

eval   5 is 0.101982840E+00 residual 0.7529E-06
```



## Configure for real 32bit ##

### driver ###

./driver -n\_eigs 3  -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

2.43042158313016e-01    2.06075489797594e-09

4.79521039879647e-01    4.41011822871539e-09

4.79521039879649e-01    7.25866549368105e-09



16 iterations

Solution process, seconds: 3.742414e-01
```

### driver\_fiedler L-matrix-double.petsc ###

./driver\_fiedler -matrix L-matrix-double.petsc -n\_eigs 3 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              

3.24057229835491e-06    1.08379498003101e-09

9.46022689455523e-06    2.33567983484665e-09

2.01380536843911e-05    4.39095965249420e-09



14 iterations

Solution process, seconds: 9.774528e+02

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

4.02544378184729e-05    6.39420026735611e-06

4.03050265235268e-05    2.16914540699202e-05

4.06041171454049e-05    4.82871036680101e-05



20 iterations

Solution process, seconds: 4.399860e+00

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

Solution process, seconds: 1.081169e-01

Final eigenvalues:

-3.277121e+159
-3.277121e+159
-3.277121e+159
```

### driver\_fortran ###


Run
```
>> ./ex2f_blopex 
```
without any options, since options are not supported by ex2f\_blopex.


Solution
```
BLOPEX complete in  12 iterations

eval   1 is 0.205227064E-01 residual 0.1767E-07

eval   2 is 0.512014707E-01 residual 0.4745E-07

eval   3 is 0.512014707E-01 residual 0.2178E-06

eval   4 is 0.818802350E-01 residual 0.8697E-06

eval   5 is 0.101982840E+00 residual 0.4591E-06
```