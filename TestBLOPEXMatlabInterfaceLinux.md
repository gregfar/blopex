

# Introduction #

Testing Matlab interface to BLOPEX. BLOPEX LOBPCG computes 4th, 5th and 6th smallest eigenpairs of the diagonal n-by-n matrix spdiags((1:n)',0,n,n), n = 10000, given the
first 3 eigenvectors as initial constraints. The BLOPEX results are then compared to the Matlab version of lobpcg (lobpcg.m) results. System configuration:
  * Fedora 10 OS, 4 Quad Core Opteron 2.0 Ghz CPUs, 64 GB RAM;
  * Open MPI version 1.2.4;
  * Compiler version gcc (GCC) 4.3.2 20081105 (Red Hat 4.3.2-7);
  * LAPACK version 3.1.1
  * Matlab version 7.7.0.471 (R2008b)


# Setup Matlab interface to the stable version of BLOPEX #

The _stable_ version of BLOPEX is the code currently available at http://www-math.cudenver.edu/~aknyazev/software/BLOPEX/ - the first BLOPEX release (before complex arithmetic changes).

Using the above link download the tarballs blopex\_abstract.tgz (abstract BLOPEX kernel) and
blopex\_matlab.tgz (Matlab interface to BLOPEX). Extract files:
```
tar -zxvf blopex_abstract.tgz 
tar -zxvf blopex_matlab.tgz 
```
This creates directory _blopex_ with two subdirectories: _blopex\_abstract_ and _blopex\_matlab_.

Before compiling, add a search path for the matlab mex compiler by issuing
```
PATH=$PATH:/opt/matlab/bin
export PATH
```
Then go to blopex/blopex\_abstract and modify Makefile.inc by specifying the mex compiler (add line CC = mex):
```
CFLAGS =

AR = ar -rvcu
RANLIB = ranlib

CC = mex
```
Save changes to Makefile.inc and run make. This builds blopex\_abstract with mex compiler.

Now shift to blopex\_matlab and modify the Makefile to add LAPACK to the linking process (add ''-llapack''):
```
........
driver/blopex_matlab_gateway.mexglx: blopex_matlab_gateway.c multivector_for_matlab.h matlab_interface.h \
                                     multivector_for_matlab.o matlab_interface.o
        $(CC) $(CFLAGS) $(CPPFLAGS) $< matlab_interface/c_files/multivector_for_matlab.o \
         matlab_interface/c_files/matlab_interface.o ''-llapack'' $(LIB_DIRS) $(LIBS) -outdir $(@D)
........
```
Save changes to Makefile and run make. This compiles the Matlab interface to BLOPEX (also with mex compiler).

# Execution of tests for Matlab interface to the stable version of BLOPEX #

Start Matlab and add directories ".../blopex/blopex\_matlab/driver" and ".../blopex/blopex\_matlab/matlab\_interface/m\_files" to Matlab path:
```
>> addpath('/home/grads/.../blopex/blopex_matlab/driver')
>> addpath('/home/grads/.../blopex/blopex_matlab/matlab_interface/m_files')
```
Run the script ".../blopex/blopex\_matlab/driver/test\_blopex\_matlab.m":
```
test_blopex_matlab

The full initial guess with 10000 colunms and 3 raws is detected  
The main operator is detected as a sparse matrix 
The second operator of the generalized eigenproblem 
is detected as a sparse matrix 
The preconditioner is detected as an M-function precond 
The full matrix of 3 constraints is detected 
Solving with tolerance 1.000000e-04 and maximum number of iterations 40 

Solving generalized eigenvalue problem with preconditioning

block size 3

3 constraints

...........
Eigenvalue lambda 4.0000000000002425e+00
Eigenvalue lambda 5.0000000000009530e+00
Eigenvalue lambda 6.0000000001127898e+00
Residual 3.0668655987253611e-05
Residual 5.4190180513113715e-05
Residual 8.5218964177237057e-05

15 iterations

SOLUTION TIME, blopex_matlab: 0.637745

The full initial guess with 10000 colunms and 3 raws is detected  
...........
Final Eigenvalues lambda 4.0000000000000222e+00 
Final Eigenvalues lambda 5.0000000000007034e+00 
Final Eigenvalues lambda 6.0000000001121920e+00 
Final Residual Norms 2.057962e-06 
Final Residual Norms 1.494745e-05 
Final Residual Norms 8.521897e-05 

SOLUTION TIME, lobpcg.m: 0.433134
```
The desired tolerance (1e-4) was achieved in 15 iterations for both (Matlab-BLOPEX and Matlab-lobpcg.m) runs.
