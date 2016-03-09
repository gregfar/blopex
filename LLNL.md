# Introduction #

Rico has (and Andrew should have) accounts on LLNL machines: ATLAS, ZEUS, YANA.
Here we describe the specifics of LLNL configure and testing.

# Details #

According to https://computing.llnl.gov/?set=resources&page=OCF_resources ATLAS, ZEUS, YANA all are essentially the same hardware (dual core AMD opteron Proc 8216) with 64 bit memory addressing. Each node has 8 CPUs and 16 GB of RAM (32 GB for yana60-79). They all run CHAOS linux which is the LLNL version of redhat.

  * ATLAS     1152 nodes, high speed interconnect
  * ZEUS      288 nodes, high speed interconnect

YANA is not a cluster as it has no interconnect between 80 nodes. 32 GB for yana60-79, 16 GB for all other notes. Jobs are limited to a single node because there is no interconnect.

LC Hot Line: 1-925-422-4531.

Use msub to submit batch jobs: msub file.sh.
Useful commands include: mshare, ju, showq, mdiag, showstart job#, checkjob job#, canceljob job#, news job.lim.atlas. Here is a typical msub script for a batch job (msub tst.sh)
<pre>
#!/bin/csh<br>
#MSUB -l nodes=8<br>
#MSUB -l walltime=25:00<br>
#MSUB -q pbatch<br>
#MSUB -m be<br>
#MSUB -V<br>
#MSUB -o tst.out<br>
#MSUB -A asccasc<br>
##### These are shell commands<br>
date<br>
srun -n64  ./ij -lobpcg -n 150 150 150 -pcgitr 0 -vrand 20 -seed 1<br>
echo 'Done'<br>
</pre>

# Configuring and Compiling #

By MEA 3/25/2010.

The HYPRE release hypre-2.6.0b is configured and compiled  on the LLNL machines ATLAS, ZEUS and YANA. They all run standard LINUX and use the same processor technology so the same install and compile works on all the machines. First download the compressed files and uncompress them according to http://code.google.com/p/blopex/wiki/HypreInstallLinux.
These included both hypre-2.6.0b.tar.gz and hypre\_lobpcg\_modifications.tar.gz.

To recap:
```
wget https://computation.llnl.gov/casc/hypre/download/hypre-2.6.0b.tar.gz
wget http://blopex.googlecode.com/files/hypre_lobpcg_modifications.tar.gz
tar xzvf hypre-2.6.0b.tar.gz 
cd hypre-2.6.0b/src
tar xzvf .../hypre_lobpcg_modifications.tar.gz
```

Testing is done using two different math libraries: 1) the standard BLAS/LAPACK libraries provided with hypre, 2) the ACML - AMD Core Math Library. See
https://computing.llnl.gov/tutorials/linux_clusters/#Software.

Create two directory structures: hypre-2.6.0b-stdlib and hypre-2.6.0b-amdlib.
First setup the standard libary:
```
$ cd hypre-2.6.0b-stdlib/src
$ configure
$ make
$ make test 
```
Next setup the AMD library:
```
$ cd hypre-2.6.0b-amdlib/src
$ configure --with-lapack-libs="acml acml_mv m" --with-lapack-lib-dirs="/usr/local/tools/acml-gfortran/lib"
$ make
$ make test 
```
To run hypre lobpcg with the AMD libraries you need to define an envronmental
variable so the programs can find the shared libraries. You could do this in the
msub script file or in your C Shell startup configuration file.
```
# add path for AMD core math libary
setenv LD_LIBRARY_PATH /usr/local/tools/acml-gfortran/lib
```

NOTE: I believe that there is a way to use configure so that the executables know
where the shared libaries are, but I don't know how to do it.