// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIControlIntroTeamInfo.h"
#include "B2ControlGameMode.h"
#include "B2InGameHUDInfo_Player.h"
#include "B2PCClassInfoBox.h"
#include "B2UIDocHelper.h"


#include "BladeIIGameImpl.h"

void UB2UIControlIntroTeamInfo::NativeConstruct()
{
	Super::NativeConstruct();
	
	CreateUserInfos();
}

void UB2UIControlIntroTeamInfo::CreateUserInfos()
{
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	
	int32 BattleGrade = 0;
	int32 BattleStarCount = 0;
	
	if (pGM)
	{		
		for (TPair<uint32, FControlPeerInfo>& PeerInfo : pGM->GetAllPeerInfos())
		{
			class ICharacterDataStore* pCharData = NULL;

			if (PeerInfo.Key == pGM->GetMyNetId())
			{
				pCharData = &BladeIIGameImpl::GetLocalCharacterData();
				BattleGrade = UB2UIDocHelper::GetDocControl()->GetBattleGrade();
				BattleStarCount = UB2UIDocHelper::GetDocControl()->GetBattleStarCount();
			}
			else
			{
				pCharData = &pGM->GetRemotePlayerInfoData(PeerInfo.Key);
				BattleGrade = UB2UIDocHelper::GetDocLobbyMatch()->GetBattleGrade(PeerInfo.Key);
				BattleStarCount = UB2UIDocHelper::GetDocLobbyMatch()->GetBattleStarCount(PeerInfo.Key);
			}

			if (pCharData)
				CreateUserInfo(pCharData, pGM->GetControlTeam(PeerInfo.Value.PlayerStartIdx), BattleGrade, BattleStarCount);
		}
	}
}

void UB2UIControlIntroTeamInfo::CreateUserInfo(class ICharacterDataStore* pCharData, ETeamType TeamType, int32 BattleGrade, int32 BattleStarCount)
{
	// 檬惑拳
	if (!FaceMaterials.Contains(pCharData->GetMainPlayerClass()))
		return;
		
	UMaterialInterface* pCharMI = NULL;

	TSoftObjectPtr<UMaterialInterface> MIPtr = FaceMaterials.FindRef(pCharData->GetMainPlayerClass());

	pCharMI = MIPtr.LoadSynchronous();

	if (!pCharMI)
		return;
		
	// 捞抚
	FString UserName = pCharData->GetUserNickName();

	AddTeamUserInfo(TeamType, FText::FromString(UserName), pCharMI, BattleGrade, BattleStarCount);
}
