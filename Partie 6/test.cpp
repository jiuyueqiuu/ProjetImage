#include <iostream>
#include <vector>
#include <stdexcept>
/** @file
 * Filtre Super Pixel
 **/
#include <cmath>
using namespace std;

struct Couleur { double r, g, b; };
typedef vector<vector<Couleur>> Image;


/// BEGIN EnsemblePoints

/** Structure de donnee representant un point dans l'espace
    spacio colorimetrique **/
typedef vector<double> Point;

/** Structure de donnee representant un ensemble de points dans l'espace
    spacio colorimetrique **/
typedef vector<Point> EnsemblePoints;
/// END EnsemblePoints

double distancePoints(Point p, Point c) {
    double sum = 0.0;

    for (int i = 0; i < p.size(); i++) {
        double diff = p[i] - c[i];
        sum += diff * diff;
    }

    return sqrt(sum);
}


double distanceAEnsemble(Point p, EnsemblePoints C) {
    long double distMin = 10000000000000;

    for (int i = 0; i < C.size(); i++) {
        double d = distancePoints(p, C[i]);
        if (d < distMin) {
            distMin = d;
        }
    }

    return distMin;
}

/** Renvoie le plus proche voisin d'un point p dans un ensemble C
 * @param p un point
 * @param C un ensemble de points
 * @return l'index du plus proche voisin
 **/
int plusProcheVoisin(Point p, EnsemblePoints C) {
    int indexMin = 0;
    long double distMin = 10000000000000;
    if (C.empty()) throw runtime_error("plusProcheVoisin called on empty EnsemblePoints");


    for (int i = 0; i < C.size(); i++) {
        double d = distancePoints(p, C[i]);
        if (d < distMin) {
            distMin = d;
            indexMin = i;
        }
    }

    return indexMin;
}
/// END plusProcheVoisin

/// BEGIN sousEnsemble

/** Renvoie les points p de P tels que C[k] est le plus proche voisin de p dans C
 * @param P un ensemble de points
 * @param C un ensemble de points
 * @param k un entier
 * @return un sous ensemble des points de P
 **/
EnsemblePoints sousEnsemble(EnsemblePoints P, EnsemblePoints C, int k) {
    EnsemblePoints res;

    for (int i = 0; i < P.size(); i++) {
        if (plusProcheVoisin(P[i], C) == k) {
            res.push_back(P[i]);
        }
    }

    return res;
}
/// END sousEnsemble

/// BEGIN barycentre

/** Renvoie le barycentre d'un ensemble de points
 * @param Q un ensemble de points
 * @return c le barycentre de Q
 **/
Point barycentre(EnsemblePoints Q) {
    if (Q.size() == 0) {
        return Point();
    }

    Point res(Q[0].size());

    for (auto p : Q) {
        for (int i = 0; i < res.size(); i++) {
            res[i] += p[i];
        }
    }

    for (int j = 0; j < res.size(); j++) {
        res[j] /= Q.size();
    }

    return res;
}

/// END barycentre

/// BEGIN kMoyenne

/** Renvoie la K-moyenne de deux ensembles de points
 * @param P un ensemble de points
 * @param C un ensemble de points
 * @param nbAmeliorations:entier le nombre de fois ou l'amelioration va etre effectuee
 * @return C un ensemble de points les positions finales de points pilotes
 **/
EnsemblePoints KMoyenne(EnsemblePoints P, EnsemblePoints C, int nbAmeliorations) {
    for (int iter = 0; iter < nbAmeliorations; iter++) {
        vector<EnsemblePoints> clusters(C.size());

        for (auto p : P) {
            int index = plusProcheVoisin(p, C);
            clusters[index].push_back(p);
        }

        for (int k = 0; k < C.size(); k++) {
            if (!clusters[k].empty()) {
                C[k] = barycentre(clusters[k]);
            }
        }
    }
    return C;
}

/// END kMoyenne

/// BEGIN FASTkMoyenne

/** Implantation optimisee de K-moyenne
 * @param P un ensemble de points
 * @param C un ensemble de points
 * @param nbAmeliorations:entier le nombre de fois ou l'amelioration va etre effectuee
 * @return C un ensemble de points les positions finales de points pilotes
 **/
EnsemblePoints FAST_KMoyenne(EnsemblePoints P, EnsemblePoints C, int nbAmeliorations) {
    vector<vector<Point>> clusters(C.size());

    for (int i = 0; i < nbAmeliorations; i++) {
        for (int k = 0; k < clusters.size(); k++) {
            clusters[k].clear();
        }

        for (int p = 0; p < P.size(); p++) {
            int index = plusProcheVoisin(P[p], C);
            clusters[index].push_back(P[p]);
        }

        for (int j= 0; j < C.size(); j++) {
            if (clusters[j].size() > 0) {
                C[j] = barycentre(clusters[j]);
            }
        }
    }

    return C;
}

/// END FASTkMoyenne

/// BEGIN pivotSuperPixel

/** Renvoie un ensemble de points (espace spatio colorimetrique)
 *  regulierement espaces dans une image
 * @param img une image
 * @param lambda un double
 * @param mu un entier
 * @return un ensemble de points dans l'espace spatio colorimetrique
 **/
EnsemblePoints pivotSuperPixel(Image img, double lambda, int mu) {
    EnsemblePoints res;
    int hauteur = img.size();
    if (hauteur == 0) return res;
    int largeur = img[0].size();
    if (largeur == 0) return res;

    for (int y = mu/2; y < hauteur; y += mu)
        for (int x = mu/2; x < largeur; x += mu) {
            Point p(5);
            p[0] = x;
            p[1] = y;
            p[2] = lambda * img[y][x].r;
            p[3] = lambda * img[y][x].g;
            p[4] = lambda * img[y][x].b;
            res.push_back(p);
        }

    if (res.empty()) {
        Point p(5);
        p[0] = largeur/2;
        p[1] = hauteur/2;
        p[2] = lambda * img[hauteur/2][largeur/2].r;
        p[3] = lambda * img[hauteur/2][largeur/2].g;
        p[4] = lambda * img[hauteur/2][largeur/2].b;
        res.push_back(p);
    }

    return res;
}

/// END pivotSuperPixel

/// BEGIN superPixels

/** Renvoie les superpixels d'une image dans l'espace spatio colorimetrique
 * @param img une image en teintes de gris
 * @param lambda un double
 * @param mu un entier
 * @param nbAmeliorations un entier
 * @return un ensemble de points, les superpixels
 **/
EnsemblePoints superPixels(Image img, double lambda, int mu, int nbAmeliorations) {
    EnsemblePoints C = pivotSuperPixel(img, lambda, mu);
    int hauteur = img.size();
    int largeur = img[0].size();
    if (img.empty() || img[0].empty()) return EnsemblePoints();
    EnsemblePoints P;
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            Point p(5);
            p[0] = x;
            p[1] = y;
            p[2] = lambda * img[y][x].r;
            p[3] = lambda * img[y][x].g;
            p[4] = lambda * img[y][x].b;
            P.push_back(p);
        }
    }
    C = FAST_KMoyenne(P, C, nbAmeliorations);
    return C;
}
/// END superPixels

/// BEGIN SuperPixel

/** Filtre SuperPixel
 * @param img une image
 * @param lambda un double
 * @param mu un entier
 * @param nbAmeliorations
 * @return l'image associee aux superpixels d'une image
 **/
Image superPixel(Image img, double lambda, int mu, int nbAmeliorations) {
    int hauteur = img.size();
    int largeur = img[0].size();
    if (img.empty() || img[0].empty()) return Image();
    Image res(hauteur, vector<Couleur>(largeur));
    EnsemblePoints C = superPixels(img, lambda, mu, nbAmeliorations);
    if (C.empty()) return res;
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            Point p(5);
            p[0] = x;
            p[1] = y;
            p[2] = lambda * img[y][x].r;
            p[3] = lambda * img[y][x].g;
            p[4] = lambda * img[y][x].b;
            int index = plusProcheVoisin(p, C);
            res[y][x].r = C[index][2] / lambda;
            res[y][x].g = C[index][3] / lambda;
            res[y][x].b = C[index][4] / lambda;
        }
    }
    return res;
}
/// END SuperPixel

