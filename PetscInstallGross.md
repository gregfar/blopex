# Introduction #

Howto install and run PETSc on UCDenver gross cluster. The installation is only required once. The installation procedure is completely described in the next section.

After successful installation of PETSc and compilation of the driver, one does not need to repeat any installation steps, but should rather simply run the compiled **driver** code with different command line options, described in Section "Running".

# Installation #

  * Choose the working MPI interface---run

`mpi-selector-menu`

and choose item 2. mvapich2\_gcc44-1.4.1. Log out and log in back.

  * remove your previous install of PETSc completely

`rm -rf name_of_the PETSc-directory`

  * If you have not done it yet, download the PETSc source

`wget  http://ftp.mcs.anl.gov/pub/petsc/release-snapshots/petsc-3.1-p8.tar.gz`

  * Uncompress it by running

`tar xzvf  petsc-3.1-p8.tar.gz`

  * Rename the newly created PETSc directory by

`mv petsc-3.1-p8 petsc-3.1-p8--mvapich2-gcc44`

  * Get inside, setup PETSC\_DIR, configure, and make PETSc by running

`cd petsc-3.1-p8--mvapich2-gcc44`

`export PETSC_DIR=$PWD`

`./config/configure.py --download-f-blas-lapack=1 --download-hypre=1 --download-blopex=1`

`make all`

`make test`

  * If requested, start the MPD service by running

`mpd &`

If this does not work, follow the instructions on the screen to make it to work.

  * Go to the driver and make it

`cd src/contrib/blopex/driver`

`make driver`

  * Now, you have the executable binary, which you can run. The simplest run is

`./driver`

If this gives no errors, the installation is successfully completed.

# Running #

  * After logging in, go to the right directory:

`cd work/petsc/petsc-3.1-p8-mvapich2-gcc44/src/contrib/blopex/driver`

where the driver is located. If you call the directory differently, modify the line above.

  * Running the driver does not require repeating any installation steps, except that you might need, if requested, to start the MPD service by running

`mpd &`

  * Driver possibilities are controlled by command line options, which could be set in any sequence. If you misspell the required name of the option, the code will give you a warning and use the default value instead. Use copy-paste to avoid the typos.

  * To control the number of the seeking eigenvalues, the tolerance, the max number of iterations, and the random generation of the initial approximation, try

`./driver -n_eigs 2  -tol 1e-3 -itr 50 -seed 2`

  * For the "seed" parameter, always use the number assigned to you in class.

  * The driver code finds the smallest eigenvalues of the 3D negative Laplacian with Dirichlet boundary conditions, approximated by the standard 7-point finite-difference scheme with the step size one in all 3 directions. The default problem size is 10x10x10. You can redefine the problem size, e.g., to 20x20x20 by running

`./driver -da_grid_x 20 -da_grid_y 20 -da_grid_z 20`

  * The exact eigenvalues for such problems are explicitly known. According to http://en.wikipedia.org/wiki/Kronecker_sum_of_discrete_Laplacians#Example:_3D_discrete_Laplacian_on_a_regular_grid_with_the_homogeneous_Dirichlet_boundary_condition , the smallest eigenvalue here is

```
lambda = 12*(sin(pi/(2*n+2)))^2
```

where n is the number of grid points in one direction, i.e., n=20 in the command above.
Compare it with the value obtained by the driver varying the tolerance.

In a general case (not needed for the project), download the MATLAB code http://www.mathworks.com/matlabcentral/fileexchange/27279-laplacian-in-1d-2d-or-3d and run it in MATLAB such as

`lambda = laplacian([20 20 20],{'DD' 'DD' 'DD'}, 1);`

to see the exact one eigenvalue lambda for the 20x20x20 grid.

  * Since we also have hypre installed, one can use it by

`./driver -pc_type hypre -pc_hypre_type boomeramg -pc_hypre_boomeramg_agg_nl 1 -pc_hypre_boomeramg_P_max 4 -pc_hypre_boomeramg_interp_type standard`

For large problems, these options are mandatory, since they significantly improve the code performance.

  * To run on the frontend with several (2 in this example) cores, use

`mpirun -np 2 ./driver`

The frontend has only 2x6=12 cores. NEVER run with mpirun with more than -np 12 on the frontend! Be considered to other users and limit yourself to -np 6 on the frontend or less as a general rule.

  * If you ssh to gross using the -X option:

`ssh -X gross`

you can open another terminal window by running

`gnome-terminal &`

In this separate terminal, run

`top`

to see how busy the frontend is at the moment and what other users and you are currently running. You can get out of the "top" window by pressing "q".

  * Compare the timing and the results of the following 3 runs:

```
./driver -da_grid_x 128 -da_grid_y 128 -da_grid_z 128
```

```
./driver -da_grid_x 128 -da_grid_y 128 -da_grid_z 128 -pc_type hypre -pc_hypre_type boomeramg -pc_hypre_boomeramg_agg_nl 1 -pc_hypre_boomeramg_P_max 4 -pc_hypre_boomeramg_interp_type standard
```


```
mpirun -np 6 ./driver -da_grid_x 128 -da_grid_y 128 -da_grid_z 128 -pc_type hypre -pc_hypre_type boomeramg -pc_hypre_boomeramg_agg_nl 1 -pc_hypre_boomeramg_P_max 4 -pc_hypre_boomeramg_interp_type standard
```

to see advantages of using hypre and running the code in parallel.

  * If you get the following error:

```
[5]PETSC ERROR: --------------------- Error Message ------------------------------------
[5]PETSC ERROR: Argument out of range!
[5]PETSC ERROR: Partition in y direction is too fine! 10 12!
```

this means that you use too many cores (12 in this case) for the default partitioning, which is in the y direction only (10 points in this case). Either reduce the number of cores requested, or use the new option "-freepart" In the latter case, the partitioning will be done automatically, which should work for most practical cases.

  * To run on the cluster nodes, use the submitter, called PBS. Create the file called "run" with the following lines

```
# Reserve nodes and cores per node
# each node contains 12 processing cores, so ppn<13
#PBS -l nodes=6:ppn=12

# Set the maximum amount of the time the job will run (HH:MM:SS)
#PBS -l walltime=01:00:00

# Give the job a name
#PBS -N test_pbs

# Keep all environment variables from the current session (PATH, LD_LIBRARY_PATH, etc)
#PBS -V

# Merge stderr and stdout
#PBS -j oe

# Set log file
#PBS -o test.log

# Change to the run directory (where job was submitted)
cd $PBS_O_WORKDIR

# Execute the program through mpi with the machine file specified by PBS.
# If the binary supports OpenMP, we should specify the number of thread to use
# per process using the OMP_NUM_THREADS variable.
mpirun_rsh -np 72 -hostfile $PBS_NODEFILE OMP_NUM_THREADS=1/home/aknyazev/work/petsc/petsc-3.1-p8-mvapich2-gcc44/src/contrib/blopex/driver/driver -da_grid_x 128 -da_grid_y 128 -da_grid_z 128 -pc_type hypre -pc_hypre_type boomeramg -pc_hypre_boomeramg_agg_nl 1 -pc_hypre_boomeramg_P_max 4 -pc_hypre_boomeramg_interp_type standard -freepart

```

In the last line, edit the location of the file "run" to match your location.
The "np" parameter must be equal to the product of the "nodes" and "ppn".

The gross cluster has 12 compute nodes with 2x6=12 cores each, i.e. 144 cores total. So you use all nodes by taking "nodes=12:ppn=12" and "-np 144" In the example above, half of the nodes are used.

  * Submit the job by running

`qsub run`

You can check the status of your and other runs by

`qstat`

`qstat -n`

`qstat -f`

To delete your job from the queue, if it is not running yet, use

`qdel 1578.frontend`

where you need to put your actual job ID number instead of 1578

  * Use http://ccm.ucdenver.edu/ganglia/gross/ to check which nodes are up and how busy they are. A more reliable, but not so nice looking check is by running

`pbsnodes`

and

`uptimetest0`

  * You can also login to one of the nodes that the qsub has used for your code and monitor the execution on this node by running the "top" command:

`ssh node01`

`top`

  * If a run goes out of memory, it crashes, disappears from "qstat", and may generates a cryptic looking error. Unfortunately, a check at http://ccm.ucdenver.edu/ganglia/gross/ may show that your processes are still running on the nodes, on their own. To play it safe, if anything strange  happens to your run, make sure clean after yourself, i.e. to kill all your jobs after that, by running

```
pdsh -a killall -u $USER
```

  * Check the directory for the error and output files, produced by the qsub. The files have the ID number of the run as a part of their filename. If you use the script above, the main output file will be called test.log. It will be overwritten by every new qsub run.

# Project #

Run the code for grid sizes ranging from 8x8x8 to 512x512x512 (use powers of 2) and tolerances 1e-3, 1e-4, 1e-5, 1e-6. Always use all hypre options and your personal seed! After every run record the outcome, e.g., the output of the following run

```
mpirun -np 6 ./driver -da_grid_x 256 -da_grid_y 256 -da_grid_z 256 -pc_type hypre -pc_hypre_type boomeramg -pc_hypre_boomeramg_agg_nl 1 -pc_hypre_boomeramg_P_max 4 -pc_hypre_boomeramg_interp_type standard -freepart -n_eigs 1 -itr 50 -seed 4 -tol 1e-5
```

contains

```
Eigenvalue lambda       Residual              
  4.48280009467367e-04    1.77597752359368e-06
```

so the record for this run is

```
256  4.48280009467367e-04    1.77597752359368e-06
```

You may want to consider simply putting all small size jobs into one "run" script and run them all together by qsub, rather then doing them manually one-by-one. Just repeat the last line of the run script, changing only the sizes and tolerances. One can only once specify "nodes" and "ppn" values in the run script, so for all small jobs just use nodes=1:ppn=1 and -np 1.

You can also group big jobs together, as soon as you use the same "np" number for all jobs within the run script, e.g., for the same problem size just different tolerances.

Using a text editor, create a text file with a table with the records for all your runs.
The table thus looks like this

```
16   1.02161402101086e-01    1.70625751483571e-05
16   1.02161401908252e-01    3.55468264043989e-06
128  1.77918128825137e-03    9.90977710961163e-06
256  4.48280009467367e-04    1.77597752359368e-06
```

For every row of the table, read it into MATLAB, e.g.,

```
PETSc_output = [16   1.02161401908252e-01    3.55468264043989e-06];
n=PETSc_output(1);
Eigenvalue_lambda = PETSc_output(2);  
Residual = PETSc_output(3);
```

Calculate the exact eigenvalue by

```
lambda = 12*(sin(pi/(2*n+2)))^2;
```

The following quantity compares the eigenvalue accuracy with the Residual:
```
test_ratio = (Eigenvalue_lambda -lambda)/(Residual^2)
```

The conjecture is that the test\_ratio is roughly a constant,
so that the value of Residual^2 allows predicting the eigenvalue accuracy.

The table of the raw data of PETSc runs, allows computing
test\_ratio for various values of Residual and different values of n.
Use MATLAB's BAR command with the 'stacked' option to plot
test\_ratio as a function of n. Stack the values of test\_ratio for various values of Residual for a given n into a single bar, such as in http://www.mathworks.com/help/techdoc/creating_plots/f10-19972.html#f10-992

The electronic project report must contain:
  1. the text file with the table,
  1. the text file containing all the executed commands to generate the values of the table, in the same order as the rows of the table, and
  1. the figure generated by the MATLAB's BAR command. The figure must have the title, legend, and annotated axes, so that it is self-contained and requires no explanations.