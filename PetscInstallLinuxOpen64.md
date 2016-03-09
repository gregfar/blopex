# Introduction #

How To install PETSc with AMD's OPEN64 compiler in Fedora13 64 bit
(assuming bash shell)

# Preliminaries #

The basic system info:

```
>> uname -a
Linux opt4 2.6.33.6-147.fc13.x86_64 #1 SMP Tue Jul 6 22:32:17 UTC 2010 x86_64 x86_64 x86_64 GNU/Linux
```

Download from http://developer.amd.com/cpu/open64/ and install the AMD's OPEN64 compiler.
The default location is /opt/x86\_open64-4.2.4/ (depending on the version number). Make sure that you add the OPEN64 binaries location to your PATH:

```
>> PATH=/opt/x86_open64-4.2.4/bin/:$PATH; export PATH
>>  which opencc
/opt/x86_open64-4.2.3.2/bin/opencc
```

Download and install ACML libraries, compiled with OPEN64:
  * http://developer.amd.com/Downloads/acml-4-4-0-open64-64bit.tgz
  * http://developer.amd.com/Downloads/acml-4-4-0-open64-64bit-int64.tgz
The default locations are:
    * /opt/acml-4-4-0-open64-64bit
    * /opt/acml-4-4-0-open64-64bit-int64

Each library comes in two different flavours, regular and mp, e.g.,
  * /opt/acml4.4.0/open64\_64/lib
  * /opt/acml4.4.0/open64\_64\_mp/lib

According to Ch. 2 of the ACML manual, see
http://developer.amd.com/assets/acml_userguide.pdf
one should use the mp version on a multi-CPU/core nodes.However, using the mp libraries requires having pre-installed MPI libraries (compiled with OPEN64 ?), which complicates the PETSc configure, which also asks for an MPI setup. I (AK) guess that a clean setup using the mp version of ACML and PETSc with MPI compiled using OPEN64 basically requires having an MPI library (either ONEMMPI or MPICH2) pre-compiled with OPEN64 and pre-installed, so that it could be used by both the mp version of ACML and PETSc with MPI. In this test, I do the simple combination, where a non-mp version of ACML is used and PETSc downloads and complies (with OPEN64) the MPI.


We need set the path to the libraries by running

```
>> LD_LIBRARY_PATH=/opt/acml4.4.0/open64_64/lib:$LD_LIBRARY_PATH; export LD_LIBRARY_PATH
```

If SELinux is active, disable SELinux temporarily by running (as root)

```
>> echo 0 > /selinux/enforce 
```

After you are done running the codes, restore SELinux by

```
>> echo 1 > /selinux/enforce
```

IMPORTANT:
Download and extract the LATEST PETSc, at the moment this is petsc-3.2-p2, at
[ftp://info.mcs.anl.gov/pub/petsc/release-snapshots/petsc-lite-3.2.tar.gz](ftp://info.mcs.anl.gov/pub/petsc/release-snapshots/petsc-lite-3.2.tar.gz)

Extract using **tar xzvf** , rename the PETSc dir as you like, cd to PETSc dir and run

```
>> PETSC_DIR=$PWD; export PETSC_DIR
```

Version petsc-3.2 has the BLOPEX code removed from it. To install BLOPEX, one now needs to install SLEPc.

## PETSc configure without MPI ##

In PETSc, the --download-hypre=1 option is only available with MPI, so no hypre here:

```
>> ./configure --with-blas-lapack-dir=/opt/acml4.4.0/open64_64 --with-cc=opencc --with-cxx=openCC --with-fc=openf95 --with-mpi=0 
>> make all
>> make test 
```

## PETSc configure with MPI and with hypre ##

Run configure, e.g.,

```
>> ./configure --with-blas-lapack-dir=/opt/acml4.4.0/open64_64 --with-cc=opencc --with-cxx=openCC --with-fc=openf95 --download-hypre=1 --download-openmpi=1
```

Here, we
  * use the AMD's ACML library /opt/acml4.4.0/open64\_64 for BLAS/LAPACK
  * specify the AMD's OPEN64 compilers, which are already in our PATH by options --with-cc=opencc --with-cxx=openCC --with-fc=openf95
  * since this is the real arithmetic and 32-bit integers, we compile blopex and hypre together
  * we must specify the --download-openmpi=1 or --download-mpich=1 option, since the MPI libraries must be compiled with the same compiler as PETSc. This option not only compiles the MPI libraries, but also creates the local mpicc wrapper to opencc, etc, which is used during the make

Now, we run
```
>> make PETSC_ARCH=linux-gnu-c-debug all
```
and hope that there are no errors. Now, the --download-openmpi=1 or --download-mpich=1 option also creates the local version of mpirun, which must be used instead whatever other mpirun binary you might have pre-installed on your system. So we add it in front of the PATH, in my case (it will also depend on the specified PETSC\_ARCH)

```
>> PATH=/home/aknyazev/work/petsc/petsc-3.1-p3-OPENMPI_OPEN64_hypre/linux-gnu-c-debug/bin:$PATH; export PATH
>>  which mpirun
~/work/petsc/petsc-3.1-p3-OPENMPI_OPEN64_hypre/linux-gnu-c-debug/bin/mpirun
```

We also need to add the newly compiled local version of MPI libs to LD\_LIBRARY\_PATH by
```
>> LD_LIBRARY_PATH=/home/aknyazev/work/petsc/petsc-3.1-p3-OPENMPI_OPEN64_hypre/linux-gnu-c-debug/lib:$LD_LIBRARY_PATH; export LD_LIBRARY_PATH
```


```
>> make PETSC_ARCH=linux-gnu-c-debug test
```
