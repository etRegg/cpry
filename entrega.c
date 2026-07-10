
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#include <stdio.h>
#include <float.h>

#include<stdlib.h>   /* malloc() */

#include <time.h>

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}


/* Init square matrix with a specific value */
void initvalmat(double *mat, int n, double val, int transpose);

/* Multiply square matrices, blocked version */
void matmulblks(double *a, double *b, double *c, int n, int bs);

/* Multiply (block)submatrices */
void blkmul(double *ablk, double *bblk, double *cblk, int n, int bs);

/* Time in seconds from some point in the past */
double dwalltime();

double gflops(int N, double tiempo) {
    double n = (double) N;
    return (4.0 * n * n * n) / (tiempo * 1e9);
}


/************** MAIN *************/
int main(int argc, char *argv[])
{
  double *a, *b, *bt, *aux1, *aux2;
  int n, bs, i, j;
  double maxa = DBL_MIN;
  double maxb = DBL_MIN;
  double mina = DBL_MAX;
  double minb = DBL_MAX;
  double promedioa=0.0;
  double promediob=0.0;
  double total=0.0;
  double timetick;
  double suma=0.0;
  double sumb=0.0;
  /* Check command line parameters */
  if ( (argc != 3) || ((n = atoi(argv[1])) <= 0) || ((bs = atoi(argv[2])) <= 0) || ((n % bs) != 0))
  {
	printf("\nError en los parámetros. Usage: ./%s N BS (N debe ser multiplo de BS)\n", argv[0]);
	exit(1);
  }

  /* Getting memory */
  a = (double *) malloc(n*n*sizeof(double));
  b = (double *) malloc(n*n*sizeof(double));
  aux1 = (double *) malloc(n*n*sizeof(double));
 
  bt = (double *) malloc(n*n*sizeof(double));
  aux2 = (double *) malloc(n*n*sizeof(double));

  /* Init matrix operands */
  double val=1.0;
  //initvalmat(a, n, 1.0, 0);
	for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  a[i*n + j] = val;
		}
	  }






	for (i = 0; i < n; i++)
	  {
		  for (j = 0; j < n; j++)
		  {
			  b[j*n + i] = val;
		   }
	   }



	for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
			bt[i*n + j]=b[j*n + i];
		}
	  }

  //initvalmat(aux1, n, 0.0, 0);
	  for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  aux1[i*n + j] = 0;
		}
	  }
  //initvalmat(aux2, n, 0.0, 0);
	  for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  aux2[i*n + j] = 0;
		}
	  }

  timetick = dwalltime();
  //start = get_time();
 for (i = 0; i < (n*n); i++) {
    double va = a[i];
    double vb = b[i];

    suma += va;
    sumb += vb;

    if (va > maxa) maxa = va;
    else if (va < mina) mina = va;

    if (vb > maxb) maxb = vb;
    else if (vb < minb) minb = vb;
}
	  promedioa=suma/(n*n);
	  promediob=sumb/(n*n);
	  total=((maxa*maxb)-(mina*minb))/(promedioa*promediob);
  matmulblks(a, b, aux1, n, bs);
  matmulblks(aux1, bt, aux2, n, bs);
  for (i = 0; i < (n*n); i++)
  {
	  aux2[i]=total*aux2[i];
  }
//  end = get_time();
  double workTime = dwalltime() - timetick;
 // double tiempo = end - start;
 /* Check results (just in case...) */
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      if (aux2[i*n + j] != total*n*n)
      {
        printf("Error at %d, %d, value: %f\n", i, j, aux2[i*n + j]);
      }
    }
  }


//double tiempo;

//tiempo = end - start;

//printf("MMBLK-SEC;%d;%d;%lf;%lf\n",
//       n, bs, tiempo, gflops(n, tiempo));
 // printf("MMBLK-SEC;%d;%d;%lf;%lf\n",n,bs,time,gflops(n,time));
//((double)2.0*n*n*n)/(workTime*1000000000));
printf("MMBLK-SEC;%d;%d;%lf;%lf\n",n,bs,workTime,((double)4*n*n*n)/(workTime*1000000000));
  return 0;
}

/*****************************************************************/

/* Init square matrix with a specific value */
void initvalmat(double *mat, int n, double val, int transpose)
{
  int i, j;      /* Indexes */

	if (transpose == 0) {
	  for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  mat[i*n + j] = val;
		}
	  }
	} else {
	  for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  mat[j*n + i] = val;
		}
	  }
	}
}


/*****************************************************************/

/*****************************************************************/

/* Multiply square matrices, blocked version */
void matmulblks(double *a, double *b, double *c, int n, int bs)
{
  int i, j, k,in,jn, ink,jnk, inj;    /* Guess what... */

  /* Init matrix c, just in case */


  for (i = 0; i < n; i += bs)
  {

	in=i*n;
	 for  (k = 0; k < n; k += bs)
	  {
		  ink = in +k;
	for (j = 0; j < n; j += bs)
	{
        jn=j*n;
		inj = in + j;
		jnk = jn +k;

		blkmul(&a[ink], &b[jnk], &c[inj], n, bs);
	  }
	}
  }
}

/*****************************************************************/

/* Multiply (block)submatrices */
void blkmul(double *ablk, double *bblk, double *cblk, int n, int bs)
{
  int i, j, k ,in,jn,inj,ink,jnk;
  double aux;
  for (i = 0; i < bs; i++)
  {
	in = i * n;
	for  (k = 0; k < bs; k++)
	  {
		  ink= in + k;
	for (j = 0; j < bs; j++)
	{
	  aux = 0;
	  inj=in+j;
	  jn = j * n;
      jnk = jn + k;
	  aux += ablk[ink] * bblk[jnk];
	  }
	  cblk[inj] += aux;
	}
  }
}

/*****************************************************************/

#include <stdio.h>
#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
