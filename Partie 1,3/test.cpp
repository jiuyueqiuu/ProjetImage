#include <stdexcept>
/** @file
 * Lecture et ecriture d'images binaires au format PBM
 **/
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/// BEGIN ImageNB
/** Structure de données pour représenter une image binaire **/
typedef vector<vector<int>> ImageNB;
/// END ImageNB

/// BEGIN lirePBM

/** Construire une image binaire depuis un fichier PBM
 * @param source le nom d'un fichier PBM
 * @return une image binaire (0/1)
 **/
ImageNB lirePBM(string source) {

    ifstream f(source);
    if (!f) {
        throw runtime_error("Impossible d'ouvrir le fichier : " + source);
    }

    string magic;
    f >> magic;
    if (magic != "P1") {
        throw runtime_error("Format PBM incorrect (P1 attendu)");
    }

    int largeur, hauteur;
    f >> largeur >> hauteur;

    ImageNB img(hauteur, vector<int>(largeur));

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            int pixel;
            f >> pixel;
            if (!f) throw runtime_error("Fichier PBM incomplet ou incorrect");
            img[i][j] = pixel;
        }
    }

    return img;
    f.close();

}


/// BEGIN ecrirePBM

/** Ecrit une image binaire dans un fichier PBM
 * @param img une image binaire (0/1)
 * @param cible le nom d'un fichier PBM
 **/
void ecrirePBM(ImageNB img, string cible) {

    ofstream out(cible);
    if (!out) {
        throw runtime_error("Impossible de créer le fichier : " + cible);
    }

    int hauteur = img.size();
    int largeur = img[0].size();

    out << "P1\n";
    out << largeur << " " << hauteur << "\n";

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            out << img[i][j];
            out << " ";
        }

        out << "\n";
    }

    out.close();

}



/// BEGIN affichePBM

/** Affiche une image binaire PBM à l'écran avec ' ' pour 0 et '@' pour 1
 * @param img une image binaire (0/1)
 **/
void affichePBM(ImageNB img) {

    int hauteur = img.size();
    int largeur = img[0].size();

    cout << "P1\n" << endl;
    cout << hauteur << " " << largeur << "\n" << endl;
    
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            cout << ((img[i][j] == 1) ? '@' : ' ');
        }
        cout << endl;
    }
}

/// BEGIN inversePBM

/** Echange le noir et le blanc dans une image PBM
 * @param img une image binaire (0/1)
 * @return l'image où le blanc et le noir ont été inversés
 **/
ImageNB inversePBM(ImageNB img) {

    int hauteur = img.size();
    int largeur = img[0].size();

    cout << "P1\n" << endl;
    cout << hauteur << " " << largeur << "\n" << endl;

    ImageNB imgInverse(hauteur, vector<int>(largeur));

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            imgInverse[i][j] = (img[i][j] == 1) ? 0 : 1;
        }
    }
    
    return imgInverse;

}


int main() {
    return 0;
}