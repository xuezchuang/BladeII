// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIStoreBanner.h"
#include "B2UILobbyBanner.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUICondition.h"
#include "BannerManager.h"
#include "B2GameInstance.h"
#include "B2UIAttendanceMain.h"
#include "Blueprint/AsyncTaskDownloadImage.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

UB2UILobbyBanner::UB2UILobbyBanner(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	WidgetScale = 1.f;
	AutoScrollTime = 0.5f;
	BannerImageWidth = 0.f;
	CurrentViewBannerIndex = 0;
	bInitSuccess = false;
	bAutoMove = true;
	LoadedErrorImage_Large = nullptr;
	LoadedErrorImage_Valid = nullptr;
}

void UB2UILobbyBanner::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_Banner_Back);
	GET_SLOT(UB2Button, BTN_Banner_Current);
	GET_SLOT(UB2Button, BTN_Banner_Forward);
	GET_SLOT(UB2ScrollBox, BannerScrollBox);
	GET_SLOT(UImage, IMG_BannerBackward);
	GET_SLOT(UImage, IMG_BannerCurrent);
	GET_SLOT(UImage, IMG_BannerForward);
	GET_SLOT(UHorizontalBox, HB_ButtonBox);
	GET_SLOT(UHorizontalBox, HB_BannerBox);
}

void UB2UILobbyBanner::BindDelegates()
{
	Super::BindDelegates();

	if (BannerScrollBox.IsValid())
	{
		BannerScrollBox->SetOnUserScrolled(FOnUserScrolled::CreateUObject(this, &UB2UILobbyBanner::OnBeginScroll));
		BannerScrollBox->SetOnUserScrolledEnd(FSimpleDelegate::CreateUObject(this, &UB2UILobbyBanner::OnEndScroll));
	}

	BIND_CLICK_FUNC_TO_BTN(BTN_Banner_Back, &UB2UILobbyBanner::OnClickBTN_Banner_Back);
	BIND_CLICK_FUNC_TO_BTN(BTN_Banner_Current, &UB2UILobbyBanner::OnClickBTN_Banner_Current);
	BIND_CLICK_FUNC_TO_BTN(BTN_Banner_Forward, &UB2UILobbyBanner::OnClickBTN_Banner_Forward);
}

void UB2UILobbyBanner::DestroySelf(UB2UIManager* InUIManager)
{
	LoadedErrorImage_Large = nullptr;
	LoadedErrorImage_Valid = nullptr;

	Super::DestroySelf(InUIManager);
}

float UB2UILobbyBanner::GetBannerWidth()
{
	return BannerImageWidth;
}

void UB2UILobbyBanner::Init()
{
	Super::Init();

	ImageDownloader = NewObject<UAsyncTaskDownloadImage>(this, NAME_None, RF_Transient);

	if (ImageDownloader)
	{
		ImageDownloader->OnSuccess.AddDynamic(this, &UB2UILobbyBanner::OnDownloadSucess);
		ImageDownloader->OnFail.AddDynamic(this, &UB2UILobbyBanner::OnDownloadFailed);
	}
	
	BannerImages.Empty();
	BannerButtons.Empty();

	TArray<B2RollingBannerPtr> LobbybannerInfo = BladeIIGameImpl::GetClientDataStore().GetLobbyBannerData();
	for (auto BannerItem : LobbybannerInfo)
	{
		FLobbyBannerData NewData = FLobbyBannerData(BannerItem->id,
			nullptr,
			BannerItem->banner_img_url,
			BannerItem->start_time,
			BannerItem->end_time,
			BannerItem->banner_duration,
			static_cast<EBannerLobbyToGo>(BannerItem->type),
			BannerItem->sub_type,
			EErrorImageType::NONE,
			BannerItem->web_url);

		FDateTime DateNow = UB2GameInstance::GetLocalNow();
		FDateTime StartTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(BannerItem->start_time));
		FDateTime EndTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(BannerItem->end_time));

		if (DateNow - StartTime > 0 && EndTime - DateNow > 0)
			BannerImages.Add(NewData);
	}

	OnDownloadStart();
}


void UB2UILobbyBanner::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//if(!bAutoMove) return;

	//WidgetScale = MyGeometry.Scale;

	//if (BannerScrollBox.IsValid())
	//{
	//	auto* OverlaySlot = Cast<UOverlaySlot>(BannerScrollBox->Slot);
	//	float NewSize = MyGeometry.GetLocalSize().X - OverlaySlot->Padding.Left - OverlaySlot->Padding.Right;
	//	if (NewSize != BannerImageWidth)
	//	{
	//		BannerImageWidth = NewSize;
	//		if (IMG_BannerForward.IsValid())
	//		{
	//			IMG_BannerForward->GetBrush().ImageSize.X = NewSize;
	//			IMG_BannerForward->GetBrush().ImageSize.Y = MyGeometry.GetLocalSize().Y - OverlaySlot->Padding.Top - OverlaySlot->Padding.Bottom;
	//		}
	//		if (IMG_BannerCurrent.IsValid())
	//		{
	//			IMG_BannerCurrent->GetBrush().ImageSize.X = NewSize;
	//			IMG_BannerCurrent->GetBrush().ImageSize.Y = MyGeometry.GetLocalSize().Y - OverlaySlot->Padding.Top - OverlaySlot->Padding.Bottom;
	//		}
	//		if (IMG_BannerBackward.IsValid())
	//		{
	//			IMG_BannerBackward->GetBrush().ImageSize.X = NewSize;
	//			IMG_BannerBackward->GetBrush().ImageSize.Y = MyGeometry.GetLocalSize().Y - OverlaySlot->Padding.Top - OverlaySlot->Padding.Bottom;
	//		}
	//	}
	//}

	//Super::NativeTick(MyGeometry, InDeltaTime);

	//if (!bInitSuccess) return;

	//if (BannerImages.Num() > 1)
	//{
	//	if (BannerScrollBox.IsValid() && !BannerScrollBox->IsRightClickScrolling())
	//	{
	//		if (DesiredPosition != BannerScrollBox->GetScrollOffset())
	//		{
	//			AutoScroll(InDeltaTime);
	//			ElapsedNextBannerScrollTime = 0.f;
	//		}
	//		else
	//		{
	//			ElapsedNextBannerScrollTime += InDeltaTime;
	//			if (ElapsedNextBannerScrollTime > BannerImages[CurrentViewBannerIndex].NextBannerScrollTime)
	//			{
	//				ShowBanner(CurrentViewBannerIndex + 1, false);
	//				ElapsedNextBannerScrollTime = 0.f;
	//			}
	//		}
	//	}
	//}
}

void UB2UILobbyBanner::AutoScroll(float InDeltaTime)
{
	if (!BannerScrollBox.IsValid())
		return;

	float CurrentPosition = BannerScrollBox->GetScrollOffset();
	bool bForwardDirection = CurrentPosition < DesiredPosition;

	float Velocity = GetBannerWidth() / AutoScrollTime;
	float TickMoveDelta = FMath::Clamp(Velocity * InDeltaTime, 0.f, FMath::Abs(DesiredPosition - CurrentPosition));

	BannerScrollBox->SetScrollOffset(bForwardDirection ? CurrentPosition + TickMoveDelta : CurrentPosition - TickMoveDelta);
}

void UB2UILobbyBanner::ShowBanner(int32 ToShowIndex, bool bForce)
{
	if (!BannerScrollBox.IsValid())
		return;

	if (BannerImages.Num() < 1)
		return;

	if (ToShowIndex < 0)
		ToShowIndex = BannerImages.Num() - 1;

	ChangeBanner(ToShowIndex,bForce);

	if (bForce)
	{
		BannerScrollBox->SetScrollOffset(DesiredPosition);
		ElapsedNextBannerScrollTime = 0.f;
	}
	else
	{
		float AdjustCurrentPosition = BannerScrollBox->GetScrollOffset() + ((CurrentViewBannerIndex - ToShowIndex) > 0 ? GetBannerWidth() : -GetBannerWidth());
		BannerScrollBox->SetScrollOffset(AdjustCurrentPosition);
	}
}

void UB2UILobbyBanner::ChangeBanner(int32 CenterBannerIndex, bool bForced)
{
	if (BannerImages.Num() < 1)
		return;

	CenterBannerIndex += BannerImages.Num();

	int32 OldViewBannerIndex = CurrentViewBannerIndex;
	CurrentViewBannerIndex = CenterBannerIndex % BannerImages.Num();

	if (BannerButtons[OldViewBannerIndex].IsValid())
		BannerButtons[OldViewBannerIndex]->ToggleImage(false);

	if (BannerButtons[CurrentViewBannerIndex].IsValid())
		BannerButtons[CurrentViewBannerIndex]->ToggleImage(true);

	int32 FirstImageIndex = (CenterBannerIndex - 1) % BannerImages.Num();
	int32 SecendImageIndex = (CenterBannerIndex) % BannerImages.Num();
	int32 ThirdImageIndex = (CenterBannerIndex + 1) % BannerImages.Num();

	FLobbyBannerData& BannerData_Back = BannerImages[(CenterBannerIndex - 1) % BannerImages.Num()];
	FLobbyBannerData& BannerData_Curr = BannerImages[CenterBannerIndex % BannerImages.Num()];
	FLobbyBannerData& BannerData_Forw = BannerImages[(CenterBannerIndex + 1) % BannerImages.Num()];

	if (IMG_BannerBackward.IsValid())
	{
		if (BannerData_Back.BannerImage == nullptr)
		{
			if (BannerData_Back.ErrorType == EErrorImageType::IMG_INVALID && LoadedErrorImage_Valid != nullptr)
				IMG_BannerBackward->SetBrushFromMaterial(LoadedErrorImage_Valid);
			else if (BannerData_Back.ErrorType == EErrorImageType::IMG_TOLARGE && LoadedErrorImage_Large != nullptr)
				IMG_BannerBackward->SetBrushFromMaterial(LoadedErrorImage_Large);
		}
		else
		{
			IMG_BannerBackward->SetBrushFromTextureDynamic(bForced ? BannerData_Curr.BannerImage : BannerData_Back.BannerImage);
		}
	}

	if (IMG_BannerCurrent.IsValid())
	{
		if (BannerData_Curr.BannerImage == nullptr)
		{
			if (BannerData_Curr.ErrorType == EErrorImageType::IMG_INVALID && LoadedErrorImage_Valid != nullptr)
				IMG_BannerCurrent->SetBrushFromMaterial(LoadedErrorImage_Valid);
			else if (BannerData_Curr.ErrorType == EErrorImageType::IMG_TOLARGE && LoadedErrorImage_Large != nullptr)
				IMG_BannerCurrent->SetBrushFromMaterial(LoadedErrorImage_Large);
		}
		else
			IMG_BannerCurrent->SetBrushFromTextureDynamic(BannerData_Curr.BannerImage);
	}

	if (IMG_BannerForward.IsValid())
	{

		if (BannerData_Forw.BannerImage == nullptr)
		{
			if (BannerData_Forw.ErrorType == EErrorImageType::IMG_INVALID && LoadedErrorImage_Valid != nullptr)
				IMG_BannerForward->SetBrushFromMaterial(LoadedErrorImage_Valid);
			else if (BannerData_Forw.ErrorType == EErrorImageType::IMG_TOLARGE && LoadedErrorImage_Large != nullptr)
				IMG_BannerForward->SetBrushFromMaterial(LoadedErrorImage_Large);
		}
		else
			IMG_BannerForward->SetBrushFromTextureDynamic(BannerData_Forw.BannerImage);
	}

	DesiredPosition = GetBannerWidth();

	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UB2UILobbyBanner::OnBeginScroll(float ScrolledDelta)
{
	bAutoMove = false;

	if (ScrolledDelta < GetBannerWidth() * 0.4f)
	{
		ChangeBanner(CurrentViewBannerIndex - 1);
		if (BannerScrollBox.IsValid())
			BannerScrollBox->SetScrollOffset(ScrolledDelta + DesiredPosition);
	}
	else if (ScrolledDelta > GetBannerWidth() * 1.6f)
	{
		ChangeBanner(CurrentViewBannerIndex + 1);
		if (BannerScrollBox.IsValid())
			BannerScrollBox->SetScrollOffset(ScrolledDelta - DesiredPosition);
	}

	if (BannerScrollBox.IsValid()) //모바일에서 SScrollBox::ScrollBy로 인해 AnimateScroll이 계속 켜지게 됨. AnimateScroll이 켜지면 ChangeBanner나 ShowBanner등 ScrollOffset이 바로 업데이트가 되지 않는다.
		BannerScrollBox->SetAnimateScroll(false);

	ElapsedNextBannerScrollTime = 0.f;
}

void UB2UILobbyBanner::OnEndScroll()
{
	ElapsedNextBannerScrollTime = 0.f;
	bAutoMove = true;
}

void UB2UILobbyBanner::OnClickBTN_Banner_Back()
{
	if (BannerImages.Num() < 1)
		return;

	int32 FirstImageIndex = (CurrentViewBannerIndex - 1) % BannerImages.Num();

	ClickBanner(FirstImageIndex);
}

void UB2UILobbyBanner::OnClickBTN_Banner_Current()
{
	if (BannerImages.Num() < 1)
		return;

	ClickBanner(CurrentViewBannerIndex);
}

void UB2UILobbyBanner::OnClickBTN_Banner_Forward()
{
	if (BannerImages.Num() < 1)
		return;

	int32 ThirdImageIndex = (CurrentViewBannerIndex + 1) % BannerImages.Num();

	ClickBanner(ThirdImageIndex);
}

void UB2UILobbyBanner::ClickBanner(int32 idx)
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (!DocStore)
		return;

	if (!BannerImages.IsValidIndex(idx))
		idx = 0;

	if (!BannerImages[idx].WebURL.IsEmpty())
	{
		B2P_ShowOpenURL(BannerImages[idx].WebURL);
		return;
	}

	switch (BannerImages[idx].WhereToGo)
	{
	case EBannerLobbyToGo::Goto_Pakage:
	{
		int32 PackageID = BannerImages[idx].WhereToGo_Sub;
		const FPackageData* PackageData = BladeIIGameImpl::GetClientDataStore().GetPackageData(PackageID);
		int32 CategoryNumber = PackageData ? PackageData->PackageTabType : 1;

		if (CategoryNumber == b2network::B2ShopPackageTabType::COSTUME)
		{
			TArray<FPackageProductData> OutList;
			BladeIIGameImpl::GetClientDataStore().GetPackageProductList(PackageID, OutList);

			if (OutList.Num() > 0)
			{
				EPCClass ItemPCClass = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(OutList[0].PackageProductId);
				//특정 패키지 PCClass 창으로 미리 셋팅
				LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Signal(ItemPCClass);
			}
			EnterCostumePageClass<ECostumePage>::GetInstance().Signal(ECostumePage::Store);
		}
		else
		{
			DocStore->SetPackageTabToOpen(CategoryNumber);
			EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::PackageStore));
		}
	}
	break;
	case EBannerLobbyToGo::Goto_EventMenu:
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::EventMain);
		break;
	case EBannerLobbyToGo::Goto_AttendanceEvent:
	{
		if (auto DocUICondition = UB2UIDocHelper::GetDocUICondition())
			DocUICondition->SetForcedOpenEventAttendance(true);

		if (auto* UIMgrInst = UB2UIManager::GetInstance())
		{
			auto* AttendanceMainWidget = UIMgrInst->OpenUI<UB2UIAttendanceMain>(UIFName::AttendanceMain);
			AttendanceMainWidget->Init();
		}
	}
	break;
	case EBannerLobbyToGo::DoNotGoAnywhere:
		break;
	}
}

void UB2UILobbyBanner::OnDownloadStart()
{
	UBannerManager* CacheManager = StaticFindLobbyBannerCacher();

	if (CacheManager)
	{
		const TMap<int32, UTexture2DDynamic*>& Imagearray = CacheManager->GetLobbyBannerImage();

		if (Imagearray.Num() > 0)
		{
			for (int32 i = 0; i < BannerImages.Num(); i++)
			{
				if (BannerImages.IsValidIndex(i))
				{
					auto FindItem = Imagearray.Contains(BannerImages[i].BannerIndex) ? Imagearray[BannerImages[i].BannerIndex] : nullptr;
					BannerImages[i].BannerImage = FindItem;
				}
			}

			CheckBannerAllLoad();

			return;
		}
	}

	CurrentDownloadIndex = 0;
	OnDownloadNext(CurrentDownloadIndex);

}

void UB2UILobbyBanner::OnDownloadNext(int32 idx)
{
	if (BannerImages.IsValidIndex(idx))
	{
		ImageDownloader->Start(BannerImages[idx].BannerUrl);
	}
	else
	{
		CheckBannerAllLoad();
	}
}

void UB2UILobbyBanner::CheckBannerAllLoad()
{
	bInitSuccess = true;

	if (HB_ButtonBox.IsValid())
	{
		HB_ButtonBox->ClearChildren();

		for (int32 idx = 0; idx < BannerImages.Num(); ++idx)
		{
			auto* CreatedWidget = CreateWidget<UB2UIStoreBannerButton>(GetWorld(), BannerButtonClass);
			if (CreatedWidget != NULL)
			{
				auto* AddedSlot = Cast<UHorizontalBoxSlot>(HB_ButtonBox->AddChild(CreatedWidget));
				check(AddedSlot);
				AddedSlot->SetPadding(FMargin(5.f, 0.f));
				CreatedWidget->Init();
				CreatedWidget->SetBannerIndex(idx);
				CreatedWidget->ToggleImage(idx == 0);

				BannerButtons.Add(CreatedWidget);
			}
		}
	}

	ShowBanner(0, true);
}

void UB2UILobbyBanner::OnDownloadSucess(class UTexture2DDynamic* Tex)
{
	if (BannerImages.IsValidIndex(CurrentDownloadIndex))
	{
		if (CheckImageSize(Tex))
		{
			BannerImages[CurrentDownloadIndex].BannerImage = Tex;

			UBannerManager* CacheManager = StaticFindLobbyBannerCacher();

			if (CacheManager)
			{
				CacheManager->SetLobbyBannerImage(Tex, BannerImages[CurrentDownloadIndex].BannerIndex);
			}
		}
	}	

	CurrentDownloadIndex++;
	OnDownloadNext(CurrentDownloadIndex);
}

void UB2UILobbyBanner::OnDownloadFailed(class UTexture2DDynamic* Tex)
{
	AddtoErrorData(true);
	CurrentDownloadIndex++;
	OnDownloadNext(CurrentDownloadIndex);
}

bool UB2UILobbyBanner::CheckImageSize(class UTexture2DDynamic* Tex)
{
	if (Tex == nullptr) 
		return false;

	FResourceSizeEx TotalAddedTextureSizeInfo;
	Tex->GetResourceSizeEx(TotalAddedTextureSizeInfo);

	if (TotalAddedTextureSizeInfo.GetTotalMemoryBytes() > 64 * 1024) // 사이즈가 너무 크다면 이미지를 대체
	{ 
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 && WITH_BII_ON_SCREEN_DEBUG_TEXT // 내부 빌드에서는 보다 자세한 내용을 디스플레이.
//		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPixelFormat"), true);
//		FString FormatInfoStr;
//		if (EnumPtr){
//			FormatInfoStr = EnumPtr->GetNameByValue((int64)Tex->Format).ToString();
//		}
//		BII_SCREEN_LOG(FString::Printf(TEXT("Too big banner image : %dx%d, %.2fKB, format %s"),
//			(int32)Tex->GetSurfaceWidth(), (int32)Tex->GetSurfaceHeight(),
//			((float)TotalAddedTextureSizeInfo.GetTotalMemoryBytes() / 1024.0f), *FormatInfoStr),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
#endif
		AddtoErrorData(false);
		return false;
	}

	return true;
}

void UB2UILobbyBanner::AddtoErrorData(bool bIsFail)
{
	LoadErrorImage();

	if (bIsFail)
	{
		BannerImages[CurrentDownloadIndex].ErrorType = EErrorImageType::IMG_INVALID;
	}
	else
	{
		BannerImages[CurrentDownloadIndex].ErrorType = EErrorImageType::IMG_TOLARGE;
	}
}

void UB2UILobbyBanner::LoadErrorImage()
{
	if(LoadedErrorImage_Valid == nullptr)
		LoadedErrorImage_Valid = MAT_ValidDownLoad.LoadSynchronous();
	if(LoadedErrorImage_Large == nullptr)
		LoadedErrorImage_Large = MAT_VeryLarge.LoadSynchronous();
}