/**
 * Utility routines.
 *
 * Functions here of two types:
 * - functions that allocate/deallocate ubiquitous in scientific computing
 *   data structures (such as vectors, matrices, and tensors)
 * - functions that dump error messages
 *
 * Most functions here are borrowed from the book [1].
 *
 * References
 * ----------
 * [1] Teukolsky, S.A., Flannery, B.P., Press, W.H. and Vetterling, W.T.
 * Numerical recipes in C, 2nd edition.
 */
#include <stdarg.h>

#include "fd.h"
#include "globvar.h"


// The need for using NR_END is described on pages 940--941 of [1].
#define NR_END 1
// The need for casting to `char *` in `free` is due to the very old
// code (First edition of [1] was published before C1989 standard).
#define FREE_ARG char *

#ifdef __GNUC__
#define ATTR_UNUSED __attribute__((unused))
#else
#define ATTR_UNUSED
#endif

void err2(char errformat[],char errfilename[]){
	char outtxt[STRING_SIZE];
	sprintf(outtxt,errformat,errfilename);
	err(outtxt);
}

/**
 *  Print an error message to stderr and abort execution.
 *
 *  format :
 *      Format string conforming to the `printf` rules.
 *  ... :
 *      Variable arguments to be substituted into the `format` string.
 */
void err(char *format, ...) {
    extern int MYID;

    // The list of VAs behind '...' in the function signature.
    va_list arg_list;

    // Initialize arg_list to point to arguments after `format`.
    va_start(arg_list, format);

    // Flush stdout to avoid interleaving of stdout and stderr streams.
    fflush(stdout);

    fprintf(stderr, "Message from PE %d\n", MYID);
    fprintf(stderr, "R U N - T I M E  E R R O R:\n");
    // `fprintf` function prints variable arguments.
    vfprintf(stderr, format, arg_list);
    fprintf(stderr, "\n");
    fprintf(stderr, "...now exiting to system.\n");
    va_end(arg_list);

    MPI_Abort(MPI_COMM_WORLD, 1);
}

void warning(char warn_text[]){
	/* standard warnings handler */
	fprintf(stdout,"W A R N I N G   M E S S A G E: \n");
	fprintf(stdout,"%s\n",warn_text);
}


double maximum(float **a, int nx, int ny){
	/* find absolute maximum of array a[1...nx][1...ny] */
	double maxi=0.0;
	int i, j;


	for (j=1;j<=ny;j++)
		for (i=1;i<=nx;i++)
			if (fabs((double) a[i][j])>maxi) maxi=fabs((double)a[i][j]);
	return maxi;
}


float *vector(int nl, int nh) {
    /**
     * Allocate a vector of floats with subscript range nl..nh.
     * 
     * Vector elements are initialized to zero.
     * Note, that the caller is obliged to access vector elements as
     * v[nl], ..., v[nh] (note that the number of elements is `nh-nl+1`),
     * otherwise a memory access violation will occur.
     * nl : lowest index
     * nh : highest index
     */
    float *v;
    int i;

    v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
    if (!v) err("allocation failure in function vector()");
    for (i=0;i<(nh-nl+1+NR_END);i++) v[i]=0.0;
    return v-nl+NR_END;
}


int *ivector(int nl, int nh) {
    /**
     * Allocate a vector of ints with subscript range nl..nh.
     * 
     * Vector elements are initialized to zero.
     * Note, that the caller is obliged to access vector elements as
     * v[nl], ..., v[nh] (note that the number of elements is `nh-nl+1`),
     * otherwise a memory access violation will occur.
     * nl : lowest index
     * nh : highest index
     */
	int *v;
	int i;

	v=(int *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(int)));
	if (!v) err("allocation failure in function ivector()");
	for (i=0;i<(nh-nl+1+NR_END);i++) v[i]=0;
	return v-nl+NR_END;
}


unsigned short int *usvector(int nl, int nh) {
    /**
     * Allocate a vector of unsigned ints with subscript range nl..nh.
     * 
     * Vector elements are initialized to zero.
     * Note, that the caller is obliged to access vector elements as
     * v[nl], ..., v[nh] (note that the number of elements is `nh-nl+1`),
     * otherwise a memory access violation will occur.
     * nl : lowest index
     * nh : highest index
     */
	unsigned short int *v;
	int i;

	v=(unsigned short int *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(unsigned short int)));
	if (!v) err("allocation failure in function usvector()");
	for (i=0;i<(nh-nl+1+NR_END);i++) v[i]=0;
	return v-nl+NR_END;
}


unsigned char *cvector(int nl, int nh){
    /**
     * Allocate a vector of chars with subscript range nl..nh.
     * 
     * Vector elements are initialized to zero.
     * Note, that the caller is obliged to access vector elements as
     * v[nl], ..., v[nh] (note that the number of elements is `nh-nl+1`),
     * otherwise a memory access violation will occur.
     * nl : lowest index
     * nh : highest index
     */
	unsigned char *v;

	v=(unsigned char *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(unsigned char)));
	if (!v) err("allocation failure in function cvector()");
	return v-nl+NR_END;
}


unsigned long *lvector(int nl, int nh){
    /**
     * Allocate a vector of unsigned longs with subscript range nl..nh.
     * 
     * Vector elements are initialized to zero.
     * Note, that the caller is obliged to access vector elements as
     * v[nl], ..., v[nh] (note that the number of elements is `nh-nl+1`),
     * otherwise a memory access violation will occur.
     * nl : lowest index
     * nh : highest index
     */
	unsigned long *v;
	int i;

	v=(unsigned long *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(unsigned long)));
	if (!v) err("allocation failure in function lvector()");
	for (i=0;i<(nh-nl+1+NR_END);i++) v[i]=0;
	return v-nl+NR_END;
}

double *dvector(int nl, int nh){
    /**
     * Allocate a vector of unsigned longs with subscript range nl..nh.
     * 
     * Vector elements are initialized to zero.
     * Note, that the caller is obliged to access vector elements as
     * v[nl], ..., v[nh] (note that the number of elements is `nh-nl+1`),
     * otherwise a memory access violation will occur.
     * nl : lowest index
     * nh : highest index
     */
	double *v;
	int i;

	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	if (!v) err("allocation failure in function dvector()");
	for (i=0;i<(nh-nl+1+NR_END);i++) v[i]=0.0;
	return v-nl+NR_END;
}

float **fmatrix(int nrl, int nrh, int ncl, int nch){
	/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch]
		   and intializing the matrix, e.g. m[nrl..nrh][ncl..nch]=0.0 */
	int i,j, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	float **m;

	/* allocate pointers to rows */
	m=(float **) malloc((size_t) ((nrow+NR_END)*sizeof(float*)));
	if (!m) err("allocation failure 1 in function fmatrix() ");
	m += NR_END;
	m -= nrl;

	/* allocation rows and set pointers to them */
	m[nrl]=(float *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float)));
	if (!m[nrl]) err("allocation failure 2 in function fmatrix() ");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for (i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* initializing matrix */
	for (i=nrl;i<=nrh;i++)
		for (j=ncl;j<=nch;j++) m[i][j]=0.0;

	/* return pointer to array of pointer to rows */
	return m;
}


double **dmatrix(int nrl, int nrh, int ncl, int nch){
	/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch]
		   and intializing the matrix, e.g. m[nrl..nrh][ncl..nch]=0.0 */
	int i,j, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	double **m;

	/* allocate pointers to rows */
	m=(double **) malloc((size_t) ((nrow+NR_END)*sizeof(double*)));
	if (!m) err("allocation failure 1 in function matrix() ");
	m += NR_END;
	m -= nrl;

	/* allocation rows and set pointers to them */
	m[nrl]=(double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
	if (!m[nrl]) err("allocation failure 2 in function dmatrix() ");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for (i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* initializing matrix */
	for (i=nrl;i<=nrh;i++)
		for (j=ncl;j<=nch;j++) m[i][j]=0.0;

	/* return pointer to array of pointer to rows */
	return m;
}

int **imatrix(int nrl, int nrh, int ncl, int nch){
	/* allocate an int matrix with subscript range m[nrl..nrh][ncl..nch]
		   and intializing the matrix, e.g. m[nrl..nrh][ncl..nch]=0.0 */
	int i,j, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	int **m;

	/* allocate pointers to rows */
	m=(int **) malloc((size_t) ((nrow+NR_END)*sizeof(int*)));
	if (!m) err("allocation failure 1 in function imatrix() ");
	m += NR_END;
	m -= nrl;

	/* allocation rows and set pointers to them */
	m[nrl]=(int *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(int)));
	if (!m[nrl]) err("allocation failure 2 in function imatrix() ");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for (i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* initializing matrix */
	for (i=nrl;i<=nrh;i++)
		for (j=ncl;j<=nch;j++) m[i][j]=0;

	/* return pointer to array of pointer to rows */
	return m;
}

unsigned short int **usmatrix(int nrl, int nrh, int ncl, int nch){
	/* allocate an int matrix with subscript range m[nrl..nrh][ncl..nch]
		   and intializing the matrix, e.g. m[nrl..nrh][ncl..nch]=0.0 */
	int i,j, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	unsigned short int **m;

	/* allocate pointers to rows */
	m=(unsigned short int **) malloc((size_t) ((nrow+NR_END)*sizeof(unsigned short int*)));
	if (!m) err("allocation failure 1 in function usmatrix() ");
	m += NR_END;
	m -= nrl;

	/* allocation rows and set pointers to them */
	m[nrl]=(unsigned short int *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(unsigned short int)));
	if (!m[nrl]) err("allocation failure 2 in function usmatrix() ");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for (i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* initializing matrix */
	for (i=nrl;i<=nrh;i++)
		for (j=ncl;j<=nch;j++) m[i][j]=0;

	/* return pointer to array of pointer to rows */
	return m;
}

float ***f3tensor(int nrl, int nrh, int ncl, int nch,int ndl, int ndh){
	/* allocate a float 3tensor with subscript range m[nrl..nrh][ncl..nch][ndl..ndh]
		   and intializing the matrix, e.g. m[nrl..nrh][ncl..nch][ndl..ndh]=0.0 */
	int i,j,d, nrow=nrh-nrl+1,ncol=nch-ncl+1,ndep=ndh-ndl+1;
	float ***t;

	/* allocate pointers to pointers to rows */
	t=(float ***) malloc((size_t) ((nrow+NR_END)*sizeof(float**)));
	if (!t) err("allocation failure 1 in function f3tensor() ");
	t += NR_END;
	t -= nrl;

	/* allocate pointers to rows and set pointers to them */
	t[nrl]=(float **) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float*)));
	if (!t[nrl]) err("allocation failure 2 in function f3tensor() ");
	t[nrl] += NR_END;
	t[nrl] -= ncl;

	/* allocate rows and set pointers to them */
	t[nrl][ncl]=(float *) malloc((size_t)((nrow*ncol*ndep+NR_END)*sizeof(float)));
	if (!t[nrl][ncl]) err("allocation failure 3 in function f3tensor() ");
	t[nrl][ncl] += NR_END;
	t[nrl][ncl] -= ndl;

	for (j=ncl+1;j<=nch;j++) t[nrl][j]=t[nrl][j-1]+ndep;
	for (i=nrl+1;i<=nrh;i++){
		t[i]=t[i-1]+ncol;
		t[i][ncl]=t[i-1][ncl]+ncol*ndep;
		for (j=ncl+1;j<=nch;j++) t[i][j]=t[i][j-1]+ndep;
	}

	/* initializing 3tensor */
	for (i=nrl;i<=nrh;i++)
		for (j=ncl;j<=nch;j++)
			for (d=ndl;d<=ndh;d++) t[i][j][d]=0.0;

	/* return pointer to array of pointer to rows */
	return t;
}


int ***i3tensor(int nrl, int nrh, int ncl, int nch,int ndl, int ndh){
	/* allocate a integer 3tensor with subscript range m[nrl..nrh][ncl..nch][ndl..ndh]
		   and intializing the matrix, e.g. m[nrl..nrh][ncl..nch][ndl..ndh]=0.0 */
	int i,j,d, nrow=nrh-nrl+1,ncol=nch-ncl+1,ndep=ndh-ndl+1;
	int ***t;

	/* allocate pointers to pointers to rows */
	t=(int ***) malloc((size_t) ((nrow+NR_END)*sizeof(int**)));
	if (!t) err("allocation failure 1 in function i3tensor() ");
	t += NR_END;
	t -= nrl;

	/* allocate pointers to rows and set pointers to them */
	t[nrl]=(int **) malloc((size_t)((nrow*ncol+NR_END)*sizeof(int*)));
	if (!t[nrl]) err("allocation failure 2 in function i3tensor() ");
	t[nrl] += NR_END;
	t[nrl] -= ncl;

	/* allocate rows and set pointers to them */
	t[nrl][ncl]=(int *) malloc((size_t)((nrow*ncol*ndep+NR_END)*sizeof(int)));
	if (!t[nrl][ncl]) err("allocation failure 3 in function i3tensor() ");
	t[nrl][ncl] += NR_END;
	t[nrl][ncl] -= ndl;

	for (j=ncl+1;j<=nch;j++) t[nrl][j]=t[nrl][j-1]+ndep;
	for (i=nrl+1;i<=nrh;i++){
		t[i]=t[i-1]+ncol;
		t[i][ncl]=t[i-1][ncl]+ncol*ndep;
		for (j=ncl+1;j<=nch;j++) t[i][j]=t[i][j-1]+ndep;
	}

	/* initializing 3tensor */
	for (i=nrl;i<=nrh;i++)
		for (j=ncl;j<=nch;j++)
			for (d=ndl;d<=ndh;d++) t[i][j][d]=0;

	/* return pointer to array of pointer to rows */
	return t;
}


float ****f4tensor(int nrl, int nrh, int ncl, int nch,int ndl, int ndh, int nvl, int nvh){
	/* allocate a float 3tensor with subscript range m[nrl..nrh][ncl..nch][ndl..ndh][nvl..nvh]
		   and intializing the matrix, e.g. m[nrl..nrh][ncl..nch][ndl..ndh][nvl..nvh]=0.0 */
	int i,j,d,v, nrow=nrh-nrl+1,ncol=nch-ncl+1,ndep=ndh-ndl+1, nval=nvh-nvl+1;
	float ****t;

	/* allocate pointers to pointers to rows */
	t=(float ****) malloc((size_t) ((nrow+NR_END)*sizeof(float**)));
	if (!t) err("allocation failure 1 in function f4tensor() ");
	t += NR_END;
	t -= nrl;

	/* allocate pointers to rows and set pointers to them */
	t[nrl]=(float ***) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float*)));
	if (!t[nrl]) err("allocation failure 2 in function f4tensor() ");
	t[nrl] += NR_END;
	t[nrl] -= ncl;

	/* allocate rows and set pointers to them */
	t[nrl][ncl]=(float **) malloc((size_t)((nrow*ncol*ndep+NR_END)*sizeof(float)));
	if (!t[nrl][ncl]) err("allocation failure 3 in function f4tensor() ");
	t[nrl][ncl] += NR_END;
	t[nrl][ncl] -= ndl;

	/* allocate values and set pointers to them */
	t[nrl][ncl][ndl]=(float *) malloc((size_t)((nrow*ncol*ndep*nval+NR_END)*sizeof(float)));
	if (!t[nrl][ncl][ndl]) err("allocation failure 4 in function f4tensor() ");
	t[nrl][ncl][ndl] += NR_END;
	t[nrl][ncl][ndl] -= nvl;

	/*for (j=ncl+1;j<=nch;j++) t[nrl][j]=t[nrl][j-1]+ndep;*/
	for (d=ndl+1;d<=ndh;d++) t[nrl][ncl][d]=t[nrl][ncl][d-1]+nval;

	for (i=nrl+1;i<=nrh;i++){
		t[i]=t[i-1]+ncol;
		t[i][ncl]=t[i-1][ncl]+ncol*ndep;
		t[i][ncl][ndl]=t[i-1][ncl][ndl]+ncol*ndep*nval;
		for (j=ncl+1;j<=nch;j++){
			t[i][j]=t[i][j-1]+ndep;
			t[i][j][ndl]=t[i][j-1][ndl]+ndep*nval;
			for (d=ndl+1;d<=ndh;d++){
				t[i][j][d]=t[i][j][d-1]+nval;
			}
		}
	}

	/* initializing 4tensor */
	for (i=nrl;i<=nrh;i++)
		for (j=ncl;j<=nch;j++)
			for (d=ndl;d<=ndh;d++)
				for (v=nvl;d<=nvh;v++) t[i][j][d][v]=0.0;

	/* return pointer to array of pointer to rows */
	return t;
}

void free_vector(float *v, int nl, int nh ATTR_UNUSED){
	/* free a float vector allocated with vector() */
	free((FREE_ARG) (v+nl-NR_END));
}

void free_ivector(int *v, int nl, int nh ATTR_UNUSED){
	/* free a int vector allocated with vector() */
	free((FREE_ARG) (v+nl-NR_END));
}

void free_cvector(char *v, int nl, int nh ATTR_UNUSED){
	/* free a char vector allocated with vector() */
	free((FREE_ARG) (v+nl-NR_END));
}

void free_dvector(double *v, int nl, int nh ATTR_UNUSED){
	/* free a char vector allocated with vector() */
	free((FREE_ARG) (v+nl-NR_END));
}

void free_matrix(float **m, int nrl,
        int nrh ATTR_UNUSED,
        int ncl, int nch ATTR_UNUSED) {
	/* free a float matrix allocated by matrix() */
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}

void free_imatrix(int **m,
        int nrl, int nrh ATTR_UNUSED,
        int ncl, int nch ATTR_UNUSED) {
	/* free a integer matrix allocated by imatrix() */
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}

void free_usmatrix(unsigned short int **m,
        int nrl, int nrh ATTR_UNUSED,
        int ncl, int nch ATTR_UNUSED) {
	/* free a integer matrix allocated by imatrix() */
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}

void free_f3tensor(float ***t,
        int nrl, int nrh ATTR_UNUSED,
        int ncl, int nch ATTR_UNUSED,
        int ndl, int ndh ATTR_UNUSED) {
	/* free a float matrix allocated by f3tensor() */
	free((FREE_ARG) (t[nrl][ncl]+ndl-NR_END));
	free((FREE_ARG) (t[nrl]+ncl-NR_END));
	free((FREE_ARG) (t+nrl-NR_END));
}

void free_i3tensor(int ***t,
        int nrl, int nrh ATTR_UNUSED,
        int ncl, int nch ATTR_UNUSED,
        int ndl, int ndh ATTR_UNUSED) {
	/* free a float matrix allocated by i3tensor() */
	free((FREE_ARG) (t[nrl][ncl]+ndl-NR_END));
	free((FREE_ARG) (t[nrl]+ncl-NR_END));
	free((FREE_ARG) (t+nrl-NR_END));
}

void free_f4tensor(float ****t,
        int nrl, int nrh ATTR_UNUSED,
        int ncl, int nch ATTR_UNUSED,
        int ndl, int ndh ATTR_UNUSED,
        int nvl, int nvh ATTR_UNUSED) {
	/* free a float matrix allocated by f4tensor() */
	free((FREE_ARG) (t[nrl][ncl][ndl]+nvl-NR_END));
	free((FREE_ARG) (t[nrl][ncl]+ndl-NR_END));
	free((FREE_ARG) (t[nrl]+ncl-NR_END));
	free((FREE_ARG) (t+nrl-NR_END));
}

void reverse(char s[]){
	int c, i, j;
	for (i = 0, j = strlen(s) - 1; i<j; i++, j--){
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
