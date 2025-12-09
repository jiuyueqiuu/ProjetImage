#include <iostream>
#include <vector>
#include <stdexcept>
/** @file
 * Filtre Super Pixel
 **/
#include <cmath>
using namespace std;

typedef vector<vector<double>> Image;

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

    for (int i = 0; i < nbAmeliorations; i++) {
        EnsemblePoints nouveauC = C;
        for (int k = 0; k < C.size(); k++) {
            EnsemblePoints Q = sousEnsemble(P, C, k);
            if (Q.size() > 0) {
                nouveauC[k] = barycentre(Q);
            }
        }
        C = nouveauC;
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
EnsemblePoints pivotSuperPixel (Image img, double lambda, int mu);
/// END pivotSuperPixel

/// BEGIN superPixels

/** Renvoie les superpixels d'une image dans l'espace spatio colorimetrique
 * @param img une image en teintes de gris
 * @param lambda un double
 * @param mu un entier
 * @param nbAmeliorations un entier
 * @return un ensemble de points, les superpixels
 **/
EnsemblePoints superPixels(Image img, double lambda, int mu, int nbAmeliorations);
/// END superPixels

/// BEGIN SuperPixel

/** Filtre SuperPixel
 * @param img une image
 * @param lambda un double
 * @param mu un entier
 * @param nbAmeliorations
 * @return l'image associee aux superpixels d'une image
 **/
Image superPixel(Image img, double lambda, int mu, int nbAmeliorations);
/// END SuperPixel

