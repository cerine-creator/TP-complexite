# TP 1 - RSD : Complexité des Algorithmes de Primalité

Ce projet compare le temps d'exécution et la complexité de différents algorithmes de test de primalité pour un même problème :
- **A1** : Approche naïve $O(N)$
- **A2** : Méthode moitié $O(N/2)$
- **A3** : Méthode racine carrée $O(\sqrt{N})$
- **A4** : Méthode racine carrée avec saut des pairs (*Odd Skip*) $O(\frac{\sqrt{N}}{2})$

---

## 📁 Jeux de Données (`data/`)

Les fichiers de test contiennent des nombres (un par ligne) au format `.csv` :
- `data/Random100.csv` : 100 nombres premiers tirés aléatoirement
- `data/Random1000.csv` : 1000 nombres premiers tirés aléatoirement
- `data/Test-1.csv` : 100 nombres premiers dans l'intervalle $[1000, 1000000]$
- `data/Test-2.csv` : Échantillon contrôlé par nombre de chiffres (de 3 à 12 chiffres)
- `data/Test-3.csv` : Échantillon structuré de nombres (premiers et composés) à 3, 6, 9 et 12 chiffres

---

## 🚀 Comment Compiler et Exécuter dans le Terminal

> **Note :** Si la commande `gcc` n'est pas reconnue dans votre terminal, ouvrez un nouveau terminal ou utilisez le chemin direct `C:\msys64\mingw64\bin\gcc.exe`.

### 1. Algorithme 2 (A2 - Méthode Moitié)

#### Étape 1 : Compiler
Dans le dossier racine du projet (`tpAlgoRsd`) :
```powershell
gcc algo2/A2.c -o algo2/A2.exe
```

#### Étape 2 : Exécuter
```powershell
.\algo2\A2.exe
```

#### Étape 3 : Donner le fichier d'entrée
Le programme lit automatiquement dans `data/` et écrit dans `results/`. Tapez simplement le nom du fichier :
```text
Entrez le nom du fichier : Random100.csv
```
Le fichier résultat sera enregistré dans le dossier `results/` : `results/Random100.csv_A2_resultat.csv`.

---

### 2. Algorithme 4 (A4 - Odd Skip / Racine Carrée)

#### Étape 1 : Compiler
```powershell
gcc algo4/A4.c -o algo4/A4.exe
```

#### Étape 2 : Exécuter
```powershell
.\algo4\A4.exe
```

#### Étape 3 : Donner le fichier d'entrée
```text
Entrez le nom du fichier : Random100.csv
```
Le fichier résultat sera enregistré dans le dossier `results/` : `results/Random100.csv_A4_resultat.csv`.

---

## 📊 Fichiers Résultats

Chaque exécution génère un fichier CSV contenant deux colonnes :
```csv
Nombre,Temps (secondes)
406697,0.0000010000
932051,0.0000020000
...
```
Ces résultats permettent de tracer les courbes de temps et de comparer directement les performances de chaque algorithme.

