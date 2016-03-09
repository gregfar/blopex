

# Introduction #

Testing new BLOPEX in Hypre 2.6.0b. BLOPEX LOBPCG computes a number of smallest eigenpairs utilizing Hypre's IJ, Struct and SStruct interfaces.
Test drivers ij, struct, sstruct in test and ex11 in examples.


# Setup Hypre #

See
http://code.google.com/p/blopex/wiki/LLNL

All of these test were done using the LLNL ATLAS system: 8 AMD Opteron processors and 16G of memory per node, 64bit memory addressing, a total of 1152 nodes and 9216 processors. These tests use the standard BLAS/LAPACK libraries provided with hypre.

Here is a typical msub script:
```
#!/bin/csh
#MSUB -l nodes=1
#MSUB -l walltime=10:00
#MSUB -q pbatch
#MSUB -m be
#MSUB -V
#MSUB -o test_hypre1.out
#MSUB -A asccasc
##### These are shell commands
date
srun -n2  ij -lobpcg -n 64 64 64 -pcgitr 0 -vrand 20 -seed 1
echo 'Done'
```
Lines #2, #3 and #11 are changed for each run.

# Execution of Tests for Hypre's IJ interface #
Find 20 smallest eigenpairs of 3-D 7-point 64x64x64 Laplacian. Use 2 cores, random guess and BoomerAMG (default) preconditioner.
```
$ srun -n2  ij -lobpcg -n 64 64 64 -pcgitr 0 -vrand 20 -seed 1
........
Initial Max. Residual   2.43837832680517e+00
Iteration 1 	bsize 20 	maxres   9.27760154340509e-01
Iteration 2 	bsize 20 	maxres   9.95859757156789e-02
Iteration 3 	bsize 20 	maxres   5.35809124539832e-02
........
Iteration 32 	bsize 3 	maxres   1.63915213644935e-08
Iteration 33 	bsize 2 	maxres   1.00061006177016e-08
Iteration 34 	bsize 1 	maxres   9.79423242872933e-09

Eigenvalue lambda       Residual              
  7.00663900604040e-03    3.11322750992554e-09
  1.40078232353962e-02    2.18555493423167e-09
  1.40078232353966e-02    1.56740554270983e-09
........
  3.96606622255917e-02    9.79423242872933e-09
  3.96606622273213e-02    7.52027698802309e-09
  3.96606622335298e-02    7.01148270739451e-09

34 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 157.880000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 157.840000 seconds
  cpu MFLOPS      = 0.000000
```

Below are the similar runs and corresponding timings on 4, 8 and 12 cores:
```
srun -n4  ij -lobpcg -n 64 64 64 -pcgitr 0 -vrand 20 -seed 1
........
Initial Max. Residual   2.43707798239336e+00
Iteration 1 	bsize 20 	maxres   9.19578401303560e-01
Iteration 2 	bsize 20 	maxres   1.05575154554308e-01
Iteration 3 	bsize 20 	maxres   4.94189137866536e-02
........
Iteration 39 	bsize 1 	maxres   2.32192704559918e-08
Iteration 40 	bsize 1 	maxres   1.47105047511431e-08
Iteration 41 	bsize 1 	maxres   8.97634657275717e-09

Eigenvalue lambda       Residual              
  7.00663900604048e-03    3.82339243630002e-09
  1.40078232353942e-02    4.64989432090594e-09
  1.40078232353962e-02    4.36956251539866e-09
........  
  3.96606622168279e-02    8.01279743679407e-09
  3.96606622272654e-02    7.83277321700525e-09
  3.96606622273116e-02    8.39957241938649e-09

41 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 76.540000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 76.540000 seconds
  cpu MFLOPS      = 0.000000


srun -n8  ij -lobpcg -n 64 64 64 -pcgitr 0 -vrand 20 -seed 1
........
Initial Max. Residual   2.43975107136868e+00
Iteration 1 	bsize 20 	maxres   9.42509159068712e-01
Iteration 2 	bsize 20 	maxres   1.06547393971217e-01
Iteration 3 	bsize 20 	maxres   4.20822219529653e-02
........
Iteration 33 	bsize 3 	maxres   1.32343428294644e-08
Iteration 34 	bsize 1 	maxres   1.05484409989219e-08
Iteration 35 	bsize 1 	maxres   9.11921415936450e-09

Eigenvalue lambda       Residual              
  7.00663900604047e-03    3.70085899255288e-09
  1.40078232353957e-02    4.90069048034002e-09
  1.40078232353970e-02    1.98359423970836e-09
........ 
  3.96606622273044e-02    9.11921415936450e-09
  3.96606622273126e-02    6.58539147644802e-09
  3.96606622277364e-02    6.55620097895291e-09

35 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 40.970000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 40.970000 seconds
  cpu MFLOPS      = 0.000000

srun -n12  ij -lobpcg -n 64 64 64 -pcgitr 0 -vrand 20 -seed 1
........
Initial Max. Residual   2.43769022193455e+00
Iteration 1 	bsize 20 	maxres   9.36812311008159e-01
Iteration 2 	bsize 20 	maxres   1.11863318144932e-01
Iteration 3 	bsize 20 	maxres   5.01242692640469e-02
........
Iteration 33 	bsize 2 	maxres   1.75108824274872e-08
Iteration 34 	bsize 1 	maxres   1.31438097854918e-08
Iteration 35 	bsize 1 	maxres   9.28416855326690e-09

Eigenvalue lambda       Residual              
  7.00663900603874e-03    1.16582862186407e-09
  1.40078232353951e-02    1.97027315050180e-09
  1.40078232353964e-02    1.02406385727161e-09
........ 
  3.96606622272498e-02    8.57657681057407e-09
  3.96606622272987e-02    9.14301724087940e-09
  3.96606622273175e-02    6.16593293200329e-09

35 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 61.580000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 61.570000 seconds
  cpu MFLOPS      = 0.000000
```

In all the tests the desired tolerance for residual norms (1e-8) was achieved at each eigenpair.

# Execution of Tests for Hypre's Struct interface #
Find 20 smallest eigenpairs of 3-D 7-point 64x64x64 Laplacian. Use 2 cores, random guess and SMG (default) preconditioner.
```
srun -n2 struct -lobpcg -P 2 1 1 -n 32 64 64 -pcgitr 0 -tol 1.e-6 -vrand 20 -seed 1
........
Initial Max. Residual   2.44147768637573e+00
Iteration 1 	bsize 20 	maxres   9.61366917411159e-01
Iteration 2 	bsize 20 	maxres   1.16958182015928e-01
Iteration 3 	bsize 20 	maxres   7.54703862046335e-02
........
Iteration 36 	bsize 1 	maxres   1.26361673376445e-06
Iteration 37 	bsize 1 	maxres   1.01621060896889e-06
Iteration 38 	bsize 1 	maxres   9.77612585752585e-07

Eigenvalue lambda       Residual              
  7.00663900604137e-03    7.56594294724381e-08
  1.40078232354006e-02    3.77279806460283e-07
  1.40078232354045e-02    7.15222192077976e-07
........ 
  3.96606622278755e-02    6.76919987150030e-07
  3.96606622283022e-02    6.36262557074980e-07
  3.96606622286556e-02    6.80569313188676e-07

38 iterations
=============================================
Solve phase times:
=============================================
PCG Solve:
  wall clock time = 210.590000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 210.560000 seconds
  cpu MFLOPS      = 0.000000
```

Below are the similar runs and corresponding timings on 4 and 8 cores:
```
srun -n4 struct -lobpcg -n 64 64 64 -pcgitr 0 -vrand 20 -seed 1
........
Initial Max. Residual   2.44389386670883e+00
Iteration 1 	bsize 20 	maxres   9.08302043327602e-01
Iteration 2 	bsize 20 	maxres   9.93203912671460e-02
Iteration 3 	bsize 20 	maxres   6.50486977171459e-02
........
Iteration 20 	bsize 2 	maxres   2.00702240659784e-06
Iteration 21 	bsize 2 	maxres   1.22272570610140e-06
Iteration 22 	bsize 2 	maxres   9.32297872042726e-07

Eigenvalue lambda       Residual              
  4.82051933120808e-03    4.08419642237034e-07
  5.26877698441874e-03    4.92678783383892e-07
  6.01579864841470e-03    1.03485402240996e-07
........ 
  1.54068271154632e-02    6.79964477667430e-07
  1.67625995014159e-02    6.32181629832286e-07
  1.70492925785677e-02    6.00467774810507e-07

22 iterations
=============================================
Solve phase times:
=============================================
PCG Solve:
  wall clock time = 429.050000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 429.010000 seconds
  cpu MFLOPS      = 0.000000

srun -n8 struct -lobpcg -P 8 1 1 -n 8 64 64 -pcgitr 0 -tol 1.e-6 -vrand 20 -seed 1 
........
Initial Max. Residual   2.44408933274620e+00
Iteration 1 	bsize 20 	maxres   1.14006356546377e+00
Iteration 2 	bsize 20 	maxres   1.49131321672789e-01
Iteration 3 	bsize 20 	maxres   1.32045118727645e-01
........
Iteration 26 	bsize 1 	maxres   1.82839821558345e-06
Iteration 27 	bsize 1 	maxres   1.24712146542881e-06
Iteration 28 	bsize 1 	maxres   9.50723501978925e-07

Eigenvalue lambda       Residual              
  7.00663900604126e-03    9.75486466223631e-08
  1.40078232353970e-02    2.70584840420372e-07
  1.40078232354029e-02    4.31678613414848e-07
........ 
  3.96606622279406e-02    9.50723501978925e-07
  3.96606622282889e-02    8.40847685491894e-07
  3.96606622288377e-02    7.28101790402948e-07

28 iterations
=============================================
Solve phase times:
=============================================
PCG Solve:
  wall clock time = 65.620000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 65.620000 seconds
  cpu MFLOPS      = 0.000000

Done
```

In all the tests the desired tolerance for residual norms (1e-6) was achieved at each eigenpair.

Note: dimensions _-n nx ny nz_ in the driver's parameter list, are assumed to be per processor, while the processors topology is defined by _-P Px Py Pz_.

# Execution of Tests for Hypre's SStruct interface #
To test Hypre's SStruct interface we use the default input file sstuct.in.default (can be found in src/test/TEST\_sstruct and copied to src/test). We compute 10 smallest eigenpairs using 2 cores, random initial guess and SMG preconditioner:
```

srun -n2 sstruct -P 1 1 2 -lobpcg -solver 10 -tol 1.e-6 -pcgitr 0 -seed 1 -vrand 10 
........
Initial Max. Residual   3.66746834197939e+00
Iteration 1 	bsize 10 	maxres   3.95931766889061e+00
Iteration 2 	bsize 10 	maxres   2.52086174796120e+00
Iteration 3 	bsize 10 	maxres   1.82502104263550e+00
........
Iteration 98 	bsize 1 	maxres   1.36386601977662e-06
Iteration 99 	bsize 1 	maxres   1.12325862360037e-06
Iteration 100 	bsize 1 	maxres   1.01253561108829e-06

Eigenvalue lambda       Residual              
  1.34880848246844e+00    8.63315792564494e-07
  1.36560847743383e+00    7.12441389392736e-07
  1.47908699641188e+00    8.01558782275141e-07
........  
  1.81237285967070e+00    9.25492569443323e-07
  1.82920488646799e+00    8.43603474141757e-07
  1.84668108358999e+00    1.01253561108829e-06

100 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 18.640000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 18.630000 seconds
  cpu MFLOPS      = 0.000000
```

Compute 10 smallest eigenpairs using 2 cores, random initial guess and PFMG preconditioner:
```

srun -n2 sstruct -P 1 1 2 -lobpcg -solver 11 -tol 1.e-6 -pcgitr 0 -seed 1 -vrand 10 
........
Initial Max. Residual   3.66746834197939e+00
Iteration 1 	bsize 10 	maxres   4.01329957316064e+00
Iteration 2 	bsize 10 	maxres   2.71181526959483e+00
Iteration 3 	bsize 10 	maxres   1.80491843909126e+00
........
Iteration 98 	bsize 1 	maxres   2.43721630804724e-06
Iteration 99 	bsize 1 	maxres   2.26280601667150e-06
Iteration 100 	bsize 1 	maxres   2.13094586186140e-06

Eigenvalue lambda       Residual              
  1.34880848246839e+00    7.70861393247927e-07
  1.36560847743322e+00    7.60135734276439e-07
  1.47908699641186e+00    9.62472424047735e-07
........ 
  1.81237285961860e+00    9.97971810445109e-07
  1.82920488646791e+00    9.98086964234622e-07
  1.84668108359012e+00    2.13094586186140e-06

100 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 5.190000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 5.190000 seconds
  cpu MFLOPS      = 0.000000
```

For both cases the desired tolerance (1e-6) for residual norms was achieved at each eigenpair.

# Hypre Testing on Larger Problems #
These jobs were run on LLNL using the standard BLAS/LAPACK libraries provided with hypre. The following jobs were run on 64 processors to see if we could increase the size of the problem. Runs #2 and #3 crashed (running out of memory). #4 with AMG tuning parameters from LLNL ran ok and #5 using both of the LLNL recommendations ran ok.
```
1)
srun -n64  ij -n 200 200 200 -P 4 4 4
successful 
2)
srun -n64  ij -n 250 250 250 -P 4 4 4
crashed
3)
srun -n64  ij -n 400 400 400 -P 4 4 4
crashed
4)
srun -n64  ij  -n 400 400 400 -hmis -agg_nl 1 -interptype 6 -Pmx 4
successful 
5)
srun -n64  ij  -n 400 400 400  -P 4 4 4 -hmis -agg_nl 1 -interptype 6 -Pmx 4
successful 
```

Test on LLNL ATLAS system. Execute tests for Hypre's IJ interface. Find 20 smallest eigenpairs of 3-D 7-point 400x400x400 Laplacian. Use 8 nodes and 64 processors, random guess and BoomerAMG (default) preconditioner.
```
srun -n64  ij -lobpcg -n 400 400 400 -pcgitr 0 -vrand 20 -seed 1  -P 4 4 4 -hmis -agg_nl 1 -interptype 6 -Pmx 4
........
  Laplacian:
    (nx, ny, nz) = (400, 400, 400)
    (Px, Py, Pz) = (4, 4, 4)
    (cx, cy, cz) = (1.000000, 1.000000, 1.000000)
........
Initial Max. Residual   2.44739676813972e+00
Iteration 1 	bsize 20 	maxres   8.10036373965025e-01
Iteration 2 	bsize 20 	maxres   2.74010627338399e-02
Iteration 3 	bsize 20 	maxres   2.38522241400591e-02
........
Iteration 98 	bsize 3 	maxres   1.75733759952715e-06
Iteration 99 	bsize 3 	maxres   1.52178814791134e-06
Iteration 100 	bsize 3 	maxres   1.41696513919346e-06
........
Eigenvalue lambda       Residual              
  1.84132323622540e-04    7.88362030113262e-09
  3.68260879920936e-04    6.32103299991598e-09
  3.68260879931142e-04    6.25805840799856e-09
........
  1.04338636286942e-03    1.82336151666657e-08
  1.04338636342027e-03    4.31563373965021e-07
  1.04338637164431e-03    1.41696513919346e-06

100 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 4176.460000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 4175.910000 seconds
  cpu MFLOPS      = 0.000000
```

Test on LLNL ATLAS system. Execute tests for Hypre's IJ interface. Find 20 smallest eigenpairs of 3-D 7-point 500x500x500 Laplacian. Use 64 nodes and 512 processors, random guess and BoomerAMG (default) preconditioner.
```
srun -n512  ij -lobpcg -n 500 500 500 -pcgitr 0 -vrand 20 -seed 1  -P 8 8 8 -hmis -agg_nl 1 -interptype 6 -Pmx 4
........
  Laplacian:
    (nx, ny, nz) = (500, 500, 500)
    (Px, Py, Pz) = (8, 8, 8)
    (cx, cy, cz) = (1.000000, 1.000000, 1.000000)
........
Initial Max. Residual   2.44762003452109e+00
Iteration 1     bsize 20        maxres   7.78059751283823e-01
Iteration 2     bsize 20        maxres   2.24893106059477e-02
Iteration 3     bsize 20        maxres   1.80924248589866e-02
........
Iteration 98    bsize 3         maxres   1.88845907948380e-06
Iteration 99    bsize 3         maxres   1.70505327519112e-06
Iteration 100   bsize 3         maxres   1.69226446409738e-06
........
Eigenvalue lambda       Residual
  1.17962542906779e-04    9.12159288714868e-09
  2.35923539283847e-04    9.26846395155813e-09
  2.35923539299908e-04    6.48999956327757e-09
........
  6.68442040292635e-04    4.06660781310217e-07
  6.68442041063227e-04    6.45320260847077e-07
  6.68442052000848e-04    1.69226446409738e-06

100 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 1166.910000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 1166.760000 seconds
  cpu MFLOPS      = 0.000000
```

# Hypre Testing on Larger Problems (AMD BLAS/LAPACK libraries) #
Test on LLNL ATLAS system. Execute tests for Hypre's IJ interface. Find 20 smallest eigenpairs of 3-D 7-point 400x400x400 Laplacian. Use 8 nodes and 64 processors, random guess and BoomerAMG (default) preconditioner.
```
srun -n64  ij -lobpcg -n 400 400 400 -pcgitr 0 -vrand 20 -seed 1  -P 4 4 4 -hmis -agg_nl 1 -interptype 6 -Pmx 4
........
  Laplacian:
    (nx, ny, nz) = (400, 400, 400)
    (Px, Py, Pz) = (4, 4, 4)
    (cx, cy, cz) = (1.000000, 1.000000, 1.000000)
........
Initial Max. Residual   2.44739676813973e+00
Iteration 1     bsize 20        maxres   8.10036373965024e-01
Iteration 2     bsize 20        maxres   2.74010627338399e-02
Iteration 3     bsize 20        maxres   2.38522241565166e-02
........
Iteration 98    bsize 3         maxres   1.76268238384281e-06
Iteration 99    bsize 3         maxres   1.52652960627237e-06
Iteration 100   bsize 3         maxres   1.42073159770084e-06
........
Eigenvalue lambda       Residual
  1.84132323721136e-04    7.88363056635417e-09
  3.68260879905147e-04    8.50482771093186e-09
  3.68260879923057e-04    6.97386968653446e-09
........
  1.04338636289971e-03    4.71260888702569e-08
  1.04338636337951e-03    4.16761054854837e-07
  1.04338637175944e-03    1.42073159770084e-06

100 iterations  
=============================================
Solve phase times:
=============================================
LOBPCG Solve:   
  wall clock time = 4109.830000 seconds  
  wall MFLOPS     = 0.000000    
  cpu clock time  = 4108.830000 seconds
  cpu MFLOPS      = 0.000000
```
There was no significant performance difference between the standard BLAS/LAPACK libraries provided with hypre and the AMD BLAS/LAPACK libraries available on the LLNL ATLAS system.

