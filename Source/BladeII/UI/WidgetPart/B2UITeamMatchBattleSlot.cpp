#include "BladeII.h"
#include "B2UIDocHelper.h"
#include "Event.h"
#include "B2UIManager.h"
#include "B2UIBattleMain.h"
#include "B2UITeamMatchBattleSlot.h"

#include "BladeIIGameImpl.h"

UB2UITeamMatchBattleSlot::UB2UITeamMatchBattleSlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SlotIdx = INDEX_NONE;
	OldDeathstate = true;
	BuffIconSize = FVector2D(14.f, 14.f);
	UserName = FText::GetEmpty();
}

void UB2UITeamMatchBattleSlot::Init()
{
	Super::Init();

	SetCamera(false);
	SetDeath(false);
	SetContributionValue(INDEX_NONE);

	PCClass = static_cast<int32>(EPCClass::EPC_End);

	if (GetB2GameModeType(GetWorld()) == EB2GameMode::PVP_Team)
	{
		SetDamagePercent(0.0f);
	}
}

void UB2UITeamMatchBattleSlot::SetDamagePercent(float DamagePercent)
{
	if (TB_Contribution.IsValid())
	{
		TB_Contribution->SetVisibility(ESlateVisibility::HitTestInvisible);
		TB_Contribution->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), static_cast<int32>(DamagePercent))));
	}
}

int32 UB2UITeamMatchBattleSlot::GetCurrentPCClass() const
{
	return PCClass;
}

void UB2UITeamMatchBattleSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	DestroyAllBuffIcons_SoR();

	Super::DestroySelf(InUIManager);
}

void UB2UITeamMatchBattleSlot::SetData(EPCClass CharClass, int32 Lv, bool bInMyTeam, int32 InSlotIdx, int8 FormationNum)
{
	SlotIdx = InSlotIdx;
	bMyTeam = bInMyTeam;
	if (FormationNum != 0 && TBX_FomationNum.IsValid())
	{
		TBX_FomationNum->SetText(FText::AsNumber(FormationNum));
	}
	else if(FormationNum == 0 && X_FormationNumber.IsValid())
	{
		X_FormationNumber->SetVisibility(ESlateVisibility::Collapsed);
	}
	TB_Lv->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::AsNumber(Lv)));		
	PB_HP->SetPercent(1.f);

	if (CharClass != EPCClass::EPC_End)
	{
		if (UB2UIDocHero* HeroDoc = UB2UIDocHelper::GetDocHero(PCClassToInt(CharClass)))
		{
			if (UserName.ToString().IsEmpty())
				TB_Job->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(HeroDoc->GetHeroClass()));
			else
				TB_Job->SetText(UserName);

			IMG_CharacterImage->SetBrushFromMaterial(CharacterTexture[PCClassToInt(CharClass)]);

			PCClass = static_cast<int32>(CharClass);
		}
	}
}

void UB2UITeamMatchBattleSlot::SetHpAndShield(float CurHPValue, float MaxHPValue, float ShieldValue)
{
	if (SlotIdx == INDEX_NONE)
		return;

	SetHPShieldPBCommon(PB_HP.Get(), PB_Shield.Get(), CurHPValue, MaxHPValue, ShieldValue);

	SetDeath(CurHPValue <= 0.f);
}

void UB2UITeamMatchBattleSlot::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Character);
	GET_SLOT(UImage, IMG_CharacterImage);
	GET_SLOT(UTextBlock, TB_Lv);
	GET_SLOT(UTextBlock, TB_Job);
	GET_SLOT(UTextBlock, TBX_FomationNum);
	GET_SLOT(UProgressBar, PB_HP);
	GET_SLOT(UProgressBar, PB_Shield);

	GET_SLOT(UImage, IMG_Death);
	GET_SLOT(UImage, IMG_Camera);
	GET_SLOT(UImage, IMG_Circle_Camera);
	GET_SLOT(UImage, IMG_Circle_Death);

	GET_SLOT(UTextBlock, TB_Contribution);
	GET_SLOT(UHorizontalBox, HB_StageResurrectBuffIconSet);
	GET_SLOT(UPanelWidget, X_FormationNumber);
}

void UB2UITeamMatchBattleSlot::SetCamera(bool bSet)
{
	//if (IMG_Camera.IsValid())
	//	IMG_Camera->SetVisibility(bSet ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	//if (IMG_CharacterImage.IsValid())
	//	IMG_CharacterImage->GetBrush().TintColor = bSet ? FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f)) : FSlateColor(FLinearColor::White);

	//if (IMG_Circle_Camera.IsValid())
	//{
	//	if (bSet){
	//		PlayCameraEvent();
	//		IMG_Circle_Camera->SetVisibility(ESlateVisibility::Visible);
	//	}
	//	else{
	//		IMG_Circle_Camera->SetVisibility(ESlateVisibility::Hidden);
	//	}
	//}
}

void UB2UITeamMatchBattleSlot::SetDeath(bool IsDeath)
{
	//if (OldDeathstate == IsDeath)
	//	return;

	//OldDeathstate = IsDeath;

	//if (IsDeath)
	//	SetCamera(false);

	//if (IMG_Death.IsValid())
	//	IMG_Death->SetVisibility(IsDeath ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	//if (IMG_CharacterImage.IsValid())
	//	IMG_CharacterImage->GetBrush().TintColor = IsDeath ? FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f)) : FSlateColor(FLinearColor::White);

	//if (IMG_Circle_Death.IsValid())
	//{
	//	if (IsDeath)
	//	{
	//		PlayDeathEvent();
	//		IMG_Circle_Death->SetVisibility(ESlateVisibility::Visible);
	//	}
	//	else
	//	{
	//		IMG_Circle_Death->SetVisibility(ESlateVisibility::Hidden);
	//	}
	//}
}


void UB2UITeamMatchBattleSlot::SetContributionValue(int32 Value)
{
	bool IsContributionEnable = (Value != INDEX_NONE);

	if (TB_Contribution.IsValid())
	{
		TB_Contribution->SetVisibility(IsContributionEnable ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

		if (IsContributionEnable)
			TB_Contribution->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), Value)));
	}
}

void UB2UITeamMatchBattleSlot::CreateResurrectBuffIcon(EResurrectBuffType InResurrectBuffType, int32 InTeamIndex)
{
	// 부활 버프는 하나만 표시될 거기 땜에 기존에 있는 걸 찾아서 업데이트 하든지 새로 생성
	UB2UIInGameBuffIcon_Resurrect* CreatedOrFound = NULL;

	for (UB2UIInGameBuffIcon* ThisBuffIcon : AllBuffIcons_SoR)
	{
		UB2UIInGameBuffIcon_Resurrect* CastedResurrectBuffIcon = Cast<UB2UIInGameBuffIcon_Resurrect>(ThisBuffIcon);

		if (CastedResurrectBuffIcon)
		{
			CreatedOrFound = CastedResurrectBuffIcon;
			break;
		}
	}

	if (CreatedOrFound == nullptr)
		CreatedOrFound = CreateBuffIconCommon(BuffIconClass_Resurrect);

	if (CreatedOrFound == nullptr)
		return;

	CreatedOrFound->SetBuffType(InResurrectBuffType);
	CreatedOrFound->SetResurrectAdditionalInfo(InTeamIndex);
}

void UB2UITeamMatchBattleSlot::DestroyAllBuffIcons_SoR()
{
	for (UB2UIInGameBuffIcon* ThisBuffIcon : AllBuffIcons_SoR)
	{
		if (ThisBuffIcon)
		{
			ThisBuffIcon->UnbindDoc();
			ThisBuffIcon->DestroySelf(UB2UIManager::GetInstance());
		}
	}
	AllBuffIcons_SoR.Empty();
}

void UB2UITeamMatchBattleSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Character, &UB2UITeamMatchBattleSlot::OnClickBtnCharacter);
}

void UB2UITeamMatchBattleSlot::OnClickBtnCharacter()
{
	OnClickImplement();
}

void UB2UITeamMatchBattleSlot::OnChangedCharName(class UB2UIDocBase* Sender, FText CurrName, FText PrevName)
{
	UserName = CurrName;
}

void UB2UITeamMatchBattleSlot::OnChanagedHealth(class UB2UIDocBase* Sender, float CurrHealth, float PrevHealth)
{
	OnChangedHealthAndShieldCommon();
}

void UB2UITeamMatchBattleSlot::OnChanagedShield(class UB2UIDocBase* Sender, float CurrShield, float PrevShield)
{
	OnChangedHealthAndShieldCommon();
}

void UB2UITeamMatchBattleSlot::OnChangedHealthAndShieldCommon()
{
	FOnChangedHealthAndShieldCommon.ExecuteIfBound(bMyTeam, SlotIdx);
}

void UB2UITeamMatchBattleSlot::OnChanagedPCClass(class UB2UIDocBase* Sender, EPCClass CurPCClass, EPCClass PrePCClass)
{
	if (CurPCClass == EPCClass::EPC_End)
		return;

	if (UB2UIDocHero* HeroDoc = UB2UIDocHelper::GetDocHero(PCClassToInt(CurPCClass)))
	{
		if (UserName.ToString().IsEmpty())
			TB_Job->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(HeroDoc->GetHeroClass()));
		else
			TB_Job->SetText(UserName);

		IMG_CharacterImage->SetBrushFromMaterial(CharacterTexture[PCClassToInt(CurPCClass)]);
	}	
}

void UB2UITeamMatchBattleSlot::OnChanagedPCLevel(class UB2UIDocBase* Sender, int32 CurPCLevel, int32 PrePCLevel)
{
	TB_Lv->SetText(
		FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::AsNumber(CurPCLevel))
		);
}

void UB2UITeamMatchBattleSlot::OnChanagedContribution(class UB2UIDocBase* Sender, int32 CurContribution, int32 PreContribution)
{
	SetContributionValue(CurContribution);
}

UB2UIInGameBuffIcon_Resurrect* UB2UITeamMatchBattleSlot::CreateBuffIconCommon(TSubclassOf<UB2UIInGameBuffIcon> InCreateClass)
{
	if (InCreateClass && HB_StageResurrectBuffIconSet.IsValid())
	{
		UHorizontalBox* ToCreatePanel = HB_StageResurrectBuffIconSet.Get();
		UB2UIInGameBuffIcon* CreateClassDefObj = Cast<UB2UIInGameBuffIcon>(InCreateClass->GetDefaultObject());
		UB2UIInGameBuffIcon_Resurrect* CreatedWidget = Cast<UB2UIInGameBuffIcon_Resurrect>(
			DynCreateInHorizontalBox(InCreateClass, this, ToCreatePanel, BuffIconSize, FMargin())
			);

		if (CreatedWidget)
		{
			CreatedWidget->Init();
			AllBuffIcons_SoR.Add(CreatedWidget);
		}

		return CreatedWidget;
	}
	return NULL;
}

void UB2UITeamMatchBattleSlot::OnClickImplement()
{
	FOnClickCharacter.ExecuteIfBound(bMyTeam, SlotIdx);
}
