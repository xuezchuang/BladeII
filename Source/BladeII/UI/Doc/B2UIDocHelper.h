// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "B2UIDocManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "B2UIDocSome.h"
#include "B2UIDocUser.h"
#include "B2UIDocUICondition.h"
#include "B2UIDocHero.h"
#include "B2UIDocBattle.h"
#include "B2UIDocBattleStage.h"
#include "B2UIDocSkill.h"
//#include "B2UIDocItem.h"
#include "B2UIDocEquip.h"
#include "B2UIDocScenarioDialog.h"
#include "B2UIDocChapter.h"
#include "B2UIDocMode.h"
#include "B2UIDocPVP1on1Rival.h"
#include "B2UIDocTeamMatch.h"
#include "B2UIDocCounterAttack.h"
#include "B2UIDocRaid.h"
#include "B2UIDocHeroTower.h"
#include "B2UIDocGuild.h"
#include "B2UIDocStore.h"
#include "B2UIDocControl.h"
#include "B2UIDocLobbyMatch.h"
#include "B2UIDocFriend.h"
#include "B2UIDocDimension.h"


#include "B2UIDocHelper.generated.h"


/*
* This class includes helper functions for Doc Create/Get.
*/

UCLASS(Transient)
class BLADEII_API UB2UIDocHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//UIDocSome
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocSome* GetDocSome();
	static class UB2UIDocSome* CreateDocSome();

	//UIDocUser
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocUser* GetDocUser();
	static class UB2UIDocUser* CreateDocUser();

	//UIDocCondition
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocUICondition* GetDocUICondition();
	static class UB2UIDocUICondition* CreateDocUICondition();

	//UIDocHero
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocHero* GetDocHero(const int32 PCClassType);
	static class UB2UIDocHero* CreateDocHero(int32 PCClassType);

	//UIDocSkill
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	//static class UB2UIDocSkill* GetDocSkill(); //HeroDoc俊辑 Get且巴.
	static class UB2UIDocSkill* GetDocSkill(const int32 PCClassType, const int32 SkillInfoSkillID);

	//UIDocItem
	//static class UB2UIDocItem* GetDocItem(const int32 InstanceUID, const int32 ItemRefID = INDEX_NONE);
	//static class UB2UIDocItem* CreateDocItem(const int32 InstanceUID, const int32 ItemRefID);

	//UIDocEquip
	static class UB2UIDocEquip* GetDocEquip(const int32 PCClassType, const int32 EquipPlaceIndex);

	//UIDocBattle
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocBattle* GetDocBattle();
	static class UB2UIDocBattle* CreateDocBattle();

	//UIDocBattleStage
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocBattleStage* GetDocBattleStage();
	static class UB2UIDocBattleStage* CreateDocBattleStage();

	//UIDocScenarioDialog
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocScenarioDialog* GetDocScenarioDialog();
	static class UB2UIDocScenarioDialog* CreateDocScenarioDialog();

	//UIDocChapter
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocChapter* GetDocChapter();
	static class UB2UIDocChapter* CreateDocChapter();

	//UIDocMode
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocMode* GetDocMode();
	static class UB2UIDocMode* CreateDocMode();

	//UIDocPVP1on1Rival
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocPVP1on1Rival* GetDocPVP1on1Rival();
	static class UB2UIDocPVP1on1Rival* CreateDocPVP1on1Rival();

	//UIDocTeamMatch
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocTeamMatch* GetDocTeamMatch();
	static class UB2UIDocTeamMatch* CreateDocTeamMatch();

	//UIDocCounterAttack
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocCounterAttack* GetDocCounterAttack();
	static class UB2UIDocCounterAttack* CreateDocCounterAttack();

	//UIDocRaid
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocRaid* GetDocRaid();
	static class UB2UIDocRaid* CreateDocRaid();

	//UIDocHeroTower
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocHeroTower* GetDocHeroTower();
	static class UB2UIDocHeroTower* CreateDocHeroTower();

	//UIDocGuild
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocGuild* GetDocGuild();
	static class UB2UIDocGuild* CreateDocGuild();

	//UIDocStore
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocStore* GetDocStore();
	static class UB2UIDocStore* CreateDocStore();

	//UIDocControl
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocControl* GetDocControl();
	static class UB2UIDocControl* CreateDocControl();

	//UIDocLobbyMatch
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocLobbyMatch* GetDocLobbyMatch();
	static class UB2UIDocLobbyMatch* CreateDocLobbyMatch();

	//UIDocLobbyMatch
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	static class UB2UIDocFriend* GetDocFriend();
	static class UB2UIDocFriend* CreateDocFriend();

	//UIDocDonation
	static class UB2UIDocDonation* GetDocDonation();
	static class UB2UIDocDonation* CreateDocDonation();

	//UIDocDimesion
	static class UB2UIDocDimension* GetDocDimension();
	static class UB2UIDocDimension* CreateDocDimension();
};
