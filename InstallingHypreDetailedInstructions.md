# Downloading Hypre #

Hypre can be downloaded from the Hypre website: http://acts.nersc.gov/hypre/. For a direct download of the version 2.6.0b Hypre tar file, type:

```
$ wget https://computation.llnl.gov/casc/hypre/download/hypre-2.6.0b.tar.gz
```

Then unzip:

```
$ tar -xzvf hypre-2.6.0b.tar.gz
```

Now we can rename the Hypre folder if we choose. E.g.,

```
$ mv hypre-2.6.0b hypre1
```

For version 2.6.0, we have updates to Blopex that we can install. Go to the src directory within Hypre and download and unzip the Blopex modifications tar file. E.g.,

```
$ cd hypre1/src
$ wget http://blopex.googlecode.com/files/hypre_lobpcg_modifications.tar.gz
$ tar -xzvf hypre_lobpcg_modifications.tar.gz
```

# Configuration Options #

There are many options we can specify for configuring Hypre. For a list of these options, in the src directory of your Hypre install, type:

```
$ ./configure –help
```

We will be mainly concerned with selecting versions of MPI and BLAS/Lapack.


## Selecting a version of MPI: ##

There are two main versions of MPI we will use here: MPICH2 and OpenMPI. You can find out if either of these are installed by typing:

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

### Open64 (OpenMPI) ###

For Open64, the official page is: http://developer.amd.com/cpu/open64/pages/default.aspx. If you choose this option, you just need to download and unpack the Open64 files. For example:

```
$ wget http://download2-developer.amd.com/amd/open64/x86_open64-4.2.4-1.x86_64.tar.bz2
$ tar xjvf x86_open64-4.2.4-1.x86_64.tar.bz2
$ mv x86_open64-4.2.4 ../x86_open64
$ rpm -q openmpi
$ rpm -q openmpi-devel
```

You should now see that OpenMPI is installed.

### OpenMPI ###

OpenMPI can be downloaded by itself from the official website: http://www.open-mpi.org/. Installation instructions are included in the README file.

### MPICH2 ###

MPICH2 may also be downloaded form the official website: http://www.mcs.anl.gov/research/projects/mpich2/. Installation instructions are also included in the README file.

### Switching Between MPI's ###

Now that we have one or both versions of MPI installed, we can install Hypre. The default configuration of Hypre will use the version of MPI currently stored in your environment variables. To find out which this is, type:

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

Note: It is a good idea to unload the existing MPI module before loading the new one, otherwise some compilers may still be associated with the first MPI module.

Double check that the desired MPI module is loaded:

```
$ env | grep MPI
$ $LD_LIBRARY_PATH
$ which mpicc
```

Each of these commands should give a reference to the desired version of MPI.

You will have to load the correct MPI module for your Hypre install each time you log in.


## Specifying BLAS/Lapack Libraries: ##

BLAS and Lapack are software libraries for numerical linear algebra that are used by Hypre. If one does not specify the location of BLAS/Lapack libraries in the Hypre install, then Hypre will automatically install these libraries. You may want to specify BLAS/Lapack libraries if they already exist on your computer.

AMD and Intel put out math libraries which contain versions of BLAS and Lapack. The AMD version is called ACML and the Intel version is called MKL. There also might be precompiled libraries provided with your distribution of Linux.

### ACML ###

On AMD systems, to see if ACML is installed, type:

```
$ locate libacml.so
```

This will show the directory where the ACML libraries are stored. In my case, we have

```
/opt/acml4.3.0/gfortran64/lib/libacml.so
```

There is also a file in the same directory called libacml\_mv.so which will be important.

ACML also includes a folder called gfortran64\_mp, which contains files called libacml\_mp.so and libacml\_mv.so. This is a version of ACML for use with OpenMP compilers (instead of MPICH2 or Open64). Reference: "OpenMP versions of ACML now include _mp as part of the library name (e.g. libacml\_mp.so instead of libacml.so) to make it easier to link to the appropriate version and avoid internal soname clashes."
- http://developer.amd.com/assets/ReleaseNotes.txt
An example configuration using MPICH2 and ACML may look like this:
```
./configure --with-lapack-libs="acml acml_mv" –with-lapack-lib-dirs="/opt/acml4.3.0/gfortran64/lib"
```_

The references to acml and acml\_mv refer to the libraries libacml.so and libacml\_mv.so
### Distribution Provided ###
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

We can configure Hypre using these distribution provided packages. An example configuration might look like:

```
$ ./configure --with-lapack-libs="lapack blas" –with-lapack-lib-dirs="/usr/lib64"
```

# Configuring Hypre #

At this point, we are ready to configure Hypre and run LOBPCG. We have chosen between two versions of MPI: Open64 (OpenMPI) and MPICH2. And we have chosen between two versions of BLAS/Lapack: ACML version and distribution provided version, and also we will let Hypre install it's own BLAS/Lapack libraries. Below are 5 example configurations using different options for MPI and BLAS/Lapack.

**Before configuring Hypre, there is one more very important step if we are using ACML libraries.** We need to reference the ACML directory in the environment variable LD\_LIBRARY\_PATH. LD\_LIBRARY\_PATH is an environment variable with a list of directories where libraries will be search for before searching in the other environment directories. To specify the ACML directory in LD\_LIBRARY\_PATH, type:

```
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/acml4.3.0/gfortran64/lib
```

(replace /opt/acml4.3.0/gfortran64/lib with your ACML directory)

Adding the ACML directory to the LD\_LIBRARY\_PATH variable must be done each time you log in to use Hypre with ACML.

### Example Configurations: ###

Assuming you are in the src directory of your Hypre install:

**MPICH2 / ACML**

```
$ which mpicc
/usr/lib64/mpich2/bin/mpicc
$ locate libacml.so
/opt/acml4.2.0/gfortran64/lib/libacml.so
/opt/acml4.3.0/gfortran64/lib/libacml.so
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/acml4.3.0/gfortran64/lib
$ ./configure --with-lapack-libs="acml acml_mv" --with-lapack-lib-dirs="/opt/acml4.3.0/gfortran64/lib"
$ make
$ make test
$ cd test
$ mpirun -np 8 ./ij -lobpcg
```


**OpenMPI / ACML**

```
$ module unload mpich2-x86_64
$ module load openmpi-x86_64
$ which mpicc
/usr/lib64/openmpi/bin/mpicc
$ locate libacml.so
/opt/acml4.2.0/gfortran64/lib/libacml.so
/opt/acml4.3.0/gfortran64/lib/libacml.so
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/acml4.3.0/gfortran64/lib
$ ./configure --with-lapack-libs="acml acml_mv" –with-lapack-lib-dirs="/opt/acml4.3.0/gfortran64/lib"
$ make
$ make test
$ cd test
$ mpirun -np 8 ./ij -lobpcg
```

**MPICH2 / Distribution Libraries**

```
$ which mpicc
/usr/lib64/mpich2/bin/mpicc
$ rpm -ql blas | grep lib
/usr/lib64/libblas.so.3
/usr/lib64/libblas.so.3.2
/usr/lib64/libblas.so.3.2.1
$ rpm -ql lapack | grep lib
/usr/lib64/liblapack.so.3
/usr/lib64/liblapack.so.3.2
/usr/lib64/liblapack.so.3.2.1
$ ./configure --with-lapack-libs="lapack blas" --with-lapack-lib-dirs="/usr/lib64"
$ make
$ make test
$ cd test
$ mpirun -np 8 ./ij -lobpcg
```

**OpenMPI / Distribution Libraries**

```
$ module unload mpich2-x86_64
$ module load openmpi-x86_64
$ which mpicc
/usr/lib64/openmpi/bin/mpicc
$ rpm -ql blas | grep lib
/usr/lib64/libblas.so.3
/usr/lib64/libblas.so.3.2
/usr/lib64/libblas.so.3.2.1
$ rpm -ql lapack | grep lib
/usr/lib64/liblapack.so.3
/usr/lib64/liblapack.so.3.2
/usr/lib64/liblapack.so.3.2.1
$ ./configure --with-lapack-libs="lapack blas" --with-lapack-lib-dirs="/usr/lib64"
$ make
$ make test
$ cd test
$ mpirun -np 8 ./ij -lobpcg
```

**MPICH2 / Hypre Installed BLAS/LAPACK**

```
$ which mpicc
/usr/lib64/mpich2/bin/mpicc
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/acml4.3.0/gfortran64/lib
$ ./configure
$ make
$ make test
$ cd test
$ mpirun -np 8 ./ij -lobpcg
```