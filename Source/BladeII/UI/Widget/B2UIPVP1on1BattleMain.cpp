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
	//	//硅撇霓焊
	//GET_SLOT(UB2UIBattleCombo, UIP_BattleCombo);

	//	//厚池飘
	//GET_SLOT(UB2UIBattleVignette, UIP_BattleVignette);

	//	//滚瓢甸
	//GET_SLOT(UButton, BTN_Pause);
	//GET_SLOT(UButton, BTN_Attack);
	//GET_SLOT(UButton, BTN_Guard);

	//GET_SLOT(UPanelWidget, CP_GuardEnable);
	//GET_SLOT(UPanelWidget, CP_GuardDisable);
	//GET_SLOT(UTextBlock, TB_GuradCoolTime);

	//GET_SLOTS_PVP_CHARACTER(LeftCharAssets, Left);
	//GET_SLOTS_PVP_CHARACTER(RightCharAssets, Right);

	//GET_SLOT(UTextBlock, TB_LeftTime);
	//GET_SLOT(UTextBlock, TB_Fight);

	//GET_SLOT(UTextBlock, TB_TagAnim_Gladiator);
	//GET_SLOT(UTextBlock, TB_TagAnim_Assassin);
	//GET_SLOT(UTextBlock, TB_TagAnim_Wizard);
	//GET_SLOT(UTextBlock, TB_TagAnim_Fighter);

	//GET_SLOT(UTextBlock, TB_LeaveEnemy);

	//GET_SLOT(UImage, IMG_FocusTimer);
	//GET_SLOT(UImage, IMG_Victory);
	//GET_SLOT(UImage, IMG_Lose);
	//GET_SLOT(UImage, IMG_Draw);

	//GET_SLOT(UB2Button, BTN_Tag);
	//GET_SLOT(UB2UIPAutoBattleIcon, UIP_AutoBattleIcon)
	//GET_SLOT(UPanelWidget, P_SkillSet);
	//GET_SLOT(UWidgetAnimation, ANIM_TimeFocus);

	//if (UIP_BattleCombo.IsValid())
	//{
	//	UIP_BattleCombo->Init();
	//	UIP_BattleCombo->SetVisibility(ESlateVisibility::Collapsed);
	//}

	//if (UIP_BattleVignette.IsValid())
	//{
	//	UIP_BattleVignette->Init();
	//	UIP_BattleVignette->SetVisibility(ESlateVisibility::Collapsed);
	//}

	//if (IMG_FocusTimer.IsValid())
	//{
	//	IMG_FocusTimer->SetVisibility(ESlateVisibility::Collapsed);
	//}

	//GET_SLOT(UB2UIEtherSetSkillIcon, UIP_OffenseEtherIcon);
	//if (UIP_OffenseEtherIcon.IsValid())
	//	UIP_OffenseEtherIcon->Init();
	//GET_SLOT(UB2UIEtherSetSkillIcon, UIP_DefenseEtherIcon);
	//if (UIP_DefenseEtherIcon.IsValid())
	//	UIP_DefenseEtherIcon->Init();

	//GET_SLOT(UImage, IMG_EvadeSkillCooltime);

	//GET_SLOT(UB2UIBattleNotice, UIP_BattleNotice);
	//if (UIP_BattleNotice.IsValid())
	//{
	//	UIP_BattleNotice->Init();
	//}
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
	//抗寇贸府 呈公 腹捞 秦辑....抗寇贸府 菩胶...
	UB2UIDocHero* DocHero(nullptr);
	//PlayAnimationEvent_AnimResultAction(result); 醚 茄锅父 龋免 登绢具 茄促 ShowResultImage俊辑 龋免
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
	else //Draw矫俊绰 剧率促
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
	//[@AKI, 170821] [B2CLT-1888] 葛靛喊 铰菩 楷免 烹老 俺急
	PlayAnimationEvent_AnimResultAction(result);
	/*
	//趣矫 葛福聪 老窜 傈何 促 阐
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
	//促 救焊捞霸 茄 惑怕俊辑 焊咯具 且 巴父 茫酒辑 焊咯淋
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
	else //Draw矫俊绰 剧率促
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

	//胶懦s
	for (uint8 Cnt = 0; Cnt < MAX_SKILL_INPUT; Cnt++)
	{
		FString SkillAssetName = PrefixSkillAsset + FString::FromInt(Cnt);
		auto SkillAsset = GetSlot<UB2UIBattleSkill>(FName(*SkillAssetName));
		if (SkillAsset)
		{
			SkillAssets.Add(SkillAsset);
			SkillAsset->Init();
			SkillAsset->SetIndex(Cnt);	//SkillRT率俊辑绰 1海捞胶肺 累诀茄 淀
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

	SetLocalMyBadgeImage(true); //bIsLeftControlledLocalPlayer 看栏唱 困俊辑 谅快甫 舅酒辑 官层林骨肺 弊成 Local篮 true肺 窍搁 凳
	SetRivalMyBadgeImage(false); //!bIsLeftControlledLocalPlayer

	if (UIP_BattleCombo.IsValid())
	{
		UIP_BattleCombo->BindDoc(InDocBattleForLocal);
	}

	if (UIP_BattleVignette.IsValid())
		UIP_BattleVignette->BindDoc(InDocBattleForLocal);

	//矫累 且锭, Main苞 Sub狼 努贰胶甫 技泼窍绊 CurPCClass唱 TagPCClass狼 蔼捞 官拆锭 滴俺甫 胶恳秦霖促.
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
	// 肺厚俊辑狼 皋牢 某腐磐 急琶捞 酒聪扼 泅犁 殿厘茄 某腐磐. 霸烙 矫累 贸澜俊绰 角力 皋牢 某腐磐烙.

	MainHeroDoc = InDocHero;
	if (!InDocHero) return;

	//胶懦 贸府. 辟单 泅犁 4俺狼 胶懦父 贸府登绊 乐绊 "傍拜"苞 "规绢"滚瓢篮 胶懦肺 秒鞭窍绊 乐瘤 臼绰单 弊贰登 登妨唱 -..-
	for (int SkillCnt = 0; SkillCnt < SkillAssets.Num(); ++SkillCnt)
	{
		UB2UIBattleSkill* SkillAsset = SkillAssets[SkillCnt];
		int32 SkillInputIndex = SkillAsset->GetIndex();

		auto SkillDoc = InDocHero->GetSkillDocByInputIndex(SkillInputIndex);
		BindDocSkill(SkillInputIndex, SkillDoc);
	}

	//檬惑拳 贸府
	check(LocalCharAssets);

	EPCClass pcClass = InDocHero->GetHeroClass();

	LocalCharAssets->SetPortraitMainHero(UB2UIDocHero::GetPlayerPortraitMaterial(this, pcClass));

	auto DocBattle = Cast<UB2UIDocBattle>(GetDoc());
	if (DocBattle)
		LocalCharAssets->SetNameMainChar(DocBattle);
	else
		LocalCharAssets->SetNameMainChar(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(pcClass));

	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
	//LocalCharAssets->SetLevelMainChar(InDocHero->GetCurrentLevel());
	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
	const int32 CharLevel = CharacterDataStore.GetCharacterLevel(pcClass);
	LocalCharAssets->SetLevelMainChar(CharLevel);
}

void UB2UIPVP1on1BattleMain::BindDocSubChar(UB2UIDocHero* InDocHero)
{
	// 肺厚俊辑狼 辑宏 某腐磐 急琶捞 酒聪扼 泅犁 措扁吝牢 某腐磐. 霸烙 矫累 贸澜俊绰 角力 辑宏 某腐磐烙.
	SubHeroDoc = InDocHero;

	if (!InDocHero)
		return;

	//檬惑拳 贸府.
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
		TArray<FUISkillBuffDisplayInfo_United> PrevSkillBuffData = AllCollectedSkillBuffData; // OnlyTimeChanged 八荤甫 困秦.
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

		// 付瘤阜栏肺 积己 矫痢俊 蝶扼 沥纺. 刚历 积己等 巴捞 菊栏肺 坷档废.
		AllCollectedSkillBuffData.Sort([](const FUISkillBuffDisplayInfo_United& A, const FUISkillBuffDisplayInfo_United& B)->bool
		{
			return A.LastStartedTime < B.LastStartedTime;
		});

		// 墨款飘 矫埃父 官差菌绰瘤, Buff 亲格 磊眉啊 官诧 霸 乐绰瘤 八荤.
		bOutOnlyCountTimeChanged = true;
		if (PrevSkillBuffData.Num() != AllCollectedSkillBuffData.Num())
		{ // 亲格 荐啊 促福搁 拱沸 歹 杭 鞘夸 绝绊.
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

				if (!bFoundSameBuff) // 窍唱扼档 鞍篮 吧 给 茫篮 霸 乐促搁 墨款飘 鸥烙 寇俊 官诧 霸 乐绰 芭
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
	{ // 捞 版快绰 老窜 扁粮 巴甸阑 力芭. 捞吧 芭摹瘤 臼疽促搁 扁粮俊 乐绰 巴甸阑 茫酒辑 蔼父 官操霸 瞪 巴.
		DestroyAllMyBuffIcons_Skill();
	}

	for (const FUISkillBuffDisplayInfo_United& ThisBuff : AllCollectedSkillBuffData) // LastStartedTime 俊 蝶扼 沥纺登绢 乐绢具 窃.
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
	/*if (UIP_DefenseEtherIcon.IsValid())
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
	}*/
}
void UB2UIPVP1on1BattleMain::OnChangedIsOffenseEther(class UB2UIDocBase* Sender, bool IsOffenseEther, bool PrevIsOffenseEther)
{
	//if (UIP_OffenseEtherIcon.IsValid())
	//{
	//	if (IsOffenseEther)
	//	{ 
	//		if(UIP_OffenseEtherIcon->GetVisibility() == ESlateVisibility::Collapsed)
	//			UIP_OffenseEtherIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//		UIP_OffenseEtherIcon->Init();
	//	}
	//	else if (!IsOffenseEther)
	//	{
	//		if(UIP_OffenseEtherIcon->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	//			UIP_OffenseEtherIcon->SetVisibility(ESlateVisibility::Collapsed);
	//	}
	//}
}

void UB2UIPVP1on1BattleMain::OnAbnormalStateChanged(class UB2UIDocBase* Sender, bool NewValue, bool PrevValue)
{
	const bool IsAbnormalState = NewValue;

	for (uint8 Cnt = 0; Cnt < MAX_SKILL_INPUT; Cnt++)
	{
		// Freezing/Stun 惑怕俊辑 哈绢唱绰 格利牢 SurviveSkill篮 Disable 矫虐瘤 臼澜
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
	//if (UIP_OffenseEtherIcon.IsValid())
	//{
	//	if (UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc()))
	//	{
	//		UIP_OffenseEtherIcon->UpdateCoolTime(DocBattle->GetOffenseEtherMaxCoolTime(), InRemainingCooltime, DocBattle->GetEnableOffenseEther());
	//	}
	//}
}

void UB2UIPVP1on1BattleMain::OnChangedDefenseEtherCoolTime(class UB2UIDocBase* Sender, float InRemainingCooltime, float InPrevRemainingCooltime)
{
	//if (UIP_DefenseEtherIcon.IsValid())
	//{
	//	if (UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc()))
	//	{
	//		UIP_DefenseEtherIcon->UpdateCoolTime(DocBattle->GetDefenseEtherMaxCoolTime(), InRemainingCooltime, DocBattle->GetEnableDefenseEther());
	//	}
	//}
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
		//久绢具 登绰 纳捞胶
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
		Tag_BP(MainHeroClass, SubHeroClass, bIsTagAttack); // B2UIBattleTagAnim 俊辑 怕弊 局聪皋捞记 包访茄 葛电 吧 贸府窍瘤绰 臼绰促.
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
// TagDisabledByCooltime 矫痢何磐 酿鸥烙 咆胶飘甫 焊捞扁 矫累. 寸厘 墨款飘绰 窍瘤 臼歹扼档
DEFINITION_PVP_ENTRY_FUNC_PARAM(UB2UIPVP1on1BattleMain::OnChangedIsTagDisabledByCooltime, bool, SetTagCooltimeVisible)
DEFINITION_PVP_ENTRY_FUNC_PARAM(UB2UIPVP1on1BattleMain::OnChangedIsTagPossible, bool, SetTagPossible)
DEFINITION_PVP_ENTRY_FUNC_PARAM(UB2UIPVP1on1BattleMain::OnChangedRemainingTagCooltime, float, SetTagCooltime)

void UB2UIPVP1on1BattleMain::OnChangedIsTagSuggested_Left(class UB2UIDocBase* Sender, bool bTagSuggested, bool bPrevTagSuggested)
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	const bool bIsForTagAttack = DocBattle ? DocBattle->GetIsTagAttackSuggested() : false; // 捞霸 酒聪搁 敲饭捞绢 牧叼记 (HP) 俊 狼茄 巴. IsTagAttackSuggested 甫 咯扁辑 眉农且 荐 乐档废 IsTagSuggested 焊促 刚历 技泼秦具 窃.
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
			StopTagSuggestByPlayerConditionAnim_Left_BP(); // 咯扁俊 HP 肺 牢茄 怕弊鼻厘 惑怕烙阑 疙矫窍绰 炼扒捞 乐促搁 娟扁啊 促福变 茄单..
		}
	}
}

void UB2UIPVP1on1BattleMain::OnChangedIsTagSuggested_Right(class UB2UIDocBase* Sender, bool bTagSuggested, bool bPrevTagSuggested)
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	const bool bIsForTagAttack = DocBattle ? DocBattle->GetIsTagAttackSuggested() : false; // 捞霸 酒聪搁 敲饭捞绢 牧叼记 (HP) 俊 狼茄 巴. IsTagAttackSuggested 甫 咯扁辑 眉农且 荐 乐档废 IsTagSuggested 焊促 刚历 技泼秦具 窃.
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
			StopTagSuggestByPlayerConditionAnim_Right_BP(); // 咯扁俊 HP 肺 牢茄 怕弊鼻厘 惑怕烙阑 疙矫窍绰 炼扒捞 乐促搁 娟扁啊 促福变 茄单..
		}
	}
}

void UB2UIPVP1on1BattleMain::BeginTagAnim(EPCClass NewTaggedClass, bool bIsTagAttack)
{
	DestroyTagAnimUIP(false); // 沥惑利牢 惑炔捞搁 捞傈俊 敲饭捞窍带 霸 辆丰登绊 积己沁带 扒 妮归罐酒 捞固 destroy 登绢 乐绢具.. 弊矾唱 destroy 捞傈俊 怕弊甫 肚 沁阑 荐档.

	if (bClickedPause || bIsTagAttack) // Pause甫 喘范芭唱 楷拌 怕弊绰 捞芭 救 窃.
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
	//		//酿鸥烙 赣磐府倔 贸府
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
{// UB2UIBattleTagAnim 率栏肺何磐 坷绰 脚龋. 皋葛府 例距阑 困秦 促 登菌栏搁 destroy.. 窍妨绊 沁绰单 捞哩 hitch 力绢啊 歹 吝夸秦辑 肯傈洒 郴府瘤绰 臼绰促.
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
		HUDInfo->ApplyAttackBtnMargin(BtnStyle, GetWorld()); // 霸烙葛靛俊 蝶扼 促弗 技泼阑 且 荐 乐促.
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
	//[@AKI 170414] EClass甫 啊廉坷带 巴阑 Docs俊 UserNickName阑 父甸绢辑 啊廉坷霸 函版
	FText nickName;
	//唱
	if (InDoc->GetClass()->IsChildOf<UB2UIDocBattle>())
	{
		auto* DocForLocal = Cast<UB2UIDocBattle>(InDoc);
		if (DocForLocal) //困俊辑 犬牢 沁栏聪 null篮 寸楷洒 酒聪摆瘤父 救沥己阑 困秦
			nickName = FText::FromString(DocForLocal->GetUserNickName());
	}
	//惑措祈
	else if (InDoc->GetClass()->IsChildOf<UB2UIDocPVP1on1Rival>())
	{
		auto* DocForRival = Cast<UB2UIDocPVP1on1Rival>(InDoc);
		if (DocForRival) //困俊辑 犬牢 沁栏聪 null篮 寸楷洒 酒聪摆瘤父 救沥己阑 困秦
			nickName = FText::FromString(DocForRival->GetUserNickName());
	}
	//父距 蜡历 葱匙烙阑 啊廉 坷瘤 给且 版快
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

		if (DocHero) // 咯辨 ClassOrNickName 阑 镜瘤 犬牢 鞘夸.
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
		// 胶懦酿鸥烙贸烦 霉 家荐痢阑 墨款飘窍绰 悼救俊绰 家荐痢 霉掳磊府鳖瘤 钎矫窍绰单 咯扁绰 檬馆俊 局聪皋捞记栏肺 牢秦 酿鸥烙 钎矫 磊眉啊 救登绢辑 迭洒 传俊绰 救 堕 淀.. 弊贰档 怕弊 酿鸥烙 皑家 可记捞 乐栏聪 扁瓷篮 持绢敌促.
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
