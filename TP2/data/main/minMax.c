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



void MaxEtMinBVersion2(int T[], int n, int *max, int *min, long *nb_comparaisons)
{
    //hadi la variable ta7sab ch7al min mara comparina la case i mea i+1 
    *nb_comparaisons = 0;
    if (n <= 0)
    //ida tableau vide akhroj
        return;

    int i = 0;

 
    if (n % 2 == 0)
    {
        // ida kanat la taille nta3 tableau paire 
        (*nb_comparaisons)++;
        if (T[0] > T[1])
        {
            // ida kan element 0 kbir 3la lowal alors bayna bli 0 ywali max 
            *max = T[0];
            *min = T[1];
        }
        else
        {
            // el 3aks sa7i7
            *max = T[1];
            *min = T[0];
        }
        i = 2;
         // nabdaw la boucle pour i = > 2 haka
    }
    //
    else
    {
        //ida kanat la taille nta3 tableau impaire+
        //vu que tableau impaire donc l'element 0 ywali max w min
        *max = T[0];
        *min = T[0];
        // vu que tableau impaire donc l'element 0 ywali max w min 
        // w biensure que index ntea la boucle ywali 1 haka bach nabdaw la boucle men 1
        i = 1;
    }


    while (i < n - 1)
    {
        int local_max, local_min;
         // bon dirna 2 variabnle local bach ncompariwhom m3a max w min nta3na
  
         // donc si i kanat paire , nb_comparaisons++ ykon  2 w (4 6 8 10 )  mais  ida kanat impaire nb_comparaisons++ tkon 1 w tala3 (3 5 7 9 )
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
        
        
        (*nb_comparaisons)++;
        if (local_max > *max)
        {
            *max = local_max;
        }

        
        (*nb_comparaisons)++;
        if (local_min < *min)
        {
            *min = local_min;
        }

        i += 2; 
        // pour paire on avance de 2 ( 2 , 4 , 6 , 8 , 10 ) POUR impaire on avance de 2 ( 1 , 3 , 5 , 7 , 9 )
        //haka ndamno nshado deux couple 9rab w ntestiw binathom w ncompariwom m3a max w min nta3na 
    }
}

typedef void (*MinMaxFn)(int[], int, int*, int*, long*); 

// les entrees ntea fonction nom de fucntion li 7abin n7asbo son temps d'execution , tableau nta3na , taille nta3 tableau , nombre d'iteration li 7abin n3awdouha bach n7asbo le temps d'execution , w un element compteur nta3 comparaisons
double calculerTemps( MinMaxFn fonctionRecherche, int T[], int n, int iterations, long *comp_out)
{
    // bon bash n7asbo temps pour celui la , 
    //eandna deux variable max w min bach ncompariwom m3a tableau nta3na   w un element  compteur nta3 comparaisons
  //jsp 3lah dirnah lol
    int max, min;
    //bon hadi bayna variables system 
    clock_t start, end;

      // 3ayat l fucntion clock() bach n7asbo le temps nta3 execution nta3 fonction nta3na
    start = clock();

    int k ;
       //bouble tboucli le nombre diterations  w bien sure t3ayat l fucntion nombre deiterasion fois bach n7asbo le temps d'execution nta3 fonction nta3na
    for ( k = 0; k < iterations; k++)
    {
        fonctionRecherche(T, n, &max, &min, comp_out);
    }
    // 7abas clock 
    end = clock();


    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main()
{
    // initialization de fichier de resultats
    FILE *fichier = fopen("resultats_complexiteMinMax.csv", "w");
    if (fichier == NULL)
    {  
        //error 
        printf("Erreur d'ouverture du fichier de resultats !\n");
        return 1;
    }
    fprintf(fichier, "n,temps_naive (s),temps_paires (s),comp_naive,comp_paires\n");
      //les tailles de diffrent fichier de tableaux li eandi
    int tailles[] = {100000, 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000, 1600000, 1800000, 2000000, 4000000, 5000000, 6000000, 7000000, 8000000};   
    int nb_tailles = 16;
    // la taille ntea tableau li fih les taiiles 16
    int iterations = 1000;

    printf("n\t\t naive approache  (s)\t paires  (s)\n");
     int t;

    for (t = 0; t < nb_tailles; t++) {
        int n = tailles[t];
        
         //dir espace memoire lil unsorted tableau fi la ram , basically ra7 ndiro une espace memoire tshad GA3 LES 
         //LES CAS DE TABLEAUX LI FI GA3 LES FICHIERS NTEA LES TABLEAUX !!!
        int *arrayUnsorted = (int *)malloc(n * sizeof(int));
        if (arrayUnsorted == NULL) {
            //ERROR D'ALLOCATION MEMOIRE
            fclose(fichier);
            printf("Erreur d'allocation memoire (unsorted) pour n = %d\n", n);
            return 1;
        }
        //SE 
        char chemin_unsorted[100];
          // WIN  RA7 N7ATO FICHIER F LE DOSSIER TEST_DATA/UNSORTED/ LI FIHA LES TABLEAUX NTEA LES FICHIERS NTEA LES TABLEAUX
        sprintf(chemin_unsorted, "../test_data/unsorted/%d.txt", n);
        FILE *fichier_unsorted = fopen(chemin_unsorted, "r");
        if (fichier_unsorted == NULL) {
            printf("Erreur d'ouverture du fichier %s\n", chemin_unsorted);
           //KHALI ESPACE W LIBIRIHA 
            free(arrayUnsorted);
            //AGHLO9 L FICHIER
            fclose(fichier);
            return 1;
        }

        int i;
        for (i = 0; i < n; i++) {
            // AFATH FICHIER KAMAL
            fscanf(fichier_unsorted, "%d", &arrayUnsorted[i]);
        }

        fclose(fichier_unsorted);

   
      

       long comp_naive = 0, comp_paires = 0;
       // initializithom a 0 just pur compilateur  maydorch error 
        double temps_naive = calculerTemps(MaxEtMinA, arrayUnsorted, n, iterations, &comp_naive);
        double temps_paires = calculerTemps(MaxEtMinBVersion2, arrayUnsorted, n, iterations, &comp_paires);
         printf("%-10d\t%f\t\t%f\t\t%ld\t\t%ld\n", n, temps_naive, temps_paires, comp_naive, comp_paires);
      fprintf(fichier, "%d,%f,%f,%ld,%ld\n", n, temps_naive, temps_paires, comp_naive, comp_paires);
    
        free(arrayUnsorted);
        
    }
    
    fclose(fichier);
    printf("\nLes resultats ont ete enregistres dans 'resultats_complexiteMinMax.csv'.\n");

    return 0;
}