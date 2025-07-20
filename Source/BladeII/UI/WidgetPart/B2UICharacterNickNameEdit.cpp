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

	// 辨捞 力茄 逞绢啊辑 肋赴 咆胶飘 咯扁辑 技泼.
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
		ETB_NickName->SetText(FText::FromString(TEXT(""))); // 厚款促.
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
	// TODO : 陛蘑绢 扁瓷 盲泼苞 窃膊 眠饶 备泅..

	return true;
}

bool UB2UICharacterNickNameEdit::CheckForWrongNickName(const FText& InNickName)
{
	FString TextString = InNickName.ToString();
	if (TextString.Len() < B2_CHAR_NICKNAME_MIN_LENGTH || TextString.Len() > B2_CHAR_NICKNAME_MAX_LENGTH) // 辨捞 逞绢啊绰 扒 拱沸 扁夯利栏肺 俊叼飘 矫 贸府甫 窍变 窃.
	{
		return false;
	}

	// 扁鸥?

	return true;
}

bool UB2UICharacterNickNameEdit::CheckForDuplicatedNickName(const FText& InNickName)
{
	// 厚背绰 老窜 彬阐窍霸 FString 栏肺 秦 焊磊.. FText 肺 厚背窍促啊 磊末 促惫绢 包访秦辑 肋给 部老瘤档 葛福摆促.
	const FString InNickNameStr = InNickName.ToString();

	// CharacterDataStore 俊 立辟秦辑 促弗 某腐磐甸狼 NickName 阑 啊廉客辑 厚背
	FClientDataStore& CliDS = BladeIIGameImpl::GetClientDataStore();
	for (int8 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		EPCClass ThisPCClass = IntToPCClass(PCI);
		if (ThisPCClass == EditingPCClass){
			continue;
		}
		// Nick 捞 酒囱 ClassName 阑 啊廉棵 荐档 乐绰单 咯窍埃 弊芭尔 面倒捞 唱档 帮鄂窍瘤.
		const FString ThisClassOrNickName = CliDS.GetLocalCharClassOrNickName(ThisPCClass).ToString();
		const FString ThisClassName = CliDS.GetLocalCharClassName(ThisPCClass).ToString(); // 努贰胶捞抚档 傲 厚背秦 滚府磊.
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
		// 辨捞 力茄 逞绢啊搁 磊福绰单 磊弗 吧 官肺 技泼窍搁 茄臂俊辑 农贰浆啊 唱绊 捞犯霸 磊弗 吧 历厘秦 滴菌促啊 Tick 俊辑 技泼
		// 弊府绊 咯扁急 老窜 厚况抵具 窃.

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
	const FText EdittedNickName = ETB_NickName.IsValid() ? ETB_NickName->GetText() : FText::FromString(TEXT("")); // 后 巩磊凯捞搁 WrongNickName 俊 吧府摆瘤..
	bool bPopupWarnMsg = false;
	FText WarnMsgText;

	// 痹蘑俊 绢边抄瘤, 促弗 葱匙烙苞 吝汗登瘤 臼绰瘤 眉农秦辑 扑诀.
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
		//CloseMe(); 咯扁辑 救 摧绊 览翠捞 坷搁 摧绰促. 辑滚俊辑 蝶肺 歹 沥背窍霸 肋给等 捞抚阑 皑瘤窍骨肺. (HandleServerError1012)
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
		ETB_NickName->SetText(FText::FromString(TEXT(""))); // 厚款促.
	}

	this->SetVisibility(ESlateVisibility::Collapsed);
}
