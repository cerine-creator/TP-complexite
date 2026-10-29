#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

// Algorithme A3 : test de divisibilite jusqu'a sqrt(N)
// Propriete : Les diviseurs d'un nombre entier N sont pour moitie <= sqrt(N) et l'autre > sqrt(N)

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

int main(void)
{
    FILE *fichierEntree;
    FILE *fichierSortie;
    char nomfich[50];
    char nomSortie[100];
    long long N;
    long compteur = 0;
    clock_t debut, fin;
    double tempsExecution;

    printf("Entrez le nom du fichier : ");
    scanf("%49s", nomfich);

    char nomEntree[100];
    sprintf(nomEntree, "data/%s", nomfich);
    fichierEntree = fopen(nomEntree, "r");

    if (fichierEntree == NULL)
    {
        // Essayer dans ../data/ au cas ou execute depuis le sous-dossier
        sprintf(nomEntree, "../data/%s", nomfich);
        fichierEntree = fopen(nomEntree, "r");
        if (fichierEntree == NULL)
        {
            printf("Erreur d'ouverture du fichier dans data/ ou ../data/\n");
            return 1;
        }
    }

    sprintf(nomSortie, "results/%s_A3_resultat.csv", nomfich);
    fichierSortie = fopen(nomSortie, "w");

    if (fichierSortie == NULL)
    {
        sprintf(nomSortie, "../results/%s_A3_resultat.csv", nomfich);
        fichierSortie = fopen(nomSortie, "w");
        if (fichierSortie == NULL)
        {
            printf("Erreur de creation du fichier resultat.\n");
            fclose(fichierEntree);
            return 1;
        }
    }

    fprintf(fichierSortie, "Nombre,Temps (secondes)\n");

    while (fscanf(fichierEntree, "%lld", &N) == 1)
    {
        printf("Traite [%ld] : %lld en cours...", compteur + 1, N);
        fflush(stdout);

        int iterations = 10000;
        debut = clock();
        for (int k = 0; k < iterations; k++)
        {
            estPremier3(N);
        }
        fin = clock();

        tempsExecution = ((double)(fin - debut) / CLOCKS_PER_SEC) / iterations;

        fprintf(fichierSortie, "%lld,%.10f\n", N, tempsExecution);
        fflush(fichierSortie);

        compteur++;
        printf(" Termine en %.10f s\n", tempsExecution);

        fscanf(fichierEntree, ",");
    }

    fclose(fichierEntree);
    fclose(fichierSortie);

    printf("%ld nombres traites.\n", compteur);
    printf("Resultats enregistres dans %s\n", nomSortie);

    return 0;
}
