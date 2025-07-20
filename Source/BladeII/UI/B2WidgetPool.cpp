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
	// 당연히 이걸 그대로 사용하면 아무것도 되는 게 없을 꺼다. 실제로는 Widget Blueprint 클래스를 리턴해야..
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
		// 다른 panel 에 AddChild 해야한다면 Rent 시점에..
		// 그냥 AddToViewport 해서 사용하는 경우도 Rent 시점에 바꿀 수 있겠지만 그런 사용에서는 처음 Z-order 를 유지하는 게 일반적일 것 같아 굳이 변경 가능하게 제공 안 함.
		NewCreated->AddToViewport(CreateZOrder); 
		NewCreated->SetAsPooledObject(); // 생성 직후부터 rent 는 아님.
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
	//		// 각 UI 타입별 따로.. ㅡㅡ
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

	// 현재 보유한 자산부터 검색. 공실이 있는지
	for (TMap<UB2UnitedWidgetBase*, bool>::TIterator ItPO(AllPooledObjects); ItPO; ++ItPO)
	{
		UB2UnitedWidgetBase* ThisOne = ItPO.Key();
		if (ThisOne && !ThisOne->IsCurrentlyRent())
		{
			FoundOrCreated = ThisOne;
			break;
		}
	}

	if (!FoundOrCreated) // 현재 pool 이 보유한 free 오브젝트가 없음. 자산 증식.
	{
		FoundOrCreated = CreateSingleInternal(InPC);
	}
	checkSlow(FoundOrCreated);

	if (FoundOrCreated) // 본격 임대차 계약
	{
		if (InOptionalPlacePanel)
		{
			// 꼭 CanvasPanel 일 필요는 없다. CanvasPanel 이면 따로 Z-order 를 넣어줄 수 있는 정도.
			UCanvasPanelSlot* AddedCP = Cast<UCanvasPanelSlot>(InOptionalPlacePanel->AddChild(FoundOrCreated));
			if (AddedCP && CreateZOrder >= 0)
			{ // CreateZOrder 원래 의도와는 좀 다르긴 한데..
				AddedCP->SetZOrder(CreateZOrder);
			}
		}

		FoundOrCreated->SetCurrentlyRent(true, InRenter);
		// 이미 Rent 상태는 FloatingWidget 이 들고 있으니 굳이 필요는 없다. 반납할 때 확인하려는 정도.
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
		// 사실상 이걸 TMap 으로 할 이유는 별로 없다. Rent 여부는 각 FloatingWidget 이 들고 있으므로. 거의 이렇게 확인용으로..
		bool* bFoundStatePtr = AllPooledObjects.Find(InUsedOne);
		checkSlow(bFoundStatePtr && *bFoundStatePtr == true);
		if (bFoundStatePtr)
		{
			*bFoundStatePtr = false;
		}
		InUsedOne->SetCurrentlyRent(false, nullptr); // 필요할 거 같음 반납시 빌려간 사람 전달해서 검사라도 ㅋ
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
	//return UB2FloatingWidget::StaticClass(); // 실제로 이걸로는 아무것도 못할 것이다 ㅋ
	return NULL;
}

void FFloatingWidgetPool::PreInitSetType(EFloatingWidgetPoolClassType InType)
{
	WidgetClassType = InType;
}

void FFloatingWidgetPool::InitPool(UObject* InOwnerObject, class APlayerController* InPC, int32 InPreCreateNum, int32 InCreateZOrder)
{
	check(WidgetClassType != EFloatingWidgetPoolClassType::EFWPCT_End); // 이거 call 하기 전에 WidgetClassType 지정되어 있어야.

	//// 이거 먼저 해 주어야 제대로 생성이 됨.
	//OwnerUIManager = Cast<AB2UIManager_InGameCombat>(InOwnerObject);

	//// Z-order 는 여기서 따로 명시해줌,
	//Super::InitPool(InOwnerObject, InPC, InPreCreateNum, BII_WIDGET_ZORDER_FLOATING_DEFAULT);
	//	
	//check(OwnerUIManager == Cast<AB2UIManager_InGameCombat>(MyOwner));
}

UB2UnitedWidgetBase* FFloatingWidgetPool::Rent(class APlayerController* InPC, UObject* InRenter, UPanelWidget* InOptionalPlacePanel)
{
	checkSlow(!InOptionalPlacePanel); // Floating Widget 이라 이런 거 주지 않도록.
	
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
//	return UB2DynItemIcon::StaticClass(); // 실제로 이걸로는 아무것도 못할 것이다 ㅋ
return NULL;
}

void FInventoryDynItemIconPool::PreInitSetType(EInventoryDynItemIconPoolClassType InType)
{
	WidgetClassType = InType;
}

void FInventoryDynItemIconPool::InitPool(UObject* InOwnerObject, class APlayerController* InPC, int32 InPreCreateNum, int32 InCreateZOrder)
{
	//check(WidgetClassType != EInventoryDynItemIconPoolClassType::End); // 이거 call 하기 전에 WidgetClassType 지정되어 있어야.

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
		// DynItemIcon 사용 특성 상 동적으로 생성한 dummy 에 붙여 있을 수 있다.
		// 기존에 붙였던 곳에서는 떼어냄.
		InUsedOne->ConditionalClearDynCreatedDummyParent();
		InUsedOne->RemoveFromParent();
		// 만일 이 WidgetPool 을 UPROPERTY 로 만들지 않으면 이렇게 딴 데 붙여놔야 GC 가 안됨 ㅋ 근데 AddToViewport 같은 게 성능을 많이 먹어서 이런 widget pool 효과를 반감시킴.
		//InUsedOne->AddToViewport(CreateZOrder);
	}

	Super::TurnIn(InUsedOne);
}

void FInventoryDynItemIconPool::DestroyAll()
{
	Super::DestroyAll();

	//OwnerUIManager = nullptr;
}