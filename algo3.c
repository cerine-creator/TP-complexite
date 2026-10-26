#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h> 
#include <stdbool.h>




int even(int x, int y)
{
    if (x % y == 0)
    {
        return true;
    }
    return false;
}

int isPrimeSqart(int n)
{
    if (n <= 1)
    {
        return false;
    }
    int i;
    double x = floor(sqrt(n));
    for (i = 2 ; i < x; i++)
    {
        if (even(n, i))
        {
            return false;
        }
        return true;
    }
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

    sprintf(nomEntree, "../data/%s", nomfich);
    fichierEntree = fopen(nomEntree, "r");

    if (fichierEntree == NULL)
    {
        printf("Erreur d'ouverture.\n");
        return 1;
    }

    sprintf(nomSortie, "../results/%s_A3_resultat.csv", nomfich);
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
        int iterations = 10000;
        debut = clock();
       int k ;

        for (k = 0; k < iterations; k++)
        {
            isPrimeSqart(N);
        }
        fin = clock();

        tempsExecution = ((double)(fin - debut) / CLOCKS_PER_SEC) / iterations;

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
