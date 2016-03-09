# Introduction #

Hypre installation using PGI compilers with ACML.
This information is obsolete, since it refers to hypre and PGI versions from 2005.
Also, it uses LAM-MPI, which is no longer supported and merged with openmpi.
Still, it might be useful.

# Details #
<pre>
First set the correct environment for PGI, see<br>
<br>
/opt/pgi/set_environment (bash example):<br>
<br>
export LM_LICENSE_FILE=/opt/pgi/license.dat<br>
export PGI=/opt/pgi<br>
export PATH=/opt/pgi/linux86-64/5.2/bin:$PATH<br>
export MANPATH=$MANPATH:/opt/pgi/linux86-64/man<br>
<br>
Those below are not really needed as we pass them through the command line anyway<br>
<br>
export CC=pgcc<br>
export CFLAGS="-Kpic -Mcache_align -Mreentrant -tp=k8-64 -O3"<br>
export CXX=pgCC<br>
export F77=pgf77<br>
export F77FLAGS="-Kpic -Mcache_align -Mreentrant -tp=k8-64 -O3 -fast -Mvect=sse -Mscalarsse"<br>
<br>
Optimization flags are as in /opt/acml2.5.0/ReleaseNotes<br>
<br>
Flag -Mcache_align comes from<br>
<br>
http://www.pgroup.com/support/libacml.htm<br>
<br>
./configure --with-CC=pgcc --with-CFLAGS="-Kpic -Mcache_align -Mreentrant -tp=k8-64 -O3" --with-CXX=pgCC --with-F77=pgf77 --with-F77FLAGS="-Kpic -Mcache_align -Mreentrant -tp=k8-64 -O3 -fast -Mvect=sse -Mscalarsse" --with-lapack-libs="acml" --with-lapack-lib-dirs="/opt/acml2.5.0/pgi64/lib" --with-blas-libs="acml"  --with-blas-lib-dirs="/opt/acml2.5.0/pgi64/lib" --with-MPI-include=/usr/include --with-MPI-libs="mpi lam pthread" --with-MPI-lib-dirs=/usr/lib64/<br>
<br>
To run, use<br>
<br>
mpirun -x LD_LIBRARY_PATH="/opt/acml2.5.0/gnu64/lib;/opt/pgi/linux86-64/5.2/libso/" -np 2 ij -lobpcg<br>
</pre>