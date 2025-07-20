#include "B2UIBattleQTE.h"
#include "Event.h"
#include "B2UIDocHelper.h"
#include "B2UIChatting.h"
#include "B2UIManager.h"

UB2UIBattleQTE::UB2UIBattleQTE(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MountTimeMID = NULL;
	bPressedAnimPlaying = false;
	CachedEnableTimingState = EQTEUIAttackEnableTimingState::None;
	bIsQTEMounted = false;
	MaxQTEMountTime = 10.0f;
	bEventsSubscribed = false;
	StartPauseMenuTicket = -1;
	StopPauseMenuTicket = -1;
	bHiddenByPause = false;
	bHiddenByBattleMain = false;
	bHiddenByTutorialBattleMain = false;
}

void UB2UIBattleQTE::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UB2UIBattleQTE::Init()
{
	Super::Init();

	SubscribeEvents();
}

void UB2UIBattleQTE::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	UnsubscribeEvents();
}

void UB2UIBattleQTE::SubscribeEvents()
{
	if (bEventsSubscribed) {
		return;
	}

	CAPTURE_UOBJECT(UB2UIBattleQTE);
	
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

void UB2UIBattleQTE::UnsubscribeEvents()
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

void UB2UIBattleQTE::OnPause(bool bPaused)
{
	// 喊档狼 layer 俊 悼利 积己登绰 局扼 pause 登绢档 啊妨瘤瘤 臼酒辑 蝶肺 见辨 鞘夸啊 乐促.
	bHiddenByPause = bPaused;
	UpdateOverallVisibility();
}

void UB2UIBattleQTE::UpdateByQTEEnableState(EQTEUIAttackEnableTimingState InEnableTimingState)
{
	const EQTEUIAttackEnableTimingState PrevEnableState = CachedEnableTimingState;

	CachedEnableTimingState = InEnableTimingState;

	UpdateOverallVisibility(); // 蔼 某教秦 初绊 配呕 厚历呼府萍 诀单捞飘

	if (CachedEnableTimingState != PrevEnableState && CachedEnableTimingState != EQTEUIAttackEnableTimingState::None)
	{ // QTE 鸥捞怪 钎矫 局丛 敲饭捞
		TurnOffLoopAnims();
		PlayEnableAnim(CachedEnableTimingState);
	}
}

void UB2UIBattleQTE::SetVisibilityFromBattleMainUI(ESlateVisibility InVisibility)
{
	bHiddenByBattleMain = (InVisibility == ESlateVisibility::Hidden);

	SetVisibility(InVisibility);
}

void UB2UIBattleQTE::SetVisibilityFromTutorialBattleMainUI(ESlateVisibility InVisibility)
{
	SetVisibility(bHiddenByBattleMain ? ESlateVisibility::Hidden : InVisibility);
}

void UB2UIBattleQTE::CacheAssets()
{
	GET_SLOT(UImage, IMG_MountTimeProgress);
	GET_SLOT(UTextBlock, TB_MountTimeNum);
	GET_SLOT(UB2Button, BTN_Area);
	GET_SLOT(UB2Button, BTN_Area2);		
}

void UB2UIBattleQTE::UpdateOverallVisibility()
{
	if (bHiddenByPause || bHiddenByBattleMain || bHiddenByTutorialBattleMain)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else if (UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting))
	{	
		SetVisibility(ESlateVisibility::Hidden); // 盲泼 UI啊 畴免 登菌阑 版快 洒电 贸府.
	}
	else if (CachedEnableTimingState != EQTEUIAttackEnableTimingState::None || bPressedAnimPlaying)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible); // 郴何 滚瓢篮 Visible 捞绢具 茄促.
	}
	else if (bIsQTEMounted)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible); // 付款飘 惑怕俊辑档 焊捞变 窍绰单 努腐篮 救 登档废.
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIBattleQTE::BindDelegates()
{
	BIND_PRESS_FUNC_TO_BTN(BTN_Area2, &UB2UIBattleQTE::OnPressedBtnArea)
}

void UB2UIBattleQTE::BindDoc()
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		DocBattle->OnRemainingQTEMountTimeChanged.AddUObject(this, &UB2UIBattleQTE::OnChangedRemainingQTEMountTime);
	}
}
void UB2UIBattleQTE::UnbindDoc()
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		DocBattle->OnRemainingQTEMountTimeChanged.RemoveAll(this);
	}
}

void UB2UIBattleQTE::OnPressedBtnArea()
{
	if (bIsQTEMounted) {
		return; // 付款飘 惑怕俊辑档 焊捞霸 且 搏单 巢篮 矫埃捞尔.. 炼累 侩档绰 酒丛. 努腐 磊眉啊 酒付 救 瞪 巴.
	}

	QTEAttackClass<>::GetInstance().Signal();

	//QTE己傍沁阑锭父 柳青(滚瓢 局聪皋捞记)
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();

	if (DocBattle && !DocBattle->GetbIsPlayerInQTEState())
		return;

	if (!bPressedAnimPlaying)
	{
		TurnOffLoopAnims();
		PlayPressedAnim();
		SetIsPressedAnimPlaying(true); // Reset 篮 喉风橇赴飘辑
	}	
}

void UB2UIBattleQTE::OnChangedRemainingQTEMountTime(class UB2UIDocBase* Sender, float CurrRemainingTime, float PrevRemainingTime)
{
	if (bIsQTEMounted) {
		UpdateMountTimeProgress(CurrRemainingTime);
	}
}

void UB2UIBattleQTE::UpdateMountTimeProgress(float InRemainingTime)
{
	if (TB_MountTimeNum.IsValid())
	{
		TB_MountTimeNum->SetText(GetAutoFractionalFormattedText(InRemainingTime, 0)); // 老窜篮 家荐痢 绝捞..
	}

	if (MountTimeMID)
	{
		MountTimeMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, InRemainingTime / FMath::Max(MaxQTEMountTime, KINDA_SMALL_NUMBER));
	}
}

void UB2UIBattleQTE::SetIsPressedAnimPlaying(bool bPlaying)
{
	bPressedAnimPlaying = bPlaying;

	UpdateOverallVisibility();
}

void UB2UIBattleQTE::SetQTEMounted(bool bMounted, float InMaxMountTime, bool bVisibleTimeNum)
{
	bIsQTEMounted = bMounted;
	MaxQTEMountTime = InMaxMountTime;

	bHiddenByTutorialBattleMain = !bVisibleTimeNum;

	UpdateOverallVisibility();

	if (bIsQTEMounted)
	{
		// QTE mount time 墨款飘甫 焊咯临 巴甸阑 悸诀
		if (IMG_MountTimeProgress.IsValid())
		{
			IMG_MountTimeProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
			MountTimeMID = IMG_MountTimeProgress->GetDynamicMaterial();
		}
		if (TB_MountTimeNum.IsValid())
		{
			TB_MountTimeNum->SetVisibility(bVisibleTimeNum ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
		UpdateMountTimeProgress(MaxQTEMountTime); // 老窜 父顶栏肺 橇肺弊饭胶 技泼.		

		TurnOffLoopAnims();
	}
	else
	{
		if (IMG_MountTimeProgress.IsValid())
		{
			IMG_MountTimeProgress->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (TB_MountTimeNum.IsValid())
		{
			TB_MountTimeNum->SetVisibility(ESlateVisibility::Collapsed);
		}
		MountTimeMID = NULL;

		TurnOffLoopAnims();
	}
}