#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <thread>
#include <sys/ioctl.h>
#include <termios.h>
#include <queue>
#include <set>

std::vector<std::vector<int>> creegrille(int largeur, int hauteur) {
    int total = largeur * hauteur;
    std::vector<int> numbers(total);
    
    for (int i = 0; i < total; ++i) {
        numbers[i] = i;
    }
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(numbers.begin(), numbers.end(), g);

    std::vector<std::vector<int>> grille(hauteur, std::vector<int>(largeur));
    for (int j = 0; j < hauteur; j++) {
                for (int i = 0; i < largeur; i++) {
                        grille[j][i] = numbers [j*largeur + i];
                    }
                }
    return grille;
}

std::pair<int,int> trouve_case_vide(const std::vector<std::vector<int>>& grille,const int largeur,const int hauteur){
    std::pair<int,int> result = {-1,-1};
    for (int j = 0; j < hauteur; j++) {
                for (int i = 0; i < largeur; i++) {
                        if (grille[j][i] == 0) {result={j,i};}
                    }
    }
    return result;
    }


class Taquin {
    protected:
        std::vector<std::vector<int>> grille_;
        std::pair<int,int> case_vide_;
        int largeur_, hauteur_;

    public:
        Taquin(int largeur,int hauteur): largeur_{largeur}, hauteur_{hauteur},
        grille_(creegrille(largeur,hauteur)),
        case_vide_{-1,-1} { case_vide_ = trouve_case_vide(grille_,largeur_,hauteur_);};

        void print() {
            for (int j = 0; j < hauteur_; j++) {
                for (int i = 0; i < largeur_; i++) {
                    if (grille_[j][i] == 0) {
                        std::cout << "   ";
                    } else  if (grille_[j][i]<10){
                        std::cout << "  " <<grille_[j][i];
                    } else {
                        std::cout <<" "<< grille_[j][i];
                    }
                }
            std::cout << std::endl;
            }
        }

        void printcasevide() {
            std::cout<< case_vide_.first << ", " << case_vide_.second << std::endl;
        }

    bool resolu() {
        int count = 1;
        for (int j = 0; j < hauteur_; j++) {
            for (int i = 0; i < largeur_; i++) {
                if (j == hauteur_ - 1 && i == largeur_ - 1) {
                    if (grille_[j][i] != 0) return false;
                } else {
                    if (grille_[j][i] != count++) return false;
                }
            }
        }
        return true;
    }

    bool desordre() {
    std::vector<int> flatten;
    int D = 0;
    
    for (int j = 0; j < hauteur_; j++) {
        for (int i = 0; i < largeur_; i++) {
            if (grille_[j][i] != 0) {
                flatten.push_back(grille_[j][i]);
            }
        }
    }
    
    int size = flatten.size();

    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (flatten[i] > flatten[j]) {
                D++;
            }
        }
    } 
    return (D%2==0);   
}

void bouge(char key) {
    int j = case_vide_.first;
    int i = case_vide_.second;

    if (key == 's') {
        if (j > 0) {
            std::swap(grille_[j][i], grille_[j - 1][i]);
            case_vide_.first--;
        }
    } 
    else if (key == 'z' || key =='w') { 
        if (j < hauteur_ - 1) {
            std::swap(grille_[j][i], grille_[j + 1][i]);
            case_vide_.first++;
        }
    } 
    else if (key == 'd') {
        if (i > 0) {
            std::swap(grille_[j][i], grille_[j][i - 1]);
            case_vide_.second--;
        }
    } 
    else if (key == 'q' || key =='a') { 
        if (i < largeur_ - 1) {
            std::swap(grille_[j][i], grille_[j][i + 1]);
            case_vide_.second++;
        }
    }
}
};

namespace internal
{

  static const int STDIN = 0;
  static bool initialized = false;


  int keyEvent()
  {
    if (!initialized)
    {
      termios term;
      tcgetattr(STDIN, &term);
      term.c_lflag &= ~ICANON;
      tcsetattr(STDIN, TCSANOW, &term);
      setbuf(stdin, NULL);
      initialized = true;
    }
    int bytesWaiting;
    // int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
  }

}

int main() {
    int n,m;
    std::cout << "Choisis le nombre de lignes du taquin (entre 2 et 10): " ;
    std::cin>>n ;
    std::cout << "Choisis le nombre de colonnes du taquin (entre 2 et 10): ";
    std::cin>>m;
    Taquin taquin(n,m);
    taquin.print();
    taquin.printcasevide();
    taquin.desordre();
    taquin.resolu();

    while(true) {
        char key; 
        std::system("clear");
        taquin.print();
        if (taquin.resolu()){
            std::cout <<"Félicitations !!!!!!!" << std::endl;
            std::cout <<"Vous avez gagné" <<std::endl;
            return EXIT_SUCCESS;}
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (internal::keyEvent()) {
                std::cin >> key;
                taquin.bouge(key);
            }
        
    }
    return EXIT_SUCCESS;
}   