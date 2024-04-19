// Fill out your copyright notice in the Description page of Project Settings.


#include "interfacePrincipale.h"
#include "Animation/WidgetAnimation.h"
#include <Components/Button.h>
void UinterfacePrincipale::NativeConstruct()
{
	Super::NativeConstruct();
	//reconstruction du constructeur/ appelle des autres fonctions dans le constructeur

	//boutonFichier->OnClicked.AddUniqueDynamic(this, &UinterfacePrincipale::EvenementBoutonFichier);
	boutonParametres->OnClicked.AddUniqueDynamic(this, &UinterfacePrincipale::EvenementBoutonParametres);
	boutonAide->OnClicked.AddUniqueDynamic(this, &UinterfacePrincipale::EvenementBoutonAide);
	boutonFonctionAutre->OnClicked.AddUniqueDynamic(this, &UinterfacePrincipale::EvenementBoutonFonctionAutre);

}
//void UinterfacePrincipale::EvenementBoutonFichier()
//{
	//ouvrir la page fichier
//}
void UinterfacePrincipale::EvenementBoutonParametres()
{
	//ouvrir la page parametres
}
void UinterfacePrincipale::EvenementBoutonAide()
{
	//ouvrir la page aide
}
void UinterfacePrincipale::EvenementBoutonFonctionAutre()
{

}

