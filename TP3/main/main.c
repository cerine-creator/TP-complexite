#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DOSSIER1 "../data/exo1/"
#define DOSSIER2 "../data/exo2/"


// Crée une matrice rows x cols
double **alloc_matrix(int rows, int cols) {
    double **M = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
        M[i] = malloc(cols * sizeof(double));
    return M;
}

// Libère une matrice
void free_matrix(double **M, int rows) {
    for (int i = 0; i < rows; i++)
        free(M[i]);
    free(M);
}

// Lit une matrice rows x cols depuis les fichiers txt (NULL si erreur)
double **read_matrix(const char *chemin, int rows, int cols) {
    FILE *f = fopen(chemin, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir %s\n", chemin);
        return NULL;
    }
    double **M = alloc_matrix(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (fscanf(f, "%lf", &M[i][j]) != 1) {
                printf("Erreur de lecture dans %s\n", chemin);
                free_matrix(M, rows);
                fclose(f);
                return NULL;
            }
    fclose(f);
    return M;
}



//Ex1 : produit de matrices 

// C = A x B   (A: n x m, B: m x p, C: n x p)
void multiply(double **A, double **B, double **C, int n, int m, int p) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++) {
            double sum = 0;
            for (int k = 0; k < m; k++)
                sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
}

void exercice1() {
    int tailles[] = {100, 200, 400, 600, 800, 1000, 2000};
    int nb = 7;

    FILE *out = fopen("../data/resultats_exo1.csv", "w");
    if (out == NULL) { printf("Impossible de creer le CSV\n"); return; }
    fprintf(out, "n;T(s);T/n^3\n");

    for (int idx = 0; idx < nb; idx++) {
        int n = tailles[idx];
        char cA[200], cB[200];
        sprintf(cA, DOSSIER1 "A_%d.txt", n);
        sprintf(cB, DOSSIER1 "B_%d.txt", n);

        double **A = read_matrix(cA, n, n);
        double **B = read_matrix(cB, n, n);
        double **C = alloc_matrix(n, n);
        if (A == NULL || B == NULL) { printf("n = %d ignore\n", n); continue; }

        clock_t debut = clock();                 // on mesure seulement le calcul
        multiply(A, B, C, n, n, n);
        clock_t fin = clock();

        double T = (double)(fin - debut) / CLOCKS_PER_SEC;
        double n3 = (double)n * n * n;
        printf("n = %d   T = %.4f s   T/n^3 = %.3e\n", n, T, T / n3);
        fprintf(out, "%d;%.4f;%.3e\n", n, T, T / n3);

        free_matrix(A, n); free_matrix(B, n); free_matrix(C, n);
    }
    fclose(out);
    printf("Resultats dans ../data/resultats_exo1.csv\n");
}

// Exercice 2: recherche d'une sous-matrice 

// Vrai si B correspond a A en partant de la case i et j
int correspond(double **A, double **B, int i, int j, int n2, int m2) {
    for (int x = 0; x < n2; x++)
        for (int y = 0; y < m2; y++)
            if (A[i + x][j + y] != B[x][y])
                return 0;                        // on s'arrete a la 1ere difference
    return 1;
}

// Version 1 : naive, on essaie toutes les positions
int sousMat1(double **A, int n, int m, double **B, int n2, int m2) {
    for (int i = 0; i <= n - n2; i++)
        for (int j = 0; j <= m - m2; j++)
            if (correspond(A, B, i, j, n2, m2))
                return 1;
    return 0;
}

// Version 2 : lignes triees recherche dichotomique dans chaque ligne
int sousMat2(double **A, int n, int m, double **B, int n2, int m2) {
    double cle = B[0][0];
    for (int i = 0; i <= n - n2; i++) {
        // dichotomie : premier indice g tel que 
        int g = 0, d = m;
        while (g < d) {
            int milieu = (g + d) / 2;
            if (A[i][milieu] < cle) g = milieu + 1;
            else d = milieu;
        }
        // on teste les positions ou A[i][j] (il peut y avoir des doublons)
        for (int j = g; j < m && j <= m - m2 && A[i][j] == cle; j++)
            if (correspond(A, B, i, j, n2, m2))
                return 1;
    }
    return 0;
}

void exercice2() {
    int tailles[] = {100, 200, 400, 600, 800, 1000, 2000};
    int nb = 7;
    int petits[] = {10, 20, 30, 40, 50};      
    int grands[] = {100, 200, 300, 400, 500};   


    FILE *out = fopen("../data/resultats_exo2.csv", "w");
    if (out == NULL) { printf("Impossible de creer le CSV\n"); return; }
    fprintf(out, "n;k;sousMat1(s);sousMat2(s)\n");

    for (int idx = 0; idx < nb; idx++) {
        int n = tailles[idx];
        char cA[200];
        sprintf(cA, DOSSIER2 "A_%d_triee.txt", n);
        double **A = read_matrix(cA, n, n);
        if (A == NULL) continue;

        int *ks = (n <= 1000) ? petits : grands;

        for (int q = 0; q < 5; q++) {
            int k = ks[q];
            char cB[200];
            sprintf(cB, DOSSIER2 "B_pour_A%d_%d_triee.txt", n, k);
            double **B = read_matrix(cB, k, k);
            if (B == NULL) continue;

            // sousMat1 : une seule execution
            clock_t d1 = clock();
            sousMat1(A, n, n, B, k, k);
            clock_t f1 = clock();
            double T1 = (double)(f1 - d1) / CLOCKS_PER_SEC;

            // sousMat2 : tres rapide -> on repete 1000 fois et on fait la moyenne
            clock_t d2 = clock();
            for (int r = 0; r < 1000; r++)
                sousMat2(A, n, n, B, k, k);
            clock_t f2 = clock();
            double T2 = (double)(f2 - d2) / CLOCKS_PER_SEC / 1000;

            printf("n = %d  k = %d   sousMat1 = %.6f s   sousMat2 = %.6f s\n",
                   n, k, T1, T2);
            fprintf(out, "%d;%d;%.6f;%.6f\n", n, k, T1, T2);
            free_matrix(B, k);
        }
        free_matrix(A, n);
    }
    fclose(out);
    printf("Resultats dans ../data/resultats_exo2.csv\n");
}

/* ---------- Programme principal ---------- */

int main() {
    srand(time(NULL));
    int choix;
    printf("1 = Exercice 1 (produit de matrices)\n");
    printf("2 = Exercice 2 (sous-matrice)\n");
    printf("Votre choix : ");
    scanf("%d", &choix);

    if (choix == 1) exercice1();
    else if (choix == 2) exercice2();
    else printf("Choix invalide\n");
    return 0;
}