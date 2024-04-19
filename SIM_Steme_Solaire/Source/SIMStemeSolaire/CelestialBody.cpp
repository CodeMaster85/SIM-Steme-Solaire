// Fill out your copyright notice in the Description page of Project Settings.


#include "CelestialBody.h"

// Sets default values
ACelestialBody::ACelestialBody()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	StaticMeshComponent->SetStaticMesh(StaticMesh.Object);
	StaticMeshComponent->SetupAttachment(SceneComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SceneComponent);
	FVector NewLocation(-250, 0, 0);
	CameraComponent->AddRelativeLocation(NewLocation);

}

// Called when the game starts or when spawned
void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();
	G = Universe::GetG();
}

void ACelestialBody::UpdateVelocity(TArray<ACelestialBody*> Bodies) {
	for (ACelestialBody* Body : Bodies) {
		if (Body != this) {
			float SqrDistance = (Body->GetActorLocation() - GetActorLocation()).SquaredLength();
			FVector ForceDir = (Body->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FVector Acceleration = ForceDir * G * Body->GetMass() / SqrDistance;
			SetInstantVelocity(GetInstantVelocity() + Acceleration);
		}
	}
}

void ACelestialBody::UpdatePosition() {
	SetActorLocation(GetActorLocation() + GetInstantVelocity());
}

// Called every frame
void ACelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<ACelestialBody*> Bodies;
	for (TActorIterator<ACelestialBody> It(GetWorld()); It; ++It) {
		ACelestialBody* Body = *It;
		Bodies.Add(*It);
	}

	for (ACelestialBody* Body : Bodies) {
		Body->UpdateVelocity(Bodies);
	}

	for (ACelestialBody* Body : Bodies) {
		Body->UpdatePosition();
	}

}

void ACelestialBody::SetName(FString Name) {
	m_Name = Name;
}

FString ACelestialBody::GetName() {
	return m_Name;
}

void ACelestialBody::SetMass(float Mass) {
	m_Mass = Mass;
}

float ACelestialBody::GetMass() {
	return m_Mass;
}

void ACelestialBody::SetInstantVelocity(FVector InstantVelocity) {
	m_InstantVelocity = InstantVelocity;
}

FVector ACelestialBody::GetInstantVelocity() {
	return m_InstantVelocity;
}

// Called to bind functionality to input
void ACelestialBody::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

