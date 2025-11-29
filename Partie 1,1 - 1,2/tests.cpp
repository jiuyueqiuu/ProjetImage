#include <stdexcept>
/** @file
 * Affichage d'une image PBM
 **/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/// BEGIN affichePBM

/** Affiche une image binaire PBM à l'écran avec ' ' pour 0 et '@' pour 1
 * @param source le nom d'un fichier PBM
 **/
void affichePBM(string source) {

    ifstream file(source);
    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir le fichier : " + source);
    }
    
    string magic;
    int hauteur, largeur;
    file >> magic >> hauteur >> largeur;
    
    if (magic != "P1") {
        throw runtime_error("Format PBM non supporté (attendu P1)");
    }
    
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            int pixel;
            file >> pixel;
            cout << ((pixel == 1) ? '@' : ' ');
        }
        cout << endl;
    }
    
    file.close();

}

/** Echange le noir et le blanc dans une image PBM
 * @param source le nom du fichier PBM à lire
 * @param cible le nom du nouveau fichier PMB créé
 **/
void inversePBM(string source, string cible) {

    ifstream in(source);
    if (!in) {
        throw runtime_error("Impossible d'ouvrir le fichier : " + source);
    }

    ofstream out(cible);
    if (!out) {
        throw runtime_error("Impossible de créer le fichier : " + cible);
    }

    string magic;
    in >> magic;
    if (magic != "P1") {
        throw runtime_error("Format PBM incorrect (P1 attendu)");
    }

    int hauteur, largeur;
    in >> hauteur >> largeur;

    // Écrire l'entête dans le nouveau fichier
    out << "P1\n";
    out << hauteur << " " << largeur << "\n";

    // Lire et inverser les pixels
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            int pixel;
            in >> pixel;

            if (!in) throw runtime_error("Données PBM insuffisantes");

            int inverse = (pixel == 0 ? 1 : 0);
            out << inverse;

            out << " "; // Espace entre les pixels
        }
        out << "\n";
    }

    in.close();
    out.close();

}


int main() {
    cout << "Affichage du smiley " << endl;
    affichePBM("../images/smiley.pbm");
    cout << "Fabrication du smiley inversé (vérifiez que le nouveau fichier a été créé dans le dossier pbm/smiley-inverse.pbm" << endl;
    inversePBM("../images/smiley.pbm","../images/smiley-inverse.pbm");
    cout << "Affichage du smiley inversé" << endl;
    affichePBM("../images/smiley-inverse.pbm");
    return 0;
}
