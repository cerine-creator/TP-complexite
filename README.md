

## Comment Compiler et Exécuter dans le Terminal

> **Note :** Si la commande `gcc` n'est pas reconnue dans votre terminal, ouvrez un nouveau terminal ou utilisez le chemin direct `C:\msys64\mingw64\bin\gcc.exe`.

###  Compilation et Exécution du Programme Unifié

Le dossier `main/` regroupe l'ensemble des 4 algorithmes dans un seul programme :

#### Étape 1 : Compiler
```powershell
gcc main/main.c -o main/main.exe -lm
```

#### Étape 2 : Exécuter
Depuis la racine du projet :
```powershell
.\main\main.exe
```

#### Étape 3 : Utilisation
1. Entrez le nom du fichier (ex: `Random100.csv` ou simplement `Random100`).
2. Choisissez l'algorithme à exécuter (1 pour A1, 2 pour A2, 3 pour A3, 4 pour A4).
3. Les résultats sont enregistrés directement dans le dossier `results/` avec les temps en microsecondes.

---

##  Fichiers Résultats

Chaque exécution génère un fichier CSV contenant deux colonnes :
```csv
Nombre,Temps (secondes)
406697,0.0000010000
932051,0.0000020000
...
```
Ces résultats permettent de tracer les courbes de temps et de comparer directement les performances de chaque algorithme.

