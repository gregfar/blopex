

# changes to blopex\_abstract #

## support for complex arithmetic ##

This was done 2 years ago and have been through a round of testing by everyone.   But now we need to retest due to recent changes.

## support for 64 bit integer arithmetic ##

Up until now blopex supported only array dimensions of size "int".  The size of "int" is dependent on the complier and system.  Typically this is 32 bit; that is integers of size 2,147,483,647.   However, the newest versions of matlab implement versions of lapack which use 64 bit arithmetic which can be in conflict with the size of "int".

To provide for a way to support this we implement an integer size called `BlopexInt` which is defined via the  c preprocessor.  This defaults to `"#define BlopexInt int"`.   To override; specify `BlopexInt` in the compiler command.  For example to make it a long integer add  `"-DBlopexInt=long"` to the compiler command.

The default for `BlopexInt` occurs in a new header `../blopex_abstract/utilities/fortran_options.h`.  This is included in various other blopex\_abstract headers.   The location and name for this header was chosen to maximize compatibility with the existing HYPRE Makefile's.

Note that `BlopexInt` will be used in the function calls to the lapack functions called from lobpcg.c.  If this is not compatible with the lapack function definitions a pointer compiler error can occur.   To get around this another level of redirection will need to be coded in the interface where type casting can be done.  This is the case for the matlab interface (also, we have always done this in the Petsc interface).  We could recode the calls in lobpcg.c but as a general rule we want to avoid this.  More on this under "changes to blopex\_matlab".

## support for hypre\_assert macro ##

Hypre has recoded some of our abstract modules to use their hypre\_assert macro instead of the standard c assert function.  To support this but still use the c assert for the other interfaces, we have added a preprocessor variable called `BlopexAssert`.   This is implemented along with `BlopexInt` in fortran\_options.h.   The HYPRE version of  fortran\_options.h contains an include for "_hypre\_utilities.h"  which is commented out for the version used for the other interfaces.  "_hypre\_utilities.h" contains the macro definition for hypre\_assert.

## other changes for HYPRE ##

There are two minor changes done for HYPRE which apply to all interfaces.

  1. format %22.16e changed to %22.14e.
  1. in temp\_multivector.h  `"typedef struct mv_TempMultiVector * mv_tempMultiVectorPtr"`
changed to `"typedef mv_TempMultiVector * mv_tempMultiVectorPtr"`

# changes to blopex\_hypre #

## modifications due to blopex\_abstract changes ##

HYPRE only supports double (real) arithmetic for array elements and 32 bit integer.  The following changes are needed due to the new complex code.  These are the files that should be given to HYPRE for upgrading our code in HYPRE.

```
krylov/lobpcg.c			"multiple changes
krylov/lobpcg.h			"multiple changes
krylov/HYPRE_lobpcg.c		"change lobpcg_solve to lobpcg_solve_double
multivector/multivector.c	"multiple changes
multivector/multivector.h	"multiple changes
multivector/temp_multivector.c	"multiple changes
multivector/temp_multivector.h	"multiple changes
multivector/interpreter.h	"multiple changes
utilities/fortran_matrix.c	"multiple changes
utilities/fortran_matrix.h	"multiple changes
utilities/fortran_interpreter.h	"new blopex_abstract module
utilities/fortran_options.h	"new blopex_abstract module (with #include "_hypre_utilities.h"
parcsr_ls/HYPRE_parcsr_int.c	"add WRAPPER for hypre_ParKrylovInnerProd and hypre_ParKrylovAxpy
parcsr_ls/ame.c			"change lobpcg_solve to lobpcg_solve_double
struct_ls/HYPRE_struct_int.c	"add WRAPPER for hypre_StructKrylovInnerProd and hypre_StructKrylovAxpy
sstruct_ls/HYPRE_sstruct_int.c	"add WRAPPER for hypre_SStructKrylovInnerProd and Hypre_SStructKrylovAxpy
test/ex11.c			"new test: change include HYPRE_parcsr_int.h to _hypre_parcsr_ls.h
```

Note that the starting code for all of the HYPRE_modules is version hypre-2.6.0b.  All of our previous testing was done with code from an earlier version._

The tar file hypre\_lobpcg\_modifications.tar.gz that we have been using for testing has been changed to reflect the new changes and is on the google source site under the downloads tab.

## support for hypre\_assert macro ##

lobpcg.c, multivector.c, and temp\_multivector.c use hypre\_assert instead of assert.  This is controlled via fortran\_options.h as described under "changes to blopex\_abstract".

## New ex11.c test ##

This is the new test we were given in the last round of communication with HYPRE prior to the hypre-2.6.0b release.

It was not included in hypre-2.6.0b.  So to test we have to go through the process of modifying the Makefile in hypre-2.6.0b/src/test/.   This is documented in document ex11\_testing\_in\_xvia.txt included in the same email as this document.

## Things to know about hypre-2.6.0b ##

Beware of the following with hypre-2.6.0b.  When doing a make (even with the unmodified files from the hypre install_, the following error occurs.
```
cp: '/home/grads/dmccuan/hypre-2.6.0b/src/hypre/lib/libHYPRE.a' and `/home/grads/dmccuan/grads/dmccuan/hypre-2.6.0b/src/hypre/lib/./libHYPRE.a' are the same file
make: *** [install] Error 1
```
Note that after this error you can successfully do the make in the src/test/ directory._

But when trying to run a sstruct test you get the following:
```
[dmccuan@xvia test]$ mpirun -np 2 ./sstruct -P 1 1 2 -lobpcg -solver 10 -tol 1.e-6 -pcgitr 0 -seed 1 -vrand 10
Error: can't open input file sstruct.in.default
rank 0 in job 12  xvia.cudenver.edu_55315   caused collective abort of all ranks
  exit status of rank 0: return code 1 
```

If appears that the make under the /hypre-2.6.0b/src/ directory stopped before copying sstruct.in.default from
from directory .../src/test/TEST\_sstruct to ../src/test/.  If you do this manually then the sstruct test runs OK.

Files HYPRE\_parcsr\_int.h, HYPRE\_sstruct\_int.h, and HYPRE\_struct\_ls.h are no more.  They are replaced by
> _hypre\_parcsr\_ls.h,_hypre\_struct\_ls.h, and _hypre\_sstruct\_int.h.  Note that these new headers contain many more function definitions than existed in the previous HYPRE\_xxxxx headers.  It appears that HYPRE has consolidated a number of headers._


# changes to blopex\_matlab #

## support for 64 bit versions of matlab ##

Currently the default version of matlab in math or xvia is `Version 7.8.0.347 (R2009a) 64-bit (glnxa64)`.

This uses a version of lapack where the interger variables to lapack functions are defined as ptrdiff\_t.

This has resulted in MKL errors within the lapack function and caused us considerable difficulties last fall.

Sinced then Matlab has provided some documentation (see Calling LAPACK and BLAS Functions from MEX-Files: under http://www.mathworks.com/access/helpdesk/help/techdoc/matlab_external/f13120.html).  Using this the issue has been resolved.

First, support of 64 integers was added to blopex\_abstract via the use of preprocessor variable `BlopexInt`.

Then, instead of assigning dpotrf_to blap\_fn.dpotrf  we assign a new function dpotrf\_redirect.  This function is defined in matlab\_interface.c which includes the matlab header file lapack.h.  dpotrf\_redirect recasts the variables (`BlopexInt`) into a form compatible with the matlab definitions (i.e. mwSignedIndex).   The same is done for dsygv_, zpotrf_, and zhegv_.

Then some changes to the blopex\_matlab Makefile are needed.

1.`CFLAGS = -g -largeArrayDims  or CFLAGS = -g -largeArrayDims -DBlopexInt=long`

You must be consistent in the blopex\_abstract make versus the blopex\_matlab make for the `BlopexInt` definition.
You can just leave it off in which case the default of "int" will be used and this will execute OK (assuming your array dimension is less than 2,147,483,647 ).  Be sure to run the make for blopex\_abstract first.
The `largeArrayDims` option supplied insures mex is compatible with 64 bit.  This is an option to use 32 bit but this doesn't seem to apply to the lapack functions and so it won't work.  Matlab warns that `largeArrayDims` will soon be the default and the 32 bit option will not be supported.

2. Specify -lmwlapack instead of "lapack" for the lapack library.

If you don't do this you can get some contradictory results.  What appears to be happening when we specify "lapack" is we are compiling for the standard 32 bit lapack shared library.  Remember that the shared library must also be determined at execution time and may not be the same.   The fact that our mex function is not the options main (that's apparently matlab itself) and the fact that matlab modifies the LD\_LIBRARY\_PATH on execution complicates the situation.   I tried for 2 days to force it to use the standard 32 bit lapack library with no success and finally used Matlabs examples from their web site to code to their lapack.h definitions and the -lmwlapack shared library.

This illustrates the difficulty of supporting 64 bit lapack libraries.  Depending on the parameter types, some kind of redirection and recasting of variables may be needed.   This should be done in the interface and potentially will vary from library to library.

## support for complex arithematic ##

Eugene last fall was able to modify the matlab interface (32 bit) to support complex arithematic.
I have made some adjustments to this code to add 64 bit integer support (`BlopexInt`) and redirection of lapack functions to handle casting of parameters.  In addition the code was setup to compile to either double or complex support. This was implemented via a preprocessor.  I have modified this to support both double and complex without recompilation.  To do this in blopex\_matlab\_gateway.c  I define BLOPEX\_MATLAB\_COMPLEX as an external variable rather than a preprocessor variable,  then check whether OperatorA is complex and set BLOPEX\_MATLAB\_COMPLEX = 1 if true and 0 otherwise.  Then in the remainder of blopex\_matlab\_gateway.c and in matlab\_interface.c we do a regular c "if" statement with BLOPEX\_MATLAB\_COMPLEX rather than a preprocessor "#ifdef" statement.

Eugene has also added new tests for both complex and double.

To test, after starting matlab you will need to add paths ../blopex\_matlab/driver, ../blopex\_matlab/driver/test\_complex,  ../blopex\_matlab/driver/test\_real, and ../blopex\_matlab/matlab\_interface/m\_files.  Other than this the instructions given by Eugene under wiki TestBLOPEXMatlabInterfaceLinux should be followed.

## 32 bit support ##

Matlab interface has code to support both 64 and 32 bit matlab.  To compile for 32 bit 1st add path to 32 bit matlab  if it is not the default (on xvia;  PATH=/opt/matlab\_2009a\_32/bin:$PATH; export PATH).  Then specify "make matlab=32".   If the architecture is not glnx86, you may have to modify the Makefile.

To test start matlab with "matlab glnx86 -nojvm".  Then add paths as specified under "support for complex arithmetic.

Warning!:  Testing on xvia was not completed.   The mex command (as in CC = mex) is a script which constructs the architecture to use from /bin/uname and does not allow it to be overridden (this may be a bug).  It needs to be glnx86 for the 32 bit matlab but tries to link using glnxa64 which leads to an invalid subdirectory under /opt/matlab\_2009a\_32/extern/lib.

# changes to blopex\_petsc #

## support for complex arithematic ##

We have tested this before.  Just need to retest.

## support for 64 bit arithematic ##

`BlopexInt` changes are in place in petsc-interface.c.  Note that Petsc lapack funtions are LAPACKpotrf_, LAPACKsygv_, etc. The integer parameters in these functions are defined as `PetscBLASInt`.  These functions get called via redirection functions PETSC\_dpotrf\_interface, PETSC\_dsygv\_interface etc. where `BlopexInt` is recast as `PetscBLASInt`.

Should  PETSC start to use 64 bit lapack routines we should be compatible using `BlopexInt` as either int or long.

# changes for blopex\_serial\_double #

## modifications due to blopex\_abstract changes ##

Since this interface is only for double (real); the only change is to call lobpcg\_solve\_double and interface function parameters must conform to new definitions (mostly using `void* in place of double*`).  Also `BlopexInt` has been added.

This should be tested exactly the same as we did last fall.

# changes for blopex\_serial\_complex #

## modifications due to blopex\_abstract changes ##

This is a new interface for testing of complex changes.  It calls lobpcg\_solve\_complex.   `BlopexInt` has been added.

This should be tested exactly the same as we did last fall.