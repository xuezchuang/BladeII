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

	// 鸥捞赣绰 GameTime 静绰 措脚 咯扁辑 流立 墨款飘
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

	// 父老 泅犁 盲泼 芒捞 劝己拳 登绢 乐促搁 捞吧 凯绢急 救等促. 盲泼 芒捞 绝阑 锭 焊咯林扁 困茄 巴捞骨肺.
	// 弊犯瘤父 咯扁辑 弊吧 眉农窍瘤绰 臼绰 吧肺..
	
	if (TB_Message.IsValid())
	{
		// 茄临 盲泼 波初澜. 傍瘤荤亲捞尔 盲泼 皋矫瘤, 绢恫 惑炔俊辑 钎矫窍绊 救窍绊 殿捞 犬角洒 等 饶俊 促矫 凯绢 焊档废.
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
	// 牢霸烙俊辑 攫力电 躲 荐 乐栏骨肺 傲 见败滴绰 霸 唱阑瘤档 葛福摆促.
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