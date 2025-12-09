#include <stdexcept>
/** @file
 * Filtres de seuillage
 **/

#include <cmath>
#include "image.hpp"
#include "sobel.hpp"
#include "seuillage.hpp"

ImageGris renormalise(ImageGris img) {
    int hauteur = img.size();
    int largeur = img[0].size();
    double max_val = 0;

    ImageGris res(hauteur, vector<double>(largeur,0));

    for(int i = 0; i < hauteur; i++){
        for(int j = 0; j < largeur; j++){
            if(img[i][j] > max_val) max_val = img[i][j];
        }
    }
    if(max_val == 0) return img;

    for(int i = 0; i < hauteur; i++){
        for(int j = 0; j < largeur; j++){
            res[i][j] = (img[i][j] / max_val) * 255.0;
        }
    }
    return res;
}

ImageGris seuillage(ImageGris img, int seuil) {
    int hauteur = img.size();
    int largeur = img[0].size();
    ImageGris res(hauteur, vector<double>(largeur,0));

    for(int i = 0; i < hauteur; i++){
        for(int j = 0; j < largeur; j++){
            res[i][j] = (img[i][j] < seuil) ? 255 : 0;
        }
    }
    return res;
}

ImageGris doubleSeuillage(ImageGris imgIntensite, ImageGris imgContour, int seuil) {
    int hauteur = imgIntensite.size();
    int largeur = imgIntensite[0].size();
    ImageGris res(hauteur, vector<double>(largeur,255));

    for(int i = 1; i < hauteur - 1; i++){
        for(int j = 1; j < largeur - 1; j++){
            if(imgIntensite[i][j] > seuil){
                bool voisinSelectionne = false;
                for(int di = -1; di <= 1 && !voisinSelectionne; di++){
                    for(int dj = -1; dj <= 1 && !voisinSelectionne; dj++){
                        if(imgContour[i + di][j + dj] == 0){
                            voisinSelectionne = true;
                        }
                    }
                }
                if(voisinSelectionne){
                    res[i][j] = 0;
                }
            }
        }
    }
    return res;                    
}

ImageGris doubleSeuillage(ImageGris imgIntensite, int seuilFort, int seuilFaible, int nbAmeliorations) {
   ImageGris imgContour = seuillage(imgIntensite, seuilFort);

    for(int k = 0; k < nbAmeliorations; k++) {
        imgContour = doubleSeuillage(imgIntensite, imgContour, seuilFaible);
    }
    return imgContour;
}