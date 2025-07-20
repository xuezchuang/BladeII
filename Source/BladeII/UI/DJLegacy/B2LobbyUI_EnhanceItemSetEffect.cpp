// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_EnhanceItemSetEffect.h"
//#include "BladeII.h"

#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "CharacterDataStore.h"


#include "BladeIIGameImpl.h"

UB2LobbyUI_EnhanceItemSetEffect::UB2LobbyUI_EnhanceItemSetEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2LobbyUI_EnhanceItemSetEffect::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title)
	GET_SLOT(UB2RichTextBlock, RTB_SetEffectNotice)
	GET_SLOT(UTextBlock, TB_EnhanceLevel)
	GET_SLOT(UTextBlock, TB_NextEnhanceLevel)
	GET_SLOT(UTextBlock, TB_AttackPoint)
	GET_SLOT(UTextBlock, TB_NextAttackPoint)
	GET_SLOT(UTextBlock, TB_DefensePoint)
	GET_SLOT(UTextBlock, TB_NextDefensePoint)
	GET_SLOT(UTextBlock, TB_HealthPoint)
	GET_SLOT(UTextBlock, TB_NextHealthPoint)
	GET_SLOT(UTextBlock, TB_DeltaAttackPoint)
	GET_SLOT(UTextBlock, TB_DeltaDefensePoint)
	GET_SLOT(UTextBlock, TB_DeltaHealthPoint)

	GET_SLOT(UTextBlock, STB_EnhanceLevel)
	GET_SLOT(UTextBlock, STB_CurrentValue)
	GET_SLOT(UTextBlock, STB_NextValue)
	GET_SLOT(UTextBlock, STB_Attck)
	GET_SLOT(UTextBlock, STB_Defense)
	GET_SLOT(UTextBlock, STB_Health)

	GET_SLOT(UCanvasPanel, CP_MaxEnhanceLevel)
	GET_SLOT(UTextBlock, TB_MaxEnhanceLevelDesc)
}

void UB2LobbyUI_EnhanceItemSetEffect::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	// 窜鉴洒 酒捞袍 沥焊父 掘绢坷妨搁 Inventory 牢胶畔胶 绝捞 static 栏肺 掘绢棵 荐 乐栏唱 碍拳 扁瓷篮 牢亥配府啊 栋 乐绰 惑怕俊辑 悼累.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);
	
	UpdateStaticTexts();
	UpdateEnhanceItemSetEffect();
}

void UB2LobbyUI_EnhanceItemSetEffect::UpdateStaticTexts()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhanceSetEffect_Title")));
	if (STB_EnhanceLevel.IsValid())
		STB_EnhanceLevel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhanceSetEffect_EnhanceLevel")));
	if (STB_CurrentValue.IsValid())
		STB_CurrentValue->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhanceSetEffect_CurStat")));
	if (STB_NextValue.IsValid())
		STB_NextValue->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhanceSetEffect_NextStat")));
	if (STB_Attck.IsValid())
		STB_Attck->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Attack")));
	if (STB_Defense.IsValid())
		STB_Defense->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Defense")));
	if (STB_Health.IsValid())
		STB_Health->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Health")));
	if(TB_MaxEnhanceLevelDesc.IsValid())
		TB_MaxEnhanceLevelDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhanceSetEffect_MaxEnhanceLevel")));
}

void UB2LobbyUI_EnhanceItemSetEffect::DestroySelf()
{
	Super::DestroySelf();
}

void UB2LobbyUI_EnhanceItemSetEffect::UpdateEnhanceItemSetEffect()
{
	EPCClass CurrentPC = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;

	UpdateDynamicStaticTexts(CurrentPC);
}

void UB2LobbyUI_EnhanceItemSetEffect::UpdateDynamicStaticTexts(EPCClass CurrentPC)
{
	BII_CHECK(CachedInventory);

	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto& LocalCharacterDataStore = BladeIIGameImpl::GetLocalCharacterData();
	
	// 利侩 饭骇 棺 啊吝摹蔼 犬牢
	const int32 MinEnhanceLevelSetEffect = ClientDataStore.GetMinEnhanceLevelForSetEffect();
	int32 CurEnhanceEffectLevel = LocalCharacterDataStore.GetEnhanceSetEffectLevel(CurrentPC);
	bool IsAcceptEnhanceEffectLevel = CurEnhanceEffectLevel != 0;
	double CurEnhanceEffectFactor = (IsAcceptEnhanceEffectLevel ? LocalCharacterDataStore.GetEnhanceSetEffectFactor(CurrentPC) : 0.f);
	int32 NextEnhanceEffectLevel = (IsAcceptEnhanceEffectLevel ? CurEnhanceEffectLevel + 1 : MinEnhanceLevelSetEffect);	// 泅犁 饭骇捞 弥家饭骇阑 档崔窍瘤 给沁阑 版快 弥家饭骇捞 Next促!
	bool IsMaxLevel = ClientDataStore.GetMaxEnhanceLevelForSetEffect() <= CurEnhanceEffectLevel;
	double NextEnhanceEffectFactor = ClientDataStore.GetEnhanceItemSetEffect(NextEnhanceEffectLevel);	
	
	double DeltaEnhanceEffectFactor = (NextEnhanceEffectFactor - CurEnhanceEffectFactor);

	if (RTB_SetEffectNotice.IsValid())
	{
		RTB_SetEffectNotice->SetText(
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhanceSetEffect_Notice")), FText::AsNumber(MinEnhanceLevelSetEffect), FText::AsNumber(MIN_COUNT_ENHANCE_ITEM_SET_EFFECT)));
	}

	if (TB_EnhanceLevel.IsValid())
		TB_EnhanceLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel")), FText::AsNumber(CurEnhanceEffectLevel)));
	if (TB_NextEnhanceLevel.IsValid())
		TB_NextEnhanceLevel->SetText(IsMaxLevel ? FText::FromString(TEXT("")) : FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel")), FText::AsNumber(NextEnhanceEffectLevel)));

	if (TB_AttackPoint.IsValid())
		TB_AttackPoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Percent")), FText::AsNumber(CurEnhanceEffectFactor)));
	if (TB_NextAttackPoint.IsValid())
		TB_NextAttackPoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Percent")), FText::AsNumber(NextEnhanceEffectFactor)));
	if (TB_DefensePoint.IsValid())
		TB_DefensePoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Percent")), FText::AsNumber(CurEnhanceEffectFactor)));
	if (TB_NextDefensePoint.IsValid())
		TB_NextDefensePoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Percent")), FText::AsNumber(NextEnhanceEffectFactor)));
	if (TB_HealthPoint.IsValid())
		TB_HealthPoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Percent")), FText::AsNumber(CurEnhanceEffectFactor)));
	if (TB_NextHealthPoint.IsValid())
		TB_NextHealthPoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Percent")), FText::AsNumber(NextEnhanceEffectFactor)));

	if (TB_DeltaAttackPoint.IsValid())
		TB_DeltaAttackPoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Percent")), FText::AsNumber(DeltaEnhanceEffectFactor)));
	if (TB_DeltaDefensePoint.IsValid())
		TB_DeltaDefensePoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Percent")), FText::AsNumber(DeltaEnhanceEffectFactor)));
	if (TB_DeltaHealthPoint.IsValid())
		TB_DeltaHealthPoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Percent")), FText::AsNumber(DeltaEnhanceEffectFactor)));
	if (CP_MaxEnhanceLevel.IsValid())
		CP_MaxEnhanceLevel->SetVisibility(IsMaxLevel ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2LobbyUI_EnhanceItemSetEffect::OnCloseButtonPressed()
{
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EnhanceItemSetEffectPopup);
}

void UB2LobbyUI_EnhanceItemSetEffect::CloseWidgetDelegate()
{
	OnCloseButtonPressed();
}