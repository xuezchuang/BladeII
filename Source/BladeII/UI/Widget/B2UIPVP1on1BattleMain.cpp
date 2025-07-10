#include "B2UIPVP1on1BattleMain.h"
#include "B2UIBattleSkill.h"
#include "B2UIBattleCombo.h"
#include "B2UIBattleVignette.h"
#include "B2UIManager.h"
#include "Event.h"
#include "B2UIBattleSkill.h"
#include "B2UIDocHelper.h"
#include "B2UIGradeRankAndStar.h"
#include "B2UIBattleMain.h"
#include "B2UIBattleTagAnim.h"
#include "BladeIIGameMode.h"
#include "B2PCClassInfoBox.h"
#include "B2InGameHUDInfo_Player.h"
#include "B2UIBattleNotice.h"
#include "B2CombatStatEvaluator.h"
#include "B2UIUtil.h"

#include "B2AssetLoader.h"
#include "B2UIEtherSetSkillIcon.h"
#include "B2UIPAutoBattleIcon.h"
#include "BladeIIGameImpl.h"
#include "B2GameInstance.h"

UB2UIPVP1on1BattleMain::UB2UIPVP1on1BattleMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LocalCharAssets = nullptr;
	RivalCharAssets = nullptr;
	CreatedTagAnimUIP = NULL;
	LoadedTagAnimUIPClass = NULL;
	bClickedPause = false;
}

void UB2UIPVP1on1BattleMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ProcessEvadeCoolTimeUI();
}

#define GET_SLOTS_PVP_CHARACTER(STRUCT, PREFIX)													\
	GET_SLOT_BYNAME(UButton, STRUCT.BTN_Tag, BTN_##PREFIX##_Tag)								\
	GET_SLOT_BYNAME(UImage, STRUCT.IMG_PortraitMain, IMG_##PREFIX##_PortraitMain)				\
	GET_SLOT_BYNAME(UTextBlock, STRUCT.TB_MainCharLevel, TB_##PREFIX##_MainCharLevel)			\
	GET_SLOT_BYNAME(UTextBlock, STRUCT.TB_MainCharName, TB_##PREFIX##_MainCharName)				\
	GET_SLOT_BYNAME(UTextBlock, STRUCT.TB_MainCharLevel, TB_##PREFIX##_MainCharLevel)			\
	GET_SLOT_BYNAME(UProgressBar, STRUCT.PB_MainCharHP, PB_##PREFIX##_MainCharHP)				\
	GET_SLOT_BYNAME(UProgressBar, STRUCT.PB_MainCharShield, PB_##PREFIX##_MainCharShield)		\
	GET_SLOT_BYNAME(UTextBlock, STRUCT.TB_CurrentHP, TB_##PREFIX##_CurrentHP)					\
	GET_SLOT_BYNAME(UTextBlock, STRUCT.TB_MaxHP, TB_##PREFIX##_MaxHP)							\
	GET_SLOT_BYNAME(UImage, STRUCT.IMG_PortraitSub, IMG_##PREFIX##_PortraitSub)					\
	GET_SLOT_BYNAME(UImage, STRUCT.IMG_PortraitSubDisable, IMG_##PREFIX##_PortraitSubDisable)	\
	GET_SLOT_BYNAME(UProgressBar, STRUCT.PB_SubCharHP, PB_##PREFIX##_SubCharHP)					\
	GET_SLOT_BYNAME(UImage, STRUCT.IMG_TagCooltime, IMG_##PREFIX##_TagCooltime)					\
	GET_SLOT_BYNAME(UImage, STRUCT.IMG_TagCooltimeBottom, IMG_TagCooltimeBottom)				\
	GET_SLOT_BYNAME(UImage, STRUCT.IMG_MyBadge, IMG_##PREFIX##_MyBadge)							\
	GET_SLOT_BYNAME(UTextBlock, STRUCT.TB_TagCooltimeNum, TB_##PREFIX##_TagCooltimeNum)			\
	if (STRUCT.IMG_TagCooltime.IsValid())														\
		STRUCT.TagCooltimeDisplayMID = STRUCT.IMG_TagCooltime->GetDynamicMaterial();			\
	if (STRUCT.IMG_TagCooltimeBottom.IsValid())													\
		STRUCT.TagCooltimeDisplayMIDBottom = STRUCT.IMG_TagCooltimeBottom->GetDynamicMaterial();\
	GET_SLOT_BYNAME(UPanelWidget, STRUCT.CP_TagBottomEnabledTrue, CP_TagBottomEnabledTrue)		\
	GET_SLOT_BYNAME(UPanelWidget, STRUCT.CP_TagBottomEnabledFalse, CP_TagBottomEnabledFalse)	\
	GET_SLOT_BYNAME(UImage, STRUCT.IMG_TagOblivionLockBG, IMG_TagOblivionLockBG)	\
	GET_SLOT_BYNAME(UOverlay, STRUCT.O_Death, O_##PREFIX##_Death)								\
	GET_SLOT_BYNAME(UImage, STRUCT.IMG_FX_TagSuggest, IMG_##PREFIX##_FX_TagSuggest)			\
	if (STRUCT.IMG_FX_TagSuggest.IsValid())													\
	{																						\
		STRUCT.IMG_FX_TagSuggest->SetVisibility(ESlateVisibility::Hidden);					\
	}																						\
	GET_SLOT_BYNAME(UB2UIGradeRankAndStar, STRUCT.UIP_PVPGrade, UIP_##PREFIX##_PVPGrade);	\
	if(	STRUCT.UIP_PVPGrade.IsValid())														\
	{																						\
		STRUCT.UIP_PVPGrade->Init();														\
	}																						\
	GET_SLOT_BYNAME(UHorizontalBox, STRUCT.HB_BuffIconSet, HB_##PREFIX##_BuffIconSet)		\
	GET_SLOT_BYNAME(UHorizontalBox, STRUCT.HB_Guild, HB_##PREFIX##_Guild)					\


void UB2UIPVP1on1BattleMain::CacheAssets()
{
		//배틀콤보
	GET_SLOT(UB2UIBattleCombo, UIP_BattleCombo);

		//비넥트
	GET_SLOT(UB2UIBattleVignette, UIP_BattleVignette);

		//버튼들
	GET_SLOT(UButton, BTN_Pause);
	GET_SLOT(UButton, BTN_Attack);
	GET_SLOT(UButton, BTN_Guard);

	GET_SLOT(UPanelWidget, CP_GuardEnable);
	GET_SLOT(UPanelWidget, CP_GuardDisable);
	GET_SLOT(UTextBlock, TB_GuradCoolTime);

	GET_SLOTS_PVP_CHARACTER(LeftCharAssets, Left);
	GET_SLOTS_PVP_CHARACTER(RightCharAssets, Right);

	GET_SLOT(UTextBlock, TB_LeftTime);
	GET_SLOT(UTextBlock, TB_Fight);

	GET_SLOT(UTextBlock, TB_TagAnim_Gladiator);
	GET_SLOT(UTextBlock, TB_TagAnim_Assassin);
	GET_SLOT(UTextBlock, TB_TagAnim_Wizard);
	GET_SLOT(UTextBlock, TB_TagAnim_Fighter);

	GET_SLOT(UTextBlock, TB_LeaveEnemy);

	GET_SLOT(UImage, IMG_FocusTimer);
	GET_SLOT(UImage, IMG_Victory);
	GET_SLOT(UImage, IMG_Lose);
	GET_SLOT(UImage, IMG_Draw);

	GET_SLOT(UB2Button, BTN_Tag);
	GET_SLOT(UB2UIPAutoBattleIcon, UIP_AutoBattleIcon)
	GET_SLOT(UPanelWidget, P_SkillSet);
	GET_SLOT(UWidgetAnimation, ANIM_TimeFocus);

	if (UIP_BattleCombo.IsValid())
	{
		UIP_BattleCombo->Init();
		UIP_BattleCombo->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (UIP_BattleVignette.IsValid())
	{
		UIP_BattleVignette->Init();
		UIP_BattleVignette->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IMG_FocusTimer.IsValid())
	{
		IMG_FocusTimer->SetVisibility(ESlateVisibility::Collapsed);
	}

	GET_SLOT(UB2UIEtherSetSkillIcon, UIP_OffenseEtherIcon);
	if (UIP_OffenseEtherIcon.IsValid())
		UIP_OffenseEtherIcon->Init();
	GET_SLOT(UB2UIEtherSetSkillIcon, UIP_DefenseEtherIcon);
	if (UIP_DefenseEtherIcon.IsValid())
		UIP_DefenseEtherIcon->Init();

	GET_SLOT(UImage, IMG_EvadeSkillCooltime);

	GET_SLOT(UB2UIBattleNotice, UIP_BattleNotice);
	if (UIP_BattleNotice.IsValid())
	{
		UIP_BattleNotice->Init();
	}
}

void UB2UIPVP1on1BattleMain::Init()
{
	Super::Init();
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (UIP_AutoBattleIcon.IsValid() && B2GM)
	{
		UIP_AutoBattleIcon->Init();
		UIP_AutoBattleIcon->SetIconAutoBattleType(B2GM->GetPlayerAutoBattleType());
	}

	StopPauseMenuEvent = StopPauseMenuClass<>::GetInstance().Subscribe([this]() { SetClickedPause(false); });

	IsPlayingTimeFocusAnimation = false;

	SetGuardUI(false);
}

void UB2UIPVP1on1BattleMain::UpdateStaticText()
{
	if (TB_Fight.IsValid())
	{
		TB_Fight->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_Fight_Message")));
	}
	if (TB_TagAnim_Gladiator.IsValid())
	{
		TB_TagAnim_Gladiator->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(EPCClass::EPC_Gladiator));
	}
	if (TB_TagAnim_Assassin.IsValid())
	{
		TB_TagAnim_Assassin->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(EPCClass::EPC_Assassin));
	}
	if (TB_TagAnim_Wizard.IsValid())
	{
		TB_TagAnim_Wizard->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(EPCClass::EPC_Wizard));
	}
	if (TB_TagAnim_Fighter.IsValid())
	{
		TB_TagAnim_Fighter->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(EPCClass::EPC_Fighter));
	}
	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
}

void UB2UIPVP1on1BattleMain::BindDelegates()
{
	BIND_PRESS_FUNC_TO_BTN(BTN_Attack, &UB2UIPVP1on1BattleMain::OnPressedAttackBtn);
	BIND_RELEASE_FUNC_TO_BTN(BTN_Attack, &UB2UIPVP1on1BattleMain::OnReleasedAttackBtn);
	BIND_PRESS_FUNC_TO_BTN(BTN_Guard, &UB2UIPVP1on1BattleMain::OnPressedGuardBtn);
	BIND_RELEASE_FUNC_TO_BTN(BTN_Guard, &UB2UIPVP1on1BattleMain::OnReleasedGuardBtn);
	/*BIND_CLICK_FUNC_TO_BTN(LeftCharAssets.BTN_Tag, &UB2UIPVP1on1BattleMain::OnClickedLeftTagBtn);
	BIND_CLICK_FUNC_TO_BTN(RightCharAssets.BTN_Tag, &UB2UIPVP1on1BattleMain::OnClickedRightTagBtn);*/
	BIND_CLICK_FUNC_TO_BTN(BTN_Tag, &UB2UIPVP1on1BattleMain::OnClickedTagBtn);
	BIND_CLICK_FUNC_TO_BTN(BTN_Pause, &UB2UIPVP1on1BattleMain::OnClickedPauseBtn);
}

void UB2UIPVP1on1BattleMain::DestroySelf(class UB2UIManager* InUIManager)
{
	DestroyTagAnimUIP(true);

	if (UIP_AutoBattleIcon.IsValid())
	{
		UIP_AutoBattleIcon->DestroySelf(InUIManager);
	}

	StopPauseMenuClass<>::GetInstance().Unsubscribe(StopPauseMenuEvent);

	Super::DestroySelf(InUIManager);
}

void UB2UIPVP1on1BattleMain::SetLeftTime(int32 LeftTimeSeconds)
{
	if (TB_LeftTime.IsValid())
	{
		TB_LeftTime->SetText(FText::FromString(UB2UIManager::FormatSecondsAsTime(LeftTimeSeconds, TEXT("m:ss"))));
	}
	
	if (LeftTimeSeconds < 11 && !IsPlayingTimeFocusAnimation && ANIM_TimeFocus.IsValid())
	{
		PlayAnimation(ANIM_TimeFocus.Get(), 0.0f, 0);
		IsPlayingTimeFocusAnimation = true;
	}
	
	if (LeftTimeSeconds < 5 && IMG_FocusTimer.IsValid() && IMG_FocusTimer->GetVisibility() != ESlateVisibility::Visible)
	{
		IMG_FocusTimer->SetVisibility(ESlateVisibility::Visible);
		PlayFocusTimer_BP();
	}
	else
	{
		IMG_FocusTimer->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIPVP1on1BattleMain::SetLocalPlayerToLeft(bool bLeft)
{
	bIsLeftControlledLocalPlayer = bLeft;

	LocalCharAssets = bIsLeftControlledLocalPlayer ? &LeftCharAssets : &RightCharAssets;
	RivalCharAssets = !bIsLeftControlledLocalPlayer ? &LeftCharAssets : &RightCharAssets;

	InitializeBindDoc(UB2UIDocHelper::GetDocBattle(), UB2UIDocHelper::GetDocPVP1on1Rival());	

	RequestAsyncLoadWidgets();
}

void UB2UIPVP1on1BattleMain::SetLocalPlayerGradeStar(int32 InGrade, int32 InStar)
{
	LocalCharAssets->SetGradeStar(InGrade, InStar);
}

void UB2UIPVP1on1BattleMain::SetRivalPlayerGradeStar(int32 InGrade, int32 InStar)
{
	RivalCharAssets->SetGradeStar(InGrade, InStar);
	RivalCharAssets->IsLocalPlayer = false;
}

void UB2UIPVP1on1BattleMain::SetLocalMyBadgeImage(bool _isLocalPlayer)
{
	LocalCharAssets->SetMyBadgeImage(_isLocalPlayer);
	LocalCharAssets->IsLocalPlayer = true;
}

void UB2UIPVP1on1BattleMain::SetRivalMyBadgeImage(bool _isLocalPlayer)
{
	RivalCharAssets->SetMyBadgeImage(_isLocalPlayer);
}

void UB2UIPVP1on1BattleMain::SetDeathPortrait(const ENetMatchResult& result)
{
	//예외처리 너무 많이 해서....예외처리 패스...
	UB2UIDocHero* DocHero(nullptr);
	//PlayAnimationEvent_AnimResultAction(result); 총 한번만 호출 되어야 한다 ShowResultImage에서 호출
	if (result == ENetMatchResult::LocalWin)
	{
		UB2UIDocPVP1on1Rival* DocPVPRival = UB2UIDocHelper::GetDocPVP1on1Rival();
		if (RivalCharAssets && DocPVPRival)
		{
			RivalCharAssets->SetPortraitMainHero(UB2UIDocHero::GetPlayerDeathPortraitMaterial(this, IntToPCClass(DocPVPRival->GetCurPCClass())));
			RivalCharAssets->SetPortraitSubHero(UB2UIDocHero::GetPlayerDeathPortraitMaterial(this, IntToPCClass(DocPVPRival->GetTagPCClass())));
		}
	}
	else if(result == ENetMatchResult::LocalLose)
	{
		UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
		if(LocalCharAssets && DocBattle)
		{
			LocalCharAssets->SetPortraitMainHero(UB2UIDocHero::GetPlayerDeathPortraitMaterial(this, IntToPCClass(DocBattle->GetCurPCClass())));
			LocalCharAssets->SetPortraitSubHero(UB2UIDocHero::GetPlayerDeathPortraitMaterial(this, IntToPCClass(DocBattle->GetTagPCClass())));
		}
	}
	else //Draw시에는 양쪽다
	{
		UB2UIDocPVP1on1Rival* DocPVPRival = UB2UIDocHelper::GetDocPVP1on1Rival();
		if (RivalCharAssets && DocPVPRival)
		{
			RivalCharAssets->SetPortraitMainHero(UB2UIDocHero::GetPlayerDeathPortraitMaterial(this, IntToPCClass(DocPVPRival->GetCurPCClass())));
			RivalCharAssets->SetPortraitSubHero(UB2UIDocHero::GetPlayerDeathPortraitMaterial(this, IntToPCClass(DocPVPRival->GetTagPCClass())));
		}

		UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
		if (LocalCharAssets && DocBattle)
		{
			LocalCharAssets->SetPortraitMainHero(UB2UIDocHero::GetPlayerDeathPortraitMaterial(this, IntToPCClass(DocBattle->GetCurPCClass())));
			LocalCharAssets->SetPortraitSubHero(UB2UIDocHero::GetPlayerDeathPortraitMaterial(this, IntToPCClass(DocBattle->GetTagPCClass())));
		}
	}

	if (IMG_FocusTimer.IsValid())
	{
		IMG_FocusTimer->SetVisibility(ESlateVisibility::Collapsed);
		StopFocusTimer_BP();
	}
	LocalCharAssets->IMG_FX_TagSuggest->SetVisibility(ESlateVisibility::Hidden);
	RivalCharAssets->IMG_FX_TagSuggest->SetVisibility(ESlateVisibility::Hidden);

	StopTagSuggestAnim_Left_BP();
	StopTagSuggestAnim_Right_BP();
	StopTagSuggestByPlayerConditionAnim_Left_BP();
	StopTagSuggestByPlayerConditionAnim_Right_BP();
}

void UB2UIPVP1on1BattleMain::ShowResultImage(const ENetMatchResult& result)
{
	//[@AKI, 170821] [B2CLT-1888] 모드별 승패 연출 통일 개선
	PlayAnimationEvent_AnimResultAction(result);
	/*
	//혹시 모르니 일단 전부 다 끔
	if (IMG_Victory.IsValid())
	{
		IMG_Victory->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IMG_Lose.IsValid())
	{
		IMG_Lose->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IMG_Draw.IsValid())
	{
		IMG_Draw->SetVisibility(ESlateVisibility::Collapsed);
	} 
	//다 안보이게 한 상태에서 보여야 할 것만 찾아서 보여줌
	if (result == ENetMatchResult::LocalWin)
	{
		if (IMG_Victory.IsValid())
		{
			IMG_Victory->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else if (result == ENetMatchResult::LocalLose)
	{
		if (IMG_Lose.IsValid())
		{
			IMG_Lose->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else //Draw시에는 양쪽다
	{
		if (IMG_Draw.IsValid())
		{
			IMG_Draw->SetVisibility(ESlateVisibility::Visible);
		}
	}
	*/
}

void UB2UIPVP1on1BattleMain::SetLeaveEnemyText(ESlateVisibility InVisible)
{
	if (TB_LeaveEnemy.IsValid())
	{
		TB_LeaveEnemy->SetVisibility(InVisible);
	}
}

void UB2UIPVP1on1BattleMain::InitializeBindDoc(class UB2UIDocBattle* InDocBattleForLocal, class UB2UIDocPVP1on1Rival* InDocBattleForRival)
{
	UnbindDoc();
		
	if (!InDocBattleForLocal || !InDocBattleForRival)
		return;	

	SetDoc(InDocBattleForLocal);

	BindedRivalDoc = InDocBattleForRival;

	LocalCharAssets->CachedDoc = InDocBattleForLocal;
	RivalCharAssets->CachedDoc = InDocBattleForRival;

	//스킬s
	for (uint8 Cnt = 0; Cnt < MAX_SKILL_INPUT; Cnt++)
	{
		FString SkillAssetName = PrefixSkillAsset + FString::FromInt(Cnt);
		auto SkillAsset = GetSlot<UB2UIBattleSkill>(FName(*SkillAssetName));
		if (SkillAsset)
		{
			SkillAssets.Add(SkillAsset);
			SkillAsset->Init();
			SkillAsset->SetIndex(Cnt);	//SkillRT쪽에서는 1베이스로 작업한 듯
			SkillAsset->OnBattleSkillClicked.BindDynamic(this, &UB2UIPVP1on1BattleMain::OnPressedSkillBtn);
		}
	}

	if (InDocBattleForRival->GetIsFriendshipGame())
	{
		LocalCharAssets->SetVisibleGradeStar(false);
		RivalCharAssets->SetVisibleGradeStar(false);
	}
	else
	{
		SetLocalPlayerGradeStar(InDocBattleForLocal->GetTagMatchGrade(), InDocBattleForLocal->GetTagMatchGradeStar());
		SetRivalPlayerGradeStar(InDocBattleForRival->GetTagMatchGrade(), InDocBattleForRival->GetTagMatchGradeStar());
	}

	SetLocalMyBadgeImage(true); //bIsLeftControlledLocalPlayer 였으나 위에서 좌우를 알아서 바꿔주므로 그냥 Local은 true로 하면 됨
	SetRivalMyBadgeImage(false); //!bIsLeftControlledLocalPlayer

	if (UIP_BattleCombo.IsValid())
	{
		UIP_BattleCombo->BindDoc(InDocBattleForLocal);
	}

	if (UIP_BattleVignette.IsValid())
		UIP_BattleVignette->BindDoc(InDocBattleForLocal);

	//시작 할때, Main과 Sub의 클래스를 세팅하고 CurPCClass나 TagPCClass의 값이 바뀔때 두개를 스왑해준다.
	BindDocMainChar(UB2UIDocHelper::GetDocHero(InDocBattleForLocal->GetCurPCClass()));
	BindDocSubChar(UB2UIDocHelper::GetDocHero(InDocBattleForLocal->GetTagPCClass()));

	UpdateAttackButtonStyleToPCClass(IntToPCClass(InDocBattleForLocal->GetCurPCClass()));

	UpdateGuildBuff(&BladeIIGameImpl::GetLocalCharacterData());

#define DELEGATE_CHECK_LEFT_RIGHT(LOCALDOC, RIVALDOC, DELEGATE, THIS, FUNCNAME, BOOL_LEFT)	\
	if(BOOL_LEFT)																			\
	{																						\
		LOCALDOC->DELEGATE##Changed.AddUObject(THIS, &FUNCNAME##_Left);						\
		RIVALDOC->DELEGATE##Changed.AddUObject(THIS, &FUNCNAME##_Right);					\
	}																						\
	else																					\
	{																						\
		LOCALDOC->DELEGATE##Changed.AddUObject(THIS, &FUNCNAME##_Right);					\
		RIVALDOC->DELEGATE##Changed.AddUObject(THIS, &FUNCNAME##_Left);						\
	}


	DELEGATE_CHECK_LEFT_RIGHT(InDocBattleForLocal, InDocBattleForRival, OnCurPCClass, this, UB2UIPVP1on1BattleMain::OnChangedCurPCClass, bIsLeftControlledLocalPlayer);
	DELEGATE_CHECK_LEFT_RIGHT(InDocBattleForLocal, InDocBattleForRival, OnCurrPCHealth, this, UB2UIPVP1on1BattleMain::OnChangedCurrPCHealth, bIsLeftControlledLocalPlayer);
	DELEGATE_CHECK_LEFT_RIGHT(InDocBattleForLocal, InDocBattleForRival, OnCurrPCMaxHealth, this, UB2UIPVP1on1BattleMain::OnChangedCurrPCMaxHealth, bIsLeftControlledLocalPlayer);
	DELEGATE_CHECK_LEFT_RIGHT(InDocBattleForLocal, InDocBattleForRival, OnCurrPCShield, this, UB2UIPVP1on1BattleMain::OnChangedCurrPCShield, bIsLeftControlledLocalPlayer);
	DELEGATE_CHECK_LEFT_RIGHT(InDocBattleForLocal, InDocBattleForRival, OnTagPCHealth, this, UB2UIPVP1on1BattleMain::OnChangedTagPCHealth, bIsLeftControlledLocalPlayer);
	DELEGATE_CHECK_LEFT_RIGHT(InDocBattleForLocal, InDocBattleForRival, OnTagPCMaxHealth, this, UB2UIPVP1on1BattleMain::OnChangedTagPCMaxHealth, bIsLeftControlledLocalPlayer);
	DELEGATE_CHECK_LEFT_RIGHT(InDocBattleForLocal, InDocBattleForRival, OnIsCountingTagCooltime, this, UB2UIPVP1on1BattleMain::OnChangedIsCountingTagCooltime, bIsLeftControlledLocalPlayer);
	DELEGATE_CHECK_LEFT_RIGHT(InDocBattleForLocal, InDocBattleForRival, OnIsTagPossible, this, UB2UIPVP1on1BattleMain::OnChangedIsTagPossible, bIsLeftControlledLocalPlayer);
	DELEGATE_CHECK_LEFT_RIGHT(InDocBattleForLocal, InDocBattleForRival, OnIsTagDisabledByCooltime, this, UB2UIPVP1on1BattleMain::OnChangedIsTagDisabledByCooltime, bIsLeftControlledLocalPlayer);
	DELEGATE_CHECK_LEFT_RIGHT(InDocBattleForLocal, InDocBattleForRival, OnRemainingTagCooltime, this, UB2UIPVP1on1BattleMain::OnChangedRemainingTagCooltime, bIsLeftControlledLocalPlayer);
	DELEGATE_CHECK_LEFT_RIGHT(InDocBattleForLocal, InDocBattleForRival, OnIsTagSuggested, this, UB2UIPVP1on1BattleMain::OnChangedIsTagSuggested, bIsLeftControlledLocalPlayer);
	InDocBattleForRival->OnCurPCLevelChanged.AddUObject(this, bIsLeftControlledLocalPlayer ? &UB2UIPVP1on1BattleMain::OnChangedCurPCLevel_Right : &UB2UIPVP1on1BattleMain::OnChangedCurPCLevel_Left);
	InDocBattleForLocal->OnOffenseEtherCoolTimeChanged.AddUObject(this, &UB2UIPVP1on1BattleMain::OnChangedOffenseEtherCoolTime);
	InDocBattleForLocal->OnDefenseEtherCoolTimeChanged.AddUObject(this, &UB2UIPVP1on1BattleMain::OnChangedDefenseEtherCoolTime);
	InDocBattleForLocal->OnIsOffenseEtherChanged.AddUObject(this, &UB2UIPVP1on1BattleMain::OnChangedIsOffenseEther);
	InDocBattleForLocal->OnIsDefenseEtherChanged.AddUObject(this, &UB2UIPVP1on1BattleMain::OnChangedIsDefenseEther);
	InDocBattleForLocal->SetIsOffenseEther(InDocBattleForLocal->GetIsOffenseEther());
	InDocBattleForLocal->SetIsDefenseEther(InDocBattleForLocal->GetIsDefenseEther());
	InDocBattleForLocal->OnHasAbnormalBuffChanged.AddUObject(this, &UB2UIPVP1on1BattleMain::OnAbnormalStateChanged);
	InDocBattleForLocal->OnCharacterBuffStateChanged.AddUObject(this, &UB2UIPVP1on1BattleMain::OnChangedCharacterSkillBuffState);
	InDocBattleForLocal->OnGuardCoolTimeChanged.AddUObject(this, &UB2UIPVP1on1BattleMain::OnChangedGuradCoolTime);
	InDocBattleForLocal->OnHasNotTagDefenceChanged.AddUObject(this, &UB2UIPVP1on1BattleMain::OnChangedHasNotTagDefence);
}

void UB2UIPVP1on1BattleMain::BindDocMainChar(UB2UIDocHero* InDocHero)
{
	// 로비에서의 메인 캐릭터 선택이 아니라 현재 등장한 캐릭터. 게임 시작 처음에는 실제 메인 캐릭터임.

	MainHeroDoc = InDocHero;
	if (!InDocHero) return;

	//스킬 처리. 근데 현재 4개의 스킬만 처리되고 있고 "공격"과 "방어"버튼은 스킬로 취급하고 있지 않는데 그래되 되려나 -..-
	for (int SkillCnt = 0; SkillCnt < SkillAssets.Num(); ++SkillCnt)
	{
		UB2UIBattleSkill* SkillAsset = SkillAssets[SkillCnt];
		int32 SkillInputIndex = SkillAsset->GetIndex();

		auto SkillDoc = InDocHero->GetSkillDocByInputIndex(SkillInputIndex);
		BindDocSkill(SkillInputIndex, SkillDoc);
	}

	//초상화 처리
	check(LocalCharAssets);

	EPCClass pcClass = InDocHero->GetHeroClass();

	LocalCharAssets->SetPortraitMainHero(UB2UIDocHero::GetPlayerPortraitMaterial(this, pcClass));

	auto DocBattle = Cast<UB2UIDocBattle>(GetDoc());
	if (DocBattle)
		LocalCharAssets->SetNameMainChar(DocBattle);
	else
		LocalCharAssets->SetNameMainChar(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(pcClass));

	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
	//LocalCharAssets->SetLevelMainChar(InDocHero->GetCurrentLevel());
	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
	const int32 CharLevel = CharacterDataStore.GetCharacterLevel(pcClass);
	LocalCharAssets->SetLevelMainChar(CharLevel);
}

void UB2UIPVP1on1BattleMain::BindDocSubChar(UB2UIDocHero* InDocHero)
{
	// 로비에서의 서브 캐릭터 선택이 아니라 현재 대기중인 캐릭터. 게임 시작 처음에는 실제 서브 캐릭터임.
	SubHeroDoc = InDocHero;

	if (!InDocHero)
		return;

	//초상화 처리.
	check(LocalCharAssets);

	LocalCharAssets->SetPortraitSubHero(UB2UIDocHero::GetPlayerPortraitMaterial(this, InDocHero->GetHeroClass()));
}

void UB2UIPVP1on1BattleMain::UnbindDoc()
{
	for (int SkillCnt = 0; SkillCnt < SkillAssets.Num(); ++SkillCnt)
	{
		UB2UIBattleSkill* SkillAsset = SkillAssets[SkillCnt];
		SkillAsset->OnBattleSkillClicked.Unbind();
		SkillAsset->UnbindDoc();
	}

	if (UIP_BattleCombo.IsValid())
		UIP_BattleCombo->UnbindDoc();

	if (UIP_BattleVignette.IsValid())
		UIP_BattleVignette->UnbindDoc();

	auto DocBattle = Cast<UB2UIDocBattle>(GetDoc());
	if (DocBattle)
	{
		DocBattle->OnInAutoPlayChanged.RemoveAll(this);
	}

#define DELEGATE_REMOVE(DELEGATE, THIS)		\
	if(DocBattle) DocBattle->DELEGATE##Changed.RemoveAll(THIS);		\
	if(BindedRivalDoc) BindedRivalDoc->DELEGATE##Changed.RemoveAll(THIS);

	DELEGATE_REMOVE(OnCurPCClass, this);
	DELEGATE_REMOVE(OnCurrPCHealth, this);
	DELEGATE_REMOVE(OnCurrPCMaxHealth, this);
	DELEGATE_REMOVE(OnCurrPCShield, this);
	DELEGATE_REMOVE(OnTagPCHealth, this);
	DELEGATE_REMOVE(OnTagPCMaxHealth, this);
	DELEGATE_REMOVE(OnIsCountingTagCooltime, this);
	DELEGATE_REMOVE(OnIsTagPossible, this);
	DELEGATE_REMOVE(OnIsTagDisabledByCooltime, this);
	DELEGATE_REMOVE(OnRemainingTagCooltime, this);

	if (DocBattle)
	{
		DocBattle->OnOffenseEtherCoolTimeChanged.RemoveAll(this);
		DocBattle->OnDefenseEtherCoolTimeChanged.RemoveAll(this);
		DocBattle->OnIsOffenseEtherChanged.RemoveAll(this);
		DocBattle->OnIsDefenseEtherChanged.RemoveAll(this);
		DocBattle->OnHasAbnormalBuffChanged.RemoveAll(this);
	}
		
	if (BindedRivalDoc)
		BindedRivalDoc->OnCurPCLevelChanged.RemoveAll(this);

	MainHeroDoc = nullptr;
	SubHeroDoc = nullptr;

	SetDoc(nullptr);

	BindedRivalDoc = nullptr;
}

void UB2UIPVP1on1BattleMain::BindDocSkill(int32 SkillInputIndex, UB2UIDocSkill* SkillDoc)
{
	UB2UIBattleSkill* SkillAsset = SkillAssets.IsValidIndex(SkillInputIndex) ? SkillAssets[SkillInputIndex] : nullptr;
	if (SkillAsset)
	{
		SkillAsset->SetEmpty(SkillDoc == nullptr);
		if (SkillDoc)
			SkillAsset->BindDoc(SkillDoc);
		else
			SkillAsset->UnbindDoc();
	}
}


void UB2UIPVP1on1BattleMain::SetOffenseEtherlCooltime(float InCoolTime)
{
	if (TagCooltimeDisplayMID)
	{
		if (auto DocBattle = Cast<UB2UIDocBattle>(GetDoc()))
		{
			TagCooltimeDisplayMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, InCoolTime / DocBattle->GetOffenseEtherMaxCoolTime());
		}
	}
}

void UB2UIPVP1on1BattleMain::SetDefenseEtherCooltime(float InCoolTime)
{
	if (TagCooltimeDisplayMID)
	{
		if (auto DocBattle = Cast<UB2UIDocBattle>(GetDoc()))
		{
			TagCooltimeDisplayMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, InCoolTime / DocBattle->GetDefenseEtherMaxCoolTime());
		}
	}
}

void UB2UIPVP1on1BattleMain::SetEnableOffenseEther(bool InEnable)
{
}

void UB2UIPVP1on1BattleMain::SetEnableDefenseEther(bool InEnable)
{

}

void UB2UIPVP1on1BattleMain::UpdateSkillBuffIcons()
{
	bool bOnlyCountTimeChanged = false;
	CollectAndSortSkillBuffData(bOnlyCountTimeChanged);
	UpdateSkillBuffIconsByCollectedData(bOnlyCountTimeChanged);
}

void UB2UIPVP1on1BattleMain::CollectAndSortSkillBuffData(bool& bOutOnlyCountTimeChanged)
{
	UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc());
	if (DocBattle)
	{
		TArray<FUISkillBuffDisplayInfo_United> PrevSkillBuffData = AllCollectedSkillBuffData; // OnlyTimeChanged 검사를 위해.
		AllCollectedSkillBuffData.Empty();

		const TArray<FUISkillBuffDisplayInfo_Character>& CharacterSkillBuffInfo = DocBattle->GetCharacterBuffState();
		for (const FUISkillBuffDisplayInfo_Character& ThisStateDamageBuff : CharacterSkillBuffInfo)
		{
			if (ThisStateDamageBuff.MyType != EBuffType::Buff_None && ThisStateDamageBuff.MyType != EBuffType::Buff_End)
			{
				EUIUnitedSkillBuffType BuffIconType = UnitedSkillBT_from_CharacterBT(ThisStateDamageBuff.MyType);

				if (BuffIconType == EUIUnitedSkillBuffType::EUUSBT_End)
					continue;

				FUISkillBuffDisplayInfo_United NewSet;
				NewSet.MyType = BuffIconType;
				NewSet.LastStartedTime = ThisStateDamageBuff.LastStartedTime;
				NewSet.RemainingTime = ThisStateDamageBuff.RemainingTime;
				AllCollectedSkillBuffData.Add(NewSet);
			}
		}

		// 마지막으로 생성 시점에 따라 정렬. 먼저 생성된 것이 앞으로 오도록.
		AllCollectedSkillBuffData.Sort([](const FUISkillBuffDisplayInfo_United& A, const FUISkillBuffDisplayInfo_United& B)->bool
		{
			return A.LastStartedTime < B.LastStartedTime;
		});

		// 카운트 시간만 바뀌었는지, Buff 항목 자체가 바뀐 게 있는지 검사.
		bOutOnlyCountTimeChanged = true;
		if (PrevSkillBuffData.Num() != AllCollectedSkillBuffData.Num())
		{ // 항목 수가 다르면 물론 더 볼 필요 없고.
			bOutOnlyCountTimeChanged = false;
		}
		else
		{
			for (const FUISkillBuffDisplayInfo_United& ThisPrevBuff : PrevSkillBuffData)
			{
				bool bFoundSameBuff = false;
				for (const FUISkillBuffDisplayInfo_United& ThisCurrBuff : AllCollectedSkillBuffData)
				{
					if (ThisPrevBuff.MyType == ThisCurrBuff.MyType) {
						bFoundSameBuff = true;
						break;
					}
				}

				if (!bFoundSameBuff) // 하나라도 같은 걸 못 찾은 게 있다면 카운트 타임 외에 바뀐 게 있는 거
				{
					bOutOnlyCountTimeChanged = false;
					break;
				}
			}
		}
	}
}
void UB2UIPVP1on1BattleMain::UpdateSkillBuffIconsByCollectedData(bool bUpdateCountTimeOnly)
{
	if (!bUpdateCountTimeOnly)
	{ // 이 경우는 일단 기존 것들을 제거. 이걸 거치지 않았다면 기존에 있는 것들을 찾아서 값만 바꾸게 될 것.
		DestroyAllMyBuffIcons_Skill();
	}

	for (const FUISkillBuffDisplayInfo_United& ThisBuff : AllCollectedSkillBuffData) // LastStartedTime 에 따라 정렬되어 있어야 함.
	{
		CreateOrUpdateSkillBuffIcon(ThisBuff.MyType, ThisBuff.RemainingTime);
	}
}

UB2UIInGameBuffIcon_Skill* UB2UIPVP1on1BattleMain::CreateOrUpdateSkillBuffIcon(EUIUnitedSkillBuffType InType, float RemainingTime)
{
	UB2UIInGameBuffIcon_Skill* CreatedOrFound = NULL;
	for (UB2UIInGameBuffIcon* ThisBuffIcon : AllMyBuffIcons_Skill)
	{
		UB2UIInGameBuffIcon_Skill* CastedSkillBuffIcon = Cast<UB2UIInGameBuffIcon_Skill>(ThisBuffIcon);
		if (CastedSkillBuffIcon && CastedSkillBuffIcon->GetBuffType() == InType)
		{
			CreatedOrFound = CastedSkillBuffIcon;
			break;
		}
	}

	if (!CreatedOrFound)
	{
		CreatedOrFound = Cast<UB2UIInGameBuffIcon_Skill>(DynCreateInHorizontalBox(BuffIconClass_Skill, this, LocalCharAssets->HB_BuffIconSet.Get(), BuffIconSize, FMargin()));
		if (CreatedOrFound)
		{
			CreatedOrFound->Init();
			CreatedOrFound->SetBuffType(InType);
			AllMyBuffIcons_Skill.Add(CreatedOrFound);
		}
	}

	if (CreatedOrFound)
	{
		CreatedOrFound->SetRemainigTime(RemainingTime);
	}

	return CreatedOrFound;
}
UB2UIInGameBuffIcon_GuildBuff* UB2UIPVP1on1BattleMain::CreateOrUpdateGuildBuffIcon(EItemOption InBuffType)
{
	UB2UIInGameBuffIcon_GuildBuff* CreatedOrFound = nullptr;
	for (UB2UIInGameBuffIcon* ThisBuffIcon : AllMyBuffIcons_Guild)
	{
		UB2UIInGameBuffIcon_GuildBuff* CastedSkillBuffIcon = Cast<UB2UIInGameBuffIcon_GuildBuff>(ThisBuffIcon);
		if (CastedSkillBuffIcon && CastedSkillBuffIcon->GetBuffType() == InBuffType)
		{
			CreatedOrFound = CastedSkillBuffIcon;
			break;
		}
	}

	if (!CreatedOrFound)
	{
		if (BuffIconClass_Guild.Get() != nullptr)
		{
			CreatedOrFound = Cast<UB2UIInGameBuffIcon_GuildBuff>(DynCreateInHorizontalBox(BuffIconClass_Guild, this, LocalCharAssets->HB_Guild.Get(), BuffIconSize, FMargin()));

			if (CreatedOrFound != nullptr)
			{
				CreatedOrFound->Init();
				CreatedOrFound->SetBuffType(InBuffType);
				AllMyBuffIcons_Guild.Add(CreatedOrFound);
			}
		}
	}

	return CreatedOrFound;
}

void UB2UIPVP1on1BattleMain::DestroyAllMyBuffIcons_Skill()
{
	for (UB2UIInGameBuffIcon* ThisBuffIcon : AllMyBuffIcons_Skill)
	{
		if (ThisBuffIcon)
		{
			ThisBuffIcon->UnbindDoc();
			ThisBuffIcon->DestroySelf(UB2UIManager::GetInstance());
		}
	}
	AllMyBuffIcons_Skill.Empty();
}
void UB2UIPVP1on1BattleMain::DestroyAllMyBuffIcons_Guild()
{
	for (UB2UIInGameBuffIcon* ThisBuffIcon : AllMyBuffIcons_Guild)
	{
		if (ThisBuffIcon)
		{
			ThisBuffIcon->UnbindDoc();
			ThisBuffIcon->DestroySelf(UB2UIManager::GetInstance());
		}
	}
	AllMyBuffIcons_Guild.Empty();
}


void UB2UIPVP1on1BattleMain::OnChangedIsDefenseEther(class UB2UIDocBase* Sender, bool IsDefenseEther, bool PrevIsDefenseEther)
{
	if (UIP_DefenseEtherIcon.IsValid())
	{
		if (IsDefenseEther)
		{
			if(UIP_DefenseEtherIcon->GetVisibility() == ESlateVisibility::Collapsed)
				UIP_DefenseEtherIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UIP_DefenseEtherIcon->Init();
		}
		else if (!IsDefenseEther)
		{
			if(UIP_DefenseEtherIcon->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
				UIP_DefenseEtherIcon->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
void UB2UIPVP1on1BattleMain::OnChangedIsOffenseEther(class UB2UIDocBase* Sender, bool IsOffenseEther, bool PrevIsOffenseEther)
{
	if (UIP_OffenseEtherIcon.IsValid())
	{
		if (IsOffenseEther)
		{ 
			if(UIP_OffenseEtherIcon->GetVisibility() == ESlateVisibility::Collapsed)
				UIP_OffenseEtherIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UIP_OffenseEtherIcon->Init();
		}
		else if (!IsOffenseEther)
		{
			if(UIP_OffenseEtherIcon->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
				UIP_OffenseEtherIcon->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIPVP1on1BattleMain::OnAbnormalStateChanged(class UB2UIDocBase* Sender, bool NewValue, bool PrevValue)
{
	const bool IsAbnormalState = NewValue;

	for (uint8 Cnt = 0; Cnt < MAX_SKILL_INPUT; Cnt++)
	{
		// Freezing/Stun 상태에서 벗어나는 목적인 SurviveSkill은 Disable 시키지 않음
		if (SkillAssets.IsValidIndex(Cnt) && SkillAssets[Cnt] != nullptr)
		{
			auto* SkillAsset = SkillAssets[Cnt];
			if (SkillAsset->IsSurviveSkill() == false && SkillAsset->IsInCooltime() == false)
				SkillAsset->SetEnabledBtn(!IsAbnormalState);
		}
	}
}

void UB2UIPVP1on1BattleMain::OnChangedOffenseEtherCoolTime(class UB2UIDocBase* Sender, float InRemainingCooltime, float InPrevRemainingCooltime)
{
	if (UIP_OffenseEtherIcon.IsValid())
	{
		if (UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc()))
		{
			UIP_OffenseEtherIcon->UpdateCoolTime(DocBattle->GetOffenseEtherMaxCoolTime(), InRemainingCooltime, DocBattle->GetEnableOffenseEther());
		}
	}
}

void UB2UIPVP1on1BattleMain::OnChangedDefenseEtherCoolTime(class UB2UIDocBase* Sender, float InRemainingCooltime, float InPrevRemainingCooltime)
{
	if (UIP_DefenseEtherIcon.IsValid())
	{
		if (UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc()))
		{
			UIP_DefenseEtherIcon->UpdateCoolTime(DocBattle->GetDefenseEtherMaxCoolTime(), InRemainingCooltime, DocBattle->GetEnableDefenseEther());
		}
	}
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UIPVP1on1BattleMain::OnPressedSkillBtn(UB2UIBattleSkill* ClickedSkill)
{
	int32 SkillIndex = ClickedSkill->GetIndex();
	switch (SkillIndex)
	{
	case 0: PlayerStartSkill01Class<>::GetInstance().Signal(); break;
	case 1: PlayerStartSkill02Class<>::GetInstance().Signal(); break;
	case 2: PlayerStartSkill03Class<>::GetInstance().Signal(); break;
	case 3: PlayerStartWeaponSkillClass<>::GetInstance().Signal(); break;
	}
}

void UB2UIPVP1on1BattleMain::OnPressedAttackBtn()
{
	PlayerStartAttackClass<>::GetInstance().Signal();
}

void UB2UIPVP1on1BattleMain::CloseWidgetDelegate()
{
	OnClickedPauseBtn();
}

void UB2UIPVP1on1BattleMain::OnClickedPauseBtn()
{
	bClickedPause = true;

	StartPauseMenuClass<EB2GameMode>::GetInstance().Signal(GetB2GameModeType(this));
}

void UB2UIPVP1on1BattleMain::OnReleasedAttackBtn()
{
	PlayerStopAttackClass<>::GetInstance().Signal();
}

void UB2UIPVP1on1BattleMain::OnPressedGuardBtn()
{
	PlayerStartGuardClass<>::GetInstance().Signal();
}

void UB2UIPVP1on1BattleMain::OnReleasedGuardBtn()
{
	PlayerStopGuardClass<>::GetInstance().Signal();
}

void UB2UIPVP1on1BattleMain::OnClickedTagBtn()
{
	//if (bIsLeftControlledLocalPlayer)
		PlayerStartTagClass<>::GetInstance().Signal();
}

void UB2UIPVP1on1BattleMain::OnChangedCharacterSkillBuffState(UB2UIDocBase * Sender, TArray<FUISkillBuffDisplayInfo_Character> CurrentState, TArray<FUISkillBuffDisplayInfo_Character> Prevstate)
{
	UpdateSkillBuffIcons();
}

//====================================================Doc Delegate
void UB2UIPVP1on1BattleMain::OnChangedCurPCClass_Left(class UB2UIDocBase* Sender, int32 CurPCClass, int32 PrevCurPCClass)
{
	if (bIsLeftControlledLocalPlayer)
	{
		OnChangedCurPCClassCommon(CurPCClass, PrevCurPCClass);
	}
	else
	{
		LeftCharAssets.SetPortraitMainHero(this, BindedRivalDoc);
		LeftCharAssets.SetNameMainChar(BindedRivalDoc);
		LeftCharAssets.SetLevelMainChar(BindedRivalDoc->GetCurPCLevel());
		LeftCharAssets.SetPortraitSubHero(this, BindedRivalDoc);
	}
}

void UB2UIPVP1on1BattleMain::OnChangedCurPCClass_Right(class UB2UIDocBase* Sender, int32 CurPCClass, int32 PrevCurPCClass)
{
	if (!bIsLeftControlledLocalPlayer)
	{
		OnChangedCurPCClassCommon(CurPCClass, PrevCurPCClass);
	}
	else
	{
		RightCharAssets.SetPortraitMainHero(this, BindedRivalDoc);
		RightCharAssets.SetNameMainChar(BindedRivalDoc);
		RightCharAssets.SetLevelMainChar(BindedRivalDoc->GetCurPCLevel());
		RightCharAssets.SetPortraitSubHero(this, BindedRivalDoc);
	}
}

void UB2UIPVP1on1BattleMain::OnChangedCurPCClassCommon(int32 CurPCClass, int32 PrevCurPCClass)
{
	if (MainHeroDoc && SubHeroDoc)
	{
		//씹어야 되는 케이스
		if (MainHeroDoc->GetDocId() == CurPCClass/* || SubHeroDoc->GetDocId() == DocBattle->GetTagPCClass()*/)
			return;

		BindDocMainChar(UB2UIDocHelper::GetDocHero(CurPCClass));
		BindDocSubChar(UB2UIDocHelper::GetDocHero(PrevCurPCClass));

		if (!MainHeroDoc || !SubHeroDoc) return;

		UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc());

		EPCClass MainHeroClass = IntToPCClass(CurPCClass);
		EPCClass SubHeroClass = IntToPCClass(PrevCurPCClass);
		bool bIsTagAttack = (DocBattle ? DocBattle->GetbTagAttackPending() : false);

		BeginTagAnim(MainHeroClass, bIsTagAttack);
		Tag_BP(MainHeroClass, SubHeroClass, bIsTagAttack); // B2UIBattleTagAnim 에서 태그 애니메이션 관련한 모든 걸 처리하지는 않는다.
	}
	UpdateAttackButtonStyleToPCClass(IntToPCClass(CurPCClass));
}

void UB2UIPVP1on1BattleMain::UpdateGuildBuff(ICharacterDataStore* CharacterDataStore)
{
	if (CharacterDataStore != nullptr)
	{
		TArray<b2network::B2GuildSkillPtr> SortArray;
		SortArray.Append(CharacterDataStore->GetGuildSkillInfos());

		bool IsLeft = bIsLeftControlledLocalPlayer;

		SortArray.Sort([IsLeft](auto& a, auto& b) {
			auto aData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(a->id, a->level);
			auto bData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(b->id, b->level);

			if (aData && bData)
			{
				auto GetSortIndex = [](int32 OptionType) { return B2UIUtil::GetGuildBuffSortValue(SvrToCliOptionType(OptionType)); };
				if(IsLeft)
					return GetSortIndex(aData->buff_option_type) > GetSortIndex(bData->buff_option_type);
				else
					return GetSortIndex(aData->buff_option_type) < GetSortIndex(bData->buff_option_type);
			}
			return false;
		});

		for (const auto& GuildSkill : SortArray)
		{
			auto GuildSkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(GuildSkill->id, GuildSkill->level);
			if (GuildSkillMasterData != nullptr && CombatStatEval::IsActiveGuildBuff(CharacterDataStore, SvrToCliOptionType(GuildSkillMasterData->buff_option_type)))
				CreateOrUpdateGuildBuffIcon(SvrToCliOptionType(GuildSkillMasterData->buff_option_type));

		}
	}
}

void UB2UIPVP1on1BattleMain::OnChangedGuradCoolTime(class UB2UIDocBase* Sender, float GuradCoolTime, float PrevGuradCoolTime)
{
	if (TB_GuradCoolTime.IsValid())
	{
		TB_GuradCoolTime->SetText(GetAutoFractionalFormattedText(GuradCoolTime, 0));
	}
}

void UB2UIPVP1on1BattleMain::OnChangedHasNotTagDefence(class UB2UIDocBase* Sender, bool HasNotTagDefence, bool PrevHasNotTagDefence)
{
	SetGuardUI(HasNotTagDefence);
}

void UB2UIPVP1on1BattleMain::SetGuardUI(bool HasNotTagDefence)
{
	if (HasNotTagDefence == true)
	{
		if (CP_GuardDisable.IsValid())
		{
			CP_GuardDisable->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (CP_GuardEnable.IsValid())
		{
			CP_GuardEnable->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BTN_Guard.IsValid())
		{
			BTN_Guard->SetIsEnabled(false);
		}

		PlayAnim_Ether_OblivionAnim_BP();
	}
	else
	{
		if (CP_GuardDisable.IsValid())
		{
			CP_GuardDisable->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (CP_GuardEnable.IsValid())
		{
			CP_GuardEnable->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (BTN_Guard.IsValid())
		{
			BTN_Guard->SetIsEnabled(true);
		}
	}
}

void UB2UIPVP1on1BattleMain::OnChangedCurPCLevel_Left(UB2UIDocBase* Sender, int32 CurPCLevel, int32 PrevPCLevel)
{
	LeftCharAssets.SetLevelMainChar(CurPCLevel);
}
void UB2UIPVP1on1BattleMain::OnChangedCurPCLevel_Right(UB2UIDocBase* Sender, int32 CurPCLevel, int32 PrevPCLevel)
{
	RightCharAssets.SetLevelMainChar(CurPCLevel);
}

#define DEFINITION_PVP_ENTRY_FUNC_DOC(FUNCNAME, PARAMTYPE, CALLFUNC)						\
	void FUNCNAME##_Left(class UB2UIDocBase* Sender, PARAMTYPE cur, PARAMTYPE prev)			\
	{																						\
		LeftCharAssets.CALLFUNC(Sender);													\
	}																						\
	void FUNCNAME##_Right(class UB2UIDocBase* Sender, PARAMTYPE cur, PARAMTYPE prev)		\
	{																						\
		RightCharAssets.CALLFUNC(Sender);													\
	}																									

#define DEFINITION_PVP_ENTRY_FUNC_PARAM(FUNCNAME, PARAMTYPE, CALLFUNC)						\
	void FUNCNAME##_Left(class UB2UIDocBase* Sender, PARAMTYPE cur, PARAMTYPE prev)			\
	{																						\
		LeftCharAssets.CALLFUNC(cur);														\
	}																						\
	void FUNCNAME##_Right(class UB2UIDocBase* Sender, PARAMTYPE cur, PARAMTYPE prev)		\
	{																						\
		RightCharAssets.CALLFUNC(cur);														\
	}	

DEFINITION_PVP_ENTRY_FUNC_DOC(UB2UIPVP1on1BattleMain::OnChangedCurrPCHealth, float, SetHPMainChar)
DEFINITION_PVP_ENTRY_FUNC_DOC(UB2UIPVP1on1BattleMain::OnChangedCurrPCMaxHealth, float, SetHPMainChar)
DEFINITION_PVP_ENTRY_FUNC_DOC(UB2UIPVP1on1BattleMain::OnChangedCurrPCShield, float, SetHPMainChar)
DEFINITION_PVP_ENTRY_FUNC_DOC(UB2UIPVP1on1BattleMain::OnChangedTagPCHealth, float, SetHPSubChar)
DEFINITION_PVP_ENTRY_FUNC_DOC(UB2UIPVP1on1BattleMain::OnChangedTagPCMaxHealth, float, SetHPSubChar)
DEFINITION_PVP_ENTRY_FUNC_PARAM(UB2UIPVP1on1BattleMain::OnChangedIsCountingTagCooltime, bool, SetIsCountingTagCooltime)
// TagDisabledByCooltime 시점부터 쿨타임 텍스트를 보이기 시작. 당장 카운트는 하지 않더라도
DEFINITION_PVP_ENTRY_FUNC_PARAM(UB2UIPVP1on1BattleMain::OnChangedIsTagDisabledByCooltime, bool, SetTagCooltimeVisible)
DEFINITION_PVP_ENTRY_FUNC_PARAM(UB2UIPVP1on1BattleMain::OnChangedIsTagPossible, bool, SetTagPossible)
DEFINITION_PVP_ENTRY_FUNC_PARAM(UB2UIPVP1on1BattleMain::OnChangedRemainingTagCooltime, float, SetTagCooltime)

void UB2UIPVP1on1BattleMain::OnChangedIsTagSuggested_Left(class UB2UIDocBase* Sender, bool bTagSuggested, bool bPrevTagSuggested)
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	const bool bIsForTagAttack = DocBattle ? DocBattle->GetIsTagAttackSuggested() : false; // 이게 아니면 플레이어 컨디션 (HP) 에 의한 것. IsTagAttackSuggested 를 여기서 체크할 수 있도록 IsTagSuggested 보다 먼저 세팅해야 함.
	if (bTagSuggested != bPrevTagSuggested)
	{
		if (LeftCharAssets.IMG_FX_TagSuggest.IsValid())
			LeftCharAssets.IMG_FX_TagSuggest->SetVisibility(bTagSuggested ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

		if (bTagSuggested && bIsLeftControlledLocalPlayer) {
			PlayTagSuggestAnim_Left_BP();

			if (!bIsForTagAttack) {
				PlayTagSuggestByPlayerConditionAnim_Left_BP();
			}
		}
		else
		{
			StopTagSuggestAnim_Left_BP();
			StopTagSuggestByPlayerConditionAnim_Left_BP(); // 여기에 HP 로 인한 태그권장 상태임을 명시하는 조건이 있다면 얘기가 다르긴 한데..
		}
	}
}

void UB2UIPVP1on1BattleMain::OnChangedIsTagSuggested_Right(class UB2UIDocBase* Sender, bool bTagSuggested, bool bPrevTagSuggested)
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	const bool bIsForTagAttack = DocBattle ? DocBattle->GetIsTagAttackSuggested() : false; // 이게 아니면 플레이어 컨디션 (HP) 에 의한 것. IsTagAttackSuggested 를 여기서 체크할 수 있도록 IsTagSuggested 보다 먼저 세팅해야 함.
	if (bTagSuggested != bPrevTagSuggested)
	{
		if (RightCharAssets.IMG_FX_TagSuggest.IsValid())
			RightCharAssets.IMG_FX_TagSuggest->SetVisibility(bTagSuggested ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

		if (bTagSuggested && !bIsLeftControlledLocalPlayer) {
			PlayTagSuggestAnim_Right_BP();

			if (!bIsForTagAttack) {
				PlayTagSuggestByPlayerConditionAnim_Right_BP();
			}
		}
		else
		{
			StopTagSuggestAnim_Right_BP();
			StopTagSuggestByPlayerConditionAnim_Right_BP(); // 여기에 HP 로 인한 태그권장 상태임을 명시하는 조건이 있다면 얘기가 다르긴 한데..
		}
	}
}

void UB2UIPVP1on1BattleMain::BeginTagAnim(EPCClass NewTaggedClass, bool bIsTagAttack)
{
	DestroyTagAnimUIP(false); // 정상적인 상황이면 이전에 플레이하던 게 종료되고 생성했던 건 콜백받아 이미 destroy 되어 있어야.. 그러나 destroy 이전에 태그를 또 했을 수도.

	if (bClickedPause || bIsTagAttack) // Pause를 눌렀거나 연계 태그는 이거 안 함.
		return;

	CreatedTagAnimUIP = LoadAndBeginTAssetBattleTagAnimUIP<UB2UIPVP1on1BattleMain>(this, TagAnimUIPClass, &LoadedTagAnimUIPClass, NewTaggedClass, bIsTagAttack);
}

void UB2UIPVP1on1BattleMain::DestroyTagAnimUIP(bool bUnloadTAsset)
{
	if (bUnloadTAsset)
	{
		UnloadTAssetBattleTagAnimUIP(TagAnimUIPClass, &LoadedTagAnimUIPClass);
		LoadedTagAnimUIPClass = NULL;
		if (TagAnimUIPAsyncHandle.IsValid())
		{
			TagAnimUIPAsyncHandle->ReleaseHandle();
		}
	}
	if (CreatedTagAnimUIP)
	{
		CreatedTagAnimUIP->DestroySelf(UB2UIManager::GetInstance());
		CreatedTagAnimUIP = NULL;
	}
}

void UB2UIPVP1on1BattleMain::ProcessEvadeCoolTimeUI()
{
	//if (IMG_EvadeSkillCooltime.IsValid())
	//{
	//	ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));

	//	if (pPlayer)
	//	{
	//		//쿨타임 머터리얼 처리
	//		UMaterialInstanceDynamic* pMID = IMG_EvadeSkillCooltime->GetDynamicMaterial();

	//		if (pMID)
	//		{
	//			pMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, pPlayer->GetEvadeCoolTimeProgress());
	//		}
	//	}
	//}
}

void UB2UIPVP1on1BattleMain::RequestAsyncLoadWidgets()
{
	if (auto* AssetLoader = UB2AssetLoader::Get())
	{
		auto LoadCallback = FB2AsyncLoadingComplete::CreateUObject(this, &UB2UIPVP1on1BattleMain::OnCompleteAsyncLoad);
		AssetLoader->RequestAsyncLoad(TEXT("TagAnimUIP"), TagAnimUIPClass, LoadCallback);
	}
}

void UB2UIPVP1on1BattleMain::OnCompleteAsyncLoad(const FString& RequesetName)
{
	if (RequesetName == TEXT("TagAnimUIP"))
		LoadedTagAnimUIPClass = TagAnimUIPClass.Get();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (UB2AssetLoader::bEnableDevLog)
	{
		BII_SCREEN_LOG(FString::Printf(TEXT("[UB2UIBattleMain] AsyncLoad request of %s completed."), *RequesetName),
			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 8.0f);
	}
#endif

}


void UB2UIPVP1on1BattleMain::OnBattleTagAnimUIPExpired(UB2UIBattleTagAnim* ExpiredUIP)
{// UB2UIBattleTagAnim 쪽으로부터 오는 신호. 메모리 절약을 위해 다 되었으면 destroy.. 하려고 했는데 이젠 hitch 제어가 더 중요해서 완전히 내리지는 않는다.
	checkSlow(ExpiredUIP && ExpiredUIP == CreatedTagAnimUIP);
	DestroyTagAnimUIP(false);
}

void UB2UIPVP1on1BattleMain::StartInGameFight()
{
	if (UIP_AutoBattleIcon.IsValid())
	{
		UIP_AutoBattleIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (P_SkillSet.IsValid())
		P_SkillSet->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (BTN_Pause.IsValid())
		BTN_Pause->SetVisibility(ESlateVisibility::Visible);
}

void UB2UIPVP1on1BattleMain::UpdateAttackButtonStyleToPCClass(EPCClass InPCClass)
{
	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));

	UB2PCClassInfoBox* ClassInfoBox = B2GI ? B2GI->GetPCClassInfoBox() : NULL;
	UB2PCClassInfo* PCInfo = ClassInfoBox ? ClassInfoBox->GetSingleClassInfo(InPCClass) : NULL;
	UB2InGameHUDInfo_Player* HUDInfo = PCInfo ? Cast<UB2InGameHUDInfo_Player>(PCInfo->HUDInfoClassOverride->GetDefaultObject()) : NULL;

	if (BTN_Attack.IsValid() && HUDInfo && HUDInfo->bOverrideAttackButtonStyle)
	{
		FButtonStyle BtnStyle = HUDInfo->AttackButtonStyle;
		HUDInfo->ApplyAttackBtnMargin(BtnStyle, GetWorld()); // 게임모드에 따라 다른 세팅을 할 수 있다.
		BTN_Attack->SetStyle(BtnStyle);
	}
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetPortraitMainHero(UObject* WorldContextObject, UB2UIDocBase* InDoc)
{
	UB2UIDocHero* DocHero = nullptr;

	auto* DocForLocal = Cast<UB2UIDocBattle>(InDoc);
	if (DocForLocal)
		DocHero = UB2UIDocHelper::GetDocHero(DocForLocal->GetCurPCClass());
	else
	{
		auto* DocForRival = Cast<UB2UIDocPVP1on1Rival>(InDoc);
		if (DocForRival)
			DocHero = UB2UIDocHelper::GetDocHero(DocForRival->GetCurPCClass());
	}

	if (DocHero)
		SetPortraitMainHero(UB2UIDocHero::GetPlayerPortraitMaterial(WorldContextObject, DocHero->GetHeroClass()));
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetPortraitMainHero(UMaterialInterface* InPortraitMtrl)
{
	if (IMG_PortraitMain.IsValid() && InPortraitMtrl)
		IMG_PortraitMain->SetBrushFromMaterial(InPortraitMtrl);
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetPortraitSubHero(UObject* WorldContextObject, UB2UIDocBase* InDoc)
{
	UB2UIDocHero* DocHero = nullptr;

	auto* DocForLocal = Cast<UB2UIDocBattle>(InDoc);
	if (DocForLocal)
		DocHero = UB2UIDocHelper::GetDocHero(DocForLocal->GetTagPCClass());
	else
	{
		auto* DocForRival = Cast<UB2UIDocPVP1on1Rival>(InDoc);
		if (DocForRival)
			DocHero = UB2UIDocHelper::GetDocHero(DocForRival->GetTagPCClass());
	}

	if (DocHero)
		SetPortraitSubHero(UB2UIDocHero::GetPlayerPortraitMaterial(WorldContextObject, DocHero->GetHeroClass()));
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetPortraitSubHero(UMaterialInterface* InPortraitMtrl)
{
	if (IMG_PortraitSub.IsValid() && InPortraitMtrl)
		IMG_PortraitSub->SetBrushFromMaterial(InPortraitMtrl);

	if (IMG_PortraitSubDisable.IsValid() && InPortraitMtrl)
		IMG_PortraitSubDisable->SetBrushFromMaterial(InPortraitMtrl);
}	

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetHPMainChar(UB2UIDocBase* InDoc)
{
	auto* DocForLocal = Cast<UB2UIDocBattle>(InDoc);
	if (DocForLocal)
	{
		SetHPMainChar(DocForLocal->GetCurrPCHealth(), DocForLocal->GetCurrPCMaxHealth(), DocForLocal->GetCurrPCShield());
	}
	else
	{
		auto* DocForRival = Cast<UB2UIDocPVP1on1Rival>(InDoc);
		if (DocForRival)
			SetHPMainChar(DocForRival->GetCurrPCHealth(), DocForRival->GetCurrPCMaxHealth(), DocForRival->GetCurrPCShield());
	}
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetHPMainChar(float CurHP, float MaxHP, float CurShield)
{
	SetTextHPMainChar(false, CurHP, MaxHP);
	SetHPShieldPBCommon(PB_MainCharHP.Get(), PB_MainCharShield.Get(), CurHP, MaxHP, CurShield);
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetTextHPMainChar(bool bShow, float CurHP, float MaxHP)
{
	if (TB_CurrentHP.IsValid())
	{
		TB_CurrentHP->SetVisibility(bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		TB_CurrentHP->SetText(FText::AsNumber(static_cast<int32>(CurHP)));
	}
	if (TB_MaxHP.IsValid())
	{
		TB_MaxHP->SetVisibility(bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		TB_MaxHP->SetText(FText::AsNumber(static_cast<int32>(MaxHP)));
	}
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetHPSubChar(UB2UIDocBase* InDoc)
{
	auto* DocForLocal = Cast<UB2UIDocBattle>(InDoc);
	if (DocForLocal)
		SetHPSubChar(DocForLocal->GetTagPCHealth(), DocForLocal->GetTagPCMaxHealth());
	else
	{
		auto* DocForRival = Cast<UB2UIDocPVP1on1Rival>(InDoc);
		if (DocForRival)
			SetHPSubChar(DocForRival->GetTagPCHealth(), DocForRival->GetTagPCMaxHealth());
	}
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetHPSubChar(float CurHP, float MaxHP)
{
	if (PB_SubCharHP.IsValid())
		PB_SubCharHP->SetPercent(CurHP / MaxHP);

	if (O_Death.IsValid())
	{
		O_Death->SetVisibility(CurHP <= 0.f? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if(CurHP <= 0.f)
		SetTagCooltimeVisible(0.f);
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetNameMainChar(UB2UIDocBase* InDoc)
{
	//[@AKI 170414] EClass를 가져오던 것을 Docs에 UserNickName을 만들어서 가져오게 변경
	FText nickName;
	//나
	if (InDoc->GetClass()->IsChildOf<UB2UIDocBattle>())
	{
		auto* DocForLocal = Cast<UB2UIDocBattle>(InDoc);
		if (DocForLocal) //위에서 확인 했으니 null은 당연히 아니겠지만 안정성을 위해
			nickName = FText::FromString(DocForLocal->GetUserNickName());
	}
	//상대편
	else if (InDoc->GetClass()->IsChildOf<UB2UIDocPVP1on1Rival>())
	{
		auto* DocForRival = Cast<UB2UIDocPVP1on1Rival>(InDoc);
		if (DocForRival) //위에서 확인 했으니 null은 당연히 아니겠지만 안정성을 위해
			nickName = FText::FromString(DocForRival->GetUserNickName());
	}
	//만약 유저 닉네임을 가져 오지 못할 경우
	if (nickName.IsEmpty())
	{
		UB2UIDocHero* DocHero = nullptr;

		auto* DocForLocal = Cast<UB2UIDocBattle>(InDoc);
		if (DocForLocal)
			DocHero = UB2UIDocHelper::GetDocHero(DocForLocal->GetCurPCClass());
		else
		{
			auto* DocForRival = Cast<UB2UIDocPVP1on1Rival>(InDoc);
			if (DocForRival)
				DocHero = UB2UIDocHelper::GetDocHero(DocForRival->GetCurPCClass());
		}

		if (DocHero) // 여길 ClassOrNickName 을 쓸지 확인 필요.
			nickName = BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(DocHero->GetHeroClass());
	}
	SetNameMainChar(nickName);
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetNameMainChar(const FText& InText)
{
	if (TB_MainCharName.IsValid())
		TB_MainCharName->SetText(InText);
}
void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetLevelMainChar(int32 InValue)
{
	if (TB_MainCharLevel.IsValid())
		TB_MainCharLevel->SetText(FText::AsNumber(InValue));
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetTagPossible(bool bSet)
{
	if(IsLocalPlayer)
		SetEnabledTagBtn(bSet);

	SetEnabledSubPortrait(bSet);
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetEnabledTagBtn(bool bEnabledTag)
{
	if (BTN_Tag.IsValid())
		BTN_Tag->SetIsEnabled(bEnabledTag);

	if (CP_TagBottomEnabledTrue.IsValid())
		CP_TagBottomEnabledTrue->SetVisibility(bEnabledTag ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (CP_TagBottomEnabledFalse.IsValid())
		CP_TagBottomEnabledFalse->SetVisibility(bEnabledTag ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);

	if (IMG_TagOblivionLockBG.IsValid() && bEnabledTag == true)
	{
		IMG_TagOblivionLockBG->SetOpacity(0.0f);
	}
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetEnabledSubPortrait(bool bEnabledTag)
{
	if (IMG_PortraitSub.IsValid())
		IMG_PortraitSub->SetVisibility(bEnabledTag ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_PortraitSubDisable.IsValid())
		IMG_PortraitSubDisable->SetVisibility(bEnabledTag ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetTagCooltime(float RemainingTagCooltime)
{
	if (TB_TagCooltimeNum.IsValid())
	{
		UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(CachedDoc.Get());
		const float MaxCoolTime = DocBattle ? DocBattle->GetFinalTagCooltime() : 0.0f;
		// 스킬쿨타임처럼 첫 소수점을 카운트하는 동안에는 소수점 첫째자리까지 표시하는데 여기는 초반에 애니메이션으로 인해 쿨타임 표시 자체가 안되어서 딱히 눈에는 안 띌 듯.. 그래도 태그 쿨타임 감소 옵션이 있으니 기능은 넣어둔다.
		TB_TagCooltimeNum->SetText(GetAutoFractionalFormattedText_FirstFracCountOnly(RemainingTagCooltime, MaxCoolTime, 1));
	}

	UpdateTagCooltimeMaterial(RemainingTagCooltime);
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::UpdateTagCooltimeMaterial(float RemainingTagCooltime)
{
	if (TagCooltimeDisplayMID && CachedDoc.IsValid())
	{
		//TagCooltimeDisplayMID->SetScalarParameterValue(CoolTimeProgressParamName, 1.0f - RemainingTagCooltime);

		auto DocBattle = Cast<UB2UIDocBattle>(CachedDoc.Get());
		if (DocBattle)
		{
			TagCooltimeDisplayMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, RemainingTagCooltime / DocBattle->GetFinalTagCooltime());
			TagCooltimeDisplayMIDBottom->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, RemainingTagCooltime / DocBattle->GetFinalTagCooltime());
		}
		else
		{
			auto DocBattleRival = Cast<UB2UIDocPVP1on1Rival>(CachedDoc.Get());
			if (DocBattleRival)
			{
				TagCooltimeDisplayMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, RemainingTagCooltime / DocBattleRival->GetFinalTagCooltime());
				TagCooltimeDisplayMIDBottom->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, RemainingTagCooltime / DocBattle->GetFinalTagCooltime());
			}
		}
	}
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetIsCountingTagCooltime(bool bIsCounting)
{
	bIsCountingTagCoolTime = bIsCounting;
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetTagCooltimeVisible(bool bShowTagCooltime)
{
	if (TB_TagCooltimeNum.IsValid())
		bShowTagCooltime ? TB_TagCooltimeNum->SetVisibility(ESlateVisibility::HitTestInvisible) : TB_TagCooltimeNum->SetVisibility(ESlateVisibility::Collapsed);

	if (IMG_TagCooltime.IsValid())
		bShowTagCooltime ? IMG_TagCooltime->SetVisibility(ESlateVisibility::HitTestInvisible) : IMG_TagCooltime->SetVisibility(ESlateVisibility::Collapsed);

	if (IMG_TagCooltimeBottom.IsValid())
		bShowTagCooltime ? IMG_TagCooltimeBottom->SetVisibility(ESlateVisibility::HitTestInvisible) : IMG_TagCooltimeBottom->SetVisibility(ESlateVisibility::Collapsed);
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetGradeStar(int32 InGrade, int32 InStar)
{
	if (UIP_PVPGrade.IsValid())
		UIP_PVPGrade->SetGradeRank(InGrade, InStar);
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetVisibleGradeStar(bool bVisible)
{
	if (UIP_PVPGrade.IsValid())
		UIP_PVPGrade->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void FB2UIPVP1on1BattleMainCachedAssetPerChar::SetMyBadgeImage(bool _isLocalPlayer)
{
	if (IMG_MyBadge.IsValid())
	{
		ESlateVisibility visible = _isLocalPlayer ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		IMG_MyBadge->SetVisibility(visible);
	}
}
