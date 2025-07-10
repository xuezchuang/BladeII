#include "B2UIMsgPopupTooltipSkillOption.h"
#include "B2UISkillOptionText.h"
#include "B2UIManager.h"
#include "B2PCClassInfoBox.h"
#include "B2SkillInfo.h"
#include "B2UISkillOptionList.h"
#include "Event.h"


#include "BladeIIGameImpl.h"
#include "B2GuildNPCInfo.h"

UB2UIMsgPopupTooltipSkillOption::UB2UIMsgPopupTooltipSkillOption(const FObjectInitializer& ObjectInitializer)
	:	Super(ObjectInitializer)
{

}

void UB2UIMsgPopupTooltipSkillOption::DestroySelf(class UB2UIManager* InUIManager)
{
	CloseSkillTooltipClass<int32>::GetInstance().Signal(CurrentSkillID);

	Super::DestroySelf(InUIManager);	
}

void UB2UIMsgPopupTooltipSkillOption::SetSkillID(int32 SkillId)
{
	if (CurrentSkillID == SkillId)
		return;

	CurrentSkillID = SkillId;

	auto* PCClassInfoBox = StaticFindPCClassInfoBox();
	auto* AllSkillInfo = PCClassInfoBox ? PCClassInfoBox->GetAllSkillInfo() : nullptr;

	if (AllSkillInfo)
	{
		// LoadSynchronous 를 하려니 const 를 없애야..
		FSingleSkillInfo* SkillInfo = const_cast<FSingleSkillInfo*>(AllSkillInfo->GetSingleInfoOfID(SkillId));
		if (SkillInfo)
		{

			SetSkillType(SkillInfo->SkillType);
			if (IMG_SkillIcon.IsValid())
			{
				auto* SkillIconMat = SkillInfo->ButtonIcon_Normal.LoadSynchronous();
				IMG_SkillIcon->SetVisibility(SkillIconMat ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
				IMG_SkillIcon->SetBrushFromMaterial(Cast<UMaterialInterface>(SkillIconMat));
			}

			if (TB_SkillName.IsValid())
			{
				TB_SkillName->SetText(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::SkillName, SkillId)));
			}

			if (TB_SkillDesc_Text.IsValid())
			{
				TB_SkillDesc_Text->SetText(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::SkillDesc, SkillId)));
			}

			int32 SkillLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterSkillLevel(SkillId);

			if (OverlayOptionList.IsValid())
				OverlayOptionList->SetVisibility(SkillLevel < 0 ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);

			if (VB_OptionList.IsValid())
			{
				VB_OptionList->ClearChildren();				
							
				if (SkillLevel > 0)
				{
					auto* CreatedWidget = CreateWidget<UB2UISkillOptionList>(GetOwningPlayer(), SkillOptionListClass);
					if (CreatedWidget != NULL)
					{
						auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_OptionList->AddChild(CreatedWidget));
						CreatedWidget->Init();
						CreatedWidget->SetSkillOptionType(ESkillOptionTitleType::CurrentSkillOption);
						CreatedWidget->InitializeList(SkillId, SkillLevel);
					}
				}

				if (0 <= SkillLevel && SkillLevel < BladeIIGameImpl::GetClientDataStore().GetMaxSkillLevel(SkillId))
				{
					auto* CreatedWidget = CreateWidget<UB2UISkillOptionList>(GetOwningPlayer(), SkillOptionListClass);
					if (CreatedWidget != NULL)
					{
						auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_OptionList->AddChild(CreatedWidget));
						CreatedWidget->Init();
						CreatedWidget->SetSkillOptionType(ESkillOptionTitleType::NextSkillOption);
						CreatedWidget->InitializeList(SkillId, SkillLevel + 1, true);
					}
				}
				int32 SpecialOptionLevel = INDEX_NONE;
				if (HasNextSpecialDisplayOption(SkillId, SkillLevel, SpecialOptionLevel))
				{
					auto* CreatedWidget = CreateWidget<UB2UISkillOptionList>(GetOwningPlayer(), SkillOptionListClass);
					if (CreatedWidget != NULL)
					{
						auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_OptionList->AddChild(CreatedWidget));
						CreatedWidget->Init();
						CreatedWidget->SetSkillOptionType(ESkillOptionTitleType::SpecialOption, SpecialOptionLevel);
						CreatedWidget->SetSpecialOptionList(SkillId, SpecialOptionLevel);
					}
				}
			}
		}
	}
}

void UB2UIMsgPopupTooltipSkillOption::SetUnitySkillID(int32 SkillId, EPCClass eMainClass)
{
	if (CurrentSkillID == SkillId)
		return;

	CurrentSkillID = SkillId;

	auto* PCClassInfoBox = StaticFindPCClassInfoBox();
	auto* AllSkillInfo = PCClassInfoBox ? PCClassInfoBox->GetAllSkillInfo() : nullptr;

	if (AllSkillInfo)
	{
		// LoadSynchronous 를 하려니 const 를 없애야..
		FSingleSkillInfo* SkillInfo = const_cast<FSingleSkillInfo*>(AllSkillInfo->GetSingleInfoOfID(SkillId));
		if (SkillInfo)
		{

			if (P_Skill_Badge.IsValid())
				P_Skill_Badge->SetVisibility(ESlateVisibility::Collapsed);
			if (HB_SkillLevel.IsValid())
				HB_SkillLevel->SetVisibility(ESlateVisibility::Collapsed);

			if (IMG_SkillIcon.IsValid())
			{
				auto* SkillIconMat = SkillInfo->ButtonIcon_Normal.LoadSynchronous();
				IMG_SkillIcon->SetVisibility(SkillIconMat ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
				IMG_SkillIcon->SetBrushFromMaterial(Cast<UMaterialInterface>(SkillIconMat));
			}

			if (TB_SkillName.IsValid())
			{
				TB_SkillName->SetText(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::SkillName, SkillId)));
			}

			if (TB_SkillDesc_Text.IsValid())
			{
				TB_SkillDesc_Text->SetText(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::SkillDesc, SkillId)));
			}

			int32 SkillLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterSkillLevel(SkillId);

			if (OverlayOptionList.IsValid())
				OverlayOptionList->SetVisibility(ESlateVisibility::HitTestInvisible);

			if (VB_OptionList.IsValid())
			{
				VB_OptionList->ClearChildren();

				auto* CreatedWidget = CreateWidget<UB2UISkillOptionList>(GetOwningPlayer(), SkillOptionListClass);
				if (CreatedWidget != NULL)
				{
					auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_OptionList->AddChild(CreatedWidget));
					CreatedWidget->Init();
					CreatedWidget->SetSkillOptionType(ESkillOptionTitleType::CurrentSkillOption);
					CreatedWidget->InitializeUnitySkillList(SkillId, eMainClass);
				}
			}
		}
	}
}

int32 UB2UIMsgPopupTooltipSkillOption::GetSkillID()
{
	return CurrentSkillID;
}

void UB2UIMsgPopupTooltipSkillOption::SetSkillLevel(int32 InSkillLv, int32 InSkillMaxLevel)
{
	if (HB_SkillLevel.IsValid())
		HB_SkillLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (TB_SkillLevel.IsValid())
	{
		TB_SkillLevel->SetText(FText::AsNumber(InSkillLv));
		TB_SkillLevel->SetColorAndOpacity(InSkillLv == 0 ? FSlateColor(FLinearColor::White) : SkillLevelColor);
	}
	
	if (TB_SkillMaxLevel.IsValid())
	{
		TB_SkillMaxLevel->SetText(FText::Format(FText::FromString(TEXT("/{0}")), FText::AsNumber(InSkillMaxLevel)));
	}
}

void UB2UIMsgPopupTooltipSkillOption::SetSkillType(ESkillType InSkillType)
{
	if (P_Skill_Badge.IsValid())
		P_Skill_Badge->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (TB_SkillType.IsValid())
	{
		switch (InSkillType)
		{
		case ESkillType::ESkillType_Attack:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Attack")));
			break;
		case ESkillType::ESkillType_Defense:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Defense")));
			break;
		case ESkillType::ESkillType_Survive:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Survive")));
			break;
		case ESkillType::ESkillType_Buff:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Buff")));
			break;
		case ESkillType::ESkillType_Counter:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Counter")));
			break;
		case ESkillType::ESkillType_Tag:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Tag")));
			break;
		case ESkillType::ESkillType_Professional:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Mercenary_Talent")));
			break;
		}

		TB_SkillType->SetColorAndOpacity(FSkillBadgeImage::GetSkillBadgeFontColor(SkillBadgeImages, InSkillType));
	}

	if (IMG_Skill_Badge.IsValid())
	{
		auto* SkillBadgeImage = FSkillBadgeImage::GetSkillBadgeImage(SkillBadgeImages, InSkillType);
		if (SkillBadgeImage)
			IMG_Skill_Badge->SetBrushFromMaterial(SkillBadgeImage);
	}
}

void UB2UIMsgPopupTooltipSkillOption::SetMercenarySkillID(int32 InSkillID)
{
	HB_SkillLevel->SetVisibility(ESlateVisibility::Hidden);
	VB_OptionList->ClearChildren();

	UB2GuildNPCInfo* NPCInfo = StaticFindGuildNPCInfo();
	const FGuildMercenarySkill* ResourceInfo = NPCInfo->GetMercenarySkillOfID(InSkillID);

	if (IMG_SkillIcon.IsValid())
		IMG_SkillIcon->SetBrushFromMaterial(Cast<UMaterialInstance>(ResourceInfo->SkillImageNormal.LoadSynchronous()));


	if (TB_SkillName.IsValid())
		TB_SkillName->SetText(BladeIIGetLOCText(B2LOC_CAT_GUILDNPCSKILL, GetGuildNPCSkillLocalizedTextKey(EGuildNPCSkillTextDataType::SkillName, InSkillID)));

	if (TB_SkillDesc_Text.IsValid())
		TB_SkillDesc_Text->SetText(BladeIIGetLOCText(B2LOC_CAT_GUILDNPCSKILL, GetGuildNPCSkillLocalizedTextKey(EGuildNPCSkillTextDataType::SkillNameDesc, InSkillID)));

	SetSkillType(ResourceInfo->SkillType);
}

void UB2UIMsgPopupTooltipSkillOption::SetMercenarySpecialSKillID(int32 InSkillID, int32 InLv, ENPCSpecialSKill InSkillType)
{

	FGuildDataStore& GuildData = BladeIIGameImpl::GetGuildDataStore();
	FB2GuildMercenaryInfo GuildMasterData = GuildData.GetGuildMercenaryMasterData();

	b2network::B2mdGuildMercenaryMasteryPtr MercenaryMasteries = GuildMasterData.GetMercenaryMasteries(InSkillID, InLv);	//레벨업위한경험치, 전문화기술 뭐있나 가져오기

	check(MercenaryMasteries)

	UB2GuildNPCInfo* NPCInfo = StaticFindGuildNPCInfo();
	const FGuildNPCInfo* ResourceInfo = NPCInfo->GetGuildNPCInfoOfID(InSkillID);

	SetSkillType(ResourceInfo->SpecialType);

	VB_OptionList->ClearChildren();

	if(TB_SkillLevel.IsValid())
		TB_SkillLevel->SetText(FText::AsNumber(InLv));

	if (TB_SkillMaxLevel.IsValid())
		TB_SkillMaxLevel->SetText(FText::Format(FText::FromString(TEXT("/{0}")), FText::AsNumber(GuildMasterData.GetMercenaryMasteriesCount(InSkillID))));

	if (IMG_SkillIcon.IsValid() && TB_SkillName.IsValid() && TB_SkillDesc_Text.IsValid())
	{
		if (InSkillType == ENPCSpecialSKill::Option1)
		{
			IMG_SkillIcon->SetBrushFromMaterial(Cast<UMaterialInstance>(ResourceInfo->SpecialSkill1.LoadSynchronous()));
			TB_SkillName->SetText(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::SpecialSkillName1, InSkillID)));
			TB_SkillDesc_Text->SetText(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::SpecialSkillNameDesc1, InSkillID)));
		}
		else if (InSkillType == ENPCSpecialSKill::Option2)
		{
			IMG_SkillIcon->SetBrushFromMaterial(Cast<UMaterialInstance>(ResourceInfo->SpecialSkill2.LoadSynchronous()));
			TB_SkillName->SetText(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::SpecialSkillName2, InSkillID)));
			TB_SkillDesc_Text->SetText(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::SpecialSkillNameDesc2, InSkillID)));
		}
		else if (InSkillType == ENPCSpecialSKill::Option3)
		{
			IMG_SkillIcon->SetBrushFromMaterial(Cast<UMaterialInstance>(ResourceInfo->SpecialSkill3.LoadSynchronous()));
			TB_SkillName->SetText(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::SpecialSkillName3, InSkillID)));
			TB_SkillDesc_Text->SetText(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::SpecialSkillNameDesc3, InSkillID)));
		}
	}

	if (InLv > 0)
	{
		auto* CreatedWidget = CreateWidget<UB2UISkillOptionList>(GetOwningPlayer(), SkillOptionListClass);
		if (CreatedWidget != NULL)
		{
			auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_OptionList->AddChild(CreatedWidget));
			CreatedWidget->Init();
			CreatedWidget->SetSkillOptionType(ESkillOptionTitleType::CurrentSkillOption);
			CreatedWidget->InitializeMercenaryOptions(InSkillID, InLv, InSkillType);
		}
	}

	if (0 <= InLv && InLv < GuildMasterData.GetMercenaryMasteriesCount(InSkillID))
	{
		auto* CreatedWidget = CreateWidget<UB2UISkillOptionList>(GetOwningPlayer(), SkillOptionListClass);
		if (CreatedWidget != NULL)
		{
			auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_OptionList->AddChild(CreatedWidget));
			CreatedWidget->Init();
			CreatedWidget->SetSkillOptionType(ESkillOptionTitleType::NextSkillOption);
			CreatedWidget->InitializeMercenaryOptions(InSkillID, InLv + 1, InSkillType);
		}
	}
}

void UB2UIMsgPopupTooltipSkillOption::CacheAssets()
{
	GET_SLOT(UButton, BTN_Close);
	GET_SLOT(UImage, IMG_SkillIcon);
	GET_SLOT(UTextBlock, TB_SkillName);
	GET_SLOT(UTextBlock, TB_SkillDesc_Text);
	GET_SLOT(UVerticalBox, VB_OptionList);
	GET_SLOT(UOverlay, OverlayOptionList);

	GET_SLOT(UImage, IMG_Skill_Badge);
	GET_SLOT(UTextBlock, TB_SkillType);

	GET_SLOT(UTextBlock, TB_SkillLevel);
	GET_SLOT(UTextBlock, TB_SkillMaxLevel);

	GET_SLOT(UPanelWidget, P_Skill_Badge);
	GET_SLOT(UHorizontalBox, HB_SkillLevel);

	if (VB_OptionList.IsValid())
		VB_OptionList->ClearChildren();
}

void UB2UIMsgPopupTooltipSkillOption::BindDelegates()
{
	//BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIMsgPopupTooltipSkillOption::OnClickBtnClose);
}

void UB2UIMsgPopupTooltipSkillOption::OnClickBtnClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::MsgPopupTooltipSkillOption);
}

bool UB2UIMsgPopupTooltipSkillOption::HasNextSpecialDisplayOption(int32 InSkillLv, int32 InCurrentSkillLevel, int32 & SpecialOptionLevel)
{
	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto* SkillMasterData = ClientDataStore.GetSkillMasterData(InSkillLv);
	int32 MaxSkillLevel = ClientDataStore.GetMaxSkillLevel(InSkillLv);

	auto* PCClassInfoBox = StaticFindPCClassInfoBox();
	UB2SkillInfo* AllSkillInfo = PCClassInfoBox ? PCClassInfoBox->GetAllSkillInfo() : nullptr;
	SpecialOptionLevel = INDEX_NONE;

	if (SkillMasterData && AllSkillInfo)
	{
		SpecialOptionLevel = AllSkillInfo->GetNextSpecialDisplaySkillLevel(GetSkillID(), InCurrentSkillLevel);
		return SpecialOptionLevel != INDEX_NONE;
	}

	return false;
}

void UB2UIMsgPopupTooltipSkillOption::CloseWidgetDelegate()
{
	OnClickBtnClose();
}