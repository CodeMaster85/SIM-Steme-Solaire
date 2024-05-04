#include "MyBlueprintFunctionLibraryTest.h"
#include <iostream>
#include <iomanip>
#include "Engine/Engine.h"
/*
Yohan Chuet 
Coder entre le 1 mars et le 14 mai 
Calcul la position/Velocite initiale des planetes du systeme solaire 
Est un code utiliser dans Unreal Engin 5
Le code est seulement utiliser en fonction, c'est a dire que on ne peu pas créer d'instance de cette fonction

Les données ont été trouver en ligne sur : ssd.jpl.nasa.gov/planets/approx_pos.html et nssdc.gsfc.nasa.gov/planetary/factsheet/
La masse des planète est masse(10^24)kg

Le temps est calculer en Julian Date puisque c'est celui-ci qui est utiliser dans les fonctions de Kepler
Le jour julien est un système de datation consistant à compter le nombre de jours et fraction de jour écoulés
depuis une date conventionnelle fixée au 1er janvier de l'an 4713 av. J.-C 
Source Wikipedia : fr.wikipedia.org/wiki/Jour_julien
*/
const TArray<FString> UMyBlueprintFunctionLibraryTest::NOM_PLANETE = {
    "Mercure", "Venus", "Terre", "Mars", "Jupiter", "Saturne", "Uranus", "Neptune"
};
const TArray<TArray<TArray<float>>> UMyBlueprintFunctionLibraryTest::ELEMENTS = {
    // Mercury
    {{0.38709927, 0.20563593, 7.00497902, 252.25032350, 77.45779628, 48.33076593},
    {0.00000037, 0.00001906, -0.00594749, 149472.67411175, 0.16047689, -0.12534081}},
    // Venus
    {{0.72333566, 0.00677672, 3.39467605, 181.97909950, 131.60246718, 76.67984255},
    {0.00000390, -0.00004107, -0.00078890, 58517.81538729, 0.00268329, -0.27769418}},
    // Earth-Moon Barycenter
    {{1.00000261, 0.01671123, -0.00001531, 100.46457166, 102.93768193, 0.0},
    {0.00000562, -0.00004392, -0.01294668, 35999.37244981, 0.32327364, 0.0}},
    // Mars
    {{1.52371034, 0.09339410, 1.84969142, -4.55343205, -23.94362959, 49.55953891},
    {0.00001847, 0.00007882, -0.00813131, 19140.30268499, 0.44441088, -0.29257343}},
    // Jupiter
    {{5.20288700, 0.04838624, 1.30439695, 34.39644051, 14.72847983, 100.47390909},
    {-0.00011607, -0.00013253, -0.00183714, 3034.74612775, 0.21252668, 0.20469106}},
    // Saturn
    {{9.53667594, 0.05386179, 2.48599187, 49.95424423, 92.59887831, 113.66242448},
    {-0.00125060, -0.00050991, 0.00193609, 1222.49362201, -0.41897216, -0.28867794}},
    // Uranus
    {{19.18916464, 0.04725744, 0.77263783, 313.23810451, 170.95427630, 74.01692503},
    {-0.00196176, -0.00004397, -0.00242939, 428.48202785, 0.40805281, 0.04240589}},
    // Neptune
    {{30.06992276, 0.00859048, 1.77004347, -55.12002969, 44.96476227, 131.78422574},
    {0.00026291, 0.00005105, 0.00035372, 218.45945325, -0.32241464, -0.00508664}},
};
const TArray<TArray<float>> UMyBlueprintFunctionLibraryTest::EXTRA_TERME = {
    {-0.00012452, 0.06064060, -0.35635438, 38.35125}, // Jupiter
    {0.00025899, -0.13434469, 0.87320147, 38.35125},  // Saturn
    {0.00058331, -0.97731848, 0.17689245, 7.67025},   // Uranus
    {-0.00041348, 0.68346318, -0.10162547, 7.67025},  // Neptune
};
const TArray<float> UMyBlueprintFunctionLibraryTest::Masse = {
    0.330f, 4.87f, 5.97f, 0.643f, 1898.0f, 568.0f, 86.8f, 102.0f
};


FVector UMyBlueprintFunctionLibraryTest::GetInitialeVelocite(const TArray<int32>& Time, int32 index)
{
    FVector positionInitial = GetInitialPosition(Time, index);
    TArray<int32> Time2 = Time;
    // ajouter 1 jour pour pouvoir calculer la direction et la vitesse de la planete 
    Time2[3]++;
    FVector position2 = GetInitialPosition(Time2, index);
    UE_LOG(LogTemp, Warning, TEXT("Pos 1 : %s, Pos 2: %s"), *positionInitial.ToString(), *position2.ToString()); // erreur position mauvaise
    FVector vecteurDirection;
    float magnitude = 0.0f;
    float temps = 60.0f;
    // Calcul du vecteur de position
    for (int32 i = 0; i < 3; ++i) {
        vecteurDirection[i] = position2[i] - positionInitial[i];
        magnitude += vecteurDirection[i] * vecteurDirection[i];
    }
    magnitude = FMath::Sqrt(magnitude);

    // Normalisation du vecteur en divisant chaque composante par sa magnitude
    for (int32 i = 0; i < 3; ++i) {
        vecteurDirection[i] /= magnitude;
    }

    // Calcul de la vitesse scalaire (distance parcourue divisée par le temps écoulé)
    float vitesse_scalaire = magnitude / temps;

    // Calcul de la vélocité en multipliant la vitesse scalaire par le vecteur directionnel
    FVector velocite;
    for (int32 i = 0; i < 3; ++i) {
        velocite[i] = vitesse_scalaire * vecteurDirection[i];
    }
    UE_LOG(LogTemp, Warning, TEXT("velocite : %s, "), *velocite.ToString()); // erreur position mauvaise

    return velocite;
}
FVector UMyBlueprintFunctionLibraryTest::GetInitialPosition(const TArray<int32>& Time, int32  indice)
{
    TArray<float> elements;
    TArray<float> rates;
    TArray<float> extraTerme;
    if (indice >= 0 && indice < NOM_PLANETE.Num()) {
        // Copier les éléments correspondants
        elements = ELEMENTS[indice][0];
        rates = ELEMENTS[indice][1];
    }
    if (indice > 3 && indice < NOM_PLANETE.Num()) {
        extraTerme = EXTRA_TERME[indice];
    }

    double JulianDate = GetJulianDate(Time[0], Time[1], Time[2], Time[3], Time[4], Time[5]);
    return  1000 * ComputePlanetePosition(JulianDate, elements, rates, extraTerme);
}
FString UMyBlueprintFunctionLibraryTest::GetName(int32 index)
{
    if (index >= 0 && index < NOM_PLANETE.Num())
    {
        return NOM_PLANETE[index];
    }
    else{
        UE_LOG(LogTemp, Warning, TEXT("GetName - index out of range"));
        return "Error";
    }
}
float UMyBlueprintFunctionLibraryTest::GetMass(int32 index)
{
    if (index >= 0 && index < NOM_PLANETE.Num())
    {
        return Masse[index];
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("GetMass - index out of range"));
        return -1;
    }
}


FVector UMyBlueprintFunctionLibraryTest::ComputePlanetePosition(double jd, TArray<float>& elements, TArray<float>& rates, TArray<float>& extraTerms)
{
    // Cette fonction calcul la distance entre les planetes et le soleil 
    // Les unité de cette distance sont des UA qui correspond 1 UA = 149,6 millions de km 
    // Cette mesure est souvent utiliser dans les calculs du systeme solaire car c la distance moyenne entre le soleil et la terre 
    const float toRad = PI / 180;
    // Step 1 
    const float T = (jd - 2451545.0) / 36525;
    float a = elements[0] + rates[0] * T;
    float e = elements[1] + rates[1] * T;
    float I = elements[2] + rates[2] * T;
    float L = elements[3] + rates[3] * T;
    float w0 = elements[4] + rates[4] * T;
    float O = elements[5] + rates[5] * T;

    // Step 2 
    float w1 = w0 - O;
    float M = L - w0;
    if (extraTerms.Num() > 2) {
        const float b = extraTerms[0];
        const float c = extraTerms[1];
        const float s = extraTerms[2];
        const float f = extraTerms[3];
        M = L - w0 + b * T * T + c * FMath::Cos(f * T * toRad) + s * FMath::Sin(f * T * toRad);
    }
    while (M > 180) {
        M -= 360;
    }
    // Step 3 let 
    float E = M + 57.29578 * e * FMath::Sin(M * toRad);
    float dE = 1;
    int32 n = 0;
    while (FMath::Abs(dE) > 1e-7 && n < 10) {
        dE = CalculKepler(M, e, E);
        E += dE;
        n++;
    }

    //Step 4
    const float xp = a * (FMath::Cos(E * toRad) - e);
    const float yp = a * FMath::Sqrt(1 - e * e) * FMath::Sin(E * toRad);
    const float zp = 0;

    //Step 5:
    a *= toRad;
    e *= toRad;
    I *= toRad;
    L *= toRad;
    w1 *= toRad;
    O *= toRad;
    const float xecl = (FMath::Cos(w1) * FMath::Cos(O) - FMath::Sin(w1) * FMath::Sin(O) * FMath::Cos(I)) * xp + (-FMath::Sin(w1) * FMath::Cos(O) - FMath::Cos(w1) * FMath::Sin(O) * FMath::Cos(I)) * yp;
    const float yecl = (FMath::Cos(w1) * FMath::Sin(O) + FMath::Sin(w1) * FMath::Cos(O) * FMath::Cos(I)) * xp + (-FMath::Sin(w1) * FMath::Sin(O) + FMath::Cos(w1) * FMath::Cos(O) * FMath::Cos(I)) * yp;
    const float zecl = (FMath::Sin(w1) * FMath::Sin(I)) * xp + (FMath::Cos(w1) * FMath::Sin(I)) * yp;

    // Step 6 
    const float eps = 23.43928 * toRad;

    const float x = xecl;
    const float y = FMath::Cos(eps) * yecl - FMath::Sin(eps) * zecl;
    const float z = FMath::Sin(eps) * yecl + FMath::Cos(eps) * zecl;

    FVector pos;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    return pos;
}
float UMyBlueprintFunctionLibraryTest::CalculKepler(float M, float e, float E)
{
    const float toRad = PI / 180;

    float dM = M - (E - e / toRad * FMath::Sin(E * toRad));
    float dE = dM / (1 - e * FMath::Cos(E * toRad));
    return dE;
}
double UMyBlueprintFunctionLibraryTest::GetJulianDate(int32 sec, int32 min, int32 heure, int32 jour, int32 mois, int32 annee)
{
    double a = (14 - mois) / 12;
    double y = annee + 4800 - a;
    double m = mois + 12 * a - 3;
    double JD = jour + (153 * m + 2) / 5 + (y * 365) + (y / 4) - (y / 100 + y / 400) - 32045;

    double JD_decimal = heure / 24.0 + min / 1440.0 + sec / 86400.0;

    // Date julienne finale
    JD += JD_decimal;
    return JD;
}


