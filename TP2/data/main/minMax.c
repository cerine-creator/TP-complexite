#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// naive approache mf

void MaxEtMinA(int T[], int n, int *max, int *min, long *nb_comparaisons)
{
    *nb_comparaisons = 0;
    if (n <= 0)
        return;

    *max = T[0];
    *min = T[0];
    int i;
    for (i = 1; i < n; i++)
    {
        // Première comparaison : tester avec le maximum
        (*nb_comparaisons)++;
        if (T[i] > *max)
        {
            *max = T[i];
        }
        else
        {
            // Deuxième comparaison : tester avec le minimum (si ce n'est pas le max)
            (*nb_comparaisons)++;
            if (T[i] < *min)
            {
                *min = T[i];
            }
        }
    }
}

// Principe par paires

void MaxEtMinBVersion2(int T[], int n, int *max, int *min, long *nb_comparaisons)
{
    *nb_comparaisons = 0;
    if (n <= 0)
        return;

    int i = 0;

    // Initialisation selon la parité de n
    if (n % 2 == 0)
    {
        (*nb_comparaisons)++;
        if (T[0] > T[1])
        {
            *max = T[0];
            *min = T[1];
        }
        else
        {
            *max = T[1];
            *min = T[0];
        }
        i = 2;
    }
    else
    {
        *max = T[0];
        *min = T[0];
        i = 1;
    }

    // Traitement par paires
    while (i < n - 1)
    {
        int local_max, local_min;

        // 1. Comparer les deux éléments de la paire entre eux
        (*nb_comparaisons)++;
        if (T[i] > T[i + 1])
        {
            local_max = T[i];
            local_min = T[i + 1];
        }
        else
        {
            local_max = T[i + 1];
            local_min = T[i];
        }

        // 2. Comparer le plus grand au maximum global
        (*nb_comparaisons)++;
        if (local_max > *max)
        {
            *max = local_max;
        }

        // 3. Comparer le plus petit au minimum global
        (*nb_comparaisons)++;
        if (local_min < *min)
        {
            *min = local_min;
        }

        i += 2;
    }
}

typedef void (*MinMaxFn)(int[], int, int*, int*, long*); 

double calculerTemps( MinMaxFn fonctionRecherche, int T[], int n, int iterations, long *comp_out)
{
    int max, min;
    clock_t start, end;

    start = clock();
    int k ;
    for ( k = 0; k < iterations; k++)
    {
        fonctionRecherche(T, n, &max, &min, comp_out);
    }
    end = clock();

    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main()
{
    FILE *fichier = fopen("resultats_complexiteMinMax.csv", "w");
    if (fichier == NULL)
    {
        printf("Erreur d'ouverture du fichier de resultats !\n");
        return 1;
    }
    fprintf(fichier, "n,NonTrie_Pire (s),Trie_Pire (s),MAX  ,   MIN\n");

    int tailles[] = {100000, 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000, 1600000, 1800000, 2000000, 4000000, 5000000, 6000000, 7000000, 8000000};   
    int nb_tailles = 16;
    int iterations = 1000;

    printf("n\t\t naive approache  (s)\t pires  (s)\n");
     int t;
    for (t = 0; t < nb_tailles; t++) {
        int n = tailles[t];
        
        // --- 1. CHARGEMENT DU FICHIER NON TRIÉ ---
        int *arrayUnsorted = (int *)malloc(n * sizeof(int));
        if (arrayUnsorted == NULL) {
            fclose(fichier);
            printf("Erreur d'allocation memoire (unsorted) pour n = %d\n", n);
            return 1;
        }

        char chemin_unsorted[100];

        sprintf(chemin_unsorted, "../test_data/unsorted/%d.txt", n);
        FILE *fichier_unsorted = fopen(chemin_unsorted, "r");
        if (fichier_unsorted == NULL) {
            printf("Erreur d'ouverture du fichier %s\n", chemin_unsorted);
            free(arrayUnsorted);
            fclose(fichier);
            return 1;
        }

        int i;
        for (i = 0; i < n; i++) {
            fscanf(fichier_unsorted, "%d", &arrayUnsorted[i]);
        }
        fclose(fichier_unsorted);

        // --- 2. CHARGEMENT DU FICHIER TRIÉ (pour recherche séquentielle triée et dichotomique) ---
 
      

        // --- 3. MESURES DE TEMPS ---
       long comp_naive = 0, comp_paires = 0;
        double temps_naive = calculerTemps(MaxEtMinA, arrayUnsorted, n, iterations, &comp_naive);
        double temps_paires = calculerTemps(MaxEtMinBVersion2, arrayUnsorted, n, iterations, &comp_paires);
         printf("%-10d\t%f\t\t%f\t\t%ld\t\t%ld\n", n, temps_naive, temps_paires, comp_naive, comp_paires);
      fprintf(fichier, "%d,%f,%f,%ld,%ld\n", n, temps_naive, temps_paires, comp_naive, comp_paires);
        // Libération de la mémoire pour cette taille
        free(arrayUnsorted);
        
    }
    
    fclose(fichier);
    printf("\nLes resultats ont ete enregistres dans 'resultats_complexiteMinMax.csv'.\n");

    return 0;
}