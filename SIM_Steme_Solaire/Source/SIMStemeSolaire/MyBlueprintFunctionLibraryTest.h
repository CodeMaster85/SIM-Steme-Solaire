// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibraryTest.generated.h"

/**
 * 
 */
UCLASS()
class SIMSTEMESOLAIRE_API UMyBlueprintFunctionLibraryTest : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
    //UPlanetaryOrbitCalculator();

    UFUNCTION(BlueprintCallable)
    static FVector GetInitialeVelocite(const TArray<int32>& Time, const FString& NomPlanete);

    UFUNCTION(BlueprintCallable)
    static FVector GetInitialPosition(const TArray<int32>& Time, int32 index);
    

private:
    // Fonction
    static FVector ComputePlanetePosition(double jd, TArray<float>& elements, TArray<float>& rates, TArray<float>& extraTerms);
    static float CalculKepler(float M, float e, float E);
    static double GetJulianDate(int32 sec, int32 min, int32 heure, int32 day, int32 month, int32 years);

    // constante
    static const TArray<FString> NOM_PLANETE;
    static const TArray<TArray<TArray<float>>> ELEMENTS;
    // Termes supplémentaires pour certaines planètes
    static const TArray<TArray<float>> EXTRA_TERME;
};
