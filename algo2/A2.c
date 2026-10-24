    
#include <stdio.h>
#include <time.h>
#include <string.h>

// Algorithme A2

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
    scanf("%49s",nomfich);

    char nomEntree[100];
    sprintf(nomEntree, "data/%s", nomfich);
    fichierEntree = fopen(nomEntree, "r");

    if (fichierEntree == NULL)
    {
        printf("Erreur d'ouverture.\n");
        return 1;
    }

    sprintf(nomSortie, "results/%s_A2_resultat.csv", nomfich);
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
       estPremier2(N);
       fin = clock();

       tempsExecution = (double)(fin - debut) / CLOCKS_PER_SEC ;

       fprintf(fichierSortie,"%lld,%.10f\n", N, tempsExecution);
        compteur++;

       fscanf(fichierEntree,",");
    }

    fclose(fichierEntree);
    fclose(fichierSortie);

    printf("%ld nombres traites.\n", compteur);
    printf("Resultats enregistres dans %s\n", nomSortie);

    return 0;
}    
