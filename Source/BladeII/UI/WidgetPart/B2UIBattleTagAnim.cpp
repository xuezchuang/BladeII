// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIBattleTagAnim.h"
#include "B2UIManager.h"

#include "B2UIChatting.h"
#include "B2UIManager.h"

#include "BladeIIGameImpl.h"

UMaterialInterface* FUITagAnimSetupPerPCClass::GetMainPic_1()
{
	if (!MainPic_1.IsValid() && MainPic_1.ToString().Len() > 0)
	{
		MainPic_1.LoadSynchronous();
	}
	return MainPic_1.Get();
}
UMaterialInterface* FUITagAnimSetupPerPCClass::GetMainPic_2()
{
	if (!MainPic_2.IsValid() && MainPic_2.ToString().Len() > 0)
	{
		MainPic_2.LoadSynchronous();
	}
	return MainPic_2.Get();
}

void FUITagAnimWidgetCollectionPerPCClass::CacheAssetsForChar(UB2UIWidget* InOwnerWidget, EPCClass InCharClass)
{
	if (InOwnerWidget && InCharClass != EPCClass::EPC_End)
	{
		// 여기서 하는 거에 네이밍을 맞춰야 함.
		const FString NamePostfix = GetWidgetNamePostFix(InCharClass);

		// 얘들은 UMG 에서는 material 할당이 없이 비어 있어야 한다. 메모리 아끼려고 여기서 FUITagAnimSetupPerPCClass 통해 TAsset 셋업하는데 UMG 에서 세팅을 해 놓으면 첨부터 메모리 로드되어 소용이 없다.
		IMG_Main_01 = InOwnerWidget->GetSlot<UImage>(FName( *FString::Printf(TEXT("IMG_Main_01_%s"), *NamePostfix)));
		IMG_Main_02 = InOwnerWidget->GetSlot<UImage>(FName(*FString::Printf(TEXT("IMG_Main_02_%s"), *NamePostfix)));
		TB_CharName = InOwnerWidget->GetSlot<UTextBlock>(FName(*FString::Printf(TEXT("TB_CharName_%s"), *NamePostfix)));
	}
}

void FUITagAnimWidgetCollectionPerPCClass::UpdateWidgets(EPCClass InCharClass, FUITagAnimSetupPerPCClass* InResoureSetup)
{
	if (InResoureSetup)
	{
		UMaterialInterface* MtrlForMain01 = InResoureSetup->GetMainPic_1();
		if (IMG_Main_01.IsValid() && MtrlForMain01)
		{
			IMG_Main_01->SetBrushFromMaterial(MtrlForMain01);
		}
		// 2 번은 캐릭터에 따라 없을 수도 있다.
		UMaterialInterface* MtrlForMain02 = InResoureSetup->GetMainPic_2();
		if (IMG_Main_02.IsValid() && MtrlForMain02)
		{
			IMG_Main_02->SetBrushFromMaterial(MtrlForMain02);
		}
	}

	if (TB_CharName.IsValid() && InCharClass != EPCClass::EPC_End)
	{
		TB_CharName->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(InCharClass));
	}
}

UB2UIBattleTagAnim::UB2UIBattleTagAnim(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TotalRunningPeriod = 3.0f;
	bEventsSubscribed = false;
	StartPauseMenuTicket = -1;
	StopPauseMenuTicket = -1;
}


FUITagAnimSetupPerPCClass* UB2UIBattleTagAnim::GetResourceSetup(EPCClass InCharClass)
{
	if (InCharClass != EPCClass::EPC_End)
	{
		return (InCharClass == EPCClass::EPC_Gladiator) ? &ResourceSetup_Gladiator :
			((InCharClass == EPCClass::EPC_Assassin) ? &ResourceSetup_Assassin :
			((InCharClass == EPCClass::EPC_Wizard) ? &ResourceSetup_Wizard : &ResourceSetup_Fighter));
	}
	return NULL;
}

FUITagAnimWidgetCollectionPerPCClass* UB2UIBattleTagAnim::GetWidgetSetForChar(EPCClass InCharClass)
{
	if (InCharClass != EPCClass::EPC_End)
	{
		// CacheAssets 할 때에 순서대로 함. 꺼내올 때도 순서대로.
		int32 CastedPCClassInt = PCClassToInt(InCharClass);
		if (CachedWidgetSets.IsValidIndex(CastedPCClassInt))
		{
			return &CachedWidgetSets[CastedPCClassInt];
		}
	}
	return NULL;
}

void UB2UIBattleTagAnim::BeginTagAnim(EPCClass InCharClass, bool bIsTagAttack)
{
	if (InCharClass != EPCClass::EPC_End && !bIsTagAttack) // bIsTagAttack 연계 태그에서는 이거 안함. 나중에 바뀔 수도 있으니 일단 인자로는 받아두고..
	{
		int32 CastedClassInt = PCClassToInt(InCharClass);
		if (WS_TagEffectPerPCSet.IsValid())
		{
			WS_TagEffectPerPCSet->SetActiveWidgetIndex(CastedClassInt);
		}
		
		// 캐릭터 클래스에 따라 기타 잡다한 widget 리소스 적절히 셋업을..
		FUITagAnimSetupPerPCClass* DesiredResourceSet = GetResourceSetup(InCharClass);
		FUITagAnimWidgetCollectionPerPCClass* DesiredWidgetSet = GetWidgetSetForChar(InCharClass);
		if (DesiredWidgetSet)
		{
			DesiredWidgetSet->UpdateWidgets(InCharClass, DesiredResourceSet);
		}

		// 블루프린트 쪽의 애니메이션 플레이 호출
		BeginTagAnimBP(InCharClass, bIsTagAttack);

		UWorld* TheWorld = GetWorld();
		if (TotalRunningPeriod > 0.0f && TheWorld)
		{
			TheWorld->GetTimerManager().SetTimer(TotalPeriodExpireTH, this, &UB2UIBattleTagAnim::TotalPeriodExpireCB, TotalRunningPeriod, false);
		}
	}

	// 노출 시점에 채팅 떠있으면 히든처리
	if (UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting))
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIBattleTagAnim::CacheAssets()
{
	Super::CacheAssets();
	
	GET_SLOT(UWidgetSwitcher, WS_TagEffectPerPCSet);

	// 순서에 맞춰 넣는다. GetWidgetSetForChar 에서도 이런 가정으로.
	CachedWidgetSets.Empty();
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		EPCClass CastedPCClass = IntToPCClass(PCI);

		FUITagAnimWidgetCollectionPerPCClass ThisSet;
		ThisSet.CacheAssetsForChar(this, CastedPCClass);
		CachedWidgetSets.Add(ThisSet);
	}

}

void UB2UIBattleTagAnim::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIBattleTagAnim::Init()
{
	Super::Init();

	SubscribeEvents();
}

void UB2UIBattleTagAnim::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	UnsubscribeEvents();
}

void UB2UIBattleTagAnim::SubscribeEvents()
{
	if (bEventsSubscribed) {
		return;
	}

	CAPTURE_UOBJECT(UB2UIBattleTagAnim);

	StartPauseMenuTicket = StartPauseMenuClass<EB2GameMode>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(StartPauseMenu, EB2GameMode gm)
			Capture->OnPause(true);
		END_CAPTURE_OBJECT()
	);
	StopPauseMenuTicket = StopPauseMenuClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(StopPauseMenu)
			Capture->OnPause(false);
		END_CAPTURE_OBJECT()
	);

	StartVisibleChatPopupTicket = StartVisibleChatPopupClass<EB2GameMode>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(StartVisibleChatPopup, EB2GameMode gm)
			Capture->OnPause(true);
		END_CAPTURE_OBJECT()
	);
	StopVisibleChatPopupTicket = StopVisibleChatPopupClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(StopVisibleChatPopup)
			Capture->OnPause(false);
		END_CAPTURE_OBJECT()
	);

	bEventsSubscribed = true;
}
void UB2UIBattleTagAnim::UnsubscribeEvents()
{
	if (!bEventsSubscribed) {
		return;
	}

	StartPauseMenuClass<EB2GameMode>::GetInstance().Unsubscribe(StartPauseMenuTicket);
	StopPauseMenuClass<>::GetInstance().Unsubscribe(StopPauseMenuTicket);

	StartVisibleChatPopupClass<EB2GameMode>::GetInstance().Unsubscribe(StartVisibleChatPopupTicket);
	StopVisibleChatPopupClass<>::GetInstance().Unsubscribe(StopVisibleChatPopupTicket);

	bEventsSubscribed = false;
}

void UB2UIBattleTagAnim::OnPause(bool bPaused)
{
	// 별도의 layer 에 동적 생성되는 애라 pause 되어도 가려지지 않아서 따로 숨길 필요가 있다. 여기선 다른 조건으로 visibility 제어하는 게 없으니 걍 이렇게
	this->SetVisibility(bPaused ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
}

void UB2UIBattleTagAnim::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// 혹 타이머 직접 돌릴 일 있으면?
}

void UB2UIBattleTagAnim::TotalPeriodExpireCB()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TotalPeriodExpireTH);
	}

	// 나를 생성한 owner 쪽에 destroy 신호를 보낸다.
	OnTotalPeriodExpireDelegate.ExecuteIfBound(this);
}