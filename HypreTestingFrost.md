

# Introduction #

How to test BLOPEX under Beta version of Hypre on Frost (IBM).  Hypre does not support complex arithematic, so this test is only for double precision arithematic.
# Setup Hypre #

Follow the instructions in wiki HypreTestingIBM.

# Compile Tests #

Follow the instructions in wiki HypreTestingIBM.

# Execution of Tests #

Frost executes mpi jobs under the cobalt batch system.
Jobs are submitted via command cqsub.

For example to submist struct from the change to the .../src/test subdirectory and issue command:
```
cqsub -n 64 -t 00:10:00 -q debug ./struct ...etc...
```

A batch job number is returned (say 894111).  Output is returned to current directory.
Output consists of 894111.cobaltlog, 894111.error, and 894111.output

See http://www.cisl.ucar.edu/docs/frost/cbr.jsp for details.

# Test Results #

The tests were executed on UCAR Frost system in May 2010.  This is an Linux operating system and the IBM blrts\_xlc compiler was used.

## STRUCT test ##

cqsub -n 64 -t 00:10:00 -q debug ./struct -solver 10 -n 64 64 64 -P 4 4 4 -lobpcg -pcgitr 0

The eigenvalues computed are
```
Eigenvalue lambda       Residual              
  4.48279981675279e-04    2.41823048611788e-07
```

BLOPEX execution times
```
6 iterations
=============================================
Solve phase times:
=============================================
PCG Solve:
  wall clock time = 25.100000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 25.100000 seconds
  cpu MFLOPS      = 0.000000
```

## IJ test ##

cqsub -n 10 -t 00:30:00 -q debug ./ij -lobpcg -n 50 50 50 -pcgitr 0 -vrand 96 -seed 1

The first few eigenvalues computed are

```
Eigenvalue lambda       Residual              
  1.13800275777358e-02    4.89532463161163e-09
  2.27456657079501e-02    6.19605677420893e-09
  2.27456657079516e-02    4.26731646682549e-09
  2.27456657079588e-02    4.63123119826148e-09
  3.41113038381629e-02    3.50846601499871e-09
  3.41113038381685e-02    3.59604071613089e-09
  3.41113038381696e-02    1.89213926232985e-09
  4.16404856840148e-02    8.53613292028870e-09
  4.16404856840189e-02    7.37519801165653e-09
  4.16404856840242e-02    1.00515923147608e-09
  4.54769419683858e-02    2.45846450351339e-09
  5.30061238142303e-02    6.15069444321083e-09
  5.30061238142345e-02    7.67982579864841e-09
  5.30061238142364e-02    8.83081502735603e-09
  5.30061238142372e-02    8.57699413710310e-09
  5.30061238142379e-02    8.36257610180726e-09
```

BLOPEX execution times
```
  100 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 1532.010000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 1532.010000 seconds
  cpu MFLOPS      = 0.000000
```