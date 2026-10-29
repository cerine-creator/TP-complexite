#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir_compat(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define mkdir_compat(dir) mkdir(dir, 0755)
#endif

// ============================================================================
// TP N°1 : Test de Primalite - Master 1 (IL & RSD) - USTHB
// ============================================================================

// Algorithme 1 (A1) : Approche naive O(N)
// Teste la divisibilite de N par tous les entiers de 2 a N-1
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

// Algorithme 2 (A2) : Amelioration naive O(N/2)
// Tout diviseur i de N verifie i <= N/2 (avec i != N)
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

// Algorithme 3 (A3) : Propriete de la racine carree O(sqrt(N))
// Les diviseurs d'un entier N sont pour moitie <= sqrt(N) et l'autre > sqrt(N)
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

// Algorithme 4 (A4) : Racine carree + Saut des pairs (Odd Skip) O(sqrt(N)/2)
// Verifie si N est pair, sinon teste uniquement les diviseurs impairs jusqu'a sqrt(N)
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

// ============================================================================
// GESTION INTELLIGENTE DES CHEMINS (Racine ou dossier main/)
// ============================================================================

// Trouve le fichier d'entree peu importe le repertoire courant d'execution
int resoudreCheminEntree(const char *nomSaisi, char *cheminTrouve, char *nomBase)
{
    char candidats[8][260];
    int nb = 0;

    // 1. Tel que saisi
    snprintf(candidats[nb++], sizeof(candidats[0]), "%s", nomSaisi);
    // 2. data/<saisi>
    snprintf(candidats[nb++], sizeof(candidats[0]), "data/%s", nomSaisi);
    // 3. ../data/<saisi> (si execute depuis ./main/)
    snprintf(candidats[nb++], sizeof(candidats[0]), "../data/%s", nomSaisi);

    // Si pas d'extension .csv specifiee, tenter avec .csv
    if (strstr(nomSaisi, ".csv") == NULL && strstr(nomSaisi, ".CSV") == NULL)
    {
        snprintf(candidats[nb++], sizeof(candidats[0]), "%s.csv", nomSaisi);
        snprintf(candidats[nb++], sizeof(candidats[0]), "data/%s.csv", nomSaisi);
        snprintf(candidats[nb++], sizeof(candidats[0]), "../data/%s.csv", nomSaisi);
    }

    for (int i = 0; i < nb; i++)
    {
        FILE *f = fopen(candidats[i], "r");
        if (f != NULL)
        {
            fclose(f);
            strncpy(cheminTrouve, candidats[i], 259);
            cheminTrouve[259] = '\0';

            // Extraire le nom de base sans chemin de dossier
            const char *p = strrchr(nomSaisi, '/');
            if (!p) p = strrchr(nomSaisi, '\\');
            const char *base = (p != NULL) ? p + 1 : nomSaisi;

            if (strstr(base, ".csv") == NULL && strstr(base, ".CSV") == NULL)
            {
                snprintf(nomBase, 100, "%s.csv", base);
            }
            else
            {
                snprintf(nomBase, 100, "%s", base);
            }
            return 1;
        }
    }
    return 0;
}

// Trouve ou cree le dossier results adapte (results ou ../results)
void resoudreDossierSortie(char *dossierSortie)
{
    // Verifier 'results/' local
    FILE *test = fopen("results/.test_write", "w");
    if (test != NULL)
    {
        fclose(test);
        remove("results/.test_write");
        strcpy(dossierSortie, "results");
        return;
    }

    // Verifier '../results/' parent
    test = fopen("../results/.test_write", "w");
    if (test != NULL)
    {
        fclose(test);
        remove("../results/.test_write");
        strcpy(dossierSortie, "../results");
        return;
    }

    // Tenter de creer le dossier
    if (mkdir_compat("results") == 0)
    {
        strcpy(dossierSortie, "results");
        return;
    }
    if (mkdir_compat("../results") == 0)
    {
        strcpy(dossierSortie, "../results");
        return;
    }

    // Repertoire courant par defaut
    strcpy(dossierSortie, ".");
}

// ============================================================================
// EXECUTION D'UN ALGORITHME SPECIFIQUE
// ============================================================================

void executerAlgorithme(int algoNum, const char *cheminEntree, const char *dossierSortie, const char *nomBase)
{
    FILE *fichierEntree = fopen(cheminEntree, "r");
    if (fichierEntree == NULL)
    {
        printf("[ERREUR] Impossible d'ouvrir le fichier : %s\n", cheminEntree);
        return;
    }

    char nomSortie[300];
    snprintf(nomSortie, sizeof(nomSortie), "%s/%s_A%d_resultat.csv", dossierSortie, nomBase, algoNum);

    FILE *fichierSortie = fopen(nomSortie, "w");
    if (fichierSortie == NULL)
    {
        printf("[ERREUR] Impossible de creer le fichier : %s\n", nomSortie);
        fclose(fichierEntree);
        return;
    }

    const char *nomAlgoDesc = "";
    switch (algoNum)
    {
        case 1: nomAlgoDesc = "A1 (Naif O(N))"; break;
        case 2: nomAlgoDesc = "A2 (Moitie O(N/2))"; break;
        case 3: nomAlgoDesc = "A3 (Racine carree O(sqrt(N)))"; break;
        case 4: nomAlgoDesc = "A4 (Odd Skip O(sqrt(N)/2))"; break;
    }

    printf("\n------------------------------------------------------------\n");
    printf(" Lancement de l'Algorithme %s\n", nomAlgoDesc);
    printf(" Fichier de sortie : %s\n", nomSortie);
    printf("------------------------------------------------------------\n");

    fprintf(fichierSortie, "Nombre,Temps (secondes)\n");

    long long N;
    long compteur = 0;
    clock_t debut, fin;
    double tempsExecution;

    // Pour A1 et A2, 1 iteration suffit car O(N) est lent sur les grands nombres
    // Pour A3 et A4, 10000 iterations permettent de mesurer avec precision les microsecondes
    int iterations = (algoNum >= 3) ? 10000 : 1;

    while (fscanf(fichierEntree, "%lld", &N) == 1)
    {
        printf("Traite [%ld] : %lld en cours...", compteur + 1, N);
        fflush(stdout);

        debut = clock();
        for (int k = 0; k < iterations; k++)
        {
            switch (algoNum)
            {
                case 1: estPremier1(N); break;
                case 2: estPremier2(N); break;
                case 3: estPremier3(N); break;
                case 4: estPremier4(N); break;
            }
        }
        fin = clock();

        tempsExecution = ((double)(fin - debut) / CLOCKS_PER_SEC) / iterations;

        fprintf(fichierSortie, "%lld,%.10f\n", N, tempsExecution);
        fflush(fichierSortie); // Sauvegarde immediate sur disque

        compteur++;
        printf(" Termine en %.10f s\n", tempsExecution);

        fscanf(fichierEntree, ",");
    }

    fclose(fichierEntree);
    fclose(fichierSortie);

    printf("\n[SUCCES] %ld nombres traites pour A%d.\n", compteur, algoNum);
    printf("Resultats enregistres dans : %s\n", nomSortie);
}

// ============================================================================
// MAIN
// ============================================================================

int main(void)
{
    char nomSaisi[100];
    char cheminEntree[260];
    char nomBase[100];
    char dossierSortie[260];

    printf("============================================================\n");
    printf("       TP N°1 - TEST DE PRIMALITE (Master 1 RSD/IL)         \n");
    printf("============================================================\n");

    printf("Entrez le nom du fichier (ex: Random100.csv) : ");
    if (scanf("%99s", nomSaisi) != 1)
    {
        printf("Saisie invalide.\n");
        return 1;
    }

    if (!resoudreCheminEntree(nomSaisi, cheminEntree, nomBase))
    {
        printf("\n[ERREUR] Fichier introuvable !\n");
        printf("Verifiez que le fichier existe dans data/ ou dans le dossier courant.\n");
        return 1;
    }

    resoudreDossierSortie(dossierSortie);

    printf("\n[OK] Fichier d'entree : %s\n", cheminEntree);
    printf("[OK] Repertoire de sortie : %s\n\n", dossierSortie);

    printf("Choisissez l'algorithme a executer :\n");
    printf("  1. Algorithme 1 (A1 - Naif O(N))\n");
    printf("  2. Algorithme 2 (A2 - Moitie O(N/2))\n");
    printf("  3. Algorithme 3 (A3 - Racine carree O(sqrt(N)))\n");
    printf("  4. Algorithme 4 (A4 - Racine carree avec saut des pairs)\n");
    printf("  5. Executer TOUS les algorithmes (A1, A2, A3, A4)\n");
    printf("Votre choix (1-5) : ");

    int choix = 0;
    if (scanf("%d", &choix) != 1 || choix < 1 || choix > 5)
    {
        printf("Choix invalide. Sortie.\n");
        return 1;
    }

    if (choix >= 1 && choix <= 4)
    {
        executerAlgorithme(choix, cheminEntree, dossierSortie, nomBase);
    }
    else if (choix == 5)
    {
        for (int a = 1; a <= 4; a++)
        {
            executerAlgorithme(a, cheminEntree, dossierSortie, nomBase);
        }
    }

    printf("\n============================================================\n");
    printf("                    Fin de l'execution                     \n");
    printf("============================================================\n");

    return 0;
}
