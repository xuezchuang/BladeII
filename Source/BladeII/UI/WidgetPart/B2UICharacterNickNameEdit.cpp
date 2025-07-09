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

	// 길이 제한 넘어가서 잘린 텍스트 여기서 세팅.
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
		ETB_NickName->SetText(FText::FromString(TEXT(""))); // 비운다.
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
	// TODO : 금칙어 기능 채팅과 함께 추후 구현..

	return true;
}

bool UB2UICharacterNickNameEdit::CheckForWrongNickName(const FText& InNickName)
{
	FString TextString = InNickName.ToString();
	if (TextString.Len() < B2_CHAR_NICKNAME_MIN_LENGTH || TextString.Len() > B2_CHAR_NICKNAME_MAX_LENGTH) // 길이 넘어가는 건 물론 기본적으로 에디트 시 처리를 하긴 함.
	{
		return false;
	}

	// 기타?

	return true;
}

bool UB2UICharacterNickNameEdit::CheckForDuplicatedNickName(const FText& InNickName)
{
	// 비교는 일단 깔끔하게 FString 으로 해 보자.. FText 로 비교하다가 자칫 다국어 관련해서 잘못 꼬일지도 모르겠다.
	const FString InNickNameStr = InNickName.ToString();

	// CharacterDataStore 에 접근해서 다른 캐릭터들의 NickName 을 가져와서 비교
	FClientDataStore& CliDS = BladeIIGameImpl::GetClientDataStore();
	for (int8 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		EPCClass ThisPCClass = IntToPCClass(PCI);
		if (ThisPCClass == EditingPCClass){
			continue;
		}
		// Nick 이 아닌 ClassName 을 가져올 수도 있는데 여하간 그거랑 충돌이 나도 곤란하지.
		const FString ThisClassOrNickName = CliDS.GetLocalCharClassOrNickName(ThisPCClass).ToString();
		const FString ThisClassName = CliDS.GetLocalCharClassName(ThisPCClass).ToString(); // 클래스이름도 걍 비교해 버리자.
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
		// 길이 제한 넘어가면 자르는데 자른 걸 바로 세팅하면 한글에서 크래쉬가 나고 이렇게 자른 걸 저장해 두었다가 Tick 에서 세팅
		// 그리고 여기선 일단 비워둬야 함.

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
	const FText EdittedNickName = ETB_NickName.IsValid() ? ETB_NickName->GetText() : FText::FromString(TEXT("")); // 빈 문자열이면 WrongNickName 에 걸리겠지..
	bool bPopupWarnMsg = false;
	FText WarnMsgText;

	// 규칙에 어긋난지, 다른 닉네임과 중복되지 않는지 체크해서 팝업.
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
		//CloseMe(); 여기서 안 닫고 응답이 오면 닫는다. 서버에서 따로 더 정교하게 잘못된 이름을 감지하므로. (HandleServerError1012)
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
		ETB_NickName->SetText(FText::FromString(TEXT(""))); // 비운다.
	}

	this->SetVisibility(ESlateVisibility::Collapsed);
}
