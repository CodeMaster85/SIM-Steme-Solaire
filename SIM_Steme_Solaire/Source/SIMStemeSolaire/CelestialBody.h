// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "Universe.h"
#include "CelestialBody.generated.h"

UCLASS()
class SIMSTEMESOLAIRE_API ACelestialBody : public APawn, public Universe
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACelestialBody();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCameraComponent* CameraComponent;

	float G;

	UFUNCTION(BlueprintCallable)
	void UpdateVelocity(TArray <ACelestialBody*> Bodies);

	UFUNCTION(BlueprintCallable)
	void UpdatePosition();

	UFUNCTION(BlueprintCallable)
	void SetName(FString Name);

	UFUNCTION(BlueprintCallable)
	FString GetName();

	UFUNCTION(BlueprintCallable)
	void SetMass(float Mass);

	UFUNCTION(BlueprintCallable)
	float GetMass();

	UFUNCTION(BlueprintCallable)
	FVector GetInstantVelocity();

	UFUNCTION(BlueprintCallable)
	void SetInstantVelocity(FVector InstantVelocity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString m_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FVector m_InstantVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float m_Mass;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
