
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

// Algorithme A4 - Odd Skip : teste uniquement les diviseurs impairs jusqu'a sqrt(N)

int estPremier4(long long N)
{
    if (N <= 1)
        return 0;   // 0 or 1 are NOT prime
    if (N == 2)
        return 1;   // 2 is prime
    if (N % 2 == 0)
        return 0;   // all other even numbers are NOT prime

    long long x = (long long)floor(sqrt((double)N));
    long long i;
    for (i = 3; i <= x; i += 2)   // only odd divisors, up to sqrt(N)
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
        printf("Erreur d'ouverture.\n");
        return 1;
    }

    sprintf(nomSortie, "results/%s_A4_resultat.csv", nomfich);
    fichierSortie = fopen(nomSortie, "w");

    if (fichierSortie == NULL)
    {
        printf("Erreur de creation du fichier.\n");
        fclose(fichierEntree);
        return 1;
    }

    fprintf(fichierSortie, "Nombre,Temps (secondes)\n");

    while (fscanf(fichierEntree, "%lld", &N) == 1)
    {
        debut = clock();
        estPremier4(N);
        fin = clock();

        tempsExecution = (double)(fin - debut) / CLOCKS_PER_SEC;

        fprintf(fichierSortie, "%lld,%.10f\n", N, tempsExecution);
        compteur++;

        fscanf(fichierEntree, ","); // try to skip comma (if CSV input)
    }

    fclose(fichierEntree);
    fclose(fichierSortie);

    printf("%ld nombres traites.\n", compteur);
    printf("Resultats enregistres dans %s\n", nomSortie);

    return 0;
}
