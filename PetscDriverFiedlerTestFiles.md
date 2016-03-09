

# Overview #

For Petsc testing with driver\_fiedler the following files are available.

File names containing "complex" are for complex scalars, with "double" are for real scalars.

File names containinng "_64" are for 64bit integers, otherwise they are for 32bit integers._

It is important to match the files to the Petsc configuration for scalar and integer size.

The file formats are different for each combination.

# How the files were created #

The graph Laplacian L has been first created in MATLAB from the 256x256 gray-scale picture ![http://blopex.googlecode.com/files/lenna.gif](http://blopex.googlecode.com/files/lenna.gif) using the MATLAB code  http://eslab.bu.edu/software/graphanalysis/graphanalysisDEMOS.html

These files are setup via some Matlab programs in the PETSc socket interface to Matlab; `PetscBinaryRead.m` and `PetscBinaryWrite.m`. These programs read and write Matlab matrices and vectors to files formatted for Petsc. The version from Petsc only supports double. We have modified these programs to also support complex and 64bit integers. Our versions are included in the Google source directory  `../blopex_petsc` along with `PetscWriteReadExample.m` to illustrate how to use them.

# Files with suffix of -complex.petsc, -double.petsc, -complex\_64.petsc, -double\_64.petsc #

_Developer Notes:_  These files were original to ver 1.0 of Blopex. They were uploaded to Matlab and used to create the complex and 64bit versions, without changing the actual values.
```
 L-matrix       65536x65536 graph laplacian of 256x256 lattice with edge weights from 0 to 4.  

Run driver_fiedler, e.g.,  with  -matrix L-matrix-double.petsc 

                Smallest eigenvalue is zero with constant eigenvector.  
                Use constant eigenvector as a constraint. 
 
                Eigenvalue lambda       Residual
                3.24059222840027e-06    9.35410996347492e-07
                9.46022776819983e-06    2.32669917432429e-07
                2.01381014910642e-05    3.06346512297800e-07


 DL-matrix      65536x65536 diagonal matrix.  This is diagonals of L-matrix.

                Run driver_fiedler with -matrix L-matrix-double.petsc -mass_matrix DL-matrix-double.petsc to get the eigenpairs of the normalized graph Laplacian inv(DL)L. 

```

# Files just for complex scalars with suffix of .petsc, _64.petsc. #_

_Developer Notes:_  These files are new to ver 1.1 of Blopex. They were created in Matlab and downloaded to unix.  They are hermition postitive definite, sparse, with randomly generated values.  These are the only files with imaginary values.  They do not have a constant eigenvector and driver\_fiedler should be run with option "no\_con"; otherwise solution of eigenvalues is to about 1e-3 at which point they stagnate there and do not improve. test_complex1  40x40 134 nz hpd random
  
              Eigenvalue lambda       Residual
              9.31610207578953e-01    6.96955369464395e-07
              9.43228356410202e-01    9.67561486865887e-07
              9.45843781717145e-01    9.05928218002553e-07

 test_complex2  1000x1000 50786 nz hpd random

              Eigenvalue lambda       Residual
              5.83754860883465e-01    9.47692074183951e-07
              5.92733440713132e-01    9.30055866146296e-07
              5.95423063672527e-01    8.89839810686321e-07

 test_complex3  1000x1000 10876 nz hpd random

              Eigenvalue lambda       Residual
              9.61904495051511e+01    7.59173881115497e-07
              9.62288325907204e+01    8.48299651119655e-07
             9.62817871735585e+01    9.43808423633657e-07}}}```