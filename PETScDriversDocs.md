

# Introduction #

Our PETSc test drivers are very poorly documented.
To such an extreme that it is in some cases impossible to conclude from their
results if they actually work.
Let us provide the description of the drivers here, to be added to the headers
of the c codes and/or readme files later.


# **driver** (being replaced by SLEPc's ex19) #

driver builds a negative 3D laplacian approximated by a 7pt stencil.  In each dimension there are 10 mesh points with step size of 1. The boundary conditions are Dirichlet. This yields a 1000x1000 matrix which is then solved for some of its smallest eigenvalues via the LOBPCG solver implemented in BLOPEX.

The exact eigenvalues for this matrix can be computed explicitly using http://www.mathworks.com/matlabcentral/fileexchange/27279-laplacian-in-1d-2d-3d by calling (in Matlab)

```
>> [A,lambda]=laplacian([10,10,10],{'DD','DD','DD'},5);
```
This yields 5 smallest eigenvalues of
```
      0.243042158313016
      0.479521039879648
      0.479521039879648
      0.479521039879648
      0.71599992144628
```

Default Petsc KSP preconditioning is used by BLOPEX.  This is KSP type of gmres with a restart of 30.  No constraints are used.

Some command line options for BLOPEX are available:
```
Usage: mpirun -np <procs> ./driver [-help] [all PETSc options]

Special options:
-n_eigs <integer>      Number of eigenvalues to calculate
-tol <real number>     absolute tolerance for residuals
-full_out              Produce more output
-freepart              Let PETSC fully determine partitioning
-seed <integer>        seed for random number generator
-itr <integer>         Maximal number of iterations

Example:
mpirun -np 2 ./driver -n_eigs 3 -tol 1e-6 -itr 20
```

The default command values are `-n_eigs 1 -tol 1e-8 -seed 1 -itr 100`.

A typical 1 cpu run for 5 eigenvalues gives

```
Eigenvalue lambda       Residual              
  2.43042158313016e-01    1.41962359736518e-09
  4.79521039879645e-01    5.97064247545570e-09
  4.79521039879647e-01    2.93208033313447e-09
  4.79521039879650e-01    4.49862021977072e-09
  7.15999921446300e-01    6.97319623015366e-09

22 iterations
```

It is also possible to adjust the mesh dimensions via the Petsc command line options  `-da_grid_x <integer>`, `-da_grid_y <integer>`, and `-da_grid_z <integer>`, e.g.,

```
mpirun -np 10 ./driver -da_grid_x 100 -da_grid_y 100 -da_grid_z 100 -freepart
```

# **driver\_diag** #

driver\_diag constructs a diagonal matrix of size NxN with diagonal entries from 1 thrugh N.  The value of N defaults to 1000 and can be changed via the parameter `-N <integer>` .  This matrix is then solved for some of its smallest eigenvalues via the LOBPCG solver implemented in BLOPEX.

This driver was implemented to test BLOPEX against very large sparse matrices.  Given enough memory, it has been run with over 7 million rows.

The smallest eigenvalues are just the values 1,2,3,4,etc.  Default Petsc KSP preconditioning is used by BLOPEX.  This is KSP type of gmres with a restart of 30.  No constraints are used.

Some command line options for BLOPEX are available:
```
Usage: mpirun -np <procs> driver_fiedler [-help] [all PETSc options]

Special options:
-n_eigs <integer>      Number of eigenvalues to calculate
-tol <real number>     absolute tolerance for residuals
-full_out              Produce more output
-seed <integer>        seed for random number generator
-itr <integer>         Maximal number of iterations
-output_file <string>  Filename to write calculated eigenvectors.
-shift <real number>   Apply shift to 'stiffness' matrix
-N <integer>           Size of Matrix to create 

Example:
mpirun -np 2 ./driver_diag -N 10000 -n_eigs 3 -tol 1e-6 -itr 20";
```

The default command values are `-n_eigs 3 -tol 1e-6 -seed 1 -itr 100 -N 1000`.

A typical 1 cpu run for 5 eigenvalues and matrix of 100000 rows gives

```
Eigenvalue lambda       Residual
  1.00000000000029e+00    1.70085222055097e-07
  2.00000000000974e+00    2.01071302476260e-07
  3.00000000000907e+00    2.14158734063248e-07
  4.00000000000096e+00    7.29490422057064e-07
  4.99999999997239e+00    8.75458902728146e-07

19 iterations
```

# **driver\_fiedler** (being replaced by SLEPc's ex7) #

driver\_fiedler accepts as input matrices in Petsc format. These matrices must be setup for either 32bit or 64bit arithematic, real or complex. The test matrices are setup for double (real) or complex and for 32bit or 64bit. The 64bit version have a file name containing "64". All our matrices for testing can be downloaded from http://code.google.com/p/blopex/downloads/detail?name=blopex_petsc_test.tar.gz by running

```
wget http://blopex.googlecode.com/files/blopex_petsc_test.tar.gz
```

These test files are documented in wiki PetscDriverFiedlerTestFiles.

driver\_fiedler accepts as input the matrix A in Petsc format. These can be setup via some Matlab programs in the PETSc socket interface to Matlab; `PetscBinaryRead.m` and `PetscBinaryWrite.m`. These programs read and write Matlab matrices and vectors to files formatted for Petsc. The version from Petsc only supports double. We have modified these programs to also support complex and 64bit integers. Our versions are included in the Google source directory  ../blopex\_petsc along with `PetscWriteReadExample.m` to illustrate how to use them.

driver\_fiedler was written primarily to compute the fiedler vector of a graph Laplacian L.  This is the eigenvector of the second largest eigenvalue.  Since, a graph  laplacian matrix has 0 eigenvalue with constant eigenvector, a constant vector is used as a orthogonal constraint in BLOPEX.  This is the default for the driver.  With constraint BLOPEX solves for the smallest eigenvalues excluding the constraint. The command option "-no\_con" is also available to execute BLOPEX without constraints.

Like all other drivers, default Petsc KSP preconditioning is used by BLOPEX.  This is KSP type of gmres with a restart of 30.

Some command line options for BLOPEX are available:
```
Usage: mpirun -np <procs> ./driver_fiedler [-help] [all PETSc options]

Special options:
-matrix <filename>      (mandatory) specify file with 'stiffness' matrix (in petsc format)
-mass_matrix <filename> (optional) 'mass' matrix for generalized eigenproblem
-n_eigs <integer>      Number of eigenvalues to calculate
-tol <real number>     absolute tolerance for residuals
-full_out              Produce more output
-seed <integer>        seed for random number generator
-itr <integer>         Maximal number of iterations
-output_file <string>  Filename to write calculated eigenvectors.
-shift <real number>   Apply shift to 'stiffness' matrix 
-no_con                Do not apply constraint of constant vector

Example:
mpirun -np 2 ./driver_fiedler -matrix my_matrix.bin -n_eigs 3 -tol 1e-6 -itr 20 ;
```

The default command values are `-n_eigs 3 -tol 1e-6 -seed 1 -itr 100`.

A typical run for the L-matrix with Petsc configured for real scalars and 32bit integers would be "mpirun -np 2 ./driver\_fiedler -matrix L-matrix-double.petsc -n\_eigs 3 -tol 1e-6 -itr 30" and give output of

```
Solving standard eigenvalue problem with preconditioning

block size 3

1 constraint


Initial Max. Residual   1.45873307290522e+00
Iteration 1     bsize 3         maxres   8.39697624564061e-04
Iteration 2     bsize 3         maxres   2.89666836454388e-04
Iteration 3     bsize 3         maxres   1.11597400132607e-04
Iteration 4     bsize 2         maxres   6.42740963133184e-05
Iteration 5     bsize 2         maxres   3.95714680768718e-05
Iteration 6     bsize 1         maxres   1.37799490772302e-05
Iteration 7     bsize 1         maxres   3.80150236488940e-06
Iteration 8     bsize 1         maxres   1.21769060239718e-06
Iteration 9     bsize 1         maxres   9.35410996347492e-07

Eigenvalue lambda       Residual              
  3.24059222840027e-06    9.35410996347492e-07
  9.46022776819983e-06    2.32669917432429e-07
  2.01381014910642e-05    3.06346512297800e-07

9 iterations
Solution process, seconds: 1.138196e+02
```

The test above finds Fiedler vectors of the graph Laplacian L. To find the similar vectors for the normalized graph Laplacian, i.e., inv(DL)L, use "mpirun -np 2 ./driver\_fiedler -matrix L-matrix-double.petsc -mass\_matrix DL-matrix-double.petsc -n\_eigs 3 -tol 1e-6 -itr 30". Here, DL is simply a diagonal of the matrix L. Both L and inv(DL)L have a zero eigenvalue with the eigenvector made of 1's, so the constraint of constant vector makes sense. Without it, i.e. with an extra option "-no\_con", the driver will also find this trivial eigenpair, which is already known analytically.

The driver can also be used with other matrices provided by a user, in which case the option "-no\_con" will be necessary.

DONALD, PLEASE CHECK THIS: An assumption is made in the driver\_fiedler that the sparsity pattern of the DL matrix is a subset of the sparsity pattern of the L matrix!

# **driver\_fortran** #

is an example of a PETSc call (BLOPEX call in this case) from FORTRAN 90. This is a modified version of the Fortran example ex2f.F from the PETSc source distribution (Version 2.3.3-p4). It calls the LOBPCG solver from BLOPEX with blocksize=5 and displays the final eigenvalues and the corresponding residuals. There is no history output. The executable **ex2f\_blopex** not accept any command-line options.

**ex2f\_blopex** solves a standard eigenvalue problem for a matrix A, which is the negative 2D Laplacian approximated on the standard 5-point stencil. Technically, **ex2f\_blopex** solves a generalized eigenvalue problem, but the second, "mass," matrix is just an identity.

The domain is a square, the mesh is 30-by-30, the mesh step is one, the BC are Dirichlet.
The exact eigenvalues using http://www.mathworks.com/matlabcentral/fileexchange/27279-laplacian-in-1d-2d-or-3d specifically calling
```
>> [A,lambda]=laplacian([30,30],5); lambda
```
are
```
     2.052270643241941e-02
     5.120147071122071e-02
     5.120147071122071e-02
     8.188023499002201e-02
     1.019828404161120e-01
```

The default KSP preconditioning is used for the matrix A. The default KSP type is GMRES with a restart of 30, using modified Gram-Schmidt orthogonalization, see KSPCreate PETSc docs.

Here is an example of a typical one-CPU run:

```
./ex2f_blopex
BLOPEX complete in  12 iterations
eval   1 is 0.205227064E-01 residual 0.1767E-07
eval   2 is 0.512014707E-01 residual 0.4771E-07
eval   3 is 0.512014707E-01 residual 0.2177E-06
eval   4 is 0.818802350E-01 residual 0.8697E-06
eval   5 is 0.101982840E+00 residual 0.4592E-06 
```

Here is an example of a typical 4-CPU run:
```
mpirun -np 4 ./ex2f_blopex
BLOPEX complete in  13 iterations
eval   1 is 0.205227064E-01 residual 0.2477E-07
eval   2 is 0.512014707E-01 residual 0.1560E-06
eval   3 is 0.512014707E-01 residual 0.2785E-06
eval   4 is 0.818802350E-01 residual 0.7383E-06
eval   5 is 0.101982840E+00 residual 0.9081E-06
```

The residuals will vary with the number of CPUs and on different computers.

**ex2f\_blopex** computes 5 eigenpairs simultaneously in a block and displays only the final results. To see the complete convergence history, edit the code **blopex\_c.c** and replace 0 with 2 in lines 241 and 262.

**BLOPEX 1.1 changes in in blopex\_c.c** :

  1. Add checks for PETSC\_USE\_COMPLEX to call correct function for lobpcg\_solve
  1. Add type casting when moving void parameter points
  1. Surround petsc\_lobpcg\_solve\_c_with  extern "C" { .... }. This is to enforce compatibility of object names between fortran and C++._

**Possible improvement.**

  1. Check the most recent version of the Fortran example ex2f.F and the related C code from the PETSc source and try to sync with them.
  1. What does the code actually do in complex arithmetic?