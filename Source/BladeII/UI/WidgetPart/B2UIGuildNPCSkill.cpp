#include "B2UIGuildNPCSkill.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "BladeIIMercenaryPlayer.h"
#include "Styling/SlateStyle.h"

UB2UIGuildNPCSkill::UB2UIGuildNPCSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MercenaryPlayer = nullptr;
	CacheEnableSkill = false;
	bDead = false;
}

void UB2UIGuildNPCSkill::CacheAssets()
{
	GET_SLOT(UB2Button,		BTN_Area);
	GET_SLOT(UB2Button,		BTN_Area2);
	GET_SLOT(UTextBlock,	TB_SkillName);
	GET_SLOT(UTextBlock,	TB_SkillCooltimeNum);
	GET_SLOT(UImage,		IMG_SkillCooltime);
	GET_SLOT(UTextBlock,	TB_NPCName); 
	

	GET_SLOT(UImage,		IMG_Lock);
	
	Skill_On = GetAnimation(FName("Skill_On"));
}

void UB2UIGuildNPCSkill::UpdateStaticText()
{
}

void UB2UIGuildNPCSkill::BindDelegates()
{
	BIND_PRESS_FUNC_TO_BTN(BTN_Area2, &UB2UIGuildNPCSkill::OnPressedBtnArea)
}

void UB2UIGuildNPCSkill::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	/*if (MercenaryPlayer && MercenaryPlayer->IsValidObj())
	{
		if (MercenaryPlayer->IsAlive() && UseSkillData.IsValid())
		{
			SetCooltime(UseSkillData->RemainSkillCoolTime, UseSkillData->MaxSkillCoolTime);
			SetCooltimeVisible(UseSkillData->RemainSkillCoolTime > 0.0f);

			bool CurrentActiveUseSkill = MercenaryPlayer->IsActiveUseSkill(false);
			if (CacheEnableSkill != CurrentActiveUseSkill)
			{
				CacheEnableSkill = CurrentActiveUseSkill;
				SetEnabledBtn(CacheEnableSkill);
				if (!CacheEnableSkill)
				{
					MID_SkilCooltime = NULL;
					PlayAnimation(Skill_On);
				}
			}
		}
		if (!bDead && !MercenaryPlayer->IsAlive())
		{
			bDead = true;
			SetEnabledBtn(false);
			SetCooltime(0.0f, 1.0f);
			SetCooltimeVisible(false);
		}
	}*/
}

void UB2UIGuildNPCSkill::SetNPCSkill(ABladeIIMercenaryPlayer * InMercenaryPlayer)
{
	if (!InMercenaryPlayer)
		return;

	MercenaryPlayer = InMercenaryPlayer;

	UseSkillData = MercenaryPlayer->GetUseSkillData();
	
	if (!UseSkillData.IsValid())
	{
		SetEmpty(true);
		SetLock(true);
		return;
	}

	UB2GuildNPCInfo* GuildNPCInfo = StaticFindGuildNPCInfo();
	if (!GuildNPCInfo)
		return;

	const FGuildMercenarySkill* MercenarySkillData = GuildNPCInfo->GetMercenarySkillOfID(UseSkillData->SkillID);
	if (!MercenarySkillData)
		return;

	CacheEnableSkill = false;
	bDead = false;

	SetEmpty(false);
	SetLock(false);

	SetSkillName(BladeIIGetLOCText(B2LOC_CAT_GUILDNPCSKILL, GetGuildNPCSkillLocalizedTextKey(EGuildNPCSkillTextDataType::SkillName, UseSkillData->SkillID)));
	SetMercenaryName(BladeIIGetLOCText(B2LOC_CAT_GUILDNPC, GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType::NPCName, InMercenaryPlayer->GetMercenaryInfo().MercenaryID)));
	SetButtonStyle(MercenarySkillData);
	SetCooltime(UseSkillData->RemainSkillCoolTime, UseSkillData->MaxSkillCoolTime);
	SetCooltimeVisible(false);	
	SetEnabledBtn(MercenaryPlayer->IsActiveUseSkill(false));
}

void UB2UIGuildNPCSkill::SetSkillName(const FText& InText)
{
	if (TB_SkillName.IsValid())
		TB_SkillName->SetText(InText);
}

void UB2UIGuildNPCSkill::SetMercenaryName(const FText & InText)
{
	if (TB_NPCName.IsValid())
		TB_NPCName->SetText(InText);
}

void UB2UIGuildNPCSkill::SetCooltime(float InValue, float MaxCoolTime)
{
	//箭磊贸府
	if (TB_SkillCooltimeNum.IsValid())
		TB_SkillCooltimeNum->SetText(GetAutoFractionalFormattedText_FirstFracCountOnly(InValue, MaxCoolTime, 1));

	UpdateCooltimeMaterial(InValue, MaxCoolTime);
}

void UB2UIGuildNPCSkill::UpdateCooltimeMaterial(float RemainingCooltime, float MaxCoolTime)
{
	//酿鸥烙 赣磐府倔 贸府
	if (IMG_SkillCooltime.IsValid() && !MID_SkilCooltime)
		MID_SkilCooltime = IMG_SkillCooltime->GetDynamicMaterial();

	if (MID_SkilCooltime)
		MID_SkilCooltime->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, RemainingCooltime / MaxCoolTime);
}

void UB2UIGuildNPCSkill::OnPressedBtnArea()
{
	if (MercenaryPlayer)
		MercenaryPlayer->ReserveUseSkill();
}

void UB2UIGuildNPCSkill::SetCooltimeVisible(bool bVisible)
{
	if (TB_SkillCooltimeNum.IsValid())
		bVisible ? TB_SkillCooltimeNum->SetVisibility(ESlateVisibility::HitTestInvisible) : TB_SkillCooltimeNum->SetVisibility(ESlateVisibility::Collapsed);

	if (IMG_SkillCooltime.IsValid())
		bVisible ? IMG_SkillCooltime->SetVisibility(ESlateVisibility::HitTestInvisible) : IMG_SkillCooltime->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIGuildNPCSkill::SetCooltimeTextColor(const FSlateColor& InColor)
{
	if (TB_SkillCooltimeNum.IsValid())
		TB_SkillCooltimeNum->SetColorAndOpacity(InColor);
}

void UB2UIGuildNPCSkill::SetEnabledBtn(bool bEnabled)
{
	if (BTN_Area.IsValid())
		BTN_Area->SetIsEnabled(bEnabled);
	if (BTN_Area2.IsValid())
		BTN_Area2->SetIsEnabled(bEnabled);
}

void UB2UIGuildNPCSkill::SetButtonStyle(const FGuildMercenarySkill* InSkillData)
{	
	if (!InSkillData)
		return;

	//if (BTN_Area.IsValid())
	//{
	//	FButtonStyle* RetStyle = &BTN_Area->WidgetStyle;

	//	if (RetStyle)
	//	{
	//		RetStyle->Normal.SetResourceObject(InSkillData->SkillImageNormal.LoadSynchronous());
	//		RetStyle->Hovered.SetResourceObject(InSkillData->SkillImageHover.LoadSynchronous());
	//		RetStyle->Pressed.SetResourceObject(InSkillData->SkillImagePress.LoadSynchronous());
	//		RetStyle->Disabled.SetResourceObject(InSkillData->SkillImageDisabled.LoadSynchronous());
	//	}
	//}
}

void UB2UIGuildNPCSkill::SetLock(bool bLock)
{
	if (IMG_Lock.IsValid())
		IMG_Lock->SetVisibility(bLock ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	SetEnabledBtn(!bLock);
}

bool UB2UIGuildNPCSkill::IsActivatable()
{
	//if (MercenaryPlayer && MercenaryPlayer->IsValidObj() && MercenaryPlayer->IsAlive())
	//	return MercenaryPlayer->IsActiveUseSkill();

	return false;
}

bool UB2UIGuildNPCSkill::IsAbnormalState()
{
	//if (MercenaryPlayer && MercenaryPlayer->IsValidObj() && MercenaryPlayer->IsAlive())
	//	return MercenaryPlayer->IsStunned() || MercenaryPlayer->IsFrozen();

	return true;
}
