#include <stdexcept>
/** @file
 * Lecture et ecriture d'images en niveau de gris au format PGM
 **/

#include <iostream>
#include <fstream>
#include "image.hpp"
#include "pgm.hpp"
#include <cmath>



ImageGris lirePGM(string source) {
    ifstream pgm;
    pgm.open(source);
    if(!pgm){
        throw runtime_error("Fichier non trouve: "+source);
    }
    string P_ligne;
    pgm >> P_ligne;
    if (P_ligne != "P2"){
       throw runtime_error("Format PGM non supporté");
    }
    int hauteur, largeur;
    int val_max;
    pgm >> largeur >> hauteur;
    pgm >> val_max;
    ImageGris img_gr(hauteur, vector<double>(largeur));
    
    for(int i = 0; i < hauteur; i++){
        for(int j = 0; j < largeur; j++){
            double val;
            pgm >> val;
            img_gr[i][j] = val;
          }
      }
    pgm.close();
    return img_gr;
    
}

void ecrirePGM(ImageGris img, string cible) {
    ofstream pgm;
    pgm.open(cible);
    if (!pgm)
        throw runtime_error("Impossible d'ouvrir le fichier: "+cible);
    int hauteur = img.size();
    int largeur = img[0].size();
    int val_max = 255;
    // Ecriture de l'en-tête du fichier
    pgm << "P2\n";
    pgm << largeur << " " << hauteur << "\n";
    pgm << val_max << "\n";
     for(int i = 0; i < hauteur; i++){
        for(int j = 0; j < largeur; j++){
            int v = ((int)round(img[i][j]));
             if(v <= val_max){
              pgm << v << "\n";
           }
        }
     }
    pgm.close();
}

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

ImageGris inversePGM(ImageGris img) {
    int hauteur = img.size();
    int largeur = img[0].size();
    int val_max = 255;
    ImageGris img_inv_gr(hauteur, vector<double>(largeur));

    for(int i = 0; i < hauteur; i++){
        for(int j = 0; j < largeur; j++){
            img_inv_gr[i][j] = val_max - (img[i][j]);
        }
    }
    return img_inv_gr;
}