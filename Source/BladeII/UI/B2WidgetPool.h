// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "Components/PanelWidget.h"
#include "B2WidgetPool.generated.h"

/** 
 * Defines common functionalities of UnitedWidgetBase widget pool.
 * It can be extended to reduce excessively repeated creation/destruction of specific type of user widget.
 * You must extend it (and customize if necessary) for actual use.
 */
USTRUCT()
struct FWidgetPoolBase
{
	GENERATED_USTRUCT_BODY()

	FWidgetPoolBase()
	{
		MyOwner = nullptr;
		CreateZOrder = 0;
	}
	virtual ~FWidgetPoolBase()
	{

	}

protected:

	UPROPERTY(Transient)
	UObject* MyOwner;

	/** List of pooled objects. bool field is redundant. It is TMap just for quick search. */
	UPROPERTY(Transient)
	TMap<class UB2UnitedWidgetBase*, bool> AllPooledObjects;

	int32 CreateZOrder;

public:
	/** It must be overridden to return proper UnitedWidgetBase derived class to create necessary pooled object. */
	virtual UClass* GetDesiredClass();

	virtual void InitPool(UObject* InOwnerObject, class APlayerController* InPC, int32 InPreCreateNum, int32 InCreateZOrder);

private:
	class UB2UnitedWidgetBase* CreateSingleInternal(class APlayerController* InPC);

public:
	virtual void DestroyAll();

	/** Get available one or create new one if none exist. 
	 * You may designate InOptionalPlacePanel to be a parent panel of borrowing widget */
	virtual class UB2UnitedWidgetBase* Rent(class APlayerController* InPC, UObject* InRenter, UPanelWidget* InOptionalPlacePanel = nullptr);
	/** Return after use. */
	virtual void TurnIn(class UB2UnitedWidgetBase* InUsedOne);
};


UENUM()
enum class EFloatingWidgetPoolClassType : uint8
{
	EFWPCT_DamageNum_Mob = 0,
	EFWPCT_DamageNum_PC = 1,
	EFWPCT_AbnormalEffect = 2,
	EFWPCT_GoldAcq = 3,
	EFWPCT_End
};

/** 
 * WidgetPool for In-game floating widgets to reduce runtime dynamic creation hitch, especially FloatingDamageNum.
 */
USTRUCT()
struct FFloatingWidgetPool : public FWidgetPoolBase
{
	GENERATED_USTRUCT_BODY()
	
	FFloatingWidgetPool()
		: Super()
	{
		WidgetClassType = EFloatingWidgetPoolClassType::EFWPCT_End;
		OwnerUIManager = nullptr;
	}
	virtual ~FFloatingWidgetPool()
	{

	}
private:
	EFloatingWidgetPoolClassType WidgetClassType;
	/** Casted MyOwner. Feel a little stupid but need this to make life easier. */
	UPROPERTY(Transient)
	class AB2UIManager_InGameCombat* OwnerUIManager;
	
public:
	virtual UClass* GetDesiredClass() override;

	/** It must be called before InitPool, for proper GetDesiredClass working. */
	void PreInitSetType(EFloatingWidgetPoolClassType InType);

	virtual void InitPool(UObject* InOwnerObject, class APlayerController* InPC, int32 InPreCreateNum, int32 InCreateZOrder) override;

	virtual class UB2UnitedWidgetBase* Rent(class APlayerController* InPC, UObject* InRenter, UPanelWidget* InOptionalPlacePanel = nullptr) override;

	virtual void DestroyAll() override;
};

UENUM()
enum class EInventoryDynItemIconPoolClassType : uint8
{
	StoredItem = 0,
	EquippedItem = 1,
	End
};

/**
 * WidgetPool for item icons in lobby inventory view. to reduce excessively repeated item icon creation/destruction .
 */
USTRUCT()
struct FInventoryDynItemIconPool : public FWidgetPoolBase
{
	GENERATED_USTRUCT_BODY()

	FInventoryDynItemIconPool()
		: Super()
	{
		WidgetClassType = EInventoryDynItemIconPoolClassType::End;
	}
	virtual ~FInventoryDynItemIconPool()
	{

	}
private:
	
	EInventoryDynItemIconPoolClassType WidgetClassType;

	/** Casted MyOwner. */
	UPROPERTY(Transient)
	class UB2UIManager_Lobby* OwnerUIManager;

public:
	virtual UClass* GetDesiredClass() override;

	/** It must be called before InitPool, for proper GetDesiredClass working. */
	void PreInitSetType(EInventoryDynItemIconPoolClassType InType);

	virtual void InitPool(UObject* InOwnerObject, class APlayerController* InPC, int32 InPreCreateNum, int32 InCreateZOrder) override;

	virtual class UB2UnitedWidgetBase* Rent(class APlayerController* InPC, UObject* InRenter, UPanelWidget* InOptionalPlacePanel = nullptr) override;

	virtual void TurnIn(class UB2UnitedWidgetBase* InUsedOne) override;

	virtual void DestroyAll() override;
};
