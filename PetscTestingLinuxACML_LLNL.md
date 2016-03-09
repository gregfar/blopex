# Introduction #

How to setup and test BLOPEX under version 3.1 of Petsc on Linux at LLNL (Atlas machine) with Petsc, e.g., configured for 64bit integers and complex using the ACML libraries at LLNL.


# Details #

See http://code.google.com/p/blopex/wiki/PetscInstallLinuxACML and
http://code.google.com/p/blopex/wiki/PetscTestingLinux for most of the installation details.

Petsc version petsc-3.1-p1 was used.
I downloaded a special version of `BlasLapack.py` (provided by Andrew)
and replaced `.../config/BuildSystem/config/packages/BlasLapack.py` for testing of the ACML library.

Document the configuration step here using Petsc for both complex and 64bit:
```
config/configure.py --with-blas-lapack-dir=/g/g22/argen1/lib/acml-4-4-0-gfortran-64bit-
int64/gfortran64_mp_int64/lib --with-scalar-type=complex --with-64-bit-indices --download-
blopex=/g/g22/argen1/Downloads/Petsc/blopex_petsc_abstract.tar.gz
```

# Execution of Tests #

To execute driver:
```
srun -n2 -ppdebug driver -n_eigs 3 -itr 20
```

This gives the following results:
```
Partitioning: 1 2 1
Preconditioner setup, seconds: 0.000696
Solving standard eigenvalue problem with preconditioning
block size 3
No constraints

Initial Max. Residual   2.36275597190805e+00
Iteration 1     bsize 3         maxres   1.36854860556819e+00
Iteration 2     bsize 3         maxres   4.60249737516755e-01
Iteration 3     bsize 3         maxres   1.81142996391104e-01
Iteration 4     bsize 3         maxres   5.36018111602656e-02
Iteration 5     bsize 3         maxres   1.69306765406134e-02
Iteration 6     bsize 3         maxres   4.50314165014696e-03
Iteration 7     bsize 3         maxres   1.02045240578399e-03
Iteration 8     bsize 3         maxres   1.99167510305146e-04
Iteration 9     bsize 3         maxres   3.96751512817884e-05
Iteration 10    bsize 3         maxres   9.15214998030157e-06
Iteration 11    bsize 3         maxres   2.52806768270152e-06
Iteration 12    bsize 2         maxres   8.96269840598450e-07
Iteration 13    bsize 2         maxres   1.89462955843519e-07
Iteration 14    bsize 2         maxres   3.71156225287738e-08
Iteration 15    bsize 2         maxres   9.27903687029665e-09

Eigenvalue lambda       Residual              
  2.43042158313016e-01    2.33732855712450e-09
  4.79521039879647e-01    7.46893054591811e-09
  4.79521039879661e-01    9.27903687029665e-09

15 iterations
Solution process, seconds: 3.313789e-01
```

driver\_fiedler accepts as input matrices in Petsc format. These matrices must be setup for either 32bit or 64bit arithematic. The test matrices are setup for double (real) or complex and for 32bit or 64bit. The 64bit version have a file name containing "64"`.
To execute Fiedler driver for example:
```
driver_fiedler -matrix DL-matrix-complex_64.petsc -n_eigs 3 -itr 200
```

This gives the following results:
```
Preconditioner setup, seconds: 0.026089
Solving standard eigenvalue problem with preconditioning
block size 3
1 constraint

Initial Max. Residual   9.32569736871893e-01
Iteration 1     bsize 3         maxres   1.25435910893594e-02
Iteration 2     bsize 3         maxres   1.12169888204856e-02
Iteration 3     bsize 3         maxres   2.01586998110087e-03
Iteration 4     bsize 3         maxres   1.26519755131709e-03
.................
Iteration 197   bsize 3         maxres   3.84901434292814e-06
Iteration 198   bsize 3         maxres   3.82215117346368e-06
Iteration 199   bsize 3         maxres   3.84794342322297e-06
Iteration 200   bsize 3         maxres   3.83687176782694e-06

Eigenvalue lambda       Residual              
  4.01468223580437e-05    3.83687176782694e-06
  4.01897713934938e-05    3.28372181438621e-06
  4.02222246852095e-05    3.53631825207124e-06

200 iterations
Solution process, seconds: 9.115923e+01
Final eigenvalues:
4.014682e-05
4.018977e-05
```