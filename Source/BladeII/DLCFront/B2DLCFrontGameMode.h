// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
//#include "BladeII.h"
#include "GameFramework/GameMode.h"
#include "CommonStruct.h"
//#include "BladeII.h"
#include "B2DLCFrontGameMode.generated.h"

UENUM(BlueprintType)
enum class EDLCFrontUIName : uint8
{
	EDLCUI_Begin = 0,
	EDLCUI_Main,
	EDLCUI_Patch,
	EDLCUI_ActionLogo,
	EDLCUI_MsgPopup,
	EDLCUI_AccessTerms,
	EDLCUI_Title,

	EDLCUI_End
};

/** It defines UI data set for DLCFrontGameMode, like FB2UIWidgetData of B2UIManager, but must be simpler */
USTRUCT()
struct FDLCFrontUIData
{
	GENERATED_USTRUCT_BODY()

	FDLCFrontUIData()
	{
		WidgetName = EDLCFrontUIName::EDLCUI_End;
		WidgetClass = NULL;
		CreateZOrder = 0;
	}

	UPROPERTY(VisibleAnywhere)
	EDLCFrontUIName WidgetName; // Not editable, it will come from its TMap container key.

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UB2UIDLCFrontBase> WidgetClass; // Mapped WidgetClass data.

	UPROPERTY(EditAnywhere)
	int32 CreateZOrder;
};

/**
 * Provided for simple resource reference of DLCFrontGameMode.
 * Just place resource reference here (most are expected to be Widget class) 
 * Doesn't matter if they are hard reference (i.e. no need for lazy loading)
 */
USTRUCT()
struct FDLCFrontAssetRef
{
	GENERATED_USTRUCT_BODY()

	FDLCFrontAssetRef()
	{
	}

	UPROPERTY(EditAnywhere)
	TMap<EDLCFrontUIName, FDLCFrontUIData> AllUIData;



	// 
	// Put any other specially required resources here.
	// 


};

/**
 * For minimal DLC front-end implementation.
 * No BladeII InfoAsset is supposed to be loaded with this GameMode.
 */
UCLASS(config = game)
class BLADEII_API AB2DLCFrontGameMode : public AGameMode
{
	GENERATED_BODY()

protected:

	/** Assets for DLCFrontGameMode */
	UPROPERTY(EditAnywhere)
	FDLCFrontAssetRef MyAssetRef;


	UPROPERTY(Transient)
	TMap<EDLCFrontUIName, class UB2UIDLCFrontBase*> AllCreatedUI;

	/** Cached front title movie file name, to extend it to lobby loading screen.
	 * Must be without extension. 
	 * Might need access this outside of DLCFrontGameMode (Lobby) so it is declared as static */
	static FString SelectedFrontTitleMovieName;
	static EPCClass SelectedFrontTitleMovieClass;

public:
	AB2DLCFrontGameMode(const FObjectInitializer& ObjectInitializer);
	
	virtual void PostLoad() override;
	virtual void StartPlay() override;
protected:
	void PreBeginPlay();
public:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Destroyed() override;
protected:
	void OnDestroyCommon();
	void OnPreLoadMap(const FString& InString);

	uint32 bEventsSubscribed : 1; // To un-subscribe only in subscribed GameMode object.
	void SubscribeEvents(); // Events of EventSubsystem
	void UnsubscribeEvents();
	
	///// Simple UI manager functionality
	/** Open UI registered by FDLCFrontUIData */
	class UB2UIDLCFrontBase* OpenUI(EDLCFrontUIName InUIName);
	void CloseUI(EDLCFrontUIName InUIName);
	void CloseAllUIs();
	class UB2UIDLCFrontBase* GetUI(EDLCFrontUIName InUIName); // Return widget ref if created.
	///// 

	void SetupLoadingScreen_Lobby();

	void LaunchBladeIIGame(); // Leave this front and go ahead to Lobby.


	//>> Event tickets
private:
	uint32 DLCFrontLaunchBladeIIGameTicket;

	
	//<< Event tickets

public:
	void GmaeModeGotoTitle();
	void OpenUIScene(EDLCFrontUIName InUIName, bool bCloseOtherUIs = false);

	void SetSelectedFrontTitleMovieInfo(const FString& InMovieName, EPCClass InMovieRepresentingClass);
	static void ClearSelectedFrontTitleMovieInfo();
	static FString& GetSelectedFrontTitleMovieName() { return SelectedFrontTitleMovieName; }
	static EPCClass GetSelectedFrontTitleMovieClass() { return SelectedFrontTitleMovieClass; }
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	static void DevSetFrontTitleMovieClassOnly(EPCClass InClass);
#endif

	/** If you call this to setup the loading screen at PreLoadMap timing, 
	 * you will see the DLC front title movie shown at startup as the loading screen. */
	static void TrySetupLoadingScreenWithInitialTitleMovie(UObject* WorldContextObject);
	
	void ShowDisconnectNetworkPopup();

	void ShowKakaoErrorPopup(int32 ErrorCode);

#if WITH_EDITOR
public:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
protected:
	void EditorCheckForWidgetNameIntegrity();
#endif
};
