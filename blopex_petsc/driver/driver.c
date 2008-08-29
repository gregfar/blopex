/* This code was developed by Merico Argentati, Andrew Knyazev, Ilya Lashuk and Evgueni Ovtchinnikov */

/* Program usage:  mpiexec -n <procs> driver [-help] [all PETSc options] */ 

static char help[] = "Test driver for 'abstract lobpcg' in PETSC\n\
Usage: mpiexec -n <procs> driver [-help] [all PETSc options]\n\
Special options:\n\
-n_eigs <integer>      Number of eigenvalues to calculate\n\
-tol <real number>     absolute tolerance for residuals\n\
-full_out              Produce more output\n\
-freepart              Let PETSC fully determine partitioning\n\
-seed <integer>        seed for random number generator\n\
-itr <integer>         Maximal number of iterations\n\
Example:\n\
mpiexec -n 2 driver -n_eigs 3 -tol 1e-6 -itr 20\n";

/* 
  Include "petscksp.h" so that we can use KSP solvers.  Note that this file
  automatically includes:
     petsc.h       - base PETSc routines   petscvec.h - vectors
     petscsys.h    - system routines       petscmat.h - matrices
     petscis.h     - index sets            petscksp.h - Krylov subspace methods
     petscviewer.h - viewers               petscpc.h  - preconditioners
*/

#include "petscksp.h"
#include "petscda.h"
#include <assert.h>
#include "lobpcg.h"
#include "src/contrib/blopex/petsc-interface/petsc-interface.h"
#include "interpreter.h"
#include "multivector.h"

typedef struct
{
  KSP                      ksp;
  Mat                      A;
  mv_InterfaceInterpreter  ii;
} aux_data_struct;

PetscErrorCode FillMatrix(DA da,Mat jac)
{
  PetscErrorCode ierr;
  PetscInt       i,j,k,mx,my,mz,xm,ym,zm,xs,ys,zs,idx;
  PetscScalar    v[7];
  MatStencil     row,col[7];
  
  ierr = DAGetInfo(da,0,&mx,&my,&mz,0,0,0,0,0,0,0);CHKERRQ(ierr);
  ierr = DAGetCorners(da,&xs,&ys,&zs,&xm,&ym,&zm);CHKERRQ(ierr);
  
  for (k=zs; k<zs+zm; k++){
    for (j=ys; j<ys+ym; j++){
      for(i=xs; i<xs+xm; i++){
       row.i = i; row.j = j; row.k = k;
	
       v[0] = 6.0;col[0].i = row.i; col[0].j = row.j; col[0].k = row.k;
       idx=1;
	     if (k>0) { v[idx] = -1.0; col[idx].i = i; col[idx].j = j; col[idx].k = k-1; idx++; }
	     if (j>0) { v[idx] = -1.0; col[idx].i = i; col[idx].j = j-1; col[idx].k = k; idx++; }
	     if (i>0) { v[idx] = -1.0; col[idx].i = i-1; col[idx].j = j; col[idx].k = k; idx++; }
	     if (i<mx-1) { v[idx] = -1.0; col[idx].i = i+1; col[idx].j = j; col[idx].k = k; 
                                                                              idx++; }	  
	     if (j<my-1) { v[idx] = -1.0; col[idx].i = i; col[idx].j = j+1; col[idx].k = k; idx++; }
	     if (k<mz-1) { v[idx] = -1.0; col[idx].i = i; col[idx].j = j; col[idx].k = k+1; idx++; }
	     ierr = MatSetValuesStencil(jac,1,&row,idx,col,v,INSERT_VALUES);CHKERRQ(ierr);
        
      }
    }
  }
  ierr = MatAssemblyBegin(jac,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(jac,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  return 0;
}

void Precond_FnSingleVector(void * data, void * x, void * y)
{
      PetscErrorCode     ierr;
      
      ierr = KSPSolve(((aux_data_struct*)data)->ksp, (Vec)x, (Vec)y);
      assert(!ierr);
}

void Precond_FnMultiVector(void * data, void * x, void * y)
{
   ((aux_data_struct*)data)->ii.Eval(Precond_FnSingleVector, data, x, y);
}

void OperatorASingleVector (void * data, void * x, void * y)
{
   PetscErrorCode     ierr;
   
   ierr = MatMult(((aux_data_struct*)data)->A, (Vec)x, (Vec)y);
   assert(!ierr);
}

void OperatorAMultiVector(void * data, void * x, void * y)
{
   ((aux_data_struct*)data)->ii.Eval(OperatorASingleVector, data, x, y);
}


#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **args)
{
   Vec            u;
   Mat            A;

   PetscErrorCode ierr;
  
   mv_MultiVectorPtr          eigenvectors;
   double *                   eigs;
   double *                   eigs_hist;
   double *                   resid;
   double *                   resid_hist;
   int                        iterations;
   PetscMPIInt                rank;
   int                        n_eigs = 1;
   int                         seed = 1;
   int                         i,j;
   PetscLogDouble              t1,t2,elapsed_time;
   DA                          da;
   double                      tol=1e-08;
   PetscTruth                  freepart=PETSC_FALSE;
   PetscTruth                  full_output=PETSC_FALSE;
   PetscInt                    m,n,p;
   KSP                        ksp;
   lobpcg_Tolerance           lobpcg_tol;
   int                        maxIt = 100;
   mv_InterfaceInterpreter    ii;
   lobpcg_BLASLAPACKFunctions blap_fn;
   aux_data_struct            aux_data;
	
   PetscInitialize(&argc,&args,(char *)0,help);
   ierr = PetscOptionsGetInt(PETSC_NULL,"-n_eigs",&n_eigs,PETSC_NULL);CHKERRQ(ierr);
   ierr = PetscOptionsGetReal(PETSC_NULL,"-tol", &tol,PETSC_NULL); CHKERRQ(ierr);
   ierr = PetscOptionsHasName(PETSC_NULL,"-freepart",&freepart); CHKERRQ(ierr);
   ierr = PetscOptionsHasName(PETSC_NULL,"-full_out",&full_output); CHKERRQ(ierr);
   ierr = PetscOptionsGetInt(PETSC_NULL,"-seed",&seed,PETSC_NULL);CHKERRQ(ierr);
   ierr = PetscOptionsGetInt(PETSC_NULL,"-itr",&maxIt,PETSC_NULL);CHKERRQ(ierr);
   
   if (seed<1)
    seed=1;
    
  /* we actually run our code twice: first time we solve small problem just to make sure
    that all program code is actually loaded into memory; then we solve the problem
    we are interested in; this trick is done for accurate timing
  */
  PreLoadBegin(PETSC_TRUE,"grid and matrix assembly");

  /* "create" the grid and stencil data; on first run we form small problem */
  if (PreLoadIt==0)
  {
      /* small problem */
      ierr=DACreate3d(PETSC_COMM_WORLD,DA_NONPERIODIC,DA_STENCIL_STAR,10,10,10,
            1,PETSC_DECIDE,1,1,1,0,0,0,&da); CHKERRQ(ierr);
  }
  else
  {
     /* actual problem */
      if (freepart)     /* petsc determines partitioning */
      {
        ierr=DACreate3d(PETSC_COMM_WORLD,DA_NONPERIODIC,DA_STENCIL_STAR,-10,-10,-10,
            PETSC_DECIDE,PETSC_DECIDE,PETSC_DECIDE,1,1,0,0,0,&da); CHKERRQ(ierr);
      }
      else             /* (1,NP,1) partitioning */
      {
        ierr=DACreate3d(PETSC_COMM_WORLD,DA_NONPERIODIC,DA_STENCIL_STAR,-10,-10,-10,
            1,PETSC_DECIDE,1,1,1,0,0,0,&da); CHKERRQ(ierr);
      }

      /* now we print what partitioning is chosen */
      ierr=DAGetInfo(da,PETSC_NULL,PETSC_NULL,PETSC_NULL,PETSC_NULL,&m,
                      &n,&p,PETSC_NULL,PETSC_NULL,PETSC_NULL,PETSC_NULL); CHKERRQ(ierr);
      PetscPrintf(PETSC_COMM_WORLD,"Partitioning: %u %u %u\n",m,n,p);  
  }

  /* create matrix, whose nonzero structure and probably partitioning corresponds to
  grid and stencil structure */
  ierr=DAGetMatrix(da,MATMPIAIJ,&A); CHKERRQ(ierr);
    
  /* fill the matrix with values. I intend to build 7-pt Laplas */ 
  /* this procedure includes matrix assembly */
  ierr=FillMatrix(da,A); CHKERRQ(ierr);

  /*
  PetscViewerBinaryOpen(PETSC_COMM_WORLD,"matrix.dat",PETSC_FILE_CREATE,&viewer);
  MatView(A,PETSC_VIEWER_STDOUT_WORLD);
  PetscViewerDestroy(viewer);
  */
  
  /* 
     Create parallel vectors.
      - We form 1 vector from scratch and then duplicate as needed.
  */
  
  ierr = DACreateGlobalVector(da,&u); CHKERRQ(ierr);
  /* ierr = VecSetFromOptions(u);CHKERRQ(ierr); */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
                Create the linear solver and set various options
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* Here we START measuring time for preconditioner setup */
  PreLoadStage("preconditioner setup");
  ierr = PetscGetTime(&t1);CHKERRQ(ierr);
  
  /* 
     Create linear solver context
  */
  ierr = KSPCreate(PETSC_COMM_WORLD,&ksp);CHKERRQ(ierr);

  /* 
     Set operators. Here the matrix that defines the linear system
     also serves as the preconditioning matrix.
  */
  ierr = KSPSetOperators(ksp,A,A,DIFFERENT_NONZERO_PATTERN);CHKERRQ(ierr);

  /* 
    Set runtime options, e.g.,
        -ksp_type <type> -pc_type <type> -ksp_monitor -ksp_rtol <rtol>
    These options will override those specified above as long as
    KSPSetFromOptions() is called _after_ any other customization
    routines.
  */
   ierr = KSPSetFromOptions(ksp);CHKERRQ(ierr);

   /* probably this call actually builds the preconditioner */
   ierr = KSPSetUp(ksp);CHKERRQ(ierr);

/* Here we STOP measuring time for preconditioner setup */
   PreLoadStage("solution");

   ierr = PetscGetTime(&t2);CHKERRQ(ierr);
   elapsed_time=t2-t1;
   if (PreLoadIt==1) 
    PetscPrintf(PETSC_COMM_WORLD,"Preconditioner setup, seconds: %f\n",elapsed_time);

   /* request memory for eig-vals */
   ierr = PetscMalloc(sizeof(double)*n_eigs,&eigs); CHKERRQ(ierr);
   
   /* request memory for eig-vals history */
   ierr = PetscMalloc(sizeof(double)*n_eigs*(maxIt+1),&eigs_hist); CHKERRQ(ierr);
   
   /* request memory for resid. norms */
   ierr = PetscMalloc(sizeof(double)*n_eigs,&resid); CHKERRQ(ierr);
   
   /* request memory for resid. norms hist. */
   ierr = PetscMalloc(sizeof(double)*n_eigs*(maxIt+1),&resid_hist); CHKERRQ(ierr);
   
   LOBPCG_InitRandomContext();
   
   MPI_Comm_rank(PETSC_COMM_WORLD,&rank);
 	

   PETSCSetupInterpreter( &ii );
   eigenvectors = mv_MultiVectorCreateFromSampleVector(&ii, n_eigs,u);

   for (i=0; i<seed; i++) /* this cycle is to imitate changing random seed */
      mv_MultiVectorSetRandom (eigenvectors, 1234);
	

   lobpcg_tol.absolute = tol;
   lobpcg_tol.relative = 1e-50;

   blap_fn.dpotrf = PETSC_dpotrf_interface;
   blap_fn.dsygv = PETSC_dsygv_interface;
   
   aux_data.A = A;
   aux_data.ksp = ksp;
   aux_data.ii = ii;

/* Here we START measuring time for solution process */
   ierr = PetscGetTime(&t1);CHKERRQ(ierr);
  
   lobpcg_solve( eigenvectors,
                 &aux_data,
              OperatorAMultiVector,
              NULL,
              NULL,
              &aux_data,
              Precond_FnMultiVector,
              NULL,
              blap_fn,
              lobpcg_tol,
              PreLoadIt? maxIt:1,
              !rank && PreLoadIt,
              &iterations,

	      eigs,                    /* eigenvalues; "lambda_values" should point to array 
                                       containing <blocksize> doubles where <blocksize> is the
                                       width of multivector "blockVectorX" */

	      eigs_hist, 
                                      /* eigenvalues history; a pointer to the entries of the 
                                         <blocksize>-by-(<maxIterations>+1) matrix stored in 
                                         fortran-style. (i.e. column-wise) The matrix may be 
                                         a submatrix of a larger matrix, see next argument */

              n_eigs,                  /* global height of the matrix (stored in fotran-style) 
                                         specified by previous argument */
                                         
	      resid, 
                                      /* residual norms; argument should point to
                                         array of <blocksize> doubles */
                                         
	      resid_hist ,
                                      /* residual norms history; a pointer to the entries of the 
                                         <blocksize>-by-(<maxIterations>+1) matrix stored in 
                                         fortran-style. (i.e. column-wise) The matrix may be 
                                         a submatrix of a larger matrix, see next argument */
              n_eigs
                                      /* global height of the matrix (stored in fotran-style) 
                                         specified by previous argument */ 
   );

/* Here we STOP measuring time for solution process */
  ierr = PetscGetTime(&t2);CHKERRQ(ierr);
  elapsed_time=t2-t1;
  if (PreLoadIt)
   PetscPrintf(PETSC_COMM_WORLD,"Solution process, seconds: %e\n",elapsed_time);
  
  if (PreLoadIt && full_output)
  {
      PetscPrintf(PETSC_COMM_WORLD,"Output from LOBPCG, eigenvalues:\n");
      for (i=0;i<n_eigs;i++)
	{
		ierr = PetscPrintf(PETSC_COMM_WORLD,"%e\n",eigs[i]);
		CHKERRQ(ierr);
	}
      
      PetscPrintf(PETSC_COMM_WORLD,"Output from LOBPCG, eigenvalues history:\n");
      for (j=0; j<iterations+1; j++)
         for (i=0;i<n_eigs;i++)
         {
            ierr = PetscPrintf(PETSC_COMM_WORLD,"%e\n",*(eigs_hist+j*n_eigs+i));
            CHKERRQ(ierr);
	 }
      PetscPrintf(PETSC_COMM_WORLD,"Output from LOBPCG, residual norms:\n");
      for (i=0;i<n_eigs;i++)
	{
		ierr = PetscPrintf(PETSC_COMM_WORLD,"%e\n",resid[i]);
		CHKERRQ(ierr);
	}

      PetscPrintf(PETSC_COMM_WORLD,"Output from LOBPCG, residual norms history:\n");
      for (j=0; j<iterations+1; j++)
         for (i=0;i<n_eigs;i++)
         {
            ierr = PetscPrintf(PETSC_COMM_WORLD,"%e\n",*(resid_hist+j*n_eigs+i));
            CHKERRQ(ierr);
	 }
   }	
  /*
     Free work space.  All PETSc objects should be destroyed when they
     are no longer needed.
  */
   ierr = VecDestroy(u);CHKERRQ(ierr); 
   ierr = MatDestroy(A);CHKERRQ(ierr);
   ierr = KSPDestroy(ksp);CHKERRQ(ierr);
   ierr = DADestroy(da); CHKERRQ(ierr);
	
   LOBPCG_DestroyRandomContext();
   mv_MultiVectorDestroy(eigenvectors);

   /* free memory used for eig-vals */
   ierr = PetscFree(eigs); CHKERRQ(ierr);
   ierr = PetscFree(eigs_hist); CHKERRQ(ierr);
   ierr = PetscFree(resid); CHKERRQ(ierr);
   ierr = PetscFree(resid_hist); CHKERRQ(ierr);
	
  /*
     Always call PetscFinalize() before exiting a program.  This routine
       - finalizes the PETSc libraries as well as MPI
       - provides summary and diagnostic information if certain runtime
         options are chosen (e.g., -log_summary). 
  */

  PreLoadEnd();
  ierr = PetscFinalize();CHKERRQ(ierr);
  return 0;
}
