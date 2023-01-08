#include <stdio.h>
#include <math.h>
#include <string.h>

#include <lapacke.h>

#include "settings.h"

/* indexing of N-by-N matrix, column major, 1 <= i,j <= N */
#define IDX_CM(N,i,j) ((i-1)+N*(j-1))
#define MAX_VECTORS_TO_FILE 128

void* msg_malloc(size_t size, const char* message) {

    char* p = malloc(size);
    if (p == NULL && message != NULL) {
        fprintf(stderr, " # ERROR: malloc failed for %s", message);
        exit(EXIT_FAILURE);
    }
    return p;
}

double V(double x) {
    const double a1=1.0e-4, a2=-3.0e-5, a4=1.0e-9;
    return a1*x+a2*pow(x,2)+a4*pow(x,4);
}

int main(int argc, char** argv) {

    /* Configurations for this DVR, can be changed using comman-line arguments */
    Config config;
    config.highPrecision = true;
    config.onlyValues = false;
    config.N = 256;
    config.a = 1.0;
    config.omega = 0.05;
    config.x0 = -config.N/2;
    config.HO = true;

    /* Process the command-line arguments */
    for (int i = 1; i != argc; ++i) {
        setSetting(&config, argv[i]);
    }

    const int N = config.N;
    const double omega = config.omega;
    const double a = config.a;
    const int x0 = config.x0;
    
    double* H = msg_malloc(N*N*sizeof(double), "memory for Hamiltonian");
    int m; /* Number of eigenvalues found */
    double* w = (double*) msg_malloc(N*sizeof(double), "memory for eigenvalues");
    double* z = (double*) msg_malloc(N*N*sizeof(double), "memory for eigenvectors");
    int* isuppz = (int*) msg_malloc(N*N*sizeof(int), "memory for eigenvalues indexing"); // IDK

    /* Kinetic energy matrix */
    if (config.highPrecision) {
        for (int i = 1; i <= N; ++i) {
            for (int j = 1; j <= N; ++j) {
                if (i == j) {
                    H[IDX_CM(N,i,j)] = M_PI*M_PI*(1+2.0/(N*N))/6/a/a;
                }
                else {
                    H[IDX_CM(N,i,j)] = M_PI*M_PI*((i+j)%2==0 ? 1 : -1)/(sin(M_PI*(i-j)/N)*sin(M_PI*(i-j)/N))/(N*N)/a/a;
                }
            }
        }
    }
    else {
        for (int i = 1; i <= N; ++i) {
            for (int j = 1; j <= N; ++j) {
                if (i == j) {
                    H[IDX_CM(N,i,j)] = 1.0/a/a;
                }
                else if (i-j==1 || j-i==1) {
                    H[IDX_CM(N,i,j)] = -0.5/a/a;
                }
                else {
                    H[IDX_CM(N,i,j)] = 0.0;
                }
            }
        }
    }

    /* Add the potential matrix */
    if (config.HO) {
        for (int i = 1; i <= N; ++i) {
            H[IDX_CM(N,i,i)] += omega*omega*a*a*(i+x0)*(i+x0)/2;
        }
    }
    else {
        for (int i = 1; i <= N; ++i) {
            H[IDX_CM(N,i,i)] += V(a*(i+x0));
        }
    }

    const char val_vec = config.onlyValues ? 'N' : 'V';

    /* The actual solving LoL */
    LAPACKE_dsyevr(LAPACK_COL_MAJOR, val_vec, 'A', 'U', N, H, N, 0.0, 0.0, 1, 20, 0.0, &m, w, z, N, isuppz);

    /* Files for the eigenvalues and eigenvectors */
    FILE* f_eval = fopen("evalues.txt", "w");
    FILE* f_evec = fopen("evectors.txt", "w");

    if (f_eval == NULL || f_evec == NULL) {
        fprintf(stderr, "Failed to open output file\n");
        exit(EXIT_FAILURE);
    }

    if (config.HO) {
        for (int i = 0; i != N; ++i) {
            /* First column: E/hw, second: deltaE/hw */
            fprintf(f_eval, "%.26le\t%.26le\n", w[i]/omega, fabs(w[i]/omega-(i+0.5)));
        }
    }
    else {
        for (int i = 0; i != N; ++i) {
            /* Only one column: eigenvalues */
            fprintf(f_eval, "%.26le\n", w[i]);
        }
    }

    /* Writing out only MAX_VECTORS_TO_FILE first eigenvectors */
    const int N_VEC = N < MAX_VECTORS_TO_FILE ? N : MAX_VECTORS_TO_FILE;
    for (int j = 1; j <= N; ++j) {
        for (int i = 1; i <= N_VEC; ++i) {
            fprintf(f_evec, "%.20le ", z[IDX_CM(N,j,i)]);
        }
        fputc('\n', f_evec);
    }

    fclose(f_eval);
    fclose(f_evec);

    free(H);
    free(w);
    free(z);
    free(isuppz);

    return 0;
}
