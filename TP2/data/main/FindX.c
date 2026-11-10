#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int rechElets_TabNonTries(int T[], int n, int x)
{
    int i = 0;
    while (i < n && T[i] != x)
    {
        i++;
    }
    if (i < n)
        return 1; // Trouvé
    return 0;     // Non trouvé
}

int rechElets_TabTries(int T[], int n, int x)
{
    int i = 0;
    while (i < n && T[i] < x)
    {
        i++;
    }
    if (i < n && T[i] == x)
        return 1;
    return 0;
}

int rechElets_Dicho(int T[], int n, int x)
{
    int g = 0, d = n - 1;
    while (g <= d)
    {
        int m = g + (d - g) / 2;
        if (T[m] == x)
            return 1;
        if (T[m] < x)
            g = m + 1;
        else
            d = m - 1;
    }
    return 0;
}

double calculerTemps(int (*fonctionRecherche)(int[], int, int), int T[], int n, int x, int iterations)
{
    clock_t start, end;

    start = clock();
    int k;
    for (k = 0; k < iterations; k++)
    {
        fonctionRecherche(T, n, x);
    }
    end = clock();

    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main()
{
    FILE *fichier = fopen("resultats_complexite.csv", "w");
    if (fichier == NULL)
    {
        printf("Erreur d'ouverture du fichier de resultats !\n");
        return 1;
    }
    fprintf(fichier, "n,NonTrie_Pire (s),Trie_Pire (s),Dicho_Pire (s)\n");

    int tailles[] = {100000, 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000, 1600000, 1800000, 2000000, 4000000, 5000000, 6000000, 7000000, 8000000};
    int nb_tailles = 16;
    int iterations = 1000;
    int x;

    printf("Entrez la valeur de x a rechercher : ");
    scanf("%d", &x);
    printf("n\t\tNonTrie_Pire (s)\tTrie_Pire (s)\tDicho_Pire (s)\n");

    int t;
    for (t = 0; t < nb_tailles; t++)
    {
        int n = tailles[t];

        // --- 1. CHARGEMENT DU FICHIER NON TRIÉ ---
        int *arrayUnsorted = (int *)malloc(n * sizeof(int));
        if (arrayUnsorted == NULL)
        {
            fclose(fichier);
            printf("Erreur d'allocation memoire (unsorted) pour n = %d\n", n);
            return 1;
        }

        char chemin_unsorted[100];
        sprintf(chemin_unsorted, "../test_data/unsorted/%d.txt", n);
        FILE *fichier_unsorted = fopen(chemin_unsorted, "r");
        if (fichier_unsorted == NULL)
        {
            printf("Erreur d'ouverture du fichier %s\n", chemin_unsorted);
            free(arrayUnsorted);
            fclose(fichier);
            return 1;
        }

        int i;
        for (i = 0; i < n; i++)
        {
            fscanf(fichier_unsorted, "%d", &arrayUnsorted[i]);
        }
        fclose(fichier_unsorted);

        // --- 2. CHARGEMENT DU FICHIER TRIÉ (pour recherche séquentielle triée et dichotomique) ---
        int *tableauSorted = (int *)malloc(n * sizeof(int));
        if (tableauSorted == NULL)
        {
            free(arrayUnsorted);
            fclose(fichier);
            printf("Erreur d'allocation memoire (sorted) pour n = %d\n", n);
            return 1;
        }

        char chemin_sorted[100];
        sprintf(chemin_sorted, "../test_data/sorted/%d.txt", n); // Assurez-vous d'avoir un dossier nommé "sorted"
        FILE *fichierSorted = fopen(chemin_sorted, "r");
        if (fichierSorted == NULL)
        {
            printf("Erreur d'ouverture du fichier %s\n", chemin_sorted);
            free(arrayUnsorted);
            free(tableauSorted);
            fclose(fichier);
            return 1;
        }

        for (i = 0; i < n; i++)
        {
            fscanf(fichierSorted, "%d", &tableauSorted[i]);
        }
        fclose(fichierSorted);

        double temps_non_trie = calculerTemps(rechElets_TabNonTries, arrayUnsorted, n, x, iterations);
        double temps_trie = calculerTemps(rechElets_TabTries, tableauSorted, n, x, iterations);
        double temps_dicho = calculerTemps(rechElets_Dicho, tableauSorted, n, x, iterations);

        int res_non_trie = rechElets_TabNonTries(arrayUnsorted, n, x);
        int res_trie = rechElets_TabTries(tableauSorted, n, x);
        int res_dicho = rechElets_Dicho(tableauSorted, n, x);

        // --- 3. AFFICHAGE PROPRE EN UNE SEULE LIGNE ---
        printf("%-10d\t%-18.6f\t%-18.6f\t%-18.6f\t| Trouve? [NT:%d | T:%d | D:%d]\n",
               n, temps_non_trie, temps_trie, temps_dicho,
               res_non_trie, res_trie, res_dicho);

fprintf(fichier, "%d,%f,%f,%f\n", n, temps_non_trie, temps_trie, temps_dicho);
        // Libération de la mémoire pour cette taille
        free(arrayUnsorted);
        free(tableauSorted);
    }

    fclose(fichier);
    printf("\nLes resultats ont ete enregistres dans 'resultats_complexite.csv'.\n");
    return 0;
}