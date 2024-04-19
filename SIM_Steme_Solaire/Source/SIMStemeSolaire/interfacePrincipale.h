// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "interfacePrincipale.generated.h"

/**
 * 
 */
UCLASS()
class SIMSTEMESOLAIRE_API UinterfacePrincipale : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))//propriete qui reli le code au bouton dans le Widget
		class UButton* boutonFichier;

	UPROPERTY(meta = (BindWidget))
		class UButton* boutonParametres;

	UPROPERTY(meta = (BindWidget))
		class UButton* boutonAide;

	UPROPERTY(meta = (BindWidget))
	class UButton* boutonFonctionAutre;

	void NativeConstruct() override; //override du constructeur pour initialiser les boutons
	//void EvenementBoutonFichier();
	void EvenementBoutonParametres();
	void EvenementBoutonAide();
	void EvenementBoutonFonctionAutre();

};
