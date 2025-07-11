
#include "BladeIIBlockToSyncNetwork.h"
#include "B2UIManager.h"
#include "B2UIMsgPopupCenter.h"
#include "Retailer.h"
#include "B2NetworkBridge.h"
#include "B2UIMsgPopupSimple.h"
#include "B2LogManager.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2NetGameMode.h"
#include "ChannelManager.h"
#include "B2ServerResultCodeTable.h"


FBladeIIBlockToSyncNetwork& FBladeIIBlockToSyncNetwork::GetInstance()
{
	static FBladeIIBlockToSyncNetwork Instance;

	return Instance;
}

static const float LimitTime = 15.f;
static const int8 MaxRetryCount = 2;

FBladeIIBlockToSyncNetwork::FBladeIIBlockToSyncNetwork()
//	: BlockingLimitTime(0), BlockUIInstance(nullptr), RetryPopupUIMsgID(INDEX_NONE)
//	, RetryCount(0)
//	, m_DisconnectReason(DisconnectResasonType::EDISCONNECT_REASON_NONE)
//	, EnableBlockToSync(false)
//	, IsNetworkInitialized(false)
//	, IsRequestMasterData(false)
//	, m_errorCode(-1)
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	, bIsDevStandaloneMode(BUILD_STANDALONE_TEST_APK)
//#endif
{

}

FBladeIIBlockToSyncNetwork::~FBladeIIBlockToSyncNetwork()
{

}

void FBladeIIBlockToSyncNetwork::Tick(float DeltaTime)
{ 
//	if (m_DisconnectReason == DisconnectResasonType::EDISCONNECT_REASON_NONE) return;
//	if (BlockingLimitTime < FLT_EPSILON) return;
//
//	switch (m_DisconnectReason)
//	{
//		case DisconnectResasonType::EDISCONNECT_REASON_RESPONSE:
//		{
//			//�̰� ��ȣ �ڵ� ����
//			if (RetryRequest == nullptr)
//			{//UI Ŭ��¡�� ����� ���� ���� ��� �߻�.
//				SetFree();
//				return;
//			}
//			
//			UpdateBlockingLimitTime(DeltaTime, [this]() {this->OpenNetworkConnectionFailUI();});
//			break;
//		}
//
//		case DisconnectResasonType::EDISCONNECT_REASON_SESSION:
//		{
//			if (IsConnectedToServer())
//			{
//				SetValueByDefault();
//				CloseBlockToSyncNetworkUI();
//				return;
//			}
//
//			UpdateBlockingLimitTime(DeltaTime, [this]() {this->OpenNetworkConnectionFailUI();});
//
//			break;
//		}
//
//		case DisconnectResasonType::EDISCONNECT_REASON_CHANNEL:
//		{
//			static float DisconnectDelaySpeed = 3.0f;
//			UpdateBlockingLimitTime(DeltaTime * DisconnectDelaySpeed, [this]() {this->OpenChannelConnectionFailUI();});
//			break;
//		}
//
//		case DisconnectResasonType::EDISCONNECT_REASON_TOTAL:
//		{
//
//		}
//		default:
//			SetValueByDefault();
//			break;
//	}
}

//void FBladeIIBlockToSyncNetwork::UpdateBlockingLimitTime(float DeltaTime, std::function<void(void)> ConnectFailUIFunc)
//{
//	BlockingLimitTime = FMath::Max(BlockingLimitTime - DeltaTime, 0.f);
//
//	if (BlockingLimitTime < FLT_EPSILON) //== 0���ٴ� EPSILON�� ���°� �����ϴ�...
//	{
//		ConnectFailUIFunc();
//	}
//}
//
TStatId FBladeIIBlockToSyncNetwork::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FBladeIIBlockToSyncNetwork, STATGROUP_Tickables);
}

void FBladeIIBlockToSyncNetwork::SetBlockTillResponse(std::function<void(void)> RequestFunc)
{
	//if (IsEnableRequest(RequestFunc))
	//{
	//	m_DisconnectReason = DisconnectResasonType::EDISCONNECT_REASON_RESPONSE;
	//	try
	//	{
	//		RequestFunc();
	//	}
	//	catch (const std::bad_function_call& e)
	//	{
	//		UE_LOG(LogBladeII, Error, TEXT("FBladeIIBlockToSyncNetwork::Retry() bad_function_call %s"), e.what());
	//	}

	//	RetryRequest = RequestFunc;

	//	SetValueByLimitTime();
	//	OpenBlockToSyncNetworkUI();
	//}
}

void FBladeIIBlockToSyncNetwork::SetValueByDefault()
{
	RetryCount = 0;
	m_DisconnectReason = DisconnectResasonType::EDISCONNECT_REASON_NONE;
	m_errorCode = -1;
}

void FBladeIIBlockToSyncNetwork::SessionDisconnect(int32 _errorCode)
{
	//m_DisconnectReason = DisconnectResasonType::EDISCONNECT_REASON_SESSION;

	////FB2NetworkBridge::GetInstance()->Shutdown();
	////FB2NetworkBridge::GetInstance()->Init();

	//SetValueByLimitTime();
	//OpenBlockToSyncNetworkUI();

	RetryCount = MaxRetryCount;
}

void FBladeIIBlockToSyncNetwork::ChannelDisconnect(int32 _errorCode)
{
	//auto* UIMgr = UB2UIManager::GetInstance();
	//if (UIMgr && UIMgr->GetCurrUIScene() == EUIScene::Title)
	//{
	//	RetryRequest = nullptr;
	//	return;
	//}

	if (m_DisconnectReason == DisconnectResasonType::EDISCONNECT_REASON_CHANNEL)
		return;

	m_DisconnectReason = DisconnectResasonType::EDISCONNECT_REASON_CHANNEL;

	SetValueByLimitTime();
	OpenBlockToSyncNetworkUI();

	m_errorCode = _errorCode;
}

void FBladeIIBlockToSyncNetwork::ServiceNotAvailable(int32 _errorCode)
{
	if (auto* UIMgr = UB2UIManager::GetInstance())
	{
		EUIMsgPopupButtonGroup group = EUIMsgPopupButtonGroup::ReconnectOrGoToTitle;
		FText titleText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Network_Error"));
		FText contentText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Network_RetryContent"));
		FMsgPopupOnClick OnBtnPosClick = FMsgPopupOnClick::CreateLambda([this]() {GoToTitle(); });
		EPopUpPriority PopupPriority = EPopUpPriority::Server_Message_GoToTitle;

		m_errorCode = _errorCode;
		//[@AKI, 180717][todo]
		//FText -> FString is dangerous as it is a potentially lossy conversion for some languages. https://docs.unrealengine.com/en-us/Programming/UnrealArchitecture/StringHandling/FText
		if (m_errorCode > -1)
			contentText = FText::FromString(FString::Printf(TEXT("%s(%d)"), *contentText.ToString(), m_errorCode));

		auto* MsgPopup = UIMgr->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
			titleText,
			contentText,
			0.f,
			true,
			true,
			group,
			OnBtnPosClick,
			0,
			true,
			PopupPriority
			);
	}
}

void FBladeIIBlockToSyncNetwork::SessionDisconnetFromServer(int32 _reasonCode)
{
	FText title, context;
	ESessionClosedNotifyReason closeReason(static_cast<ESessionClosedNotifyReason>(_reasonCode));
	switch (closeReason)
	{
		case ESessionClosedNotifyReason::ECLOSEREASON_InvalidAccountInformation:
		{
			title = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification"));
			context = BladeIIGetLOCText(B2LOC_CAT_ERROR_CODE, FString::FromInt(B2ResultCode::FAILURE_UPDATE_ACCOUNT_BATTLE_STATUS));
		}
		break;
		// 2: �ߺ� ���� (���� �������� �������� SignIn)
		case ESessionClosedNotifyReason::ECLOSEREASON_MultipleAcountAccess:
		{
			title = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification"));
			context = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("MultipleAcountAccess"));
		}
		break;
	}

	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	if (UIMgr)
	{
		UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
			title,
			context,
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm,
			FMsgPopupOnClick::CreateLambda([this]()
			{
				GoToDLCMap();
			}
			)
			, 0
			, false
			, EPopUpPriority::Server_Message_DLCMap
		);
	}
	else
	{
		GoToDLCMap();
	}
}

void FBladeIIBlockToSyncNetwork::SetFree()
{
	if (RetryRequest != nullptr)
	{
		//SetValueByDefault();
		//RetryRequest = nullptr;
		//CloseBlockToSyncNetworkUI();

		//UB2UIManager* UIMgr = UB2UIManager::GetInstance();
		//if (UIMgr)
		//{
		//	UIMgr->CloseMsgPopup(RetryPopupUIMsgID);
		//}
	}
}

void FBladeIIBlockToSyncNetwork::OpenBlockToSyncNetworkUI()
{
	if (IsOpenBlockToSyncUI())
	{
		//LobbyShowNetworkLoadingIconClass<bool>::GetInstance().Signal(true);
	}
}

void FBladeIIBlockToSyncNetwork::CloseBlockToSyncNetworkUI()
{
	//LobbyShowNetworkLoadingIconClass<bool>::GetInstance().Signal(false);
}

void FBladeIIBlockToSyncNetwork::OpenNetworkConnectionFailUI()
{
	CloseBlockToSyncNetworkUI();

	//if (GetIsRequestMasterData())
	//{
	//	SetIsRequestMasterData(false);
	//	FailResponseMasterDataClass<>::GetInstance().Signal();
	//}
}

void FBladeIIBlockToSyncNetwork::OpenChannelConnectionFailUI()
{
//	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
//	if (UIMgr
//		&& UIMgr->GetCurrUIScene() != EUIScene::Title
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		&& !bIsDevStandaloneMode
//#endif
//		)
//	{
//		EUIMsgPopupButtonGroup group = EUIMsgPopupButtonGroup::ReconnectOrGoToTitle;
//		FText titleText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Network_Error"));
//		FText contentText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Network_ChannelDisconnect"));
//
//		//[@AKI, 180717][todo]
//		//FText -> FString is dangerous as it is a potentially lossy conversion for some languages. https://docs.unrealengine.com/en-us/Programming/UnrealArchitecture/StringHandling/FText
//		if (m_errorCode > -1)
//			contentText = FText::FromString(FString::Printf(TEXT("%s(%d)"), *contentText.ToString(), m_errorCode));
//
//		FMsgPopupOnClick OnBtnPosClick = FMsgPopupOnClick::CreateLambda([this]()
//		{
//			if (FB2NetworkBridge::GetInstance()->IsServiceAvailable())
//			{
//				GoToLobby();
//			}
//			else
//			{
//				GoToTitle();
//			}
//		});
//		
//		auto* MsgPopup = UIMgr->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
//			titleText,
//			contentText,
//			0.f,
//			true,
//			true,
//			group,
//			OnBtnPosClick,
//			0,
//			true,
//			EPopUpPriority::Server_Message_GoToLobby
//			);
//
//		if (MsgPopup)
//			RetryPopupUIMsgID = MsgPopup->GetMsgID();
//	}
//
//	CloseBlockToSyncNetworkUI();
}

void FBladeIIBlockToSyncNetwork::Retry()
{
	FB2NetworkBridge::GetInstance()->Shutdown();
	FB2NetworkBridge::GetInstance()->Init();

	if (!BladeIIGameImpl::GetClientDataStore().ExistAccountInfo())
	{
		GoToTitle();
	}
	else
	{
		SetBlockTillResponse(RetryRequest);
	}
	
	SetValueByLimitTime();
	RetryCount++;
}

void FBladeIIBlockToSyncNetwork::GoToTitle()
{
	//[@AKI, 180129] Version���� request������ �������� �ű�
	//GoToTitleClass Lamda���� �Ϸ��� ������ ����ϴ� ���� ���Ƽ� �����ϰ� ��Ʈ��ũ Ŀ�ؼ��� ���������� �ϰ� ��
	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	if (UIMgr)
	{
		UIMgr->MasterDestroy(); // �׳� Destroy ���� MasterDestroy �� �ؾ� ��.
		UIMgr->ResetRepeatBattle();

		//[@AKI, 180711] AB2MatchGameModeBase::ShutdownChannelIfStillExist() �� ���� ���� ��.
		//Request�� ���� ��
		//ChanelManager���� ���� �ϴ� ������ ������ ���߾���.
		AB2NetGameMode* B2GM = Cast<AB2NetGameMode>(GetBladeIIGameMode(UIMgr));
		if (B2GM)
		{
			const FString roomID(B2GM->GetRoomId());
			if (roomID != "" || !roomID.IsEmpty())
			{
				TArray<FString> Erase;
				Erase.Add(roomID);

				data_trader::ChannelManager::GetInstance().ShutdownChannel(Erase);

				B2GM->SetRoomId(TEXT(""));
			}
		}
	}

	SetValueByDefault();

	//[@AKI, 180629][B2BUG-1494] //�ݺ������� �� ��Ʈ��ũ�� ���� Ÿ��Ʋ�̳� DLCFrontMap�� ���ư� �� ���� ���Խ� �ݺ������� ��� �Ǵ� ���� ����
	//[TO-DO] �� ��Ʈ��ũ ��Ȳ �� Ŭ���� ���� �����͸� ��� ���� ���� ������ ��!!
	CancelOrStopRepeatBattleClass<>::GetInstance().Signal();
	GoToTitleClass<>::GetInstance().Signal();
}

void FBladeIIBlockToSyncNetwork::GoToLobby() //For Channel Server like PVP
{
	SetValueByDefault();
	
	//[@AKI, 180711] AB2MatchGameModeBase::ShutdownChannelIfStillExist() �� ���� ���� ��.
	//Request�� ���� ��
	//ChanelManager���� ���� �ϴ� ������ ������ ���߾���.
	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	if (UIMgr)
	{
		UIMgr->ResetRepeatBattle();

		AB2NetGameMode* B2GM = Cast<AB2NetGameMode>(GetBladeIIGameMode(UIMgr));
		if (B2GM)
		{
			const FString roomID(B2GM->GetRoomId());
			if (roomID != "" || !roomID.IsEmpty())
			{
				TArray<FString> Erase;
				Erase.Add(roomID);

				data_trader::ChannelManager::GetInstance().ShutdownChannel(Erase);

				B2GM->SetRoomId(TEXT(""));
			}
		}
	}

	GoToVillageClass<>::GetInstance().Signal();
}

void FBladeIIBlockToSyncNetwork::GoToDLCMap()
{
	SetValueByDefault();

	//[@AKI, 180711] AB2MatchGameModeBase::ShutdownChannelIfStillExist() �� ���� ���� ��.
	//Request�� ���� ��
	//ChanelManager���� ���� �ϴ� ������ ������ ���߾���.
	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	if (UIMgr)
	{
		UIMgr->ResetRepeatBattle();

		AB2NetGameMode* B2GM = Cast<AB2NetGameMode>(GetBladeIIGameMode(UIMgr));
		if (B2GM)
		{
			const FString roomID(B2GM->GetRoomId());
			if (roomID != "" || !roomID.IsEmpty())
			{
				TArray<FString> Erase;
				Erase.Add(roomID);

				data_trader::ChannelManager::GetInstance().ShutdownChannel(Erase);

				B2GM->SetRoomId(TEXT(""));
			}
		}
	}

	GoToDLCMapClass<>::GetInstance().Signal();
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
void FBladeIIBlockToSyncNetwork::DevSetStandaloneMode(bool bInStandalone)
{
	bIsDevStandaloneMode = bInStandalone;
	if (bIsDevStandaloneMode) {
		SetFree();
	}
}
#endif

bool FBladeIIBlockToSyncNetwork::IsConnectedToServer()
{
	return Utilities::FConnectionState::GetInstance().IsConnected();
}

bool FBladeIIBlockToSyncNetwork::IsOpenBlockToSyncUI() const
{
//	if (!EnableBlockToSync
//		|| UB2UIManager::GetInstance()->IsPopupPriority(EPopUpPriority::Server_Message_DLCMap)
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		|| bIsDevStandaloneMode
//#endif
//		)
//	{
//		return false;
//	}

	return true;
}

bool FBladeIIBlockToSyncNetwork::IsEnableRequest(std::function<void(void)> RequestFunc)
{
	if (EnableBlockToSync == false
		// || RequestFunc == nullptr // try-catch�� �ʹ� ��Ȯ�ϰ� ��� ���� �ּ� ó�� ��
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
		|| bIsDevStandaloneMode
#endif // BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
		// RetryRequest.target_type().hash_code() == RequestFunc.target_type().hash_code()
		)
	{
		return false;
	}

	return true;
}

void FBladeIIBlockToSyncNetwork::SetValueByLimitTime()
{
	BlockingLimitTime = LimitTime;
}