
"""
TP3 - Generation des matrices (Exercice 1 et 2)
"""
import os
import numpy

# Exo 1 : tailles des matrices A et B
TAILLES_EXO1 = [100, 200, 400, 600, 800, 1000, 2000, 4000, 6000, 8000]

# Exo 2 : tailles de B selon la taille de A
TAILLES_B_PETITES = [10, 20, 30, 40, 50]       # pour A = 100 ... 1000
TAILLES_B_GRANDES = [100, 200, 300, 400, 500]  # pour A = 2000 ... 8000
# ----------------------------------------------------------------------

# placez script.py dans le dossier TP3, a cote du dossier Data
DOSSIER_DATA = os.path.join(os.path.dirname(os.path.abspath(__file__)), "Data")

rng = numpy.random.default_rng()

VAL_MIN = 0
VAL_MAX = 200_000


def generer(n, m, trier=False):
    """ Si trier=True, chaque ligne est triee par ordre croissant."""
    M = rng.integers(VAL_MIN, VAL_MAX, size=(n, m), dtype=numpy.int32)
    if trier:
        M.sort(axis=1)
    return M


def sauvegarder(M, chemin):
    numpy.savetxt(chemin, M, fmt="%d")
    print(f"  {chemin}  ({M.shape[0]}x{M.shape[1]})")


def main():
    dossier_exo1 = os.path.join(DOSSIER_DATA, "exo1")
    dossier_exo2 = os.path.join(DOSSIER_DATA, "exo2")
    os.makedirs(dossier_exo1, exist_ok=True)
    os.makedirs(dossier_exo2, exist_ok=True)

    # ---------------- Exercice 1 ----------------
    print("=== Exercice 1 ===")
    for n in TAILLES_EXO1:
        A = generer(n, n, trier=False)
        sauvegarder(A, os.path.join(dossier_exo1, f"A_{n}.txt"))            # A non triee
        sauvegarder(generer(n, n, trier=False), os.path.join(dossier_exo1, f"B_{n}.txt")) # B non triee

        # Exo 2 : meme matrice A, mais avec chaque ligne triee (pour sousMat2)
        # (pour sousMat1, on utilise A non triee : exo1/A_<n>.txt) 
        A.sort(axis=1)
        sauvegarder(A, os.path.join(dossier_exo2, f"A_{n}_triee.txt"))

    # ---------------- Exercice 2 ----------------
    print("\n=== Exercice 2 ===")
    for n in TAILLES_EXO1:
        tailles_b = TAILLES_B_PETITES if n <= 1000 else TAILLES_B_GRANDES
        for k in tailles_b:
            B = generer(k, k, trier=False)
            # B non triee -> sousMat1 (avec A non triee de l'exo 1)
            sauvegarder(B, os.path.join(dossier_exo2, f"B_pour_A{n}_{k}_non_triee.txt"))
            # meme B avec lignes triees -> sousMat2 (avec A triee)
            B.sort(axis=1)
            sauvegarder(B, os.path.join(dossier_exo2, f"B_pour_A{n}_{k}_triee.txt"))

    print("\nGeneration terminee avec succes.")


if __name__ == "__main__":
    main()