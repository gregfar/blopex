

# Introduction #

How to test the complex serial driver for BLOPEX.

This is a c program  (serial\_driver) which was created for testing of the
complex additions to blopex\_abstract.  It is similar to blopex\_serial\_double but
  1. deals only with complex matrices,
  1. matmultivec, pcg\_multi, and multivector routines have been modified to handle complex matrices and vectors,
  1. calls lobpcg\_solve\_complex, and
  1. the serial\_driver.c has been enhanced for additional test options.

The serial\_driver does not require installation of Petsc or Hypre.

This interface is for does not utilize is for basic testing of the BLOPEX code and does not does not utilize mpirun.

# Compile/Link serial\_driver #

For the following assume all BLOPEX source is installed uner directory BLOPEX.

If BLOPEX library has not been created then do this first.

Change directory to BLOPEX abstract code directory and issue make command.
```
>>cd BLOPEX/blopex_abstract
>>make
```

Note: On some computers, one may have to edit Makefile.inc in the blopex\_abstract directory and change the lines:

```
# cc = gcc
 cc = mex
```

to

```
 cc = gcc
# cc = mex
```

as the mex compiler may not be installed.


Next, change directory to the code for the complex serial\_driver.
```
>>cd BLOPEX/blopex_serial_complex
```
To create the executable for serial\_driver, the blopex\_abstract objects must be compiled and Makefile.inc in ../blopex\_serial\_double updated with appropriate locations for blopex\_abstract, compile flags, and lapack library.

For example:
```
LOBPCG_ROOT_DIR =  ../../blopex_abstract
CFLAGS = -Wall -g
LAPACKLIB = -llapack
```
Then, issue command
```
>> make
```
This should create a test executable "driver/serial\_driver"

# Test Execution #

To run the executible (which is in ../blopex\_serial\_double/driver), shift to that directory and enter command
```
>> ./serial_driver R 100 10 
```
or
```
>> ./serial_driver L test1a.txt test1x.txt
```
The first form creates a 100x100 random hermitian matrix and solves for 10 eigenvalues.

The second form loads matrix from test1a.txt, initial eigenvectors from test1x.txt and
solves for the the same number of eigenvalues as initial eigenvectors.

# Test file creation #

These files are formated as follows: ascii lines with cr.
1st line is '%d %d\n'  number of rows, number of columns.
All other lines, '%22.16e %22.16e\n'   real part of number, imaginary part of number.
Numbers are in standard fortran row rank order.

These files can be created in Matlab by 1st setting up the matrix and saving using the
following (matrix in variable x and x is complex).
```
fid = fopen('c:\text1a.txt','w');
[row,col]=size(x);
fprintf(fid,'%d %d\n',row,col);
for j=1:4 
   for i=1:6 
      fprintf(fid,'%22.16e   %22.16e\n',real(x(i,j)),imag(x(i,j)));
   end
end
fclose(fid);
```

The matrix to solve should be hermitian positive definite.

The matrix of initial values should have the same number of rows as the matrix to solve and the number of columns determines the number of eivenvalues to solve for.

# Test Results #

## test1 ##

./serial\_driver L test1a.txt test1x.txt

Eigenvalues computed
```
Eigenvalue lambda       Residual              
-6.7895342371693745e+00  6.9799183444283968e-07
-6.1261513439877628e+00  6.8987519527870938e-07
-5.8103745267211648e+00  6.0087666718421241e-07
-5.2386164184952140e+00  6.5674786833799589e-07
-4.8808702224124803e+00  7.5848791467513662e-07

31 iterations
CPU time used = 4.1957000000000001e-02 

```

## test2 ##

./serial\_driver L test2a.txt test2x.txt

Eigenvalues computed
```
Eigenvalue lambda       Residual              
-1.1080244958656804e+01  8.7715396842764707e-07
-1.0619031629224237e+01  8.3380688568426980e-07
-1.0091481158509795e+01  9.3749198003235374e-07
-9.9117793842421449e+00  8.9288849369553002e-07
-9.7514313051991888e+00  9.7493416067963172e-07
-9.3207970608165578e+00  9.8447217142738573e-07
-8.8124520024677189e+00  6.4165207440313912e-07
-8.5728211944494994e+00  7.6010455033173900e-07
-8.3669043134042269e+00  9.8320903772396290e-07
-7.9116588662085574e+00  7.6053312739064901e-07

76 iterations
CPU time used = 6.3530100000000000e-01 

```


## random ##

./serial\_driver R 100 10

Eigenvalues computed
```
Eigenvalue lambda       Residual              
-1.1210091685718119e+01  4.7967132991532764e-07
-1.0501158146809857e+01  4.4024260766008527e-07
-9.8940023904399990e+00  5.3411187229378858e-07
-9.7773232788479785e+00  8.8890256227145167e-07
-9.5346640107774565e+00  8.0222425924588777e-07
-9.2501084356249024e+00  6.7195348176370916e-07
-8.9349365221119932e+00  8.5500400698241940e-07
-8.8367798054460582e+00  6.3309352129526724e-07
-8.3516247436515307e+00  7.9788232965401455e-07
-7.8577158442232040e+00  9.5954673989161707e-07

86 iterations
CPU time used = 5.2436099999999997e-01 
```