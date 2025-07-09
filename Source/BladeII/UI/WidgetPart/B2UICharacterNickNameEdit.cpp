#include "BladeII.h"
#include "B2UICharacterNickNameEdit.h"
#include "B2UIManager.h"
#include "BladeIIGameMode.h"
#include "Retailer.h"
#include "B2ClientDataStore.h"


#include "BladeIIGameImpl.h"

UB2UICharacterNickNameEdit::UB2UICharacterNickNameEdit(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EditingPCClass = EPCClass::EPC_End;
	bClampedEditNickNamePending = false;
}

void UB2UICharacterNickNameEdit::Init()
{
	Super::Init();		
}

void UB2UICharacterNickNameEdit::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// ���� ���� �Ѿ�� �߸� �ؽ�Ʈ ���⼭ ����.
	if (bClampedEditNickNamePending && ETB_NickName.IsValid())
	{
		ETB_NickName->SetText(PendingClampedEditNickName);
		bClampedEditNickNamePending = false;
	}
}

void UB2UICharacterNickNameEdit::BeginEditPopup(EPCClass InEditClass)
{
	this->SetVisibility(ESlateVisibility::Visible);

	EditingPCClass = InEditClass;

	if (ETB_NickName.IsValid())
	{
		ETB_NickName->SetText(FText::FromString(TEXT(""))); // ����.
	}
}

void UB2UICharacterNickNameEdit::CacheAssets()
{
	GET_SLOT(UEditableText, ETB_NickName)
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UB2RichTextBlock, RTB_Desc);
	GET_SLOT(UTextBlock, TB_EditConfirmBtn);
	GET_SLOT(UTextBlock, TB_CancelBtn);
	GET_SLOT(UB2Button, BTN_EditConfirm);
	GET_SLOT(UB2Button, BTN_Cancel);
}

void UB2UICharacterNickNameEdit::UpdateStaticText()
{
	if (TB_Title.IsValid()){
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterNickNameEditTitle")));
	}
	if (RTB_Desc.IsValid()){
		RTB_Desc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterNickNameEditDesc")));
	}
	if (TB_EditConfirmBtn.IsValid()){
		TB_EditConfirmBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Change")));
	}
	if (TB_CancelBtn.IsValid()){
		TB_CancelBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Cancel")));
	}
	if (ETB_NickName.IsValid()){
		ETB_NickName->SetHintText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterNickNameEditHint")));
	}
}

void UB2UICharacterNickNameEdit::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_EditConfirm, &UB2UICharacterNickNameEdit::OnClickEditConfirmBtn)
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel, &UB2UICharacterNickNameEdit::OnClickCancelBtn)

	if (ETB_NickName.IsValid())
	{
		ETB_NickName->OnTextChanged.RemoveDynamic(this, &UB2UICharacterNickNameEdit::OnNickNameEditChanged);
		ETB_NickName->OnTextChanged.AddDynamic(this, &UB2UICharacterNickNameEdit::OnNickNameEditChanged);
	}
}

bool UB2UICharacterNickNameEdit::CheckForProhibitedNickName(const FText& InNickName)
{
	// TODO : ��Ģ�� ��� ä�ð� �Բ� ���� ����..

	return true;
}

bool UB2UICharacterNickNameEdit::CheckForWrongNickName(const FText& InNickName)
{
	FString TextString = InNickName.ToString();
	if (TextString.Len() < B2_CHAR_NICKNAME_MIN_LENGTH || TextString.Len() > B2_CHAR_NICKNAME_MAX_LENGTH) // ���� �Ѿ�� �� ���� �⺻������ ����Ʈ �� ó���� �ϱ� ��.
	{
		return false;
	}

	// ��Ÿ?

	return true;
}

bool UB2UICharacterNickNameEdit::CheckForDuplicatedNickName(const FText& InNickName)
{
	// �񱳴� �ϴ� ����ϰ� FString ���� �� ����.. FText �� ���ϴٰ� ��ĩ �ٱ��� �����ؼ� �߸� �������� �𸣰ڴ�.
	const FString InNickNameStr = InNickName.ToString();

	// CharacterDataStore �� �����ؼ� �ٸ� ĳ���͵��� NickName �� �����ͼ� ��
	FClientDataStore& CliDS = BladeIIGameImpl::GetClientDataStore();
	for (int8 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		EPCClass ThisPCClass = IntToPCClass(PCI);
		if (ThisPCClass == EditingPCClass){
			continue;
		}
		// Nick �� �ƴ� ClassName �� ������ ���� �ִµ� ���ϰ� �װŶ� �浹�� ���� �������.
		const FString ThisClassOrNickName = CliDS.GetLocalCharClassOrNickName(ThisPCClass).ToString();
		const FString ThisClassName = CliDS.GetLocalCharClassName(ThisPCClass).ToString(); // Ŭ�����̸��� �� ���� ������.
		if (InNickNameStr == ThisClassOrNickName || InNickNameStr == ThisClassName){
			return false;
		}
	}

	return true;
}

void UB2UICharacterNickNameEdit::OnNickNameEditChanged(const FText& InText)
{
	if (InText.ToString().Len() > B2_CHAR_NICKNAME_MAX_LENGTH)
	{
		// ���� ���� �Ѿ�� �ڸ��µ� �ڸ� �� �ٷ� �����ϸ� �ѱۿ��� ũ������ ���� �̷��� �ڸ� �� ������ �ξ��ٰ� Tick ���� ����
		// �׸��� ���⼱ �ϴ� ����־� ��.

		PendingClampedEditNickName = ClampTextLength(InText, B2_CHAR_NICKNAME_MAX_LENGTH);
		bClampedEditNickNamePending = true;

		if (ETB_NickName.IsValid())
		{
			ETB_NickName->SetText(FText::FromString(TEXT("")));
		}
	}
}

void UB2UICharacterNickNameEdit::OnClickEditConfirmBtn()
{
	const FText EdittedNickName = ETB_NickName.IsValid() ? ETB_NickName->GetText() : FText::FromString(TEXT("")); // �� ���ڿ��̸� WrongNickName �� �ɸ�����..
	bool bPopupWarnMsg = false;
	FText WarnMsgText;

	// ��Ģ�� ��߳���, �ٸ� �г��Ӱ� �ߺ����� �ʴ��� üũ�ؼ� �˾�.
	if (!CheckForProhibitedNickName(EdittedNickName))
	{
		bPopupWarnMsg = true;
		WarnMsgText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterNickNameEditProhibitedName"));
	}
	else if (!CheckForWrongNickName(EdittedNickName))
	{
		bPopupWarnMsg = true;
		WarnMsgText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterNickNameEditWrongName"));
	}
	else if (!CheckForDuplicatedNickName(EdittedNickName))
	{
		bPopupWarnMsg = true;
		WarnMsgText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterNickNameEditConflict"));
	}
	else
	{
		if (EditingPCClass != EPCClass::EPC_End)
		{
			BladeIIGameImpl::GetClientDataStore().RequestSetCharNickName(EditingPCClass, EdittedNickName);
		}
		//CloseMe(); ���⼭ �� �ݰ� ������ ���� �ݴ´�. �������� ���� �� �����ϰ� �߸��� �̸��� �����ϹǷ�. (HandleServerError1012)
	}
	
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (bPopupWarnMsg && UIMgrInst)
	{
		UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			WarnMsgText,
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
			);
	}
}

void UB2UICharacterNickNameEdit::OnClickCancelBtn()
{
	CloseMe();
}

void UB2UICharacterNickNameEdit::CloseMe()
{
	if (ETB_NickName.IsValid())
	{
		ETB_NickName->SetText(FText::FromString(TEXT(""))); // ����.
	}

	this->SetVisibility(ESlateVisibility::Collapsed);
}
