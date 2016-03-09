

# Introduction #

How to test BLOPEX under version 3.1 of Petsc on IBM Linux system Frost.

# Setup Petsc #

To use blopex\_solve\_double or blopex\_solve\_complex using Petsc first download the development version from
the Petsc website at http://www.mcs.anl.gov/petsc/petsc-2/download/index.html .

If your already have version 3.1-p2 installed remove it.
```
rm -r petsc-3.1-p2
```

To install files issue command:
```
>> tar -zxvf petsc-lite-3.1-p2.tar.gz
```

This creates files in directory petsc-3.1-p2.

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
>> cd petsc-3.1-p2
```

Now we need to install a patch for the Frost system to Petsc. This is for version 3.0 but is still valid for version 3.1.  It adds the line "#define SO\_REUSEADDR 2" to "src/sys/viewer/impls/socket/socket.h".
```
>> patch -p0 < /contrib/bgl/petsc/petsc-3.0.0-p4/petsc-3.0.0-p4.patch
```

Download the blopex\_petsc\_interface.tar.gz and blopex\_petsc\_abstract.tar.gz from the Blopex Google Code site http://code.google.com/p/blopex to your home directory in Frost.

blopex\_petsc\_interface.tar.gz contains the changes to the blopex interface which is distributed as part of Petsc.  Install as follows:
```
>> tar -zxvf $HOME/blopex_petsc_interface.tar.gz
```

Start the configuration process as follows:
```
>> ./config/bgl-ibm-goto_lapack.py --with-shared --with-debugging=1 --download-blopex=$HOME/blopex_petsc_abstract.tar.gz --with-64-bit-indices
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
>> setenv PETSC_DIR /home/dmccuan/petsc-3.1-p2
```

We will also need these set when compiling the blopex\_petsc drivers.

Petsc runs with either double or complex objects but not both.
To test the blopex routines for double configure Petsc with the
option --with-scalar-type=complex.

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

## Configure for double 64bit ##

### driver ###

cqsub -n 2 -t 00:10:00 ./driver -n\_eigs 3 -tol 1e-6 -itr 100

Eigenvalues computed
```
Eigenvalue lambda       Residual
  2.43042158313057e-01    3.10104509873990e-07
  4.79521039879720e-01    2.86830771055640e-07
  4.79521039879793e-01    4.47026029928304e-07

12 iterations
Solution process, seconds: 5.273230e-01
```

### driver\_fiedler  L-Matrix-double\_64.petsc ###

cqsub -n 32 -t 00:10:00 ./driver\_fiedler -matrix L-matrix-double\_64.petsc -n\_eigs 3 -tol 1e-6 -itr 100

Eigenvalues computed
```
Eigenvalue lambda       Residual
  3.24058511701059e-06    9.32220664952614e-07
  9.46022771228424e-06    5.41591796408131e-07
  2.01381051637836e-05    7.93246331928602e-07

10 iterations
Solution process, seconds: 3.537082e+01
```

### driver\_fiedler  DL-matrix-double\_64.petsc ###

cqsub -n 32 -t 00:10:00 ./driver\_fiedler -matrix DL-matrix-double\_64.petsc -n\_eigs 3 -tol 1e-6 -itr 100

Eigenvalues computed
```
Eigenvalue lambda       Residual
  4.01481104481621e-05    4.31173215041207e-06
  4.01933186080514e-05    3.76125152651444e-06
  4.02251135174289e-05    4.42453528320163e-06

100 iterations
Solution process, seconds: 1.077944e+00
```

### driver\_diag ###

cqsub -n 2 -t 00:10:00 ./driver\_diag -N 1000000 -n\_eigs 3 -tol 1e-6 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual
  1.18514681741652e+00    2.05852723173605e+02
  2.21912019728413e+00    2.35335764278854e+02
  3.24440742392664e+00    2.54876796330367e+02

20 iterations
Solution process, seconds: 1.598297e+00
```

Note: couldn't allocate sufficient memory for 9000000 which was the maximum tested on xvia.

## Configure for complex ##

### driver ###

cqsub -n 2 -t 00:10:00 ./driver -n\_eigs 3 -tol 1e-6 -itr 100

Eigenvalues computed
```
Eigenvalue lambda       Residual
  2.43042158313038e-01    2.08649386830968e-07
  4.79521039879770e-01    3.96871687423308e-07
  4.79521039880099e-01    9.36144043172215e-07

12 iterations
Solution process, seconds: 7.651389e-01
```

### driver\_fiedler L-matrix-complex.petsc ###

cqsub -n 32 -t 00:10:00 ./driver\_fiedler -matrix L-matrix-complex.petsc -n\_eigs 3 -tol 1e-6 -itr 100

Eigenvalues computed
```
 Eigenvalue lambda       Residual
  3.24057358358309e-06    1.91215402474947e-07
  9.46022761654950e-06    1.57345131019328e-07
  2.01381155630837e-05    4.94125848572324e-07

9 iterations
Solution process, seconds: 1.674311e+02
Final eigenvalues:
3.240574e-06
9.460228e-06
2.013812e-05
```

### driver\_fiedler DL-matrix-complex.petsc ###

cqsub -n 32 -t 00:10:00 ./driver\_fiedler -matrix DL-matrix-complex.petsc -n\_eigs 3 -tol 1e-6 -itr 100

Eigenvalues computed
```
Eigenvalue lambda       Residual
  4.01465628960898e-05    4.00976249036245e-06
  4.01895683207737e-05    3.43961310385852e-06
  4.02219736598776e-05    3.71128092391366e-06

100 iterations
Solution process, seconds: 3.279958e+00
Final eigenvalues:
4.014656e-05
4.018957e-05
4.022197e-05
```

### driver\_fiedler test\_complex1.petsc ###

cqsub -n 32 -t 00:10:00 ./driver\_fiedler -matrix test\_complex1.petsc -n\_eigs 3 -tol 1e-5 -itr 20

Eigenvalues computed
```
Eigenvalue lambda       Residual
  9.31730862341453e-01    2.57258834855579e-03
  9.43321847266005e-01    1.68303531203725e-03
  9.46571607138541e-01    4.34998449410501e-03

20 iterations
Solution process, seconds: 2.958031e-01
Final eigenvalues:
9.317309e-01
9.433218e-01
9.465716e-01
```