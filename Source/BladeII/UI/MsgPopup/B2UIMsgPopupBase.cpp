#include "BladeII.h"
#include "B2UIMsgPopupBase.h"
#include "B2UIManager.h"


UB2UIMsgPopupBase::UB2UIMsgPopupBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bModalWindow= false;
	MsgPopupType = EUIMsgPopup::Simple;
	AutoCloseTime = 0.f;
	RemainAutoCloseTime = 0.f;
	PopupPriority = EPopUpPriority::Base;
}

void UB2UIMsgPopupBase::CacheAssets()
{
	GET_SLOT(UButton, BTN_Modal)
	GET_SLOT(UTextBlock, TB_Title)
	GET_SLOT(UB2RichTextBlock,TB_Content)
}

void UB2UIMsgPopupBase::UpdateStaticText()
{
}

void UB2UIMsgPopupBase::BindDelegates()
{
}

void UB2UIMsgPopupBase::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(NULL); // ������ ��Ȯ�� ������ �� �𸣰����� �Ʒ����� UIManager �� ó���� �� �ϹǷ� ���⼱ NULL �� �Ѱ��ش�..

	check(InUIManager);
	if (InUIManager->IsMsgPopupOpened(GetMsgID()))
		InUIManager->CloseMsgPopup(GetMsgID());
}

void UB2UIMsgPopupBase::NativeAutoCloseTimeCallback(float InRemainAutoCloseTime)
{
}

void UB2UIMsgPopupBase::SetTitle(const FText& InText)
{
	if (TB_Title.IsValid())
		TB_Title->SetText(InText);
}

void UB2UIMsgPopupBase::SetContent(const FText& InText)
{
	if (TB_Content.IsValid())
		TB_Content->SetText(InText);
}

void UB2UIMsgPopupBase::SetClickButtonState(bool ButtonType)
{
	ButtonState = ButtonType;
}

bool UB2UIMsgPopupBase::GetClickButtonState()
{
	return ButtonState;
}

void UB2UIMsgPopupBase::ShowModalArea(bool bShow)
{
	if (!BTN_Modal.IsValid())
		return;

	BTN_Modal->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UB2UIMsgPopupBase::SetModalWindow(bool bModal)
{
	bModalWindow = bModal;

	//���� ������ �׻� � ��޿��� �����ش�
	ShowModalArea(bModalWindow);
}

void UB2UIMsgPopupBase::SetAutoCloseTime(float InAutoCloseTime)
{
	if (InAutoCloseTime > 0.f)
	{
		AutoCloseTime = InAutoCloseTime;
		RemainAutoCloseTime = InAutoCloseTime;
	}
}

void UB2UIMsgPopupBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//�ܺο��� ���õ� ���� �־�� �ڵ� �ݱⰡ Ȱ��ȭ �ȴ�.
	if (AutoCloseTime > 0.f)
	{
		RemainAutoCloseTime -= InDeltaTime;
		NativeAutoCloseTimeCallback(RemainAutoCloseTime);

		if (RemainAutoCloseTime < 0.f)
		{
			AutoCloseTime = 0.f;
			RemainAutoCloseTime = 0.f;
			DestroySelf(UB2UIManager::GetInstance());
		}
	}
}