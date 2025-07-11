// Fill out your copyright notice in the Description page of Project Settings.
#include "B2WidgetPool.h"

#include "B2UIManager_InGameCombat.h"
#include "B2UIManager.h"
#include "B2UIWidgetBase.h"
#include "BladeIIUserWidget.h"

#include "BladeIICharacter.h"
#include "Event.h"
#include "B2FloatingDamageNum.h"
#include "B2FloatingAbnormalEffect.h"
#include "B2FloatingGoldAcq.h"
#include "B2LobbyGameMode.h"
#include "B2UIManager_Lobby.h"
#include "B2DynItemIcon.h"

UClass* FWidgetPoolBase::GetDesiredClass()
{
	// �翬�� �̰� �״�� ����ϸ� �ƹ��͵� �Ǵ� �� ���� ����. �����δ� Widget Blueprint Ŭ������ �����ؾ�..
	check(0);
	return UB2UnitedWidgetBase::StaticClass();
}

void FWidgetPoolBase::InitPool(UObject* InOwnerObject, APlayerController* InPC, int32 InPreCreateNum, int32 InCreateZOrder)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("FWidgetPoolBase::InitPool, PreCreateNum %d"), InPreCreateNum));
	
	MyOwner = InOwnerObject;
	checkSlow(MyOwner);

	CreateZOrder = InCreateZOrder;
	checkSlow(CreateZOrder >= 0);

	for (int32 CreateIdx = 0; CreateIdx < InPreCreateNum; ++CreateIdx)
	{
		UB2UnitedWidgetBase* NewCreated = CreateSingleInternal(InPC);
		checkSlow(NewCreated && AllPooledObjects.Find(NewCreated));
	}
}

UB2UnitedWidgetBase* FWidgetPoolBase::CreateSingleInternal(APlayerController* InPC)
{
	UB2UnitedWidgetBase* NewCreated = CreateWidget<UB2UnitedWidgetBase>(InPC, GetDesiredClass());
	if (NewCreated)
	{
		// �ٸ� panel �� AddChild �ؾ��Ѵٸ� Rent ������..
		// �׳� AddToViewport �ؼ� ����ϴ� ��쵵 Rent ������ �ٲ� �� �ְ����� �׷� ��뿡���� ó�� Z-order �� �����ϴ� �� �Ϲ����� �� ���� ���� ���� �����ϰ� ���� �� ��.
		NewCreated->AddToViewport(CreateZOrder); 
		NewCreated->SetAsPooledObject(); // ���� ���ĺ��� rent �� �ƴ�.
		AllPooledObjects.Add(NewCreated, false);
	}
	return NewCreated;
}

void FWidgetPoolBase::DestroyAll()
{
	//for (TMap<UB2UnitedWidgetBase*, bool>::TIterator ItPO(AllPooledObjects); ItPO; ++ItPO)
	//{
	//	UB2UnitedWidgetBase* ThisOne = ItPO.Key();
	//	if (ThisOne)
	//	{
	//		// �� UI Ÿ�Ժ� ����.. �Ѥ�
	//		UB2UIWidgetBase* CastedUIWidgetBase = Cast<UB2UIWidgetBase>(ThisOne);
	//		UBladeIIUserWidget* CastedDJLegacyWidget = Cast<UBladeIIUserWidget>(ThisOne);
	//		check(CastedUIWidgetBase || CastedDJLegacyWidget);

	//		if (CastedUIWidgetBase) {
	//			CastedUIWidgetBase->DestroySelf(UB2UIManager::GetInstance());
	//		}
	//		else if (CastedDJLegacyWidget) {
	//			CastedDJLegacyWidget->DestroySelf();
	//		}
	//	}
	//}
	//AllPooledObjects.Empty();
	//	
	//MyOwner = nullptr;
}

UB2UnitedWidgetBase* FWidgetPoolBase::Rent(APlayerController* InPC, UObject* InRenter, UPanelWidget* InOptionalPlacePanel /*= nullptr*/)
{
	UB2UnitedWidgetBase* FoundOrCreated = nullptr;

	// ���� ������ �ڻ���� �˻�. ������ �ִ���
	for (TMap<UB2UnitedWidgetBase*, bool>::TIterator ItPO(AllPooledObjects); ItPO; ++ItPO)
	{
		UB2UnitedWidgetBase* ThisOne = ItPO.Key();
		if (ThisOne && !ThisOne->IsCurrentlyRent())
		{
			FoundOrCreated = ThisOne;
			break;
		}
	}

	if (!FoundOrCreated) // ���� pool �� ������ free ������Ʈ�� ����. �ڻ� ����.
	{
		FoundOrCreated = CreateSingleInternal(InPC);
	}
	checkSlow(FoundOrCreated);

	if (FoundOrCreated) // ���� �Ӵ��� ���
	{
		if (InOptionalPlacePanel)
		{
			// �� CanvasPanel �� �ʿ�� ����. CanvasPanel �̸� ���� Z-order �� �־��� �� �ִ� ����.
			UCanvasPanelSlot* AddedCP = Cast<UCanvasPanelSlot>(InOptionalPlacePanel->AddChild(FoundOrCreated));
			if (AddedCP && CreateZOrder >= 0)
			{ // CreateZOrder ���� �ǵ��ʹ� �� �ٸ��� �ѵ�..
				AddedCP->SetZOrder(CreateZOrder);
			}
		}

		FoundOrCreated->SetCurrentlyRent(true, InRenter);
		// �̹� Rent ���´� FloatingWidget �� ��� ������ ���� �ʿ�� ����. �ݳ��� �� Ȯ���Ϸ��� ����.
		bool* bFoundStatePtr = AllPooledObjects.Find(FoundOrCreated);
		checkSlow(bFoundStatePtr && *bFoundStatePtr == false);
		if (bFoundStatePtr)
		{
			*bFoundStatePtr = true;
		}
	}

	return FoundOrCreated;
}

void FWidgetPoolBase::TurnIn(class UB2UnitedWidgetBase* InUsedOne)
{
	if (InUsedOne)
	{
		// ��ǻ� �̰� TMap ���� �� ������ ���� ����. Rent ���δ� �� FloatingWidget �� ��� �����Ƿ�. ���� �̷��� Ȯ�ο�����..
		bool* bFoundStatePtr = AllPooledObjects.Find(InUsedOne);
		checkSlow(bFoundStatePtr && *bFoundStatePtr == true);
		if (bFoundStatePtr)
		{
			*bFoundStatePtr = false;
		}
		InUsedOne->SetCurrentlyRent(false, nullptr); // �ʿ��� �� ���� �ݳ��� ������ ��� �����ؼ� �˻�� ��
	}
}

//////////////////////////////////////////////////////////////////

UClass* FFloatingWidgetPool::GetDesiredClass()
{
	//if (OwnerUIManager)
	//{
	//	switch (WidgetClassType)
	//	{
	//	case EFloatingWidgetPoolClassType::EFWPCT_DamageNum_Mob: return OwnerUIManager->GetDamageNumWidgetClass_Mob();
	//	case EFloatingWidgetPoolClassType::EFWPCT_DamageNum_PC: return OwnerUIManager->GetDamageNumWidgetClass_PC();
	//	case EFloatingWidgetPoolClassType::EFWPCT_AbnormalEffect: return OwnerUIManager->GetAbnormalEffectWidgetClass();
	//	case EFloatingWidgetPoolClassType::EFWPCT_GoldAcq: return OwnerUIManager->GetFloatingGoldAcqWidgetClass();
	//	}
	//}
	//return UB2FloatingWidget::StaticClass(); // ������ �̰ɷδ� �ƹ��͵� ���� ���̴� ��
	return NULL;
}

void FFloatingWidgetPool::PreInitSetType(EFloatingWidgetPoolClassType InType)
{
	WidgetClassType = InType;
}

void FFloatingWidgetPool::InitPool(UObject* InOwnerObject, class APlayerController* InPC, int32 InPreCreateNum, int32 InCreateZOrder)
{
	check(WidgetClassType != EFloatingWidgetPoolClassType::EFWPCT_End); // �̰� call �ϱ� ���� WidgetClassType �����Ǿ� �־��.

	//// �̰� ���� �� �־�� ����� ������ ��.
	//OwnerUIManager = Cast<AB2UIManager_InGameCombat>(InOwnerObject);

	//// Z-order �� ���⼭ ���� �������,
	//Super::InitPool(InOwnerObject, InPC, InPreCreateNum, BII_WIDGET_ZORDER_FLOATING_DEFAULT);
	//	
	//check(OwnerUIManager == Cast<AB2UIManager_InGameCombat>(MyOwner));
}

UB2UnitedWidgetBase* FFloatingWidgetPool::Rent(class APlayerController* InPC, UObject* InRenter, UPanelWidget* InOptionalPlacePanel)
{
	checkSlow(!InOptionalPlacePanel); // Floating Widget �̶� �̷� �� ���� �ʵ���.
	
	UB2UnitedWidgetBase* RetObject = Super::Rent(InPC, InRenter, nullptr);
	//check(Cast<UB2FloatingWidget>(RetObject));
	return RetObject;
}

void FFloatingWidgetPool::DestroyAll()
{
	Super::DestroyAll();

	//OwnerUIManager = nullptr;
}

//////////////////////////////////////////////////////////////////

UClass* FInventoryDynItemIconPool::GetDesiredClass()
{
//	if (OwnerUIManager)
//	{
//		switch (WidgetClassType)
//		{
//		case EInventoryDynItemIconPoolClassType::StoredItem: return OwnerUIManager->GetInventoryItemIconClass(false);
//		case EInventoryDynItemIconPoolClassType::EquippedItem: return OwnerUIManager->GetInventoryItemIconClass(true);
//		}
//	}
//	return UB2DynItemIcon::StaticClass(); // ������ �̰ɷδ� �ƹ��͵� ���� ���̴� ��
return NULL;
}

void FInventoryDynItemIconPool::PreInitSetType(EInventoryDynItemIconPoolClassType InType)
{
	WidgetClassType = InType;
}

void FInventoryDynItemIconPool::InitPool(UObject* InOwnerObject, class APlayerController* InPC, int32 InPreCreateNum, int32 InCreateZOrder)
{
	//check(WidgetClassType != EInventoryDynItemIconPoolClassType::End); // �̰� call �ϱ� ���� WidgetClassType �����Ǿ� �־��.

	//OwnerUIManager = Cast<UB2UIManager_Lobby>(InOwnerObject);

	//Super::InitPool(InOwnerObject, InPC, InPreCreateNum, InCreateZOrder);

	//check(OwnerUIManager == Cast<UB2UIManager_Lobby>(MyOwner));
}

UB2UnitedWidgetBase* FInventoryDynItemIconPool::Rent(class APlayerController* InPC, UObject* InRenter, UPanelWidget* InOptionalPlacePanel)
{
	UB2UnitedWidgetBase* RetObject = Super::Rent(InPC, InRenter, InOptionalPlacePanel);
	return RetObject;
}

void FInventoryDynItemIconPool::TurnIn(UB2UnitedWidgetBase* InUsedOne)
{
	if (InUsedOne)
	{
		// DynItemIcon ��� Ư�� �� �������� ������ dummy �� �ٿ� ���� �� �ִ�.
		// ������ �ٿ��� �������� ���.
		InUsedOne->ConditionalClearDynCreatedDummyParent();
		InUsedOne->RemoveFromParent();
		// ���� �� WidgetPool �� UPROPERTY �� ������ ������ �̷��� �� �� �ٿ����� GC �� �ȵ� �� �ٵ� AddToViewport ���� �� ������ ���� �Ծ �̷� widget pool ȿ���� �ݰ���Ŵ.
		//InUsedOne->AddToViewport(CreateZOrder);
	}

	Super::TurnIn(InUsedOne);
}

void FInventoryDynItemIconPool::DestroyAll()
{
	Super::DestroyAll();

	//OwnerUIManager = nullptr;
}