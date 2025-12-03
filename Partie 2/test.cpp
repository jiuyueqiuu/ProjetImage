#include <stdexcept>
/** @file
 * Lecture et ecriture d'images en niveau de gris au format PGM
 **/
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/// BEGIN ImageGris

/** Structure de donnees pour representer une image en teintes de gris **/
typedef vector<vector<double>> ImageGris;
/// END ImageGris

/// BEGIN lirePGM

/** Construire une image en teintes de gris depuis un fichier PGM
 * @param source le nom d'un fichier PGM
 * @return une image en teintes de gris
 **/
ImageGris lirePGM(string source) {
    ifstream f(source);
    if (!f) throw runtime_error("Impossible d'ouvrir le fichier : " + source);

    string magic;
    f >> magic;
    if (magic != "P2") throw runtime_error("Format PGM incorrect (P2 attendu)");

    int hauteur, largeur, maxval;
    f >> largeur >> hauteur >> maxval;

    ImageGris img(hauteur, vector<double>(largeur));

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            double pixel;
            f >> pixel;
            if (!f) throw runtime_error("Fichier PGM incomplet ou incorrect");
            img[i][j] = pixel;
        }
    }

    return img;

}


/// BEGIN ecrirePGM

/** Ecrit une image en teintes de gris dans un fichier PGM
 * @param img une image en teintes de gris
 * @param cible le nom d'un fichier PGM
 **/
void ecrirePGM(ImageGris img, string cible) {
    ofstream out(cible);
    if (!out) throw runtime_error("Impossible de créer le fichier : " + cible);

    int hauteur = img.size();
    int largeur = img[0].size();

    out << "P2\n";
    out << largeur << " " << hauteur << "\n";  // PGM: width height
    out << 255 << "\n";

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            int val = round(img[i][j]);
            val = max(0, min(255, val));
            out << val << " ";
        }
        out << "\n";
    }
}



/** Construit une image cliché d'une image en niveau de gris
 * @param img une image en niveau de gris
 * @return l'image cliché qui inverse les niveaux de gris
 **/
ImageGris inversePGM(ImageGris img) {

    int hauteur = img.size();
    int largeur = img[0].size();

    ImageGris imgInv(hauteur, vector<double>(largeur));

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            imgInv[i][j] = 255 - img[i][j];
        }
    }

    return imgInv;
}

void lirePGMTest(){
    cout << "Vérifier que les images obtenues dans 'pgm/' sont semblables à celles fournies dans 'pgm/correction/'" << endl;
    ecrirePGM(lirePGM("images/brain.pgm"),  "pgm/brain.pgm");
    ecrirePGM(lirePGM("images/illusion.pgm"),  "pgm/illusion.pgm");
}

void inversePGMTest() {
    ImageGris imageOriginale;
    cout << "Vérifier que les images obtenues dans 'pgm/' sont semblables à celles fournies dans 'pgm/correction/'" << endl;
    imageOriginale = lirePGM("images/brain.pgm");
    ecrirePGM(inversePGM(imageOriginale),  "pgm/brain-inverse.pgm");
    imageOriginale = lirePGM("images/illusion.pgm");
    ecrirePGM(inversePGM(imageOriginale),  "pgm/illusion-inverse.pgm");
}

int main() {
    lirePGMTest();
    inversePGMTest();
    return 0;
}

