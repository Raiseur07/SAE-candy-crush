/**
 * @file main.cpp
 * @author Mefautvosnoms (nosadresse@mail)
 * @brief Implémentation jeu type Candy Crush
 * 
 * @version 1.0
 * @date 03/01/2026
 */
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;
/**
 * @typedef CVLine
 * @brief Type représentant une ligne de la grille
 * 
 * Un CVLine est un vecteur d'entiers naturel où chaque élément représente
 * un type (couleur) de bonbon.
 */
typedef vector <unsigned> CVLine; // un type représentant une ligne de la grille

/**
 * @typedef CMat
 * @brief Type représentant la grille entière de ce jeu
 * 
 * CMat est un vecteur de CVLine (matrice 2D).
 */
typedef vector <CVLine> CMat; // un type représentant la grille

/**
 * @struct CPosition
 * @brief Structure qui représente une position dans la grille du jeu
 * Crée et type les variables abs et ord
 */
struct CPosition {
    unsigned abs;
    unsigned ord;
}; // une position dans la grille
const unsigned KReset   (0);
const unsigned KNoir    (30);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);
const unsigned KImpossible (0);

/**
 * @brief Effacer l'écran du terminal
 * 
 * Suite de caractère permettant d'effacer le contenu du terminal
 */
void clearScreen () {
    cout << "\033[H\033[2J";
}

/**
 * @brief Change la couleur du texte dans le terminal
 * @param[in] coul Code couleur qui doot être appliqués
 * 
 * le code couleur est celui défini plus haut :\n(KReset, KBleu,Kjaune, etc...)
 */
void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

/**
 * @brief Fonction de pour sortir le n-ième nombre de la suite de Fibonacci
 * @param[in] n le n-ième nombre de la suite de Fibonacci
 * 
 * @note source : https://www.delftstack.com/fr/howto/cpp/fibonacci-sequence-in-cpp/
 */
unsigned long long generateFibonacci(unsigned long long n) {
    if (n == 1) {
        return 0;
    } else if (n == 2 || n == 3) {
        return 1;
    }

    unsigned long long a = 1;
    unsigned long long b = 1;
    unsigned long long c;

    for (unsigned long long i = 3; i < n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }

    return c;
}

/**
 * @brief Fonction permettant de calculer le score
 * @param[in] howMany nombre de bonbons consécutifs
 * 
 * @note Si howMany < 3, renvoie 0
 * @note source : https://www.delftstack.com/fr/howto/cpp/fibonacci-sequence-in-cpp/
 * 
 */
int calculateScore(unsigned howMany) {
    if (howMany < 3)
    {
        return 0;
    }
    const int multiplicateur = 100;
    unsigned long long fib_term = generateFibonacci(howMany + 1);
    return (fib_term * multiplicateur);
}
/**
 * @brief Fonction qui gère la saisie d'un coup du joueur
 * @param[in] N Taille de la grille
 * @param[out] CPosition récupère la position du bonbon à déplacer
 * @param[out] direction Direction du déplacement
 * 
 * @return true si saisie valide, sinon false
 * @note source : https://www.delftstack.com/fr/howto/cpp/fibonacci-sequence-in-cpp/
 */
bool inputMove(unsigned N, CPosition & pos, char & direction) {
    cout << "\nMenu : Z, S, Q, D (Direction)\n";
    cout << "Entrez Ligne (ord) et Colonne (abs) du bonbon a deplacer: ";
    if (!(cin >> pos.ord >> pos.abs)) return false;

    cout << "Entrez la direction (Z/S/Q/D) : ";
    if (!(cin >> direction)) return false;

    if (pos.ord >= N || pos.abs >= N) return false;

    return true;
}

/**
 * @brief Initialisation grille de jeu avec bonbons aléatoires
 * @param[out] Grid Grille
 * @param[in] Size Taille de la grille (Size x Size)
 * @param[in] KNbCandies Nombre de types de bonbons différents
 * 
 * La fonction crée une grille carrée de taille Size (Size est un entier naturel)
 * 
 */
void InitGrid(CMat & Grid, unsigned Size, const unsigned KNbCandies)
{
    Grid.resize(Size, CVLine(Size));
    for (unsigned i = 0; i < Size; ++i)
    {
        for (unsigned j = 0; j < Size; ++j)
        {
            Grid[i][j] = 1 + rand() % KNbCandies;
        }
    }
}

/**
 * @brief Affiche la grille de jeu dans le terminal
 * @param[in] Grid Grille
 * @param[in] KNbCandies Nombre de types de bonbons différents
 * 
 * Chaque bonbon est affiché avec une largeur de 3 caractères.
 * Les valeurs impossibles (0 ou > KNbCandies) sont considérés comme des espaces.
 */
void  DisplayGrid (const CMat & Grid)
{
    for (const CVLine & uneLigne : Grid)
    {
        for (const unsigned & uneCel : uneLigne)
        {
            if (uneCel == KImpossible)
            {
                cout << " ";
            }
            else
            {
                cout << setw(3) << uneCel;
            }
        }
        cout << endl;
    }
}
/**
 * @brief Fonction qui permet de déplacer un bonbon dans la grille
 * @param[in,out] Grid Grille de jeu
 * @param[in] Pos Position bonbon à déplacer
 * @param[in] Direction Direction du déplacement
 * 
 * @details
 * Les touches ZQSD (touches du clavier AZERTY) :
 * - 'Z' ou 'z' : déplacement vers le haut
 * - 'S' ou 's' : déplacement vers le bas  
 * - 'Q' ou 'q' : déplacement vers la gauche
 * - 'D' ou 'd' : déplacement vers la droite
 */
void MakeAMove (CMat & Grid, const CPosition & Pos, const char & Direction)
{
    unsigned i = Pos.ord;
    unsigned j = Pos.abs;
    unsigned n = Grid.size();
    if (i >= n || j >= n) return;
    if ((Direction == 'Z' || Direction == 'z') && i > 0)
    {
        swap(Grid[i][j], Grid[i-1][j]);
    }
    else if ((Direction == 'S' || Direction == 's') && i < n-1)
    {
        swap(Grid[i][j], Grid[i+1][j]);
    }
    else if ((Direction == 'Q' || Direction == 'q') && j > 0)
    {
        swap(Grid[i][j], Grid[i][j-1]);
    }
    else if ((Direction == 'D' || Direction == 'd') && j < n-1)
    {
        swap(Grid[i][j], Grid[i][j+1]);
    }
}

/**
 * @brief Fonction qui permet de récuperer la direction inverse d'une direction
 * @param[in] Direction Direction 
 * 
 * @details
 * Inverse : Touche 'Z' == 'S'
 * Inverse : Touche 'S' == 'Z'
 * Inverse : Touche 'Q' == 'D'
 * Inverse : Touche 'D' == 'Q'
 * @return Direction inverse de la direction données (Direction)
 */
char getInverseDirection(char Direction)
{
    char dir = toupper(Direction);
    if (dir == 'Z') return 'S';
    if (dir == 'S') return 'Z';
    if (dir == 'Q') return 'D';
    if (dir == 'D') return 'Q';
    return Direction;
}

/**
 * @brief Fonction booléene qui détecte si il y a 3 bonbons alignés sur une collone (verticale donc)
 * @param[in] grid Grille de jeu
 * @param[out] pos Position premier bonbon
 * @param[out] howMany Nombre de bonbon consécutifs
 * 
 * 
 * @return true si 3 bonbons alignés, sinon false
 */
bool atLeastThreeInAColumn (const CMat & grid, CPosition & pos, unsigned & howMany) {
    const unsigned N = grid.size();
    for (unsigned col = 0; col < N; ++col)
    {
        unsigned count = 1;
        unsigned start_row = 0;
        for (unsigned row = 0; row < N - 1; ++row)
        {
            if (grid[row][col] == KImpossible)
            {
                count = 1;
                start_row = row + 1;
                continue;
            }

            if (grid[row][col] == grid[row + 1][col]) {
                count++;
            }
            else
            {
                if (count >= 3)
                {
                    pos.ord = start_row;
                    pos.abs = col;
                    howMany = count;
                    return true;
                }
                count = 1;
                start_row = row + 1;
            }
        }
        if (count >= 3)
        {
            pos.ord = start_row;
            pos.abs = col;
            howMany = count;
            return true;
        }
    }
    return false;
}

/**
 * @brief Fonction booléene qui détecte si il y a 3 bonbons alignés sur une ligne (horizontale donc)
 * @param[in] grid Grille de jeu
 * @param[out] pos Position premier bonbon
 * @param[out] howMany Nombre de bonbon consécutifs
 * 
 * 
 * @return true si 3 bonbons alignés, sinon false
 */
bool atLeastThreeInARow (const CMat & grid, CPosition & pos, unsigned & howMany) {
    const unsigned N = grid.size();
    for (unsigned row = 0; row < N; ++row)
    {
        unsigned count = 1;
        unsigned start_col = 0;
        for (unsigned col = 0; col < N - 1; ++col)
        {
            if (grid[row][col] == KImpossible)
            {
                count = 1;
                start_col = col + 1;
                continue;
            }
            if (grid[row][col] == grid[row][col + 1])
            {
                count++;
            }
            else
            {
                if (count >= 3)
                {
                    pos.ord = row;
                    pos.abs = start_col;
                    howMany = count;
                    return true;
                }
                count = 1;
                start_col = col + 1;
            }
        }
        if (count >= 3)
        {
            pos.ord = row;
            pos.abs = start_col;
            howMany = count;
            return true;
        }
    }
    return false;
}

/**
 * @brief Fonction qui supprime les bonbons consécutifs en collone (verticale) et deplacer les bonbons pour pas avoir d'espace vide
 * 
 * @param[in] grid Grille de jeu
 * @param[out] pos Position premier bonbon
 * @param[out] howMany Nombre de bonbon consécutifs
 */
void removalInColumn (CMat & grid, const CPosition & pos, unsigned howMany)
{
    const unsigned N = grid.size();
    if (pos.ord >= N || pos.abs >= N)
    {
        return;
    }
    const unsigned col = pos.abs;
    const unsigned start_row = pos.ord;
    const unsigned end_row = start_row + howMany;
    for (unsigned i = end_row; i < N; ++i)
    {
        unsigned target_row = i - howMany;
        grid[target_row][col] = grid[i][col];
    }
    for (unsigned i = N - howMany; i < N; ++i)
    {
        grid[i][col] = KImpossible;
    }
}

/**
 * @brief Fonction qui supprime les bonbons consécutifs en lignes (horizontale) et deplacer les bonbons pour pas avoir d'espace vide
 * 
 * @param[in] grid Grille de jeu
 * @param[out] pos Position premier bonbon
 * @param[out] howMany Nombre de bonbon consécutifs
 */
void removalInRow (CMat & grid, const CPosition & pos, unsigned howMany)
{
    const unsigned N = grid.size();
    if (pos.abs >= N || pos.ord >= N)
    {
        return;
    }
    const unsigned row = pos.ord;
    const unsigned start_col = pos.abs;
    const unsigned end_col = start_col + howMany;
    for (unsigned i = end_col; i < N; ++i)
    {
        unsigned target_col = i - howMany;
        grid[row][target_col] = grid[row][i];
    }
    for (unsigned i = N - howMany; i < N; ++i)
    {
        grid[row][i] = KImpossible;
    }
}

int main()
{
    // Initialisation
    srand(time(0));
    cout << "\033[30m\033[47m";
    clearScreen();

    unsigned KNbCandies;
    cout << "Entrer le nombre de bonbons différents : ";
    cin >> KNbCandies;

    unsigned Size;
    cout << "Entrer la taille de votre tableau : ";
    cin >> Size;

    CMat Grid;
    InitGrid(Grid, Size, KNbCandies);

    // Variables de jeu
    const int MAX_COUPS (20);
    int coups_restants (MAX_COUPS);
    unsigned long long score (0);

    CPosition pos_saisie;
    char direction_saisie;
    CPosition pos_match;
    unsigned howMany_match;

    // Boucle de jeu (Tant qu'on n'a pas atteint le nombre maximal de coups)
    while (coups_restants > 0)
    {
        clearScreen();

        // Afficher l'état
        cout << "--- COUPS RESTANTS : " << coups_restants << " | SCORE : " << score << " ---\n";

        DisplayGrid(Grid);

        // Menu et Saisie (Rafael : je vais l'implmenter)
        // La fonction inputMove affiche le menu et lit les entrées
        if (!inputMove(Size, pos_saisie, direction_saisie))
        {
            // Si la saisie échoue (ex: mauvaise direction, coordonnées hors limites)
            cout << "Saisie invalide. Réessayez.\n";
            continue;
        }

        // 1. Faire un coup
        MakeAMove(Grid, pos_saisie, direction_saisie);

        bool match_trouve = false;
//unsigned combo = 1;

        // 2. Détection et Suppression
        do
        {
            match_trouve = false;

            // a) Test en Colonne
            if (atLeastThreeInAColumn(Grid, pos_match, howMany_match))
            {
//unsigned points = calculateScore(howMany_match) * combo;
//score += points;
                removalInColumn(Grid, pos_match, howMany_match);
                score += calculateScore(howMany_match); 

                match_trouve = true;
//combo++
            }

            // b) Test en Ligne
            if (atLeastThreeInARow(Grid, pos_match, howMany_match))
            {
                removalInRow(Grid, pos_match, howMany_match);
                score += calculateScore(howMany_match);
                match_trouve = true;
            }

            // Si un match a été fait, on l'affiche et on continue la boucle pour voir si les nouvelles positions (KImpossible) ont créé de nouveaux matches
            if (match_trouve)
            {
// cout << "\n COMBO x" << combo - 1 << " !";
//cout << "  Score : " << score << "\n";
                cout << "\nMatch trouve ! Score mis a jour : " << score << "\n";
                DisplayGrid(Grid);
                // Pause pour que l'utilisateur voie la suppression
                cout << "Jeu mis en pause. Appuyer sur entrée pour continuer.";
                cin.get();
            }
        } while (match_trouve); // Tant qu'il y a des réactions en chaîne

        // 3. Mise à jour du nombre de coups
        coups_restants--;
        // Gestion des cas où le coup n'a produit aucun match
        if (!match_trouve)
        {
            // a) Déterminer la direction inverse
            char direction_inverse = getInverseDirection(direction_saisie);
            // b) Annuler le déplacement (Remettre les éléments en place)
            MakeAMove(Grid, pos_saisie, direction_inverse);
            cout << "ÉCHEC : Pas de Match créé. Annulation du déplacement.\n";
        }
    }

    // Affichage du score final
    clearScreen();
    cout << "\n##########################################\n";
    cout << "# FIN DE PARTIE ! Le nombre de coups est atteint.\n";
    cout << "# Votre Score Final est : " << score << "\n";
    cout << "##########################################\n";

    return 0;
}
