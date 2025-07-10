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

	// 타이머는 GameTime 쓰는 대신 여기서 직접 카운트
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

	// 만일 현재 채팅 창이 활성화 되어 있다면 이걸 열어선 안된다. 채팅 창이 없을 때 보여주기 위한 것이므로.
	// 그렇지만 여기서 그걸 체크하지는 않는 걸로..
	
	if (TB_Message.IsValid())
	{
		// 한줄 채팅 꺼놓음. 공지사항이랑 채팅 메시지, 어떤 상황에서 표시하고 안하고 등이 확실히 된 후에 다시 열어 보도록.
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
	// 인게임에서 언제든 뜰 수 있으므로 걍 숨겨두는 게 나을지도 모르겠다.
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