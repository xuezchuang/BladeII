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

	//InitResources(); 이걸 여기서 부르게 하면 생성하면서 자동으로 InitResources 가 되고 PCClassInfo 를 가져오게 된다. 스테이지에서 사용하지 않는 클래스라면 TAsset Lazy-loading 의 효과가 없어짐.
}

void UB2UIDocHero::Destroy()
{
	Super::Destroy();
}

void UB2UIDocHero::InitResources()
{
	////설명
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

	////일단 각 캐릭터 생성시 스킬도 넣어준다.
	//SkillDocs.Empty();
	//UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	//for (int32 i = 0; i < RegistredSkillIDs.Num(); ++i) // ESI_End 가 아닌 SkillTag 로 체크해야 하는 게 좀 구질구질하군..
	//{
	//	FB2UIDocKey DocKeySkill(EUIDocType::Skill, RegistredSkillIDs[i]);
	//	auto* DocSkill = UIDocManager ? UIDocManager->GetDoc<UB2UIDocSkill>(DocKeySkill) : NULL;

	//	if (!DocSkill){
	//		DocSkill = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocSkill>(DocKeySkill) : NULL;
	//	}

	//	AddSkillDoc(DocSkill, RegistredSkillIDs[i], i == WEAPON_6STAR_SKILL_INDEX ? i : INDEX_NONE);
	//}

	////장착아이템은 8개씩
	//EquipDocs.Empty();
	//for (int32 i = 0; i < static_cast<int32>(EItemEquipPlace::EIEP_End); ++i)
	//{
	//	FB2UIDocKey DocKeyEquip(EUIDocType::Equip);
	//	auto* DocEquip = UIDocManager ? UIDocManager->GetDoc<UB2UIDocEquip>(DocKeyEquip) : NULL;

	//	if (!DocEquip){
	//		DocEquip = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocEquip>(DocKeyEquip) : NULL;
	//	}

	//	AddEquipDoc(DocEquip, i); // i 는 EItemEquipPlace 로 변환될 것..
	//}
}

void UB2UIDocHero::ReleaseResources()
{
	// PCClassInfo 의 TAsset Lazy-loading 을 안전하게 처리하기 위해 레벨 언로드시 매 HeroDoc 은 ReleaseResources 를 함.
	SkillDocs.Empty();
	EquipDocs.Empty();
}

UB2PCClassInfo* UB2UIDocHero::GetRelevantPCClassInfo()
{
	//auto ClassInfoBox = StaticFindPCClassInfoBox();
	//BII_CHECK(ClassInfoBox);

	////이름
	//return ClassInfoBox ? ClassInfoBox->GetSingleClassInfo(IntToPCClass(GetDocId())) : NULL;
	return NULL;
}

//UB2InGameHUDInfo_Player* UB2UIDocHero::GetRelevantHUDInfo()
//{
//	UB2PCClassInfo* PCInfo = GetRelevantPCClassInfo();
//	// 어차피 override 를 해야 하는군..
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
	// 이전에 UIDocHero 에서 초상화 브러쉬를 직접 reference 하던 적이 있는데 그건 없애고 대신에 이전 사용을 보다 쉽게 대체하기 위해 여기에 helper 형식으로 만들어 놓음.
	// 실제로 UIDoc 에 리소스 레퍼런스를 멤버로 넣지 말것!
	//UB2InGameHUDInfo_Player* PlayerHUDInfo = StaticGetRelevantHUDInfo(InPCClass);
	//return PlayerHUDInfo ? PlayerHUDInfo->GetPlayerPortrait(WorldContextObject) : NULL;
	return NULL;
}

UMaterialInterface* UB2UIDocHero::GetPlayerDeathPortraitMaterial(UObject* WorldContextObject, EPCClass InPCClass)
{
	// 이전에 UIDocHero 에서 초상화 브러쉬를 직접 reference 하던 적이 있는데 그건 없애고 대신에 이전 사용을 보다 쉽게 대체하기 위해 여기에 helper 형식으로 만들어 놓음.
	// 실제로 UIDoc 에 리소스 레퍼런스를 멤버로 넣지 말것!
	//UB2InGameHUDInfo_Player* PlayerHUDInfo = StaticGetRelevantHUDInfo(InPCClass);
	//return PlayerHUDInfo ? PlayerHUDInfo->GetPlayerDeathPortrait(WorldContextObject) : NULL;
	return NULL;
}

UTexture2D* UB2UIDocHero::GetPlayerLargePortraitTexture(EPCClass InPCClass)
{
	// 이전에 UIDocHero 에서 초상화 브러쉬를 직접 reference 하던 적이 있는데 그건 없애고 대신에 이전 사용을 보다 쉽게 대체하기 위해 여기에 helper 형식으로 만들어 놓음.
	// 실제로 UIDoc 에 리소스 레퍼런스를 멤버로 넣지 말것!
	//UB2InGameHUDInfo_Player* PlayerHUDInfo = StaticGetRelevantHUDInfo(InPCClass);
	//return PlayerHUDInfo ? PlayerHUDInfo->GetPlayerLargePortrait() : NULL;
	return NULL;
}

void UB2UIDocHero::AddSkillDoc(UB2UIDocSkill* NewSkillDoc, int32 SkillInfoSkillID, int32 ToPlaceInput)
{
	if (NewSkillDoc)
	{
		NewSkillDoc->SetOwnerHeroDoc(this);
		NewSkillDoc->SetAndLoadFromSkillID(SkillInfoSkillID); // SkillDoc 에는 동적으로 변화무쌍한 데이터는 아마 없을 거라 기본적인 데이터들은 여기서 들어갈 것.

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

//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
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

	//SetAttackPower( // 전투중에 레벨업이 없다면 사실상 InitialLevel 로 사용.
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
