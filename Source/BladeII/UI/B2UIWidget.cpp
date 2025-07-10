
#include "B2UIWidget.h"
#include "MovieScene.h"
#include "Animation/WidgetAnimation.h"
#include "B2UIManager.h"

UB2UIWidget::UB2UIWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bPendingPause = false;
	MyUIFName = NAME_None;
}

UB2UIWidget::~UB2UIWidget()
{
}

void UB2UIWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UB2UIWidget::Init()
{
	Super::Init();
}

void UB2UIWidget::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	// UB2UIManager::InternalCloseUI 에서 하는 짓이긴 한데 만에 하나 CloseUI 를 거치지 않고 직접 닫아준다면 UIManager 쪽의 레퍼런스가 남아있게 되므로 매개변수로 넘겨주도록 해서 제거해 줌.
	// 그러나 일부 UIWidget 의 경우 추가 처리를 하는 게 좀 있어서 여기서 UIManager 쪽 처리를 하기 곤란한 것도 있음. NULL 이면 무시하도록.
	FB2UIWidgetData* RelevantWidgetData = InUIManager ? InUIManager->GetWidgetDataByWidget(this) : NULL;
	if (RelevantWidgetData)
	{
		RelevantWidgetData->Widget = NULL;
	}
}

void UB2UIWidget::OnOpen(bool RightNow)
{
	if (RightNow)
	{
		//여기서 오픈 애님을 마지막에서 끊어 재생해준다

		OnOpenComplete();
	}
	else
	{
		OpenState = EUIOpenState::OnOpening;

		// GetAnimation 제대로 작동 안 함.
		//UWidgetAnimation* OpenCloseAnim = GetAnimation(TEXT("ANIM_OpenClose"), this);
		//if (OpenCloseAnim)
		//{
		//	//여기서 오픈 애님을 재생해준다.

		//	//현재 오픈 애님 관련 작업 안했으니 열린걸로
		//	OnOpenComplete();
		//}
		//else
			//애님이 없으면 열린걸로
			OnOpenComplete();
	}

	OnOpen_BP();
}

void UB2UIWidget::OnClose(bool RightNow)
{
	if (RightNow)
	{
		//여기서 클로즈 애님을 마지막에서 끊어 재생해준다

		OnCloseComplete();
	}
	else
	{
		OpenState = EUIOpenState::OnClosing;

		// GetAnimation 제대로 작동 안 함.
		//UWidgetAnimation* OpenCloseAnim = GetAnimation(TEXT("ANIM_OpenClose"), this);
		//if (OpenCloseAnim)
		//{
		//	//여기서 클로즈 애님을 재생해준다.

		//	//현재 클로즈 애님 관련 작업 안했으니 닫힌걸로
		//	OnCloseComplete();
		//}
		//else
			OnCloseComplete();
	}

	OnClose_BP();
}

void UB2UIWidget::OnSceneOpen(EUIScene InOpenedScene)
{
	// OnOpen 이나 Init 은 자신을 포함한 Scene 이 열리는 상황에서 Scene 의 다른 Widget 이 열렸는지 여부에 대한 보장은 없다.
	// 이것이 불리는 타이밍은 자신을 포함한 Scene 전체가 열린 이후임.
}

void UB2UIWidget::OnHistoryOpen(bool IsHistory)
{
	//뒤로가기로 들어왔을경우 불려진다
}

void UB2UIWidget::HandlePendingPause()
{
	if (bPendingPause)
	{
		UGameplayStatics::SetGamePaused(GetOwningPlayer(), true);
		bPendingPause = false;
	}
}

void UB2UIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HandlePendingPause();
}

void UB2UIWidget::CloseSelf()
{
	//UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	//if (pUIManager)
	//	pUIManager->CloseUI(MyUIFName);

	//// 매니져 안통하구 그냥 만들어 붙였을수도 있음.
	//if (MyUIFName == NAME_None)
	//{
	//	RemoveFromViewport();
	//}
}

void UB2UIWidget::OnOpenComplete()
{
	OpenState = EUIOpenState::Opened;

	UIWidgetOpenCompleteClass<FName>::GetInstance().Signal(this->GetMyUIFName());
}

void UB2UIWidget::OnCloseComplete()
{
	OpenState = EUIOpenState::Closed;
	UB2UIManager::GetInstance()->CloseUI(GetMyUIFName()); //나 연출 끝났어 닫아줘
}

void UB2UIWidget::SetPendingPause()
{
	bPendingPause = true;
}
