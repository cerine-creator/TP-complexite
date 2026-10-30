#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// Algorithme 1 (A1) : approche naive O(N)
int estPremier1(long long N)
{
    if (N <= 1)
        return 0;

    long long i;
    for (i = 2; i < N; i++)
    {
        if (N % i == 0)
            return 0;
    }
    return 1;
}

// Algorithme 2 (A2) : approche naive amelioree O(N/2)
int estPremier2(long long N)
{
    if (N <= 1)
        return 0;

    long long i;
    for (i = 2; i <= N / 2; i++)
    {
        if (N % i == 0)
            return 0;
    }
    return 1;
}

// Algorithme 3 (A3) : racine carree O(sqrt(N))
int estPremier3(long long N)
{
    if (N <= 1)
        return 0;

    long long racine = (long long)floor(sqrt((double)N));
    long long i;
    for (i = 2; i <= racine; i++)
    {
        if (N % i == 0)
            return 0;
    }
    return 1;
}

// Algorithme 4 (A4) : racine carree + impairs (Odd Skip)
int estPremier4(long long N)
{
    if (N <= 1)
        return 0;
    if (N == 2)
        return 1;
    if (N % 2 == 0)
        return 0;

    long long racine = (long long)floor(sqrt((double)N));
    long long i;
    for (i = 3; i <= racine; i += 2)
    {
        if (N % i == 0)
            return 0;
    }
    return 1;
}

int main(void)
{
    FILE *fichierEntree;
    FILE *fichierSortie;
    char nomfich[100];
    char nomEntree[150];
    char nomSortie[150];
    long long N;
    long compteur = 0;
    clock_t debut, fin;
    double tempsExecution;
    int choix;

    printf("Entrez le nom du fichier : ");
    scanf("%49s", nomfich);

    char *p = strstr(nomfich, ".txt");
    if (p) *p = '\0';

    printf("Choisissez l'algorithme (1: A1, 2: A2, 3: A3, 4: A4) : ");
    scanf("%d", &choix);

    if (choix < 1 || choix > 4)
    {
        printf("Choix invalide.\n");
        return 1;
    }

    sprintf(nomEntree, "data/%s.txt", nomfich);
    fichierEntree = fopen(nomEntree, "r");

    if (fichierEntree == NULL)
    {
        printf("Erreur d'ouverture du fichier dans data/%s.txt\n", nomfich);
        return 1;
    }

    sprintf(nomSortie, "results/%s_A%d_resultat.csv", nomfich, choix);
    fichierSortie = fopen(nomSortie, "w");

    if (fichierSortie == NULL)
    {
        printf("Erreur de creation du fichier dans results/\n");
        fclose(fichierEntree);
        return 1;
    }

    fprintf(fichierSortie, "Nombre,Temps (microsecondes)\n");

    // Pour A3 et A4, 10000 iterations car O(sqrt(N)) est tres rapide
    // Pour A1 et A2, 1 iteration suffit
    int iterations = (choix >= 3) ? 10000 : 1;

    while (fscanf(fichierEntree, "%lld", &N) == 1)
    {
        printf("Traite [%ld] : %lld en cours...", compteur + 1, N);
        fflush(stdout);

        debut = clock();
        for (int k = 0; k < iterations; k++)
        {
            if (choix == 1) estPremier1(N);
            else if (choix == 2) estPremier2(N);
            else if (choix == 3) estPremier3(N);
            else if (choix == 4) estPremier4(N);
        }
        fin = clock();

        // Calcul du temps directement en microsecondes (1 s = 1 000 000 us)
        tempsExecution = (((double)(fin - debut) / CLOCKS_PER_SEC) / iterations) * 1000000.0;

        fprintf(fichierSortie, "%lld,%.3f\n", N, tempsExecution);
        fflush(fichierSortie); // Sauvegarde immediate sur disque

        compteur++;
        printf(" Termine en %.3f us\n", tempsExecution);

        fscanf(fichierEntree, ",");
    }

    fclose(fichierEntree);
    fclose(fichierSortie);

    printf("%ld nombres traites.\n", compteur);
    printf("Resultats enregistres dans %s\n", nomSortie);

    return 0;
}
