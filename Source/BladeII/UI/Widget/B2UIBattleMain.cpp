#include "B2UIBattleMain.h"
#include "B2UIBattleSkill.h"
#include "B2UIBattleCombo.h"
#include "B2UIBattleVignette.h"
#include "B2UIBattleQTE.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"
#include "B2UIDocUser.h"
#include "B2UIDocBattle.h"
#include "Styling/SlateBrush.h"
#include "B2SkillRTManager.h"
#include "CommonStruct.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameMode.h"
#include "B2PCClassInfoBox.h"
#include "B2InGameHUDInfo_Player.h"
#include "Event.h"
#include "B2StageManager.h"
#include "B2UIBattleTagAnim.h"
#include "BladeIIPlayerController.h"
#include "B2UIBattleNotice.h"
#include "B2UIBossStatus.h"
#include "B2AssetLoader.h"
#include "B2UIEtherSetSkillIcon.h"
#include "B2AssetLoader.h"
#include "B2UIRepeatBattleCountDown.h"
#include "BladeIIGameImpl.h"
#include "B2GameInstance.h"
#include "B2UIHotTimeIcon.h"
#include "B2CharacterTagDialogueInfo.h"
#include "B2UIPAutoBattleIcon.h"
#include "B2UIChatting.h"
#include "B2CombatStatEvaluator.h"
#include "B2UIUtil.h"

UB2UIBattleMain::UB2UIBattleMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bPendingPause = false;
	StageClearGradePreDropAnimTime = 10.0f;
	bLastPreExpectedGradeDifferent = false;
	StagePlayTimeTextColor_Normal = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	StagePlayTimeTextColor_PreDrop = FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));

	BuffIconClass_Stage = NULL;
	BuffIconClass_Resurrect = NULL;
	BuffIconClass_Skill = NULL;
	BuffIconClass_Guild = NULL;
	BuffIconSize = FVector2D(40.f, 40.f);

	LoadedTagAnimUIPClass = NULL;
	CreatedTagAnimUIP = NULL;
	LoadedQTEUIPClass = NULL;
	CreatedQTEUIP = NULL;	

	bIsCountingTagCooltime = false;
}

#define ADD_WIDGET_SLOT(Array, Type, Var)	\
	GET_SLOT(Type, Var);					\
	if(Var.IsValid())						\
		Array.Add(Var);


void UB2UIBattleMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ProcessEvadeCoolTimeUI();
}

void UB2UIBattleMain::CacheAssets()
{
	//////////////////////////////////////////////////////////////////
	// 
	// 馆拜 带傈 / 老馆 傈捧 UI 傍侩 Widget
	// 

	//滚瓢甸
	ADD_WIDGET_SLOT(CommonWidgets, UButton, BTN_Tag);
	ADD_WIDGET_SLOT(CommonWidgets, UButton, BTN_Pause);
	ADD_WIDGET_SLOT(CommonWidgets, UButton, BTN_Pause2);
	ADD_WIDGET_SLOT(CommonWidgets, UButton, BTN_Attack);	
		
	//皋牢某腐
	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_PortraitMain);
	ADD_WIDGET_SLOT(CommonWidgets, UImage, CRTInfoBGImage_01);
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_MainCharLevel);
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_MainCharName);
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_MainCharLevel);
	ADD_WIDGET_SLOT(CommonWidgets, UProgressBar, PB_MainCharHP);
	ADD_WIDGET_SLOT(CommonWidgets, UProgressBar, PB_MainCharShield);

	//辑宏某腐
	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_PortraitSub);
	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_PortraitSubDisable);
	ADD_WIDGET_SLOT(CommonWidgets, UProgressBar, PB_SubCharHP);

	//酿鸥烙
	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_TagCooltime);
	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_TagCooltimeBottom);
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_TagCooltimeNumBottom);
	if (IMG_TagCooltime.IsValid())
		TagCooltimeDisplayMID = IMG_TagCooltime->GetDynamicMaterial();

	if (IMG_TagCooltimeBottom.IsValid())
		TagCooltimeDisplayMIDBottom = IMG_TagCooltimeBottom->GetDynamicMaterial();

	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_FX_TagSuggest);
	if (IMG_FX_TagSuggest.IsValid())
	{ // 见败初绊 矫累.
		IMG_FX_TagSuggest->SetVisibility(ESlateVisibility::Hidden);
	}
	ADD_WIDGET_SLOT(CommonWidgets, UPanelWidget, CP_TagSuggest_TagCharChallenge);
	if (CP_TagSuggest_TagCharChallenge.IsValid())
	{
		CP_TagSuggest_TagCharChallenge->SetVisibility(ESlateVisibility::Hidden);
	}
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_TagSuggest_TagCharChallenge)
		ADD_WIDGET_SLOT(CommonWidgets, UPanelWidget, CP_TagAttackSuggestSet)
	if (CP_TagAttackSuggestSet.IsValid())
	{
		CP_TagAttackSuggestSet->SetVisibility(ESlateVisibility::Hidden);
	}
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_TagAttack);

	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_TagAnim_Gladiator);
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_TagAnim_Assassin);
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_TagAnim_Wizard);
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_TagAnim_Fighter);

	GET_SLOT(UB2UIEtherSetSkillIcon, UIP_OffenseEtherIcon);
	if (UIP_OffenseEtherIcon.IsValid())
		UIP_OffenseEtherIcon->Init();
	GET_SLOT(UB2UIEtherSetSkillIcon, UIP_DefenseEtherIcon);
	if (UIP_DefenseEtherIcon.IsValid())
		UIP_DefenseEtherIcon->Init();

	ADD_WIDGET_SLOT(NormalBattleWidgets, UB2UIPAutoBattleIcon, UIP_AutoBattleIcon);

	//硅撇霓焊
	ADD_WIDGET_SLOT(CommonWidgets, UB2UIBattleCombo, UIP_BattleCombo);

	//厚池飘
	ADD_WIDGET_SLOT(CommonWidgets, UB2UIBattleVignette, UIP_BattleVignette);
	
	//侵鸥烙 酒捞能
	ADD_WIDGET_SLOT(CommonWidgets, UB2UIHotTimeIcon, UIP_Hottime);
	
	ADD_WIDGET_SLOT(CommonWidgets, UPanelWidget, CP_Attack);
	ADD_WIDGET_SLOT(CommonWidgets, UPanelWidget, CP_PortraitSub);
	ADD_WIDGET_SLOT(CommonWidgets, UPanelWidget, CP_MainCharacterStatus);
	ADD_WIDGET_SLOT(CommonWidgets, UScaleBox, SB_TbMainCharName);

	ADD_WIDGET_SLOT(CommonWidgets, UButton, BTN_Guard);
	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_GuardBG);
	ADD_WIDGET_SLOT(CommonWidgets, UPanelWidget, CP_Guard);

	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_GuradCoolTime);
	ADD_WIDGET_SLOT(CommonWidgets, UPanelWidget, CP_GuardEnable);
	ADD_WIDGET_SLOT(CommonWidgets, UPanelWidget, CP_GuardDisable);

	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_QTEBG);
	//////////////////////////////////////////////////////////////////
	// 
	// 老馆 傈捧俊父 静捞绰 UI Widget
	// 
	ADD_WIDGET_SLOT(NormalBattleWidgets, UButton, BTN_MountAttack);
	ADD_WIDGET_SLOT(NormalBattleWidgets, UPanelWidget, CP_MountAttack);

	ADD_WIDGET_SLOT(NormalBattleWidgets, UTextBlock, TB_Gold);

	ADD_WIDGET_SLOT(NormalBattleWidgets, UPanelWidget, CP_Gold);
	ADD_WIDGET_SLOT(NormalBattleWidgets, UPanelWidget, CP_PlayTimeSet);
	ADD_WIDGET_SLOT(NormalBattleWidgets, UPanelWidget, P_StageNumberSet);
	ADD_WIDGET_SLOT(NormalBattleWidgets, UPanelWidget, P_Tag);

	ADD_WIDGET_SLOT(NormalBattleWidgets, UTextBlock, TB_StagePlayingTime);	
	ADD_WIDGET_SLOT(NormalBattleWidgets, UImage, IMG_StageClearGradeStarTwinkle_01);
	ADD_WIDGET_SLOT(NormalBattleWidgets, UImage, IMG_StageClearGradeStarTwinkle_02);
	ADD_WIDGET_SLOT(NormalBattleWidgets, UImage, IMG_StageClearGradeStarTwinkle_03);	

	ADD_WIDGET_SLOT(NormalBattleWidgets, UB2Button, BTN_ShowClearGradeCondition);
	ADD_WIDGET_SLOT(NormalBattleWidgets, UTextBlock, TB_ClearGradeCondition);

	ADD_WIDGET_SLOT(NormalBattleWidgets, UB2UIRepeatBattleInGameInfo, UIP_RepeatBattleInGameInfoP);
	if (UIP_RepeatBattleInGameInfoP.IsValid())
	{
		UIP_RepeatBattleInGameInfoP->Init();
	}

	ADD_WIDGET_SLOT(NormalBattleWidgets, UHorizontalBox, HB_StageResurrectBuffIconSet);
	
	ADD_WIDGET_SLOT(NormalBattleWidgets, UHorizontalBox, HB_Resurrect);
	ADD_WIDGET_SLOT(NormalBattleWidgets, UHorizontalBox, HB_Guild);

	ADD_WIDGET_SLOT(NormalBattleWidgets, UImage, IMG_SkillSetBG);

	ADD_WIDGET_SLOT(NormalBattleWidgets, UTextBlock, TB_StageNumber);
	ADD_WIDGET_SLOT(NormalBattleWidgets, UTextBlock, TB_StageDifficulty);

	ADD_WIDGET_SLOT(NormalBattleWidgets, UImage, IMG_EvadeSkillCooltime);
	//////////////////////////////////////////////////////////////////
	// 
	// 馆拜 带傈俊父 静捞绰 UI Widget
	// 

	ADD_WIDGET_SLOT(CounterAttackWidgets, UB2UIBattleSkill, UIP_CounterAttack_Skill);
	ADD_WIDGET_SLOT(CounterAttackWidgets, UB2UIWidgetBase, UIP_CounterFinishAttack);

	if (UIP_CounterFinishAttack.IsValid())
	{
		UIP_CounterFinishAttack->Init();		
	}
	ADD_WIDGET_SLOT(CounterAttackWidgets, UImage, FX_IMG_Enable);	
	ADD_WIDGET_SLOT(CounterAttackWidgets, UImage, IMG_CounterSuggestRing);

	//////////////////////////////////////////////////////////////////
	// 
	// Mount 傍拜 锭 见败廉具窍绰 困连甸.
	// 

	WidgetsToHideInMountAttackState.Add(TPairInitializer<TWeakObjectPtr<UObject>, ESlateVisibility>(IMG_SkillSetBG,	ESlateVisibility::SelfHitTestInvisible));	
	WidgetsToHideInMountAttackState.Add(TPairInitializer<TWeakObjectPtr<UObject>, ESlateVisibility>(BTN_Guard,		ESlateVisibility::Visible));
	WidgetsToHideInMountAttackState.Add(TPairInitializer<TWeakObjectPtr<UObject>, ESlateVisibility>(IMG_GuardBG,	ESlateVisibility::SelfHitTestInvisible));
	WidgetsToHideInMountAttackState.Add(TPairInitializer<TWeakObjectPtr<UObject>, ESlateVisibility>(IMG_QTEBG,		ESlateVisibility::SelfHitTestInvisible));
	WidgetsToHideInMountAttackState.Add(TPairInitializer<TWeakObjectPtr<UObject>, ESlateVisibility>(IMG_EvadeSkillCooltime, ESlateVisibility::HitTestInvisible));
	
	GET_SLOT(UB2UIBattleNotice, UIP_BattleNotice);
	if (UIP_BattleNotice.IsValid())
	{
		UIP_BattleNotice->Init();
	}

	GET_SLOT(UPanelWidget, CP_TagBottomEnabledTrue);
	GET_SLOT(UPanelWidget, CP_TagBottomEnabledFalse);
	GET_SLOT(UOverlay, O_Death);

	GET_SLOT(UPanelWidget, CP_MainButton);

	GET_SLOT(UTextBlock, TB_CounterDungeonLv);

	GET_SLOT(UB2Button, BTN_Chat);
	
#if !UE_BUILD_SHIPPING // 俺惯 抛胶飘侩
	GET_SLOT(UPanelWidget, P_DevLODTest);
	GET_SLOT(UB2Button, BTN_Dev_FxLOD_PC);
	GET_SLOT(UB2Button, BTN_Dev_FxLOD_Mob);
	GET_SLOT(UB2Button, BTN_Dev_FxLOD_Boss);
	GET_SLOT(UTextBlock, TB_Dev_FxLOD_PC);
	GET_SLOT(UTextBlock, TB_Dev_FxLOD_Mob);
	GET_SLOT(UTextBlock, TB_Dev_FxLOD_Boss);
#endif
}

void UB2UIBattleMain::Init()
{
	CurrentGameModeType = GetB2GameModeType(this);

	Super::Init();

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

	SetCloseBattleMainUIEventID = SetCloseBattleMainUIClass<bool>::GetInstance().Subscribe([this](bool Value)
	{
		this->CloseBattleMainUIEvent(Value);
	}
	);

	StopVisibleChatPopupTicket = StopVisibleChatPopupClass<>::GetInstance().Subscribe([this]()
	{
		this->DoMarkRedDot();
	});

	ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (UIP_AutoBattleIcon.IsValid() && pGM)
	{
		UIP_AutoBattleIcon->Init();
		UIP_AutoBattleIcon->SetIconAutoBattleType(pGM->GetPlayerAutoBattleType());

		if (CurrentGameModeType == EB2GameMode::Scenario)
		{
			UIP_AutoBattleIcon->SetIsStageMode(true);
		}
	}

	SetCounterDungeonLv();

	bIsTagDeath = false;

	SetGuardUI(false);

#if !UE_BUILD_SHIPPING
	UpdateDevFxLODWidgets();
#endif
}

void UB2UIBattleMain::CloseBattleMainUIEvent(bool Value)
{
	if (
		BTN_Pause.IsValid() && BTN_Pause->GetVisibility() == ESlateVisibility::Visible && 
		BTN_Pause2.IsValid() && BTN_Pause2->GetVisibility() == ESlateVisibility::Visible
		)
	{
		SetClose(Value);
	}
	else
	{
		SetClose(false);
	}
}


void UB2UIBattleMain::SetWidgetForCounterAttackBattle()
{
	for (auto WidgetWeakObjPtr : NormalBattleWidgets)
	{
		if (WidgetWeakObjPtr.IsValid())
		{
			Cast<UWidget>(WidgetWeakObjPtr.Get())->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (UIP_CounterFinishAttack.IsValid())
	{
		UIP_CounterFinishAttack->SetVisibility(ESlateVisibility::Collapsed);
	}

	SetAttackEnable(false);
}

void UB2UIBattleMain::SetWidgetForNormalBattle()
{
	for (auto WidgetWeakObjPtr : CounterAttackWidgets)
	{
		if (WidgetWeakObjPtr.IsValid())
		{
			Cast<UWidget>(WidgetWeakObjPtr.Get())->SetVisibility(ESlateVisibility::Collapsed);
		}			
	}

	SetForQTEMountUI(false);
}

void UB2UIBattleMain::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	DestroyTagAnimUIP(true);
	DestroyQTEUIP(true);
	DestroyAllMyBuffIcons_Resurrection();
	DestroyAllMyBuffIcons_Skill();
	DestroyAllMyBuffIcons_Guild();

	if (UIP_BattleNotice.IsValid())
		UIP_BattleNotice->DestroySelf(InUIManager);

	if (UIP_RepeatBattleInGameInfoP.IsValid())
	{
		UIP_RepeatBattleInGameInfoP->DestroySelf(InUIManager);
	}

	if (UIP_AutoBattleIcon.IsValid())
	{
		UIP_AutoBattleIcon->DestroySelf(InUIManager);
	}

	SetCloseBattleMainUIClass<bool>::GetInstance().Unsubscribe(SetCloseBattleMainUIEventID);
	StopVisibleChatPopupClass<>::GetInstance().Unsubscribe(StopVisibleChatPopupTicket);
}

void UB2UIBattleMain::UpdateStaticText()
{
	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	
	if (TB_TagAttack.IsValid())
	{
		TB_TagAttack->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("InGameHUD_TagAttack")));
	}
	if (TB_TagAnim_Gladiator.IsValid())
	{
		TB_TagAnim_Gladiator->SetText(ClientDataStore.GetLocalCharClassName(EPCClass::EPC_Gladiator));
	}
	if (TB_TagAnim_Assassin.IsValid())
	{
		TB_TagAnim_Assassin->SetText(ClientDataStore.GetLocalCharClassName(EPCClass::EPC_Assassin));
	}
	if (TB_TagAnim_Wizard.IsValid())
	{
		TB_TagAnim_Wizard->SetText(ClientDataStore.GetLocalCharClassName(EPCClass::EPC_Wizard));
	}
	if (TB_TagAnim_Fighter.IsValid())
	{
		TB_TagAnim_Fighter->SetText(ClientDataStore.GetLocalCharClassName(EPCClass::EPC_Fighter));
	}
	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
	if (TB_TagSuggest_TagCharChallenge.IsValid())
	{
		TB_TagSuggest_TagCharChallenge->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("InGameHUD_TagCharChallengingWord")));
	}

	if (TB_ClearGradeCondition.IsValid())
	{
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		const FServerStageID CurrentServerStageId = DocBS ? DocBS->GetServerStageId() : FServerStageID(1);
		// 喊 3俺 罐扁 困茄 炼扒 咆胶飘
		TB_ClearGradeCondition->SetText(GetStageClearGradeConditonText(CurrentServerStageId, STAGE_CLEAR_GRADE_STAR_3));
	}
}

void UB2UIBattleMain::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Tag,			&UB2UIBattleMain::OnClickTagBtn)

	//if(BTN_Tag.IsValid())
	//	BTN_Tag->OnEnabledStateChanged.AddDynamic(this, &UB2UIBattleMain::OnTagBTNEnableFunction);

	BIND_PRESS_FUNC_TO_BTN(BTN_Pause,		&UB2UIBattleMain::OnPressedPauseBtn)
	BIND_PRESS_FUNC_TO_BTN(BTN_Pause2,		&UB2UIBattleMain::OnPressedPauseBtn)

	BIND_PRESS_FUNC_TO_BTN(BTN_Attack,		&UB2UIBattleMain::OnPressedAttackBtn)
	BIND_RELEASE_FUNC_TO_BTN(BTN_Attack,	&UB2UIBattleMain::OnReleasedAttackBtn)
	BIND_PRESS_FUNC_TO_BTN(BTN_MountAttack, &UB2UIBattleMain::OnPressedAttackBtn)
	BIND_RELEASE_FUNC_TO_BTN(BTN_MountAttack, &UB2UIBattleMain::OnReleasedAttackBtn)

	BIND_PRESS_FUNC_TO_BTN(BTN_Guard,		&UB2UIBattleMain::OnPressedGuardBtn)
	BIND_RELEASE_FUNC_TO_BTN(BTN_Guard,		&UB2UIBattleMain::OnReleasedGuardBtn)

	BIND_PRESS_FUNC_TO_BTN(BTN_ShowClearGradeCondition, &UB2UIBattleMain::OnPressedShowClearGradeConditionBtn)
	BIND_RELEASE_FUNC_TO_BTN(BTN_ShowClearGradeCondition, &UB2UIBattleMain::OnReleasedShowClearGradeConditionBtn)
	if (BTN_ShowClearGradeCondition.IsValid())
	{ // 娟甸篮 滚瓢 格利惑 Hovered 档 秦 焊磊..
		BTN_ShowClearGradeCondition->OnHovered.RemoveDynamic(this, &UB2UIBattleMain::OnPressedShowClearGradeConditionBtn);
		BTN_ShowClearGradeCondition->OnHovered.AddDynamic(this, &UB2UIBattleMain::OnPressedShowClearGradeConditionBtn);
		BTN_ShowClearGradeCondition->OnUnhovered.RemoveDynamic(this, &UB2UIBattleMain::OnReleasedShowClearGradeConditionBtn);
		BTN_ShowClearGradeCondition->OnUnhovered.AddDynamic(this, &UB2UIBattleMain::OnReleasedShowClearGradeConditionBtn);
	}


	if (UIP_CounterFinishAttack.IsValid())
	{
		TWeakObjectPtr<UB2Button> FinishBTN = UIP_CounterFinishAttack->GetSlot<UB2Button>(FName(TEXT("BTN_Finish")));
		
		if (FinishBTN.IsValid())
		{
			BIND_CLICK_FUNC_TO_BTN(FinishBTN, &UB2UIBattleMain::OnClickCounterFinishBtn)
		}
	}

	BIND_CLICK_FUNC_TO_BTN(BTN_Chat, &UB2UIBattleMain::OnClickBtnChat);

#if !UE_BUILD_SHIPPING
	BIND_CLICK_FUNC_TO_BTN(BTN_Dev_FxLOD_PC, &UB2UIBattleMain::OnClickBtnDevFxLODPC);
	BIND_CLICK_FUNC_TO_BTN(BTN_Dev_FxLOD_Mob, &UB2UIBattleMain::OnClickBtnDevFxLODMob);
	BIND_CLICK_FUNC_TO_BTN(BTN_Dev_FxLOD_Boss, &UB2UIBattleMain::OnClickBtnDevFxLODBoss);
#endif
}

void UB2UIBattleMain::BindDocAuto()
{
//	UnbindDoc();
//
//	//胶懦
//	for (uint8 Cnt = 0; Cnt < MAX_SKILL_INPUT; Cnt++)
//	{
//		FString SkillAssetName = PrefixSkillAsset + FString::FromInt(Cnt);
//		auto SkillAsset = GetSlot<UB2UIBattleSkill>(FName(*SkillAssetName));
//		if (SkillAsset)
//		{
//			SkillAssets.Add(SkillAsset);
//			SkillAsset->Init();
//			SkillAsset->SetIndex(Cnt);	//SkillRT率俊辑绰 1海捞胶肺 累诀茄 淀
//			SkillAsset->OnBattleSkillClicked.BindDynamic(this, &UB2UIBattleMain::OnPressedSkillBtn);
//		}
//	}
//
//	if (UIP_CounterAttack_Skill.IsValid())
//	{
//		SkillAssets.Add(UIP_CounterAttack_Skill.Get());
//		UIP_CounterAttack_Skill->Init();
//		UIP_CounterAttack_Skill->SetIndex(0);
//		UIP_CounterAttack_Skill->OnBattleSkillClicked.BindDynamic(this, &UB2UIBattleMain::OnPressedSkillBtn);		
//	}
//
//	for (auto* Skill : SkillAssets)
//	{
////		UB2UIBattleSkill* SkillWidget = Cast<UB2UIBattleSkill>(Skill);
////		BII_CHECK(SkillWidget);
//
//		WidgetsToHideInMountAttackState.Add(TPairInitializer<TWeakObjectPtr<UObject>,ESlateVisibility>(Skill, Skill->GetVisibility()));
//	}
//
//	auto DocBattle = UB2UIDocHelper::GetDocBattle();
//	SetDoc(DocBattle);
//	if (!DocBattle)return;
//
//	SetGold(0);
//
//	if (UIP_BattleCombo.IsValid())
//		UIP_BattleCombo->BindDoc(DocBattle);
//
//	if (UIP_BattleVignette.IsValid())
//		UIP_BattleVignette->BindDoc(DocBattle);
//
//	if (DocBattle->GetbCurrentlyBossAppeared()) 
//		UB2UIManager::GetInstance()->OpenUI(UIFName::BossStatus);
//	else
//		UB2UIManager::GetInstance()->CloseUI(UIFName::BossStatus);
//
//	//矫累 且锭, Main苞 Sub狼 努贰胶甫 技泼窍绊 CurPCClass唱 TagPCClass狼 蔼捞 官拆锭 滴俺甫 胶恳秦霖促.
//	BindDocMainChar(UB2UIDocHelper::GetDocHero(DocBattle->GetCurPCClass()));
//	BindDocSubChar(UB2UIDocHelper::GetDocHero(DocBattle->GetTagPCClass()));
//
//	DocBattle->OnCurPCClassChanged.AddUObject(this, &UB2UIBattleMain::OnChangedCurPCClass);
//	DocBattle->OnCurrPCHealthChanged.AddUObject(this, &UB2UIBattleMain::OnChangedCurrPCHealth);
//	DocBattle->OnCurrPCMaxHealthChanged.AddUObject(this, &UB2UIBattleMain::OnChangedCurrPCMaxHealth);
//	DocBattle->OnCurrPCShieldChanged.AddUObject(this, &UB2UIBattleMain::OnChangedCurrPCShield);
//	DocBattle->OnTagPCHealthChanged.AddUObject(this, &UB2UIBattleMain::OnChangedTagPCHealth);
//	DocBattle->OnTagPCMaxHealthChanged.AddUObject(this, &UB2UIBattleMain::OnChangedTagPCMaxHealth);
//	DocBattle->OnIsCountingTagCooltimeChanged.AddUObject(this, &UB2UIBattleMain::OnChangedIsCountingTagCooltime);
//	DocBattle->OnIsTagPossibleChanged.AddUObject(this, &UB2UIBattleMain::OnChangedIsTagPossible);
//	DocBattle->OnIsTagDisabledByCooltimeChanged.AddUObject(this, &UB2UIBattleMain::OnChangedIsTagDisabledByCooltime);
//	DocBattle->OnRemainingTagCooltimeChanged.AddUObject(this, &UB2UIBattleMain::OnChangedRemainingTagCooltime);
//	DocBattle->OnbCurrentlyBossAppearedChanged.AddUObject(this, &UB2UIBattleMain::OnChangedbCurrentlyBossAppeared);
//	DocBattle->OnIsTagSuggestedChanged.AddUObject(this, &UB2UIBattleMain::OnChangedIsTagSuggested);	
//	DocBattle->OnCharacterBuffStateChanged.AddUObject(this, &UB2UIBattleMain::OnChangedCharacterSkillBuffState);
//	DocBattle->OnQTEEnableStateChanged.AddUObject(this, &UB2UIBattleMain::OnChangedQTEEnableState);
//	DocBattle->OnbIsQTEMountedChanged.AddUObject(this, &UB2UIBattleMain::OnChangedQTEMounted);
//	DocBattle->OnbIsQTEMountedAndReadyForAttackChanged.AddUObject(this, &UB2UIBattleMain::OnChangedQTEMountedAndReadyForAttack);
//	DocBattle->OnbIsPlayerInQTEStateChanged.AddUObject(this, &UB2UIBattleMain::OnChangedPlayerInQTEState);
//	DocBattle->OnOffenseEtherCoolTimeChanged.AddUObject(this, &UB2UIBattleMain::OnChangedOffenseEtherCoolTime);
//	DocBattle->OnDefenseEtherCoolTimeChanged.AddUObject(this, &UB2UIBattleMain::OnChangedDefenseEtherCoolTime);
//	DocBattle->OnIsOffenseEtherChanged.AddUObject(this, &UB2UIBattleMain::OnChangedIsOffenseEther);
//	DocBattle->OnIsDefenseEtherChanged.AddUObject(this, &UB2UIBattleMain::OnChangedIsDefenseEther);
//	DocBattle->OnHasAbnormalBuffChanged.AddUObject(this, &UB2UIBattleMain::OnAbnormalStateChanged);
//	DocBattle->OnGuardCoolTimeChanged.AddUObject(this, &UB2UIBattleMain::OnChangedGuradCoolTime);
//	DocBattle->OnHasNotTagDefenceChanged.AddUObject(this, &UB2UIBattleMain::OnChangedHasNotTagDefence);
//
//	UpdateAttackButtonStyleToPCClass(IntToPCClass(DocBattle->GetCurPCClass()));
//	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
//
//	DocBattle->SetIsOffenseEther(DocBattle->GetIsOffenseEther());
//	DocBattle->SetIsDefenseEther(DocBattle->GetIsDefenseEther());
//
//	if (DocBS)
//	{
//		DocBS->OnStagePlayTimeChanged.AddUObject(this, &UB2UIBattleMain::OnChangedStagePlayTime);
//
//		//// StageBuff 绰 胶抛捞瘤 柳青 档吝 官差瘤 臼澜. 贸澜俊父 积己窍搁 瞪 巴.
//		//const TArray<EStageBuffType> AllStageBuff = DocBS->GetSelectedStageBuffs();
//		//for (EStageBuffType ThisStageBuff : AllStageBuff)
//		//	CreateOrFindStageBuffIcon(ThisStageBuff);
//
//		int32 ClientStageId = DocBS->GetClientStageId();
//		UB2StageInfo* StageInfoTable = StaticFindStageInfo();
//		int32 ChapterNum = StageInfoTable ? StageInfoTable->GetChapterNumByClientStageId(ClientStageId) : 0;
//		int32 StageNum = StageInfoTable ? StageInfoTable->GetSubChapterNumByClientStageId(ClientStageId) : 0;
//
//		if (TB_StageNumber.IsValid())
//			TB_StageNumber->SetText(FText::FromString(FString::Printf(TEXT("%d - %d"), ChapterNum, StageNum)));
//
//		if (TB_StageDifficulty.IsValid() && StageInfoTable)
//		{
//			FText StageDifficultyText;
//						
//			EStageDifficulty StageDifficulty = AB2StageManager::GetCacheStageKeepEssentialData().GetStageDifficulty();
//			switch (StageDifficulty)
//			{
//			case EStageDifficulty::ESD_Normal:
//				StageDifficultyText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AdventureMod_NormalTitle"));
//				break;
//			case EStageDifficulty::ESD_Hard:
//				StageDifficultyText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AdventureMod_HardTitle"));
//				break;
//			case EStageDifficulty::ESD_VeryHard:
//				StageDifficultyText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AdventureMod_VeryHardTitle"));
//				break;
//			case EStageDifficulty::ESD_Hell:
//				StageDifficultyText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AdventureMod_HellTitle"));
//				break;
//			}	
//						
//			TB_StageDifficulty->SetText(FText::Format(FText::FromString(TEXT("({0})")), StageDifficultyText));
//		}
//
//		if (UIP_Hottime.IsValid())
//		{
//			bool IsActiveHotTime = DocBS->GetStartExpBuffRate() > 0 || DocBS->GetStartGoldBuffRate() > 0;
//			UIP_Hottime->SetVisibility(IsActiveHotTime ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
//			if(IsActiveHotTime)
//				UIP_Hottime->ActiveEffect_BP(IsActiveHotTime);
//		}
//			
//	}
//
//	// 何劝滚橇绰 积己父 秦 狄. 老窜 钎矫 救 瞪 巴.
//	CreateOrUpdateResurrectBuffIcon(EResurrectBuffType::RBT_None);
//
//	// 怕弊 酿鸥烙 檬扁拳
//	SetTagCooltime(0);
//
//	//辨靛 滚橇 殿废
//	UpdateGuildBuff();
//	
//	SetVisibilityTagWidget(true);
//	//SetVisibilityGoldWidget(true);
//	SetVisibilityPlayTimeWidget(true);
//	SetVisibilityCRTInfoBGImage_01(true);
//	CheckWidgetVisibilityForMode();
//
//	DocBattle->OnMaxCounterAttackPointChanged.AddUObject(this, &UB2UIBattleMain::OnChangedCounterAttackPoint);
//	DocBattle->OnCounterAttackPointChanged.AddUObject(this, &UB2UIBattleMain::OnChangedCounterAttackPoint);
//	
//	// 眠饶俊 眠啊瞪 版快 捞犯霸 盒扁窍瘤富绊, 惑加秦辑 父奠矫促夸~ 炼父埃 绝局具瘤.. (曼绊 RaidBattle.h)
//	if (CurrentGameModeType == EB2GameMode::CounterDungeon)
//		SetWidgetForCounterAttackBattle();
//	else
//		SetWidgetForNormalBattle();
//
//	DocBattle->SetMaxCounterAttackPoint(MAX_COUNTER_ATTACK_POINT);
}

void UB2UIBattleMain::BindDocMainChar(UB2UIDocHero* InDocHero)
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

		if (SkillDoc && CurrentGameModeType == EB2GameMode::CounterDungeon)
		{
			SkillDoc->OnIsCountingCooltimeChanged.RemoveAll(SkillAsset);
			SkillDoc->OnIsDisabledByCooltimeChanged.RemoveAll(SkillAsset);
			SkillDoc->OnRemainingCooltimeChanged.RemoveAll(SkillAsset);
		}
	}

	FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	EPCClass pcClass = InDocHero->GetHeroClass();

	//檬惑拳 贸府
	SetPortraitMainHero(UB2UIDocHero::GetPlayerPortraitMaterial(this, pcClass));

	SetNameMainChar(ClientDataStore.GetLocalCharClassOrNickName(pcClass));

	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
	//SetLevelMainChar(InDocHero->GetCurrentLevel());
	SetLevelMainChar(ClientDataStore.GetLocalCharacterData().GetCharacterLevel(pcClass));
}

void UB2UIBattleMain::BindDocSubChar(UB2UIDocHero* InDocHero)
{
	// 肺厚俊辑狼 辑宏 某腐磐 急琶捞 酒聪扼 泅犁 措扁吝牢 某腐磐. 霸烙 矫累 贸澜俊绰 角力 辑宏 某腐磐烙.
	SubHeroDoc = InDocHero;

	if (!InDocHero) 
		return;

	//檬惑拳 贸府.
	SetPortraitSubHero(UB2UIDocHero::GetPlayerPortraitMaterial(this, InDocHero->GetHeroClass()));
}

void UB2UIBattleMain::UnbindDoc()
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
		DocBattle->OnCurPCClassChanged.RemoveAll(this);
		DocBattle->OnCurrPCHealthChanged.RemoveAll(this);
		DocBattle->OnCurrPCMaxHealthChanged.RemoveAll(this);
		DocBattle->OnCurrPCShieldChanged.RemoveAll(this);
		DocBattle->OnTagPCHealthChanged.RemoveAll(this);
		DocBattle->OnTagPCMaxHealthChanged.RemoveAll(this);
		DocBattle->OnIsCountingTagCooltimeChanged.RemoveAll(this);
		DocBattle->OnIsTagPossibleChanged.RemoveAll(this);
		DocBattle->OnIsTagDisabledByCooltimeChanged.RemoveAll(this);
		DocBattle->OnRemainingTagCooltimeChanged.RemoveAll(this);
		DocBattle->OnbCurrentlyBossAppearedChanged.RemoveAll(this);
		DocBattle->OnIsTagSuggestedChanged.RemoveAll(this);
		DocBattle->OnCharacterBuffStateChanged.RemoveAll(this);
		DocBattle->OnQTEEnableStateChanged.RemoveAll(this);
		DocBattle->OnbIsQTEMountedChanged.RemoveAll(this);
		DocBattle->OnbIsQTEMountedAndReadyForAttackChanged.RemoveAll(this);
		DocBattle->OnbIsPlayerInQTEStateChanged.RemoveAll(this);

		DocBattle->OnMaxCounterAttackPointChanged.RemoveAll(this);
		DocBattle->OnCounterAttackPointChanged.RemoveAll(this);
		DocBattle->OnOffenseEtherCoolTimeChanged.RemoveAll(this);
		DocBattle->OnDefenseEtherCoolTimeChanged.RemoveAll(this);
		DocBattle->OnIsOffenseEtherChanged.RemoveAll(this);
		DocBattle->OnIsDefenseEtherChanged.RemoveAll(this);

		DocBattle->OnHasAbnormalBuffChanged.RemoveAll(this);
	}

	UB2UIDocBattleStage* DocBS = Cast<UB2UIDocBattleStage>(UB2UIDocHelper::GetDocBattleStage());
	if (DocBS)
	{
		DocBS->OnStagePlayTimeChanged.RemoveAll(this);
	}

	auto DocUser = UB2UIDocHelper::GetDocUser();
	if (DocUser)
	{
		DocUser->OnGoldChanged.RemoveAll(this);
	}
	
	MainHeroDoc = nullptr;
	SubHeroDoc = nullptr;

	SetDoc(nullptr);
}

void UB2UIBattleMain::BindDocSkill(int32 SkillInputIndex, UB2UIDocSkill* SkillDoc)
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

void UB2UIBattleMain::Tag()
{
	auto TempHeroDocForSwap = MainHeroDoc;
	BindDocMainChar(SubHeroDoc);
	BindDocSubChar(TempHeroDocForSwap);

	if (!MainHeroDoc || !SubHeroDoc) return;

	TagCommon(IntToPCClass(MainHeroDoc->GetDocId()), IntToPCClass(SubHeroDoc->GetDocId()));
}

void UB2UIBattleMain::Tag(int32 MainClass, int32 SubClass)
{
	BindDocMainChar(UB2UIDocHelper::GetDocHero(MainClass));
	BindDocSubChar(UB2UIDocHelper::GetDocHero(SubClass));

	TagCommon(IntToPCClass(MainClass), IntToPCClass(SubClass));
}

void UB2UIBattleMain::TagCommon(EPCClass MainClass, EPCClass SubClass)
{
	UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc());
	bool bIsTagAttack = (DocBattle ? DocBattle->GetbTagAttackPending() : false);

	BeginTagAnim(MainClass, bIsTagAttack);
	Tag_BP(MainClass, SubClass, bIsTagAttack); // B2UIBattleTagAnim 俊辑 怕弊 局聪皋捞记 包访茄 葛电 吧 贸府窍瘤绰 臼绰促.

	PlayTagDialogueSound(MainClass, SubClass);
}

void UB2UIBattleMain::SetCounterDungeonLv()
{
	UB2UIDocCounterAttack* CounterDoc = UB2UIDocHelper::GetDocCounterAttack();

	if (!CounterDoc)
		return;

	if (TB_CounterDungeonLv.IsValid())
	{
		TB_CounterDungeonLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("counterDungeon_Step")), FText::AsNumber(CounterDoc->GetNewDifficulty())));
	}
}

void UB2UIBattleMain::SetPortraitMainHero(UMaterialInterface* InPortraitMtrl)
{
	if (IMG_PortraitMain.IsValid() && InPortraitMtrl)
		IMG_PortraitMain->SetBrushFromMaterial(InPortraitMtrl);
}

void UB2UIBattleMain::SetPortraitSubHero(UMaterialInterface* InPortraitMtrl)
{
	if (IMG_PortraitSub.IsValid() && InPortraitMtrl)
		IMG_PortraitSub->SetBrushFromMaterial(InPortraitMtrl);

	if (IMG_PortraitSubDisable.IsValid() && InPortraitMtrl)
		IMG_PortraitSubDisable->SetBrushFromMaterial(InPortraitMtrl);
}

void UB2UIBattleMain::SetHPMainChar(float CurHP, float MaxHP, float CurShield)
{
	SetHPShieldPBCommon(PB_MainCharHP.Get(), PB_MainCharShield.Get(), CurHP, MaxHP, CurShield);
}

void UB2UIBattleMain::SetHPSubChar(float CurHP, float MaxHP)
{
	if (PB_SubCharHP.IsValid())
		PB_SubCharHP->SetPercent(CurHP / MaxHP);

	if (CurHP > 0)
		OnPlayResurrection_BP();
}

void UB2UIBattleMain::SetNameMainChar(const FText& InText)
{
	if (TB_MainCharName.IsValid())
		TB_MainCharName->SetText(InText);
}
void UB2UIBattleMain::SetLevelMainChar(int32 InValue)
{
	// 咯扁 啊瓷茄 Lv 咆胶飘档 促惫绢肺 窍绰 霸 亮促. 泅犁绰 喉风橇赴飘俊 冠囚 乐澜
	//FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(InValue))) :

	if (TB_MainCharLevel.IsValid())
		TB_MainCharLevel->SetText(FText::AsNumber(InValue));
}


void UB2UIBattleMain::SetOffenseEtherlCooltime(float InCoolTime)
{
	if (TagCooltimeDisplayMID && TagCooltimeDisplayMIDBottom)
	{
		if (auto DocBattle = Cast<UB2UIDocBattle>(GetDoc()))
		{
			TagCooltimeDisplayMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, InCoolTime / DocBattle->GetOffenseEtherMaxCoolTime());
			TagCooltimeDisplayMIDBottom->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, InCoolTime / DocBattle->GetOffenseEtherMaxCoolTime());
		}
	}
}

void UB2UIBattleMain::SetDefenseEtherCooltime(float InCoolTime)
{
	if (TagCooltimeDisplayMID && TagCooltimeDisplayMIDBottom)
	{
		if (auto DocBattle = Cast<UB2UIDocBattle>(GetDoc()))
		{
			TagCooltimeDisplayMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, InCoolTime / DocBattle->GetDefenseEtherMaxCoolTime());
			TagCooltimeDisplayMIDBottom->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, InCoolTime / DocBattle->GetOffenseEtherMaxCoolTime());
		}
	}
}

void UB2UIBattleMain::SetEnableOffenseEther(bool InEnable)
{
}

void UB2UIBattleMain::SetEnableDefenseEther(bool InEnable)
{

}

void UB2UIBattleMain::SetTagCooltime(float RemainingTagCooltime)
{
	if (TB_TagCooltimeNumBottom.IsValid())
	{
		UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc());
		const float MaxCoolTime = DocBattle ? DocBattle->GetFinalTagCooltime() : 0.0f;
		// 胶懦酿鸥烙贸烦 霉 家荐痢阑 墨款飘窍绰 悼救俊绰 家荐痢 霉掳磊府鳖瘤 钎矫窍绰单 咯扁绰 檬馆俊 局聪皋捞记栏肺 牢秦 酿鸥烙 钎矫 磊眉啊 救登绢辑 迭洒 传俊绰 救 堕 淀.. 弊贰档 怕弊 酿鸥烙 皑家 可记捞 乐栏聪 扁瓷篮 持绢敌促.
		FText CoolTimeText = GetAutoFractionalFormattedText_FirstFracCountOnly(RemainingTagCooltime, MaxCoolTime, 1);
		TB_TagCooltimeNumBottom->SetText(CoolTimeText);
	};

	UpdateTagCooltimeMaterial(RemainingTagCooltime);
}

void UB2UIBattleMain::UpdateTagCooltimeMaterial(float RemainingTagCooltime)
{
	if (TagCooltimeDisplayMID && TagCooltimeDisplayMIDBottom)
	{
		//TagCooltimeDisplayMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, 1.0f - RemainingTagCooltime);

		auto DocBattle = Cast<UB2UIDocBattle>(GetDoc());
		if (!DocBattle) return;
		TagCooltimeDisplayMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, RemainingTagCooltime / DocBattle->GetFinalTagCooltime());
		TagCooltimeDisplayMIDBottom->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, RemainingTagCooltime / DocBattle->GetFinalTagCooltime());
	}
}

void UB2UIBattleMain::UpdateAttackButtonStyleToPCClass(EPCClass InPCClass)
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

void UB2UIBattleMain::SetTagCooltimeVisible(bool bShowTagCooltime)
{
	if (TB_TagCooltimeNumBottom.IsValid())
		bShowTagCooltime ? TB_TagCooltimeNumBottom->SetVisibility(ESlateVisibility::HitTestInvisible) : TB_TagCooltimeNumBottom->SetVisibility(ESlateVisibility::Collapsed);

	if (IMG_TagCooltime.IsValid())
		bShowTagCooltime ? IMG_TagCooltime->SetVisibility(ESlateVisibility::HitTestInvisible) : IMG_TagCooltime->SetVisibility(ESlateVisibility::Collapsed);

	if (IMG_TagCooltimeBottom.IsValid())
		bShowTagCooltime ? IMG_TagCooltimeBottom->SetVisibility(ESlateVisibility::HitTestInvisible) : IMG_TagCooltimeBottom->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIBattleMain::SetEnabledTagBtn(bool bEnabledTag)
{
	ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	if (!pGM)
		return;

	bool bAllowTag = pGM->IsAllowTag();

	if (BTN_Tag.IsValid())
		BTN_Tag->SetIsEnabled(bAllowTag && bEnabledTag);
}

void UB2UIBattleMain::SetEnabledSubPortrait(bool bEnabled)
{
	if (IMG_PortraitSub.IsValid())
		IMG_PortraitSub->SetVisibility(bEnabled ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_PortraitSubDisable.IsValid())
		IMG_PortraitSubDisable->SetVisibility(bEnabled ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);

}

void UB2UIBattleMain::SetVisibilitySB_TbMainCharName(bool bVisibility)
{
	if (SB_TbMainCharName.IsValid())
		SB_TbMainCharName->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIBattleMain::SetVisibilityPlayerInfoWidget(bool bVisibility)
{
	if (CP_MainCharacterStatus.IsValid())
		CP_MainCharacterStatus->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIBattleMain::SetVisibilityTagWidget(bool bVisibility)
{
	if (CP_PortraitSub.IsValid())
		CP_PortraitSub->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if(P_Tag.IsValid())
		P_Tag->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIBattleMain::SetVisibilityGoldWidget(bool bVisibility)
{
	if (CP_Gold.IsValid())
		CP_Gold->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIBattleMain::SetVisibilityPlayTimeWidget(bool bVisibility)
{
	if (CP_PlayTimeSet.IsValid())
		CP_PlayTimeSet->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);		
}

void UB2UIBattleMain::SetVisibilityStageNumberWidget(bool bVisibility)
{
	if (P_StageNumberSet.IsValid())
		P_StageNumberSet->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIBattleMain::SetVisibilityCRTInfoBGImage_01(bool bVisibility)
{
	if (CRTInfoBGImage_01.IsValid())
		CRTInfoBGImage_01->SetVisibility(bVisibility ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UB2UIBattleMain::SetVisibilityPauseButton(bool bVisibility)
{
	if (BTN_Pause.IsValid())
		BTN_Pause->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (BTN_Pause2.IsValid())
		BTN_Pause2->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	SetClose(bVisibility);
}

void UB2UIBattleMain::SetVisibilityHotTime(bool bVisibility)
{
	if (UIP_Hottime.IsValid())
		UIP_Hottime->SetVisibility(bVisibility ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UIBattleMain::CheckWidgetVisibilityForMode()
{
	SetVisibilityHotTime(CurrentGameModeType == EB2GameMode::Scenario);

	if (CurrentGameModeType != EB2GameMode::Scenario)
	{
		if (BTN_Chat.IsValid())
			BTN_Chat->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (CurrentGameModeType == EB2GameMode::HeroTower)
	{
		SetVisibilityPlayTimeWidget(false);
		SetVisibilityStageNumberWidget(false);
	}

	if (CurrentGameModeType == EB2GameMode::Control)
	{
		SetVisibilitySB_TbMainCharName(false);
		SetVisibilityCRTInfoBGImage_01(false);
		SetVisibilityPlayerInfoWidget(false);
		SetVisibilityPlayTimeWidget(false);
		SetVisibilityStageNumberWidget(false);
		//SetVisibilityGoldWidget(false);
		SetVisibilityPauseButton(false);
		SetVisibilityTagWidget(false);

		if (UIP_AutoBattleIcon.IsValid())
		{
			UIP_AutoBattleIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2UIBattleMain::SetGold(int32 InValue)
{
	if (TB_Gold.IsValid())
		TB_Gold->SetText(FText::AsNumber(InValue));

	SetGold_BP(InValue);
}

void UB2UIBattleMain::UpdateStagePlayTimeInfoDisplay(int32 InCurrentTime)
{
	// 胶抛捞瘤 辆丰 矫 焊霸 瞪 沥焊客 促弗 版快甫 弥家拳窍扁 困秦 捞扒 概 平 诀单捞飘甫 窍绰 霸 亮阑 荐档.. 咯窍埃 面盒洒 磊林 诀单捞飘 登变 且 芭. 酒公府 词绢档 1檬俊 茄锅.
	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	const FServerStageID CurrentServerStageId = DocBS ? DocBS->GetServerStageId() : FServerStageID(1);
	const int32 ExpectedGrade = StageDataStore.GetStageExpectedClearStarGrade(CurrentServerStageId, InCurrentTime);
	// 割檬 饶狼 惑炔阑 固府 抗螟.
	const int32 PreExpectedGrade = StageDataStore.GetStageExpectedClearStarGrade(CurrentServerStageId, InCurrentTime + StageClearGradePreDropAnimTime);
	// 矫埃栏肺 Grade甫 窍遏矫虐绰 版快啊 茄啊瘤啊 登搁辑 3己->2己 力茄矫埃篮 荤侩登单 2己->1己 函拳 力茄矫埃篮 荤侩登瘤 臼绰促.
	bool bThisTimePreExpectedGradeDifferent = (ExpectedGrade != PreExpectedGrade && PreExpectedGrade != STAGE_CLEAR_GRADE_STAR_1);
	check(ExpectedGrade >= PreExpectedGrade);

	if (TB_StagePlayingTime.IsValid())
	{
		TB_StagePlayingTime->SetColorAndOpacity(bThisTimePreExpectedGradeDifferent ? StagePlayTimeTextColor_PreDrop : StagePlayTimeTextColor_Normal);
		
		if(static_cast<int32>(InCurrentTime/3600) > 0)
			TB_StagePlayingTime->SetText(FText::FromString(UB2UIManager::FormatSecondsAsTime(InCurrentTime, TEXT("hh:mm:ss"))));
		else
			TB_StagePlayingTime->SetText(FText::FromString(UB2UIManager::FormatSecondsAsTime(InCurrentTime, TEXT("mm:ss"))));
	}
	// 贸澜浚 救 弊法绰单 货肺 硅摹窍绊 窍促焊聪 狼档茄 箭磊尔 grade 啊 绢边巢 ぱ0ぱ
	if (IMG_StageClearGradeStarTwinkle_01.IsValid())
	{
		IMG_StageClearGradeStarTwinkle_01->SetVisibility(ExpectedGrade >= STAGE_CLEAR_GRADE_STAR_3 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (IMG_StageClearGradeStarTwinkle_02.IsValid())
	{
		IMG_StageClearGradeStarTwinkle_02->SetVisibility(ExpectedGrade >= STAGE_CLEAR_GRADE_STAR_2 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (IMG_StageClearGradeStarTwinkle_03.IsValid())
	{
		IMG_StageClearGradeStarTwinkle_03->SetVisibility(ExpectedGrade >= STAGE_CLEAR_GRADE_STAR_1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden); // 荤角惑 grade 啊 1焊促 累阑 老篮 绝阑 淀.
	}
	
	if (bThisTimePreExpectedGradeDifferent && !bLastPreExpectedGradeDifferent)
	{ // 割檬 饶俊 殿鞭捞 撤酒龙 吧肺 抗螟登扁 矫累茄 惑炔.
		PlayStageClearGradePreDropAnim(PreExpectedGrade);
	}

	bLastPreExpectedGradeDifferent = bThisTimePreExpectedGradeDifferent;
}

void UB2UIBattleMain::SetVisibilityClearGradeCondition(bool InShow)
{
	// 焊咯瘤绰 咆胶飘 磊眉绰 static 捞扼 促弗 镑俊辑 技泼.

	if (TB_ClearGradeCondition.IsValid())
	{
		TB_ClearGradeCondition->SetVisibility(InShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UIBattleMain::OnPressedSkillBtn(UB2UIBattleSkill* ClickedSkill)
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

void UB2UIBattleMain::OnClickTagBtn()
{
	PlayerStartTagClass<>::GetInstance().Signal();
}

void UB2UIBattleMain::CloseWidgetDelegate()
{
	OnPressedPauseBtn();
}

void UB2UIBattleMain::OnPressedPauseBtn()
{
	if (UGameplayStatics::IsGamePaused(GetOwningPlayer()) == false)
	{
		// Pause 皋春 widget 积己. 咯扁辑 post process DOF 汲沥阑 窍骨肺 捞巴捞 利侩登档废 窍扁 困秦 角力 pause 绰 茄 平 词冕促.
		StartPauseMenuClass<EB2GameMode>::GetInstance().Signal(GetB2GameModeType(this)); // UIManager_InGameMenu 率俊 殿废茄 StartPauseMenu 甫 妮窍霸 瞪 巴.
		SetPendingPause();
		//bPendingPause = true;

		PlayPauseBtnSound_BP();
	}
	else
	{
		// 荤角 Pause 惑怕俊辑绰 促矫 咯扁狼 PuaseButton 阑 穿福绰 巴捞 酒聪扼 Pause 皋春俊辑 Continue 滚瓢阑 喘矾具 倒酒啊霸 瞪 巴.
		UGameplayStatics::SetGamePaused(GetOwningPlayer(), false);
	}
}

void UB2UIBattleMain::OnPressedAttackBtn()
{
	PlayerStartAttackClass<>::GetInstance().Signal();
}

void UB2UIBattleMain::OnReleasedAttackBtn()
{
	PlayerStopAttackClass<>::GetInstance().Signal();
}

void UB2UIBattleMain::OnPressedGuardBtn()
{
	PlayerStartGuardClass<>::GetInstance().Signal();
}

void UB2UIBattleMain::OnReleasedGuardBtn()
{
	PlayerStopGuardClass<>::GetInstance().Signal();
}

void UB2UIBattleMain::OnClickBtnChat()
{
	if (UB2UIManager* UIMgrInst = UB2UIManager::GetInstance())
		UIMgrInst->OpenUI(UIFName::Chatting);
}

//====================================================================================
//									Doc Delegate
//====================================================================================
void UB2UIBattleMain::OnChangedCurPCClass(class UB2UIDocBase* Sender, int32 CurPCClass, int32 PrevCurPCClass)
{
	auto DocBattle = Cast<UB2UIDocBattle>(GetDoc());
	if (DocBattle)
	{
		if (MainHeroDoc && SubHeroDoc)
		{
			//久绢具 登绰 纳捞胶
			if(MainHeroDoc->GetDocId() == CurPCClass/* || SubHeroDoc->GetDocId() == DocBattle->GetTagPCClass()*/)
				return;

			Tag(CurPCClass, PrevCurPCClass);
		}
	}
	UpdateAttackButtonStyleToPCClass(IntToPCClass(CurPCClass));
}

void UB2UIBattleMain::OnChangedCurrPCHealth(class UB2UIDocBase* Sender, float CurrPCHealth, float PrevCurrPCHealth)
{
	auto DocBattle = Cast<UB2UIDocBattle>(Sender);
	if (!DocBattle)return;

	SetHPMainChar(CurrPCHealth, DocBattle->GetCurrPCMaxHealth(), DocBattle->GetCurrPCShield());
}
void UB2UIBattleMain::OnChangedCurrPCMaxHealth(class UB2UIDocBase* Sender, float CurrPCMaxHealth, float PrevCurrPCMaxHealth)
{
	auto DocBattle = Cast<UB2UIDocBattle>(Sender);
	if (!DocBattle)return;

	SetHPMainChar(DocBattle->GetCurrPCHealth(), CurrPCMaxHealth, DocBattle->GetCurrPCShield());
}
void UB2UIBattleMain::OnChangedCurrPCShield(class UB2UIDocBase* Sender, float CurrPCShield, float PrevCurrPCShield)
{
	auto DocBattle = Cast<UB2UIDocBattle>(Sender);
	if (!DocBattle)return;

	SetHPMainChar(DocBattle->GetCurrPCHealth(), DocBattle->GetCurrPCMaxHealth(), CurrPCShield);
}
void UB2UIBattleMain::OnChangedTagPCHealth(class UB2UIDocBase* Sender, float TagPCHealth, float PrevTagPCHealth)
{
	auto DocBattle = Cast<UB2UIDocBattle>(Sender);
	if (!DocBattle)return;

	SetHPSubChar(TagPCHealth, DocBattle->GetTagPCMaxHealth());
}
void UB2UIBattleMain::OnChangedTagPCMaxHealth(class UB2UIDocBase* Sender, float TagPCMaxHealth, float PrevTagPCMaxHealth)
{
	auto DocBattle = Cast<UB2UIDocBattle>(Sender);
	if (!DocBattle)return;

	SetHPSubChar(DocBattle->GetTagPCHealth(), TagPCMaxHealth);
}
void UB2UIBattleMain::OnChangedIsCountingTagCooltime(class UB2UIDocBase* Sender, bool IsCountingTagCooltime, bool PrevIsCountingTagCooltime)
{
	bIsCountingTagCooltime = IsCountingTagCooltime;
}

void UB2UIBattleMain::OnChangedIsTagPossible(class UB2UIDocBase* Sender, bool IsTagPossible, bool PrevIsTagPossible)
{
	if (!CheckInDebuffAction())
	{
		SetEnabledTagBtn(IsTagPossible);
		SetEnabledSubPortrait(IsTagPossible);

		auto DocBattle = Cast<UB2UIDocBattle>(Sender);
		if (!DocBattle)return;

		if (IsTagPossible == bIsTagDeath)
		{
			if (DocBattle->GetCurrPCHealth() <= 0.f ||
				DocBattle->GetTagPCHealth() <= 0.f)
			{
				bIsTagDeath = true;
			}
			else
				bIsTagDeath = false;
		}
		if (O_Death.IsValid())
			O_Death->SetVisibility(bIsTagDeath ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		if (bIsTagDeath)
			SetTagCooltimeVisible(0);

	}
}

void UB2UIBattleMain::OnChangedIsTagDisabledByCooltime(class UB2UIDocBase* Sender, bool IsTagDisabledByCooltime, bool PrevIsTagDisabledByCooltime)
{
	//SetEnabledTagBtn(!IsTagDisabledByCooltime);
	SetTagCooltimeVisible(IsTagDisabledByCooltime); // 寸厘 墨款飘绰 救 窍瘤父 咯扁辑何磐 焊咯林扁 矫累.
}

void UB2UIBattleMain::OnChangedRemainingTagCooltime(class UB2UIDocBase* Sender, float RemainingTagCooltime, float PrevRemainingTagCooltime)
{
	SetTagCooltime(RemainingTagCooltime);
}

void UB2UIBattleMain::OnChangedIsDefenseEther(class UB2UIDocBase* Sender, bool IsDefenseEther, bool PrevIsDefenseEther)
{
	if (UIP_DefenseEtherIcon.IsValid())
	{
		if (IsDefenseEther)
		{
			if (UIP_DefenseEtherIcon->GetVisibility() == ESlateVisibility::Collapsed)
				UIP_DefenseEtherIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UIP_DefenseEtherIcon->Init();
		}
		else
		{
			if (UIP_DefenseEtherIcon->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
				UIP_DefenseEtherIcon->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
void UB2UIBattleMain::OnChangedIsOffenseEther(class UB2UIDocBase* Sender, bool IsOffenseEther, bool PrevIsOffenseEther)
{
	if (UIP_OffenseEtherIcon.IsValid())
	{
		if (IsOffenseEther)
		{
			if (UIP_OffenseEtherIcon->GetVisibility() == ESlateVisibility::Collapsed)
				UIP_OffenseEtherIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UIP_OffenseEtherIcon->Init();
		}
		else
		{
			if (UIP_OffenseEtherIcon->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
				UIP_OffenseEtherIcon->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIBattleMain::OnChangedOffenseEtherCoolTime(class UB2UIDocBase* Sender, float InRemainingCooltime, float InPrevRemainingCooltime)
{
	if (UIP_OffenseEtherIcon.IsValid())
	{
		if (UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc()))
		{
			UIP_OffenseEtherIcon->UpdateCoolTime(DocBattle->GetOffenseEtherMaxCoolTime(), InRemainingCooltime, DocBattle->GetEnableOffenseEther());
		}
	}
}

void UB2UIBattleMain::OnChangedDefenseEtherCoolTime(class UB2UIDocBase* Sender, float InRemainingCooltime, float InPrevRemainingCooltime)
{
	if (UIP_DefenseEtherIcon.IsValid())
	{
		if (UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc()))
		{
			UIP_DefenseEtherIcon->UpdateCoolTime(DocBattle->GetDefenseEtherMaxCoolTime(), InRemainingCooltime, DocBattle->GetEnableDefenseEther());
		}
	}
}

void UB2UIBattleMain::OnChangedGold(class UB2UIDocBase* Sender, int32 Gold, int32 PrevGold)
{
	
	auto DocUser = Cast<UB2UIDocUser>(Sender);
	if (!DocUser)return;
	SetGold(Gold - DocUser->GetCombatStartGold());
	

	SetGold(Gold);
}

void UB2UIBattleMain::OnChangedbCurrentlyBossAppeared(class UB2UIDocBase* Sender, bool bCurrentlyBossAppeared, bool PrevbCurrentlyBossAppeared)
{
	if (bCurrentlyBossAppeared == true)
	{
		if(!PrevbCurrentlyBossAppeared)  //趣矫 吝汗秦辑 甸绢 棵 荐 乐栏聪 茄锅 鞘磐
		{
			UB2UIManager::GetInstance()->OpenUI(UIFName::BossStatus);
		}
		else
		{ // 粱 汗棱茄 荤沥栏肺 UB2UIBossStatus 率俊辑 BossMobClass 客 BossMobClassVariation 函版俊 措茄 贸府甫 窍瘤 给秦辑 咯扁辑 措脚..
			// 拱沸 CurrentlyBossAppeared 技泼窍扁 傈俊 BossMobClass 客 BossMobClassVariation 阑 力措肺 技泼秦 初酒具 茄促.
			// BossStatus 凯府扁 傈俊 攫力唱 BossMobClass 客 Variation 捞 利例洒 技泼捞 等促搁 咯扁辑 救 秦拎档 凳. 焊胶啊 楷崔酒 殿厘且 锭 部老鳖毫 救傈厘摹肺 秦 滴绰 芭.
			UB2UIBossStatus* BossStatusUI = UB2UIManager::GetInstance()->GetUI<UB2UIBossStatus>(UIFName::BossStatus);
			UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
			if (BossStatusUI && DocBattle)
			{
				BossStatusUI->UpdateBossInfo(DocBattle->GetBossMobClass(), DocBattle->GetBossMobClassVariation());
			}
		}
	}
	else if (PrevbCurrentlyBossAppeared == true && bCurrentlyBossAppeared == false)
	{
		UB2UIManager::GetInstance()->CloseUI(UIFName::BossStatus);
	}
}

void UB2UIBattleMain::OnChangedIsTagSuggested(class UB2UIDocBase* Sender, bool bTagSuggested, bool bPrevTagSuggested)
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	const bool bIsForTagAttack = DocBattle ? DocBattle->GetIsTagAttackSuggested() : false; // 捞霸 酒聪搁 敲饭捞绢 牧叼记 (HP) 俊 狼茄 巴. IsTagAttackSuggested 甫 咯扁辑 眉农且 荐 乐档废 IsTagSuggested 焊促 刚历 技泼秦具 窃.

	if (bTagSuggested != bPrevTagSuggested)
	{
		if (IMG_FX_TagSuggest.IsValid())
		{
			IMG_FX_TagSuggest->SetVisibility((bTagSuggested && !bIsForTagAttack) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
		if (CP_TagSuggest_TagCharChallenge.IsValid())
		{
			//胶懦捞电 牧叼记捞电 葛滴 SuggestText 劝己拳
			//CP_TagSuggest_TagCharChallenge->SetVisibility((bTagSuggested && !bIsForTagAttack) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
			CP_TagSuggest_TagCharChallenge->SetVisibility(bTagSuggested ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
		if (CP_TagAttackSuggestSet.IsValid())
		{
			CP_TagAttackSuggestSet->SetVisibility((bTagSuggested && bIsForTagAttack) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
		
		if (bTagSuggested){
			PlayTagSuggestAnim_BP();
			UpdateTagSuggestDialogue(bIsForTagAttack);

			if (!bIsForTagAttack){
				PlayTagSuggestByPlayerConditionAnim_BP();
			}
		}
		else{
			StopTagSuggestAnim_BP();
			StopTagSuggestByPlayerConditionAnim_BP(); // 咯扁俊 HP 肺 牢茄 怕弊鼻厘 惑怕烙阑 疙矫窍绰 炼扒捞 乐促搁 娟扁啊 促福变 茄单..
		}
	}
}

void UB2UIBattleMain::OnChangedStagePlayTime(class UB2UIDocBase* Sender, int32 CurrentTime, int32 PrevTime)
{
	if (CurrentGameModeType == EB2GameMode::CounterDungeon)
		return;

	if (CurrentTime != PrevTime)
	{
		UpdateStagePlayTimeInfoDisplay(CurrentTime);
	}
}

void UB2UIBattleMain::OnChangedCharacterSkillBuffState(class UB2UIDocBase* Sender, TArray<FUISkillBuffDisplayInfo_Character> CurrentState, TArray<FUISkillBuffDisplayInfo_Character> Prevstate)
{
	// 馆拜 带傈老锭绰 公矫
	if (CurrentGameModeType == EB2GameMode::CounterDungeon 
		|| CurrentGameModeType == EB2GameMode::Tutorial 
		|| CurrentGameModeType == EB2GameMode::Control)
		return;

	// TODO : 纳腐磐 滚橇率栏肺 橇府隆捞尔 胶畔 捞侥且订 OnChangedStateDamangeSkillBuffState 窃荐救俊 牢遣 阜绰何盒 爱绊客具窃.
	UpdateSkillBuffIcons();

	const bool bDisableInput = CheckInDebuffAction();

	SetAttackEnable(!bDisableInput);
	SetGuardEnable(!bDisableInput);
	
	auto* DocBattle = Cast<UB2UIDocBattle>(Sender);
	if (DocBattle && DocBattle->GetIsTagPossible())
	{
		SetEnabledTagBtn(!bDisableInput);
		SetEnabledSubPortrait(!bDisableInput);
	}
}

void UB2UIBattleMain::OnChangedCounterAttackPoint(class UB2UIDocBase* Sender, int32 CurrPoint, int32 PrevPoint)
{
	if (CurrentGameModeType == EB2GameMode::CounterDungeon)
	{
		if (UIP_CounterAttack_Skill.IsValid() && MainHeroDoc)
		{
			auto SkillDoc = MainHeroDoc->GetSkillDocByInputIndex(0);
			auto* DocBattle = Cast<UB2UIDocBattle>(Sender);

			if (SkillDoc && DocBattle)
			{				
				SkillDoc->SetSkillStackMaxPoint(DocBattle->GetMaxCounterAttackPoint());
				SkillDoc->SetSkillStackPoint(DocBattle->GetCounterAttackPoint());
			}
		}
	}
}

void UB2UIBattleMain::OnChangedQTEEnableState(class UB2UIDocBase* Sender, EQTEUIAttackEnableTimingState CurrQTEState, EQTEUIAttackEnableTimingState PrevQTEState)
{
	SetQTEEnable(CurrQTEState);
}

void UB2UIBattleMain::OnChangedQTEMounted(class UB2UIDocBase* Sender, bool bCurrentMounted, bool bPrevMounted)
{
	SetForQTEMountUI(bCurrentMounted);
}

void UB2UIBattleMain::OnChangedQTEMountedAndReadyForAttack(class UB2UIDocBase* Sender, bool bCurrentMountedAndReady, bool bPrevMountedAndReady)
{
	CheckForQTEStateUISetup();
}

void UB2UIBattleMain::OnChangedPlayerInQTEState(class UB2UIDocBase* Sender, bool bCurrentInQTEState, bool bPrevInQTEState)
{
	CheckForQTEStateUISetup();
}

void UB2UIBattleMain::OnChangedResurrectBuffValueAttack(class UB2UIDocBase* Sender, int32 NewValue, int32 PrevValue)
{
	CreateOrUpdateResurrectBuffIcon(EResurrectBuffType::RBT_Attack);
}

void UB2UIBattleMain::OnChangedResurrectBuffValueDefense(class UB2UIDocBase* Sender, int32 NewValue, int32 PrevValue)
{
	CreateOrUpdateResurrectBuffIcon(EResurrectBuffType::RBT_Defense);
}

void UB2UIBattleMain::OnChangedResurrectBuffValueHealth(class UB2UIDocBase* Sender, int32 NewValue, int32 PrevValue)
{
	CreateOrUpdateResurrectBuffIcon(EResurrectBuffType::RBT_Health);
}

void UB2UIBattleMain::OnTagBTNEnableFunction(bool IsEnable)
{
	if (CP_TagBottomEnabledTrue.IsValid())
		CP_TagBottomEnabledTrue->SetVisibility(IsEnable ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (CP_TagBottomEnabledFalse.IsValid())
	{
		CP_TagBottomEnabledFalse->SetVisibility(IsEnable ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
	}
}

void UB2UIBattleMain::OnAbnormalStateChanged(class UB2UIDocBase* Sender, bool NewValue, bool PrevValue)
{
	const bool IsAbnormalState = NewValue;

	for (uint8 Cnt = 0; Cnt < MAX_SKILL_INPUT; Cnt++)
	{
		// Freezing/Stun 惑怕俊辑 哈绢唱绰 格利牢 SurviveSkill篮 Disable 矫虐瘤 臼澜
		if (SkillAssets.IsValidIndex(Cnt) && SkillAssets[Cnt] != nullptr)
		{
			auto* SkillAsset = SkillAssets[Cnt];
			if (SkillAsset->IsSurviveSkill() == false && SkillAsset->IsInCooltime() == false)
				SetSkillEnable(Cnt, !IsAbnormalState);
		}
	}
}

UB2UIInGameBuffIcon* UB2UIBattleMain::CreateBuffIconCommon(TSubclassOf<UB2UIInGameBuffIcon> InCreateClass, UHorizontalBox* InToCreatePanel)
{
	if (InCreateClass)
	{
		const bool bIsForSoRSet = (InCreateClass == BuffIconClass_Stage || InCreateClass == BuffIconClass_Resurrect); // For Stage or Resurrect buff.
		UHorizontalBox* ToCreatePanel = InToCreatePanel == nullptr ? HB_StageResurrectBuffIconSet.Get() : InToCreatePanel;
		UB2UIInGameBuffIcon* CreateClassDefObj = Cast<UB2UIInGameBuffIcon>(InCreateClass->GetDefaultObject());
		UB2UIInGameBuffIcon* CreatedWidget = Cast<UB2UIInGameBuffIcon>(
			DynCreateInHorizontalBox(InCreateClass, this, ToCreatePanel, BuffIconSize, FMargin())
			);

		if (CreatedWidget)
		{
			CreatedWidget->Init();
			if (bIsForSoRSet){
				AllMyBuffIcons_Resurrection.Add(CreatedWidget);
			}
			else{
				AllMyBuffIcons_Skill.Add(CreatedWidget);
			}			
		}

		return CreatedWidget;
	}
	return NULL;
}

UB2UIInGameBuffIcon_Stage* UB2UIBattleMain::CreateOrFindStageBuffIcon(EStageBuffType InBuffType)
{
	// 老窜 茫扁何磐.
	for (UB2UIInGameBuffIcon* ThisBuffIcon : AllMyBuffIcons_Resurrection)
	{
		UB2UIInGameBuffIcon_Stage* CastedStageBuffIcon = Cast<UB2UIInGameBuffIcon_Stage>(ThisBuffIcon);

		if (CastedStageBuffIcon && CastedStageBuffIcon->GetBuffType() == InBuffType)
		{
			return CastedStageBuffIcon;
		}
	}

	UB2UIInGameBuffIcon_Stage* NewIcon = Cast<UB2UIInGameBuffIcon_Stage>(CreateBuffIconCommon(BuffIconClass_Stage));
	if (NewIcon)
	{
		NewIcon->SetBuffType(InBuffType);
		return NewIcon;
	}
	return NULL;
}

UB2UIInGameBuffIcon_Resurrect* UB2UIBattleMain::CreateOrUpdateResurrectBuffIcon(EResurrectBuffType InBuffType)
{
	// 何劝 滚橇绰 窍唱父 钎矫瞪 芭扁 东俊 扁粮俊 乐绰 吧 茫酒辑 诀单捞飘 窍电瘤 货肺 积己
	UB2UIInGameBuffIcon_Resurrect* CreatedOrFound = NULL;

	for (UB2UIInGameBuffIcon* ThisBuffIcon : AllMyBuffIcons_Resurrection)
	{
		UB2UIInGameBuffIcon_Resurrect* CastedResurrectBuffIcon = Cast<UB2UIInGameBuffIcon_Resurrect>(ThisBuffIcon);

		if (CastedResurrectBuffIcon)
		{
			CreatedOrFound = CastedResurrectBuffIcon;
			break;
		}
	}

	if (!CreatedOrFound)
		CreatedOrFound = Cast<UB2UIInGameBuffIcon_Resurrect>(CreateBuffIconCommon(BuffIconClass_Resurrect, HB_Resurrect.Get()));
		
	if (CreatedOrFound)
	{
		CreatedOrFound->SetBuffType(InBuffType);
		return CreatedOrFound;
	}
	return NULL;
}

UB2UIInGameBuffIcon_Skill* UB2UIBattleMain::CreateOrUpdateSkillBuffIcon(EUIUnitedSkillBuffType InType, float RemainingTime)
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
		CreatedOrFound = Cast<UB2UIInGameBuffIcon_Skill>(CreateBuffIconCommon(BuffIconClass_Skill));
		if (CreatedOrFound)
		{
			CreatedOrFound->SetBuffType(InType);
		}
	}

	if (CreatedOrFound)
	{
		CreatedOrFound->SetRemainigTime(RemainingTime);
	}

	return CreatedOrFound;
}

UB2UIInGameBuffIcon_GuildBuff * UB2UIBattleMain::CreateOrUpdateGuildBuffIcon(EItemOption InBuffType)
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
			CreatedOrFound = Cast<UB2UIInGameBuffIcon_GuildBuff>(DynCreateInHorizontalBox(BuffIconClass_Guild, this, HB_Guild.Get(), BuffIconSize, FMargin()));

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

void UB2UIBattleMain::DestroyAllMyBuffIcons_Resurrection()
{
	for (UB2UIInGameBuffIcon* ThisBuffIcon : AllMyBuffIcons_Resurrection)
	{
		if (ThisBuffIcon)
		{
			ThisBuffIcon->UnbindDoc();
			ThisBuffIcon->DestroySelf(UB2UIManager::GetInstance());
		}
	}
	AllMyBuffIcons_Resurrection.Empty();
}

void UB2UIBattleMain::DestroyAllMyBuffIcons_Skill()
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

void UB2UIBattleMain::DestroyAllMyBuffIcons_Guild()
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

void UB2UIBattleMain::CollectAndSortSkillBuffData(bool& bOutOnlyCountTimeChanged)
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
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
					if (ThisPrevBuff.MyType == ThisCurrBuff.MyType){
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

void UB2UIBattleMain::UpdateSkillBuffIconsByCollectedData(bool bUpdateCountTimeOnly)
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

void UB2UIBattleMain::UpdateSkillBuffIcons()
{
	bool bOnlyCountTimeChanged = false;
	CollectAndSortSkillBuffData(bOnlyCountTimeChanged);
	UpdateSkillBuffIconsByCollectedData(bOnlyCountTimeChanged);
}

void UB2UIBattleMain::BeginTagAnim(EPCClass NewTaggedClass, bool bIsTagAttack)
{
	DestroyTagAnimUIP(false); // 沥惑利牢 惑炔捞搁 捞傈俊 敲饭捞窍带 霸 辆丰登绊 积己沁带 扒 妮归罐酒 捞固 destroy 登绢 乐绢具.. 弊矾唱 destroy 捞傈俊 怕弊甫 肚 沁阑 荐档.

	if (!bIsTagAttack) // 楷拌 怕弊绰 捞芭 救 窃.
	{
		CreatedTagAnimUIP = LoadAndBeginTAssetBattleTagAnimUIP<UB2UIBattleMain>(
			this, TagAnimUIPClass, &LoadedTagAnimUIPClass, NewTaggedClass, bIsTagAttack);
	}
}

void UB2UIBattleMain::DestroyTagAnimUIP(bool bUnloadTAsset)
{
	if (bUnloadTAsset)
	{
		UB2AssetLoader::Get()->UnloadAsset(TagAnimUIPClass);
		LoadedTagAnimUIPClass = NULL;
	}

	if (CreatedTagAnimUIP)
	{
		CreatedTagAnimUIP->DestroySelf(UB2UIManager::GetInstance());
		CreatedTagAnimUIP = NULL;
	}
}

void UB2UIBattleMain::RequestAsyncLoadWidgets()
{ 	// 咯扁辑 悼利 肺爹窍绰 UIP 甸阑 固府 归弊扼款靛 肺爹秦 初酒辑 傈捧 吝 hitch 甫 弥家拳 窍扁 困窃.
	if (auto* AssetLoader = UB2AssetLoader::Get())
	{
		auto LoadCallback = FB2AsyncLoadingComplete::CreateUObject(this, &UB2UIBattleMain::OnCompleteAsyncLoad);

		AssetLoader->RequestAsyncLoad(GetQTEAsyncLoadName(), QTEUIPClass, LoadCallback);
		AssetLoader->RequestAsyncLoad(GetTagAnimAsyncLoadName(), TagAnimUIPClass, LoadCallback);
	}
}

void UB2UIBattleMain::OnCompleteAsyncLoad(const FString& RequesetName)
{
	if (RequesetName == GetQTEAsyncLoadName())
		LoadedQTEUIPClass = QTEUIPClass.Get();

	else if (RequesetName == GetTagAnimAsyncLoadName())
		LoadedTagAnimUIPClass = TagAnimUIPClass.Get();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (UB2AssetLoader::bEnableDevLog)
	{ 
		BII_SCREEN_LOG(FString::Printf(TEXT("[UB2UIBattleMain] AsyncLoad request of %s completed."), *RequesetName),
			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 8.0f);
	}
#endif

}

void UB2UIBattleMain::ProcessEvadeCoolTimeUI()
{
	if (IMG_EvadeSkillCooltime.IsValid())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		ABladeIIPlayer* pPlayer = PC ? Cast<ABladeIIPlayer>(PC->GetCharacter()) : nullptr;

		if (pPlayer)
		{
			//酿鸥烙 赣磐府倔 贸府
			UMaterialInstanceDynamic* pMID = IMG_EvadeSkillCooltime->GetDynamicMaterial();

			if (pMID)
			{
				pMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, pPlayer->GetEvadeCoolTimeProgress());
			}
		}
	}
}

bool UB2UIBattleMain::CheckInDebuffAction()
{
	bool bDisableInputByBuff = false;
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();

	if (DocBattle)
	{
		for (const FUISkillBuffDisplayInfo_Character& BuffInfo : DocBattle->GetCharacterBuffState())
		{
			if (BuffInfo.MyType == EBuffType::Debuff_Freezing ||
				BuffInfo.MyType == EBuffType::Debuff_Stun)
			{
				bDisableInputByBuff = true;
				break;
			}
		}
	}

	return bDisableInputByBuff;
}

void SetHPShieldPBCommon(UProgressBar* InHPPB, UProgressBar* InShieldPB, float HPValue, float MaxHPValue, float ShieldValue)
{
	// HPValue 客 ShieldValue 绰 MaxHPValue 俊 措茄 厚啦捞 酒囱 例措蔼捞绢具 窃.

	float FinalShieldAmount = InShieldPB ? ShieldValue : 0.0f;// 咯扁肺 甸绢坷绰 蔼俊 蝶扼 促弗 侥 (MaxHPValue * ShieldValue) 阑 荤侩窍霸 瞪 荐档.

	float FinalMaxValue = FMath::Max(MaxHPValue + FinalShieldAmount, KINDA_SMALL_NUMBER);
	
	if (InHPPB){
		InHPPB->SetPercent( FMath::Clamp(HPValue / FinalMaxValue, 0.0f, 1.0f) );
	}

	if (InShieldPB){
		InShieldPB->SetPercent((FinalShieldAmount <= 0.0f) ? 0.0f : FMath::Clamp((HPValue + FinalShieldAmount) / FinalMaxValue, 0.0f, 1.0f));
	}
}

void UB2UIBattleMain::SetAttackEnable(bool bToSet)
{
	if (BTN_Attack.IsValid() || BTN_MountAttack.IsValid())
	{
		if (!bToSet)
		{
			//SWidget捞 Focus甫 啊廉埃盲 Disable登绰 版快, 促弗 Widget 棺 GameViewport俊辑 Focus甫 给 啊廉啊绰 泅惑捞 惯积窍咯
			//碍力利栏肺 MouseUp阑 阂矾 Focus甫 酪霸 茄促.
// 			FSlateApplication::Get().OnMouseUp(EMouseButtons::Left);
// 
// 			for (int32 i = 0; i < EKeys::NUM_TOUCH_KEYS; ++i)
// 				FSlateApplication::Get().OnTouchEnded(FVector2D::ZeroVector, i, 0);
		}
		if(BTN_Attack.IsValid())
			BTN_Attack->SetIsEnabled(bToSet);
		if(BTN_MountAttack.IsValid())
			BTN_MountAttack->SetIsEnabled(bToSet);
	}

	if (FX_IMG_Enable.IsValid())
	{
		FX_IMG_Enable->SetVisibility(bToSet ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2UIBattleMain::SetGuardEnable(bool bEnabled)
{
	if (BTN_Guard.IsValid())
		BTN_Guard->SetIsEnabled(bEnabled);
}

void UB2UIBattleMain::SetCounterFinishMode()
{
	for (auto Widget : CounterAttackWidgets)
	{
		if (Widget.IsValid())
			Cast<UWidget>(Widget.Get())->SetVisibility(ESlateVisibility::Hidden);
	}
		
	if (UIP_CounterFinishAttack.IsValid())
		UIP_CounterFinishAttack->SetVisibility(ESlateVisibility::Visible);

	if (CP_Attack.IsValid())
		CP_Attack->SetVisibility(ESlateVisibility::Hidden);

	if (CP_Guard.IsValid())
		CP_Guard->SetVisibility(ESlateVisibility::Collapsed);

	RemoveAllCounterSuggestRing();
}

void UB2UIBattleMain::SetTagPCDead()
{

}

void UB2UIBattleMain::OnClickCounterFinishBtn()
{	
	if (UIP_CounterFinishAttack.IsValid())
		UIP_CounterFinishAttack->SetVisibility(ESlateVisibility::Hidden);

	CounterFinishAttackClass<>::GetInstance().Signal();
}

void UB2UIBattleMain::OnPressedShowClearGradeConditionBtn()
{
	SetVisibilityClearGradeCondition(true);
}
void UB2UIBattleMain::OnReleasedShowClearGradeConditionBtn()
{
	SetVisibilityClearGradeCondition(false);
}
void UB2UIBattleMain::OnBattleTagAnimUIPExpired(UB2UIBattleTagAnim* ExpiredUIP)
{// UB2UIBattleTagAnim 率栏肺何磐 坷绰 脚龋. 皋葛府 例距阑 困秦 促 登菌栏搁 destroy.. 窍妨绊 沁绰单 捞哩 hitch 力绢啊 歹 吝夸秦辑 肯傈洒 郴府瘤绰 臼绰促.
	checkSlow(ExpiredUIP && ExpiredUIP == CreatedTagAnimUIP);
	DestroyTagAnimUIP(false);
}

void UB2UIBattleMain::SetQTEEnable(EQTEUIAttackEnableTimingState InEnableState)
{ // 措眉肺 QTE 涝仿 啊瓷 鸥捞怪阑 舅府绰 芭绊 QTE 惑怕绰 酒丛.
	// 档吝俊 QTE UIP 甫 力芭且 荐 乐档废 泅犁 QTE 惑怕甫 咯扁肺 焊郴绰 霸 亮阑 淀..

	if (InEnableState != EQTEUIAttackEnableTimingState::None) { // None 老 锭 概 橇饭烙 甸绢坷绰 芭 鞍酒辑 咯扁绰 Enable 登菌阑 锭父 眉农. 波龙 锭绰 促弗 版肺肺 甸绢咳.
		CheckForQTEStateUISetup(); // 包访 UI 积己 棺 visibility 悸诀何磐.
	}

	if (CreatedQTEUIP) 
	{
		CreatedQTEUIP->UpdateByQTEEnableState(InEnableState);
	}
}

void UB2UIBattleMain::CheckForQTEStateUISetup()
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		// 扁夯利牢 QTE UIP 焊捞绰 炼扒篮 气承霸.
		const bool bQTEUIPMustbeVisible = (DocBattle->GetQTEEnableState() != EQTEUIAttackEnableTimingState::None || DocBattle->GetbIsQTEMounted() || DocBattle->GetbIsPlayerInQTEState());
		
		// 鞘夸且 锭父 积己秦辑 荤侩窍档废.
		if (bQTEUIPMustbeVisible){
			ConditionalCreateQTEUIP();
		}
		else{
			DestroyQTEUIP(false); // 盖 付瘤阜 酒聪搁 TAsset 篮 郴府瘤 臼澜.
		}

		// 措眉肺 楷免苞 剐立茄 QTE 惑怕 档吝俊绰 QTE 寇狼 促弗 UI 甸阑 见变促. 霉 QTE 鸥捞怪篮 酒流 角力 QTE 绰 酒聪聪 咯扁辑 见扁搁 寸炔胶矾匡 波绊 付款飘俊辑 傍拜捞 啊瓷茄 惑怕绰 傈捧 炼累阑 秦具窍聪 UI 啊 鞘夸窍绊.
		const bool bHideBattleMainUI = bQTEUIPMustbeVisible && (!DocBattle->GetbIsQTEMountedAndReadyForAttack() && DocBattle->GetQTEEnableState() == EQTEUIAttackEnableTimingState::None);
		HideHUDForQTE(bHideBattleMainUI ? true : false);
	}
}

void UB2UIBattleMain::UpdateGuildBuff()
{
	if (CurrentGameModeType == EB2GameMode::Scenario || CurrentGameModeType == EB2GameMode::Raid
		|| CurrentGameModeType == EB2GameMode::HeroTower || CurrentGameModeType == EB2GameMode::PVP_Tag
		|| CurrentGameModeType == EB2GameMode::Dimension)
	{
		ICharacterDataStore* CharacterDataStore = &BladeIIGameImpl::GetLocalCharacterData();
		if (CharacterDataStore != nullptr)
		{
			TArray<b2network::B2GuildSkillPtr> SortArray; 
			SortArray.Append(CharacterDataStore->GetGuildSkillInfos());

			SortArray.Sort([](auto& a, auto& b) {
				auto aData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(a->id, a->level);
				auto bData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(b->id, b->level);

				if (aData && bData)
				{
					auto GetSortIndex = [](int32 OptionType) { return B2UIUtil::GetGuildBuffSortValue(SvrToCliOptionType(OptionType)); };

					return GetSortIndex(aData->buff_option_type) > GetSortIndex(bData->buff_option_type);
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
}

void UB2UIBattleMain::HideHUDForQTE(bool bHide)
{
	// PlayerController 狼 CinematicMode 甫 荤侩窍妨聪 粱 部老 芭 鞍澜. 弊成 蝶肺 贸府.
	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetOwningPlayer());

	if (bHide)
	{
		// 芭狼 夸 BattleMain 父 见扁绰 沥档..
		// QTE UIP 绰 BattleMain 狼 child 啊 酒囱 刀磊利牢 UI widget 栏肺 积己登骨肺 咯扁俊 康氢阑 罐瘤 臼绰促. 沥富肺 见败具 且 锭啊 登绢档 唱棵 波鄂 娟变单 弊霸 巩力啊 登绰 惑炔捞 惯积窍搁 蝶肺 贸府甫 秦具 且 淀..
		// -> 捞力 鞍捞 见变促.
		this->ForceHide();

		if (CreatedQTEUIP)
		{
			CreatedQTEUIP->SetVisibilityFromBattleMainUI(ESlateVisibility::Hidden);
		}

		if (B2PC)
		{
			B2PC->SetVirtualJoystickVisibility(false);
		}
	}
	else
	{
		if (B2PC && !B2PC->IsInCinematicMode()) // 咯扁急 CinematicMode 柳涝阑 窍瘤 臼栏骨肺 趣矫扼档 CinematicMode 扼搁 促弗 镑俊辑 贸府且 抛聪 出狄
		{
			this->RestoreFromForceHidden();

			if (CreatedQTEUIP)
				CreatedQTEUIP->SetVisibilityFromBattleMainUI(ESlateVisibility::SelfHitTestInvisible);

			B2PC->SetVirtualJoystickVisibility(true);
		}
	}
}

void UB2UIBattleMain::ConditionalCreateQTEUIP()
{
	// 捞固 积己登绢 乐栏搁 被捞 货肺 父甸瘤 臼绰促. 
	if (!CreatedQTEUIP)
	{
		CreatedQTEUIP = LoadAndBeginTAssetQTEUIP<UB2UIBattleMain>(this, QTEUIPClass, &LoadedQTEUIPClass);
		if (CreatedQTEUIP){

			CreatedQTEUIP->BindDoc(); // 老何矾 咯扁辑 妮茄促. 官肺 酒贰 Destroy 俊辑狼 UnbindDoc 窍绊 娄嘎苗 初扁 困秦.
		}
	}
}
void UB2UIBattleMain::DestroyQTEUIP(bool bUnloadTAsset)
{
	if (bUnloadTAsset)
	{
		UB2AssetLoader::Get()->UnloadAsset(QTEUIPClass);
		LoadedQTEUIPClass = NULL;
	}

	if (CreatedQTEUIP)
	{
		CreatedQTEUIP->UnbindDoc();
		CreatedQTEUIP->DestroySelf(UB2UIManager::GetInstance());
		CreatedQTEUIP = NULL;
	}
}

void UB2UIBattleMain::SetSkillEnable(int32 SkillIndex, bool bEnable)
{
	if (SkillAssets.IsValidIndex(SkillIndex))
	{
		if (auto* SkillAsset = SkillAssets[SkillIndex])
		{
			SkillAsset->SetEnabledBtn(bEnable);
		}
	}
}

void UB2UIBattleMain::SetSkillLock(int32 SkillIndex, bool bLock)
{
	if (SkillAssets.IsValidIndex(SkillIndex))
	{
		if (auto* SkillAsset = SkillAssets[SkillIndex])
		{
			SkillAsset->SetLock(bLock);
		}
	}
}

void UB2UIBattleMain::SetForQTEMountUI(bool ToSet)
{ // ToSet 捞 true 扼 秦档 寸厘 付款飘 傍拜 涝仿鳖瘤 啊瓷茄 惑怕绰 酒匆 荐 乐促.
	if (CP_Attack.IsValid())
		CP_Attack->SetVisibility(ToSet ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	if (CP_MountAttack.IsValid())
		CP_MountAttack->SetVisibility(ToSet ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	CheckForQTEStateUISetup(); // 包访 UI 积己 棺 visibility 悸诀何磐.

	if (CreatedQTEUIP)
	{ // 付款飘 郴郴 难廉 乐绢具 窃. 窜, 努腐篮 救 登霸. 郴何俊辑 炼例..
		bool bVisibleTimeNum = (CurrentGameModeType == EB2GameMode::Tutorial) ? false : true;
		UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
		CreatedQTEUIP->SetQTEMounted(ToSet, DocBattle ? DocBattle->GetMaxQTEMountTime() : 1.0f, bVisibleTimeNum);
	}

	for (TPair<TWeakObjectPtr<UObject>,ESlateVisibility>& Pair : WidgetsToHideInMountAttackState)
	{
		auto WidgetWeakObjPtr	= Pair.Key;
		auto WidgetVisibility	= Pair.Value;

		if (!WidgetWeakObjPtr.IsValid())
			continue;

		UWidget* Widget = Cast<UWidget>(WidgetWeakObjPtr.Get());
		Widget->SetVisibility(ToSet ? ESlateVisibility::Hidden : WidgetVisibility);
	}
}

void UB2UIBattleMain::SetComboUIPosition(FVector2D vPos)
{
	if (UIP_BattleCombo.IsValid())
	{
		UCanvasPanelSlot* pSlot = Cast<UCanvasPanelSlot>(UIP_BattleCombo->Slot);

		if (pSlot)
		{
			pSlot->SetPosition(vPos);
		}
	}
}

void UB2UIBattleMain::InitCounterSuggestTimer()
{
	CounterAttackTimers.Empty();
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearAllTimersForObject(this);
}

void UB2UIBattleMain::AddCounterSuggestTimer(float time)
{
	// CounterSuggestRingTime 篮 CounterBattleMain狼 Construct俊辑 SuggetRing 局聪皋捞记狼 辨捞 蔼栏肺 技泼.
	if (time - CounterSuggestRingTime <= 0) return;

	FTimerHandle AnimStartTimerHandle;
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(AnimStartTimerHandle, this, &UB2UIBattleMain::OnPlayCounterSuggetAnim, time - CounterSuggestRingTime, false);
	CounterAttackTimers.AddUnique(AnimStartTimerHandle);
}

void UB2UIBattleMain::RemoveAllCounterSuggestRing()
{
	InitCounterSuggestTimer();

	if (IMG_CounterSuggestRing.IsValid())
		IMG_CounterSuggestRing->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIBattleMain::OnPauseCounterSuggestRing(float time)
{
	for (auto timerhandle : CounterAttackTimers)
	{
		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().PauseTimer(timerhandle);
	}

	FTimerHandle AnimStartTimerHandle;
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(AnimStartTimerHandle, this, &UB2UIBattleMain::OnUnPauseCounterSuggestRing, time, false);

}

void UB2UIBattleMain::OnUnPauseCounterSuggestRing()
{
	for (auto timerhandle : CounterAttackTimers)
	{
		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().UnPauseTimer(timerhandle);
	}
}

void UB2UIBattleMain::OnStageBegin()
{
	RequestAsyncLoadWidgets();
}

void UB2UIBattleMain::UpdateTagSuggestDialogue(const bool bIsForTagAttack)
{
	if (!TB_TagSuggest_TagCharChallenge.IsValid() ||
		!SubHeroDoc)
		return;

	if (UB2CharacterTagDialogueInfo* CharacterTagDialogueInfo = StaticFindCharacterTagDialogueInfo())
	{
		const int32 iSubClass = SubHeroDoc->GetDocId();

		if (bIsForTagAttack) //傍拜 胶懦 怕弊
			TB_TagSuggest_TagCharChallenge->SetText(CharacterTagDialogueInfo->GetRandomTagSuggestTextFromSkill(iSubClass));
		else //后荤 惑怕 怕弊
			TB_TagSuggest_TagCharChallenge->SetText(CharacterTagDialogueInfo->GetRandomTagSuggestTextFromDyingHp(iSubClass));
	}
}

void UB2UIBattleMain::PlayTagDialogueSound(EPCClass SoundPlayPCClass, EPCClass IntarActionPCClass)
{
	if (UB2CharacterTagDialogueInfo* CharacterTagDialogueInfo = StaticFindCharacterTagDialogueInfo())
	{
		if (USoundCue* pTagDialogueSound = CharacterTagDialogueInfo->GetRandomTagSound(SoundPlayPCClass, IntarActionPCClass))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), pTagDialogueSound);
		}
	}
}

void UB2UIBattleMain::OnInitAnimationBegin()
{
	if (CP_MainButton.IsValid())
	{
		CP_MainButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UB2UIBattleMain::OnInitAnimationFinish()
{
	if (CP_MainButton.IsValid())
	{
		CP_MainButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UB2UIBattleMain::OnPlayCounterSuggetAnim()
{
	OnPlayAnimCounterSuggestRing_BP();
	
	if(CounterAttackTimers.Num() > 0)
		CounterAttackTimers.RemoveAt(0);
}

#if !UE_BUILD_SHIPPING
void OnChangedDevFxLODLevel()
{
	//for (TObjectIterator<UParticleSystemComponent> PSCIT; PSCIT; ++PSCIT)
	//{
	//	UParticleSystemComponent* ThisComp = Cast<UParticleSystemComponent>(*PSCIT);
	//	if (ThisComp && ThisComp->Template)
	//	{
	//		if (IsB2CharFxCheatLODInUse()) {
	//			ThisComp->SetLODLevel(0); // 咯扁 持绢林绰 蔼篮 狼固绝绊 傲 诀单捞飘 瞒盔..
	//		}
	//		else {
	//			if (ThisComp->GetWorld()) {
	//				ThisComp->RestoreLODLevelToDesiredValue(); // 捞矾搁 盔贰 抗惑登绰 蔼栏肺 倒酒啊妨唱
	//			}
	//		}
	//	}
	//}
}
#endif

void UB2UIBattleMain::OnClickBtnDevFxLODPC()
{
//#if !UE_BUILD_SHIPPING
//	GB2CharFxCheatLODLevel_PC++;
//	if (GB2CharFxCheatLODLevel_PC > GB2CharFxCheatLODLevel_PC_Max)
//	{
//		GB2CharFxCheatLODLevel_PC = -1; //0 捞 酒聪扼 -1 肺 倒酒皑. 扁夯 LOD 扁瓷狼 狼固肺
//	}
//	OnChangedDevFxLODLevel();
//	UpdateDevFxLODWidgets();
//#endif
}
void UB2UIBattleMain::OnClickBtnDevFxLODMob()
{
//#if !UE_BUILD_SHIPPING
//	GB2CharFxCheatLODLevel_Mob++;
//	if (GB2CharFxCheatLODLevel_Mob > GB2CharFxCheatLODLevel_Mob_Max)
//	{
//		GB2CharFxCheatLODLevel_Mob = -1; //0 捞 酒聪扼 -1 肺 倒酒皑. 扁夯 LOD 扁瓷狼 狼固肺
//	}
//	OnChangedDevFxLODLevel();
//	UpdateDevFxLODWidgets();
//#endif
}
void UB2UIBattleMain::OnClickBtnDevFxLODBoss()
{
//#if !UE_BUILD_SHIPPING
//	GB2CharFxCheatLODLevel_Boss++;
//	if (GB2CharFxCheatLODLevel_Boss > GB2CharFxCheatLODLevel_Boss_Max)
//	{
//		GB2CharFxCheatLODLevel_Boss = -1; //0 捞 酒聪扼 -1 肺 倒酒皑. 扁夯 LOD 扁瓷狼 狼固肺
//	}
//	OnChangedDevFxLODLevel();
//	UpdateDevFxLODWidgets();
//#endif
}

#if !UE_BUILD_SHIPPING
FSlateColor GetDevFxLODTestBtnColor(int32 InCheatLODLevel, int32 SupposedMaxLODLevel)
{
	return FSlateColor(
		(InCheatLODLevel == SupposedMaxLODLevel) ? FLinearColor(1.0f,0.0f,0.0f) :
			(InCheatLODLevel == 0 ? FLinearColor(1.0f,1.0f,1.0f) : (
				(InCheatLODLevel < 0 ? FLinearColor(0.5f,0.5f,0.5f) : // 扁夯 LOD 扁瓷阑 荤侩窍绰 惑炔俊辑 雀祸
					FLinearColor(1.0f,0.5f,0.0f) ) // 捞扒 1 ~ Max 荤捞 吝埃 LOD 窜拌啊 乐绰 版快.. 林炔祸 钎矫.
	)));
}

void AdjustDevFxLODTestBtnStyleCommon(UB2Button* InBtn, int32 CurrCheatLODLevel, int32 MaxCheatLODLevel)
{
	if (InBtn)
	{
		FButtonStyle AdjustStyle = InBtn->GetStyle();
		AdjustStyle.Normal.TintColor = GetDevFxLODTestBtnColor(CurrCheatLODLevel, MaxCheatLODLevel);
		AdjustStyle.Hovered.TintColor = AdjustStyle.Normal.TintColor; // Normal 父 函版秦辑绰 面盒窍瘤啊 臼焙. ぱぱ
		AdjustStyle.Pressed.TintColor = AdjustStyle.Hovered.TintColor;
		InBtn->SetStyle(AdjustStyle);
	}
}

void UB2UIBattleMain::UpdateDevFxLODWidgets() // 配呕 诀单捞飘
{
	//extern bool gIsDevUI;

	//if (P_DevLODTest.IsValid())
	//{
	//	P_DevLODTest->SetVisibility(gIsDevUI ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	//}

	//if (BTN_Dev_FxLOD_PC.IsValid())
	//{
	//	AdjustDevFxLODTestBtnStyleCommon(BTN_Dev_FxLOD_PC.Get(), GB2CharFxCheatLODLevel_PC, GB2CharFxCheatLODLevel_PC_Max);
	//}
	//if (BTN_Dev_FxLOD_Mob.IsValid())
	//{
	//	AdjustDevFxLODTestBtnStyleCommon(BTN_Dev_FxLOD_Mob.Get(), GB2CharFxCheatLODLevel_Mob, GB2CharFxCheatLODLevel_Mob_Max);
	//}
	//if (BTN_Dev_FxLOD_Boss.IsValid())
	//{
	//	AdjustDevFxLODTestBtnStyleCommon(BTN_Dev_FxLOD_Boss.Get(), GB2CharFxCheatLODLevel_Boss, GB2CharFxCheatLODLevel_Boss_Max);
	//}

	//if (TB_Dev_FxLOD_PC.IsValid())
	//{
	//	TB_Dev_FxLOD_PC->SetText(FText::FromString(FString::Printf(TEXT("FX LOD\r\nPC %d"), GB2CharFxCheatLODLevel_PC)));
	//}
	//if (TB_Dev_FxLOD_Mob.IsValid())
	//{
	//	TB_Dev_FxLOD_Mob->SetText(FText::FromString(FString::Printf(TEXT("FX LOD\r\nMob %d"), GB2CharFxCheatLODLevel_Mob)));
	//}
	//if (TB_Dev_FxLOD_Boss.IsValid())
	//{
	//	TB_Dev_FxLOD_Boss->SetText(FText::FromString(FString::Printf(TEXT("FX LOD\r\nBoss %d"), GB2CharFxCheatLODLevel_Boss)));
	//}
}
#endif

#undef ADD_WIDGET_SLOT

bool UB2UIBattleMain::RedDotCondition_Chat()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Chat();
}

void UB2UIBattleMain::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Chat.Get()), &UB2UIBattleMain::RedDotCondition_Chat);
}

void UB2UIBattleMain::OnChangedGuradCoolTime(class UB2UIDocBase* Sender, float GuradCoolTime, float PrevGuradCoolTime)
{
	if (TB_GuradCoolTime.IsValid())
	{
		TB_GuradCoolTime->SetText(GetAutoFractionalFormattedText(GuradCoolTime, 1));
	}
}

void UB2UIBattleMain::OnChangedHasNotTagDefence(class UB2UIDocBase* Sender, bool HasNotTagDefence, bool PrevHasNotTagDefence)
{
	SetGuardUI(HasNotTagDefence);
}

void UB2UIBattleMain::SetGuardUI(bool HasNotTagDefence)
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
