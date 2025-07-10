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
	// 별도의 layer 에 동적 생성되는 애라 pause 되어도 가려지지 않아서 따로 숨길 필요가 있다.
	bHiddenByPause = bPaused;
	UpdateOverallVisibility();
}

void UB2UIBattleQTE::UpdateByQTEEnableState(EQTEUIAttackEnableTimingState InEnableTimingState)
{
	const EQTEUIAttackEnableTimingState PrevEnableState = CachedEnableTimingState;

	CachedEnableTimingState = InEnableTimingState;

	UpdateOverallVisibility(); // 값 캐싱해 놓고 토탈 비저빌리티 업데이트

	if (CachedEnableTimingState != PrevEnableState && CachedEnableTimingState != EQTEUIAttackEnableTimingState::None)
	{ // QTE 타이밍 표시 애님 플레이
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
		SetVisibility(ESlateVisibility::Hidden); // 채팅 UI가 노출 되었을 경우 히든 처리.
	}
	else if (CachedEnableTimingState != EQTEUIAttackEnableTimingState::None || bPressedAnimPlaying)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible); // 내부 버튼은 Visible 이어야 한다.
	}
	else if (bIsQTEMounted)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible); // 마운트 상태에서도 보이긴 하는데 클릭은 안 되도록.
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
		return; // 마운트 상태에서도 보이게 할 껀데 남은 시간이랑.. 조작 용도는 아님. 클릭 자체가 아마 안 될 것.
	}

	QTEAttackClass<>::GetInstance().Signal();

	//QTE성공했을때만 진행(버튼 애니메이션)
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();

	if (DocBattle && !DocBattle->GetbIsPlayerInQTEState())
		return;

	if (!bPressedAnimPlaying)
	{
		TurnOffLoopAnims();
		PlayPressedAnim();
		SetIsPressedAnimPlaying(true); // Reset 은 블루프린트서
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
		TB_MountTimeNum->SetText(GetAutoFractionalFormattedText(InRemainingTime, 0)); // 일단은 소수점 없이..
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
		// QTE mount time 카운트를 보여줄 것들을 셋업
		if (IMG_MountTimeProgress.IsValid())
		{
			IMG_MountTimeProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
			MountTimeMID = IMG_MountTimeProgress->GetDynamicMaterial();
		}
		if (TB_MountTimeNum.IsValid())
		{
			TB_MountTimeNum->SetVisibility(bVisibleTimeNum ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
		UpdateMountTimeProgress(MaxQTEMountTime); // 일단 만땅으로 프로그레스 세팅.		

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