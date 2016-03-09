# Introduction #

BLOPEX has been moved from PETSc to SLEPc. The SLEPc BLOPEX install needs to be tested. This applies to PETSc and SLEPc both version 3.2 and above.

# Installation #

Install PETSc and then SLEPc according to the instructions from http://www.mcs.anl.gov/petsc/petsc-as/documentation/installation.html and http://slepc.upv.es/documentation/instal.htm

In the SLEPc installation, use
```
./configure --download-blopex 
```

# Testing procedures #

SLEPc's make test will pick up from the SLEPc configure if --download-blopex has been used, and if so, run one BLOPEX test. If everything runs correctly, make test should produce no errors.

Additionally, it is possible to carry out a more comprehensive testing with make alltest. While make alltest should not produce any output at all, ideally, it is unfinished in 3.2, and only one test is run with Blopex. It will be developed further in future versions.

SLEPc examples for linear symmetric eigenvalue problems give the user an opportunity to replace the default SLEPc solver with BLOPEX LOBPCG, e.g., without preconditioning as in

```
 ./ex19 -eps_type blopex -eps_view -eps_monitor -eps_conv_abs 
```

To test BLOPEX LOBPCG with hypre algebraic multigrid preconditioning, run

```
 ./ex19 -eps_type blopex -st_pc_type hypre -st_pc_hypre_type boomeramg -eps_view -eps_nev 10 -eps_monitor -eps_conv_abs 
```

SLEPc's Example 19 is for the 3D Laplacian. The user can specify the number of the eigenpairs to compute  and the number of the grid points as follows,

```
 ./ex19 -eps_type blopex -st_pc_type hypre -st_pc_hypre_type boomeramg -eps_view -eps_tol 1e-6 -eps_nev 10 -eps_monitor -eps_conv_abs -da_grid_x 40 -da_grid_y 40 -da_grid_z 40
```

The -eps\_conv\_abs option is not really necessary since BLOPEX will do absolute residual anyway.
To obtain timings one has to add -log\_summary