// Fill out your copyright notice in the Description page of Project Settings.
#include "B2AndroidBackManager.h"
//#include "BladeII.h"

#include "B2StageGameMode.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIClientIndicator.h"
#include "B2PlatformAPI.h"
#include "B2UIBackWidget.h"
#include "B2UIBattleMain.h"
#include "Global.h"

B2AndroidBackManager::B2AndroidBackManager()
{
	HeaderWidget = NULL;
	BattleMainWidget = NULL;
	CombineMenuWidget = NULL;
	//FGenericPlatformMisc::SetAndroidBackEvent([this]() { this->OnClickBackButton(); });

	SubscribeEvents();
}

B2AndroidBackManager::B2AndroidBackManager(const B2AndroidBackManager& other)
	: B2AndroidBackManager()
{ 
}

B2AndroidBackManager::~B2AndroidBackManager()
{
	HeaderWidget = NULL;
	BattleMainWidget = NULL;
	CombineMenuWidget = NULL;
	//FGenericPlatformMisc::SetAndroidBackEvent(nullptr);

	UnsubscribeEvents();
}

B2AndroidBackManager* B2AndroidBackManager::GetInstance()
{
	static B2AndroidBackManager AndroidBackManager;
	return &AndroidBackManager;
}

void B2AndroidBackManager::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(PlatformPausePressClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnPlatformPausePress();
	}
	));
}

void B2AndroidBackManager::UnsubscribeEvents()
{
	Issues.Empty();
}

void B2AndroidBackManager::OnPlatformPausePress()
{
	switch (GetAndroidBackState())
	{
	case EAndroidBackState::CLOSEBACKWIDGET:
	case EAndroidBackState::CLOSEBATTLEMAIN:
	{
		if (BattleMainWidget != NULL && BattleMainWidget.IsValid())
		{
			// [모험모드] 중 "반복전투" 및 "연속전투" 일 경우 시스템(FJavaWrapper::OnPlatformResumeDelegate) 을 통해 들어온 멈춤메뉴의 경우 처리 하지 않는다
			if ( AB2StageGameMode* StageGameMode = Cast< AB2StageGameMode >( UGameplayStatics::GetGameMode( GWorld ) ) )
			{
				if ( StageGameMode->IsRepeatBattleOn() )
				{
					return;
				}
			}

			IB2UIBackWidget* BackWidget = Cast<IB2UIBackWidget>(BattleMainWidget.Get());
			if (BackWidget)
			{
				BackWidget->OpenBattleMainPause();
			}
		}
		break;
	}
	default:
		break;
	}
}

void B2AndroidBackManager::OnClickBackButton()
{
	B2_SCOPED_TRACK_LOG(TEXT("B2AndroidBackManager::OnClickBackButton"));
//
//	if (GetMoviePlayer())
//	{
//		if (GetMoviePlayer()->IsLoadingScreenCurrentlyPlaying() == false)
//		{
//			switch (GetAndroidBackState())
//			{
//			case EAndroidBackState::INDICATOR:
//			{
//				break;
//			}
//			case EAndroidBackState::CLOSEBACKWIDGET:
//			{
//				TWeakObjectPtr<UObject> LastWidget = NULL;
//				int LastIndex = BackWidgetHistory.Num() - 1;
//
//				if (BackWidgetHistory.IsValidIndex(LastIndex))
//				{
//					LastWidget = BackWidgetHistory[LastIndex];
//					if (LastWidget.IsValid())
//					{
//						IB2UIBackWidget* BackWidget = Cast<IB2UIBackWidget>(LastWidget.Get());
//						if (BackWidget)
//						{
//							BackWidget->CloseWidget();
//						}
//					}
//				}
//				break;
//			}
//			case EAndroidBackState::CLOSEHEADER:
//			{
//				if (HeaderWidget != NULL && HeaderWidget.IsValid())
//				{
//					IB2UIBackWidget* BackWidget = Cast<IB2UIBackWidget>(HeaderWidget.Get());
//					if (BackWidget)
//					{
//						BackWidget->CloseWidget();
//					}
//				}
//				break;
//			}
//			case EAndroidBackState::CLOSEBATTLEMAIN:
//			{
//				if (BattleMainWidget != NULL && BattleMainWidget.IsValid())
//				{
//					IB2UIBackWidget* BackWidget = Cast<IB2UIBackWidget>(BattleMainWidget.Get());
//					if (BackWidget)
//					{
//						BackWidget->CloseWidget();
//					}
//				}
//				break;
//			}
//			case EAndroidBackState::CLOSECOMBINEMENU:
//			{
//				if (CombineMenuWidget != NULL && CombineMenuWidget.IsValid())
//				{
//					IB2UIBackWidget* BackWidget = Cast<IB2UIBackWidget>(CombineMenuWidget.Get());
//					if (BackWidget)
//					{
//						BackWidget->CloseWidget();
//					}
//				}
//				break;
//			}
//			case EAndroidBackState::EXIT:
//			{
//				// 아래 EndingPromotion 을 통해 종료를 하는 경우 정상적인 엔진 종료 루트를 통하지는 않게 된다. 로컬 ini 라도 저장.
//				SaveLocalIniStateOnAppBecomeInactive();
//
//				//B2P_ShowKakaoPromotionView();
//				//B2P_GameExitPopUp("byebye");
//				B2P_ShowEndingPromotion();
//				break;
//			}
//			default:
//				break;
//			}
//		}
//#if !UE_BUILD_SHIPPING
//		else
//		{
//			UE_LOG(LogBladeII, Log, TEXT("Skip B2AndroidBackManager::OnClickBackButton handling, as we are currently under loading screen."))
//		}
//#endif
//	}
}

void B2AndroidBackManager::AddBackWidget(UObject* Value, FName UIName)
{
	switch (GetBackWidgetType(UIName))
	{
	case EBackWidgetType::BACKWIDGET:
		BackWidgetHistory.Add(Value);
		break;
	case EBackWidgetType::HEADER:
		HeaderWidget = Value;
		break;
	case EBackWidgetType::BATTLEMAIN:
		BattleMainWidget = Value;
		break;
	case EBackWidgetType::COMBINEMENU:
		CombineMenuWidget = Value;
		break;
	}

	//if (BackWidgetHistory.Num() > 0 || CombineMenuWidget != NULL)
	//{
	//	SetPopupOpen(true);
	//}
	//else
	//{
	//	SetPopupOpen(false);
	//}
	
}

void B2AndroidBackManager::RemoveBackWidget(UObject* Value, FName UIName)
{
	switch (GetBackWidgetType(UIName))
	{
	case EBackWidgetType::BACKWIDGET:
		BackWidgetHistory.Remove(Value);
		break;
	case EBackWidgetType::HEADER:
		if (Value == HeaderWidget)
		{
			HeaderWidget = NULL;
		}
		break;
	default:
		RemoveBackWidget(UIName);
		break;
	}

	//if (BackWidgetHistory.Num() > 0 || CombineMenuWidget != NULL)
	//{
	//	SetPopupOpen(true);
	//}
	//else
	//{
	//	SetPopupOpen(false);
	//}
}

void B2AndroidBackManager::RemoveBackWidget(FName UIName)
{
	switch (GetBackWidgetType(UIName))
	{
	case EBackWidgetType::BATTLEMAIN:
		BattleMainWidget = NULL;
		break;
	case EBackWidgetType::COMBINEMENU:
		CombineMenuWidget = NULL;
		break;
	}

	//if (BackWidgetHistory.Num() > 0 || CombineMenuWidget != NULL)
	//{
	//	SetPopupOpen(true);
	//}
	//else
	//{
	//	SetPopupOpen(false);
	//}
}

void B2AndroidBackManager::RemoveAllPopup()
{
	BackWidgetHistory.Empty();
	SetPopupOpen(false);
}

EAndroidBackState B2AndroidBackManager::GetAndroidBackState()
{
	UB2UIManager* UIManager = UB2UIManager::GetInstance();
	if (UIManager)
	{
		// 인디게이터 체크
		bool IsNetworkIndicator = UIManager->GetIndicatorVisible() == true;
		bool IsLoopIndicator = UIManager->GetUI<UB2UIClientIndicator>(UIFName::ClientIndicator) != nullptr;
		if (IsNetworkIndicator || IsLoopIndicator)
		{
			return EAndroidBackState::INDICATOR;
		}
	}

	BackWidgetHistory.RemoveAll([](TWeakObjectPtr<UObject> Val) {
		return Val.IsValid() == false;
	});

	if (BackWidgetHistory.Num() > 0)
	{
		return EAndroidBackState::CLOSEBACKWIDGET;
	}

	if (CombineMenuWidget != NULL)
	{
		return EAndroidBackState::CLOSECOMBINEMENU;
	}

	if (UIManager && UIManager->GetCurrUIScene() == EUIScene::LobbyMain)
	{
		return EAndroidBackState::EXIT;
	}

	if (HeaderWidget != NULL)
	{
		return EAndroidBackState::CLOSEHEADER;
	}

	if (BattleMainWidget != NULL)
	{
		return EAndroidBackState::CLOSEBATTLEMAIN;
	}

	return EAndroidBackState::NONE;
}

EBackWidgetType B2AndroidBackManager::GetBackWidgetType(FName& UIName)
{
	EBackWidgetType Result = EBackWidgetType::BACKWIDGET;
	
	if (
		UIName.Compare(UIFName::BattleMain) == 0|| 
		UIName.Compare(UIFName::PVPOneOnOneBattleMain) == 0 ||
		UIName.Compare(UIFName::CounterDGBattleMain) == 0 ||
		UIName.Compare(UIFName::TeamMatchBattle) == 0 ||
		UIName.Compare(UIFName::RaidBattle) == 0 ||
		UIName.Compare(UIFName::TutorialBattleMain) == 0||
		UIName.Compare(UIFName::GuildMapUI) == 0 ||
		UIName.Compare(UIFName::GuildMatchBattle) == 0
		)
	{
		Result = EBackWidgetType::BATTLEMAIN;
	}
	else if (UIName.Compare(UIFName::Header) == 0)
	{
		Result = EBackWidgetType::HEADER;
	}
	else if (UIName.Compare(FName(TEXT("CombineMenu"))) == 0)
	{
		Result = EBackWidgetType::COMBINEMENU;
	}
	
	return Result;
}

bool B2AndroidBackManager::HasHaderUI()
{
	return HeaderWidget != NULL ? true : false;
}