// Copyright Epic Games, Inc. All Rights Reserved.

#include "bruitPerlinBPLibrary.h"
#include "bruitPerlin.h"
#include <iostream>
#include <cmath>
#include <random>
#include <ctime>
#include <Windows.h>
using namespace std;
const int IYMAX = 256;//valeur de la map initial
const int IXMAX = 256;
float Gradient[IYMAX][IXMAX][2];


UbruitPerlinBPLibrary::UbruitPerlinBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UbruitPerlinBPLibrary::bruitPerlinSampleFunction(float Param)
{
	return -1;
}

float UbruitPerlinBPLibrary::bruitPerlin(int32 graine, float x, float y)
{
    initialisationDesGradients();
    float bruit = perlin(x / 10.0, y / 10.0);
    bruit = (bruit + 1) / 2; // Normaliser le vecteur [0, 1]
    return bruit;
}
void UbruitPerlinBPLibrary::initialisationDesGradients() {
    mt19937 rng(time(nullptr)); // graine initial avec le temps
    uniform_real_distribution<float> dist(0.0, 2 * 3.1415);//valeur du cercle
    //initialise les gradients selon un angle theta qui est générer aleatoirement
    for (int i = 0; i < IYMAX; ++i) {
        for (int j = 0; j < IXMAX; ++j) {
            float theta = dist(rng);
            Gradient[i][j][0] = cos(theta);
            Gradient[i][j][1] = sin(theta);
        }
    }
}

float UbruitPerlinBPLibrary::linearisation(float poid) {
    if (poid <= 0.0) return 0.0;
    if (poid >= 1.0) return 1.0;
    return poid * poid * (3.0 - 2.0 * poid);
}

float UbruitPerlinBPLibrary::interpolation(float a0, float a1, float poid) {
    return a0 + (a1 - a0) * linearisation(poid);
}
//calcul du produit scalaire entre la distance et le vecteur gradient
float UbruitPerlinBPLibrary::produitScalaire(int ix, int iy, float x, float y) {
    //calcul le vecteur distance
    float dx = x - static_cast<float>(ix);
    float dy = y - static_cast<float>(iy);
    //retourne le produit scalaire
    return (dx * Gradient[iy][ix][0] + dy * Gradient[iy][ix][1]);
}

float UbruitPerlinBPLibrary::perlin(float x, float y) {
    //détermine les coordonnees
    int x0 = static_cast<int>(floor(x));
    int x1 = x0 + 1;
    int y0 = static_cast<int>(floor(y));
    int y1 = y0 + 1;
    //détermine le poid de l'interpolation
    float sx = x - static_cast<float>(x0);
    float sy = y - static_cast<float>(y0);

    float n0, n1, ix0, ix1;
    n0 = produitScalaire(x0, y0, x, y);
    n1 = produitScalaire(x1, y0, x, y);
    ix0 = interpolation(n0, n1, sx);
    n0 = produitScalaire(x0, y1, x, y);
    n1 = produitScalaire(x1, y1, x, y);
    ix1 = interpolation(n0, n1, sx);
    //calcule le nouveau vecteur et renvoie la valeur interpoler(lisse)
    return interpolation(ix0, ix1, sy);
}

int UbruitPerlinBPLibrary::generationGraine(int graine)
{
    srand(time(NULL));
    if (graine != 0)
    {
        return graine;
    }
    else
    {
        return graine = rand();//récupère le temps comme graine initial
    }
    return graine;
}
int UbruitPerlinBPLibrary::valeurAleatoire(int max,int min,int m_dernierChiffre,int m_chiffreSuivant)
{
    m_dernierChiffre += m_chiffreSuivant;
    m_dernierChiffre %= max;
    m_dernierChiffre += min;
    return m_dernierChiffre;
}

int UbruitPerlinBPLibrary::PRNGFibonacci(int graine,int iterations)
{
    if (iterations == 0) {
        return graine;
    }
    else if (iterations == 1)
    {
        return graine + 1;
    }
    else {
        return (PRNGFibonacci(graine, iterations - 1) + PRNGFibonacci(graine, iterations - 2))%100000;
    }
}