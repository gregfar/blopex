

# Introduction #

How to test BLOPEX under Beta version of Hypre on Bluefire (IBM).  Hypre does not support complex arithematic, so this test is only for double precision arithematic.
# Setup Hypre #

Goto https://computation.llnl.gov/casc/hypre/software.html and download hypre.
For example the latest beta version of hypre is hypre-2.6.0b.tar.gz

Extract hypre files from download file.
```
be1105en$ gunzip -c hypre-2.6.0b.tar.gz | tar -xof -
```

Download the lobpcg modifications from http://code.google.com/p/blopex/downloads/list to your home directory.

Change working directory to hypre source code directory.
```
be1105en$ cd hypre-2.6.0b/src
be1105en$ pwd
/blhome/dmccuan/hypre-2.6.0b/src
```
Install the logpcg modifications.
```
be1105en$ gunzip -c $HOME/hypre_lobpcg_modifications.tgz | tar -xof -
```
While in the src subdirectory, install hypre with standard setup.
Note: this will compile the BLOPEX source.
Note: the ./nopoe is required for IBM platforms (see Hypre user's manual, Chapter 7.2).
```
be1105en$ ./nopoe ./configure
........  lots of output

be1105en$ make
........  lots of output
```

# Compile Tests #
Shift to test directory execute make to compile and link test cases.
This creates executables struct and ij.
```
be1105en$ cd test
be1105en$ pwd
/blhome/dmccuan/hypre-2.6.0b/src/test
be1105en$ make
......
Building new_ij ...
        mpcc -o new_ij new_ij.o -L/blhome/dmccuan/hypre-2.0.0/src/hypre/lib -lHYPRE -L/blhome/dmccuan/hypre-2.0.0/src/hypre/lib -lHYPRE_utilities  -L/blhome/dmccuan/hypre-2.0.0/src/hypre/lib -lHYPRE_utilities       -lm  -llapi_r -L/usr/lpp/ppe.poe/lib/threads -L/usr/lpp/ppe.poe/lib -L/lib/threads -lmpi_r -lxlf90 -L/usr/lpp/xlf/lib -lxlopt -lxlf -lxlomp_ser -lpthreads -lm    -L/usr/lib/gcc-lib/i386-redhat-linux/3.2.3 -L/usr/lib -L/usr/local/lib -L/usr/apps/lib -L/lib  -blpdata
        mpcc -O3 -qstrict -blpdata -I. -I./.. -I/blhome/dmccuan/hypre-2.0.0/src/hypre/include   -DHYPRE_TIMING -DHYPRE_FORTRAN -DHAVE_CONFIG_H -c sstruct.c 
......
Target "all" is up to date.
```
# Execution of Tests #
Bluefire must execute mpi jobs unders the Load Sharing Facility (LSF).
Jobs are submitted via command bsub.  The easiest way to do this is to setup a script as follows.
Note: You must have a project number to do this.
```
be1105en$ cat mpi_struct.lsf
#!/bin/csh
#
# LSF batch script to run an MPI application
#
#BSUB -P 37481005                   # project number
##BSUB -x                            # exlusive use of node (not_shared)
#BSUB -W 2:00                       # wall clock time (in minutes)
#BSUB -n 64                         # number of tasks
#BSUB -a poe                        # select the poe elim
##BSUB -R "span[ptile=64]"           # run 64 task per node
#BSUB -J mpi_struct                 # job name
#BSUB -o mpi_struct.%J.out          # output filename
#BSUB -e mpi_struct.%J.err          # error filename 
#BSUB -q share                      # queue

# set this env for launch as default binding
setenv TARGET_CPU_LIST "-1"
 mpirun.lsf /usr/local/bin/launch ./struct -solver 10 -n 64 64 64 -P 4 4 4 -lobpcg -pcgitr 0
```
And then execute it as follows:
```
be1105en$ bsub < mpi_struct.lsf
Job <271766> is submitted to queue <share>.
```
Jobs can be monitored via command bjobs as follows:
```
be1105en$ bjobs
JOBID   USER    STAT  QUEUE      FROM_HOST   EXEC_HOST   JOB_NAME   SUBMIT_TIME
271793  dmccuan PEND  share      be1105en                mpi_ij     Apr 28 16:13
```
Output is returned to current directory in files as defined in the script.
```
be1105en$ ls -l mpi_struct*
-rw-r--r--    1 dmccuan  univ            431 Apr 28 16:12 mpi_struct.271766.err
-rw-r--r--    1 dmccuan  univ          15929 Apr 28 16:12 mpi_struct.271766.out
-rw-r--r--    1 dmccuan  univ            776 Apr 28 16:05 mpi_struct.lsf
```

# Test Results #

The test is executed on UCAR Bluefire system in May 2010.  This is an AIX operating system and the IBM XL C/C++ compiler (xlc).

## STRUCT test ##
```
be1005en$ cat mpi_struct.lsf
#!/bin/csh
#
# LSF batch script to run an MPI application
#
#BSUB -P 37481005                   # project number
##BSUB -x                            # exlusive use of node (not_shared)
#BSUB -W 10:00                      # wall clock time (in minutes)
#BSUB -n 64                         # number of tasks
#BSUB -a poe                        # select the poe elim
##BSUB -R "span[ptile=64]"           # run 64 task per node
#BSUB -J mpi_struct                # job name
#BSUB -o mpi_struct.%J.out          # output filename
#BSUB -e mpi_struct.%J.err          # error filename 
#BSUB -q share                      # queue

# set this env for launch as default binding
setenv TARGET_CPU_LIST "-1"
 mpirun.lsf /usr/local/bin/launch ./struct -solver 10 -n 64 64 64 -P 4 4 4 -lobpcg  -pcgitr 0 
```

The eigenvalues computed are
```
Eigenvalue lambda       Residual              
4.48279981743157e-04    2.62909485966864e-07
```

BLOPEX execution times
```
6 iterations
=============================================
Solve phase times:
=============================================
PCG Solve:
  wall clock time = 4.630000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 4.609700 seconds
  cpu MFLOPS      = 0.000000
```

## IJ test ##

```
be1005en$ cat mpi_ij.lsf
#!/bin/csh
#
# LSF batch script to run an MPI application
#
#BSUB -P 37481005                   # project number
##BSUB -x                            # exlusive use of node (not_shared)
#BSUB -W 10:00                      # wall clock time (in minutes)
#BSUB -n 10                         # number of tasks
#BSUB -a poe                        # select the poe elim
##BSUB -R "span[ptile=64]"           # run 64 task per node
#BSUB -J mpi_ij                 # job name
#BSUB -o mpi_ij.%J.out          # output filename
#BSUB -e mpi_ij.%J.err          # error filename 
#BSUB -q share                      # queue

# set this env for launch as default binding
setenv TARGET_CPU_LIST "-1"
 mpirun.lsf /usr/local/bin/launch ./ij -lobpcg -n 50 50 50 -pcgitr 0 -vrand 96 -seed 1
```

The first few eigenvalues computed are
```
Eigenvalue lambda       Residual              
  1.13800275777369e-02    4.89532361198085e-09
  2.27456657079491e-02    5.23651180092394e-09
  2.27456657079526e-02    5.54157538576400e-09
  2.27456657079561e-02    4.46313137226598e-09
  3.41113038381640e-02    3.34392903105364e-09
  3.41113038381707e-02    3.59684589893089e-09
  3.41113038381710e-02    2.16836581877328e-09
  4.16404856840192e-02    8.37499996905196e-09
  4.16404856840206e-02    8.65544066661302e-09
  4.16404856840226e-02    9.56972716502999e-10
  4.54769419683833e-02    2.45794568105811e-09
  5.30061238142314e-02    8.90985834786345e-09
  5.30061238142333e-02    7.72720854716801e-09
  5.30061238142363e-02    8.01016025969816e-09
  5.30061238142385e-02    8.50898851924983e-09
  5.30061238142395e-02    5.22601299093058e-09
```

BLOPEX execution times
```
100 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 114.460000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 113.715337 seconds
  cpu MFLOPS      = 0.000000
```

## ex11.c test ##

This test was added for hypre-2.6.0b and is located in directory src/examples.  The Makefile there is setup for linux and is not maintained for other operating systems.  The simplest way to compiles and run this is to move it to directory src/test and modify the Makefile there (which does support aix) to compile ex11.  Add the following lines to Makefile using your favorite editor. Note the 2nd and 3rd line must begin with a tab (not spaces).  This is the standard Makefile convention.
```
ex11: fij_mv.o fparcsr_mv.o fparcsr_ls.o ex11.o
        @echo "Building" $@ "... "
        ${LINK_CC} -o $@ fij_mv.o fparcsr_mv.o fparcsr_ls.o $@.o ${FFLAGS} ${LFLAGS}
```

Compile ex11.c
```
>>make ex11
```

And test with the following lsf file.
```
be1005en$ cat mpi_ex11.lsf
#!/bin/csh
#
# LSF batch script to run an MPI application
#
#BSUB -P 37481005                   # project number
##BSUB -x                            # exlusive use of node (not_shared)
#BSUB -W 10:00                      # wall clock time (in minutes)
#BSUB -n 4                         # number of tasks
#BSUB -a poe                        # select the poe elim
##BSUB -R "span[ptile=64]"           # run 64 task per node
#BSUB -J mpi_ex11                 # job name
#BSUB -o mpi_ex11.%J.out          # output filename
#BSUB -e mpi_ex11.%J.err          # error filename 
#BSUB -q share                      # queue

# set this env for launch as default binding
setenv TARGET_CPU_LIST "-1"
 mpirun.lsf /usr/local/bin/launch ./ex11
```

The eigenvalues computed are
```
Eigenvalue lambda       Residual
  1.70632948198618e-02    7.10733425907195e-09
  4.25854480421248e-02    5.99896497717024e-09
  4.25854480421289e-02    3.24051811834258e-09
  6.81076012643905e-02    2.99575007646128e-09
  8.48803610642906e-02    3.19037556069348e-09
  8.48803610642927e-02    4.70316798144029e-09
  1.10402514286553e-01    3.86575096165323e-09
  1.10402514286557e-01    4.36439246105009e-09
  1.43587188599757e-01    7.13116137834407e-09
  1.43587188601203e-01    8.81195266672916e-09
```

BLOPEX execution times
```
28 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 0.190000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 0.170845 seconds
  cpu MFLOPS      = 0.000000
```