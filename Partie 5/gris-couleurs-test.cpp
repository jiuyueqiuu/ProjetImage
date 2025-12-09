/** @file
 * Filtres de conversion couleur <-> teintes de gris
 **/

#include <cmath>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
using namespace std;
#include "pgm.hpp"
#include "ppm.hpp"
#include "gris-couleurs.hpp"
#include "image-test.hpp"

/** Test du filtre de conversion couleur -> teintes de gris **/
void couleurAuGrisTest() {
    CHECK( ImageGrisEgal(CouleurAuGris(imgTest),
                          imgGrisTest,
                          0.001) );
    cout << "Vérifier les images obtenues dans 'gris-couleurs/'" << endl;
    ecrirePGM(CouleurAuGris(lirePPM("images/Baboon.512.ppm" )), "gris-couleurs/Baboon.512.pgm");
    ecrirePGM(CouleurAuGris(lirePPM("images/Billes.256.ppm" )), "gris-couleurs/Billes.256.pgm");
    ecrirePGM(CouleurAuGris(lirePPM("images/Embryos.512.ppm")), "gris-couleurs/Embryos.512.pgm");
    ecrirePGM(CouleurAuGris(lirePPM("images/House.256.ppm"  )), "gris-couleurs/House.256.pgm");
}

void grisACouleurTest() {
    CHECK( ImageGrisEgal(CouleurAuGris(GrisACouleur(imgGrisTest)),
             imgGrisTest,
             0.001) );
}

int main(){
    cerr << "Tests de la fonction couleurAuGris" << endl;
    couleurAuGrisTest();
}
