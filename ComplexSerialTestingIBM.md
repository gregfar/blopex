

# Introduction #

How to test the complex serial driver for BLOPEX on the IBM Bluefire ucar machine.

This is done as described in the Wiki ComplexSerialTestingLinux, but there are some changes to the Makefiles required for the Bluefire renvironment.

# BLOPEX abstract compilation #

On Bluefire the make utility does not as a default include the CPPFLAGS in the object compilation.  In the .../blopex\_abstract/krylov/Makefile add the following line after the lobpcg.o: ... line.
```
lobpcg.o: lobpcg.c lobpcg.h ../utilities/fortran_matrix.h
        $(CC) $(CPPFLAGS) $(CFLAGS)  -c $< -o $@
```

# Complex serial interface compilation #

In the .../blopex\_serial\_complex directory the Makefile.inc needs the following Library search path added to LAPACKLIB
```
LAPACKLIB = -llapack -L/usr/local/lib64/r414/
```

The xlc compiler does not default the C math library so we need to add -lm and several extra libraries are needed.  Also the make utility does not handle the $^ command the same and the CPPFLAGS is used.  So, in  .../blopex\_serial\_complex/driver/Makefile we need the following changes
```
lobpcglibLDFLAGS = -L$(LOBPCG_ROOT_DIR)/lib -lBLOPEX -lm -lessl -lxlf90

serial_driver: serial_driver.o $(objfiles)
     $(CC)lobpcglibLDFLAGS = -L$(LOBPCG_ROOT_DIR)/lib -lBLOPEX -lm -lessl  -lxlf90

serial_driver: serial_driver.o $(objfiles)
        $(CC) -o $@ serial_driver.o $(objfiles) $(LAPACKLIB) $(lobpcglibLDFLAGS) 
        
serial_driver.o: serial_driver.c $(headers)
        $(CC) $(CPPFLAGS) $(CFLAGS)  -c $< -o $@
```

Finally, the .../blopex\_serial\_complex/matmultivec needs the following change due to the CPPFLAGS.
```
matmultivec.o: matmultivec.c matmultivec.h
        $(CC) $(CPPFLAGS) $(CFLAG)  -c $< -o $@
```