

# How Petsc works with Blopex #

First lets review how Blopex is incorporated into Petsc.
There are two components:  blopex\_abstract and blopex\_interface.
The blopex\_abstract is incorporated as an external package.

This can be specified during configuration with the option "--with-blopex=1"
in which case "blopex\_abstract\_Aug\_2006.tar.gz" is ftped from the http://ftp.mcs.anl.gov/pub/petsc/externalpackages/ site.

It can also be specified during configuration with the option "--download-blopex=/PATH/TO/package.tar.gz" if already downloaded and saved locally.
For example: I have created a zipped tar file on my system with the blopex\_abstract files and called it /home/grads/dmccuan/blopex\_petsc\_abstract.tar.gz.  To use this I would specify the configuration option `--download-blopex=$HOME/blopex_petsc_abstract.tar.gz`

In either case under the $PETSC\_DIR directory a subdirectory "externalpackages/blopex\_abstract" will be created and the files from the tar installed there, compiled, and then the header files moved to $PETSC\_DEV/$PETSC\_ARCH subdirectory /include, the object files moved to /lib,  and the BLOPEX library created and moved to /lib/libBLOPEX.a. Note that on xvia $PETSC\_ARCH=linux-gnu-c-debug.  Also, note that if externalpackages/blopex\_abstract already exists from a previous configuration, it will not be replaced with what your configuration specifies.

All of this happens during the configuration step.

Now the blopex\_petsc interface files which include the drivers and petsc-interface are not a part of the blopex\_abstract tar.  These files are included as part of the petsc distribution tar and are located in $PETSC\_DIR/src/contrib/blopex.

Configuration in Petsc is controlled via python scripts. The script petsc-dev/config/PETSc/packages/blopex.py contains flags to control configuration when the --download-blopex option is used.  It contains the following line (about line 15) "self.complex    = 0".  This specifies that the blopex interface is not defined for Petsc scalar type complex.  Edit blopex.py and change this to "self.complex    = 1". Without this change configuration will give the error _"cannot use blopex with complex numbers it is not coded for this capability"_.  Warning: On some systems (Frost) the configuration will abort when you get this message, but on others (Linux) it doesn't and while you can continue with the make you will get bad results with the complex tests.

These are the flags that must be set if blopex is compatible with complex and 64bit data types.
```
configure option --with-scalar-type=complex requires flag self.complex = 0.
configure option --with-64-bit-indices requires flag self.requires32bitint = 0.
```

Note that version 1.0 of Blopex does not support these options.  They are new for version 1.1.

After the petsc configuration step when you do a "make all test"; the petsc-interface.c gets compiled and the object saved in libpetsc.a which is located in $PETSC\_DEV/$PETSC\_ARCH/lib.  Note: prior to Petsc 3.1 it was saved in libpetscksp.a.

Note that the drivers are not compiled in this step.  Now the driver can be compiled and linked by changing to the drivers subdirectory and executing the make (for example: in src/contrib/blopex/driver execute "make driver").

# Testing Petsc with BLOPEX changes #

The changes needed to incorporate a new release of BLOPEX into PETSC are defined in two tar files.
```
 http://blopex.googlecode.com/files/blopex_petsc_abstract.tar.gz
 http://blopex.googlecode.com/files/blopex_petsc_interface.tar.gz
```

To setup PETSC perform the following steps:

  1. Download the latest PETSC distribution tar file form the Petsc download site (for example: "petsc-lite-3.1-p2.tar.gz" )
  1. Extract the tar file using:  "tar -zxvf petsc-lite-3.1-p2.tar.gz"
  1. Download the blopex\_petsc\_abstract.tar.gz and blopex\_petsc\_interface.tar.gz from the Google code site
  1. Change to the petsc directory: "cd petsc-3.1-p2"
  1. Install the blopex\_petsc\_interface changes: "tar -zxvf $HOME/blopex\_petsc\_interface.tar.gz"
  1. The blopex\_petsc\_interface tar contains an edited version of blopex.py to allow for complex scalars and 64bit integers, so you don't have to edit the blopex.py file.
  1. Then configure PETSC using the "--download-blopex" option as described above with the blopex\_petsc\_abstract.tar.gz.  Follow this with a "make all test" and then make and execute the drivers.

**This setup script will work well for most Linux, GNU, Lapack systems.  Other systems can be more complicated.  Refer to the Wiki's for details.**

# 64bit testing #

If you are going to test with Petsc configured for 64bit integers, you should be aware of the following:

  1. You use the option `--with-64-bit-indices` to configure Petsc.  The type `PetscInt` will be defined with the interger type your compiler needs for 64bit integers (usually `long long`).
  1. Petsc configuration won't allow you to do this with blopex unless blopex.py (see above) has the following line added after "self.complex =1"; "self.requires32bitint = 0". This tells the configurator that blopex is coded to use 64bit integers (or 32bit).
  1. We want the Petsc abstract, interface, and driver code to be compiled with 64bit integers.  This is accomplished by having a special version of fortran\_options.h which includes "petsc.h" (where `PetscInt` is defined) and forces our preprocessor variable `BlopexInt` equal to `PetscInt`.  This is the same approach that we have for HYPRE.  The Petsc version of fortran\_options.h is in "blopex\_petsc/utilities/".
  1. In driver\_fiedler.c matrices are read in a Petsc Format.  These files can be created by matlab pgms (see m files in source directory blopex\_petsc).  When created integers are written as either 32bit or 64bit.  This must match with the definition of `PetscInt`.  Consequently, both 32bit and 64bit versions of our test files are needed.  The 64bit versions contain "_64" in the file name._

# An alternative testing method for developers #

If you have all of done this before you know it takes a very long time to do the configure and "make all test".  Remember this compiles the petsc-interface.c.  If you are making changes to the interface or to the abstract code, this can become very tedious.

<font color='blue'>
A different method of testing, described below, is primarily to help future developers.<br>
Perhaps you have noticed there is both a Makefile and a makefile in the blopex_petsc directories.  The lowercase makefile is used by PETSC.  The uppercase Makefile is used to compile and link the petsc-interface and drivers when the code is located externally to PETSC, such as in our source downloaded from Google.  This Makefile requires PETSC to be installed but not necessarily "--download-blopex" since we link with the blopex_abstract library that is compiled externally.  So, you can make changes and quickly test them without reinstallation of PETSC.<br>
<br>
Note that if you are in the external directory and specify "make" it will execute the lowercase makefile, which is not correct.  Instead use "make -f Makefile".  Also, note in the driver files (driver.c, driver_fiedler.c, etc.) there is an include for header file petsc-interface.c.  If executing a make from within PETSC this has to be hard coded to the subdirectory for petsc-interface but if external this subdirectory is different.  To handle this without changing the code I have incorporated some preprocessor code within the Makefile's and the driver--.c files.<br>
</font>

# What's BLOPEX\_DIR and makefile vs Makefile all about? #

First some background.  To use a package such as BLOPEX with Petsc it does not have to be distributed as part of Petsc. It can be compiled and linked
separately from Petsc. When BLOPEX is distributed as part of Petsc the abstract code and interface are compiled as part of the configure/make setup process. The drivers are compiled and linked after Petsc setup. Lets call this the internal process. But, rather than do this the abstract, interface, and drivers can be compiled after Petsc setup.  Of course your make files and some includes have to change to adjust for this.  Lets call this the external process.
Why do this?  Petsc setup takes a very long time and if you are doing major work on the abstract or interface you don't want to have to go thru the setup whenever you make a change.  In other words, it's for the convenience of the developer.  This process is described in the wiki CommentsOnPetscTesting.  To manage this process there are two make files, little m "makefile" which is for the Petsc internal process and big M "Makefile" which is for the external process.  One issue is where to get the petsc-interface.h file in the driver C files.  The internal process gets it from "../src/contrib/blopex/petsc-interface/petsc-interface.h" which is where it is located in the Petsc distribution.  It may be copied to the PETSc installation include directory, but using just #include "petsc-interface.h" doesn't work with the makefile we currently have for the drivers.  It can't find the include.  The external process uses Makefile which sets BLOPEX\_DIR to control picking up petsc-interface.h.  I'm not sure how all of this fits with PHAML.  Possibly, it has its own makefiles.

# Setup of Hypre Tar File #

A tar file (`hypre_logpcg_modifications.tar.gz`) is used for testing of changes in BLOPEX and submission of those changes for inclusion in a new Hypre release.  We describe here some unix scripts that have been created to aid in this process.  These files have been added to the Google source under directory blopex\_hypre/hypre\_setup.  These scripts are meant to help future developers in this process and may require changes for future versions of BLOPEX.

To use them the developer should create the directory hypre\_setup under his home directory and move the files there.  Hypre should be installed and the scripts must be changed to reflect the directory it is installed in.  It is assumed that the Google source has been downloaded to $HOME/work/trunk.

The script files are as follows:
```
abstract_update      This copies the latest BLOPEX files from $HOME/work/trunk to the Hypre installation
diffchk              This compares the BLOPEX files in the Hypre installation to those in $HOME/work/trunk
                     to confirm they are the same.  This is done incase files were changed in the Hypre 
                     installation but the Google source was not updated.
create_tar           This creates the tar file and gzips it.  The tar file is placed in the hypre_setup directory
```

# Setup of Petsc Tar Files #

Several tar files are used for testing changes in BLOPEX and submission of those changes for inclusion in a new Petsc release.  We describe here some unix scripts that have been created to aid in this process.  These files have been added to the Google source under directory blopex\_petsc/petsc\_setup.  These scripts are meant to help future developers in this process and may require changes for future versions of BLOPEX.

The tar files created are:
```
blopex_petsc_abstract.tar.gz     This contains the latest Blopex abstract files.  This tar would be specified 
                                 in Petsc configuration during testing and supplied to Petsc for a new release.
blopex_petsc_interface.tar.gz    This contains the latest Blopex Petsc interface files.  This files are included
                                 under Petsc src/contrib/blopex directory and are part of the Petsc installation.
                                 When testing this tar would be installed after the Petsc tar is installed but 
                                 before Petsc configuration. It would also be provided to Petsc for a new release.
blopex_petsc_test.tar.gz         This contains the test files for driver_fiedler.  Because of the size of some 
                                 of these files, they are provided in a separate tar.  
```

To use the scripts to create these tars the developer should create the directory petsc\_setup under his home directory and move the files there.  An installation of Petsc should be done or a shell containing directory $HOME/petsc-base/src/contrib/blopex should be created.  If petsc-base is not used then the scripts must be changed to reflect the directory it is installed in.  It is assumed that the Google source has been downloaded to $HOME/work/trunk.

The script files are as follows:
```
abstract_list          This contains a list of all BLOPEX abstract files.
cr_abstract_tar        This script creates blopex_petsc_abstract.tar.gz in directory petsc_setup.

interface_list         This contains a list of all BLOPEX Petsc interface files
test_list              This contains a list of all driver_fiedler test files
copyfiles              This script copies files in interface_list and test_list from Google source
                       directory $HOME/work/trunk to $HOME/petsc-base

cr_interface_tar_list  This script creates file interface_tar_list.  This is a list of all files
                       to include in the interface tar.  If Petsc files other than those from
                       BLOPEX have to be modified for testing they can be setup in petsc-base and 
                       then this script is modified to include them in interface_tar_list.
                       Run this script as follows "./cr_interface_tar_list > interface_tar_list"
cr_interface_tar       Once interface_tar_list is setup by cr_interface_tar_list then this script
                       is executed to create the tar in directory petsc_setup.

cr_test_tar_list       This script creates file test_tar_list from file test_list.  This is a list
                       of all files to include in the test tar.
                       Run this script as follows "./cr_test_tar_list > test_tar_list".
cr_test_tar            Once test_tar_list is setup by cr_test_tar_list then this script is 
                       executed to create the tar in directory petsc_setup.
```