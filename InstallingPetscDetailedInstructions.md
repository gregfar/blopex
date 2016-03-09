# Downloading PETSc #

PETSc can be downloaded from the PETSc website: http://www.mcs.anl.gov/petsc/petsc-as/. For a direct download of the version 3.1-p4 lite tar file, type:

```
$ wget ftp://info.mcs.anl.gov/pub/petsc/release-snapshots/petsc-lite-3.1-p4.tar.gz
```

Then unzip:

```
$ tar -xzvf petsc-lite-3.1-p4.tar.gz
```

Now we can rename the Petsc folder if we choose. E.g.,

```
$ mv petsc-3.1-p4 petsc1
```


# Configuration Options #

There are many options we can specify for configuring PETSc. For a list of these options, in your PETSc directory, type:

```
$ ./configure –help
```

We will be mainly concerned with selecting versions of MPI and BLAS/Lapack. Also, we must specify in the configuration if we wish to work with complex numbers. We can also specify whether to use 64 bit integers instead of double 32 bit integers. This may increase performance on 64 bit processor machines. We will also specify that we want to download Blopex and will specify a directory in which we have installed Hypre. PETSc allows us to use certain external solvers, including Hypre, and we will make use of this in some of our solves.


# Selecting a version of MPI: #

PETSc may be installed without using an MPI. However, many programs written in PETSc will make use of MPI commands, and therefore it is a good idea to have an MPI installed. There are two main versions of MPI we will use here: MPICH2 and OpenMPI. You can find out if either of these are installed by typing:

```
$ rpm -q mpich2
$ rpm -q mpich2-devel
$ rpm -q openmpi
$ rpm -q openmpi-devel
```

Each MPI build must have a separate -devel subpackage, so make sure you have the -devel subpackage installed as well. (Reference: http://fedoraproject.org/wiki/Packaging:MPI)

If no version of MPI is installed, you will need to install one. OpenMPI is part of the Open64 package from AMD, so if you have an AMD processor, you can install the Open64 package. If you have root privileges just use a package manager such as:

```
$ yum install open64
$ yum install mpich2
```

If you do not have root priveleges, you can just download MPICH2 or OpenMPI or Open64 from the internet.

## Open64 (OpenMPI) ##

For Open64, the official page is: http://developer.amd.com/cpu/open64/pages/default.aspx. If you choose this option, you just need to download and unpack the Open64 files. For example:

```
$ wget http://download2-developer.amd.com/amd/open64/x86_open64-4.2.4-1.x86_64.tar.bz2
$ tar xjvf x86_open64-4.2.4-1.x86_64.tar.bz2
$ mv x86_open64-4.2.4 ../x86_open64
$ rpm -q openmpi
$ rpm -q openmpi-devel
```

You should now see that OpenMPI is installed.

## OpenMPI ##

OpenMPI can be downloaded by itself from the official website: http://www.open-mpi.org/. Installation instructions are included in the README file.

## MPICH2 ##

MPICH2 may also be downloaded form the official website: http://www.mcs.anl.gov/research/projects/mpich2/. Installation instructions are also included in the README file.

Switching Between MPI's

Now that we have one or both versions of MPI installed, we can install PETSc. The default configuration of PETSc will use the version of MPI currently stored in your environment variables. To find out which this is, type:

```
$ env | grep MPI
```

Take note of the variables MPI\_LIB and MPI\_COMPILER

Example:
```
...
MPI_LIB=/usr/lib64/mpich2/lib
...
MPI_COMPILER=mpich2-x86_64.
…
```


If we have both OpenMPI and MPICH2 installed, we can switch between the two.

To switch from MPICH2 to OpenMPI, first unload the MPICH2 module and then load the OpenMPI modules.

On x86\_64 processors:

```
$ module unload mpich2-x86_64
$ module load openmpi-x86_64
```

On x86 processors:

```
$ module unload mpich2-i686
$ module load openmpi-i686
```

Note: It is important to unload the existing MPI module before loading the new one, otherwise some compilers may still be associated with the first MPI module.

Double check that the desired MPI module is loaded:

```
$ env | grep MPI
$ $LD_LIBRARY_PATH
$ which mpicc
```

Each of these commands should give a reference to the desired version of MPI.

You will have to load the correct MPI module for your PETSc install each time you log in.


# Specifying BLAS/Lapack Libraries: #

BLAS and Lapack are software libraries for numerical linear algebra that are used by PETSc. If one does not specify the location of BLAS/Lapack libraries in the PETSc install, then PETSc will look for these libraries in places they are normally located. You may want to specify BLAS/Lapack libraries if they already exist on your computer.

AMD and Intel put out math libraries which contain versions of BLAS and Lapack. The AMD version is called ACML and the Intel version is called MKL. There also might be precompiled libraries provided with your distribution of Linux.

## ACML ##

On AMD systems, to see if ACML is installed, type:

```
$ locate libacml.so
```

This will show the directory where the ACML libraries are stored. In my case, we have

```
/opt/acml4.3.0/gfortran64/lib/libacml.so
```

There is also a file in the same directory called libacml\_mv.so which will be important.

ACML also includes a folder called gfortran64\_mp, which contains files called libacml\_mp.so and libacml\_mv.so. This is a version of ACML for use with OpenMP compilers (instead of MPICH2 or Open64). Reference: “OpenMP versions of ACML now include _mp as part of the library name (e.g. libacml\_mp.so instead of libacml.so) to make it easier to link to the appropriate version and avoid internal soname clashes.”
- http://developer.amd.com/assets/ReleaseNotes.txt
An example configuration using MPICH2 and ACML may look like this:
```
$ ./configure PETSC_ARCH=petsc1 --with-blas-lapack-dir=/opt/acml4.3.0/gfortran64/lib –with-mpi-dir=/usr/lib64/mpich2
```_

The references to acml and acml\_mv refer to the libraries libacml.so and libacml\_mv.so
## Distribution Provided ##
Your Linux distribution might also have BLAS and Lapack packages installed from your distribution provdier. To find out if they are installed, type:
```
$ rpm -ql blas | grep lib
$ rpm -ql lapack | grep lib
```

On my system, this returned:

```
$ rpm -ql blas | grep lib
/usr/lib64/libblas.so.3
/usr/lib64/libblas.so.3.2
/usr/lib64/libblas.so.3.2.1
$ rpm -ql lapack | grep lib
/usr/lib64/liblapack.so.3
/usr/lib64/liblapack.so.3.2
/usr/lib64/liblapack.so.3.2.1
```

We can configure PETSc using these distribution provided packages. An example configuration might look like:

```
$ ./configure PETSC_ARCH=petsc1 --with-blas-lapack-dir=/usr/lib64/libblas.a –with-mpi-dir=/usr/lib64/mpich2
```

# Configuring PETSc #

At this point, we are ready to configure PETSc and run LOBPCG. We have chosen between two versions of MPI: Open64 (OpenMPI) and MPICH2. And we have chosen between two versions of BLAS/Lapack: ACML version and distribution provided version.

There are also a few other options we must specify:

## PETSC\_DIR and PETSC\_ARCH ##

PETSC\_DIR and PETSC\_ARCH are environment variables that allow us to install different configurations of PETSc on the same hard drive. PETSC\_DIR specifies the location of the directory in which we install PETSC, and PETSC\_ARCH is a unique name given to each PETSc configuration. The PETSC\_ARCH variable allows us to do multiple configurations of PETSc in the same directory.

To set these PETSC\_DIR and PETSC\_ARCH as environment variables before configuration, type for example:

```
$ export PETSC_ARCH=petsc4

$ export PETSC_DIR=$PWD
```

## Hypre ##

We can have PETSc install Hypre using the configuration option

```
--download-hypre=1
```

If we already have Hypre installed, we can tell PETSc where this install is. For example:

```
–-with-hypre-dir=/home/local/brysmith/hypre2/src/hypre
```

Note: the Hypre directory that we give to PETSc contains subdirectories called “include” and “lib”. This is what we must pass to PETSc.

## BLOPEX ##

We can have PETSc download and install BLOPEX using the configuration option

```
--download-blopex=1
```

We can also specify a directory where the BLOPEX PETSc abstact files are already installed:

```
--with-blopex-dir=<dir>
```

We may want to configure PETSc with the newest release of BLOPEX. To do this, download the files:


http://blopex.googlecode.com/files/blopex_petsc_abstract.tar.gz

http://blopex.googlecode.com/files/blopex_petsc_interface.tar.gz

Copy or move these files into your PETSc root directory.

Unzip the blopex\_petsc\_interface.tar.gz file.

Configure PETSc using the option:

```
--download-blopex=$PETSC_DIR/blopex_petsc_abstract.tar.gz 
```

(This is assuming we have already set the environment variable $PETSC\_DIR. The  blopex\_petsc\_abstract.tar.gz could be in any folder as long as we specify this in the above configuration option.

## Complex numbers and 64 bit integers ##

We can specify that we wish to use complex numbers and/or 64 bit integers in the configuration, using the following options respectively:

```
--with-scalar-type=complex
--with-64-bit-indices
```

Note that Hypre does not support complex scalars or 64 bit indices, so do not use these options if you are including Hypre.

## Further Considerations ##

**Before configuring PETSc, there is one more very important step if we are using ACML libraries. We need to reference the ACML directory in the environment variable LD\_LIBRARY\_PATH. LD\_LIBRARY\_PATH is an environment variable with a list of directories where libraries will be search for before searching in the other environment directories. To specify the ACML directory in LD\_LIBRARY\_PATH, type:**

```
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/acml4.3.0/gfortran64/lib
```

(replace /opt/acml4.3.0/gfortran64/lib with your ACML directory)

Adding the ACML directory to the LD\_LIBRARY\_PATH variable must be done each time you log in to use PETSc with ACML.

## Example Configurations: ##

Assuming you are in the root directory of your PETSc install:


**MPICH2 / ACML / Hypre / Double / 32 bit indices**

```
$ tar -xzvf blopex_petsc_interface.tar.gz

$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/acml4.3.0/gfortran64/lib

$ export PETSC_ARCH=petsc7

$ export PETSC_DIR=$PWD

$ ./configure --with-blas-lapack-dir=/opt/acml4.3.0/gfortran64/lib --download-blopex=$PETSC_DIR/blopex_petsc_abstract.tar.gz --with-hypre-dir=$BX/hypre1/src/hypre

$ make all

$ mpd &

$ make test

$ cd src/contrib/blopex/driver

$ make driver

$ ./driver

$ mpirun -np 8 ./driver -ksp_type preonly -pc_type hypre -pc_hypre_type boomeramg
```

**MPICH2 / ACML / no Hypre / Complex / 64 bit indices**

**XXX There is an error with this install at the “make driver” line.**
driver.c:33:28: error: fortran\_matrix.h: No such file or directory

```
$ tar -xzvf blopex_petsc_interface.tar.gz

$ export PETSC_ARCH=petsc8

$ export PETSC_DIR=$PWD

$ locate libacml.so

$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/acml4.3.0/gfortran64/lib

$ ./configure --with-blas-lapack-dir=/opt/acml4.3.0/gfortran64/lib --with-download-blopex=$PETSC_DIR/blopex_petsc_abstract.tar.gz --with-scalar-type=complex –with-64-bit-indices

$ make all

$ make test

$ cd src/contrib/blopex/driver

$ make driver
```


**Openmpi / ACML / Hypre / Double / 32 bit indices**

**XXX There is an error with this install at the “make driver” line.**
driver.o: In function `main':
/home/local/brysmith/petsc9/src/contrib/blopex/driver/driver.c:275: undefined reference to `LOBPCG\_InitRandomContext'

```
$ tar -xzvf blopex_petsc_interface.tar.gz

$ module unload mpich2-x86_64

$ module load openmpi-x86_64

$ which mpicc

$ locate libacml.so

$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/acml4.3.0/gfortran64/lib

$ export PETSC_DIR=$PWD

$ export PETSC_ARCH=petsc9

$ ./configure --with-blas-lapack-dir=/opt/acml4.3.0/gfortran64/lib --with-hypre-dir=$BX/hypre2/src/hypre --with-download-blopex=$PWD/blopex_petsc_abstract.tar.gz

$ make all

$ make test

$ cd src/contrib/blopex/driver

$ make driver

```


**Openmpi / ACML / No Hypre / Complex / 64 bit indices**

```
$ tar -xzvf blopex_petsc_interface.tar.gz

$ export PETSC_ARCH=petsc10

$ export PETSC_DIR=$PWD

$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/acml4.3.0/gfortran64/lib

$ module unload mpich2-x86_64

$ module load openmpi-x86_64

$ which mpicc

$ ./configure --with-blas-lapack-dir=/opt/acml4.3.0/gfortran64/lib --download-blopex=$PWD/blopex_petsc_abstract.tar.gz --with-scalar-type=complex --with-64-bit-indices

$ make all

$ make test

$ cd src/contrib/blopex/driver

$ make driver

$ mpirun -np 8 ./driver
```