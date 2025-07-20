
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
//		if (FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode())// 康惑 待橇 殿俊 规秦啊 等促绰 刮盔栏肺 牢秦 粱 枚历洒 阜霸 凳.
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
		//咯扁辑 Overlay菩澄阑 积己秦辑 AddToParent茄促. 
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
		{// 茄 荤捞努 促 倒搁 促矫 贸澜何磐 敲饭捞 窍带瘤, stop 登带瘤.
			OnOnTouchImpulseRingAnimTimeCycle();
		}
	}
}

UPanelWidget* UB2UIMain::GetParentLayer(EUILayer InLayer)
{
	if (CP_TopMost.IsValid())
	{ // Init 俊辑 Layer 牢郸胶 嘎苗 Child Overlay 甸阑 眠啊秦 初疽促绊 啊沥窍绰 巴捞骨肺 CP_TopMost 酒贰俊绰 咯鸥 促弗 巴捞 乐栏搁 救等促. 
		// UMG 惑俊辑绰 厚绢乐绰 盲 叠尔 磊府父 瞒瘤窍绊 乐绰 Canvas Panel 捞绢具 窃.
		auto ChildPanel = Cast<UPanelWidget>(CP_TopMost->GetChildAt(static_cast<int32>(InLayer)));
		return ChildPanel;
	}
	return nullptr;
}

void UB2UIMain::OnOnTouchImpulseRingAnimTimeCycle()
{
	if (bSignaledMaintainImpulseRing){
		DefOnTouchImpulseRing.Play(); // 促矫 贸澜何磐 矫累
	}
	else{
		DefOnTouchImpulseRing.Stop(); // 酒聪搁 捞力 弊父
	}
}

void UB2UIMain::ConditionalPlayOnTouchImpulseRing(const FVector2D& InPosition)
{
	bSignaledMaintainImpulseRing = true;

	// 狐弗 磐摹 矫 概锅 货肺 敲饭捞 登瘤 臼档废 难廉 乐绰 悼救俊绰 鸥捞赣俊 狼秦辑父 促矫 敲饭捞.
	if (!DefOnTouchImpulseRing.IsCurrentOn())
	{
		DefOnTouchImpulseRing.Play();
		MoveOnTouchImpulseRing(InPosition);
	}
}

void UB2UIMain::PendingStopOnTouchImpulseRing()
{
	bSignaledMaintainImpulseRing = false; // 敲饭捞 登绊 乐绰 惑炔捞扼搁 OnOnTouchImpulseRingAnimTimeCycle 俊辑 侨诀且 巴.
}

void UB2UIMain::MoveOnTouchImpulseRing(const FVector2D& InMovedPosition)
{	
	FVector2D ResScale = GetDesignToCurrentViewScale(GetOwningPlayer(), true);
	DefOnTouchImpulseRing.SetImageCenterPosition(InMovedPosition / ResScale); // UMG 扁霖 秦惑档肺 傈券秦辑 逞败淋.
}

void UB2UIMain::ShowNetworkLoadingIcon(bool bInShow)
{
//	if (UIP_NetWorkWaitResponseP.IsValid()
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		&& (!FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode() || !bInShow) // 康惑 待橇 殿俊 规秦啊 等促绰 刮盔栏肺 牢秦 粱 枚历洒 阜霸 凳.
//#endif		
//		)
//	{ // 扁鸥 涝仿阑 阜扁 困秦 Visible 肺.
//		UIP_NetWorkWaitResponseP->SetVisibility(bInShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
//		UIP_NetWorkWaitResponseP->SetVisibilityReserve(bInShow);
//	}
}

void UB2UIMain::ShowMiscLoadingIcon(bool bInShow)
{
	//if (UIP_MiscLoadingP.IsValid())
	//{ // NetWorkWaitResponse 客 付蛮啊瘤.. 扁鸥 涝仿阑 阜扁 困秦 Visible 肺. 郴何利栏肺 老何 汲沥篮 促甫 巴.
	//	UIP_MiscLoadingP->SetVisibility(bInShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	//	UIP_MiscLoadingP->SetVisibilityReserve(bInShow);
	//}
}

void UB2UIMain::OpenChatSinglePopupWithNewMessage(const FB2ChatMessage& InNewMessage)
{
	//if (UIP_ChatSinglePopupP.IsValid())
	//{
	//	UIP_ChatSinglePopupP->UpdateWithNewChatMessage(InNewMessage); // Visibility 绰 救俊辑 舅酒辑 技泼登菌促啊 府悸瞪 芭.
	//}
}
void UB2UIMain::ForceCloseChatSinglePopup()
{
	//if (UIP_ChatSinglePopupP.IsValid())
	//{
	//	UIP_ChatSinglePopupP->CloseMe(); // 措眉肺绰 UpdateWithNewChatMessage 饶俊 捞犯霸 窍瘤 臼酒档 舅酒辑 摧塞. 捞扒 漂喊茄 版快 荤侩.
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

//[@AKI, 180126] 臂肺国 函荐绰 静扁 饺瘤父.....国结......
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
	//[@AKI, 180126] CDN 林家唱 GameServer 林家俊 QA客 包访等巴捞 乐栏搁 老何矾 QA肺 钎扁窍咯
	//捞惑捞 乐澜阑 皑瘤窍霸 窃
	if (gameServerCount == -1 && cdnCount == -1)
	{
		KakaoServerType = FText::FromString(TEXT("Li"));
		//扼捞宏 牢瘤 酒囱瘤 魄喊窍绰 函荐绰 咯扁俊 甸绢啊搁 凳
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
	else //kakao 辑滚
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