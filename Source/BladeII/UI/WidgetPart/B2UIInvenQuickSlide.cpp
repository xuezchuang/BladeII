#include "bladeII.h"
#include "B2UIInvenQuickSlide.h"
#include "B2UIEnum.h"
#include "Event.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2LobbyUI_HeroMgmtCommon.h"

UB2UIInvenQuickSlide::UB2UIInvenQuickSlide(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIInvenQuickSlide::Init()
{
	Super::Init();
}

void UB2UIInvenQuickSlide::CacheAssets()
{
	GET_SLOT(UB2UIWidgetBase, IMG_MarkForNew_Inven);
	GET_SLOT(UB2UIWidgetBase, IMG_MarkForNew_Skill);
	GET_SLOT(UB2UIWidgetBase, IMG_MarkForNew_Brevet);
	GET_SLOT(UB2UIWidgetBase, IMG_MarkForNew_Wing);
	GET_SLOT(UB2UIWidgetBase, IMG_MarkForNew_Relic);
	GET_SLOT(UB2UIWidgetBase, IMG_MarkForNew_All);

	ArrImg_Lock.AddUninitialized(3);
	GET_SLOT_BYNAME(UImage, ArrImg_Lock[0], IMG_BrevetLock);
	GET_SLOT_BYNAME(UImage, ArrImg_Lock[1], IMG_WingLock);
	GET_SLOT_BYNAME(UImage, ArrImg_Lock[2], IMG_ConstantLock);
	
	ArrImg_Icon.AddUninitialized(3);
	GET_SLOT_BYNAME(UImage, ArrImg_Icon[0], Img_Brevet);
	GET_SLOT_BYNAME(UImage, ArrImg_Icon[1], Img_Wing);
	GET_SLOT_BYNAME(UImage, ArrImg_Icon[2], Img_Constant);

	SetVisibleVersion();
	
}

void UB2UIInvenQuickSlide::BindDelegates()
{
	
}

void UB2UIInvenQuickSlide::SetVisibleVersion()
{
	//bool bGDC = ShouldShowGDCVersionContent();// [GDC] Feb 2017)

	//for (auto ImgWeakPtr : ArrImg_Lock)
	//{
	//	if (ImgWeakPtr.IsValid())
	//		ImgWeakPtr->SetVisibility(bGDC ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	//}

	//for (auto ImgWeakPtr : ArrImg_Icon)
	//{
	//	if (ImgWeakPtr.IsValid())
	//	{
	//		UImage* Img_Icon = static_cast<UImage*>(ImgWeakPtr.Get());
	//		Img_Icon->GetBrush().TintColor = bGDC ? FLinearColor(0.3f, 0.3f, 0.3f) : FLinearColor(1.0f, 1.0f, 1.0f);
	//	}

	//}
}

void UB2UIInvenQuickSlide::DoMarkRedDot()
{
	/*
	bool bMarkForNewInven = BladeIIGameImpl::GetRedDotManager().ShouldMarkForNewItem(EPCClass::EPC_End);
	bool bMarkForNewSkill = BladeIIGameImpl::GetRedDotManager().ShouldMarkForNewSkill(EPCClass::EPC_End);
	bool bMarkForNewWing = BladeIIGameImpl::GetRedDotManager().ShouldMarkForNewWing(EPCClass::EPC_End);

	if (IMG_MarkForNew_Inven.IsValid())
		IMG_MarkForNew_Inven->SetVisibility(bMarkForNewInven ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (IMG_MarkForNew_Skill.IsValid())
		IMG_MarkForNew_Skill->SetVisibility(bMarkForNewSkill ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (IMG_MarkForNew_Brevet.IsValid())
		IMG_MarkForNew_Brevet->SetVisibility(ESlateVisibility::Collapsed);

	if (IMG_MarkForNew_Wing.IsValid())
		IMG_MarkForNew_Wing->SetVisibility(bMarkForNewWing ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (IMG_MarkForNew_Relic.IsValid())
		IMG_MarkForNew_Relic->SetVisibility(ESlateVisibility::Collapsed);

	if (IMG_MarkForNew_All.IsValid())
		IMG_MarkForNew_All->SetVisibility(bMarkForNewInven || bMarkForNewSkill || bMarkForNewWing ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		*/
}

void UB2UIInvenQuickSlide::RegisterUIMarkForRedDot()
{
	/*
	BladeIIGameImpl::GetRedDotManager().AddUI(EUICategoryMarkForNew::Item, this);
	BladeIIGameImpl::GetRedDotManager().AddUI(EUICategoryMarkForNew::SkillPoint, this);
	BladeIIGameImpl::GetRedDotManager().AddUI(EUICategoryMarkForNew::Wing, this);
	*/
}

void UB2UIInvenQuickSlide::UnRegisterUIMarkForRedDot()
{
	/*
	BladeIIGameImpl::GetRedDotManager().RemoveUI(EUICategoryMarkForNew::Item, this);
	BladeIIGameImpl::GetRedDotManager().RemoveUI(EUICategoryMarkForNew::SkillPoint, this);
	BladeIIGameImpl::GetRedDotManager().RemoveUI(EUICategoryMarkForNew::Wing, this);
	*/
}

//====================================================================================
//									Click Handler
//====================================================================================

void UB2UIInvenQuickSlide::GotoInvenScene()
{
	SetInvenQuickSlot(true);
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory); // GameMode 쪽 신호	
}

void UB2UIInvenQuickSlide::GotoSkillScene()
{
	SetInvenQuickSlot(true);

	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Skill); // GameMode 쪽 신호
	
}

void UB2UIInvenQuickSlide::GotoBrevetScene()
{
	if (ShouldShowGDCVersionContent()) // [GDC] Feb 2017)
	{
		DevShowNotReadyYetMessage();
		return;
	}

	SetInvenQuickSlot(true);

	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Brevet); // GameMode 쪽 신호
	
}

void UB2UIInvenQuickSlide::GotoWingScene()
{
	if (ShouldShowGDCVersionContent()) // [GDC] Feb 2017)
	{
		DevShowNotReadyYetMessage();
		return;
	}
	SetInvenQuickSlot(true);

	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Wing); // GameMode 쪽 신호
	
}

void UB2UIInvenQuickSlide::GotoConstantheroScene()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Relic); // GameMode 쪽 신호
}

bool UB2UIInvenQuickSlide::GetInvenQuickSlide()
{
	return GetInvenQuickSlot();
}

