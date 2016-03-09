

# Introduction #

Testing blopex\_serial\_complex with Intel's MKL.System configuration:

  * Intel(R) Pentium(R) ,4 CPU ,3.06GHz,3.24 GB of RAM
  * Intel® Math Kernel Library (Intel® MKL) 10.1.1.019

This is included as an example of the kinds of errors that can occur and how to resolve them. In particular of modifications for the mkl libraries. If you encounter problem review the section How you get to the summary.


# Summary #

Change directories to the blopex\_serial\_complex directory.

Replace LAPACKLIB in Makefile.inc
```
[peizhen@localhost blopex_serial_complex]$ cat Makefile.inc

LOBPCG_ROOT_DIR =  /home/peizhen/blopex/work/trunk/blopex_abstract

CFLAGS = -Wall -g

#LAPACKLIB = -llapack

LAPACKLIB =-L/opt/intel/mkl/10.1.1.019/lib/em64t -lmkl_lapack -lmkl -lguide -lpthread -lm
```

Set LD\_LIBRARY\_PATH
```
> export LD_LIBRARY_PATH=/opt/intel/mkl/10.1.1.019/lib/em64t
```

Execute the test.
```
> cd driver
> ./serial_driver R 100 10
```

# Testing Results #

**Randam
```
Eigenvalue lambda       Residual              

-1.1210091685718075e+01  4.7967133067112476e-07

-1.0501158146809949e+01  4.4024260976594814e-07

-9.8940023904400078e+00  5.3411187106040125e-07

-9.7773232788479678e+00  8.8890256760028152e-07

-9.5346640107775045e+00  8.0222426140228691e-07

-9.2501084356249397e+00  6.7195348524474723e-07

-8.9349365221118866e+00  8.5500401515418215e-07

-8.8367798054460707e+00  6.3309352212388061e-07

-8.3516247436515059e+00  7.9788233026479031e-07

-7.8577158442231561e+00  9.5954674358993465e-07



86 iterations

CPU time used = 5.1000000000000001e-01 
```**


**Test1**

```
> ./serial_driver L test1a.txt test1x.txt
```
```
Eigenvalue lambda       Residual              

-6.7895342370587421e+00  6.9799181253781465e-07

-6.1261513438048025e+00  6.8987502524749751e-07

-5.8103745271698468e+00  6.0087667580829678e-07

-5.2386164182061625e+00  6.5674825770501647e-07

-4.8808702215552477e+00  7.5849063811788438e-07



31 iterations

CPU time used = 5.0000000000000003e-02 
```


**Test2**

```
>./serial_driver L test2a.txt test2x.txt
```
```
Eigenvalue lambda       Residual              

-1.1080244951277223e+01  8.7715878862374474e-07

-1.0619031628691189e+01  8.3380664305142488e-07

-1.0091481158767715e+01  9.3749017580222592e-07

-9.9117793865263479e+00  8.9288871882945691e-07

-9.7514313021142804e+00  9.7493457430970217e-07

-9.3207970589076528e+00  9.8446954467812580e-07

-8.8124520006213061e+00  6.4165207660661804e-07

-8.5728211942249271e+00  7.6010455455398733e-07

-8.3669043128529239e+00  9.8320707186776185e-07

-7.9116588717468836e+00  7.6053049306665909e-07



76 iterations

CPU time used = 6.6000000000000003e-01 
```

# How you get to the summary #

**Replace LAPACKLIB in Makefile.inc
```
[peizhen@localhost blopex_serial_complex]$ cat Makefile.inc

LOBPCG_ROOT_DIR =  /home/peizhen/blopex/work/trunk/blopex_abstract

CFLAGS = -Wall -g

#LAPACKLIB = -lla
LAPACKLIB =-L/opt/intel/mkl/10.1.1.019/lib/64 -lmkl_lapack -lmkl -lguide -lpthread 
```**

**Get errors
```
usr/bin/ld: skipping incompatible /opt/intel/mkl/10.1.1.019/lib/64/libmkl_intel_lp64.a when searching for libmkl_intel_lp64.a

/usr/bin/ld: cannot find libmkl_intel_lp64.a
```**

I found there are three folds 32, 64 and em64t in lib. Since the error is “incompatible”. Then, I tried em64t.

**Change Lapack to
```
LAPACKLIB =-L/opt/intel/mkl/10.1.1.019/lib/em64t -lmkl_lapack -lmkl -lguide -lpthread
```**

**Get errors again:
```
in function `lobpcg_ComputeResidualNorms':

lobpcg.c:(.text+0x660): undefined reference to `sqrt'

/opt/intel/mkl/10.1.1.019/lib/em64t/libmkl_lapack.so: undefined reference to `sin'

/opt/intel/mkl/10.1.1.019/lib/em64t/libmkl_lapack.so: undefined reference to `fabs'

/opt/intel/mkl/10.1.1.019/lib/em64t/libmkl_lapack.so: undefined reference to `exp'

/opt/intel/mkl/10.1.1.019/lib/em64t/libmkl_lapack.so: undefined reference to `cos'

/opt/intel/mkl/10.1.1.019/lib/em64t/libmkl_lapack.so: undefined reference to `log'

/opt/intel/mkl/10.1.1.019/lib/em64t/libmkl_lapack.so: undefined reference to `pow'

/opt/intel/mkl/10.1.1.019/lib/em64t/libmkl_lapack.so: undefined reference to `log10'

/opt/intel/mkl/10.1.1.019/lib/em64t/libmkl_lapack.so: undefined reference to `ceil'

collect2: ld returned 1 exit status

make[1]: *** [serial_driver] Error 1

make[1]: Leaving directory `/home/peizhen/blopex/work/trunk/blopex_serial_complex/driver'

make: *** [driver] Error 2
```
The -lm to pickup C math routines is explicitly needed.**

**Change Lapack to
```
LAPACKLIB =-L/opt/intel/mkl/10.1.1.019/lib/em64t -lmkl_lapack -lmkl -lguide -lpthread -lm
```**

Note: Do not foget to set LD\_LIBRARY\_PATH=/opt/intel/mkl/10.1.1.019/lib/em64t

