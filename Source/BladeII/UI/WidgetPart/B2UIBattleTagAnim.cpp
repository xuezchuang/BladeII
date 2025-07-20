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
		// 咯扁辑 窍绰 芭俊 匙捞怪阑 嘎苗具 窃.
		const FString NamePostfix = GetWidgetNamePostFix(InCharClass);

		// 娟甸篮 UMG 俊辑绰 material 且寸捞 绝捞 厚绢 乐绢具 茄促. 皋葛府 酒尝妨绊 咯扁辑 FUITagAnimSetupPerPCClass 烹秦 TAsset 悸诀窍绰单 UMG 俊辑 技泼阑 秦 初栏搁 梅何磐 皋葛府 肺靛登绢 家侩捞 绝促.
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
		// 2 锅篮 某腐磐俊 蝶扼 绝阑 荐档 乐促.
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
		// CacheAssets 且 锭俊 鉴辑措肺 窃. 波郴棵 锭档 鉴辑措肺.
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
	if (InCharClass != EPCClass::EPC_End && !bIsTagAttack) // bIsTagAttack 楷拌 怕弊俊辑绰 捞芭 救窃. 唱吝俊 官拆 荐档 乐栏聪 老窜 牢磊肺绰 罐酒滴绊..
	{
		int32 CastedClassInt = PCClassToInt(InCharClass);
		if (WS_TagEffectPerPCSet.IsValid())
		{
			WS_TagEffectPerPCSet->SetActiveWidgetIndex(CastedClassInt);
		}
		
		// 某腐磐 努贰胶俊 蝶扼 扁鸥 棱促茄 widget 府家胶 利例洒 悸诀阑..
		FUITagAnimSetupPerPCClass* DesiredResourceSet = GetResourceSetup(InCharClass);
		FUITagAnimWidgetCollectionPerPCClass* DesiredWidgetSet = GetWidgetSetForChar(InCharClass);
		if (DesiredWidgetSet)
		{
			DesiredWidgetSet->UpdateWidgets(InCharClass, DesiredResourceSet);
		}

		// 喉风橇赴飘 率狼 局聪皋捞记 敲饭捞 龋免
		BeginTagAnimBP(InCharClass, bIsTagAttack);

		UWorld* TheWorld = GetWorld();
		if (TotalRunningPeriod > 0.0f && TheWorld)
		{
			TheWorld->GetTimerManager().SetTimer(TotalPeriodExpireTH, this, &UB2UIBattleTagAnim::TotalPeriodExpireCB, TotalRunningPeriod, false);
		}
	}

	// 畴免 矫痢俊 盲泼 栋乐栏搁 洒电贸府
	if (UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting))
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIBattleTagAnim::CacheAssets()
{
	Super::CacheAssets();
	
	GET_SLOT(UWidgetSwitcher, WS_TagEffectPerPCSet);

	// 鉴辑俊 嘎苗 持绰促. GetWidgetSetForChar 俊辑档 捞繁 啊沥栏肺.
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
	// 喊档狼 layer 俊 悼利 积己登绰 局扼 pause 登绢档 啊妨瘤瘤 臼酒辑 蝶肺 见辨 鞘夸啊 乐促. 咯扁急 促弗 炼扒栏肺 visibility 力绢窍绰 霸 绝栏聪 傲 捞犯霸
	this->SetVisibility(bPaused ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
}

void UB2UIBattleTagAnim::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// 趣 鸥捞赣 流立 倒副 老 乐栏搁?
}

void UB2UIBattleTagAnim::TotalPeriodExpireCB()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TotalPeriodExpireTH);
	}

	// 唱甫 积己茄 owner 率俊 destroy 脚龋甫 焊辰促.
	OnTotalPeriodExpireDelegate.ExecuteIfBound(this);
}