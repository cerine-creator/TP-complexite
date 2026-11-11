#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// naive approache mF
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
        (*nb_comparaisons)++;
        if (T[i] > *max)
        {
            *max = T[i];
        }
        else
        {
            (*nb_comparaisons)++;
            if (T[i] < *min)
            {
                *min = T[i];
            }
        }
    }
}

void MaxEtMinBVersion2(int T[], int n, int *max_val, int *min_val, long *nb_comparaisons)
{
    *nb_comparaisons = 0;
    if (n <= 0) return;

    int end = n - 1;
    int i;
    
    // Phase 1 : Comparaison par paires et réarrangement temporaire
    for (i = 0; i < end; i += 2)
    {
        (*nb_comparaisons)++;
        if (T[i] < T[i + 1])
        {
            int temp = T[i];
            T[i] = T[i + 1];
            T[i + 1] = temp;
        }
    }

    // Phase 2 : Chercher le minimum parmi tous les plus petits (cases impaires)
    // et le maximum parmi tous les plus grands (cases paires)
    int min = T[1];
    int max = T[0];
    
    for (i = 2; i < end; i += 2)
    {
        (*nb_comparaisons)++;
        if (T[i] > max)
        {
            max = T[i];
        }

        (*nb_comparaisons)++;
        if (T[i + 1] < min)
        {
            min = T[i + 1];
        }
    }

    // Phase 3 : Traiter le cas où la taille du tableau est impaire
    if (n % 2 == 1)
    {
        (*nb_comparaisons)++;
        if (T[n - 1] < min)
        {
            min = T[n - 1];
        }
        
        (*nb_comparaisons)++;
        if (T[n - 1] > max)
        {
            max = T[n - 1];
        }
    }

    *max_val = max;
    *min_val = min;
}

typedef void (*MinMaxFn)(int[], int, int*, int*, long*); 

double calculerTemps(MinMaxFn fonctionRecherche, int T[], int n, int iterations, long *comp_out, int *max_out, int *min_out)
{
    clock_t start, end;
    start = clock();

    int k;
    for (k = 0; k < iterations; k++)
    {
        fonctionRecherche(T, n, max_out, min_out, comp_out);
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
    // En-tête CSV enrichi avec min et max
    fprintf(fichier, "n,temps_naive (s),temps_paires (s),comp_naive,comp_paires,min_A,max_A,min_B,max_B\n");
      
    int tailles[] = {100000, 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000, 1600000, 1800000, 2000000, 4000000, 5000000, 6000000, 7000000, 8000000}; 
    int nb_tailles = 16;
    int iterations = 1000;

    printf("n\t\t naive (s)\t paires (s)\t comp_n\t\t comp_p\t\t [Min - Max]\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    
    int t;
    for (t = 0; t < nb_tailles; t++) {
        int n = tailles[t];
         
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

        // Duplication du tableau pour que la version B (qui modifie l'ordre temporairement) 
        // ne fausse pas l'exécution de la version A si elles tournent sur le même contenu.
        int *arrayUnsortedB = (int *)malloc(n * sizeof(int));
        for (i = 0; i < n; i++) {
            arrayUnsortedB[i] = arrayUnsorted[i];
        }

        long comp_naive = 0, comp_paires = 0;
        int min_naive = 0, max_naive = 0;
        int min_paires = 0, max_paires = 0;

        double temps_naive = calculerTemps(MaxEtMinA, arrayUnsorted, n, iterations, &comp_naive, &max_naive, &min_naive);
        double temps_paires = calculerTemps(MaxEtMinBVersion2, arrayUnsortedB, n, iterations, &comp_paires, &max_paires, &min_paires);
         
        // Affichage console formaté
        printf("%-10d\t%.4fs\t\t%.4fs\t\t%-10ld\t%-10ld\t[%d, %d]\n", 
               n, temps_naive, temps_paires, comp_naive, comp_paires, min_naive, max_naive);
        
        // Enregistrement dans le fichier CSV
        fprintf(fichier, "%d,%f,%f,%ld,%ld,%d,%d,%d,%d\n", 
                n, temps_naive, temps_paires, comp_naive, comp_paires, min_naive, max_naive, min_paires, max_paires);
    
        free(arrayUnsorted);
        free(arrayUnsortedB);
    }
    
    fclose(fichier);
    printf("\nLes resultats ont ete enregistres dans 'resultats_complexiteMinMax.csv'.\n");

    return 0;
}