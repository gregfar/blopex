# Introduction #

openmpi and mpich2 are the two currently most popular MPI software.
They cannot be mixed, only one of them must be used at a time.
How to switch from one to another?
The info below is copied from http://osdir.com/ml/fedora-list/2009-12/msg00591.html

# Check which packages are installed #

Check which packages are installed by running:

```
>> rpm -q openmpi
>> rpm -q openmpi-devel
>> rpm -q mpich2
>> rpm -q mpich2-devel
```

If any of these 4 packages are missing, install them first, e.g.,

```
>> yum install openmpi
```

(need to be root for this)

# (re-)start MPI #

The management is done on user-level on a session basis.
To use the openmpi compiler, just run

```
>> module load openmpi-x86_64
```

on x86\_64 and

```
>> module load openmpi-i686
```

on x86 to get Open MPI working again.

If you have Open MPI support loaded, you can also get rid of it with

```
>>
module unload openmpi-x86_64
```
and switch to MPICH2 with

```
>> module load mpich2-x86_64
```

For mpich2, after the above, you may also need to start the mpich2 daemon by

```
>> mpd&
```

Double-check by running

```
>> which mpicc
>> which mpiCC
```
to make sure that they refer to the same MPI.

# Specify LD\_LIBRARY\_PATH #

Both openmpi and mpich2 are shared libraries, so you need to specify the corresponding
LD\_LIBRARY\_PATH, by running either (for mpich2)

```
>> LD_LIBRARY_PATH=/usr/lib64/mpich2/lib; export LD_LIBRARY_PATH
```

or (for openmpi)

```
>> LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib; export LD_LIBRARY_PATH
```

If you need to use several shared libraries, use the following format (for openmpi and ACML)

```
>> LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/opt/acml-4-4-0-gfortran-64bit/gfortran64/lib; export LD_LIBRARY_PATH
```

# Further notes #

Remember that this setup is only active in the given shell window till you close it.
Make sure that you name differently the directories where you compile and run the
codes. Do not forget to setup the MPI again if you return to the previously compiled
code before you run it!

P.S. RHEL 4 and 5 use the mpi-selector command instead.