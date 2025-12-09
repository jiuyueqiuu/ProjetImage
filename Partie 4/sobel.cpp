#include <stdexcept>
/** @file
 * Filtres de Sobel
 **/
#include <cmath>
#include "sobel.hpp"

ImageGris intensiteH(ImageGris img) {
    int hauteur = img.size();
    int largeur = img[0].size();
    
    ImageGris img_hor(hauteur, vector<double>(largeur,0));
    
      for(int i = 1; i < hauteur - 1; i++){
          for(int j = 1; j < largeur - 1; j++){
            double h = img[i-1][j-1] + 2*img[i][j-1] + img[i+1][j-1] - img[i-1][j+1] - 2*img[i][j+1] - img[i+1][j+1];
              img_hor[i][j] = h;
          }
      }
    return img_hor;
}

ImageGris intensiteV(ImageGris img) {
    int hauteur = img.size();
    int largeur = img[0].size();
    
    ImageGris img_ver(hauteur, vector<double>(largeur,0));
    
      for(int i = 1; i < hauteur - 1; i++){
          for(int j = 1; j < largeur - 1; j++){
              double v = img[i-1][j-1] + 2*img[i-1][j] + img[i-1][j+1] - img[i+1][j-1] - 2*img[i+1][j] - img[i+1][j+1];
              img_ver[i][j] = v;
          }
      }
    return img_ver;
}

ImageGris intensite(ImageGris img) {
    int hauteur = img.size();
    int largeur = img[0].size();

    ImageGris res(hauteur, vector<double>(largeur,0));

    for(int i = 1; i < hauteur - 1; i++){
          for(int j = 1; j < largeur - 1; j++){
              double h = img[i-1][j-1] + 2*img[i][j-1] + img[i+1][j-1] - img[i-1][j+1] - 2*img[i][j+1] - img[i+1][j+1];
              double v = img[i-1][j-1] + 2*img[i-1][j] + img[i-1][j+1] - img[i+1][j-1] - 2*img[i+1][j] - img[i+1][j+1];

              res[i][j] = sqrt(h*h + v*v);
          }
    }
    return res;
}
