// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe.h"

Universe::Universe()
{
	SetG(6.674);
}

Universe::~Universe()
{
}

void Universe::SetG(float G) {
	m_G = G;
}

float Universe::GetG() const {
	return m_G;
}
