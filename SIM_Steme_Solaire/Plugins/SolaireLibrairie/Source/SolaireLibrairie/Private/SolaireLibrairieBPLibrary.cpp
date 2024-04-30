// Copyright Epic Games, Inc. All Rights Reserved.

#include "SolaireLibrairieBPLibrary.h"
#include "SolaireLibrairie.h"

USolaireLibrairieBPLibrary::USolaireLibrairieBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float USolaireLibrairieBPLibrary::SolaireLibrairieSampleFunction(float Param)
{
	return -1;
}

FVector USolaireLibrairieBPLibrary::GetInitialeVelocite(TArray<int32> Time, FString NomPlanete)
{
    const TArray<FString> NOM_PLANETE = {
   "Mercure", "Venus", "Terre", "Mars", "Jupiter", "Saturne", "Uranus", "Neptune"
    };
    const TArray<TArray<TArray<float>>> ELEMENTS = {
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
        // Pluto
        {{39.48211675, 0.24882730, 17.14001206, 238.92903833, 224.06891629, 110.30393684},
        {-0.00031596, 0.00005170, 0.00004818, 145.20780515, -0.04062942, -0.01183482}}
    };
    const TArray<TArray<float>> EXTRA_TERME = {
        {-0.00012452, 0.06064060, -0.35635438, 38.35125}, // Jupiter
        {0.00025899, -0.13434469, 0.87320147, 38.35125},  // Saturn
        {0.00058331, -0.97731848, 0.17689245, 7.67025},   // Uranus
        {-0.00041348, 0.68346318, -0.10162547, 7.67025},  // Neptune
        {-0.01262724, 0.0, 0.0, 0.0}                      // Pluto
    };

    TArray<float> elements;
    TArray<float> rates;
    TArray<float> extraTerme;
    int32 indice = -1;
    for (int32 i = 0; i < NOM_PLANETE.Num(); ++i) {
        if (NOM_PLANETE[i] == NomPlanete) {
            indice = i; // Indice trouvé
            break;
        }
    }
    if (indice >= 0 && indice < NOM_PLANETE.Num()) {
        // Copier les éléments correspondants
        elements = ELEMENTS[indice][0];
        //std::copy(::this->ELEMENTS[indice][0].begin(), ::this->ELEMENTS[indice][0].end(), elements.begin());
        // Copier les taux correspondants
        //std::copy(::ELEMENTS[indice][1].begin(), ::ELEMENTS[indice][1].end(), rates.begin());
        rates = ELEMENTS[indice][1];
    }
    if (indice > 3 && indice < NOM_PLANETE.Num()) {
        extraTerme = EXTRA_TERME[indice];
        //std::copy(::EXTRA_TERME[indice][1].begin(), ::EXTRA_TERME[indice][1].end(), extraTerme.begin());
    }

    // Trouver la date Julian qui est necessaire pour les lois de kepler
    double JulianDate = GetJulianDate(Time[0], Time[1], Time[2], Time[3], Time[4], Time[5]);
    // On ajoute une minute afin d'avoir la position de la planète 1 minutes plus tard
    // On fait ca pour pouvoir calculer le vecteur et ainsi la velocite 
    double JulianDate2 = GetJulianDate(Time[0], Time[1] + 1, Time[2], Time[3], Time[4], Time[5]);

    TArray<float> positionInitial = ComputePlanetePosition(JulianDate, elements, rates, extraTerme);
    TArray<float> position2 = ComputePlanetePosition(JulianDate2, elements, rates, extraTerme);

    FVector vecteur;
    float magnitude = 0.0f;
    float temps = 60.0f;
    // Calcul du vecteur de position
    for (int32 i = 0; i < 3; ++i) {
        vecteur[i] = position2[i] - positionInitial[i];
        magnitude += vecteur[i] * vecteur[i];
    }
    magnitude = FMath::Sqrt(magnitude);

    // Normalisation du vecteur en divisant chaque composante par sa magnitude
    for (int32 i = 0; i < 3; ++i) {
        vecteur[i] /= magnitude;
    }

    // Calcul de la vitesse scalaire (distance parcourue divisée par le temps écoulé)
    float vitesse_scalaire = magnitude / temps;

    // Calcul de la vélocité en multipliant la vitesse scalaire par le vecteur directionnel
    FVector velocite;
    for (int32 i = 0; i < 3; ++i) {
        velocite[i] = vitesse_scalaire * vecteur[i];
    }



    return velocite;
}
TArray<float> USolaireLibrairieBPLibrary::ComputePlanetePosition(double jd, TArray<float>& elements, TArray<float>& rates, TArray<float>& extraTerms)
{
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
    TArray<float> pos;
    pos.Add(x);
    pos.Add(y);
    pos.Add(z);

    return pos;
}
float USolaireLibrairieBPLibrary::CalculKepler(float M, float e, float E)
{
    const float toRad = PI / 180;

    float dM = M - (E - e / toRad * FMath::Sin(E * toRad));
    float dE = dM / (1 - e * FMath::Cos(E * toRad));
    return dE;
}
double USolaireLibrairieBPLibrary::GetJulianDate(int32 sec, int32 min, int32 heure, int32 jour, int32 mois, int32 annee)
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