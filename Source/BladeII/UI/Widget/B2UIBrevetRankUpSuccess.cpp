// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIBrevetRankUpSuccess.h"
#include "B2LobbyUI_BrevetMain.h"
#include "B2UIManager.h"


#include "BladeIIGameImpl.h"

void UB2UIBrevetRankUpSuccess::InitBrevetRankUpSuccess(class UB2LobbyUI_BrevetMain* pOwnerUI, int32 _iPrevRank, int32 _iCurrentRank)
{
	if (!pOwnerUI)
		return;

	m_pOwnerUI = pOwnerUI;

	iCurrentRank = _iCurrentRank;

	SetRankWidgets(_iPrevRank);

	PlayAnimationEvent_Anim_RankUp();
}

void UB2UIBrevetRankUpSuccess::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_RankIcon);
	GET_SLOT(UTextBlock, TB_RankName);
	GET_SLOT(UTextBlock, TB_Title);

	UpdateStaticText();
}

void UB2UIBrevetRankUpSuccess::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_Success")));
}

void UB2UIBrevetRankUpSuccess::OnFinishAnimationEvent_Anim_RankUp()
{
	if (m_pOwnerUI)
		m_pOwnerUI->OnFinishAnimationEvent_Anim_RankUp();

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIBrevetRankUpSuccess::ChangeRankPrevAtCurrent()
{
	SetRankWidgets(iCurrentRank);
}

void UB2UIBrevetRankUpSuccess::SetRankWidgets(int32 iRank)
{
	if (IMG_RankIcon.IsValid())
	{
		IMG_RankIcon->SetBrushFromMaterial(GetRankIconMatarial(iRank));
	}

	if (TB_RankName.IsValid())
	{
		TB_RankName->SetText(GetRankName(iRank));
	}
}

UMaterialInterface* UB2UIBrevetRankUpSuccess::GetRankIconMatarial(int32 iRank)
{
	UMaterialInterface* pMaterial = BladeIIGameImpl::GetClientDataStore().GetBrevetRankIconMaterial(iRank, true);

	return pMaterial ? pMaterial : nullptr;
}

FText UB2UIBrevetRankUpSuccess::GetRankName(int32 iRank)
{
	FString RankNameKey = BladeIIGameImpl::GetClientDataStore().GetBrevetRankNameKey(iRank);

	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), RankNameKey);
}