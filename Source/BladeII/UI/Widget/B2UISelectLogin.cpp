#include "BladeII.h"
#include "B2UISelectLogin.h"
#include "B2UIManager.h"


#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"

//extern void AndroidThunkCpp_SetCharacterInfoScene(bool bSet);
//extern void AndroidThunkCpp_FacebookLogin();
#endif

UB2UISelectLogin::UB2UISelectLogin(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UB2UISelectLogin::Init()
{
	Super::Init();

#if PLATFORM_IOS
// 	IOnlineSubsystem *FactbookPtr = IOnlineSubsystem::Get(TEXT("Facebook"));
// 	if (FactbookPtr)
// 	{
// 		FactbookPtr->Init();
// 		FactbookPtr->GetIdentityInterface()->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateUObject(this, &UB2UISelectLogin::ReturnFacebookAuthToken));
// 	}
#endif 
}

void UB2UISelectLogin::CacheAssets()
{
	GET_SLOT(UButton, Facebook_btn)
	GET_SLOT(UButton, Google_btn)
	GET_SLOT(UButton, Guest_btn)
	GET_SLOT(UButton, Exit_btn)
	GET_SLOT(UButton, GameCenter_btn)

#if PLATFORM_IOS
	if (GameCenter_btn.IsValid())
		GameCenter_btn->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (Google_btn.IsValid())
		Google_btn->SetVisibility(ESlateVisibility::Hidden);
#else	// Android And PC
	if (GameCenter_btn.IsValid())
		GameCenter_btn->SetVisibility(ESlateVisibility::Hidden);
	if (Google_btn.IsValid())
		Google_btn->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
#endif // PLATFORM_IOS
}


void UB2UISelectLogin::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(Facebook_btn, &UB2UISelectLogin::OnclickBtnFacebook)
	BIND_CLICK_FUNC_TO_BTN(Google_btn, &UB2UISelectLogin::OnclickBtnGoogle)
	BIND_CLICK_FUNC_TO_BTN(Guest_btn, &UB2UISelectLogin::OnclickBtnGuest)
	BIND_CLICK_FUNC_TO_BTN(Exit_btn, &UB2UISelectLogin::OnclickBtnExit)
	BIND_CLICK_FUNC_TO_BTN(GameCenter_btn, &UB2UISelectLogin::OnclickBtnGameCenter)
}

void UB2UISelectLogin::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}


//====================================================================================
//									Click Handler
//====================================================================================
void UB2UISelectLogin::OnclickBtnFacebook()
{
#if PLATFORM_ANDROID
	//AndroidThunkCpp_FacebookLogin();
	//AndroidThunkCpp_SetCharacterInfoScene(true);
#elif PLATFORM_IOS
// 	IOnlineSubsystem *FactbookPtr = IOnlineSubsystem::Get(TEXT("Facebook"));
// 	if (FactbookPtr)
// 	{
// 		APlayerController* LocalPlayerController = UGameplayStatics::GetLocalPlayerController(GetWorld());
// 		ULocalPlayer* LP = LocalPlayerController ? LocalPlayerController->GetLocalPlayer() : nullptr;
// 
// 		if(LP)
// 			FactbookPtr->GetIdentityInterface()->Login(LP->GetControllerId(), FOnlineAccountCredentials());
// 	}
#else
	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_CharacterIntro);
	UB2UIManager::GetInstance()->CloseUI(UIFName::SelectLogin);
#endif

}

void UB2UISelectLogin::OnclickBtnGoogle()
{
	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_CharacterIntro);
	UB2UIManager::GetInstance()->CloseUI(UIFName::SelectLogin);
}

void UB2UISelectLogin::OnclickBtnGuest()
{
	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_CharacterIntro);
	UB2UIManager::GetInstance()->CloseUI(UIFName::SelectLogin);
}

void UB2UISelectLogin::OnclickBtnExit()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::SelectLogin);
}

void UB2UISelectLogin::OnclickBtnGameCenter()
{
#if PLATFORM_IOS
// 	IOnlineSubsystem *GameCenterPtr = IOnlineSubsystem::GetByPlatform();
// 	if (GameCenterPtr)
// 	{
// 		IOnlineExternalUIPtr OnlineExternalUI = GameCenterPtr->GetExternalUIInterface();
// 		if (OnlineExternalUI.IsValid())
// 		{
// 			APlayerController* LocalPlayerController = UGameplayStatics::GetLocalPlayerController(GetWorld());
// 			ULocalPlayer* LP = LocalPlayerController ? LocalPlayerController->GetLocalPlayer() : nullptr;
// 			const bool bWaitForDelegate = OnlineExternalUI->ShowLoginUI(LP->GetControllerId(), false,
// 				FOnLoginUIClosedDelegate::CreateUObject(this, &UB2UISelectLogin::ReturnGameCenterAuthToken));
// 
// 			if (!bWaitForDelegate)
// 			{
// 				// fail
// 				TSharedPtr<FUniqueNetIdString> UniqueNetId;
// 				ReturnGameCenterAuthToken(UniqueNetId, 0);
// 			}
// 
// 		}
// 	}
#endif // PLATFORM_IOS
}

#if PLATFORM_IOS

void UB2UISelectLogin::ReturnFacebookAuthToken(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
// 	if (bWasSuccessful && UserId.IsValid())
// 	{	
// 
// 	IOnlineSubsystem *FactbookPtr = IOnlineSubsystem::Get(TEXT("Facebook"));
// 	if (FactbookPtr)
// 	{
// 		APlayerController* LocalPlayerController = UGameplayStatics::GetLocalPlayerController(GetWorld());
// 		ULocalPlayer* LP = LocalPlayerController ? LocalPlayerController->GetLocalPlayer() : nullptr;
// 
// 		if (LP)
// 			FactbookPtr->GetIdentityInterface()->Logout(LP->GetControllerId());// Login(LP->GetControllerId(), FOnlineAccountCredentials());
// 	}
// 
// 		//OnSuccess
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Facebook OnSuccess: %s"), *UserId.GetHexEncodedString()));
// 	}
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Facebook OnFailure"));
// 		//OnFailure
// 	}
}

void UB2UISelectLogin::ReturnGameCenterAuthToken(TSharedPtr<const FUniqueNetId> UniqueId, int LocalUserNum)
{
// 	APlayerController* LocalPlayerController = UGameplayStatics::GetLocalPlayerController(GetWorld());
// 	ULocalPlayer* LocalPlayer = LocalPlayerController ? LocalPlayerController->GetLocalPlayer() : nullptr;
// 
// 	if (LocalPlayer != nullptr)
// 	{
// 		LocalPlayer->SetCachedUniqueNetId(UniqueId);
// 
// 		if (LocalPlayerController && LocalPlayerController->PlayerState != nullptr)
// 		{
// 			LocalPlayerController->PlayerState->SetUniqueId(UniqueId);
// 		}
// 	}
// 
// 	if (UniqueId.IsValid())
// 	{
// 		//OnSuccess
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GameCenter OnSuccess: %s"), *UniqueId->GetHexEncodedString()));
// 	}
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GameCenter OnFailure"));
// 		//OnFailure
// 	}
}
 
#endif // PLATFORM_IOS
