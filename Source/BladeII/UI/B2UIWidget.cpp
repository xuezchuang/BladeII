
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

	// UB2UIManager::InternalCloseUI ���� �ϴ� ���̱� �ѵ� ���� �ϳ� CloseUI �� ��ġ�� �ʰ� ���� �ݾ��شٸ� UIManager ���� ���۷����� �����ְ� �ǹǷ� �Ű������� �Ѱ��ֵ��� �ؼ� ������ ��.
	// �׷��� �Ϻ� UIWidget �� ��� �߰� ó���� �ϴ� �� �� �־ ���⼭ UIManager �� ó���� �ϱ� ����� �͵� ����. NULL �̸� �����ϵ���.
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
		//���⼭ ���� �ִ��� ���������� ���� ������ش�

		OnOpenComplete();
	}
	else
	{
		OpenState = EUIOpenState::OnOpening;

		// GetAnimation ����� �۵� �� ��.
		//UWidgetAnimation* OpenCloseAnim = GetAnimation(TEXT("ANIM_OpenClose"), this);
		//if (OpenCloseAnim)
		//{
		//	//���⼭ ���� �ִ��� ������ش�.

		//	//���� ���� �ִ� ���� �۾� �������� �����ɷ�
		//	OnOpenComplete();
		//}
		//else
			//�ִ��� ������ �����ɷ�
			OnOpenComplete();
	}

	OnOpen_BP();
}

void UB2UIWidget::OnClose(bool RightNow)
{
	if (RightNow)
	{
		//���⼭ Ŭ���� �ִ��� ���������� ���� ������ش�

		OnCloseComplete();
	}
	else
	{
		OpenState = EUIOpenState::OnClosing;

		// GetAnimation ����� �۵� �� ��.
		//UWidgetAnimation* OpenCloseAnim = GetAnimation(TEXT("ANIM_OpenClose"), this);
		//if (OpenCloseAnim)
		//{
		//	//���⼭ Ŭ���� �ִ��� ������ش�.

		//	//���� Ŭ���� �ִ� ���� �۾� �������� �����ɷ�
		//	OnCloseComplete();
		//}
		//else
			OnCloseComplete();
	}

	OnClose_BP();
}

void UB2UIWidget::OnSceneOpen(EUIScene InOpenedScene)
{
	// OnOpen �̳� Init �� �ڽ��� ������ Scene �� ������ ��Ȳ���� Scene �� �ٸ� Widget �� ���ȴ��� ���ο� ���� ������ ����.
	// �̰��� �Ҹ��� Ÿ�̹��� �ڽ��� ������ Scene ��ü�� ���� ������.
}

void UB2UIWidget::OnHistoryOpen(bool IsHistory)
{
	//�ڷΰ���� ��������� �ҷ�����
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

	//// �Ŵ��� �����ϱ� �׳� ����� �ٿ������� ����.
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
	UB2UIManager::GetInstance()->CloseUI(GetMyUIFName()); //�� ���� ������ �ݾ���
}

void UB2UIWidget::SetPendingPause()
{
	bPendingPause = true;
}
