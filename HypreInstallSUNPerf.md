# Introduction #

Hypre installation using SUN compilers and libraries. This information is obsolete, since it refers to hypre and SUN versions from 2005.


# Details #
<pre>
To build hypre-1.9 for 64 bit with sunperf:<br>
<br>
./configure --with-CC="mpcc -xarch=native64" --with-F77="mpf77 -xarch=native64" --with-CXX="mpCC -xarch=native64" --with-blas-libs="sunperf sunmath m" --with-lapack-libs="sunpef sunmath m"<br>
<br>
make<br>
cd test<br>
<br>
make -i<br>
<br>
<br>
(some executables won't be compiled because of the errors in the configurator. However, ij_es, struct_es, sstruct_es should compile.)<br>
<br>
you can then run things with<br>
<br>
mprun -np 6 ./ij -lobpcg<br>
<br>
<br>
to build hypre-1.9 for 64 bit with hypre's blas/lapack:<br>
<br>
./configure --with-CC="mpcc -xarch=native64" --with-F77="mpf77 -xarch=native64" --with-CXX="mpCC -xarch=native64"<br>
<br>
<br>
Typical mathsun interactive run:<br>
<br>
mprun -np 6 ./ij -lobpcg -n 100 100 100 -pcgitr 0 -vrand 27<br>
<br>
<br>
Typical mathsun non-interactive run:<br>
<br>
nohup mprun -np 6 ./ij -lobpcg -n 100 100 100 -pcgitr 0 -vrand 270 > 270.out < /dev/null &<br>
<br>
<br>
Notes:<br>
<br>
Building hypre on mathsun<br>
<br>
export HYPRE_INSTALL_DIR=../<br>
<br>
#http://www.csafe.utah.edu/Information/Thirdparty/hypre.html<br>
<br>
./configure \<br>
--with-mpi-include=/opt/SUNWhpc/include \<br>
--with-mpi-lib-dirs=/opt/SUNWhpc/lib \<br>
--with-mpi-libs='mpi pmpi' \<br>
--with-CC=mpcc --with-CXX=mpCC \<br>
--with-F77=mpif77<br>
<br>
make<br>
make test<br>
</pre>