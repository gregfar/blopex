# Introduction #

How To install PETSc with AMD's ACML in Fedora12 64 bit
(assuming bash shell)

# Details #

The basic system info:

```
>> uname -a
Linux opt 2.6.31.12-174.2.22.fc12.x86_64 #1 SMP Fri Feb 19 18:55:03 UTC 2010 x86_64 x86_64 x86_64 GNU/Linux
>> rpm -q openmpi
openmpi-1.3.3-6.fc12.x86_64
>> rpm -q mpich2
mpich2-1.2.1-2.fc12.x86_64
```

Download from
http://developer.amd.com/cpu/Libraries/acml/downloads/pages/default.aspx
the latest ACML libraries, in my case:
  * http://developer.amd.com/Downloads/acml-4-4-0-gfortran-64bit.tgz
  * http://developer.amd.com/Downloads/acml-4-4-0-gfortran-64bit-int64.tgz

If you use AMD's open64 compiler rather then GNU-FORTRAN/C, you need these:
  * http://developer.amd.com/Downloads/acml-4-4-0-open64-64bit.tgz
  * http://developer.amd.com/Downloads/acml-4-4-0-open64-64bit-int64.tgz

Install each of them into a SEPARATE local directory, i.e.,
provide different directory names when asked during the installation.
In my case, the directories are:
  * /opt/acml-4-4-0-gfortran-64bit
  * /opt/acml-4-4-0-gfortran-64bit-int64
  * /opt/acml-4-4-0-open64-64bit
  * /opt/acml-4-4-0-open64-64bit-int64
correspondingly.

Each library comes in two different flavours, regular and mp, e.g.,
  * /opt/acml-4-4-0-gfortran-64bit/gfortran64/lib
  * /opt/acml-4-4-0-gfortran-64bit/gfortran64\_mp/lib

According to Ch. 2 of the ACML manual, see
http://developer.amd.com/assets/acml_userguide.pdf
one should use the mp version on a multi-CPU/core nodes.
In the notes below, we use the regular version.

The latest ACML may not work on old CPUs!

Follow http://code.google.com/p/blopex/wiki/openmpiandmpich2switchinFedora12
to set up the MPI, e.g., by running

```
>> module load openmpi-x86_64
```

If SELinux is active, disable SELinux temporarily by running (as root)

```
>> echo 0 > /selinux/enforce 
```

After you are done running the codes, restore SELinux by

```
>> echo 1 > /selinux/enforce
```

If you cannot be root, contact the sysadmin.

Download and extract the latest PETSc, at the moment this is
[ftp://info.mcs.anl.gov/pub/petsc/release-snapshots/petsc-lite-3.1.tar.gz](ftp://info.mcs.anl.gov/pub/petsc/release-snapshots/petsc-lite-3.1.tar.gz)

Extract using **tar xzvf** , rename the PETSc dir as you like, cd to PETSc dir and run

```
>> PETSC_DIR=$PWD; export PETSC_DIR
```

Version petsc-3.1-p3 and later already includes the latest BLOPEX code!
To test BLOPEX, run

```
>> ./configure PETSC_ARCH=linux-openmpi-acml-4-4-0-gfortran-64bit --with-blas-lapack-dir=/opt/acml-4-4-0-gfortran-64bit/gfortran64 --download-blopex=1 --download-hypre=1
>> PETSC_ARCH=linux-openmpi-acml4-4-0-gfortran-64bit; export PETSC_ARCH
>> make all
```

Note the Hypre is real and 32-bit-indices only, so an attempt to use
--download-hypre=1 either with --with-scalar-type=complex or with --with-64-bit-indices would give an error.


Assuming no errors on the previous steps, run

```
>> cd src/contrib/blopex/driver
make driver
```

Now, you can execute the driver by using

```
>> mpirun -np 2 -x LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib ./driver
```

Here, both shared libraries /usr/lib64/openmpi/lib and /opt/acml-4-4-0-gfortran-64bit/gfortran64/lib are necessary to specify in the command-line.
Alternatively, you can set

```
>> LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib; export LD_LIBRARY_PATH
```

then you can simply run

```
>> mpirun -np 2 ./driver
```

See http://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html for more info on shared libraries.

<pre>
Here is a complete setup example:<br>
<br>
wget ftp://info.mcs.anl.gov/pub/petsc/release-snapshots/petsc-lite-3.1.tar.gz<br>
tar xzvf petsc-lite-3.1.tar.gz<br>
mv petsc-3.1-p3 petsc-3.1-p3-int64-complex-ACML<br>
cd petsc-3.1-p2-int64-complex-ACML<br>
PETSC_DIR=$PWD; export PETSC_DIR<br>
module load openmpi-x86_64<br>
LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib; export LD_LIBRARY_PATH<br>
./config/configure.py PETSC_ARCH=linux-openmpi-acml-4-4-0-gfortran-64bit --with-blas-lapack-dir=/opt/acml-4-4-0-gfortran-64bit/gfortran64  --with-scalar-type=complex --with-64-bit-indices --download-blopex=1<br>
make all<br>
make test<br>
cd src/contrib/blopex/driver<br>
make driver<br>
mpirun -np 2 ./driver<br>
<br>
</pre>