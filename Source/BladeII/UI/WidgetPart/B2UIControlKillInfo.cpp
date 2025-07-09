// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIControlKillInfo.h"
#include "B2UIManager.h"




UB2UIControlKillInfo::UB2UIControlKillInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LifeTime = 3.0f;

	ColorRedTeamDeco = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f));
	ColorRedTeamText = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f));
	ColorBlueTeamDeco = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f));
	ColorBlueTeamText = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f));
}

void UB2UIControlKillInfo::Init()
{
	Super::Init();

	m_fStartTime = GetWorld()->GetTimeSeconds();

	// 등장 애니메이션 필요하면 여기서 재생

}

void UB2UIControlKillInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_fStartTime > 0)
	{
		float fElapseTime = GetWorld()->GetTimeSeconds() - m_fStartTime;
		if (fElapseTime >= LifeTime)
		{
			DestroySelf(UB2UIManager::GetInstance());
			m_fStartTime = -1;
		}
	}
}

void UB2UIControlKillInfo::SetKillInfo(ETeamType KillerTeam, FText& txtKiller, FText& txtDeadMan, EPCClass KillerClass, EPCClass DeadManClass)
{
	//if (TB_Killer.IsValid() && IMG_KillerDeco.IsValid())
	//{
	//	TB_Killer->SetText(txtKiller);

	//	if (KillerTeam == ETeamType::ETT_Red)
	//	{
	//		TB_Killer->SetColorAndOpacity(ColorRedTeamText);
	//		IMG_KillerDeco->GetBrush().TintColor = ColorRedTeamDeco;
	//	}
	//	else
	//	{
	//		TB_Killer->SetColorAndOpacity(ColorBlueTeamText);
	//		IMG_KillerDeco->GetBrush().TintColor = ColorBlueTeamDeco;
	//	}
	//}

	//if (TB_DeadMan.IsValid() && IMG_DeadDeco.IsValid())
	//{
	//	TB_DeadMan->SetText(txtDeadMan);

	//	if (KillerTeam == ETeamType::ETT_Red)
	//	{
	//		TB_DeadMan->SetColorAndOpacity(ColorBlueTeamText);
	//		IMG_DeadDeco->GetBrush().TintColor = ColorBlueTeamDeco;
	//	}
	//	else
	//	{
	//		TB_DeadMan->SetColorAndOpacity(ColorRedTeamText);
	//		IMG_DeadDeco->GetBrush().TintColor = ColorRedTeamDeco;
	//	}
	//}

	//if (IMG_KillerFace.IsValid())
	//{
	//	IMG_KillerFace->SetBrushFromMaterial(FB2ClassPortraitImage::GetCharImage(PortraitInfo, KillerClass));
	//}

	//if (IMG_DeadFace.IsValid())
	//{
	//	IMG_DeadFace->SetBrushFromMaterial(FB2ClassPortraitImage::GetCharImage(PortraitInfo, DeadManClass));
	//}
}

void UB2UIControlKillInfo::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Killer);
	GET_SLOT(UImage, IMG_KillerFace);
	GET_SLOT(UImage, IMG_KillerDeco);

	GET_SLOT(UTextBlock, TB_DeadMan);
	GET_SLOT(UImage, IMG_DeadFace);
	GET_SLOT(UImage, IMG_DeadDeco);
}
