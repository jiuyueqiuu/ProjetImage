#include <stdexcept>
/** @file
 * Lecture et écriture d'images au format PPM
 **/


#include <iostream>
#include <fstream>
#include "image.hpp"
#include "ppm.hpp"

Image lirePPM(string source) {
    ifstream ppm;
    ppm.open(source);
    if(!ppm){
        throw runtime_error("Fichier non trouve"+source);
    }
    string P_ligne;
    ppm >> P_ligne;
    if (P_ligne != "P3"){
       throw runtime_error("Format PGM non supporté");
    }
    
    int hauteur, largeur;
    int val_max;
    ppm >> largeur >> hauteur;
    ppm >> val_max;
    Image res(hauteur, vector<Couleur>(largeur));
    
    for(int i = 0; i < hauteur; i++){
        for(int j = 0; j < largeur; j++){
            Couleur c;
            ppm >> c.r >> c.g >> c.b;
            res[i][j] = c;
        }
    }
    ppm.close();
    return res;
}

void ecrirePPM(Image img, string cible) {
    ofstream ppm;
    ppm.open(cible);
    if (!ppm)
        throw runtime_error("Impossible d'ouvrir le fichier: "+cible);
    
    int hauteur = img.size();
    int largeur = img[0].size();
    int val_max = 255;
    
    // Ecriture de l'en-tête du fichier
    ppm << "P3\n";
    ppm << largeur << " " << hauteur << "\n";
    ppm << val_max << "\n";
    
     for(int i = 0; i < hauteur; i++){
        for(int j = 0; j < largeur; j++){
            Couleur c = img[i][j];
              ppm << (int)c.r << " " << (int)c.g << " " << (int)c.b << "\n";
           }
        }
    ppm.close();
}

