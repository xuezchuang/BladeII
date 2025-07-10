// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIActionLogo.h"

#include "B2UIManager.h"

#include "B2PlatformAPI.h"
#include "B2DLCFrontGameMode.h"
#if PLATFORM_ANDROID
#include "AndroidRuntimeSettings.h"
#include "Android/AndroidJNI.h"
#endif


bool m_bGoTitle;
void UB2UIActionLogo::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Image, B2Image_0);
	GET_SLOT(UB2UICheckPermission, UI_CheckPermission)
		if (UI_CheckPermission.IsValid()) {
			UI_CheckPermission->Init();
		}

	GET_SLOT(UB2UIDLCFrontPopup, UI_MsgPopup01)
		if (UI_MsgPopup01.IsValid()) {
			UI_MsgPopup01->Init();
			UI_MsgPopup01->SetVisibility(ESlateVisibility::Hidden);
		}

	UI_CheckPermission->SetVisibility(ESlateVisibility::Hidden);
	
}

void UB2UIActionLogo::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIActionLogo::Init()
{
	Super::Init();
	m_PermissionDelegateCalled.AtomicSet(false);
	m_bGoTitle = false;
	m_bMovieTime = true;
#if PLATFORM_ANDROID 
	FJavaWrapper::OnCheckPermissionResultDelegate.Remove(OnCheckPermissionHandler);
	OnCheckPermissionHandler = FJavaWrapper::OnCheckPermissionResultDelegate.AddUObject(this, &UB2UIActionLogo::CheckPermissionDelegate);
#endif
}

//[@AKI, 170927][FIXME]Main Thread에서 호출되는 현상이 있어 임시 방편으로 변경 함.
void UB2UIActionLogo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_PermissionDelegateCalled && !m_bGoTitle)
	{
		auto* LoginGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this));
		if (LoginGameMode)
		{
			LoginGameMode->OpenUIScene(EDLCFrontUIName::EDLCUI_Title, true);
		}
		m_bGoTitle = true;
	}

	if(!m_bMovieTime)
		CheckMediaTime();
}

void UB2UIActionLogo::GotoPatchCheck()
{
	auto* LoginGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this));
	if (LoginGameMode)
	{
	//	LoginGameMode->OpenUIScene(EDLCFrontUIName::EDLCUI_Patch, true);

	/*	if (!B2P_CheckPermission())
		{
			//OpenPermissionNotice();
			B2P_InitPlatformPermission();
		}
		else
		{*/
			LoginGameMode->OpenUIScene(EDLCFrontUIName::EDLCUI_Title, true);
		//}
	}	
}

#if PLATFORM_ANDROID
void UB2UIActionLogo::CheckPermissionDelegate(JNIEnv *env, jobject thiz, jboolean bResult)
{
	if (bResult)
	{
		m_PermissionDelegateCalled.AtomicSet(true);
	}
	else
	{
		UI_MsgPopup01->SetVisibility(ESlateVisibility::Visible);
		UI_MsgPopup01->SetOwnerLogoUI(this);
		UI_MsgPopup01->SetPopupType(EPopupType::EPopupType_CheckPermission);
	}
	
}
#endif

void UB2UIActionLogo::OpenTitleMain()
{
	UI_CheckPermission->SetVisibility(ESlateVisibility::Hidden);
	B2P_InitPlatformPermission();
	
}

void UB2UIActionLogo::OpenPermissionNotice()
{
	UI_CheckPermission->SetVisibility(ESlateVisibility::Visible);
	UI_CheckPermission->SetOwnerUI(this);
}

void UB2UIActionLogo::RetryPlatformPermission()
{
	UI_MsgPopup01->SetVisibility(ESlateVisibility::Hidden);

	B2P_InitPlatformPermission();	
}

void UB2UIActionLogo::GameExit()
{
	UI_MsgPopup01->SetVisibility(ESlateVisibility::Hidden);
	B2P_GameExit();
}


bool UB2UIActionLogo::GetMediaTimeSet(float fTime)
{
	if (fTime > 0.8f)
	{
		UE_LOG(LogClass, Log, TEXT("####agwCheck OK 111111111111111111"));
		m_bMovieTime = true;
		return true;
	}
	UE_LOG(LogClass, Log, TEXT("####agwCheck OK 222222222222222222!"));
	return false;
}

void UB2UIActionLogo::SetMovieStart()
{
	m_bMovieTime = false;
}

bool UB2UIActionLogo::IsTablet()
{
	
	FVector2D ScreenSize(1.f, 1.f);
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient != nullptr)
		ViewportClient->GetViewportSize(ScreenSize);		
	UE_LOG(LogClass, Log, TEXT("UB2UIActionLogo :: IsTablet SizeX : %f"), ScreenSize.X);
	UE_LOG(LogClass, Log, TEXT("UB2UIActionLogo :: IsTablet SizeY : %f"), ScreenSize.Y);
	
	if (ScreenSize.X * 0.75 == ScreenSize.Y)
		return true;
	else
		return false;
}


