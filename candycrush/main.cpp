#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
#include <string>
#include <fstream>

using namespace std;

// --- 1. LES DEFINITIONS ET LES CONSTANTES ---
/**
 * @brief Constantes pour les couleurs dans le terminal
 * @details Utilisées pour que les bonbons soient en couleurs
 */

const unsigned KReset (0);
const unsigned KNoir (30);
const unsigned KRouge (31);
const unsigned KRVert (32);
const unsigned KJaune (33);
const unsigned KBleu (34);

/**
 * @brief  Constantes pour le fond et le texte par défaut (Noir sur Fond Blanc)
 */
//
const unsigned KBG_White (47);
const unsigned KTEXT_Black (30);

/**
 * @brief Constantes du jeu
 * @details Définitions paramètres de jeu de base
 */
// Constantes de Jeu
const unsigned KNbCandies (4);  // Types de bonbons (limité à 4)
const unsigned KGridSize (8);   // Taille de la grille N x N (8x8)
const unsigned KImpossible (0); // Valeur pour les cases vides/supprimées

/**
 * @brief Objectifs drs modes
 * @see runClassicMode()
 * @see runTimeTrialMode()
 * @see runTargetMode()
 */
// Objectifs des modes
const unsigned KMaxMoves (20);      // Nombre maximal de coups (Mode Classique)
const unsigned KTimeLimit (60);     // Limite de temps pour le Mode Contre-la-montre (secondes)
const unsigned KTargetScore (1000); // Score à atteindre (Mode Cible)

/**
 * @brief Fichiers de sauvegarde
 * @ingroup score_fonctions
 */
const string KFileScoresClassic = "scores_classique.txt";
const string KFileScoresTimeTrial = "scores_clm.txt";
const string KFileScoresTarget = "scores_cible.txt";

/**
 * @brief Couleurs et symboles des bonbons
 */
// absors and symbols
const unsigned CANDY_absORS[] = {KReset, KRouge, KRVert, KBleu, KJaune};
const char CANDY_SYMBOLS[] = {' ', '1', '2', '3', '4', '5', '6', '7'};


// Type aliases

/**
 * @typedef line
 * @brief Une ligne de la grille (vector d'unsigned)
 */
typedef vector <unsigned> line;

/**
 * @typedef mat
 * @brief La grille de jeu (matrice)
 */
typedef vector <line> mat;

/**
 * @struct maPosition
 * @brief Position dans la grille du jeu
 * @var maPosition::abs
 * Abscisse (les colonnes)
 * @var maPosition::ord
 * Ordonnée (les lignes)
 */
struct maPosition {
    unsigned abs;
    unsigned ord;
}; // une position dans la grille

/**
 * @struct ScoreEntry
 * @brief // Structure pour les modes Classique et Contre-la-montre (Score élevé = meilleur)
 * @ingroup score_fonctions
 */
struct ScoreEntry {
    string pseudo;
    unsigned score;
};

/**
 * @struct TargetScoreEntry
 * @brief Score d'entrée+pour le mode Cible
 * @ingroup score_fonctions
 */
// Structure pour le mode Cible (Coups faibles = meilleur)
struct TargetScoreEntry { // NOUVEAU
    string pseudo;
    unsigned moves;
};


// --- 2. LES FONCTIONS POUR LE TERMINAL ---

/**
 * @brief Fonction qui change la couleur du texte dans le terminal
 * @param coul Code couleur 
 * @ingroup terminal_fonctions
 * @callgraph
 */

void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

/**
 * @brief Initialise le fond d'écran
 * @ingroup terminal_fonctions
 * @callgraph
 */
void initializeBackground() {
    couleur(KTEXT_Black);
    couleur(KBG_White);
}

/**
 * @brief Efface l'écran du terminal
 * @ingroup terminal_fonctions
 * @callgraph
 */
void clearScreen () {
    initializeBackground();
    cout << "\033[H\033[2J";
}

/**
 * @brief AFonction qui affiche la grille du jeu
 * @param grid Grille 
 * @param score Score actuel
 * @ingroup terminal_fonctions
 * @callergraph runClassicMode(), runTimeTrialMode(), runTargetMode()
 * @callgraph couleur()
 */
void displayGrid (const mat & grid, unsigned score) {
    clearScreen();

    couleur(KTEXT_Black);
    cout << "   SCORE: " << score << endl;
    cout << "     0 1 2 3 4 5 6 7" << endl;
    cout << "    -----------------" << endl;

    for (unsigned i = 0; i < KGridSize; ++i) {
        cout << " " << i << " | ";
        for (unsigned j = 0; j < KGridSize; ++j) {
            unsigned value = grid[i][j];

            if (value == KImpossible) {
                couleur(KTEXT_Black);
                cout << ". ";
            } else if (value <= KNbCandies) {
                couleur(CANDY_absORS[value]);
                cout << CANDY_SYMBOLS[value] << " ";
            } else {
                couleur(KTEXT_Black);
                cout << "? ";
            }
        }
        couleur(KTEXT_Black);
        cout << endl;
    }
    cout << "    -----------------" << endl;
}


// --- 3. LES SCORES ---

/**
 * @brief Lit les entrées de score (ScoreEntry) depuis un fichier. 
 * @return scores
 * @ingroup score_fonctions
 * @callergraph runClassicMode(), runTimeTrialMode(), displayBestScores()
 * @callgraph
 */

vector<ScoreEntry> loadScores(const string & fileName) {
    vector<ScoreEntry> scores;
    ifstream file(fileName);

    if (file.is_open()) {
        ScoreEntry entry;
        while (file >> entry.score >> entry.pseudo) {
            scores.push_back(entry);
        }
        file.close();
    }
    return scores;
}

/**
 * @brief Lit les entrées de score du Mode Cible (TargetScoreEntry) depuis un fichier.
 * @param fileName Nom du fichier
 * @return scores 
 * @ingroup score_fonctions
 * @callergraph runTargetMode(), displayBestTargetScores()
 * @callgraph
 */

vector<TargetScoreEntry> loadTargetScores(const string & fileName) {
    vector<TargetScoreEntry> scores;
    ifstream file(fileName);

    if (file.is_open()) {
        TargetScoreEntry entry;
        while (file >> entry.moves >> entry.pseudo) {
            scores.push_back(entry);
        }
        file.close();
    }
    return scores;
}

/**
 * @brief Sauvegarde le vecteur d'entrées de score (ScoreEntry) dans un fichier.
 * @param fileName Nom du fichier
 * @param scores Scores à sauvegarder
 * @ingroup score_fonctions
 * @callergraph runClassicMode(), runTimeTrialMode()
 * @callgraph
 */
void saveScores(const string & fileName, const vector<ScoreEntry> & scores) {
    ofstream file(fileName);

    if (file.is_open()) {
        for (const ScoreEntry & entry : scores) {
            file << entry.score << " " << entry.pseudo << endl;
        }
        file.close();
    } else {
        cout << "Error: Cannot open save file " << fileName << endl;
    }
}

/**
 * @brief Sauvegarde le vecteur d'entrées de score du Mode Cible (TargetScoreEntry) dans un fichier.
 * @param fileName Nom du fichier
 * @param scores Vecteur des scores cible
 * @ingroup score_fonctions
 * @callergraph runTargetMode()
 * @callgraph
 */
void saveTargetScores(const string & fileName, const vector<TargetScoreEntry>& scores) {
    ofstream file(fileName);

    if (file.is_open()) {
        for (const TargetScoreEntry & entry : scores) {
            file << entry.moves << " " << entry.pseudo << endl;
        }
        file.close();
    } else {
        cout << "Error: Cannot open save file " << fileName << endl;
    }
}

/**
 * @brief Affiche les meilleurs scores pour un mode donné.
 * @param modeName Nom du mode
 * @param scores Vecteur des scores
 * @ingroup score_fonctions
 * @callergraph runClassicMode(), runTimeTrialMode()
 * @callgraph
 */
void displayBestScores(const string & modeName, const vector<ScoreEntry> & scores) {
    couleur(KTEXT_Black);
    cout << "\n--- Meilleurs scores (" << modeName << ") ---" << endl;
    if (scores.empty()) {
        cout << "Aucun score enregistre pour l'instant." << endl;
    } else {
        // Affiche max 10 scores
        for (size_t i = 0; i < min((size_t)10, scores.size()); ++i) {
            cout << i + 1 << ". " << scores[i].pseudo << " : " << scores[i].score << " points" << endl;
        }
    }
    cout << "--------------------------------------" << endl;
}

/**
 * @brief Affiche les meilleurs scores pour le Mode Cible (basé sur les coups).
 * @param modeName Nom du mode
 * @param scores Vecteur des scores cible
 * @ingroup score_fonctions
 * @callergraph runTargetMode()
 * @callgraph
 */
void displayBestTargetScores(const string & modeName, const vector<TargetScoreEntry> & scores) {
    couleur(KTEXT_Black);
    cout << "\n--- Classement (" << modeName << ") ---" << endl;
    cout << "Objectif : " << KTargetScore << " points" << endl;
    cout << "--------------------------------------" << endl;
    if (scores.empty()) {
        cout << "Aucun score enregistre pour l'instant." << endl;
    } else {
        // Affiche max 10 scores
        for (size_t i = 0; i < min((size_t)10, scores.size()); ++i) {
            cout << i + 1 << ". " << scores[i].pseudo << " : " << scores[i].moves << " coups" << endl;
        }
    }
    cout << "--------------------------------------" << endl;
}


// --- 4. LES MATCHS ET LES MOUVEMENTS ---

/**
 * @brief Regarde si il y a 3 ou plus de chiffres identiques après l'initialisation.
 * @param grid Grille à vérifier
 * @return true si match trouvé, sinon false
 * @ingroup match_fonctions
 * @callergraph initGrid()
 * @callgraph
 */
bool checkInitialMatch(const mat & grid) {
    for (unsigned i = 0; i < KGridSize; ++i) {
        for (unsigned j = 0; j <= KGridSize - 3; ++j) {
            unsigned type = grid[i][j];
            if (type != KImpossible && type == grid[i][j+1] && type == grid[i][j+2]) return true;
        }
    }
    for (unsigned j = 0; j < KGridSize; ++j) {
        for (unsigned i = 0; i <= KGridSize - 3; ++i) {
            unsigned type = grid[i][j];
            if (type != KImpossible && type == grid[i+1][j] && type == grid[i+2][j]) return true;
        }
    }
    return false;
}

/**
 * @brief Initialise toutes les cellules de la grille avec des chiffres aléatoires, en recommençant si checkInitialMatch n'est pas respecté.
 * @param grid Grille à initialiser
 * @param matSize Taille de la grille
 * @ingroup match_fonctions
 * @callergraph runClassicMode(), runTimeTrialMode(), runTargetMode()
 * @callgraph checkInitialMatch()
 */
void initGrid (mat & grid, const size_t & matSize) {
    grid.resize(matSize, line(matSize));

    do {
        for (unsigned i = 0; i < matSize; ++i) {
            for (unsigned j = 0; j < matSize; ++j) {
                // Génère un nombre entre 1 et KNbCandies (4 max)
                grid[i][j] = (rand() % KNbCandies) + 1;
            }
        }
    } while (checkInitialMatch(grid));
}

/**
 * @brief Enlève tout les bonbons sur les positions données, applique la gravtié et remplit avec de nouveaux bonbons.
 * @param grid Grille
 * @param pos Position de départ
 * @param howMany Nombre de cases à supprimer
 * @ingroup match_fonctions
 * @callergraph removalInRow(), atLeastThreeInAColumn()
 * @callgraph
 */
void removalInColumn (mat & grid, const maPosition & pos, unsigned howMany) {
    unsigned abs = pos.abs;
    unsigned startord = pos.ord;

    for (unsigned i = startord; i < startord + howMany; ++i) {
        if (i < KGridSize && grid[i][abs] != KImpossible) {
            grid[i][abs] = KImpossible;
        }
    }

    int next_write_ord = KGridSize - 1;

    for (int i = KGridSize - 1; i >= 0; --i) {
        if (grid[i][abs] != KImpossible) {
            if (i != next_write_ord) {
                grid[next_write_ord][abs] = grid[i][abs];
                grid[i][abs] = KImpossible;
            }
            next_write_ord--;
        }
    }

    for (unsigned i = 0; i < KGridSize; ++i) {
        if (grid[i][abs] == KImpossible) {
            grid[i][abs] = (rand() % KNbCandies) + 1;
        }
    }
}

/**
 * @brief Enlève tout les bonbons sur les positions données et appelle removalInColumn pour qu'il s'occupe de la gravité et du remplissage de nouveaux bonbons.
 * @param grid Grille
 * @param pos Position de départ
 * @param howMany Nombre de cases à supprimer
 * @ingroup match_fonctions
 * @callergraph atLeastThreeInARow()
 * @callgraph removalInColumn()
 */
void removalInRow (mat & grid, const maPosition & pos, unsigned howMany) {
    unsigned ord = pos.ord;
    unsigned startabs = pos.abs;

    for (unsigned j = startabs; j < startabs + howMany; ++j) {
        if (j < KGridSize) {
            grid[ord][j] = KImpossible;
        }
    }

    for (unsigned j = startabs; j < startabs + howMany; ++j) {
        if (j < KGridSize) {
            maPosition tmpPos = {j, 0};
            removalInColumn(grid, tmpPos, 0);
        }
    }
}

/**
 * @brief Cherche pour un match vertical de 3 ou plus bonbons.
 * @param grid Grille
 * @param[out] pos Position premier bonbon du match
 * @param[out] howMany Nombre de bonbons dans le match
 * @return true si un match est trouvé
 * @ingroup match_fonctions
 * @callergraph runClassicMode(), runTimeTrialMode(), runTargetMode()
 * @callgraph removalInColumn()
 */
bool atLeastThreeInAColumn (const mat & grid, maPosition & pos, unsigned & howMany) {
    for (unsigned j = 0; j < KGridSize; ++j) {
        for (unsigned i = 0; i <= KGridSize - 3; ++i) {
            unsigned type = grid[i][j];
            if (type == KImpossible) continue;

            if (type == grid[i+1][j] && type == grid[i+2][j]) {
                howMany = 3;
                unsigned k = i + 3;
                while (k < KGridSize && grid[k][j] == type) {
                    howMany++;
                    k++;
                }
                pos = {j, i};
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Cherche pour un match horizontal de 3 ou plus bonbons.
 * @param grid Grille
 * @param[out] pos Position premier bonbon du match
 * @param[out] howMany Nombre de bonbons dans le match
 * @return true si un match est trouvé
 * @ingroup match_fonctions
 * @callergraph runClassicMode(), runTimeTrialMode(), runTargetMode()
 * @callgraph removalInRow()
 */
bool atLeastThreeInARow (const mat & grid, maPosition & pos, unsigned & howMany) {
    for (unsigned i = 0; i < KGridSize; ++i) {
        for (unsigned j = 0; j <= KGridSize - 3; ++j) {
            unsigned type = grid[i][j];
            if (type == KImpossible) continue;

            if (type == grid[i][j+1] && type == grid[i][j+2]) {
                howMany = 3;
                unsigned k = j + 3;
                while (k < KGridSize && grid[i][k] == type) {
                    howMany++;
                    k++;
                }
                pos = {j, i};
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Echange les bonbons à une position donnée avec la direction donnée.
 * @param grid Grille à modifier
 * @param pos Position du premier bonbon
 * @param direction Direction du déplacement
 * @ingroup match_fonctions
 * @callergraph runClassicMode(), runTimeTrialMode(), runTargetMode()
 * @callgraph
 */
void makeAMove (mat & grid, const maPosition & pos, const char & direction) {
    unsigned r2 = pos.ord;
    unsigned c2 = pos.abs;

    switch (direction) {
    case 'Q': // Gauche
        if (pos.abs == 0) return;
        c2 = pos.abs - 1;
        break;
    case 'Z': // Haut
        if (pos.ord == 0) return;
        r2 = pos.ord - 1;
        break;
    case 'D': // Droit
        if (pos.abs == KGridSize - 1) return;
        c2 = pos.abs + 1;
        break;
    case 'S': // Bas
        if (pos.ord == KGridSize - 1) return;
        r2 = pos.ord + 1;
        break;
    default:
        return;
    }

    swap(grid[pos.ord][pos.abs], grid[r2][c2]);
}


// --- 5. LES MODES DE JEUX ---

/**
 * @brief Boucle principale pour le Mode Classique (Coups limités, Meilleur score).
 * @param userPseudo Pseudo du joueur
 * @ingroup game_modes
 * @callergraph main()
 * @callgraph initGrid(), displayGrid(), makeAMove(), atLeastThreeInAColumn(), 
 * atLeastThreeInARow(), removalInColumn(), removalInRow(),
 * loadScores(), saveScores(), displayBestScores()
 */
void runClassicMode(const string & userPseudo) {
    mat grid;
    initGrid(grid, KGridSize);

    unsigned score = 0;
    unsigned currentMoves = 0;
    int r1, c1;
    char direction;

    cout << "--- Mode Classique: Meilleur score en " << KMaxMoves << " coups ---" << endl;

    while (currentMoves < KMaxMoves) {
        displayGrid(grid, score);
        couleur(KTEXT_Black);
        cout << "COUPS RESTANTS : " << KMaxMoves - currentMoves << " / " << KMaxMoves << endl;

        // --- Saisie ---
        cout << "\nEntrez la Ligne et la absonne du nombre a deplacer (ex: 0 0) : ";
        if (!(cin >> r1 >> c1)) break;

        if (r1 < 0 || r1 >= (int)KGridSize || c1 < 0 || c1 >= (int)KGridSize || grid[r1][c1] == KImpossible) {
            std::cin.clear(); // Effacer les indicateurs d'erreur
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer l'entrée invalide
            std::cout << "Entrée invalide. Veuillez réessayer.\n"; //source : https://labex.io/fr/tutorials/cpp-how-to-handle-cin-input-validation-427285
            continue;
        }

        cout << "Entrez la Direction (Q:gauche, Z:haut, D:droit, S:bas) : ";
        if (!(cin >> direction)) break;
        direction = toupper(direction);

        if (direction != 'Q' && direction != 'Z' && direction != 'D' && direction != 'S') {
            cout << "Direction invalide. Reessayez." << endl;
            continue;
        }

        // --- Déplacement et Scoring ---
        maPosition pos = {(unsigned)c1, (unsigned)r1};
        makeAMove(grid, pos, direction);
        currentMoves++;

        bool moved = true;
        unsigned comboLevel = 0;

        // Boucle de réaction en chaîne
        while (moved) {
            moved = false;
            unsigned howMany = 0;
            maPosition matchPos;
            bool matchFound = false;

            // Détection du match
            if (atLeastThreeInAColumn(grid, matchPos, howMany)) {
                removalInColumn(grid, matchPos, howMany);
                matchFound = true;
            }
            else if (atLeastThreeInARow(grid, matchPos, howMany)) {
                removalInRow(grid, matchPos, howMany);
                matchFound = true;
            }

            // Mise à jour du score si un match a eu lieu
            if (matchFound) {
                comboLevel++;
                unsigned baseScore = howMany * 2;
                unsigned comboBonus = baseScore * comboLevel;

                score += comboBonus;
                moved = true;

                cout << "\n> match de " << howMany << "! COMBO x" << comboLevel
                     << " ! Score: +" << comboBonus << " (Base: " << baseScore << ")" << endl;
                displayGrid(grid, score);
            }
        }
    }

    // Condition de fin
    clearScreen();
    cout << "\n========================================" << endl;
    cout << "           FIN DU MODE CLASSIQUE           " << endl;
    cout << "   Score Final pour " << userPseudo << " : " << score << endl;
    cout << "========================================" << endl;

    // LOGIQUE DE SAUVEGARDE DE SCORE
    vector<ScoreEntry> scores = loadScores(KFileScoresClassic);
    ScoreEntry newEntry = {userPseudo, score};
    scores.push_back(newEntry);

    // Tri par insertion
    for (size_t i = 1; i < scores.size(); ++i) {
        ScoreEntry x = scores[i];
        int j = i;
        while (j > 0 && scores[j - 1].score < x.score) {
            scores[j] = scores[j - 1];
            j = j - 1;
        }

        scores[j] = x;
    }

    saveScores(KFileScoresClassic, scores);
    displayBestScores("Classique", scores);

    // Attendre l'entrée utilisateur
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer l'entrée invalide
    cout << "Appuyez sur ENTREE pour continuer...";
    cin.get();
}

/**
 * @brief Boucle principale pour le Mode Contre-la-montre (Temps limité, Meilleur score).
 * @param userPseudo Pseudo du joueur
 * @ingroup game_modes
 * @callergraph main()
 * @callgraph initGrid(), displayGrid(), makeAMove(), atLeastThreeInAColumn(), 
 * atLeastThreeInARow(), removalInColumn(), removalInRow(),
 * loadScores(), saveScores(), displayBestScores()
 */
void runTimeTrialMode(const string& userPseudo) {
    mat grid;
    initGrid(grid, KGridSize);

    unsigned score = 0;
    int r1, c1;
    char direction;

    time_t startTime = time(NULL);
    double elapsedTime; // Temps écoulé en français

    cout << "--- Mode Contre-la-montre: 4 types de bonbons - " << KTimeLimit << " secondes ---" << endl;

    while (true) {
        elapsedTime = difftime(time(NULL), startTime); // source : https://en.cppreference.com/w/cpp/chrono/c/difftime
        double timeRemaining = KTimeLimit - elapsedTime;

        if (timeRemaining <= 0) {
            break;
        }

        displayGrid(grid, score);
        couleur(KTEXT_Black);
        cout << "TEMPS RESTANT : ";
        // Affiche la couleur du temps en fonction de l'urgence
        if (timeRemaining <= 10) couleur(KRouge);
        else if (timeRemaining <= 20) couleur(KJaune);
        else couleur(KRVert);
        cout << (int)timeRemaining << " secondes";
        couleur(KTEXT_Black);
        cout << endl;

        // --- Saisie ---
        cout << "\nEntrez la Ligne et la absonne du nombre a deplacer (ex: 0 0) : ";
        if (!(cin >> r1 >> c1)) break;

        if (r1 < 0 || r1 >= (int)KGridSize || c1 < 0 || c1 >= (int)KGridSize || grid[r1][c1] == KImpossible) {
            std::cin.clear(); // Effacer les indicateurs d'erreur
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer l'entrée invalide
            std::cout << "Entrée invalide. Veuillez réessayer.\n"; //source : https://labex.io/fr/tutorials/cpp-how-to-handle-cin-input-validation-427285
            continue;
        }

        cout << "Entrez la Direction (Q:gauche, Z:haut, D:droit, S:bas) : ";
        if (!(cin >> direction)) break;
        direction = toupper(direction);

        if (direction != 'Q' && direction != 'Z' && direction != 'D' && direction != 'S') {
            cout << "Direction invalide. Reessayez." << endl;
            continue;
        }

        // --- Déplacement et Scoring ---
        maPosition pos = {(unsigned)c1, (unsigned)r1};
        makeAMove(grid, pos, direction);

        bool moved = true;
        unsigned comboLevel = 0;

        // Boucle de réaction en chaîne
        while (moved) {
            moved = false;
            unsigned howMany = 0;
            maPosition matchPos;
            bool matchFound = false;

            if (atLeastThreeInAColumn(grid, matchPos, howMany)) {
                removalInColumn(grid, matchPos, howMany);
                matchFound = true;
            }
            else if (atLeastThreeInARow(grid, matchPos, howMany)) {
                removalInRow(grid, matchPos, howMany);
                matchFound = true;
            }

            if (matchFound) {
                comboLevel++;
                unsigned baseScore = howMany * 2;
                unsigned comboBonus = baseScore * comboLevel;

                score += comboBonus;
                moved = true;

                cout << "\n> match de " << howMany << "! COMBO x" << comboLevel
                     << " ! Score: +" << comboBonus << " (Base: " << baseScore << ")" << endl;
                displayGrid(grid, score);
            }
        }
    }

    // --- Fin du jeu ---
    clearScreen();
    cout << "\n========================================" << endl;
    cout << "              TEMPS ÉCOULÉ !              " << endl;
    cout << "   Score Final pour " << userPseudo << " : " << score << endl;
    cout << "========================================" << endl;

    // LOGIQUE DE SAUVEGARDE DE SCORE
    vector<ScoreEntry> scores = loadScores(KFileScoresTimeTrial);
    ScoreEntry newEntry = {userPseudo, score};
    scores.push_back(newEntry);

    // Tri par insertion
    for (size_t i = 1; i < scores.size(); ++i) {
        ScoreEntry x = scores[i];
        int j = i;
        while (j > 0 && scores[j - 1].score < x.score) {
            scores[j] = scores[j - 1];
            j = j - 1;
        }

        scores[j] = x;
    }

    saveScores(KFileScoresTimeTrial, scores);
    displayBestScores("Contre-la-montre", scores);

    // Attendre l'entrée utilisateur
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer l'entrée invalide
    cout << "Appuyez sur ENTREE pour continuer...";
    cin.get();
}

/**
 * @brief Boucle principale pour le Mode Cible (Atteindre 1000 de score avec le moins de coups).
 * @param userPseudo Pseudo du joueur
 * @ingroup game_modes
 * @callergraph main()
 * @callgraph initGrid(), displayGrid(), makeAMove(), atLeastThreeInAColumn(), 
 * atLeastThreeInARow(), removalInColumn(), removalInRow(),
 * loadTargetScores(), saveTargetScores(), displayBestTargetScores()
 */
void runTargetMode(const string& userPseudo) {
    mat grid;
    initGrid(grid, KGridSize);

    unsigned score = 0;
    unsigned currentMoves = 0;
    int r1, c1;
    char direction;

    cout << "--- Mode Cible: Atteindre " << KTargetScore << " points (Coups minimum) ---" << endl;

    while (score < KTargetScore) {
        displayGrid(grid, score);
        couleur(KTEXT_Black);
        cout << "COUPS UTILISES : " << currentMoves << endl;
        cout << "OBJECTIF : " << KTargetScore << " points" << endl;

        // --- Saisie ---
        cout << "\nEntrez la Ligne et la absonne du nombre a deplacer (ex: 0 0) : ";
        if (!(cin >> r1 >> c1)) break;

        if (r1 < 0 || r1 >= (int)KGridSize || c1 < 0 || c1 >= (int)KGridSize || grid[r1][c1] == KImpossible) {
            std::cin.clear(); // Effacer les indicateurs d'erreur
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer l'entrée invalide
            std::cout << "Entrée invalide. Veuillez réessayer.\n"; //source : https://labex.io/fr/tutorials/cpp-how-to-handle-cin-input-validation-427285
            continue;
        }

        cout << "Entrez la Direction (Q:gauche, Z:haut, D:droit, S:bas) : ";
        if (!(cin >> direction)) break;
        direction = toupper(direction);

        if (direction != 'Q' && direction != 'Z' && direction != 'D' && direction != 'S') {
            cout << "Direction invalide. Reessayez." << endl;
            continue;
        }

        // --- Déplacement et Scoring ---
        maPosition pos = {(unsigned)c1, (unsigned)r1};
        makeAMove(grid, pos, direction);
        currentMoves++;

        bool moved = true;
        unsigned comboLevel = 0;

        // Boucle de réaction en chaîne
        while (moved) {
            moved = false;
            unsigned howMany = 0;
            maPosition matchPos;
            bool matchFound = false;

            // Détection du match
            if (atLeastThreeInAColumn(grid, matchPos, howMany)) {
                removalInColumn(grid, matchPos, howMany);
                matchFound = true;
            }
            else if (atLeastThreeInARow(grid, matchPos, howMany)) {
                removalInRow(grid, matchPos, howMany);
                matchFound = true;
            }

            // Mise à jour du score si un match a eu lieu
            if (matchFound) {
                comboLevel++;
                unsigned baseScore = howMany * 2;
                unsigned comboBonus = baseScore * comboLevel;

                score += comboBonus;
                moved = true;

                cout << "\n> match de " << howMany << "! COMBO x" << comboLevel
                     << " ! Score: +" << comboBonus << " (Base: " << baseScore << ")" << endl;
                displayGrid(grid, score);
            }
        }
    }

    // Condition de fin (Objectif atteint)
    clearScreen();
    cout << "\n========================================" << endl;
    cout << "           OBJECTIF ATTEINT !           " << endl;
    cout << "   Score : " << score << " points" << endl;
    cout << "   Coups utilises : " << currentMoves << endl;
    cout << "========================================" << endl;

    // LOGIQUE DE SAUVEGARDE DE SCORE (Coups minimum)
    vector<TargetScoreEntry> scores = loadTargetScores(KFileScoresTarget);
    TargetScoreEntry newEntry = {userPseudo, currentMoves};
    scores.push_back(newEntry);

    // Tri par insertion
    for (size_t i = 1; i < scores.size(); ++i) {
        TargetScoreEntry x = scores[i];
        int j = i;
        while (j > 0 && scores[j - 1].moves < x.moves) {
            scores[j] = scores[j - 1];
            j = j - 1;
        }

        scores[j] = x;
    }

    saveTargetScores(KFileScoresTarget, scores);
    displayBestTargetScores("Mode Cible", scores);

    // Attendre l'entrée utilisateur
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer l'entrée invalide
    cout << "Appuyez sur ENTREE pour continuer...";
    cin.get();
}


// --- 6. MAIN ALGORITHM (Menu) ---

/**
 * @brief Affiche le Menu principal du jeu
 * @callergraph main()
 * @callgraph clearScreen(), couleur()
 */
void displayMenu() {
    clearScreen();
    couleur(KTEXT_Black);
    cout << "========================================" << endl;
    cout << "         SELECTIONNEZ LE MODE DE JEU             " << endl;
    cout << "========================================" << endl;
    cout << "1. Mode Classique (Meilleur score en " << KMaxMoves << " coups)" << endl;
    cout << "2. Mode Contre-la-montre (Meilleur score en " << KTimeLimit << " secondes)" << endl;
    cout << "3. Mode Cible (Atteindre " << KTargetScore << " points, coups minimum)" << endl;
    cout << "4. Quitter" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Entrez votre choix : ";
}

int main() {
    srand(time(0));
    string userPseudo;
    int choice;

    // Saisie du pseudo
    clearScreen();
    couleur(KTEXT_Black);
    cout << "======================================" << endl;
    cout << "     BIENVENUE DANS CANDY CRUSH !     " << endl;
    cout << "======================================" << endl;
    cout << "Entrez votre pseudo: ";
    if (!(cin >> userPseudo)) {
        couleur(KReset);
        return 0;
    }
    // Vider la mémoire après la saisie du pseudo
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer l'entrée invalide

    do {
        displayMenu();
        if (!(cin >> choice)) break;

        // Vider la mémoire après la saisie du choix
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer l'entrée invalide

        switch (choice) {
        case 1:
            runClassicMode(userPseudo);
            break;
        case 2:
            runTimeTrialMode(userPseudo);
            break;
        case 3:
            runTargetMode(userPseudo);
            break;
        case 4:
            cout << "Au revoir!" << endl;
            break;
        default:
            cout << "Choix invalide. Reessayez." << endl;
            cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer l'entrée invalide
        }
    } while (choice != 4);

    // Réinitialisation de la couleur du terminal avant de quitter
    couleur(KReset);

    return 0;
}
