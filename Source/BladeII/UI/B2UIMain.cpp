
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
//		if (FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode())// 영상 덤프 등에 방해가 된다는 민원으로 인해 좀 철저히 막게 됨.
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
		//여기서 Overlay패널을 생성해서 AddToParent한다. 
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
		{// 한 사이클 다 돌면 다시 처음부터 플레이 하던지, stop 되던지.
			OnOnTouchImpulseRingAnimTimeCycle();
		}
	}
}

UPanelWidget* UB2UIMain::GetParentLayer(EUILayer InLayer)
{
	if (CP_TopMost.IsValid())
	{ // Init 에서 Layer 인덱스 맞춰 Child Overlay 들을 추가해 놓았다고 가정하는 것이므로 CP_TopMost 아래에는 여타 다른 것이 있으면 안된다. 
		// UMG 상에서는 비어있는 채 딸랑 자리만 차지하고 있는 Canvas Panel 이어야 함.
		auto ChildPanel = Cast<UPanelWidget>(CP_TopMost->GetChildAt(static_cast<int32>(InLayer)));
		return ChildPanel;
	}
	return nullptr;
}

void UB2UIMain::OnOnTouchImpulseRingAnimTimeCycle()
{
	if (bSignaledMaintainImpulseRing){
		DefOnTouchImpulseRing.Play(); // 다시 처음부터 시작
	}
	else{
		DefOnTouchImpulseRing.Stop(); // 아니면 이제 그만
	}
}

void UB2UIMain::ConditionalPlayOnTouchImpulseRing(const FVector2D& InPosition)
{
	bSignaledMaintainImpulseRing = true;

	// 빠른 터치 시 매번 새로 플레이 되지 않도록 켜져 있는 동안에는 타이머에 의해서만 다시 플레이.
	if (!DefOnTouchImpulseRing.IsCurrentOn())
	{
		DefOnTouchImpulseRing.Play();
		MoveOnTouchImpulseRing(InPosition);
	}
}

void UB2UIMain::PendingStopOnTouchImpulseRing()
{
	bSignaledMaintainImpulseRing = false; // 플레이 되고 있는 상황이라면 OnOnTouchImpulseRingAnimTimeCycle 에서 픽업할 것.
}

void UB2UIMain::MoveOnTouchImpulseRing(const FVector2D& InMovedPosition)
{	
	FVector2D ResScale = GetDesignToCurrentViewScale(GetOwningPlayer(), true);
	DefOnTouchImpulseRing.SetImageCenterPosition(InMovedPosition / ResScale); // UMG 기준 해상도로 전환해서 넘겨줌.
}

void UB2UIMain::ShowNetworkLoadingIcon(bool bInShow)
{
//	if (UIP_NetWorkWaitResponseP.IsValid()
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		&& (!FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode() || !bInShow) // 영상 덤프 등에 방해가 된다는 민원으로 인해 좀 철저히 막게 됨.
//#endif		
//		)
//	{ // 기타 입력을 막기 위해 Visible 로.
//		UIP_NetWorkWaitResponseP->SetVisibility(bInShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
//		UIP_NetWorkWaitResponseP->SetVisibilityReserve(bInShow);
//	}
}

void UB2UIMain::ShowMiscLoadingIcon(bool bInShow)
{
	//if (UIP_MiscLoadingP.IsValid())
	//{ // NetWorkWaitResponse 와 마찬가지.. 기타 입력을 막기 위해 Visible 로. 내부적으로 일부 설정은 다를 것.
	//	UIP_MiscLoadingP->SetVisibility(bInShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	//	UIP_MiscLoadingP->SetVisibilityReserve(bInShow);
	//}
}

void UB2UIMain::OpenChatSinglePopupWithNewMessage(const FB2ChatMessage& InNewMessage)
{
	//if (UIP_ChatSinglePopupP.IsValid())
	//{
	//	UIP_ChatSinglePopupP->UpdateWithNewChatMessage(InNewMessage); // Visibility 는 안에서 알아서 세팅되었다가 리셋될 거.
	//}
}
void UB2UIMain::ForceCloseChatSinglePopup()
{
	//if (UIP_ChatSinglePopupP.IsValid())
	//{
	//	UIP_ChatSinglePopupP->CloseMe(); // 대체로는 UpdateWithNewChatMessage 후에 이렇게 하지 않아도 알아서 닫힘. 이건 특별한 경우 사용.
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

//[@AKI, 180126] 글로벌 변수는 쓰기 싫지만.....벌써......
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
	//[@AKI, 180126] CDN 주소나 GameServer 주소에 QA와 관련된것이 있으면 일부러 QA로 표기하여
	//이상이 있음을 감지하게 함
	if (gameServerCount == -1 && cdnCount == -1)
	{
		KakaoServerType = FText::FromString(TEXT("Li"));
		//라이브 인지 아닌지 판별하는 변수는 여기에 들어가면 됨
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
	else //kakao 서버
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