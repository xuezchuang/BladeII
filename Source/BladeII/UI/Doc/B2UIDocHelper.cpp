#include "B2UIDocHelper.h"
#include "BladeIICharacter.h"
#include "B2Inventory.h"
#include "B2UIDocDonation.h"

UB2UIDocSome* UB2UIDocHelper::GetDocSome()
{
	FB2UIDocKey Key(EUIDocType::Some);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocSome>(Key) : NULL;
}
UB2UIDocSome* UB2UIDocHelper::CreateDocSome()
{
	FB2UIDocKey Key(EUIDocType::Some);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	auto DocSome = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocSome>(Key) : NULL;
	return DocSome;
}

UB2UIDocUser* UB2UIDocHelper::GetDocUser()
{
	FB2UIDocKey Key(EUIDocType::User);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocUser>(Key) : NULL;
}

UB2UIDocUser* UB2UIDocHelper::CreateDocUser()
{
	FB2UIDocKey Key(EUIDocType::User);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	auto DocUser = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocUser>(Key) : NULL;
	return DocUser;
}

UB2UIDocUICondition* UB2UIDocHelper::GetDocUICondition()
{
	FB2UIDocKey Key(EUIDocType::UICondition);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocUICondition>(Key) : NULL;
}
UB2UIDocUICondition* UB2UIDocHelper::CreateDocUICondition()
{
	FB2UIDocKey Key(EUIDocType::UICondition);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	auto DocUICondition = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocUICondition>(Key) : NULL;
	return DocUICondition;
}

UB2UIDocHero* UB2UIDocHelper::GetDocHero(const int32 PCClassType)
{
	FB2UIDocKey Key(EUIDocType::Hero, PCClassType);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocHero>(Key) : NULL;
}

UB2UIDocSkill* UB2UIDocHelper::GetDocSkill(const int32 PCClassType, const int32 SkillInfoSkillID)
{
	UB2UIDocHero* DocHero = GetDocHero(PCClassType);
	return DocHero ? DocHero->GetSkillDocOfID(SkillInfoSkillID) : NULL;
}

UB2UIDocEquip* UB2UIDocHelper::GetDocEquip(const int32 PCClassType, const int32 EquipPlaceIndex)
{
	//UB2UIDocHero* DocHero = GetDocHero(PCClassType);
	//return DocHero ? DocHero->GetEquipDocByPlaceIndex(EquipPlaceIndex) : NULL;
	return NULL;
}

UB2UIDocHero* UB2UIDocHelper::CreateDocHero(int32 PCClassType)
{
	FB2UIDocKey Key(EUIDocType::Hero, PCClassType);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocHero>(Key) : NULL;
}

UB2UIDocBattle* UB2UIDocHelper::GetDocBattle()
{
	FB2UIDocKey Key(EUIDocType::Battle);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocBattle>(Key) : NULL;
}

UB2UIDocBattle* UB2UIDocHelper::CreateDocBattle()
{
	FB2UIDocKey Key(EUIDocType::Battle);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	auto DocBattle = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocBattle>(Key) : NULL;
	return DocBattle;
}

UB2UIDocBattleStage* UB2UIDocHelper::GetDocBattleStage()
{
	FB2UIDocKey Key(EUIDocType::BattleStage);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocBattleStage>(Key) : NULL;
}

UB2UIDocBattleStage* UB2UIDocHelper::CreateDocBattleStage()
{
	FB2UIDocKey Key(EUIDocType::BattleStage);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	auto DocBattleStage = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocBattleStage>(Key) : NULL;
	return DocBattleStage;
}
//
// //DocItem篮 静瘤富绊, FB2Item阑 静绰 规氢栏肺 搬沥巢. 窜, FB2Item篮 Lobby俊辑父 镜 荐 乐澜俊 林狼秦具窃.
//UB2UIDocItem* UB2UIDocHelper::GetDocItem(const int32 InstanceUID, const int32 ItemRefID)
//{
//	if (InstanceUID <= INDEX_NONE) return nullptr;
//	FB2UIDocKey Key(EUIDocType::Item, InstanceUID);
//	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
//	auto DocItem = UIDocManager ? UIDocManager->GetDoc<UB2UIDocItem>(Key) : NULL;
//	if (DocItem == nullptr)
//	{
//		DocItem = CreateDocItem(InstanceUID, ItemRefID);
//	}
//
//	return DocItem;
//}
//
//UB2UIDocItem* UB2UIDocHelper::CreateDocItem(const int32 InstanceUID, const int32 ItemRefID)
//{
//	if (InstanceUID <= INDEX_NONE) return nullptr;
//	FB2UIDocKey Key(EUIDocType::Item, InstanceUID);
//	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
//	auto DocItem = UIDocManager ? UIDocManager->GetDoc<UB2UIDocItem>(Key) : NULL;
//	if (DocItem)
//	{
//		DocItem->LoadFromInfoAsset(ItemRefID);
//	}
//	else
//	{
//		DocItem = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocItem>(Key) : NULL;
//		if(DocItem){
//			DocItem->LoadFromInfoAsset(ItemRefID);
//		}
//	}
//	return DocItem;
//}


UB2UIDocScenarioDialog* UB2UIDocHelper::GetDocScenarioDialog()
{
	FB2UIDocKey Key(EUIDocType::ScenarioDialog);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocScenarioDialog>(Key) : NULL;
}

UB2UIDocScenarioDialog* UB2UIDocHelper::CreateDocScenarioDialog()
{
	FB2UIDocKey Key(EUIDocType::ScenarioDialog);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	auto DocScenarioDialog = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocScenarioDialog>(Key) : NULL;
	return DocScenarioDialog;
}

UB2UIDocChapter* UB2UIDocHelper::GetDocChapter()
{
	FB2UIDocKey Key(EUIDocType::Chapter);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocChapter>(Key) : NULL;
}

UB2UIDocChapter* UB2UIDocHelper::CreateDocChapter()
{
	FB2UIDocKey Key(EUIDocType::Chapter);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	auto DocChapter = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocChapter>(Key) : NULL;
	return DocChapter;
}

UB2UIDocMode* UB2UIDocHelper::GetDocMode()
{
	FB2UIDocKey Key(EUIDocType::Mode);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocMode>(Key) : NULL;
}

UB2UIDocMode* UB2UIDocHelper::CreateDocMode()
{
	FB2UIDocKey Key(EUIDocType::Mode);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocMode>(Key) : NULL;
}

UB2UIDocPVP1on1Rival* UB2UIDocHelper::GetDocPVP1on1Rival()
{
	FB2UIDocKey Key(EUIDocType::PVP1on1Battle);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocPVP1on1Rival>(Key) : NULL;
}

UB2UIDocPVP1on1Rival* UB2UIDocHelper::CreateDocPVP1on1Rival()
{
	FB2UIDocKey Key(EUIDocType::PVP1on1Battle);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocPVP1on1Rival>(Key) : NULL;
}

class UB2UIDocTeamMatch* UB2UIDocHelper::GetDocTeamMatch()
{
	FB2UIDocKey Key(EUIDocType::TeamMatch);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocTeamMatch>(Key) : NULL;
}

class UB2UIDocTeamMatch* UB2UIDocHelper::CreateDocTeamMatch()
{
	FB2UIDocKey Key(EUIDocType::TeamMatch);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocTeamMatch>(Key) : NULL;
}

class UB2UIDocCounterAttack* UB2UIDocHelper::GetDocCounterAttack()
{
	FB2UIDocKey Key(EUIDocType::CounterAttack);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocCounterAttack>(Key) : NULL;
}

class UB2UIDocCounterAttack* UB2UIDocHelper::CreateDocCounterAttack()
{
	FB2UIDocKey Key(EUIDocType::CounterAttack);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocCounterAttack>(Key) : NULL;
}

class UB2UIDocRaid* UB2UIDocHelper::GetDocRaid()
{
	FB2UIDocKey Key(EUIDocType::Raid);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocRaid>(Key) : NULL;
}

class UB2UIDocRaid* UB2UIDocHelper::CreateDocRaid()
{
	FB2UIDocKey Key(EUIDocType::Raid);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocRaid>(Key) : NULL;
}

class UB2UIDocHeroTower* UB2UIDocHelper::GetDocHeroTower()
{
	FB2UIDocKey Key(EUIDocType::HeroTower);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocHeroTower>(Key) : NULL;
}

class UB2UIDocHeroTower* UB2UIDocHelper::CreateDocHeroTower()
{
	FB2UIDocKey Key(EUIDocType::HeroTower);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocHeroTower>(Key) : NULL;
}

class UB2UIDocGuild* UB2UIDocHelper::GetDocGuild()
{
	FB2UIDocKey Key(EUIDocType::Guild);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocGuild>(Key) : NULL;
}

class UB2UIDocGuild* UB2UIDocHelper::CreateDocGuild()
{
	FB2UIDocKey Key(EUIDocType::Guild);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocGuild>(Key) : NULL;
}

class UB2UIDocStore* UB2UIDocHelper::GetDocStore()
{
	FB2UIDocKey Key(EUIDocType::Store);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocStore>(Key) : NULL;
}

class UB2UIDocStore* UB2UIDocHelper::CreateDocStore()
{
	FB2UIDocKey Key(EUIDocType::Store);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocStore>(Key) : NULL;
}

class UB2UIDocControl* UB2UIDocHelper::GetDocControl()
{
	FB2UIDocKey Key(EUIDocType::Control);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocControl>(Key) : NULL;
}

class UB2UIDocControl* UB2UIDocHelper::CreateDocControl()
{
	FB2UIDocKey Key(EUIDocType::Control);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocControl>(Key) : NULL;
}

class UB2UIDocLobbyMatch* UB2UIDocHelper::GetDocLobbyMatch()
{
	FB2UIDocKey Key(EUIDocType::LobbyMatch);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocLobbyMatch>(Key) : NULL;
}

class UB2UIDocLobbyMatch* UB2UIDocHelper::CreateDocLobbyMatch()
{
	FB2UIDocKey Key(EUIDocType::LobbyMatch);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocLobbyMatch>(Key) : NULL;
}

class UB2UIDocFriend* UB2UIDocHelper::GetDocFriend()
{
	FB2UIDocKey Key(EUIDocType::Friend);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocFriend>(Key) : NULL;
}

class UB2UIDocFriend* UB2UIDocHelper::CreateDocFriend()
{
	FB2UIDocKey Key(EUIDocType::Friend);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocFriend>(Key) : NULL;
}

class UB2UIDocDonation* UB2UIDocHelper::GetDocDonation()
{
	FB2UIDocKey Key(EUIDocType::Donation);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocDonation>(Key) : NULL;
}

class UB2UIDocDonation* UB2UIDocHelper::CreateDocDonation()
{
	FB2UIDocKey Key(EUIDocType::Donation);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocDonation>(Key) : NULL;
}

class UB2UIDocDimension* UB2UIDocHelper::GetDocDimension()
{
	FB2UIDocKey Key(EUIDocType::Dimension);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->GetDoc<UB2UIDocDimension>(Key) : NULL;
}

class UB2UIDocDimension* UB2UIDocHelper::CreateDocDimension()
{
	FB2UIDocKey Key(EUIDocType::Dimension);
	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	return UIDocManager ? UIDocManager->CreateDoc<UB2UIDocDimension>(Key) : NULL;
}
