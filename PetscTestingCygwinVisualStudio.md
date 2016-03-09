# Preliminaries #

For these tests the C/C++ compiler which is part of the Visual C++ 9.0 Express package is used in Cygwin. Only the drivers which are written in C are tested here, because Intel's Visual Fortran compiler for Windows is not available for free non-commercial use. This is also the case with Intel's C/C++ compilers, which are only available freely for Linux. These linux versions will not install in Cygwin.

Also for these tests the modifications in blopex-1.1.1-petsc\_abstract.tar.gz and the recent test changes made to blopex.py are used.

The basic system info:

```
>> uname -a
CYGWIN_NT-6.0 HomePC 1.7.5(0.225/5/3) 2010-04-12 19:07 i686 Cywing
```

Before starting the PETSc configure process, it is necessary to launch Cygwin from the Visual Studio 2008 Command Prompt.  To do this open the VS 2008 folder, then go into the VS 2008 tools folder, and then launch the command prompt.

Once the terminal window is up, input 'c:\cygwin\bin\bash.exe --login' to launch a Cygwin shell.  Type 'cl' to verify the Microsoft compiler is recognized. If a Microsoft Fortran compiler is available this can also be verified by inputting 'ifort'. For more information see: http://www.mcs.anl.gov/petsc/petsc-as/documentation/installation.html#Windows

Now the PETSc installation process may begin.


# Setup Petsc #

First download the latest PETSc distribution from http://ftp.mcs.anl.gov/pub/petsc/release-snapshots/petsc-3.1.tar.gz  which currently links to petsc-3.1-p6.tar.gz.  Decompress the file and then enter the newly created directory.

Once there, set the PETSC\_DIR environment variable.
```
>> export PETSC_DIR=$PWD
```

Now start the configuration process, with the petsc-3.1-p/blopex-1.1.1-petsc\_abstract\_tar.gz in the home directory, with the blopex.py file replaced in the config/PETSc/packages directory, and with the Microsoft compiler as follows:

```
>> ./config/configure.py --with-shared --with-debugging=1 --download-blopex=1  --with-mpi=0 --with-fc=0 --with-cc='win32fe cl' --download-c-blas-lapack=1
```

Note that it must be specified to use no Fortran compiler.  Otherwise the configuration will fail, because the Microsoft C compiler is incompatible with Cygwin's gfortran compiler, which PETSc may attempt to use.

Once the configuration is complete, run
```
>> make all; make test
```

Assuming no errors, enter each driver directory and compile the drivers as follows:

```
>> cd src/contrib/blopex/driver
>> make driver
>> cd ../driver_fiedler
>> make driver_fiedler
>> cd ../driver_diag
>> make driver_diag
```


# Execution of Tests #

## driver ##

./driver -n\_eigs 3 -itr 20

Eigenvalues Computed
```
Eigenvalue lambda       Residual              
 2.43042158313017e-001   9.69687664164884e-009
 4.79521039879648e-001   3.51521484658108e-009
 4.79521039887646e-001   2.32370919951873e-009

16 iterations
Solution process, seconds: 3.788256e-001
```

## driver\_fiedler ##

### driver\_fiedler L-matrix-double.petsc ###

./driver\_fiedler -matrix L-matrix-double.petsc -n\_eigs 3 -itr 20

Eigenvalues Computed
```
Eigenvalue lambda       Residual              
 3.24057709006772e-006   3.92795742665550e-007
 9.46022732748744e-006   2.96997618354929e-007
 2.01382183477503e-005   8.37520409262231e-007

9 iterations
Solution process, seconds: 7.300088e+002
Final eigenvalues:
3.240577e-006
9.460227e-006
2.013822e-005
```

### driver\_fiedler DL-matrix-double.petsc ###

./driver\_fiedler -matrix DL-matrix-double.petsc -n\_eigs 3 -itr 20 -no\_con

Eigenvalues Computed
```
Eigenvalue lambda       Residual              
 4.01537928522532e-005   8.88326655865219e-006
 4.02287238866501e-005   1.05453174246516e-005
 4.03559186288959e-005   1.63298888631709e-005

20 iterations
Solution process, seconds: 5.353354e+000
Final eigenvalues:
4.015379e-005
4.022872e-005
4.035592e-005
```


## driver\_diag ##

./driver\_diag -N 100000 -n\_eigs 5 -tol 1e-6 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              
 9.99999999999483e-001   1.23625619315561e-007
 1.99999999999707e+000   1.05362368074337e-007
 2.99999999999131e+000   4.26333417389354e-007
 3.99999999997760e+000   6.75890926989294e-007
 4.99999999999100e+000   4.33648318708520e-007

20 iterations
Solution process, seconds: 1.077368e+001
Final eigenvalues:
1.000000e+000
2.000000e+000
3.000000e+000
4.000000e+000
5.000000e+000
```