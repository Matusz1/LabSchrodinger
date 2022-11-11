#include <stdio.h>
#include <math.h>
#include <string.h>

/* Only linux */
#include <getopt.h>

#include <lapacke.h>

#include "settings.h"
#include "util.h"

/* Prints description of supported options */
void usage (FILE* fp, const char* path);

/* indexing of N-by-N matrix, column major, 1 <= i,j <= N */
#define IDX_CM(N,i,j) ((i-1)+N*(j-1))

int main(int argc, char** argv) {

    /* Configurations for this Schrodinger, can be changed using comman-line arguments */
    Config config;
    config.useDVR = true;
    config.N = 256;
    config.a = 1.0;
    config.p = 0.05;

    /* ================================================ */
    /* === This chunk is for command-line arguments === */

    char varArg[256];

    struct option longopts[] = {
        { "help", no_argument, NULL, 'h'},
        { "var", required_argument, NULL, 'v'},
        { 0 }
    };

    while (1) {
        int opt = getopt_long(argc, argv, "hv:", longopts, 0);
        if (opt == -1)
            break;

        switch (opt) {
            case 'h':
                usage(stdout, argv[0]);
                exit(EXIT_SUCCESS);
                break;
            case 'v':
                strncpy(varArg, optarg, sizeof(varArg));
                varArg[sizeof(varArg)-1] = '\0';
                setSetting(&config, varArg);
                break;
        }
    }

    /* === End of command-line arguments handling === */
    /* ============================================== */

    const int N = config.N;
    const double P = config.p;
    
    double* H = msg_malloc(N*N*sizeof(double), "memory for Hamiltonian"); /* Hamiltonian matrix */
    int m; /* Number of eigenvalues found */
    double* w = (double*) msg_malloc(N*sizeof(double), "memory for eigenvalues"); /* Eigenvalues */
    double* z = (double*) msg_malloc(N*N*sizeof(double), "memory for eigenvectors"); /* Eigenvectors */
    int* isuppz = (int*) msg_malloc(N*N*sizeof(int), "memory for eigenvalues indexing"); /* idk. some indexing */

    /* Filling the Hamiltonian */
    if (config.useDVR == true) {
        for (int i = 1; i <= N; ++i) {
            for (int j = 1; j <= N; ++j) {
                if (i == j) {
                    H[IDX_CM(N,i,j)] = M_PI*M_PI*(1+2/(N*N))/3 + P*P*config.a*config.a*(i-N/2)*(i-N/2);
                }
                else {
                    H[IDX_CM(N,i,j)] = 2*M_PI*M_PI*((i+j)%2==0 ? 1 : -1)/(sin(M_PI*(i-j)/N)*sin(M_PI*(i-j)/N))/(N*N);
                }
            }
        }
    }
    else {
        for (int i = 1; i <= N; ++i) {
            for (int j = 1; j <= N; ++j) {
                if (i == j) {
                    H[IDX_CM(N,i,j)] = 2.0 + P*P*config.a*config.a*(i-N/2)*(i-N/2);
                }
                else if (i-j==1 || j-i==1) {
                    H[IDX_CM(N,i,j)] = -1.0;
                }
                else {
                    H[IDX_CM(N,i,j)] = 0.0;
                }
            }
        }
    }

    /* The actual solving LoL */
    LAPACKE_dsyevr(LAPACK_COL_MAJOR, 'V', 'A', 'U', N, H, N, 0.0, 0.0, 1, 20, 0.0, &m, w, z, N, isuppz);


    /* Files for the eigenvalues and eigenvectors */
    FILE* f_eval = fopen("evalues.txt", "w");
    FILE* f_evec = fopen("evectors.txt", "w");

    if (f_eval == NULL || f_evec == NULL) {
        fprintf(stderr, "Failed to open output file\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i != N; ++i) {
        /* First column: E/hw, second: deltaE/hw*/
        fprintf(f_eval, "%.20le\t%.20le\n", w[i]/(2*P*config.a*config.a), fabs(w[i]/(2*P*config.a*config.a)-(i+0.5)));
    }

    /* Writing out only 50 first eigenvectors */
    for (int j = 1; j <= 50; ++j) {
        for (int i = 1; i <= N; ++i) {
            fprintf(f_evec, "%.6le ", z[IDX_CM(N,j,i)]);
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



/* Prints description of supported options */
void usage (FILE* fp, const char* path) {
    const char* basename = strrchr(path, '/');
    basename = basename ? basename + 1 : path;

    fprintf(fp, "Usage %s [OPTION]\n", basename);
    fprintf(fp, "  -h, --help\t\t"
        "Print this help and exit.\n");
    fprintf(fp, "  -v, --var=VARIABLE:VALUE\t"
        "Set VARIABLE to VALUE f.e. N:1024\n"
        "\tList of all variables:\n"
        "\t\tuseDVR:True/False\n"
        "\t\tN:int\n"
        "\t\ta:real\n"
        "\t\tp:real\n");
}
