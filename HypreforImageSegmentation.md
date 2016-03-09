# Introduction #

The purpose of this page is to describe the steps involved in using the MATLAB image segmentation package in conjunction with Blopex in Hypre to solve image segmentation eigenvalue problems.


## Preliminaries ##

The basic system info:

```
>> uname -a
Linux xvia.cudenver.edu 2.6.27.41-170.2.117.fc10.x86_64 #1 SMP Thu Dec 10 10:36:29 EST 2009 x86_64 x86_64 x86_64 GNU/Linux
```

## Hypre Installation ##

For detailed instructions on installing Hypre consult the wiki page http://code.google.com/p/blopex/wiki/InstallingHypreDetailedInstructions.

For this example Hypre is compiled using Mpich2 and already existing BLAS/Lapack libraries. Also, the file ij.c in the Hypre test directory is replaced with one that eases using constraints when running the program. In the original file the constraint parameter searches for the Hypre files named 'vector' in the test directory, which is also the file that is written to when specifying the -vout parameter. To avoid any hazards or annoyances that could result from this, the modified file searches for the Hypre files named 'constraints'. Note that this swap must occur before Hypre is compiled. Also, if constraints aren't used at all the following "Orthonormalization of Residuals Failed" error seems to invariably occur, at least for this class of problem:
```
Running with these driver parameters:
  solver ID    = 1

=============================================
IJ Matrix Setup:
=============================================
Spatial operator:
  wall clock time = 0.000000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 0.000000 seconds
  cpu MFLOPS      = 0.000000

  RHS vector has unit components
  Initial guess is 0
=============================================
IJ Vector Setup:
=============================================
RHS and Initial Guess:
  wall clock time = 0.290000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 0.290000 seconds
  cpu MFLOPS      = 0.000000

Solver: AMG-PCG
HYPRE_ParCSRLOBPCGGetPrecond got good precond

BoomerAMG SETUP PARAMETERS:

 Max levels = 25
 Num levels = 17

 Strength Threshold = 0.250000
 Interpolation Truncation Factor = 0.000000
 Maximum Row Sum Threshold for Dependency Weakening = 1.000000

 Coarsening Type = Falgout-CLJP 
 measures are determined locally

 Interpolation = modified classical interpolation

Operator Matrix Information:

            nonzero         entries per row        row sums
lev   rows  entries  sparse  min  max   avg       min         max
===================================================================
 0 8261582 57353696  0.000     4    7   6.9  -2.665e-15   2.220e-15
 1 5152366 73542002  0.000     4   36  14.3  -5.768e-15   6.384e-15
 2 2835844 89509164  0.000     4  101  31.6  -8.199e-15   1.493e-14
 3 1477913 82990041  0.000     4  205  56.2  -1.007e-14   2.085e-14
 4  744595 65914459  0.000     5  314  88.5  -1.672e-14   2.906e-14
 5  364879 47406191  0.000     4  487  129.9  -2.344e-14   4.136e-14
 6  172769 30429543  0.001     9  678  176.1  -4.490e-14   6.858e-14
 7   78237 17018791  0.003    10  803  217.5  -5.999e-14   1.269e-13
 8   33683  8010643  0.007    12  811  237.8  -8.023e-14   3.607e-13
 9   13743  3140861  0.017    22  654  228.5  -9.346e-14   1.028e-12
10    5204  1005966  0.037    16  507  193.3  -1.407e-13   1.262e-12
11    1802   262646  0.081     9  357  145.8  -4.062e-13   1.796e-12
12     532    46568  0.165     8  185  87.5  -3.605e-13   3.486e-12
13     153     7563  0.323    16   87  49.4   6.585e-15   7.755e-12
14      48     1386  0.602    15   47  28.9   1.206e-13   2.302e-11
15      15      197  0.876    10   15  13.1   6.307e-12   5.243e-11
16       6       36  1.000     6    6   6.0   1.744e-11   1.030e-10


Interpolation Matrix Information:
                 entries/row    min     max         row sums
lev  rows cols    min max     weight   weight     min       max 
=================================================================
 0 8261582 x 5152366   1   6   4.765e-02 1.000e+00 1.000e+00 1.000e+00
 1 5152366 x 2835844   1  17   2.096e-02 1.000e+00 1.000e+00 1.000e+00
 2 2835844 x 1477913   1  21   1.869e-02 1.000e+00 1.000e+00 1.000e+00
 3 1477913 x 744595   1  20   2.073e-02 1.000e+00 1.000e+00 1.000e+00
 4 744595 x 364879   1  19   2.041e-02 1.000e+00 1.000e+00 1.000e+00
 5 364879 x 172769   1  20   1.831e-02 1.000e+00 1.000e+00 1.000e+00
 6 172769 x 78237   1  20   1.910e-02 1.000e+00 1.000e+00 1.000e+00
 7 78237 x 33683   1  22   1.953e-02 1.000e+00 1.000e+00 1.000e+00
 8 33683 x 13743   1  22   1.802e-02 1.000e+00 1.000e+00 1.000e+00
 9 13743 x 5204    1  21   2.133e-02 1.000e+00 1.000e+00 1.000e+00
10  5204 x 1802    1  15   1.962e-02 1.000e+00 1.000e+00 1.000e+00
11  1802 x 532     1  10   3.078e-02 1.000e+00 1.000e+00 1.000e+00
12   532 x 153     1   7   5.862e-02 1.000e+00 1.000e+00 1.000e+00
13   153 x 48      1   5   6.601e-02 1.000e+00 1.000e+00 1.000e+00
14    48 x 15      1   4   1.725e-01 1.000e+00 1.000e+00 1.000e+00
15    15 x 6       1   3   1.298e-01 1.000e+00 1.000e+00 1.000e+00


     Complexity:    grid = 2.317156
                operator = 8.310532




BoomerAMG SOLVER PARAMETERS:

  Maximum number of cycles:         1 
  Stopping Tolerance:               0.000000e+00 
  Cycle type (1 = V, 2 = W, etc.):  1

  Relaxation Parameters:
   Visiting Grid:                     down   up  coarse
            Number of sweeps:            2    2     1 
   Type 0=Jac, 3=hGS, 6=hSGS, 9=GE:      3    3     9 
   Point types, partial sweeps (1=C, -1=F):
                  Pre-CG relaxation (down):   1  -1
                   Post-CG relaxation (up):  -1   1
                             Coarsest grid:   0

=============================================
Setup phase times:
=============================================
LOBPCG Setup:
  wall clock time = 263.350000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 263.290000 seconds
  cpu MFLOPS      = 0.000000


Solving standard eigenvalue problem with preconditioning

block size 2

No constraints


Initial Max. Residual   1.98782853225389e+00
Error in LOBPCG:
Orthonormalization of residuals failed
DPOTRF INFO = 2

Eigenvalue lambda       Residual              
  1.95144186614367e+00    1.98732273959501e+00
  1.95233716875291e+00    1.98782853225389e+00

0 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 7.760000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 7.750000 seconds
  cpu MFLOPS      = 0.000000
```

The file ij. can be modified by replacing lines 3120-3124:
```
if ( constrained ) {
         constraints = mv_MultiVectorWrap( interpreter,
                         hypre_ParCSRMultiVectorRead(MPI_COMM_WORLD,
                                                     interpreter,
                                                     "vectors" ),1);
```

With:
```
 if ( constrained ) {
         constraints = mv_MultiVectorWrap( interpreter,
                         hypre_ParCSRMultiVectorRead(MPI_COMM_WORLD,
                                                     interpreter,
                                                     "constraints" ),1);
```

The complete setup for the installation used here is as follows:

```
>> tar xzvf hypre-2.6.0b.tar.gz
>> mv hypre-2.6.0b hypre-2.6.0b_mpich2.2
>> cd hypre-2.6.0b_mpich2.1/src
>> wget http://blopex.googlecode.com/files/hypre_lobpcg_modifications.tar.gz
>> tar xzvf hypre_lobpcg_modifications.tar.gz
>> cd test; rm ij.c;
>> wget http://math.ucdenver.edu/~adougher/image_seg_test_replace/ij.c;
>> cd ..; mpd&
>> ./configure --with-lapack-libs="lapack blas" --with-lapack-lib-dirs="/usr/lib64"
>> make; make test
```

## MATLAB Image Segmentation Package ##

Next the Image Segmentation Package is downloaded from the Blopex google code site and decompressed in the MATLAB directory.  It is also convenient to create an 'images' subdirectory for storing the image files to be segmented.

```
>> wget http://code.google.com/p/blopex/downloads/detail?name=segmentation.tar&can=2&q=
>> tar -xvf segmentation.tar
>> mkdir images
```

Now MATLAB may be started and the code obtained from the package may be used to generate the graph Laplacian derived from an image and the constraint vector as follows:

```
>> matlab
>> [Mat,M,icm,dims]=img2laplacian('images/GeraniumFlowerUnfurl2.gif')
>> n=size(Mat)
>> matlab2hypreIJ(Mat,16,'/home/guests/adougher/hypre-2.6.0b_mpich2.2/src/test/geranium','16.15e')
>> matlab2hypreParVectors(ones(n(1),1),16,'/home/guests/adougher/hypre-2.6.0b_mpich2.2/src/test/constraints','16.15e')
```

Note that all the return values are retrieved from the img2laplacian function and will be used when reconstructing the partitioned image later.

At this point it is useful to open a second terminal to go to the Hypre test directory, which in this case the Hypre matrix and vector files were written to.  If an mpi daemon isn't running, it must be started before the eigenvalues/eigenvectors can be computed in Hypre:

```
>> cd hypre-2.6.0b_mpich2.1/src/test
>> mpd&  
```

Now run the solver:

```
>> mpirun -np 16 ./ij -lobpcg -vrand 2 -tol 1e-12 -pcgitr 0 -itr 50 -seed 2 -solver 0 -fromfile geranium -con -vout 1
```

Here -np 16 signifies the number of processors, -con specifies the use of the constraints which are in the test directory and -vout 1 specifies that the eigenvectors are to be written to hypre vector files. Once the iterations are complete, the resulting eigenvectors must be moved back to the MATLAB directory.  Note that all of the vector files thus generated must be moved back or the hypreParVectors2matlab( ) function will not work.

Here is the output for a typical Hypre run:
```
Running with these driver parameters:
  solver ID    = 1

=============================================
IJ Matrix Setup:
=============================================
Spatial operator:
  wall clock time = 0.000000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 0.000000 seconds
  cpu MFLOPS      = 0.000000

  RHS vector has unit components
  Initial guess is 0
=============================================
IJ Vector Setup:
=============================================
RHS and Initial Guess:
  wall clock time = 0.300000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 0.300000 seconds
  cpu MFLOPS      = 0.000000

Solver: AMG-PCG
HYPRE_ParCSRLOBPCGGetPrecond got good precond

BoomerAMG SETUP PARAMETERS:

 Max levels = 25
 Num levels = 17

 Strength Threshold = 0.250000
 Interpolation Truncation Factor = 0.000000
 Maximum Row Sum Threshold for Dependency Weakening = 1.000000

 Coarsening Type = Falgout-CLJP 
 measures are determined locally

 Interpolation = modified classical interpolation

Operator Matrix Information:

            nonzero         entries per row        row sums
lev   rows  entries  sparse  min  max   avg       min         max
===================================================================
 0 8261582 57353696  0.000     4    7   6.9  -2.665e-15   2.220e-15
 1 5152366 73542002  0.000     4   36  14.3  -5.768e-15   6.384e-15
 2 2835844 89509164  0.000     4  101  31.6  -8.199e-15   1.493e-14
 3 1477913 82990041  0.000     4  205  56.2  -1.007e-14   2.085e-14
 4  744595 65914459  0.000     5  314  88.5  -1.672e-14   2.906e-14
 5  364879 47406191  0.000     4  487  129.9  -2.344e-14   4.136e-14
 6  172769 30429543  0.001     9  678  176.1  -4.490e-14   6.858e-14
 7   78237 17018791  0.003    10  803  217.5  -5.999e-14   1.269e-13
 8   33683  8010643  0.007    12  811  237.8  -8.023e-14   3.607e-13
 9   13743  3140861  0.017    22  654  228.5  -9.346e-14   1.028e-12
10    5204  1005966  0.037    16  507  193.3  -1.407e-13   1.262e-12
11    1802   262646  0.081     9  357  145.8  -4.062e-13   1.796e-12
12     532    46568  0.165     8  185  87.5  -3.605e-13   3.486e-12
13     153     7563  0.323    16   87  49.4   6.585e-15   7.755e-12
14      48     1386  0.602    15   47  28.9   1.206e-13   2.302e-11
15      15      197  0.876    10   15  13.1   6.307e-12   5.243e-11
16       6       36  1.000     6    6   6.0   1.744e-11   1.030e-10


Interpolation Matrix Information:
                 entries/row    min     max         row sums
lev  rows cols    min max     weight   weight     min       max 
=================================================================
 0 8261582 x 5152366   1   6   4.765e-02 1.000e+00 1.000e+00 1.000e+00
 1 5152366 x 2835844   1  17   2.096e-02 1.000e+00 1.000e+00 1.000e+00
 2 2835844 x 1477913   1  21   1.869e-02 1.000e+00 1.000e+00 1.000e+00
 3 1477913 x 744595   1  20   2.073e-02 1.000e+00 1.000e+00 1.000e+00
 4 744595 x 364879   1  19   2.041e-02 1.000e+00 1.000e+00 1.000e+00
 5 364879 x 172769   1  20   1.831e-02 1.000e+00 1.000e+00 1.000e+00
 6 172769 x 78237   1  20   1.910e-02 1.000e+00 1.000e+00 1.000e+00
 7 78237 x 33683   1  22   1.953e-02 1.000e+00 1.000e+00 1.000e+00
 8 33683 x 13743   1  22   1.802e-02 1.000e+00 1.000e+00 1.000e+00
 9 13743 x 5204    1  21   2.133e-02 1.000e+00 1.000e+00 1.000e+00
10  5204 x 1802    1  15   1.962e-02 1.000e+00 1.000e+00 1.000e+00
11  1802 x 532     1  10   3.078e-02 1.000e+00 1.000e+00 1.000e+00
12   532 x 153     1   7   5.862e-02 1.000e+00 1.000e+00 1.000e+00
13   153 x 48      1   5   6.601e-02 1.000e+00 1.000e+00 1.000e+00
14    48 x 15      1   4   1.725e-01 1.000e+00 1.000e+00 1.000e+00
15    15 x 6       1   3   1.298e-01 1.000e+00 1.000e+00 1.000e+00


     Complexity:    grid = 2.317156
                operator = 8.310532




BoomerAMG SOLVER PARAMETERS:

  Maximum number of cycles:         1 
  Stopping Tolerance:               0.000000e+00 
  Cycle type (1 = V, 2 = W, etc.):  1

  Relaxation Parameters:
   Visiting Grid:                     down   up  coarse
            Number of sweeps:            2    2     1 
   Type 0=Jac, 3=hGS, 6=hSGS, 9=GE:      3    3     9 
   Point types, partial sweeps (1=C, -1=F):
                  Pre-CG relaxation (down):   1  -1
                   Post-CG relaxation (up):  -1   1
                             Coarsest grid:   0

=============================================
Setup phase times:
=============================================
LOBPCG Setup:
  wall clock time = 262.800000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 262.740000 seconds
  cpu MFLOPS      = 0.000000


Solving standard eigenvalue problem with preconditioning

block size 2

1 constraint


Initial Max. Residual   1.98782870052205e+00
Iteration 1     bsize 2         maxres   3.39903851831803e-03
Iteration 2     bsize 2         maxres   5.17425570707509e-04
Iteration 3     bsize 2         maxres   6.94089167198477e-05
Iteration 4     bsize 2         maxres   5.12254686850433e-05
Iteration 5     bsize 2         maxres   2.10494383460008e-05
Iteration 6     bsize 2         maxres   6.79429843550656e-06
Iteration 7     bsize 2         maxres   2.78694682224605e-06
Iteration 8     bsize 2         maxres   8.68222678329595e-07
Iteration 9     bsize 2         maxres   3.59982960870506e-07
Iteration 10    bsize 2         maxres   1.65203846325236e-07
Iteration 11    bsize 2         maxres   7.16690653311551e-08
Iteration 12    bsize 2         maxres   2.41296600232106e-08
Iteration 13    bsize 2         maxres   1.07031577751071e-08
Iteration 14    bsize 2         maxres   5.95108004605595e-09
Iteration 15    bsize 2         maxres   2.04651027735949e-09
Iteration 16    bsize 2         maxres   6.66687405572575e-10
Iteration 17    bsize 2         maxres   2.56853885404286e-10
Iteration 18    bsize 1         maxres   1.43762781344168e-10
Iteration 19    bsize 1         maxres   7.30005178910771e-11
Iteration 20    bsize 1         maxres   2.97889547181117e-11
Iteration 21    bsize 1         maxres   1.16660336148334e-11
Iteration 22    bsize 1         maxres   3.50658778818712e-12
Iteration 23    bsize 1         maxres   1.40102057765528e-12
Iteration 24    bsize 1         maxres   8.94285533863048e-13

Eigenvalue lambda       Residual              
  1.60674729604999e-06    3.33627319079956e-13
  3.35698714422221e-06    8.94285533863048e-13

24 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 190.320000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 190.190000 seconds
  cpu MFLOPS      = 0.000000

```


Now, back in the MATLAB terminal, run the following:
```
>> v = hypreParVectors2matlab('vectors')
>> back_to_image
```

Here back\_to\_image is a modified segment\_demo script, which uses the previous variables to carry out the conversion back into an image. With this the image has been segmented based on the computed fiedler vector and the segmented image has been written to a file along with an eigenvector image.

Some examples of using this procedure can be found here:

http://math.ucdenver.edu/~adougher/images/


## Additional Notes ##

With some images, including the one used in the example above, the iterations in LOBPCG diverge if the tolerance is set below a certain threshold. For example, using 1e-14 instead of 1e-12 as follows:

```
>> mpirun -np 16 ./ij -lobpcg -vrand 2 -tol 1e-14 -pcgitr 0 -itr 50 -seed 2 -solver 0 -fromfile geranium -con -vout 1
```

Produces the following output in Hypre:

```
Running with these driver parameters:
  solver ID    = 1

=============================================
IJ Matrix Setup:
=============================================
Spatial operator:
  wall clock time = 0.000000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 0.000000 seconds
  cpu MFLOPS      = 0.000000

  RHS vector has unit components
  Initial guess is 0
=============================================
IJ Vector Setup:
=============================================
RHS and Initial Guess:
  wall clock time = 0.290000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 0.290000 seconds
  cpu MFLOPS      = 0.000000

Solver: AMG-PCG
HYPRE_ParCSRLOBPCGGetPrecond got good precond

BoomerAMG SETUP PARAMETERS:

 Max levels = 25
 Num levels = 17

 Strength Threshold = 0.250000
 Interpolation Truncation Factor = 0.000000
 Maximum Row Sum Threshold for Dependency Weakening = 1.000000

 Coarsening Type = Falgout-CLJP 
 measures are determined locally

 Interpolation = modified classical interpolation

Operator Matrix Information:

            nonzero         entries per row        row sums
lev   rows  entries  sparse  min  max   avg       min         max
===================================================================
 0 8261582 57353696  0.000     4    7   6.9  -2.665e-15   2.220e-15
 1 5152366 73542002  0.000     4   36  14.3  -5.768e-15   6.384e-15
 2 2835844 89509164  0.000     4  101  31.6  -8.199e-15   1.493e-14
 3 1477913 82990041  0.000     4  205  56.2  -1.007e-14   2.085e-14
 4  744595 65914459  0.000     5  314  88.5  -1.672e-14   2.906e-14
 5  364879 47406191  0.000     4  487  129.9  -2.344e-14   4.136e-14
 6  172769 30429543  0.001     9  678  176.1  -4.490e-14   6.858e-14
 7   78237 17018791  0.003    10  803  217.5  -5.999e-14   1.269e-13
 8   33683  8010643  0.007    12  811  237.8  -8.023e-14   3.607e-13
 9   13743  3140861  0.017    22  654  228.5  -9.346e-14   1.028e-12
10    5204  1005966  0.037    16  507  193.3  -1.407e-13   1.262e-12
11    1802   262646  0.081     9  357  145.8  -4.062e-13   1.796e-12
12     532    46568  0.165     8  185  87.5  -3.605e-13   3.486e-12
13     153     7563  0.323    16   87  49.4   6.585e-15   7.755e-12
14      48     1386  0.602    15   47  28.9   1.206e-13   2.302e-11
15      15      197  0.876    10   15  13.1   6.307e-12   5.243e-11
16       6       36  1.000     6    6   6.0   1.744e-11   1.030e-10


Interpolation Matrix Information:
                 entries/row    min     max         row sums
lev  rows cols    min max     weight   weight     min       max 
=================================================================
 0 8261582 x 5152366   1   6   4.765e-02 1.000e+00 1.000e+00 1.000e+00
 1 5152366 x 2835844   1  17   2.096e-02 1.000e+00 1.000e+00 1.000e+00
 2 2835844 x 1477913   1  21   1.869e-02 1.000e+00 1.000e+00 1.000e+00
 3 1477913 x 744595   1  20   2.073e-02 1.000e+00 1.000e+00 1.000e+00
 4 744595 x 364879   1  19   2.041e-02 1.000e+00 1.000e+00 1.000e+00
 5 364879 x 172769   1  20   1.831e-02 1.000e+00 1.000e+00 1.000e+00
 6 172769 x 78237   1  20   1.910e-02 1.000e+00 1.000e+00 1.000e+00
 7 78237 x 33683   1  22   1.953e-02 1.000e+00 1.000e+00 1.000e+00
 8 33683 x 13743   1  22   1.802e-02 1.000e+00 1.000e+00 1.000e+00
 9 13743 x 5204    1  21   2.133e-02 1.000e+00 1.000e+00 1.000e+00
10  5204 x 1802    1  15   1.962e-02 1.000e+00 1.000e+00 1.000e+00
11  1802 x 532     1  10   3.078e-02 1.000e+00 1.000e+00 1.000e+00
12   532 x 153     1   7   5.862e-02 1.000e+00 1.000e+00 1.000e+00
13   153 x 48      1   5   6.601e-02 1.000e+00 1.000e+00 1.000e+00
14    48 x 15      1   4   1.725e-01 1.000e+00 1.000e+00 1.000e+00
15    15 x 6       1   3   1.298e-01 1.000e+00 1.000e+00 1.000e+00


     Complexity:    grid = 2.317156
                operator = 8.310532




BoomerAMG SOLVER PARAMETERS:

  Maximum number of cycles:         1 
  Stopping Tolerance:               0.000000e+00 
  Cycle type (1 = V, 2 = W, etc.):  1

  Relaxation Parameters:
   Visiting Grid:                     down   up  coarse
            Number of sweeps:            2    2     1 
   Type 0=Jac, 3=hGS, 6=hSGS, 9=GE:      3    3     9 
   Point types, partial sweeps (1=C, -1=F):
                  Pre-CG relaxation (down):   1  -1
                   Post-CG relaxation (up):  -1   1
                             Coarsest grid:   0

=============================================
Setup phase times:
=============================================
LOBPCG Setup:
  wall clock time = 262.720000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 262.660000 seconds
  cpu MFLOPS      = 0.000000


Solving standard eigenvalue problem with preconditioning

block size 2

1 constraint


Initial Max. Residual   1.98782870052205e+00
Iteration 1     bsize 2         maxres   3.39903851831803e-03
Iteration 2     bsize 2         maxres   5.17425570707509e-04
Iteration 3     bsize 2         maxres   6.94089167198477e-05
Iteration 4     bsize 2         maxres   5.12254686850433e-05
Iteration 5     bsize 2         maxres   2.10494383460008e-05
Iteration 6     bsize 2         maxres   6.79429843550656e-06
Iteration 7     bsize 2         maxres   2.78694682224605e-06
Iteration 8     bsize 2         maxres   8.68222678329595e-07
Iteration 9     bsize 2         maxres   3.59982960870506e-07
Iteration 10    bsize 2         maxres   1.65203846325236e-07
Iteration 11    bsize 2         maxres   7.16690653311551e-08
Iteration 12    bsize 2         maxres   2.41296600232106e-08
Iteration 13    bsize 2         maxres   1.07031577751071e-08
Iteration 14    bsize 2         maxres   5.95108004605595e-09
Iteration 15    bsize 2         maxres   2.04651027735949e-09
Iteration 16    bsize 2         maxres   6.66687405572575e-10
Iteration 17    bsize 2         maxres   2.56853885404286e-10
Iteration 18    bsize 2         maxres   1.30226412637203e-10
Iteration 19    bsize 2         maxres   6.48807389228269e-11
Iteration 20    bsize 2         maxres   2.58602244323723e-11
Iteration 21    bsize 2         maxres   1.05704804873557e-11
Iteration 22    bsize 2         maxres   2.64573030564432e-12
Iteration 23    bsize 2         maxres   9.00029180753994e-13
Iteration 24    bsize 2         maxres   2.06007639948745e-12
Iteration 25    bsize 2         maxres   7.48441581910725e-12
Iteration 26    bsize 2         maxres   3.78407593436425e-11
Iteration 27    bsize 2         maxres   2.81003076533856e-04
Iteration 28    bsize 2         maxres   8.44279891044392e-05
Error in LOBPCG:
GEVP solver failure
Return Code 11
INFO = 11
INFO = 11
Return Code 11
Return Code 11
Return Code 11
Return Code 11
Return Code 11
INFO = 11
INFO = 11
INFO = 11
INFO = 11
INFO = 11
INFO = 11
INFO = 11
INFO = 11
Return Code 11
Return Code 11
Return Code 11
Return Code 11

Eigenvalue lambda       Residual              INFO = 11
Return Code 11
INFO = 11
INFO = 11
INFO = 11
Return Code 11
Return Code 11
Return Code 11
Return Code 11
Return Code 11
INFO = 11

  1.06720096174625e-07    8.44279891044392e-05
INFO = 11
  1.60674729603701e-06    8.77387165014741e-10

27 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 274.710000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 274.520000 seconds
  cpu MFLOPS      = 0.000000
```

The program displayed similar behavior for some of the other images tested as well.

One method that can be used to achieve a smaller tolerance is to shift the matrix's diagonal by a positive scalar, alpha, before solving for its eigenvalues in Hypre.  In the case of the example above, the scalar used was alpha=1.0000e-05, and the matrix was shifted with the command:

```
>>Mat=Mat+alpha*speye(n);
```

The resulting Hypre output for the shifted matrix was:

```
Running with these driver parameters:
  solver ID    = 1

=============================================
IJ Matrix Setup:
=============================================
Spatial operator:
  wall clock time = 0.000000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 0.000000 seconds
  cpu MFLOPS      = 0.000000

  RHS vector has unit components
  Initial guess is 0
=============================================
IJ Vector Setup:
=============================================
RHS and Initial Guess:
  wall clock time = 0.300000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 0.300000 seconds
  cpu MFLOPS      = 0.000000

Solver: AMG-PCG
HYPRE_ParCSRLOBPCGGetPrecond got good precond

BoomerAMG SETUP PARAMETERS:

 Max levels = 25
 Num levels = 17

 Strength Threshold = 0.250000
 Interpolation Truncation Factor = 0.000000
 Maximum Row Sum Threshold for Dependency Weakening = 1.000000

 Coarsening Type = Falgout-CLJP 
 measures are determined locally

 Interpolation = modified classical interpolation

Operator Matrix Information:

            nonzero         entries per row        row sums
lev   rows  entries  sparse  min  max   avg       min         max
===================================================================
 0 8261582 57353696  0.000     4    7   6.9   1.000e-05   1.000e-05
 1 5152366 73542002  0.000     4   36  14.3   5.226e-06   5.796e-05
 2 2834209 89435833  0.000     4  101  31.6  -3.804e-05   2.075e-04
 3 1477508 82939920  0.000     4  187  56.1  -6.942e-05   4.062e-04
 4  744818 65886708  0.000     4  324  88.5  -1.733e-04   9.817e-04
 5  364449 47179285  0.000     5  454  129.5  -2.090e-04   1.996e-03
 6  172742 30449482  0.001     8  638  176.3  -2.714e-04   3.926e-03
 7   78362 17046792  0.003     8  772  217.5  -5.517e-04   8.250e-03
 8   33758  8093988  0.007    11  840  239.8  -6.897e-04   2.099e-02
 9   13746  3158102  0.017    13  714  229.7  -7.100e-04   4.144e-02
10    5266  1032258  0.037    22  506  196.0  -8.097e-04   1.099e-01
11    1800   260094  0.080    18  312  144.5   3.178e-04   2.619e-01
12     538    47090  0.163    17  166  87.5   2.844e-03   8.743e-01
13     154     7464  0.315    15   89  48.5   1.531e-02   1.724e+00
14      53     1579  0.562    15   46  29.8   9.989e-02   3.905e+00
15      19      289  0.801     9   18  15.2   6.312e-01   8.669e+00
16       6       36  1.000     6    6   6.0   4.748e+00   1.176e+01


Interpolation Matrix Information:
                 entries/row    min     max         row sums
lev  rows cols    min max     weight   weight     min       max 
=================================================================
 0 8261582 x 5152366   1   6   4.764e-02 1.000e+00 8.049e-01 1.000e+00
 1 5152366 x 2834209   1  17   2.095e-02 1.000e+00 7.225e-01 1.000e+00
 2 2834209 x 1477508   1  18   2.181e-02 1.000e+00 6.673e-01 1.000e+00
 3 1477508 x 744818   1  19   1.742e-02 1.000e+00 6.289e-01 1.000e+00
 4 744818 x 364449   1  24   1.790e-02 1.000e+00 6.939e-01 1.002e+00
 5 364449 x 172742   1  20   1.921e-02 1.000e+00 5.132e-01 1.001e+00
 6 172742 x 78362   1  21   1.805e-02 1.000e+00 6.517e-01 1.002e+00
 7 78362 x 33758   1  21   1.663e-02 1.000e+00 5.758e-01 1.005e+00
 8 33758 x 13746   1  20   2.073e-02 9.999e-01 3.503e-01 1.002e+00
 9 13746 x 5266    1  21   2.164e-02 9.997e-01 6.616e-01 1.000e+00
10  5266 x 1800    1  15   2.407e-02 9.997e-01 4.933e-01 1.000e+00
11  1800 x 538     1  11   2.751e-02 9.978e-01 6.169e-01 1.000e+00
12   538 x 154     1   7   3.460e-02 9.952e-01 8.599e-01 1.000e+00
13   154 x 53      1   5   6.145e-02 9.768e-01 6.959e-01 1.000e+00
14    53 x 19      1   4   7.831e-02 9.213e-01 5.799e-01 1.000e+00
15    19 x 6       1   3   1.185e-01 7.630e-01 3.324e-01 1.000e+00


     Complexity:    grid = 2.316914
                operator = 8.306956




BoomerAMG SOLVER PARAMETERS:

  Maximum number of cycles:         1 
  Stopping Tolerance:               0.000000e+00 
  Cycle type (1 = V, 2 = W, etc.):  1

  Relaxation Parameters:
   Visiting Grid:                     down   up  coarse
            Number of sweeps:            2    2     1 
   Type 0=Jac, 3=hGS, 6=hSGS, 9=GE:      3    3     9 
   Point types, partial sweeps (1=C, -1=F):
                  Pre-CG relaxation (down):   1  -1
                   Post-CG relaxation (up):  -1   1
                             Coarsest grid:   0

=============================================
Setup phase times:
=============================================
LOBPCG Setup:
  wall clock time = 262.990000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 262.930000 seconds
  cpu MFLOPS      = 0.000000


Solving standard eigenvalue problem with preconditioning

block size 2

1 constraint


Initial Max. Residual   1.98782870052205e+00
Iteration 1     bsize 2         maxres   3.93302682425906e-03
Iteration 2     bsize 2         maxres   6.62776537248798e-04
Iteration 3     bsize 2         maxres   1.64225892303747e-04
Iteration 4     bsize 2         maxres   1.02068938360005e-04
Iteration 5     bsize 2         maxres   9.51233336787634e-05
Iteration 6     bsize 2         maxres   5.58127141040429e-05
Iteration 7     bsize 2         maxres   2.29659819510756e-05
Iteration 8     bsize 2         maxres   1.14397351813483e-05
Iteration 9     bsize 2         maxres   5.47661685554414e-06
Iteration 10    bsize 2         maxres   2.09883944147460e-06
Iteration 11    bsize 2         maxres   1.35693740918227e-06
Iteration 12    bsize 2         maxres   1.02376056164854e-06
Iteration 13    bsize 2         maxres   7.66494618085803e-07
Iteration 14    bsize 2         maxres   2.87902502387800e-07
Iteration 15    bsize 2         maxres   1.10090920856995e-07
Iteration 16    bsize 2         maxres   3.60641943680106e-08
Iteration 17    bsize 2         maxres   2.27252497978647e-08
Iteration 18    bsize 2         maxres   2.24230147188066e-08
Iteration 19    bsize 2         maxres   1.27351869589157e-08
Iteration 20    bsize 2         maxres   4.95643070957916e-09
Iteration 21    bsize 2         maxres   1.73789933802473e-09
Iteration 22    bsize 2         maxres   9.33111765012374e-10
Iteration 23    bsize 2         maxres   7.03295467792700e-10
Iteration 24    bsize 2         maxres   6.96057014070134e-10
Iteration 25    bsize 2         maxres   3.30225074633385e-10
Iteration 26    bsize 2         maxres   1.44629868214335e-10
Iteration 27    bsize 2         maxres   5.00872770155327e-11
Iteration 28    bsize 2         maxres   2.67927118679505e-11
Iteration 29    bsize 2         maxres   1.50512921810949e-11
Iteration 30    bsize 2         maxres   8.83853375904020e-12
Iteration 31    bsize 2         maxres   4.69212274053661e-12
Iteration 32    bsize 1         maxres   2.93895046373344e-12
Iteration 33    bsize 1         maxres   1.47373417912877e-12
Iteration 34    bsize 1         maxres   6.60626731061004e-13
Iteration 35    bsize 1         maxres   2.29277216553479e-13
Iteration 36    bsize 1         maxres   1.06242543453678e-13
Iteration 37    bsize 1         maxres   9.31465727809451e-14
Iteration 38    bsize 1         maxres   6.87826983747025e-14
Iteration 39    bsize 1         maxres   2.68238169986735e-14
Iteration 40    bsize 1         maxres   1.36407906816864e-14
Iteration 41    bsize 1         maxres   8.33816600596601e-15

Eigenvalue lambda       Residual              
  1.16067472958091e-05    5.67260472574407e-15
  1.33569871439795e-05    8.33816600596601e-15

41 iterations
=============================================
Solve phase times:
=============================================
LOBPCG Solve:
  wall clock time = 351.590000 seconds
  wall MFLOPS     = 0.000000
  cpu clock time  = 351.330000 seconds
  cpu MFLOPS      = 0.000000
```

Clearly the convergence was slower all around than for the non-shifted matrix, but the residuals got smaller without the iterations diverging.  Interestingly, the segmented image and eigen-image look about the same as for the non-shifted matrix. The results are posted at the link above.