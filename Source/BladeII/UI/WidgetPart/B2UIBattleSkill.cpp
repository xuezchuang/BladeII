#include "B2UIBattleSkill.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocSkill.h"
#include "Styling/SlateStyle.h"
#include "B2PCClassInfoBox.h"
#include "B2SkillInfo.h"

UB2UIBattleSkill::UB2UIBattleSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsCountingCooltime = false;
}

void UB2UIBattleSkill::CacheAssets()
{
	GET_SLOT(UB2Button,		BTN_Area);
	GET_SLOT(UB2Button,		BTN_Area2);
	GET_SLOT(UTextBlock,	TB_SkillName);
	GET_SLOT(UTextBlock,	TB_SkillLevel);
	GET_SLOT(UTextBlock,	TB_SkillCooltimeNum);
	GET_SLOT(UImage,		IMG_SkillCooltime);

	GET_SLOT(UImage,		IMG_Lock);
	
	GET_SLOT(UImage, IMG_PointStack);
	GET_SLOT(UImage,		IMG_FX_SuggestSkill);

	if (IMG_PointStack.IsValid())
		IMG_PointStack->SetVisibility(ESlateVisibility::Collapsed);

	Skill_On = GetAnimation(FName("Skill_On"));
		
}

void UB2UIBattleSkill::UpdateStaticText()
{
}

void UB2UIBattleSkill::BindDelegates()
{
	//BIND_PRESS_FUNC_TO_BTN(BTN_Area, &UB2UIBattleSkill::OnPressedBtnArea) -> 捞扒 弊成 捞固瘤 侩栏肺 敬促.
	BIND_PRESS_FUNC_TO_BTN(BTN_Area2, &UB2UIBattleSkill::OnPressedBtnArea)
}

void UB2UIBattleSkill::BindDoc(UB2UIDocSkill* SkillDoc)
{ // 捞巴捞 阂府绰 矫痢俊 蝶扼 SkillDoc 狼 函荐甸篮 狼档茄 蔼捞 酒匆 荐 乐促. (抗甫 甸绢 UB2SkillRTManager::UpdateSkillDocDataOnPCSwap 捞 捞 捞饶俊 阂赴促芭唱) 弊繁 巴俊 措秦 霸烙 内靛 率俊辑 促矫陛 流立 诀单捞飘甫.
	UnbindDoc();
	SetDoc(SkillDoc);
	
	if (IMG_Lock.IsValid())
		IMG_Lock->SetVisibility(ESlateVisibility::Collapsed);

	if(IMG_FX_SuggestSkill.IsValid())
		IMG_FX_SuggestSkill->SetVisibility(ESlateVisibility::Collapsed);

	SkillDoc->OnIsCountingCooltimeChanged.AddUObject(this, &UB2UIBattleSkill::OnChangedIsCountingCooltime);
	SkillDoc->OnIsDisabledByCooltimeChanged.AddUObject(this, &UB2UIBattleSkill::OnChangedIsDisabledByCooltime);
	SkillDoc->OnIsDisabledByDebuffChanged.AddUObject(this, &UB2UIBattleSkill::OnChangedIsDisabledByDebuff);
	SkillDoc->OnRemainingCooltimeChanged.AddUObject(this, &UB2UIBattleSkill::OnChangedRemainingCooltime);
	SkillDoc->OnSkillStackMaxPointChanged.AddUObject(this, &UB2UIBattleSkill::OnChangedSkillPoint);
	SkillDoc->OnSkillStackPointChanged.AddUObject(this, &UB2UIBattleSkill::OnChangedSkillPoint);
	SkillDoc->OnIsExistingSkillChanged.AddUObject(this, &UB2UIBattleSkill::OnChangedIsExistingSkill);
	SkillDoc->OnIsSuggestSkillChanged.AddUObject(this, &UB2UIBattleSkill::OnChangedSuggetSkill);
	SkillDoc->OnIsDisabledByOtherSuggestChanged.AddUObject(this, &UB2UIBattleSkill::OnChangedDisabledByOtherSuggest);

	SkillDoc->SetIsExistingSkill(SkillDoc->GetIsExistingSkill()); // 捞扒 构瘤? 弊成 OnChangedIsExistingSkill 阂府霸 窍妨绰 扒啊..?

	SetLevel(SkillDoc->GetSkillLevel());
	SetName(SkillDoc->GetSkillName());
	SetButtonStyleByID(SkillDoc->GetSkillInfoSkillID());
	SetCooltime(SkillDoc->GetFinalCooltime());//SetCooltime(SkillDoc->GetRemainingCooltime());
	SetCooltimeVisible(SkillDoc->GetIsDisabledByCooltime());	//Cooltime visible
	SetEnabledBtn(!SkillDoc->GetIsDisabledByCooltime());		//Button Enable
	SetSkillPointStack(SkillDoc->GetSkillStackMaxPoint(), SkillDoc->GetSkillStackPoint());

	//CAPTURE_UOBJECT(UB2UIBattleSkill);

	//OnEtherActivatedTicket = OnEtherActivatedClass<int32, ABladeIIPlayer*, ABladeIICharacter*>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(OnEtherActivated, int32 EtherSetID, ABladeIIPlayer* Owner, ABladeIICharacter* Causer)
	//		Capture->OnEtherActivated(EtherSetID, Owner, Causer);
	//	END_CAPTURE_OBJECT()
	//	);
}

void UB2UIBattleSkill::UnbindDoc()
{
	auto SkillDoc = Cast<UB2UIDocSkill>(GetDoc());
	if (SkillDoc)
	{
		SkillDoc->OnIsCountingCooltimeChanged.RemoveAll(this);
		SkillDoc->OnIsDisabledByCooltimeChanged.RemoveAll(this);
		SkillDoc->OnIsDisabledByDebuffChanged.RemoveAll(this);
		SkillDoc->OnRemainingCooltimeChanged.RemoveAll(this);
		SkillDoc->OnSkillStackMaxPointChanged.RemoveAll(this);
		SkillDoc->OnSkillStackPointChanged.RemoveAll(this);
		SkillDoc->OnIsExistingSkillChanged.RemoveAll(this);
		SkillDoc->OnIsSuggestSkillChanged.RemoveAll(this);
	}

	SetDoc(nullptr);

	//Data initialize
	bFullStackSkillPoint = false;
	bOwnerHasDebuff = false;
	bCurrentSuggest = false;

	//OnEtherActivatedClass<int32, ABladeIIPlayer*, ABladeIICharacter*>::GetInstance().Unsubscribe(OnEtherActivatedTicket);
}

void UB2UIBattleSkill::SetLevel(int32 InValue)
{
	if (TB_SkillLevel.IsValid())
		TB_SkillLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("InGameHUD_Level"))), FText::AsNumber(InValue)));
}

void UB2UIBattleSkill::SetName(const FText& InText)
{
	if (TB_SkillName.IsValid())
		TB_SkillName->SetText(InText);
}

void UB2UIBattleSkill::SetCooltime(float InValue)
{
	//箭磊贸府
	if (TB_SkillCooltimeNum.IsValid())
	{
		// 弥措 酿鸥烙捞 家荐痢栏肺 炼沥等 版快甫 钎矫窍扁 困秦 霉 家荐痢阑 墨款飘窍绰 悼救俊绰 家荐痢 霉掳磊府鳖瘤 钎矫. 拌加 钎矫窍搁 家荐痢捞 唱鸥车促 荤扼脸促 秦辑 沥脚绝澜.
		UB2UIDocSkill* MySkillDoc = Cast<UB2UIDocSkill>(GetDoc());
		const float MaxCoolTime = MySkillDoc ? MySkillDoc->GetFinalCooltime() : 0.0f;
		TB_SkillCooltimeNum->SetText(GetAutoFractionalFormattedText_FirstFracCountOnly(InValue, MaxCoolTime, 1));
	}

	UpdateCooltimeMaterial(InValue);
}

void UB2UIBattleSkill::UpdateCooltimeMaterial(float RemainingCooltime)
{
	//酿鸥烙 赣磐府倔 贸府
	if (IMG_SkillCooltime.IsValid() && !MID_SkilCooltime)
		MID_SkilCooltime = IMG_SkillCooltime->GetDynamicMaterial();

	if (MID_SkilCooltime)
	{
		auto SkillDoc = Cast<UB2UIDocSkill>(GetDoc());
		if (!SkillDoc) return;
		MID_SkilCooltime->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, RemainingCooltime / SkillDoc->GetFinalCooltime());
	}
}

void UB2UIBattleSkill::SetCooltimeVisible(bool bVisible)
{
	if (TB_SkillCooltimeNum.IsValid())
		bVisible ? TB_SkillCooltimeNum->SetVisibility(ESlateVisibility::HitTestInvisible) : TB_SkillCooltimeNum->SetVisibility(ESlateVisibility::Collapsed);

	if (IMG_SkillCooltime.IsValid())
		bVisible ? IMG_SkillCooltime->SetVisibility(ESlateVisibility::HitTestInvisible) : IMG_SkillCooltime->SetVisibility(ESlateVisibility::Collapsed);

	if (!bVisible)
	{
		MID_SkilCooltime = NULL;
		PlayAnimation(Skill_On);
	}

}

void UB2UIBattleSkill::SetCooltimeTextColor(const FSlateColor& InColor)
{
	if (TB_SkillCooltimeNum.IsValid())
		TB_SkillCooltimeNum->SetColorAndOpacity(InColor);
}

void UB2UIBattleSkill::SetEnabledBtn(bool bEnabled)
{
	if (BTN_Area.IsValid())
		BTN_Area->SetIsEnabled(bEnabled);
	if (BTN_Area2.IsValid())
		BTN_Area2->SetIsEnabled(bEnabled);
}

void UB2UIBattleSkill::SetButtonStyleByID(int32 InSkillID)
{	
	UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox();
	UB2SkillInfo* SkillInfoObj = PCClassInfoBox ? PCClassInfoBox->GetAllSkillInfo() : NULL;

	if (SkillInfoObj)
	{
		if (BTN_Area.IsValid())
		{
			//BTN_Area->WidgetStyle = SkillInfoObj->GetButtonIconStyle(InSkillID);
		}

		const FSingleSkillInfo* SingleSkillInfo = SkillInfoObj->GetSingleInfoOfID(InSkillID);
		if (SingleSkillInfo && IMG_FX_SuggestSkill.IsValid())
		{
			ESkillType SkillType = SingleSkillInfo->SkillType;
			UMaterialInterface* SuggestMaterial = (SkillType == ESkillType::ESkillType_Survive) ? SurviveSuggestMaterial : DefaultSuggestMaterial;
			if (SuggestMaterial)
			{
				IMG_FX_SuggestSkill->SetBrushFromMaterial(SuggestMaterial);
			}
		}
	}
}

void UB2UIBattleSkill::OnChangedSuggetSkill(class UB2UIDocBase* Sender, bool CurrSuggest, bool PrevSuggest)
{
	if (IMG_FX_SuggestSkill.IsValid())
	{
		IMG_FX_SuggestSkill->SetVisibility(CurrSuggest ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		bCurrentSuggest = CurrSuggest;
	}
}

void UB2UIBattleSkill::OnChangedDisabledByOtherSuggest(class UB2UIDocBase* Sender, bool CurrDisable, bool PrevDisable)
{
	SetEnabledBtn(!CurrDisable);
}

void UB2UIBattleSkill::OnEtherActivated(int32 EtherSetID, ABladeIIPlayer* Owner, ABladeIICharacter* Causer)
{
	bool bUINotify = false;

	if (EtherSetID == b2network::B2AetherSetOptionType::SKILL_GAGE)
	{
		if (Owner && Owner->IsLocalPlayer())
			bUINotify = true;
	}

	else if (EtherSetID == b2network::B2AetherSetOptionType::SKILL_COOLTIME)
	{
		ABladeIIPlayer* CauserPlayer = Cast<ABladeIIPlayer>(Causer);
		if (CauserPlayer && CauserPlayer->IsLocalPlayer())
			bUINotify = true;
	}

	if(bUINotify)
		EtherActivated(EtherSetID);
}


//====================================================================================
//									Click Handler
//====================================================================================
void UB2UIBattleSkill::OnPressedBtnArea()
{
	OnBattleSkillClicked.ExecuteIfBound(this);
}

//====================================================================================
//									Doc Delegate
//====================================================================================
void UB2UIBattleSkill::OnChangedIsCountingCooltime(class UB2UIDocBase* Sender, bool IsCountingCooltime, bool PrevIsCountingCooltime)
{
	bIsCountingCooltime = IsCountingCooltime; // bIsCountingCooltime 篮 促弗 镑俊辑档 曼炼.

	SetCooltimeTextColor(bIsCountingCooltime ? SkillCoolTimeTextColor : SkillCoolTimeTextPendingCountColor);
}

void UB2UIBattleSkill::OnChangedIsDisabledByCooltime(class UB2UIDocBase* Sender, bool IsDisabledByCooltime, bool PrevIsDisabledByCooltime)
{
	bInCoolTime = IsDisabledByCooltime;
	if (IsDisabledByCooltime)
	{
		if (BTN_Area.IsValid())
		{
			BTN_Area->CustomStopOnTouchImpulseRing(); // 酒付档 滚瓢 press 甫 烹秦 胶懦捞 惯悼等 惑怕扼 ImpulseRing 捞 难脸阑 刨单 disable 登搁 release 皑瘤甫 给窍霸 凳. 流立 波霖促.
		}
		if (BTN_Area2.IsValid())
		{
			BTN_Area2->CustomStopOnTouchImpulseRing(); // 酒付档 滚瓢 press 甫 烹秦 胶懦捞 惯悼等 惑怕扼 ImpulseRing 捞 难脸阑 刨单 disable 登搁 release 皑瘤甫 给窍霸 凳. 流立 波霖促.
		}


		if (bCurrentSuggest && IsInCooltime())
			OnChangedSuggetSkill(nullptr, false, false);
	}

	SetCooltimeVisible(IsDisabledByCooltime);

	//bool bSkillEnable = bOwnerHasDebuff == false && bInCoolTime == false;
	if (bInCoolTime)
	{
		SetEnabledBtn(false);
	}
	else
	{
		bool bSkillEnable = !bOwnerHasDebuff;
		if (bOwnerHasDebuff && IsSurviveSkill())
			bSkillEnable = true;

		SetEnabledBtn(bSkillEnable);
	}
}

void UB2UIBattleSkill::OnChangedIsDisabledByDebuff(class UB2UIDocBase* Sender, bool IsDisabledByDebuff, bool PrevIsDisabledByDebuff)
{
	bOwnerHasDebuff = IsDisabledByDebuff;

	// 积粮胶懦, 搬加胶懦篮 Debuff 康氢 罐瘤 臼澜
	if (IsWeaponSkill() || IsSurviveSkill())
		return;

	// 扁夯利栏肺 Debuff(葫搬, 胶畔)吧府芭唱 Cooltime 矫俊绰 Skill荤侩 阂啊瓷
	bool bSkillEnable = bOwnerHasDebuff == false && bInCoolTime == false;
	SetEnabledBtn(bSkillEnable);
}

void UB2UIBattleSkill::OnChangedRemainingCooltime(class UB2UIDocBase* Sender, float RemainingCooltime, float PrevRemainingCooltime)
{
	SetCooltime(RemainingCooltime);
}

void UB2UIBattleSkill::OnChangedSkillPoint(class UB2UIDocBase* Sender, float CurrPoint, float PrevPoint)
{
	auto SkillDoc = Cast<UB2UIDocSkill>(Sender);
	if (!SkillDoc) return;

	SetSkillPointStack(SkillDoc->GetSkillStackMaxPoint(), SkillDoc->GetSkillStackPoint());
}

void UB2UIBattleSkill::OnChangedIsExistingSkill(class UB2UIDocBase* Sender, bool CurrExistingSkill, bool PrevExistingSkill)
{
	/*if (IMG_Lock.IsValid())
		IMG_Lock->SetVisibility(CurrExistingSkill ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);*/

	SetEnabledBtn(CurrExistingSkill);
}

void UB2UIBattleSkill::GetAreaBTNs(TArray<TWeakObjectPtr<UB2Button>>& OutAreaBtns)
{
	OutAreaBtns.Empty();

	if (BTN_Area.IsValid())
		OutAreaBtns.Add(BTN_Area);

	if (BTN_Area2.IsValid())
		OutAreaBtns.Add(BTN_Area2);
}

void UB2UIBattleSkill::GetButtonStyle(struct FButtonStyle& OutStyle)
{
	if (BTN_Area.IsValid())
	{
		//OutStyle = BTN_Area->WidgetStyle;
	}		
}

void UB2UIBattleSkill::SetLock(bool bLock)
{
	if (IMG_Lock.IsValid())
		IMG_Lock->SetVisibility(bLock ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	SetEnabledBtn(bLock);
}


bool UB2UIBattleSkill::IsActivatable()
{
	if (IsInCooltime())
		return false;

	if (IsWeaponSkill() && bFullStackSkillPoint == false)
		return false;

	return true;
}

bool UB2UIBattleSkill::IsSurviveSkill()
{
	auto* PCClassInfoBox = StaticFindPCClassInfoBox(this);
	UB2SkillInfo* SkillInfoObj = PCClassInfoBox ? PCClassInfoBox->GetAllSkillInfo() : nullptr;
	auto* SkillDoc = Cast<UB2UIDocSkill>(GetDoc());

	if (SkillInfoObj && SkillDoc)
	{
		auto* SkillInfo = SkillInfoObj->GetSingleInfoOfID(SkillDoc->GetSkillInfoSkillID());
		if (SkillInfo && SkillInfo->SkillType == ESkillType::ESkillType_Survive)
			return true;
	}

	return false;
}

bool UB2UIBattleSkill::IsWeaponSkill()
{
	//auto* PCClassInfoBox = StaticFindPCClassInfoBox(this);
	//UB2SkillInfo* SkillInfoObj = PCClassInfoBox ? PCClassInfoBox->GetAllSkillInfo() : nullptr;
	//auto* SkillDoc = Cast<UB2UIDocSkill>(GetDoc());

	//if (SkillInfoObj && SkillDoc)
	//{
	//	auto* SkillInfo = SkillInfoObj->GetSingleInfoOfID(SkillDoc->GetSkillInfoSkillID());
	//	if (SkillInfo && SkillInfo->WeaponSkillType != EWeaponSkillType::EWS_None && SkillInfo->WeaponSkillType != EWeaponSkillType::EWS_End)
	//		return true;
	//}

	return false;
}
