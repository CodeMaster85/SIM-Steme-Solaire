// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SIMSTEMESOLAIRE_API Universe
{
public:
	Universe();
	~Universe();

	float m_G;

	void SetG(float G);

	float GetG() const;

};
