//j'ai fait un version sur vscode parce que qt c'est chiant de fou a installé on fera l'update a l'iut

#include <iostream>
#include <vector>
#include <random>

using namespace std;

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

void KMaxTime() { //le void c'est si plus tard on dois développé la fonction KMax
    int KmaxTime = 10 //on modifira la valeur plus tard en fonction du niveau
}

int KNbCandies = 5 //nombre [0,5] dans la matrice
typedef vector <unsigned> line; // un type représentant une ligne de la grille
typedef vector <line> mat; // un type représentant la grille
typedef maPosition { 
unsigned abs;
unsigned ord
}; // une position dans la girlle

void initGrid (mat & grid, const size_t & size) {
    grid.size(size, vector<int>(size)); //dimantionnement de la grille

}

void  displayGrid (const mat & grid){
    clearScreen ();

    size_t size = grid.size();                      // suprime les valeur si elle dépasse KNbCandies
    for (size_t i = 0; i<size; ++i) {              
        for (size_t i = 0, i<grid[i].size();++j){ 
            int val = grid[i][j];
            if (val < 1 && val > KNbCandies){
                cout << "[]" <<endl;
            }
            else {
                cout << val << endl;
            }
        }
    }
}

int main()
{
    initGrid();
    displayGrid();
    return 0;
}