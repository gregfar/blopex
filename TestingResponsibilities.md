## Responsibilities for testing of BLOPEX ##

<table border='1'>
<tr>
<td> </td><td> Environment </td><td>Interface</td>
</tr>

<tr>
<td>Andrew</td><td>LLNL</td><td>Hypre</td>
</tr>

<tr>
<td>Don</td><td>Bluefire,Frost</td><td>Petsc,Abstract,Hypre</td>
</tr>

<tr>
<td>Peizhen</td><td>Cygwin,Linux</td><td>Petsc,Intel,MKL</td>
</tr>

<tr>
<td>Eugene</td><td>XVIA / XVIB</td><td>AMD ACML,Petsc,Hypre,Matlab,effect of Scaling on C=chol(A)</td>
</tr>

<tr>
<td>Bryan</td><td></td><td>Quality Control</td>
</tr>

</table>



Depending on system availability execute tests under the following:
  * Compilers
    1. gcc, Intel, xlc
    1. same with cpu optimized options where possible
  * Libraries
    1. Lapack/Blas
    1. ACML for AMD
    1. MKL for Intel