#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;
typedef vector <unsigned> CVLine; // un type représentant une ligne de la grille
typedef vector <CVLine> CMat; // un type représentant la grille
struct CPosition {
    unsigned abs;
    unsigned ord;
}; // une position dans la girlle
const unsigned KReset   (0);
const unsigned KNoir    (30);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);

void clearScreen () {
    cout << "\033[H\033[2J";
}

void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

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

void  DisplayGrid (const CMat & Grid, const unsigned & KNbCandies)
{
    for (const CVLine & uneLigne : Grid)
    {
        for (const unsigned & uneCel : uneLigne)
        {
            if (!(0 < uneCel && uneCel < KNbCandies+1))
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

void MakeAMove (CMat & Grid, const CPosition & Pos, const char & Direction)
{
    unsigned i = Pos.abs;
    unsigned j = Pos.ord;
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

bool atLeastThreeInAColumn (const CMat & grid, CPosition & pos, unsigned & howMany)
{
    unsigned k = grid.size();
    if (k == 0) return false;
    unsigned l = grid[0].size();
    for (unsigned i (0); i < l; ++i)
    {
        unsigned c (1);
        for (unsigned j (1); j < k; ++j)
        {
            if (grid[j][i] == grid[j - 1][i])
            {
                ++c;
            }
            else
            {
                if (c >= 3)
                {
                    pos.abs  = j - c;
                    pos.ord = i;
                    howMany    = c;
                    return true;
                }
                c = 1;
            }
        }
        if (c >= 3)
        {
            pos.abs  = k - c;
            pos.ord = i;
            howMany    = c;
            return true;
        }
    }
    return false;
}

bool atLeastThreeInARow (const CMat & grid, CPosition & pos, unsigned & howMany)
{
    unsigned k = grid.size();
    if (k == 0) return false;
    unsigned l = grid[0].size();
    for (unsigned i (0); i < k; ++i)
    {
        unsigned c (1);
        for (unsigned j (1); j < l; ++j)
        {
            if (grid[i][j] == grid[i][j - 1])
            {
                ++c;
            }
            else
            {
                if (c >= 3)
                {
                    pos.abs  = i;
                    pos.ord = j - c;
                    howMany    = c;
                    return true;
                }
                c = 1;
            }
        }
        if (c >= 3)
        {
            pos.abs  = i;
            pos.ord = l - c;
            howMany    = c;
            return true;
        }
    }

    return false;
}

// Définitions supposées (à adapter à votre environnement) :
// const int KImpossible = 0; // Ou une autre valeur "impossible" appropriée
// typedef std::vector<std::vector<int>> mat; // Ou votre type de grille
// struct maPosition {
//     unsigned ord; // Ligne (row)
//     unsigned abs; // Colonne (column)
// };

void removalInColumn (mat & grid, const maPosition & pos, unsigned howMany) {
    // 1. Déterminer les dimensions de la grille
    if (grid.empty()) {
        return;
    }
    unsigned numRows = grid.size();
    unsigned numCols = grid[0].size();

    // 2. Vérifier si la position de départ et le nombre à supprimer sont valides
    // On vérifie seulement la colonne pos.abs et la ligne pos.ord
    if (pos.abs >= numCols || pos.ord >= numRows || howMany == 0) {
        return;
    }

    // Le nombre réel d'éléments à supprimer ne peut pas dépasser
    // le nombre d'éléments restants dans la colonne à partir de pos.ord.
    unsigned actualRemoval = std::min((unsigned)(numRows - pos.ord), howMany);

    // --- Suppression et remontée des éléments ---

    // La colonne concernée est pos.abs (désormais 'col')
    unsigned col = pos.abs;
    // La ligne de départ est pos.ord (désormais 'startRow')
    unsigned startRow = pos.ord;

    // L'idée est de décaler les éléments de la colonne :
    // Les éléments situés entre startRow + actualRemoval et numRows - 1
    // sont déplacés vers les positions startRow à (numRows - 1) - actualRemoval.

    // 3. Remonter les éléments au-dessus du bloc supprimé
    for (unsigned r = startRow; r < numRows - actualRemoval; ++r) {
        // La nouvelle valeur à la ligne 'r' est celle qui se trouvait
        // 'actualRemoval' lignes plus bas.
        grid[r][col] = grid[r + actualRemoval][col];
    }

    //

    // 4. Compléter les 'actualRemoval' dernières lignes avec KImpossible
    // Ces lignes sont les dernières de la colonne, de (numRows - actualRemoval) à (numRows - 1).
    unsigned fillStartRow = numRows - actualRemoval;

    for (unsigned r = fillStartRow; r < numRows; ++r) {
        grid[r][col] = KImpossible;
    }
}

int main()
{
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
    DisplayGrid(Grid, KNbCandies);
    CPosition Pos;
    char Direction;
    cout << "Entrer la ligne de l'élément qui va changer : ";
    cin >> Pos.abs;
    cout << "Entrer la colonne de l'élément qui va changer : ";
    cin >> Pos.ord;
    cout << "Entrer la direction que va prendre l'élément : ";
    cin >> Direction;
    MakeAMove(Grid, Pos, Direction);
    DisplayGrid(Grid, KNbCandies);
    couleur (KReset);
    cout << "Retour à la normale" << endl;
    clearScreen();
}
