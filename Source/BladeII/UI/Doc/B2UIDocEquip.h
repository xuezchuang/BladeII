// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "B2Inventory.h"
#include "B2UIDocEquip.generated.h"

/* Documents representing each device slot. And wrap the item data. */
UCLASS()
class BLADEII_API UB2UIDocEquip : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocEquip(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocEquip();


	DECLARE_UIDOC_FIELD(int32,	EquipPlace) // Not for change once created for UIDocHero.

	DECLARE_UIDOC_FIELD(bool,	Equipped)

	DECLARE_UIDOC_FIELD(int64,	EquippedItemId)

	DECLARE_UIDOC_FIELD(struct FB2Item, EquippedItem)


public:
	/* It sets the hero who owns this skill. */
	void SetOwnerHeroDoc(class UB2UIDocHero* OwnerDoc)	{ OwnerHeroDoc = OwnerDoc; }
	class UB2UIDocHero* GetOwnerHeroDoc() const			{ return OwnerHeroDoc; }

	/** To be used by client to update data here in simpler manner */
	void DoEquip(const FB2Item& InEquipItem);
	void RemoveEquip();
	void UpdateData(const FB2Item& InNewData); // It just update item data, without altering equipping state.

private:
	class UB2UIDocHero* OwnerHeroDoc;
};
