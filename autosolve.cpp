#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>

struct Taquin {
    std::vector<std::vector<int>> grille;
    int largeur, hauteur;
    int x_vide, y_vide; 
    int g; 
    int h; 
    std::vector<std::string> chemin; 

    Taquin(std::vector<std::vector<int>> _grille, int _g, std::vector<std::string> _chemin)
        : grille(_grille), g(_g), chemin(_chemin) {
        
        hauteur = grille.size();
        largeur = grille[0].size();

        for (int j = 0; j < hauteur; j++) {
            for (int i = 0; i < largeur; i++) {
                if (grille[j][i] == 0) {
                    x_vide = i;
                    y_vide = j;
                }
            }
        }
       
        h = calcul();
    }

    bool operator>(const Taquin& autre) const {
        return (g + h) > (autre.g + autre.h);
    }

    int calcul() const {
        int somme = 0;
        for (int j = 0; j < hauteur; j++) {
            for (int i = 0; i < largeur; i++) {
                if (grille[j][i] != 0) {
                    int valeur = grille[j][i] - 1;
                    int x_cible = valeur % largeur;
                    int y_cible = valeur / largeur;
                    somme += std::abs(i - x_cible) + std::abs(j - y_cible);
                }
            }
        }
        return somme;
    }

    std::vector<Taquin> voisins() const {
        std::vector<Taquin> etats;
        std::vector<std::tuple<int, int, std::string>> mouvements = {{-1, 0, "Gauche"}, {1, 0, "Droite"}, {0, -1, "Haut"}, {0, 1, "Bas"}};
        
        for (const auto& mouvement : mouvements) {
            int dx, dy;
            std::string nom;
            std::tie(dx, dy, nom) = mouvement;
            int nx = x_vide + dx;
            int ny = y_vide + dy;
            if (nx >= 0 && nx < largeur && ny >= 0 && ny < hauteur) {
                std::vector<std::vector<int>> nouvelle_grille = grille;
                std::swap(nouvelle_grille[y_vide][x_vide], nouvelle_grille[ny][nx]);
                std::vector<std::string> nouveau_chemin = chemin;
                nouveau_chemin.push_back(nom);
                etats.emplace_back(nouvelle_grille, g + 1, nouveau_chemin);
                etats.back().x_vide = nx;
                etats.back().y_vide = ny;
            }
        }
        return etats;
    }

    
    bool resolu() const {
        int compteur = 1;
        for (int j = 0; j < hauteur; j++) {
            for (int i = 0; i < largeur; i++) {
                if (j == hauteur - 1 && i == largeur - 1) return grille[j][i] == 0;
                if (grille[j][i] != compteur++) return false;
            }
        }
        return true;
    }

    std::string cle() const {
        std::string key;
        for (const auto& ligne : grille)
            for (int val : ligne)
                key += std::to_string(val) + ",";
        return key;
    }
};

std::vector<std::string> resoudre_taquin(const std::vector<std::vector<int>>& grille_initiale) {
    std::priority_queue<Taquin, std::vector<Taquin>, std::greater<Taquin>> queue;
    std::set<std::string> visités;

    queue.push(Taquin(grille_initiale, 0, {}));

    while (!queue.empty()) {
        Taquin courant = queue.top();
        queue.pop();

        if (courant.resolu()) return courant.chemin;
        
        std::string cle_etat = courant.cle();
        if (visités.count(cle_etat)) continue;
        visités.insert(cle_etat);

        for (const auto& voisin : courant.voisins())
            queue.push(voisin);
    }

    return {}; 
}

int main() {
    std::vector<std::vector<int>> grille = {
        {4, 0, 5},
        {7, 8, 6},
        {3, 2, 1}
    };

    std::cout << "Résolution du Taquin...\n";
    std::vector<std::string> solution = resoudre_taquin(grille);

    if (!solution.empty()) {
        std::cout << "Solution trouvée en " << solution.size() << " coups !\n";
        for (const auto& mvt : solution) std::cout << mvt << " -> ";
        std::cout << "Terminé!\n";
    } else {
        std::cout << "Aucune solution trouvée.\n";
    }

    return 0;
}