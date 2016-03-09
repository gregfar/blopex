

# Introduction #

Testing new BLOPEX in Hypre 2.6.0b with the MKL library. BLOPEX LOBPCG computes a number
of smallest eigenpairs utilizing Hypre's IJ, Struct and SStruct interfaces.
Test drivers ij, struct, sstruct in test.
System configuration:

  * Intel(R) Pentium(R) ,4 CPU, 3.06GHz, 3.24 GB of RAM
  * Intel® Math Kernel Library (Intel® MKL) 10.1.1.019

Testing performed by PZ.

# Setup Hypre #

Follow http://code.google.com/p/blopex/wiki/HypreInstallLinux

# Execution of Tests for Hypre's IJ interface #
Find 20 smallest eigenpairs of 3-D 7-point 64x64x64 Laplacian. Use 2 cores, random guess
and BoomerAMG (default) preconditioner.
```
$ mpirun -np 2 ./ij -lobpcg -n 64 64 64 -pcgitr 0 -vrand 20 -seed 1
........
Initial Max. Residual   2.43837832680518e+00

Iteration 1 	bsize 20 	maxres   9.27760154340507e-01

Iteration 2 	bsize 20 	maxres   9.95859757156809e-02

Iteration 3 	bsize 20 	maxres   5.35809124515939e-02

........
eigenvalue lambda             Residual              

  7.00663900604045e-03    3.11322837230447e-09

  1.40078232353874e-02    4.57818548828887e-09

  1.40078232353958e-02    1.62566134872779e-09

  1.40078232353963e-02    2.14889545451487e-09

........
eigenvalue lambda             Residual   
3.96606622268333e-02    9.32872005531149e-09

3.96606622273050e-02    7.51110116488256e-09


35 iterations
=============================================
=============================================

Solve phase times:

=============================================

LOBPCG Solve:

  wall clock time = 287.250000 seconds

  wall MFLOPS     = 0.000000

  cpu clock time  = 280.990000 seconds

  cpu MFLOPS      = 0.000000

```
the test the desired tolerance for residual norms (1e-8) was achieved at each eigenpair

# Execution of Tests for Hypre's Struct interface #
Find 20 smallest eigenpairs of 3-D 7-point 64x64x64 Laplacian. Use 2 cores, random guess
and SMG (default) preconditioner.
```
$ mpirun -np 2 ./struct -lobpcg -P 2 1 1 -n 32 64 64 -pcgitr 0 -tol 1.e-6 -vrand 20 -seed 1
........
Eigenvalue lambda             Residual              

  7.00663900604139e-03    7.56594276478264e-08

  1.40078232353955e-02    3.76989345154822e-07

  1.40078232354047e-02    7.15309221225593e-07

........
Eigenvalue lambda             Residual  
 3.26594779980024e-02    4.54729654862343e-07

  3.26594779981157e-02    6.77070667719083e-07

  3.26594779983479e-02    5.43680775562528e-07

  3.96606622279992e-02    6.77906637333927e-07

  3.96606622283168e-02    6.30564953621517e-07

  3.96606622285524e-02    6.65041623721332e-07


38 iterations

=============================================

Solve phase times:

=============================================

PCG Solve:

  wall clock time = 507.470000 seconds

  wall MFLOPS     = 0.000000

  cpu clock time  = 456.300000 seconds

  cpu MFLOPS      = 0.000000

```
In the test the desired tolerance for residual norms (1e-6) was achieved at each eigenpair.

# Execution of Tests for Hypre's SStruct interface #
To test Hypre's SStruct interface we use the default input file sstuct.in.default (can be found in src/test/TEST\_sstruct and copied to src/test). We compute 10 smallest eigenpairs using 2 cores, random initial guess and SMG preconditioner:
```
$ mpirun -np 2 ./sstruct -P 1 1 2 -lobpcg -solver 10 -tol 1.e-6 -pcgitr 0 -seed 1 -vrand 10
Eigenvalue lambda   1.34880848246833e+00

Eigenvalue lambda   1.36560847743323e+00

Eigenvalue lambda   1.47908699641570e+00

Eigenvalue lambda   1.49589925872234e+00

Eigenvalue lambda   1.69420716798715e+00

Eigenvalue lambda   1.71104021853950e+00

Eigenvalue lambda   1.78666772443481e+00

Eigenvalue lambda   1.81237285962598e+00

Eigenvalue lambda   1.82920488646797e+00

Eigenvalue lambda   1.84668108358806e+00

Residual   8.63315718464232e-07

Residual   7.12441385694004e-07

Residual   8.01558487410104e-07

Residual   8.38541144282045e-07

Residual   8.83898725273333e-07

Residual   9.24912236823651e-07

Residual   8.69609164824751e-07

Residual   9.25491794233026e-07

Residual   8.43603160538818e-07

Residual   1.01253606865980e-06



100 iterations

=============================================

Solve phase times:

=============================================

LOBPCG Solve:

  wall clock time = 34.420000 seconds

  wall MFLOPS     = 0.000000

  cpu clock time  = 33.180000 seconds

  cpu MFLOPS      = 0.000000

```

In the test,  the desired tolerance (1e-6) for residual norms was achieved at each eigenpair.