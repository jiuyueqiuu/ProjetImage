#include <stdexcept>
/** @file
 * Filtres de conversion couleur <-> teintes de gris
 **/
#include "gris-couleurs.hpp"

ImageGris CouleurAuGris( Image img ) {
    int hauteur = img.size();
    int largeur = img[0].size();
    ImageGris res(hauteur, vector<double>(largeur));

    for(int i = 0; i < hauteur; i++){
        for(int j = 0; j < largeur; j++){
            Couleur c = img[i][j];
            double moyenne = ((c.r * 0.2126) + (c.g * 0.7152) + (c.b * 0.0722));
            res[i][j] = moyenne;
        }
    }
    return res;
}

Image GrisACouleur( ImageGris img ) {
    int hauteur = img.size();
    int largeur = img[0].size();
    Image res(hauteur, vector<Couleur>(largeur));

    for(int i = 0; i < hauteur; i++){
        for(int j = 0; j < largeur; j++){
            double v = img[i][j];
            res[i][j].r = v;
            res[i][j].g = v; 
            res[i][j].b = v;
        }
    }
    return res;
}