
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <float.h>

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

// Algorithme 4 (A4) : racine carree + impairs
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

// Fonction pour mesurer le temps d'execution
double mesurerTemps(int (*fonction)(long long), long long N, int iterations)
{
    clock_t debut, fin;
    volatile int resultat;

    debut = clock();

    for (int k = 0; k < iterations; k++)
    {
        resultat = fonction(N);
    }

    fin = clock();

    return (double)(fin - debut) / CLOCKS_PER_SEC / iterations;
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

    double tempsA1, tempsA2, tempsA3, tempsA4;

    // Initialisation des minimums et maximums
    double minA1 = DBL_MAX, maxA1 = 0;
    double minA2 = DBL_MAX, maxA2 = 0;
    double minA3 = DBL_MAX, maxA3 = 0;
    double minA4 = DBL_MAX, maxA4 = 0;
    
    // Initialisation des sommes pour calculer les moyennes
    double sommeA1 = 0;
    double sommeA2 = 0;
    double sommeA3 = 0;
    double sommeA4 = 0;

    // Saisie du nom du fichier
    printf("Entrez le nom du fichier : ");
    scanf("%99s", nomfich);

    char *p = strstr(nomfich, ".txt");
    if (p != NULL)
        *p = '\0';

    // Construction du chemin du fichier d'entree
    snprintf(nomEntree, sizeof(nomEntree), "data/%s.txt", nomfich);

    fichierEntree = fopen(nomEntree, "r");

    if (fichierEntree == NULL)
    {
        printf("Erreur d'ouverture du fichier %s\n", nomEntree);
        return 1;
    }

    // Creation du fichier de resultats
    snprintf(nomSortie, sizeof(nomSortie),
             "results/%s_resultats.csv", nomfich);

    fichierSortie = fopen(nomSortie, "w");

    if (fichierSortie == NULL)
    {
        printf("Erreur de creation du fichier resultats.\n");
        fclose(fichierEntree);
        return 1;
    }

    // Ecriture des colonnes du CSV
    fprintf(fichierSortie,
            "Nombre, A1 (secondes), A2 (secondes), A3 (secondes), A4 (secondes)\n");

    // Lecture et traitement des nombres
    while (fscanf(fichierEntree, "%lld", &N) == 1)
    {
        printf("\nTraitement [%ld] : %lld\n", compteur + 1, N);

        // Execution de A1
        tempsA1 = mesurerTemps(estPremier1, N, 10);

        // Execution de A2
        tempsA2 = mesurerTemps(estPremier2, N, 10);

        // Execution de A3
        tempsA3 = mesurerTemps(estPremier3, N, 10000);

        // Execution de A4
        tempsA4 = mesurerTemps(estPremier4, N, 10000);

        // Calcul du minimum et maximum de A1
        if (tempsA1 < minA1) minA1 = tempsA1;
        if (tempsA1 > maxA1) maxA1 = tempsA1;

        // Calcul du minimum et maximum de A2
        if (tempsA2 < minA2) minA2 = tempsA2;
        if (tempsA2 > maxA2) maxA2 = tempsA2;

        // Calcul du minimum et maximum de A3
        if (tempsA3 < minA3) minA3 = tempsA3;
        if (tempsA3 > maxA3) maxA3 = tempsA3;

        // Calcul du minimum et maximum de A4
        if (tempsA4 < minA4) minA4 = tempsA4;
        if (tempsA4 > maxA4) maxA4 = tempsA4;

        // Calcul des sommes des temps
        sommeA1 += tempsA1;
        sommeA2 += tempsA2; 
        sommeA3 += tempsA3;
        sommeA4 += tempsA4;

        // Enregistrement des quatre temps dans une seule ligne
        fprintf(fichierSortie, "%lld, %.9f, %.9f, %.9f, %.9f\n",
                N, tempsA1, tempsA2, tempsA3, tempsA4);

        fflush(fichierSortie);

        compteur++;

        printf("A1 : %.9f s\n", tempsA1);
        printf("A2 : %.9f s\n", tempsA2);
        printf("A3 : %.9f s\n", tempsA3);
        printf("A4 : %.9f s\n", tempsA4);

        // Permet de lire les nombres separes par des virgules ou espaces
        fscanf(fichierEntree, " ,");
    }
        // Ecriture des lignes Min et Max dans le fichier CSV
       if (compteur > 0)
     {
      fprintf(fichierSortie,"min, %.9f, %.9f, %.9f, %.9f\n", minA1, minA2, minA3, minA4);
      fprintf(fichierSortie,"max, %.9f, %.9f, %.9f, %.9f\n", maxA1, maxA2, maxA3, maxA4);
      fprintf(fichierSortie,"moyenne, %.9f, %.9f, %.9f, %.9f\n", sommeA1 / compteur, sommeA2 / compteur, sommeA3 / compteur, sommeA4 / compteur);
     }
     

    fclose(fichierEntree);
    fclose(fichierSortie);

    printf("\n----------------------------------\n");
    printf("%ld nombres traites.\n", compteur);
    printf("Resultats enregistres dans : %s\n", nomSortie);

    return 0;
}
