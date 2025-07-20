#pragma once

#include "BladeII.h"
#include "EngineMinimal.h"
#include "CommonStruct.h"
//#include "Tickable.h"

//#include "B2UIWidget.h"

UENUM()
enum class DisconnectResasonType : uint8
{
	EDISCONNECT_REASON_NONE,
	//荤角 Resopone档 Session捞 谗变 芭瘤父 
	//Resopone甫 给罐绰 巴苞 Session Disconnect啊 炼陛 促弗 狼固肺 静捞扁 锭巩俊 盒府
	EDISCONNECT_REASON_RESPONSE,
	EDISCONNECT_REASON_SESSION,
	EDISCONNECT_REASON_CHANNEL,
	EDISCONNECT_REASON_TOTAL,
};

enum class ESessionClosedNotifyReason : uint8
{
	ECLOSEREASON_InvalidAccountInformation = 1,
	ECLOSEREASON_MultipleAcountAccess = 2,
};

class FBladeIIBlockToSyncNetwork : public FTickableGameObject
{
public:
	FBladeIIBlockToSyncNetwork();
	~FBladeIIBlockToSyncNetwork();

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const { return true; }

	virtual bool IsTickableWhenPaused() const { return true; }
	virtual bool IsTickableInEditor() const { return true; }


	static FBladeIIBlockToSyncNetwork& GetInstance();

	virtual TStatId GetStatId() const override;
	bool GetEnableBlockToSync() const { return EnableBlockToSync; }

	void SetEnableBlockToSync(bool InEnable) { EnableBlockToSync = InEnable; }
	void SetBlockTillResponse(std::function<void(void)> RequestFunc);
	void SetFree();
	void SessionDisconnect(int32 _errorCode);
	void ChannelDisconnect(int32 _errorCode);
	void ServiceNotAvailable(int32 _errorCode);
	void SessionDisconnetFromServer(int32 _reasonCode); //捞 搬苞内靛绰 困狼 俊矾内靛客绰 促弗 俊矾内靛 烙
	void Retry();
	void GoToTitle();
	void GoToLobby(); //For PVP
	void GoToDLCMap();

	void SetIsRequestMasterData(bool InIsRequestMasterData) { IsRequestMasterData = InIsRequestMasterData; }
	bool GetIsRequestMasterData() const { return IsRequestMasterData; }

	void SetIsNetworkInit(bool IsInit) { IsNetworkInitialized = IsInit; }
	bool GetIsNetworkInit() const { return IsNetworkInitialized; }
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	void DevSetStandaloneMode(bool bInStandalone);
	FORCEINLINE bool DevIsStandaloneMode() const { return bIsDevStandaloneMode; }
#endif
	void SetValueByDefault();
//
	bool IsConnectedToServer();
	bool IsOpenBlockToSyncUI() const; 

	void OpenBlockToSyncNetworkUI();
	void CloseBlockToSyncNetworkUI();

private:
	void OpenNetworkConnectionFailUI();
	void OpenChannelConnectionFailUI();

	void UpdateBlockingLimitTime(float DeltaTime, std::function<void(void)> ConnectFailUIFunc);

	bool IsEnableRequest(std::function<void(void)> RequestFunc);
	void SetValueByLimitTime();

private:
	bool IsRequestMasterData;
	bool IsNetworkInitialized;
	bool EnableBlockToSync;

	float BlockingLimitTime;
	std::function<void(void)> RetryRequest;

	//TWeakObjectPtr<UB2UIWidget> BlockUIInstance;

	uint32 DeliveryClassTicket;

	bool bPauseForRetryPopup;

	int32 RetryPopupUIMsgID;
	int8 RetryCount;

	int32 m_errorCode;

	DisconnectResasonType m_DisconnectReason;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	bool bIsDevStandaloneMode;
#endif
};