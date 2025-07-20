#include "B2UIStoreBanner.h"
#include "B2UIDocHelper.h"
#include "Event.h"

void UB2UIStoreBannerButton::Init()
{
	Super::Init();
}

void UB2UIStoreBannerButton::ToggleImage(bool ToggleOn)
{
	if (IMG_On.IsValid())
		IMG_On->SetVisibility(ToggleOn ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_Off.IsValid())
		IMG_Off->SetVisibility(!ToggleOn ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UB2UIStoreBannerButton::CacheAssets()
{
	GET_SLOT(UImage, IMG_On);
	GET_SLOT(UImage, IMG_Off);
	GET_SLOT(UB2Button, BTN_Area);
}

void UB2UIStoreBannerButton::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UIStoreBannerButton::OnClickedButton);
}

void UB2UIStoreBannerButton::SetBannerIndex(int32 ToSet)
{
	BannerIndex = ToSet;
}

void UB2UIStoreBannerButton::SetBannerMain(class UB2UIStoreBanner* ToSet)
{
	BannerMain = ToSet;
}

void UB2UIStoreBannerButton::OnClickedButton()
{
	if (BannerMain)
		BannerMain->ShowBanner(BannerIndex, true);
}

UB2UIStoreBanner::UB2UIStoreBanner(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	WidgetScale = 1.f;
	AutoScrollTime = 0.5f;
	NextBannerScrollTime = 5.f;
	BannerImageWidth = 0.f;
	CurrentViewBannerIndex = 0;
}

float UB2UIStoreBanner::GetBannerWidth()
{
	return BannerImageWidth;
}

void UB2UIStoreBanner::Init()
{
	BannerImages.Empty();

	if (BannerDataInfos)
		BannerImages = BannerDataInfos->GetDefaultObject<UB2BannerDataInfos>()->BannerData;

	Super::Init();	

	if (HB_ButtonBox.IsValid())
	{
		HB_ButtonBox->ClearChildren();

		if (BannerImages.Num() == 0 && HB_BannerBox.IsValid())
		{
			HB_BannerBox->ClearChildren();
		}

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
				CreatedWidget->SetBannerMain(this);
				CreatedWidget->ToggleImage(idx == 0);

				BannerImages[idx].BannerButton = CreatedWidget;
			}
		}
	}
	
	ShowBanner(0, true);
}

void UB2UIStoreBanner::CacheAssets()
{
	GET_SLOT(UHorizontalBox, HB_BannerBox);
	GET_SLOT(UImage, IMG_BannerBackward);
	GET_SLOT(UImage, IMG_BannerCurrent);
	GET_SLOT(UImage, IMG_BannerForward);

	GET_SLOT(UHorizontalBox, HB_ButtonBox);

	GET_SLOT(UB2ScrollBox, BannerScrollBox);

	GET_SLOT(UB2Button, BTN_Banner);
}

void UB2UIStoreBanner::BindDelegates()
{
	if (BannerScrollBox.IsValid())
	{
		BannerScrollBox->SetOnUserScrolled(FOnUserScrolled::CreateUObject(this, &UB2UIStoreBanner::OnBeginScroll));
	}

	BIND_RELEASE_FUNC_TO_BTN(BTN_Banner, &UB2UIStoreBanner::OnClickedBanner);
}

void UB2UIStoreBanner::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
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
	//			//IMG_BannerForward->GetBrush().ImageSize.X = NewSize;
	//			//IMG_BannerForward->GetBrush().ImageSize.Y = MyGeometry.GetLocalSize().Y - OverlaySlot->Padding.Top - OverlaySlot->Padding.Bottom;
	//		}
	//		if (IMG_BannerCurrent.IsValid())
	//		{
	//			//IMG_BannerCurrent->GetBrush().ImageSize.X = NewSize;
	//			//IMG_BannerCurrent->GetBrush().ImageSize.Y = MyGeometry.GetLocalSize().Y - OverlaySlot->Padding.Top - OverlaySlot->Padding.Bottom;
	//		}
	//		if (IMG_BannerBackward.IsValid())
	//		{
	//			//IMG_BannerBackward->GetBrush().ImageSize.X = NewSize;
	//			//IMG_BannerBackward->GetBrush().ImageSize.Y = MyGeometry.GetLocalSize().Y - OverlaySlot->Padding.Top - OverlaySlot->Padding.Bottom;
	//		}
	//	}		
	//}		

	//Super::NativeTick(MyGeometry, InDeltaTime);

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
	//			if (ElapsedNextBannerScrollTime > NextBannerScrollTime)
	//			{
	//				ShowBanner(CurrentViewBannerIndex + 1, false);
	//				ElapsedNextBannerScrollTime = 0.f;
	//			}
	//		}
	//	}		
	//}		
}

void UB2UIStoreBanner::AutoScroll(float InDeltaTime)
{
	if (!BannerScrollBox.IsValid())
		return;
			
	float CurrentPosition = BannerScrollBox->GetScrollOffset();
	bool bForwardDirection = CurrentPosition < DesiredPosition;

	float Velocity = GetBannerWidth() / AutoScrollTime;
	float TickMoveDelta = FMath::Clamp(Velocity * InDeltaTime, 0.f, FMath::Abs(DesiredPosition - CurrentPosition));

	BannerScrollBox->SetScrollOffset(bForwardDirection ? CurrentPosition + TickMoveDelta : CurrentPosition - TickMoveDelta);
}

void UB2UIStoreBanner::ShowBanner(int32 ToShowIndex, bool bForce)
{
	if (!BannerScrollBox.IsValid())
		return;

	if (BannerImages.Num() <= 1)
		return;

	if (ToShowIndex < 0)
		ToShowIndex = BannerImages.Num() - 1;

	ChangeBanner(ToShowIndex);

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

void UB2UIStoreBanner::ChangeBanner(int32 CenterBannerIndex)
{
	if (BannerImages.Num() <= 1)
		return;	

	CenterBannerIndex += BannerImages.Num();

	int32 OldViewBannerIndex = CurrentViewBannerIndex;
	CurrentViewBannerIndex = CenterBannerIndex % BannerImages.Num();

	if (BannerImages[OldViewBannerIndex].BannerButton.IsValid())
		BannerImages[OldViewBannerIndex].BannerButton->ToggleImage(false);

	if (BannerImages[CurrentViewBannerIndex].BannerButton.IsValid())
		BannerImages[CurrentViewBannerIndex].BannerButton->ToggleImage(true);
		
	if (IMG_BannerBackward.IsValid())
		IMG_BannerBackward->SetBrushFromMaterial(BannerImages[(CenterBannerIndex - 1) % BannerImages.Num()].BannerImage);
	if (IMG_BannerCurrent.IsValid())
		IMG_BannerCurrent->SetBrushFromMaterial(BannerImages[CenterBannerIndex % BannerImages.Num()].BannerImage);
	if (IMG_BannerForward.IsValid())
		IMG_BannerForward->SetBrushFromMaterial(BannerImages[(CenterBannerIndex + 1) % BannerImages.Num()].BannerImage);

	DesiredPosition = GetBannerWidth();
}

void UB2UIStoreBanner::OnBeginScroll(float ScrolledDelta)
{
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

	if (BannerScrollBox.IsValid()) //葛官老俊辑 SScrollBox::ScrollBy肺 牢秦 AnimateScroll捞 拌加 难瘤霸 凳. AnimateScroll捞 难瘤搁 ChangeBanner唱 ShowBanner殿 ScrollOffset捞 官肺 诀单捞飘啊 登瘤 臼绰促.
		BannerScrollBox->SetAnimateScroll(false);

	ElapsedNextBannerScrollTime = 0.f;
}

void UB2UIStoreBanner::OnClickedBanner()
{
	if (BannerImages.Num() < 1)
		return;

	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (!DocStore)
		return;

	switch (BannerImages[CurrentViewBannerIndex].WhereToGo)
	{
	case EBannerStoreToGo::SummonItem_GemBox:
		DocStore->SetSummonItemTabToOpen(ESummonItemTabMenu::GemBox);
		break;

	case EBannerStoreToGo::SummonItem_FriendBox:
		DocStore->SetSummonItemTabToOpen(ESummonItemTabMenu::FriendShipBox);
		break;

	case EBannerStoreToGo::SummonItem_Costume:
		DocStore->SetSummonItemTabToOpen(ESummonItemTabMenu::Costume);
		break;

	case EBannerStoreToGo::Store_Package:
		DocStore->SetStoreTabToOpen(EStoreCategory::Package);
		break;

	case EBannerStoreToGo::Store_DarkCrystal:
		DocStore->SetStoreTabToOpen(EStoreCategory::DarkCrystal);
		DocStore->SetStoreGoodsTabToOpen(-1);
		break;

	case EBannerStoreToGo::Store_BladePoint:
		DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
		DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::BLADE_POINT);
		break;

	case EBannerStoreToGo::Store_Gem:
		DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
		DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::GEM);
		break;

	case EBannerStoreToGo::Store_Gold:
		DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
		DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::GOLD);
		break;

	case EBannerStoreToGo::DoNotGoAnywhere:
		break;
	}

	switch (BannerImages[CurrentViewBannerIndex].WhereToGo)
	{
	case EBannerStoreToGo::SummonItem_GemBox:
	case EBannerStoreToGo::SummonItem_FriendBox:
	case EBannerStoreToGo::SummonItem_Costume:
		EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::EquipStore));
		break;
	case EBannerStoreToGo::Store_BladePoint:
	case EBannerStoreToGo::Store_Gem:
	case EBannerStoreToGo::Store_Gold:
	case EBannerStoreToGo::Store_DarkCrystal:
		EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));
		break;
	case EBannerStoreToGo::Store_Package:
		EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::PackageStore));
		break;
	}
}