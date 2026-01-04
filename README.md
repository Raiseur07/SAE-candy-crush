======================================
     BIENVENUE DANS CANDY CRUSH !
======================================

BUT DU JEU :
Le but est d'aligner trois bonbons ou plus de la même couleur horizontalement ou verticalement. Les bonbons correspondants sont supprimés de la grille, vous gagnez des points, et de nouveaux bonbons tombent pour remplir les espaces.

RÈGLES DE JEU GÉNÉRALES :
----------------------------------------

1. LA GRILLE :
   - La grille est de taille 8x8.
   - Les bonbons sont représentés par des chiffres ('1', '2', '3', '4').
   - La case vide (KImpossible) est représentée par un point (.).

2. LE MOUVEMENT :
   - Pour jouer un coup, vous devez saisir les coordonnées (Ligne puis Colonne) du bonbon à déplacer.
   - Exemple : '0 3' pour le bonbon à la ligne 0, colonne 3.
   - Ensuite, vous devez saisir une direction pour l'échange :
     - Z : Haut
     - Q : Gauche
     - S : Bas
     - D : Droit
   - Le bonbon sélectionné échange de place avec le bonbon adjacent dans la direction indiquée.
   - Un coup est considéré comme invalide si l'échange ne crée pas d'alignement.

3. SCORING :
   - Les alignements de 3 bonbons rapportent 10 points.
   - Les alignements plus longs rapportent plus de points (4, 5, etc.).
   - Le score est cumulatif.

4. DÉROULEMENT APRÈS UN ALIGNEMENT :
   - Les bonbons alignés sont marqués KImpossible (supprimés).
   - Les bonbons situés au-dessus tombent.
   - De nouveaux bonbons sont générés en haut de la colonne pour combler les vides.
   - Si cette cascade crée un nouvel alignement (combo), le processus se répète jusqu'à ce qu'il n'y ait plus de correspondances.

MODES DE JEU :
----------------------------------------

1. MODE CLASSIQUE :
   - Objectif : Atteindre le meilleur score possible.
   - Limite : Le jeu se termine après 20 coups.

2. MODE CONTRE-LA-MONTRE :
   - Objectif : Atteindre le meilleur score possible.
   - Limite : Le jeu se termine après 60 secondes.

3. MODE CIBLE :
   - Objectif : Atteindre 1000 points.

========================================
   INSTRUCTIONS DE LANCEMENT (QT CREATOR)
========================================

Pour que le joueur puisse lancer le jeu, il doit utiliser l'environnement de développement Qt Creator avec les fichiers du projet.

1.  **Préparation des Fichiers :** Assurez-vous que le fichier de projet **candy_crush_tp.pro**, le code source **main.cpp**, et ce fichier de règles **regles_jeu.txt** sont tous dans le même dossier. Les fichiers de configuration utilisateur (.pro.user) seront créés automatiquement lors du premier lancement.

2.  **Ouvrir le Projet :** Dans Qt Creator, ouvrez le fichier **candy_crush_tp.pro** via le menu "Fichier > Ouvrir Fichier ou Projet...".

3.  **Construction (Build) :** Dans le menu "Construire" (ou "Build"), choisissez **Construire le Projet** (raccourci : Ctrl + B). Qt Creator utilise le fichier .pro (QMake) pour compiler le code C++ et créer l'exécutable.

4.  **Exécution (Run) :** Cliquez sur le bouton **Exécuter** (flèche verte, raccourci : Ctrl + R). L'environnement lancera l'exécutable du jeu dans une console, et vous pourrez interagir avec le menu de sélection des modes de jeu.
