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
	// 반격 던전 / 일반 전투 UI 공용 Widget
	// 

	//버튼들
	ADD_WIDGET_SLOT(CommonWidgets, UButton, BTN_Tag);
	ADD_WIDGET_SLOT(CommonWidgets, UButton, BTN_Pause);
	ADD_WIDGET_SLOT(CommonWidgets, UButton, BTN_Pause2);
	ADD_WIDGET_SLOT(CommonWidgets, UButton, BTN_Attack);	
		
	//메인캐릭
	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_PortraitMain);
	ADD_WIDGET_SLOT(CommonWidgets, UImage, CRTInfoBGImage_01);
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_MainCharLevel);
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_MainCharName);
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_MainCharLevel);
	ADD_WIDGET_SLOT(CommonWidgets, UProgressBar, PB_MainCharHP);
	ADD_WIDGET_SLOT(CommonWidgets, UProgressBar, PB_MainCharShield);

	//서브캐릭
	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_PortraitSub);
	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_PortraitSubDisable);
	ADD_WIDGET_SLOT(CommonWidgets, UProgressBar, PB_SubCharHP);

	//쿨타임
	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_TagCooltime);
	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_TagCooltimeBottom);
	ADD_WIDGET_SLOT(CommonWidgets, UTextBlock, TB_TagCooltimeNumBottom);
	if (IMG_TagCooltime.IsValid())
		TagCooltimeDisplayMID = IMG_TagCooltime->GetDynamicMaterial();

	if (IMG_TagCooltimeBottom.IsValid())
		TagCooltimeDisplayMIDBottom = IMG_TagCooltimeBottom->GetDynamicMaterial();

	ADD_WIDGET_SLOT(CommonWidgets, UImage, IMG_FX_TagSuggest);
	if (IMG_FX_TagSuggest.IsValid())
	{ // 숨겨놓고 시작.
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

	//배틀콤보
	ADD_WIDGET_SLOT(CommonWidgets, UB2UIBattleCombo, UIP_BattleCombo);

	//비넥트
	ADD_WIDGET_SLOT(CommonWidgets, UB2UIBattleVignette, UIP_BattleVignette);
	
	//핫타임 아이콘
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
	// 일반 전투에만 쓰이는 UI Widget
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
	// 반격 던전에만 쓰이는 UI Widget
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
	// Mount 공격 때 숨겨져야하는 위젯들.
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
	
#if !UE_BUILD_SHIPPING // 개발 테스트용
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
		// 별 3개 받기 위한 조건 텍스트
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
	{ // 얘들은 버튼 목적상 Hovered 도 해 보자..
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
//	//스킬
//	for (uint8 Cnt = 0; Cnt < MAX_SKILL_INPUT; Cnt++)
//	{
//		FString SkillAssetName = PrefixSkillAsset + FString::FromInt(Cnt);
//		auto SkillAsset = GetSlot<UB2UIBattleSkill>(FName(*SkillAssetName));
//		if (SkillAsset)
//		{
//			SkillAssets.Add(SkillAsset);
//			SkillAsset->Init();
//			SkillAsset->SetIndex(Cnt);	//SkillRT쪽에서는 1베이스로 작업한 듯
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
//	//시작 할때, Main과 Sub의 클래스를 세팅하고 CurPCClass나 TagPCClass의 값이 바뀔때 두개를 스왑해준다.
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
//		//// StageBuff 는 스테이지 진행 도중 바뀌지 않음. 처음에만 생성하면 될 것.
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
//	// 부활버프는 생성만 해 둠. 일단 표시 안 될 것.
//	CreateOrUpdateResurrectBuffIcon(EResurrectBuffType::RBT_None);
//
//	// 태그 쿨타임 초기화
//	SetTagCooltime(0);
//
//	//길드 버프 등록
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
//	// 추후에 추가될 경우 이렇게 분기하지말고, 상속해서 만듭시다요~ 조만간 없애야지.. (참고 RaidBattle.h)
//	if (CurrentGameModeType == EB2GameMode::CounterDungeon)
//		SetWidgetForCounterAttackBattle();
//	else
//		SetWidgetForNormalBattle();
//
//	DocBattle->SetMaxCounterAttackPoint(MAX_COUNTER_ATTACK_POINT);
}

void UB2UIBattleMain::BindDocMainChar(UB2UIDocHero* InDocHero)
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

		if (SkillDoc && CurrentGameModeType == EB2GameMode::CounterDungeon)
		{
			SkillDoc->OnIsCountingCooltimeChanged.RemoveAll(SkillAsset);
			SkillDoc->OnIsDisabledByCooltimeChanged.RemoveAll(SkillAsset);
			SkillDoc->OnRemainingCooltimeChanged.RemoveAll(SkillAsset);
		}
	}

	FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	EPCClass pcClass = InDocHero->GetHeroClass();

	//초상화 처리
	SetPortraitMainHero(UB2UIDocHero::GetPlayerPortraitMaterial(this, pcClass));

	SetNameMainChar(ClientDataStore.GetLocalCharClassOrNickName(pcClass));

	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
	//SetLevelMainChar(InDocHero->GetCurrentLevel());
	SetLevelMainChar(ClientDataStore.GetLocalCharacterData().GetCharacterLevel(pcClass));
}

void UB2UIBattleMain::BindDocSubChar(UB2UIDocHero* InDocHero)
{
	// 로비에서의 서브 캐릭터 선택이 아니라 현재 대기중인 캐릭터. 게임 시작 처음에는 실제 서브 캐릭터임.
	SubHeroDoc = InDocHero;

	if (!InDocHero) 
		return;

	//초상화 처리.
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
	Tag_BP(MainClass, SubClass, bIsTagAttack); // B2UIBattleTagAnim 에서 태그 애니메이션 관련한 모든 걸 처리하지는 않는다.

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
	// 여기 가능한 Lv 텍스트도 다국어로 하는 게 좋다. 현재는 블루프린트에 박혀 있음
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
		// 스킬쿨타임처럼 첫 소수점을 카운트하는 동안에는 소수점 첫째자리까지 표시하는데 여기는 초반에 애니메이션으로 인해 쿨타임 표시 자체가 안되어서 딱히 눈에는 안 띌 듯.. 그래도 태그 쿨타임 감소 옵션이 있으니 기능은 넣어둔다.
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
		HUDInfo->ApplyAttackBtnMargin(BtnStyle, GetWorld()); // 게임모드에 따라 다른 세팅을 할 수 있다.
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
	// 스테이지 종료 시 보게 될 정보와 다른 경우를 최소화하기 위해 이건 매 틱 업데이트를 하는 게 좋을 수도.. 여하간 충분히 자주 업데이트 되긴 할 거. 아무리 늦어도 1초에 한번.
	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	const FServerStageID CurrentServerStageId = DocBS ? DocBS->GetServerStageId() : FServerStageID(1);
	const int32 ExpectedGrade = StageDataStore.GetStageExpectedClearStarGrade(CurrentServerStageId, InCurrentTime);
	// 몇초 후의 상황을 미리 예측.
	const int32 PreExpectedGrade = StageDataStore.GetStageExpectedClearStarGrade(CurrentServerStageId, InCurrentTime + StageClearGradePreDropAnimTime);
	// 시간으로 Grade를 하락시키는 경우가 한가지가 되면서 3성->2성 제한시간은 사용되데 2성->1성 변화 제한시간은 사용되지 않는다.
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
	// 처음엔 안 그랬는데 새로 배치하고 하다보니 의도한 숫자랑 grade 가 어긋남 ㅡ0ㅡ
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
		IMG_StageClearGradeStarTwinkle_03->SetVisibility(ExpectedGrade >= STAGE_CLEAR_GRADE_STAR_1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden); // 사실상 grade 가 1보다 작을 일은 없을 듯.
	}
	
	if (bThisTimePreExpectedGradeDifferent && !bLastPreExpectedGradeDifferent)
	{ // 몇초 후에 등급이 낮아질 걸로 예측되기 시작한 상황.
		PlayStageClearGradePreDropAnim(PreExpectedGrade);
	}

	bLastPreExpectedGradeDifferent = bThisTimePreExpectedGradeDifferent;
}

void UB2UIBattleMain::SetVisibilityClearGradeCondition(bool InShow)
{
	// 보여지는 텍스트 자체는 static 이라 다른 곳에서 세팅.

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
		// Pause 메뉴 widget 생성. 여기서 post process DOF 설정을 하므로 이것이 적용되도록 하기 위해 실제 pause 는 한 틱 늦춘다.
		StartPauseMenuClass<EB2GameMode>::GetInstance().Signal(GetB2GameModeType(this)); // UIManager_InGameMenu 쪽에 등록한 StartPauseMenu 를 콜하게 될 것.
		SetPendingPause();
		//bPendingPause = true;

		PlayPauseBtnSound_BP();
	}
	else
	{
		// 사실 Pause 상태에서는 다시 여기의 PuaseButton 을 누르는 것이 아니라 Pause 메뉴에서 Continue 버튼을 눌러야 돌아가게 될 것.
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
			//씹어야 되는 케이스
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
	SetTagCooltimeVisible(IsTagDisabledByCooltime); // 당장 카운트는 안 하지만 여기서부터 보여주기 시작.
}

void UB2UIBattleMain::OnChangedRemainingTagCooltime(class UB2UIDocBase* Sender, float RemainingTagCooltime, float PrevRemainingTagCooltime)
{
	SetTagCooltime(RemainingTagCooltime);
}

void UB2UIBattleMain::OnChangedIsDefenseEther(class UB2UIDocBase* Sender, bool IsDefenseEther, bool PrevIsDefenseEther)
{
	/*if (UIP_DefenseEtherIcon.IsValid())
	{
		if (IsDefenseEther)
		{
			if(UIP_DefenseEtherIcon->GetVisibility() == ESlateVisibility::Collapsed)
				UIP_DefenseEtherIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UIP_DefenseEtherIcon->Init();
		}
		else
		{
			if(UIP_DefenseEtherIcon->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
				UIP_DefenseEtherIcon->SetVisibility(ESlateVisibility::Collapsed);
		}
	}*/
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
		if(!PrevbCurrentlyBossAppeared)  //혹시 중복해서 들어 올 수 있으니 한번 필터
		{
			UB2UIManager::GetInstance()->OpenUI(UIFName::BossStatus);
		}
		else
		{ // 좀 복잡한 사정으로 UB2UIBossStatus 쪽에서 BossMobClass 와 BossMobClassVariation 변경에 대한 처리를 하지 못해서 여기서 대신..
			// 물론 CurrentlyBossAppeared 세팅하기 전에 BossMobClass 와 BossMobClassVariation 을 제대로 세팅해 놓아야 한다.
			// BossStatus 열리기 전에 언제나 BossMobClass 와 Variation 이 적절히 세팅이 된다면 여기서 안 해줘도 됨. 보스가 연달아 등장할 때 꼬일까봐 안전장치로 해 두는 거.
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
	const bool bIsForTagAttack = DocBattle ? DocBattle->GetIsTagAttackSuggested() : false; // 이게 아니면 플레이어 컨디션 (HP) 에 의한 것. IsTagAttackSuggested 를 여기서 체크할 수 있도록 IsTagSuggested 보다 먼저 세팅해야 함.

	if (bTagSuggested != bPrevTagSuggested)
	{
		if (IMG_FX_TagSuggest.IsValid())
		{
			IMG_FX_TagSuggest->SetVisibility((bTagSuggested && !bIsForTagAttack) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
		if (CP_TagSuggest_TagCharChallenge.IsValid())
		{
			//스킬이든 컨디션이든 모두 SuggestText 활성화
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
			StopTagSuggestByPlayerConditionAnim_BP(); // 여기에 HP 로 인한 태그권장 상태임을 명시하는 조건이 있다면 얘기가 다르긴 한데..
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
	// 반격 던전일때는 무시
	if (CurrentGameModeType == EB2GameMode::CounterDungeon 
		|| CurrentGameModeType == EB2GameMode::Tutorial 
		|| CurrentGameModeType == EB2GameMode::Control)
		return;

	// TODO : 케릭터 버프쪽으로 프리징이랑 스턴 이식할땐 OnChangedStateDamangeSkillBuffState 함수안에 인풋 막는부분 갖고와야함.
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
		// Freezing/Stun 상태에서 벗어나는 목적인 SurviveSkill은 Disable 시키지 않음
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
	// 일단 찾기부터.
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
	// 부활 버프는 하나만 표시될 거기 땜에 기존에 있는 걸 찾아서 업데이트 하든지 새로 생성
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
					if (ThisPrevBuff.MyType == ThisCurrBuff.MyType){
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

void UB2UIBattleMain::UpdateSkillBuffIconsByCollectedData(bool bUpdateCountTimeOnly)
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

void UB2UIBattleMain::UpdateSkillBuffIcons()
{
	bool bOnlyCountTimeChanged = false;
	CollectAndSortSkillBuffData(bOnlyCountTimeChanged);
	UpdateSkillBuffIconsByCollectedData(bOnlyCountTimeChanged);
}

void UB2UIBattleMain::BeginTagAnim(EPCClass NewTaggedClass, bool bIsTagAttack)
{
	DestroyTagAnimUIP(false); // 정상적인 상황이면 이전에 플레이하던 게 종료되고 생성했던 건 콜백받아 이미 destroy 되어 있어야.. 그러나 destroy 이전에 태그를 또 했을 수도.

	if (!bIsTagAttack) // 연계 태그는 이거 안 함.
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
{ 	// 여기서 동적 로딩하는 UIP 들을 미리 백그라운드 로딩해 놓아서 전투 중 hitch 를 최소화 하기 위함.
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
	// HPValue 와 ShieldValue 는 MaxHPValue 에 대한 비율이 아닌 절대값이어야 함.

	float FinalShieldAmount = InShieldPB ? ShieldValue : 0.0f;// 여기로 들어오는 값에 따라 다른 식 (MaxHPValue * ShieldValue) 을 사용하게 될 수도.

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
			//SWidget이 Focus를 가져간채 Disable되는 경우, 다른 Widget 및 GameViewport에서 Focus를 못 가져가는 현상이 발생하여
			//강제적으로 MouseUp을 불러 Focus를 잃게 한다.
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
{// UB2UIBattleTagAnim 쪽으로부터 오는 신호. 메모리 절약을 위해 다 되었으면 destroy.. 하려고 했는데 이젠 hitch 제어가 더 중요해서 완전히 내리지는 않는다.
	checkSlow(ExpiredUIP && ExpiredUIP == CreatedTagAnimUIP);
	DestroyTagAnimUIP(false);
}

void UB2UIBattleMain::SetQTEEnable(EQTEUIAttackEnableTimingState InEnableState)
{ // 대체로 QTE 입력 가능 타이밍을 알리는 거고 QTE 상태는 아님.
	// 도중에 QTE UIP 를 제거할 수 있도록 현재 QTE 상태를 여기로 보내는 게 좋을 듯..

	if (InEnableState != EQTEUIAttackEnableTimingState::None) { // None 일 때 매 프레임 들어오는 거 같아서 여기는 Enable 되었을 때만 체크. 꺼질 때는 다른 경로로 들어옴.
		CheckForQTEStateUISetup(); // 관련 UI 생성 및 visibility 셋업부터.
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
		// 기본적인 QTE UIP 보이는 조건은 폭넓게.
		const bool bQTEUIPMustbeVisible = (DocBattle->GetQTEEnableState() != EQTEUIAttackEnableTimingState::None || DocBattle->GetbIsQTEMounted() || DocBattle->GetbIsPlayerInQTEState());
		
		// 필요할 때만 생성해서 사용하도록.
		if (bQTEUIPMustbeVisible){
			ConditionalCreateQTEUIP();
		}
		else{
			DestroyQTEUIP(false); // 맨 마지막 아니면 TAsset 은 내리지 않음.
		}

		// 대체로 연출과 밀접한 QTE 상태 도중에는 QTE 외의 다른 UI 들을 숨긴다. 첫 QTE 타이밍은 아직 실제 QTE 는 아니니 여기서 숨기면 당황스러울 꺼고 마운트에서 공격이 가능한 상태는 전투 조작을 해야하니 UI 가 필요하고.
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
	// PlayerController 의 CinematicMode 를 사용하려니 좀 꼬일 거 같음. 그냥 따로 처리.
	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetOwningPlayer());

	if (bHide)
	{
		// 거의 요 BattleMain 만 숨기는 정도..
		// QTE UIP 는 BattleMain 의 child 가 아닌 독자적인 UI widget 으로 생성되므로 여기에 영향을 받지 않는다. 정말로 숨겨야 할 때가 되어도 나올 꺼란 얘긴데 그게 문제가 되는 상황이 발생하면 따로 처리를 해야 할 듯..
		// -> 이제 같이 숨긴다.
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
		if (B2PC && !B2PC->IsInCinematicMode()) // 여기선 CinematicMode 진입을 하지 않으므로 혹시라도 CinematicMode 라면 다른 곳에서 처리할 테니 놔둠
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
	// 이미 생성되어 있으면 굳이 새로 만들지 않는다. 
	if (!CreatedQTEUIP)
	{
		CreatedQTEUIP = LoadAndBeginTAssetQTEUIP<UB2UIBattleMain>(this, QTEUIPClass, &LoadedQTEUIPClass);
		if (CreatedQTEUIP){

			CreatedQTEUIP->BindDoc(); // 일부러 여기서 콜한다. 바로 아래 Destroy 에서의 UnbindDoc 하고 짝맞춰 놓기 위해.
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
{ // ToSet 이 true 라 해도 당장 마운트 공격 입력까지 가능한 상태는 아닐 수 있다.
	if (CP_Attack.IsValid())
		CP_Attack->SetVisibility(ToSet ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	if (CP_MountAttack.IsValid())
		CP_MountAttack->SetVisibility(ToSet ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	CheckForQTEStateUISetup(); // 관련 UI 생성 및 visibility 셋업부터.

	if (CreatedQTEUIP)
	{ // 마운트 내내 켜져 있어야 함. 단, 클릭은 안 되게. 내부에서 조절..
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
	// CounterSuggestRingTime 은 CounterBattleMain의 Construct에서 SuggetRing 애니메이션의 길이 값으로 세팅.
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

		if (bIsForTagAttack) //공격 스킬 태그
			TB_TagSuggest_TagCharChallenge->SetText(CharacterTagDialogueInfo->GetRandomTagSuggestTextFromSkill(iSubClass));
		else //빈사 상태 태그
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
	//			ThisComp->SetLODLevel(0); // 여기 넣어주는 값은 의미없고 걍 업데이트 차원..
	//		}
	//		else {
	//			if (ThisComp->GetWorld()) {
	//				ThisComp->RestoreLODLevelToDesiredValue(); // 이러면 원래 예상되는 값으로 돌아가려나
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
//		GB2CharFxCheatLODLevel_PC = -1; //0 이 아니라 -1 로 돌아감. 기본 LOD 기능의 의미로
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
//		GB2CharFxCheatLODLevel_Mob = -1; //0 이 아니라 -1 로 돌아감. 기본 LOD 기능의 의미로
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
//		GB2CharFxCheatLODLevel_Boss = -1; //0 이 아니라 -1 로 돌아감. 기본 LOD 기능의 의미로
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
				(InCheatLODLevel < 0 ? FLinearColor(0.5f,0.5f,0.5f) : // 기본 LOD 기능을 사용하는 상황에서 회색
					FLinearColor(1.0f,0.5f,0.0f) ) // 이건 1 ~ Max 사이 중간 LOD 단계가 있는 경우.. 주황색 표시.
	)));
}

void AdjustDevFxLODTestBtnStyleCommon(UB2Button* InBtn, int32 CurrCheatLODLevel, int32 MaxCheatLODLevel)
{
	if (InBtn)
	{
		FButtonStyle AdjustStyle = InBtn->GetStyle();
		AdjustStyle.Normal.TintColor = GetDevFxLODTestBtnColor(CurrCheatLODLevel, MaxCheatLODLevel);
		AdjustStyle.Hovered.TintColor = AdjustStyle.Normal.TintColor; // Normal 만 변경해서는 충분하지가 않군. ㅡㅡ
		AdjustStyle.Pressed.TintColor = AdjustStyle.Hovered.TintColor;
		InBtn->SetStyle(AdjustStyle);
	}
}

void UB2UIBattleMain::UpdateDevFxLODWidgets() // 토탈 업데이트
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
