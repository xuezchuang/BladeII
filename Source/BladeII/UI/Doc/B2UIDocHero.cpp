#include "BladeII.h"
#include "B2UIDocHero.h"
#include "B2UIDocSkill.h"
#include "BladeIICharacter.h"
#include "B2PCClassInfoBox.h"
#include "B2PCClassInfo.h"
//#include "B2SkillInfo.h"
//#include "B2UIDocEquip.h"
//#include "B2InGameHUDInfo_Player.h"
#include "B2UIDocManager.h"
//#include "B2CounterAttackGameMode.h"
//#include "B2TutorialGameMode.h"
#include "B2CombatStatEvaluator.h"
//#include "B2SkillRTManager.h"

#include "BladeIIGameImpl.h"

UB2UIDocHero::UB2UIDocHero(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bMultiple = true;
	bAnonymous = false;
	DocType = EUIDocType::Hero;
}

void UB2UIDocHero::Init()
{
	Super::Init();

	//InitResources(); �̰� ���⼭ �θ��� �ϸ� �����ϸ鼭 �ڵ����� InitResources �� �ǰ� PCClassInfo �� �������� �ȴ�. ������������ ������� �ʴ� Ŭ������� TAsset Lazy-loading �� ȿ���� ������.
}

void UB2UIDocHero::Destroy()
{
	Super::Destroy();
}

void UB2UIDocHero::InitResources()
{
	////����
	//switch (IntToPCClass(GetDocId()))
	//{
	//	case EPCClass::EPC_Gladiator:
	//		ClassDesc = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyDesc_Gladiator")));
	//		break;
	//	case EPCClass::EPC_Assassin:
	//		ClassDesc = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyDesc_Assassin")));
	//		break;
	//	case EPCClass::EPC_Wizard:
	//		ClassDesc = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyDesc_Wizard")));
	//		break;
	//	case EPCClass::EPC_Fighter:
	//		ClassDesc = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyDesc_Fighter")));
	//		break;
	//}
	//DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
	//
	//TArray<int32> RegistredSkillIDs;
	////UB2SkillRTManager::GetCharacterRegisteredSkillIDs(IntToPCClass(GetDocId()), GetB2GameModeType(nullptr), RegistredSkillIDs, &BladeIIGameImpl::GetLocalCharacterData());

	////�ϴ� �� ĳ���� ������ ��ų�� �־��ش�.
	//SkillDocs.Empty();
	//UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	//for (int32 i = 0; i < RegistredSkillIDs.Num(); ++i) // ESI_End �� �ƴ� SkillTag �� üũ�ؾ� �ϴ� �� �� ���������ϱ�..
	//{
	//	FB2UIDocKey DocKeySkill(EUIDocType::Skill, RegistredSkillIDs[i]);
	//	auto* DocSkill = UIDocManager ? UIDocManager->GetDoc<UB2UIDocSkill>(DocKeySkill) : NULL;

	//	if (!DocSkill){
	//		DocSkill = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocSkill>(DocKeySkill) : NULL;
	//	}

	//	AddSkillDoc(DocSkill, RegistredSkillIDs[i], i == WEAPON_6STAR_SKILL_INDEX ? i : INDEX_NONE);
	//}

	////������������ 8����
	//EquipDocs.Empty();
	//for (int32 i = 0; i < static_cast<int32>(EItemEquipPlace::EIEP_End); ++i)
	//{
	//	FB2UIDocKey DocKeyEquip(EUIDocType::Equip);
	//	auto* DocEquip = UIDocManager ? UIDocManager->GetDoc<UB2UIDocEquip>(DocKeyEquip) : NULL;

	//	if (!DocEquip){
	//		DocEquip = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocEquip>(DocKeyEquip) : NULL;
	//	}

	//	AddEquipDoc(DocEquip, i); // i �� EItemEquipPlace �� ��ȯ�� ��..
	//}
}

void UB2UIDocHero::ReleaseResources()
{
	// PCClassInfo �� TAsset Lazy-loading �� �����ϰ� ó���ϱ� ���� ���� ��ε�� �� HeroDoc �� ReleaseResources �� ��.
	SkillDocs.Empty();
	EquipDocs.Empty();
}

UB2PCClassInfo* UB2UIDocHero::GetRelevantPCClassInfo()
{
	//auto ClassInfoBox = StaticFindPCClassInfoBox();
	//BII_CHECK(ClassInfoBox);

	////�̸�
	//return ClassInfoBox ? ClassInfoBox->GetSingleClassInfo(IntToPCClass(GetDocId())) : NULL;
	return NULL;
}

//UB2InGameHUDInfo_Player* UB2UIDocHero::GetRelevantHUDInfo()
//{
//	UB2PCClassInfo* PCInfo = GetRelevantPCClassInfo();
//	// ������ override �� �ؾ� �ϴ±�..
//	return (PCInfo && PCInfo->HUDInfoClassOverride) ? Cast<UB2InGameHUDInfo_Player>(PCInfo->HUDInfoClassOverride->GetDefaultObject()) : NULL;
//}
//
//UB2InGameHUDInfo_Player* UB2UIDocHero::StaticGetRelevantHUDInfo(EPCClass InPClass)
//{
//	UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox();
//	UB2PCClassInfo* ClassInfo = PCInfoBox ? PCInfoBox->GetSingleClassInfo(InPClass) : NULL;
//	return (ClassInfo && ClassInfo->HUDInfoClassOverride) ? Cast<UB2InGameHUDInfo_Player>(ClassInfo->HUDInfoClassOverride->GetDefaultObject()) : NULL;
//}

UMaterialInterface* UB2UIDocHero::GetPlayerPortraitMaterial(UObject* WorldContextObject, EPCClass InPCClass)
{
	// ������ UIDocHero ���� �ʻ�ȭ �귯���� ���� reference �ϴ� ���� �ִµ� �װ� ���ְ� ��ſ� ���� ����� ���� ���� ��ü�ϱ� ���� ���⿡ helper �������� ����� ����.
	// ������ UIDoc �� ���ҽ� ���۷����� ����� ���� ����!
	//UB2InGameHUDInfo_Player* PlayerHUDInfo = StaticGetRelevantHUDInfo(InPCClass);
	//return PlayerHUDInfo ? PlayerHUDInfo->GetPlayerPortrait(WorldContextObject) : NULL;
	return NULL;
}

UMaterialInterface* UB2UIDocHero::GetPlayerDeathPortraitMaterial(UObject* WorldContextObject, EPCClass InPCClass)
{
	// ������ UIDocHero ���� �ʻ�ȭ �귯���� ���� reference �ϴ� ���� �ִµ� �װ� ���ְ� ��ſ� ���� ����� ���� ���� ��ü�ϱ� ���� ���⿡ helper �������� ����� ����.
	// ������ UIDoc �� ���ҽ� ���۷����� ����� ���� ����!
	//UB2InGameHUDInfo_Player* PlayerHUDInfo = StaticGetRelevantHUDInfo(InPCClass);
	//return PlayerHUDInfo ? PlayerHUDInfo->GetPlayerDeathPortrait(WorldContextObject) : NULL;
	return NULL;
}

UTexture2D* UB2UIDocHero::GetPlayerLargePortraitTexture(EPCClass InPCClass)
{
	// ������ UIDocHero ���� �ʻ�ȭ �귯���� ���� reference �ϴ� ���� �ִµ� �װ� ���ְ� ��ſ� ���� ����� ���� ���� ��ü�ϱ� ���� ���⿡ helper �������� ����� ����.
	// ������ UIDoc �� ���ҽ� ���۷����� ����� ���� ����!
	//UB2InGameHUDInfo_Player* PlayerHUDInfo = StaticGetRelevantHUDInfo(InPCClass);
	//return PlayerHUDInfo ? PlayerHUDInfo->GetPlayerLargePortrait() : NULL;
	return NULL;
}

void UB2UIDocHero::AddSkillDoc(UB2UIDocSkill* NewSkillDoc, int32 SkillInfoSkillID, int32 ToPlaceInput)
{
	if (NewSkillDoc)
	{
		NewSkillDoc->SetOwnerHeroDoc(this);
		NewSkillDoc->SetAndLoadFromSkillID(SkillInfoSkillID); // SkillDoc ���� �������� ��ȭ������ �����ʹ� �Ƹ� ���� �Ŷ� �⺻���� �����͵��� ���⼭ �� ��.

		if (SkillInfoSkillID != SKILL_INVALID_ID)
		{
			int32 InputIndex = SkillDocs.AddUnique(NewSkillDoc);
			if (ToPlaceInput == INDEX_NONE)
			{				
				NewSkillDoc->SetSkillInputIndex(InputIndex);
			}
			else
			{
				auto* OldSkillDoc = GetSkillDocByInputIndex(ToPlaceInput);
				if (OldSkillDoc)
					OldSkillDoc->SetSkillInputIndex(-1);

				NewSkillDoc->SetSkillInputIndex(ToPlaceInput);
			}			
		}		
	}
}

UB2UIDocSkill* UB2UIDocHero::GetSkillDocOfID(int32 InSkillInfoSkillID)
{
	for (UB2UIDocSkill* SkillDoc : SkillDocs)
	{
		if (SkillDoc->GetSkillInfoSkillID() == InSkillInfoSkillID)
		{
			return SkillDoc;
		}
	}
	return NULL;
}

UB2UIDocSkill* UB2UIDocHero::GetSkillDocByInputIndex(int32 InputIndex)
{
	for (UB2UIDocSkill* SkillDoc : SkillDocs)
	{
		if (SkillDoc->GetSkillInputIndex() == InputIndex)
		{
			return SkillDoc;
		}
	}
	return NULL;
}
//
//void UB2UIDocHero::AddEquipDoc(UB2UIDocEquip* NewEquipDoc, int32 EquipPlaceIndex)
//{
//	if (GetEquipDocByPlaceIndex(EquipPlaceIndex))
//	{
//		return;
//	}
//
//	if (NewEquipDoc)
//	{
//		NewEquipDoc->SetOwnerHeroDoc(this);
//		NewEquipDoc->SetEquipPlace(EquipPlaceIndex);
//		EquipDocs.AddUnique(NewEquipDoc);
//	}
//}

//[@SameLine, 180612] explevelinfo doc, datastore -> datastore ����
//void UB2UIDocHero::InitExp()
//{
	//EPCClass ThisClass = GetHeroClass();

	//float CharExp = static_cast<float>(BladeIIGameImpl::GetLocalCharacterData().GetCharacterExp(ThisClass));
	//float CharExpMax = static_cast<float>(BladeIIGameImpl::GetLocalCharacterData().GetCharacterExpMax(ThisClass));
	//float expPercent = 0;

	//if (CharExpMax > 0)
	//{
	//	expPercent = (CharExp * 100.0f) / CharExpMax;
	//}

	//SetCurrentExp(BladeIIGameImpl::GetLocalCharacterData().GetCharacterExp(ThisClass));
	//SetCurrentLevel(BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(ThisClass));
	//SetCurrentExpPercent(expPercent);

	//SetAttackPower( // �����߿� �������� ���ٸ� ��ǻ� InitialLevel �� ���.
	//	CombatStatEval::GetPCAttack(ThisClass, NULL, GetCurrentLevel())
	//);
	//SetDefensePower(
	//	CombatStatEval::GetPCDefense(ThisClass, NULL, GetCurrentLevel())
	//);
	//SetHealthPoint(
	//	CombatStatEval::GetPCHealth(ThisClass, NULL, GetCurrentLevel())
	//);

	//SetPostClearAttackPower(
	//	CombatStatEval::GetPCAttack(ThisClass, NULL, GetCurrentLevel())
	//);
	//SetPostClearDefensePower(
	//	CombatStatEval::GetPCDefense(ThisClass, NULL, GetCurrentLevel())
	//);
	//SetPostClearHealthPoint(
	//	CombatStatEval::GetPCHealth(ThisClass, NULL, GetCurrentLevel())
	//);
//}

//void UB2UIDocHero::UpdateExp()
//{
	//EPCClass ThisClass = GetHeroClass();

	//SetBasicUpgradeExp(BladeIIGameImpl::GetLocalCharacterData().GetCharacterExp(ThisClass));
	//SetBasicUpgradeLevel(BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(ThisClass));
	//SetBasicUpgradeExpPercent(GetPostClearExpPercent(ThisClass));
	//SetExtraUpgradeExp(BladeIIGameImpl::GetLocalCharacterData().GetCharacterExp(ThisClass));
	//SetExtraUpgradeLevel(BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(ThisClass));
	//SetExtraUpgradeExpPercent(GetPostClearExpPercent(ThisClass));

	//int32 BeforeLevel = GetCurrentLevel();
	//int32 AfterLevel = GetBasicUpgradeLevel();
	//SetAttackPower(
	//	CombatStatEval::GetPCAttack(ThisClass, NULL, BeforeLevel)
	//);
	//SetDefensePower(
	//	CombatStatEval::GetPCDefense(ThisClass, NULL, BeforeLevel)
	//);
	//SetHealthPoint(
	//	CombatStatEval::GetPCHealth(ThisClass, NULL, BeforeLevel)
	//);

	//SetPostClearAttackPower(
	//	CombatStatEval::GetPCAttack(ThisClass, NULL, AfterLevel)
	//);
	//SetPostClearDefensePower(
	//	CombatStatEval::GetPCDefense(ThisClass, NULL, AfterLevel)
	//);
	//SetPostClearHealthPoint(
	//	CombatStatEval::GetPCHealth(ThisClass, NULL, AfterLevel)
	//);
//}

void UB2UIDocHero::SetPowers(const int32 iLevel)
{
	//EPCClass ThisClass = GetHeroClass();

	//SetAttackPower(
	//	CombatStatEval::GetPCAttack(ThisClass, NULL, iLevel)
	//);
	//SetDefensePower(
	//	CombatStatEval::GetPCDefense(ThisClass, NULL, iLevel)
	//);
	//SetHealthPoint(
	//	CombatStatEval::GetPCHealth(ThisClass, NULL, iLevel)
	//);

}

void UB2UIDocHero::SetPostClearPowers(const int32 iLevel)
{
//	EPCClass ThisClass = GetHeroClass();
//
//	SetPostClearAttackPower(
//		CombatStatEval::GetPCAttack(ThisClass, NULL, iLevel)
//	);
//	SetPostClearDefensePower(
//		CombatStatEval::GetPCDefense(ThisClass, NULL, iLevel)
//	);
//	SetPostClearHealthPoint(
//		CombatStatEval::GetPCHealth(ThisClass, NULL, iLevel)
//	);
}

//UB2UIDocEquip* UB2UIDocHero::GetEquipDocByPlaceIndex(int32 EquipPlaceIndex)
//{
//	for (auto EquipDoc : EquipDocs)
//	{
//		if (EquipDoc->GetEquipPlace() == EquipPlaceIndex)
//			return EquipDoc;
//	}
//	return nullptr;
//}
