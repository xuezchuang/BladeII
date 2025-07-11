#include "B2UIChatSinglePopup.h"
#include "B2UIManager.h"

UB2UIChatSinglePopup::UB2UIChatSinglePopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayTime = 3.0f;
	MaxDisplayTextLen = 60;

	LastUpdatedTime = 0.0f;
}

void UB2UIChatSinglePopup::Init()
{
	Super::Init();

	LastUpdatedTime = (float)FPlatformTime::Seconds();
}

void UB2UIChatSinglePopup::CacheAssets()
{
	GET_SLOT(UB2RichTextBlock, TB_Message)

	GET_SLOT(UB2Button, BTN_Message)
}

void UB2UIChatSinglePopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Message, &UB2UIChatSinglePopup::OnClickBTN_Message);
}

void UB2UIChatSinglePopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UE_LOG(LogBladeII, Log, TEXT("UB2UIChatSinglePopup::NativeTick"));

	// Ÿ�̸Ӵ� GameTime ���� ��� ���⼭ ���� ī��Ʈ
	const float ThisTime = GetWorld()->GetTimeSeconds();
	if (ThisTime - LastUpdatedTime >= DisplayTime)
	{
		UE_LOG(LogBladeII, Log, TEXT("UB2UIChatSinglePopup::NativeTick - CloseMe()"));
		CloseMe();
	}
}

void UB2UIChatSinglePopup::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	
}

void UB2UIChatSinglePopup::UpdateWithNewChatMessage(const FB2ChatMessage& InMessage)
{
	if (CheckDisplayCoolTime())
		return;

	// ���� ���� ä�� â�� Ȱ��ȭ �Ǿ� �ִٸ� �̰� ��� �ȵȴ�. ä�� â�� ���� �� �����ֱ� ���� ���̹Ƿ�.
	// �׷����� ���⼭ �װ� üũ������ �ʴ� �ɷ�..
	
	if (TB_Message.IsValid())
	{
		// ���� ä�� ������. ���������̶� ä�� �޽���, � ��Ȳ���� ǥ���ϰ� ���ϰ� ���� Ȯ���� �� �Ŀ� �ٽ� ���� ������.
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		TB_Message->SetText(InMessage.MessageText);

		LastUpdatedTime = GetWorld()->GetTimeSeconds(); //(float)FPlatformTime::Seconds();
	}
}

FText UB2UIChatSinglePopup::GetMessageHeader(const FB2ChatMessage& InMessage)
{
	FText BaseHeaderFormat = FText::FromString(TEXT("[{0}]"));

 	switch (InMessage.MessageType)
	{
 	case EB2ChatType::Notice: return FText::Format(BaseHeaderFormat, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_All")));
 	case EB2ChatType::Channel: return FText::Format(BaseHeaderFormat, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")));
 	case EB2ChatType::GuildNotice: return FText::Format(BaseHeaderFormat, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_GeneralNoti")));
 	case EB2ChatType::Guild: return FText::Format(BaseHeaderFormat, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_Guild")));
 	case EB2ChatType::OneToOne: return FText::Format(BaseHeaderFormat, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_Guild")));
	}

	return FText::FromString(TEXT(""));
}

void UB2UIChatSinglePopup::CloseMe()
{
	// �ΰ��ӿ��� ������ �� �� �����Ƿ� �� ���ܵδ� �� �������� �𸣰ڴ�.
	this->SetVisibility(ESlateVisibility::Collapsed);

	CahcedDisplayCoolTime = 0.f;
}

bool UB2UIChatSinglePopup::CheckDisplayCoolTime()
{
	const float ThisTime = GetWorld()->GetTimeSeconds();

	return ThisTime - CahcedDisplayCoolTime < DisplayCoolTime ? true : false;
}

void UB2UIChatSinglePopup::OnClickBTN_Message()
{
	CahcedDisplayCoolTime = GetWorld()->GetTimeSeconds();

	this->SetVisibility(ESlateVisibility::Collapsed);
}