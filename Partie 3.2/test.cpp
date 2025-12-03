#include <stdexcept>
/** @file
 * Filtres de seuillage
 **/

#include <cmath>
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

/** Structure de donnee pour representer une image en teintes de gris **/
typedef vector<vector<double> > ImageGris;


/** Infrastructure minimale de test **/
#define CHECK(test) if (!(test)) cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl


/** Une image 4x4 en teintes de gris pour faire des tests **/
ImageGris imgGrisTest = {
    {0, 255, 54.213, 236.589},
    {18.411, 182.376, 200.787, 120},
    {139.583, 172.841, 94.0878, 88.4974},
    {158.278, 172.841, 89.0236, 80.0384}
};

/** Une image 4x4 en teintes de gris pour les tests de renormalisation **/
ImageGris imgGrisRenorm = {
    {0, 50, 0, 50},
    {0, 50, 0, 50},
    {0, 50, 0, 50},
    {0, 50, 0, 50},
};


/** filtre de Sobel
 * @param img une image en teintes de gris
 * @return une image en teintes de gris de l'intensite de img
 **/
ImageGris intensite(ImageGris img) {

    int hauteur = img.size();
    int largeur = img[0].size();

    ImageGris res(hauteur, vector<double>(largeur, 0));

    for (int i = 1; i < hauteur - 1; i++) {
        for (int j = 1; j < largeur - 1; j++) {

            double H =
                img[i-1][j-1] + 2*img[i][j-1] + img[i+1][j-1]
              - img[i-1][j+1] - 2*img[i][j+1] - img[i+1][j+1];

            double V =
                img[i-1][j-1] + 2*img[i-1][j] + img[i-1][j+1]
              - img[i+1][j-1] - 2*img[i+1][j] - img[i+1][j+1];

            res[i][j] = sqrt(H*H + V*V);
        }
    }

    return res;
}


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


/** Teste si deux images en teintes de gris sont égales modulo imprécision numérique
 * En cas de différence un message est affiché
 * @param a une image en teintes de gris
 * @param b une image en teintes de gris
 * @param precision un flottant positif: la précision souhaitée; typiquement 0.001
 * @return vrai si les images sont égales et faux sinon
 **/
bool ImageGrisEgal(ImageGris a, ImageGris b, float precision) {
    if (a.size() != b.size())  {
        cout << "Nombre de lignes différent" << endl;
        return false;
    }
    for (int i=0; i<a[0].size(); i++)
        if (a[0].size() != b[i].size()) {
            cout << "Nombre de colonnes différent" << endl;
            return false;
        }
    for (int i=0; i<a.size(); i++)
        for (int j=0; j<a[0].size(); j++)
            if (abs(a[i][j] - b[i][j]) > precision) {
                cout << "Valeur differentes en position " << i << "," << j
             << ": " << a[i][j] << " au lieu de " << b[i][j] << endl;
                return false;
            }
    return true;
}


/// BEGIN renormalise

/** Renormalize une image en teinte de gris, les ramenant dans l'intervalle [0,255]
 * @param img un image en teintes de gris
 * @return une image en teintes de gris
 **/
ImageGris renormalise(ImageGris img) {

    int hauteur = img.size();
    int largeur = img[0].size();

    double maxVal = 0;
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            if (img[i][j] > maxVal) maxVal = img[i][j];
        }
    }

    if (maxVal == 0) return img;

    ImageGris res(hauteur, vector<double>(largeur, 0));

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            res[i][j] = (img[i][j] / maxVal) * 255.0;
        }
    }

    return res;
}


void renormaliseTest() {
    CHECK( ImageGrisEgal(renormalise( imgGrisRenorm),
              ImageGris( {
                  {0, 255, 0, 255},
                  {0, 255, 0, 255},
                  {0, 255, 0, 255},
                  {0, 255, 0, 255},
              }),
              0.001) );

}
/// BEGIN seuillage

/** Filtre de seuillage
 * @param img
 * @param seuil un entier dans l'intervalle [0,255]
 * @return image en noir et blanc obtenue en remplacant la teinte de
 *  chaque pixel par
 *  - du blanc si teinte < seuil
 *  - du noir  si teinte > seuil
 **/
ImageGris seuillage(ImageGris img, int seuil) {

    int hauteur = img.size();
    int largeur = img[0].size();

    ImageGris res(hauteur, vector<double>(largeur, 0));

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {

            res[i][j] = (img[i][j] < seuil) ? 255 : 0;


        }
    }

    return res;
}


/// BEGIN doubleSeuillage

/** Filtre de double seuillage
 * @param imgIntensite image d'intensite
 * @param imgContour image codant un ensemble de pixels selectionnes
 * @param seuil un entier de l'intervalle [0,255]
 * @return une copie d'imgIntensite modifiee de sorte que:
 *  -si teinte > seuil et voisin d'un pixel de imgContour, alors pixel noir
 *  -sinon pixel blanc
 **/
ImageGris doubleSeuillage(ImageGris imgIntensite, ImageGris imgContour, int seuil) {

    int hauteur = imgIntensite.size();
    int largeur = imgIntensite[0].size();

    ImageGris res(hauteur, vector<double>(largeur, 255));

    for (int i = 1; i < hauteur - 1; i++) {
        for (int j = 1; j < largeur - 1; j++) {

            if (imgIntensite[i][j] > seuil) {
                bool voisinSelectionne = false;
                for (int di = -1; di <= 1 && !voisinSelectionne; di++) {
                    for (int dj = -1; dj <= 1 && !voisinSelectionne; dj++) {
                        if (imgContour[i + di][j + dj] == 0) {
                            voisinSelectionne = true;
                        }
                    }
                }

                if (voisinSelectionne) {
                    res[i][j] = 0;
                }
            }

        }
    }

    return res;
}


/// BEGIN doubleSeuillageIteratif

/** Filtre de double seuillage iteratif
 * @param imgIntensite image d'intensité
 * @param seuilFort un entier de l'intervalle [0,255]
 * @param seuilFaible un entier de l'intervalle [0,255]
 * @param nbAmeliorations un entier non negatif: le nombre d'itérations
 * @return le double seuillage de img
 **/
ImageGris doubleSeuillage(ImageGris imgIntensite, int seuilFort, int seuilFaible, int nbAmeliorations) {

    ImageGris imgContour = seuillage(imgIntensite, seuilFort);

    for (int k = 0; k < nbAmeliorations; k++) {
        imgContour = doubleSeuillage(imgIntensite, imgContour, seuilFaible);
    }

    return imgContour;
    
}


void seuillageTest() {
    CHECK( ImageGrisEgal(seuillage( imgGrisTest, 80),
              ImageGris( {
                  {255, 0, 255, 0},
                  {255, 0, 0, 0},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0}
              }),
              0.001) );
    CHECK( ImageGrisEgal(seuillage( intensite(imgGrisTest), 80),
              ImageGris( {
                  {255, 255, 255, 255},
                  {255, 0, 0, 255},
                  {255, 255, 0, 255},
                  {255, 255, 255, 255}
              }),
              0.001) );


    cout << "Vérifier que les images obtenues dans 'seuillage/' sont semblables à celles fournies dans 'seuillage/correction/'" << endl;
    ecrirePGM(seuillage(intensite(lirePGM("images/Billes.256.pgm")), 80), "seuillage/Billes.256.pgm");
    ecrirePGM(seuillage(intensite(lirePGM("images/Willis.512.pgm"  )), 110), "seuillage/Willis.512.pgm");
    cout << "\tProposer des seuils pour Embryos.512.pgm et House.256.pgm" << endl;

}

void doubleSeuillageTest(){
    CHECK( ImageGrisEgal(doubleSeuillage(intensite(imgGrisTest), 400, 80, 4 ),
        ImageGris( {
            {255, 255, 255, 255},
            {255, 0, 0, 255},
            {255, 255, 0, 255},
            {255, 255, 255, 255}
        }),
        0.001) );

    cout << "Vérifier que les images obtenues dans 'seuillage_double/' sont semblables à celles fournies dans 'seuillage_double/correction/'" << endl;
    ecrirePGM(doubleSeuillage(lirePGM("images/Willis.512.pgm"), seuillage(intensite(lirePGM("images/Willis.512.pgm")), 100), 80), "seuillage_double/Willis.512.pgm");
    ecrirePGM(doubleSeuillage(lirePGM("images/Billes.256.pgm"), seuillage(intensite(lirePGM("images/Billes.256.pgm")), 100), 80), "seuillage_double/Billes.256.pgm");
    cout << "\tProposer des seuils pour Embryos.512.pgm et House.256.pgm" << endl;
}

void doubleSeuillageIteratifTest() {
    cout << "Vérifier que les images obtenues dans 'seuillage_double/' sont semblables à celles fournies dans 'seuillage_double/correction/'" << endl;
    ecrirePGM(doubleSeuillage(intensite(lirePGM("images/Willis.512.pgm")), 500, 80, 100), "seuillage_double/iteratif_Willis.512.pgm");

    cout << "\tProposer des seuils pour Billes.256.pgm, Embryos.512.pgm et House.256.pgm" << endl;
    
}

int main() {
    try {
        renormaliseTest();
        seuillageTest();
        doubleSeuillageTest();
        doubleSeuillageIteratifTest();
    } catch (const std::exception &e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
