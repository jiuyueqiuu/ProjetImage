/** @file
 * Programme permettant d'appliquer divers filtres à un fichier PPM
 **/

#include <iostream>
#include <unistd.h> // pour getopt
#include "image.hpp"
#include "pgm.hpp"
#include "ppm.hpp"
#include "gris-couleurs.hpp"
#include "sobel.hpp"
#include "seuillage.hpp"
#include "superpixel.hpp"

/** Fonction principale **/
int main(int argc, char **argv)
{
    char c;
    string source, cible;
    // Quels filtres appliquer
    bool niveaux_de_gris = false;
    bool sobel = false;
    bool seuil = false;
    bool doubleseuillage = false;
    bool superpixel = false;

    while ((c = getopt(argc, argv, "hgdsepc:o:")) != -1)
        switch (c)
        {
        case 'g':
            niveaux_de_gris = true;
            break;
        case 's':
            sobel = true;
            break;
        case 'e':
            seuil = true;
            break;
        case 'd':
            doubleseuillage = true;
            break;
        case 'p':
            superpixel = true;
            break;
        case 'c':
            source.assign(optarg);
            break;
        case 'o':
            cible.assign(optarg);
            break;
        case ':':
            cout << "Missing argument" << endl;
            break;
        case 'h':
        case '?':
            cout << "Usage: [-g | -s | -e | -p] SOURCE CIBLE" << endl
                 << endl;
            cout << "-g: Niveaux de gris" << endl;
            cout << "-e: Filtre de Sobel" << endl;
            cout << "-s: Seuillage" << endl;
            cout << "-d: Double seuillage" << endl;
            cout << "-p: Superpixel" << endl
                 << endl;
            return 0;
            break;
        }
    source = argv[optind];
    cible = argv[optind + 1];
    if (niveaux_de_gris)
        ecrirePGM(CouleurAuGris(lirePPM(source)), cible);
    if (sobel)
        ecrirePGM(renormalise(intensite(lirePGM(source))), cible);
    if (seuil)
        ecrirePGM(seuillage(lirePGM(source), 80), cible);
    if (doubleseuillage)
        ecrirePGM(doubleSeuillage(lirePGM(source), 500, 80, 100), cible);
    if (superpixel)
        ecrirePPM(superPixel(lirePPM(source), 2, 60, 15), cible);

    return 0;
}
