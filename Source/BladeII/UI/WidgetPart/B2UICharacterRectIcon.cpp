// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UICharacterRectIcon.h"
#include "B2UIManager.h"

void UB2UICharacterRectIcon::Init()
{
	Super::Init();
	EntryPCClass = EPCClass::EPC_Gladiator;
}

void UB2UICharacterRectIcon::CacheAssets()
{
	GET_SLOT(UImage, IMG_Character)
}

void UB2UICharacterRectIcon::SelectCharacterClass(EPCClass CharType)
{
	if (CharacterTexture.Num() < GetMaxPCClassNum())
		return;

	EntryPCClass = CharType;

	if (IMG_Character.IsValid())
	{
		IMG_Character->SetBrushFromMaterial(CharacterTexture[PCClassToInt(CharType)]);
	}
}




