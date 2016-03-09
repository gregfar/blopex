

# Introduction #

How to test BLOPEX under version 3.1-p3 of PETSc on IBM Linux system Frost.

# Setup Petsc #

To use blopex\_solve\_double or blopex\_solve\_complex using Petsc first download the current release from
the Petsc website at http://ftp.mcs.anl.gov/pub/petsc/release-snapshots/petsc-lite-3.1-p3.tar.gz

If your already have version 3.1-p3 installed remove it.
```
rm -r petsc-3.1-p3
```

To install files issue command:
```
>> tar -zxvf petsc-lite-3.1-p3.tar.gz
```

This creates files in directory petsc-3.1-p3.

A special config file is needed. This is a python script that acts as a wrapper on the normal configure.py. Frost has a version of this for Petsc 3.0 at /contrib/bgl/petsc/petsc-3.0.0-p4/bgl-ibm-goto\_lapack.py. We must make changes for Petsc 3.1.  Copy it to your home directory.
```
>> cp /contrib/bgl/petsc/petsc-3.0.0-p4/bgl-ibm-goto_lapack.py $HOME/
```

Then edit it as follows.  The names for some configure options --sizeof\_xxx and --bits\_xxx have changed.  The underscores are replace with dashes.
Change these lines to the following (they are all together).
```
  '--sizeof-char=1',
  '--sizeof-void-p=4',
  '--sizeof-short=2',
  '--sizeof-int=4',
  '--sizeof-long=4',
  '--sizeof-size_t=4',
  '--sizeof-long-long=8',
  '--sizeof-float=4',
  '--sizeof-double=8',
  '--bits-per-byte=8',
  '--sizeof-MPI_Comm=4',
  '--sizeof-MPI_Fint=4',
```

Swith to the Petsc directory.
```
>> cd petsc-3.1-p3
```

Now we need to install a patch for the Frost system to Petsc. This is for version 3.0 but is still valid for version 3.1.  It adds the line "#define SO\_REUSEADDR 2" to "src/sys/viewer/impls/socket/socket.h".
```
>> patch -p0 < /contrib/bgl/petsc/petsc-3.0.0-p4/petsc-3.0.0-p4.patch
```

Download the blopex\_petsc\_test.tar.gz from the Blopex Google Code site http://code.google.com/p/blopex to your home directory in Frost.

blopex\_petsc\_test.tar.gz contains the test files for use with driver\_fiedler and is not distributed as part of Petsc.  Install as follows:
```
>> tar -zxvf $HOME/blopex_petsc_test.tar.gz
```

Start the configuration process as follows to configure for complex scalars and 64bit integers:
```
>> ./config/bgl-ibm-goto_lapack.py --with-shared --with-debugging=1 --with-scalar-type=complex --download-blopex=1 --with-64-bit-indices
```

Or use the following to configure for real scalars:
```
>> ./config/bgl-ibm-goto_lapack.py --with-shared --with-debugging=1 --download-blopex=1
```

This creates file conftest which must be executed via the Frost job scheduler.
```
>> cqsub -n 1 -t 00:10:00 ./conftest
```

This creates file reconfigure.py which is now executed.
```
>> ./reconfigure.py
```

Now two environment variables must be setup as specified in the configuration run. Note by default we are running under the csh shell.
```
>> setenv PETSC_ARCH bgl-ibm-goto-O3_440d  
>> setenv PETSC_DIR /home/dmccuan/petsc-3.1-p3
```

We will also need these set when compiling the blopex\_petsc drivers.

After configuration is finished issue the command
```
>> make all
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

Frost executes mpi jobs under the cobalt batch system.
Jobs are submitted via command cqsub.

For example to submit driver\_fiedler change to the .../blopex\_petsc/driver\_fiedler subdirectory and issue command:
```
cqsub -n 32 -t 00:10:00 ./driver_fiedler ...etc...
```

A batch job number is returned (say 894111).  Output is returned to current directory.
Output consists of 894111.cobaltlog, 894111.error, and 894111.output

Job execution can be monitored via command cqstat; see http://www.cisl.ucar.edu/docs/frost/cbr.jsp for details.

# Test Results #

The tests were executed on UCAR Frost system in May 2009.  This is an Linux operating system and the IBM blrts\_xlc compiler was used.
We document here tests for configuration for double and complex scalars.

## Configure for complex 64bit ##

### driver ###

cqsub -n 2 -t 00:10:00 ./driver -n\_eigs 3 -tol 1e-6 -itr 100

Eigenvalues computed
```
Eigenvalue lambda       Residual              
  2.43042158313038e-01    2.08649386978591e-07
  4.79521039879770e-01    3.97437752428054e-07
  4.79521039880098e-01    9.35903863436731e-07

12 iterations
Solution process, seconds: 8.453100e-01
```

### driver\_fiedler  L-Matrix-complex\_64.petsc ###

cqsub -n 32 -t 00:10:00 ./driver\_fiedler -matrix L-matrix-complex\_64.petsc -n\_eigs 3 -tol 1e-6 -itr 100

Eigenvalues computed
```
Eigenvalue lambda       Residual
  3.24057359286083e-06    1.91494098580843e-07
  9.46022761089112e-06    1.57460918832596e-07
  2.01381164321128e-05    4.95499301342251e-07

9 iterations
Solution process, seconds: 1.607735e+02
```

### driver\_fiedler  DL-matrix-complex\_64.petsc ###

cqsub -n 32 -t 00:10:00 ./driver\_fiedler -matrix DL-matrix-complex\_64.petsc -n\_eigs 3 -tol 1e-6 -itr 100

Eigenvalues computed
```
Eigenvalue lambda       Residual
  4.01465628960898e-05    4.00976249036245e-06
  4.01895683207737e-05    3.43961310385852e-06
  4.02219736598776e-05    3.71128092391366e-06

100 iterations
Solution process, seconds: 3.270503e+00
```

### driver\_fiedler test\_complex\_64.petsc ###

cqsub -n 32 -t 00:10:00 ./driver\_fiedler -matrix test\_complex1\_64.petsc -n\_eigs 3 -tol 1e-5 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual
  9.31730862341453e-01    2.57258834855600e-03
  9.43321847266000e-01    1.68303531203714e-03
  9.46571607138539e-01    4.34998449409954e-03

20 iterations
Solution process, seconds: 2.992890e-01
```

### driver\_diag ###

cqsub -n 2 -t 00:10:00 ./driver\_diag -N 1000000 -n\_eigs 3 -tol 1e-6 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual              
  1.17941832974984e+00    2.00229003618771e+02
  2.21133286107945e+00    2.28558048602907e+02
  3.23487962354555e+00    2.47221197617945e+02

20 iterations
Solution process, seconds: 1.416870e+02
```

Note: couldn't allocate sufficient memory for 9000000 which was the maximum tested on xvia.

## Configure for real ##

### driver ###

cqsub -n 2 -t 00:10:00 ./driver -n\_eigs 3 -tol 1e-6 -itr 100

Eigenvalues computed
```
Eigenvalue lambda       Residual
  2.43042158313057e-01    3.10104509901596e-07
  4.79521039879721e-01    2.87134661860326e-07
  4.79521039879794e-01    4.46830893821923e-07

12 iterations
Solution process, seconds: 5.081041e-01
```

### driver\_fiedler L-matrix-double.petsc ###

cqsub -n 32 -t 00:10:00 ./driver\_fiedler -matrix L-matrix-double.petsc -n\_eigs 3 -tol 1e-6 -itr 100

Eigenvalues computed
```
Eigenvalue lambda       Residual              
  3.24058473765578e-06    9.44745037985923e-07
  9.46022774674964e-06    5.44058082719764e-07
  2.01381016874220e-05    7.77626225297255e-07

10 iterations
Solution process, seconds: 3.457243e+01
```

### driver\_fiedler DL-matrix-double.petsc ###

cqsub -n 32 -t 00:10:00 ./driver\_fiedler -matrix DL-matrix-double.petsc -n\_eigs 3 -tol 1e-6 -itr 100

Eigenvalues computed
```
Eigenvalue lambda       Residual              
  4.01481104481621e-05    4.31173215041207e-06
  4.01933186080514e-05    3.76125152651444e-06
  4.02251135174289e-05    4.42453528320163e-06

100 iterations
Solution process, seconds: 1.033284e+00
```

### driver\_diag ###

cqsub -n 2 -t 00:10:00 ./driver\_diag -N 100000 -n\_eigs 3 -tol 1e-6 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual
  1.17941832974984e+00    2.00229003618771e+02
  2.21133286107945e+00    2.28558048602907e+02
  3.23487962354555e+00    2.47221197617945e+02

20 iterations
Solution process, seconds: 1.416870e+02
```

### driver\_fortran ###

cqsub -n 2 -t 00:10:00 ./ex2f\_blopex

Solution
```
BLOPEX complete in  12 iterations
eval   1 is  .205227064E-01 residual  .1927E-07
eval   2 is  .512014707E-01 residual  .1479E-06
eval   3 is  .512014707E-01 residual  .2956E-06
eval   4 is  .818802350E-01 residual  .4227E-06
eval   5 is  .101982840E+00 residual  .9285E-06
```