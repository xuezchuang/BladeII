
#include "B2UIMain.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUICondition.h"
#include "B2UIDocScenarioDialog.h"
#include "BladeIIBlockToSyncNetwork.h"
//#include "B2Version.h"
#include "Global.h"
#include "Retailer.h"

UB2UIMain::UB2UIMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OnTouchImpulseRingAnimTime = 0.2f;
	OnTouchImpulseRingParamRange_Begin = 0.5f;
	OnTouchImpulseRingParamRange_End = 1.5f;
	bSignaledMaintainImpulseRing = false;
	bSubscribed = false;
	DeliveryGetServerVersionTicket = 0;
}

void UB2UIMain::CacheAssets()
{
//	GET_SLOT(UCanvasPanel, CP_TopMost)
//	GET_SLOT(UB2Image, IMG_DefaultOnTouchImpulse)
//	if (IMG_DefaultOnTouchImpulse.IsValid())
//	{
//		DefOnTouchImpulseRing.Init(this, IMG_DefaultOnTouchImpulse.Get(), OnTouchImpulseRingAnimTime, OnTouchImpulseRingParamRange_Begin, OnTouchImpulseRingParamRange_End);
//	}
//
//	GET_SLOT(UB2UINetWorkLoading, UIP_NetWorkWaitResponseP)
//	if (UIP_NetWorkWaitResponseP.IsValid())
//	{
//		UIP_NetWorkWaitResponseP->Init();
//		//UIP_NetWorkWaitResponseP->SetVisibility(ESlateVisibility::Collapsed);
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		if (FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode())// ���� ���� � ���ذ� �ȴٴ� �ο����� ���� �� ö���� ���� ��.
//		{
//			UIP_NetWorkWaitResponseP->SetVisibility(ESlateVisibility::Hidden);
//		}
//#endif		
//	}
//	GET_SLOT(UB2UINetWorkLoading, UIP_MiscLoadingP)
//	if (UIP_MiscLoadingP.IsValid())
//	{
//		UIP_MiscLoadingP->Init();
//		UIP_MiscLoadingP->SetVisibility(ESlateVisibility::Collapsed);
//	}
//
//	GET_SLOT(UB2UIChatSinglePopup, UIP_ChatSinglePopupP)
//	if (UIP_ChatSinglePopupP.IsValid())
//	{
//		UIP_ChatSinglePopupP->Init();
//		UIP_ChatSinglePopupP->SetVisibility(ESlateVisibility::Collapsed);
//	}
//
//	GET_SLOT(UTextBlock, TB_DebugVersionInfo)
//	GET_SLOT(UTextBlock, TB_DebugServerVersionInfo)
//
//	TB_DebugVersionInfo->SetVisibility(ESlateVisibility::Collapsed);
//	TB_DebugServerVersionInfo->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIMain::UpdateStaticText()
{
}

void UB2UIMain::BindDelegates()
{
}

void UB2UIMain::Init()
{
	Super::Init();

	for (uint8 LayerCnt = static_cast<uint8>(EUILayer::None); LayerCnt < static_cast<uint8>(EUILayer::Max); ++LayerCnt)
	{
		//���⼭ Overlay�г��� �����ؼ� AddToParent�Ѵ�. 
		UOverlay* NewChildPanel = NewObject<UOverlay>(this, NAME_None, RF_Transient);
		UCanvasPanelSlot* AddedSlot = Cast<UCanvasPanelSlot>(CP_TopMost->AddChild(NewChildPanel));
		if (AddedSlot)
		{
			AddedSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			AddedSlot->SetSize(FVector2D(0.f, 0.f));
		}
	}

	SetDebugVersionInfo();
	ShowVersionInfo();
}

void UB2UIMain::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (DefOnTouchImpulseRing.IsCurrentOn())
	{
		if (DefOnTouchImpulseRing.Tick(InDeltaTime))
		{// �� ����Ŭ �� ���� �ٽ� ó������ �÷��� �ϴ���, stop �Ǵ���.
			OnOnTouchImpulseRingAnimTimeCycle();
		}
	}
}

UPanelWidget* UB2UIMain::GetParentLayer(EUILayer InLayer)
{
	if (CP_TopMost.IsValid())
	{ // Init ���� Layer �ε��� ���� Child Overlay ���� �߰��� ���Ҵٰ� �����ϴ� ���̹Ƿ� CP_TopMost �Ʒ����� ��Ÿ �ٸ� ���� ������ �ȵȴ�. 
		// UMG �󿡼��� ����ִ� ä ���� �ڸ��� �����ϰ� �ִ� Canvas Panel �̾�� ��.
		auto ChildPanel = Cast<UPanelWidget>(CP_TopMost->GetChildAt(static_cast<int32>(InLayer)));
		return ChildPanel;
	}
	return nullptr;
}

void UB2UIMain::OnOnTouchImpulseRingAnimTimeCycle()
{
	if (bSignaledMaintainImpulseRing){
		DefOnTouchImpulseRing.Play(); // �ٽ� ó������ ����
	}
	else{
		DefOnTouchImpulseRing.Stop(); // �ƴϸ� ���� �׸�
	}
}

void UB2UIMain::ConditionalPlayOnTouchImpulseRing(const FVector2D& InPosition)
{
	bSignaledMaintainImpulseRing = true;

	// ���� ��ġ �� �Ź� ���� �÷��� ���� �ʵ��� ���� �ִ� ���ȿ��� Ÿ�̸ӿ� ���ؼ��� �ٽ� �÷���.
	if (!DefOnTouchImpulseRing.IsCurrentOn())
	{
		DefOnTouchImpulseRing.Play();
		MoveOnTouchImpulseRing(InPosition);
	}
}

void UB2UIMain::PendingStopOnTouchImpulseRing()
{
	bSignaledMaintainImpulseRing = false; // �÷��� �ǰ� �ִ� ��Ȳ�̶�� OnOnTouchImpulseRingAnimTimeCycle ���� �Ⱦ��� ��.
}

void UB2UIMain::MoveOnTouchImpulseRing(const FVector2D& InMovedPosition)
{	
	FVector2D ResScale = GetDesignToCurrentViewScale(GetOwningPlayer(), true);
	DefOnTouchImpulseRing.SetImageCenterPosition(InMovedPosition / ResScale); // UMG ���� �ػ󵵷� ��ȯ�ؼ� �Ѱ���.
}

void UB2UIMain::ShowNetworkLoadingIcon(bool bInShow)
{
//	if (UIP_NetWorkWaitResponseP.IsValid()
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		&& (!FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode() || !bInShow) // ���� ���� � ���ذ� �ȴٴ� �ο����� ���� �� ö���� ���� ��.
//#endif		
//		)
//	{ // ��Ÿ �Է��� ���� ���� Visible ��.
//		UIP_NetWorkWaitResponseP->SetVisibility(bInShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
//		UIP_NetWorkWaitResponseP->SetVisibilityReserve(bInShow);
//	}
}

void UB2UIMain::ShowMiscLoadingIcon(bool bInShow)
{
	//if (UIP_MiscLoadingP.IsValid())
	//{ // NetWorkWaitResponse �� ��������.. ��Ÿ �Է��� ���� ���� Visible ��. ���������� �Ϻ� ������ �ٸ� ��.
	//	UIP_MiscLoadingP->SetVisibility(bInShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	//	UIP_MiscLoadingP->SetVisibilityReserve(bInShow);
	//}
}

void UB2UIMain::OpenChatSinglePopupWithNewMessage(const FB2ChatMessage& InNewMessage)
{
	//if (UIP_ChatSinglePopupP.IsValid())
	//{
	//	UIP_ChatSinglePopupP->UpdateWithNewChatMessage(InNewMessage); // Visibility �� �ȿ��� �˾Ƽ� ���õǾ��ٰ� ���µ� ��.
	//}
}
void UB2UIMain::ForceCloseChatSinglePopup()
{
	//if (UIP_ChatSinglePopupP.IsValid())
	//{
	//	UIP_ChatSinglePopupP->CloseMe(); // ��ü�δ� UpdateWithNewChatMessage �Ŀ� �̷��� ���� �ʾƵ� �˾Ƽ� ����. �̰� Ư���� ��� ���.
	//}
}

FText GetDebugVersionInfo()
{
	FText VersionText = FText::FromString(TEXT(PREPROCESSOR_TO_STRING(VERSION)));

	return  VersionText;
}

FText GetChangelistInfo()
{
	FText CLText = FText::FromString(TEXT(PREPROCESSOR_TO_STRING(CL)));

	return CLText;
}

//[@AKI, 180126] �۷ι� ������ ���� ������.....����......
FString g_SessionServerUrl;
FString g_CDNUrl;

void SetCDNUrl(FString _CDNUrl)
{
	g_CDNUrl = _CDNUrl.ToUpper();
}

void SetSessionServerUrl(FString _sessionServerUrl)
{
	g_SessionServerUrl = _sessionServerUrl.ToUpper();
}

FText GetKakaoServerType()
{
	FText KakaoServerType = FText::FromString(TEXT("QA"));
	int32 gameServerCount = g_SessionServerUrl.Find(TEXT("QA"));
	int32 cdnCount = g_CDNUrl.Find(TEXT("BETA"));
	//[@AKI, 180126] CDN �ּҳ� GameServer �ּҿ� QA�� ���õȰ��� ������ �Ϻη� QA�� ǥ���Ͽ�
	//�̻��� ������ �����ϰ� ��
	if (gameServerCount == -1 && cdnCount == -1)
	{
		KakaoServerType = FText::FromString(TEXT("Li"));
		//���̺� ���� �ƴ��� �Ǻ��ϴ� ������ ���⿡ ���� ��
	}

	return KakaoServerType;
}

FText GetDebugServerInfo()
{
	//A(alpha), B(beta), P(public demo), Q(kakao QA), R(apple review), L(live)
	FText ResultType = FText::FromString(TEXT("Unknown"));

	int32 gameServerCount = g_SessionServerUrl.Find(TEXT("kakao"));
	int32 cdnCount = g_CDNUrl.Find(TEXT("kakao"));
	if (gameServerCount == -1 && cdnCount == -1)
	{
		FString ServerType;
		GConfig->GetString(TEXT("NetworkSettings"), TEXT("ServerType"), ServerType, GB2NetworkIni);
		if (ServerType == TEXT("Alpha"))
		{
			ResultType = FText::FromString(TEXT("Alpha"));
		}
		else if (ServerType == TEXT("Beta"))
		{
			ResultType = FText::FromString(TEXT("Beta"));
		}
		else if (ServerType == TEXT("Public"))
		{
			ResultType = FText::FromString(TEXT("Public"));
		}
	}
	else //kakao ����
	{
		ResultType = FText::FromString(TEXT("Kakao-") + GetKakaoServerType().ToString());
	}
	return ResultType;
}

FText GetDebugRendererInfo()
{
	//return FText::FromString(FPlatformMisc::GetRendererType());
	return FText::GetEmpty();
}

FText GetBuildConfiguration()
{
#if UE_BUILD_SHIPPING
	FString BuildConfig(TEXT("shp"));
#elif UE_BUILD_DEVELOPMENT
	FString BuildConfig(TEXT("dev"));
#else
	FString BuildConfig(TEXT("?"));
#endif

	return FText::FromString(BuildConfig);
}

FText GetBuildInfo()
{
	FText TextFotmat = FText::FromString(TEXT("{0}/{1}/{2}"));
	FText BuildInfo  = FText::Format(TextFotmat, GetDebugServerInfo(), GetDebugRendererInfo(), GetBuildConfiguration());

	return BuildInfo;
}

void UB2UIMain::SetDebugVersionInfo()
{
	if (!TB_DebugVersionInfo.IsValid())
		return;

	FText TextFotmat	= FText::FromString(TEXT("{0}.{1} ({2})"));
	FText VersionInfo	= FText::Format(TextFotmat, GetDebugVersionInfo(), GetChangelistInfo(), GetBuildInfo());

	TB_DebugVersionInfo->SetText(VersionInfo);
}

void UB2UIMain::ShowVersionInfo()
{
	auto* doc = UB2UIDocHelper::GetDocUser();
	if (doc)
	{
		const auto flag = doc->GetShowVersion();
		if (flag)
		{
			SetDebugVersionInfo();
		}

		const auto visibility = flag ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

		if (TB_DebugVersionInfo.IsValid())
		{
			TB_DebugVersionInfo->SetVisibility(visibility);
		}

		if (TB_DebugServerVersionInfo.IsValid())
		{
			TB_DebugServerVersionInfo->SetVisibility(visibility);
		}
	}
}

void UB2UIMain::SetServerVersionInfoText(FText ftServerInfoText)
{
	if (TB_DebugServerVersionInfo.IsValid())
	{
		TB_DebugServerVersionInfo->SetText(ftServerInfoText);
	}
}