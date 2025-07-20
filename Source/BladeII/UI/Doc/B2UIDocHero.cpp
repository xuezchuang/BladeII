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

	//InitResources(); 捞吧 咯扁辑 何福霸 窍搁 积己窍搁辑 磊悼栏肺 InitResources 啊 登绊 PCClassInfo 甫 啊廉坷霸 等促. 胶抛捞瘤俊辑 荤侩窍瘤 臼绰 努贰胶扼搁 TAsset Lazy-loading 狼 瓤苞啊 绝绢咙.
}

void UB2UIDocHero::Destroy()
{
	Super::Destroy();
}

void UB2UIDocHero::InitResources()
{
	////汲疙
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

	////老窜 阿 某腐磐 积己矫 胶懦档 持绢霖促.
	//SkillDocs.Empty();
	//UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
	//for (int32 i = 0; i < RegistredSkillIDs.Num(); ++i) // ESI_End 啊 酒囱 SkillTag 肺 眉农秦具 窍绰 霸 粱 备龙备龙窍焙..
	//{
	//	FB2UIDocKey DocKeySkill(EUIDocType::Skill, RegistredSkillIDs[i]);
	//	auto* DocSkill = UIDocManager ? UIDocManager->GetDoc<UB2UIDocSkill>(DocKeySkill) : NULL;

	//	if (!DocSkill){
	//		DocSkill = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocSkill>(DocKeySkill) : NULL;
	//	}

	//	AddSkillDoc(DocSkill, RegistredSkillIDs[i], i == WEAPON_6STAR_SKILL_INDEX ? i : INDEX_NONE);
	//}

	////厘馒酒捞袍篮 8俺究
	//EquipDocs.Empty();
	//for (int32 i = 0; i < static_cast<int32>(EItemEquipPlace::EIEP_End); ++i)
	//{
	//	FB2UIDocKey DocKeyEquip(EUIDocType::Equip);
	//	auto* DocEquip = UIDocManager ? UIDocManager->GetDoc<UB2UIDocEquip>(DocKeyEquip) : NULL;

	//	if (!DocEquip){
	//		DocEquip = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocEquip>(DocKeyEquip) : NULL;
	//	}

	//	AddEquipDoc(DocEquip, i); // i 绰 EItemEquipPlace 肺 函券瞪 巴..
	//}
}

void UB2UIDocHero::ReleaseResources()
{
	// PCClassInfo 狼 TAsset Lazy-loading 阑 救傈窍霸 贸府窍扁 困秦 饭骇 攫肺靛矫 概 HeroDoc 篮 ReleaseResources 甫 窃.
	SkillDocs.Empty();
	EquipDocs.Empty();
}

UB2PCClassInfo* UB2UIDocHero::GetRelevantPCClassInfo()
{
	//auto ClassInfoBox = StaticFindPCClassInfoBox();
	//BII_CHECK(ClassInfoBox);

	////捞抚
	//return ClassInfoBox ? ClassInfoBox->GetSingleClassInfo(IntToPCClass(GetDocId())) : NULL;
	return NULL;
}

//UB2InGameHUDInfo_Player* UB2UIDocHero::GetRelevantHUDInfo()
//{
//	UB2PCClassInfo* PCInfo = GetRelevantPCClassInfo();
//	// 绢瞒乔 override 甫 秦具 窍绰焙..
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
	// 捞傈俊 UIDocHero 俊辑 檬惑拳 宏矾浆甫 流立 reference 窍带 利捞 乐绰单 弊扒 绝局绊 措脚俊 捞傈 荤侩阑 焊促 奖霸 措眉窍扁 困秦 咯扁俊 helper 屈侥栏肺 父甸绢 初澜.
	// 角力肺 UIDoc 俊 府家胶 饭欺繁胶甫 糕滚肺 持瘤 富巴!
	//UB2InGameHUDInfo_Player* PlayerHUDInfo = StaticGetRelevantHUDInfo(InPCClass);
	//return PlayerHUDInfo ? PlayerHUDInfo->GetPlayerPortrait(WorldContextObject) : NULL;
	return NULL;
}

UMaterialInterface* UB2UIDocHero::GetPlayerDeathPortraitMaterial(UObject* WorldContextObject, EPCClass InPCClass)
{
	// 捞傈俊 UIDocHero 俊辑 檬惑拳 宏矾浆甫 流立 reference 窍带 利捞 乐绰单 弊扒 绝局绊 措脚俊 捞傈 荤侩阑 焊促 奖霸 措眉窍扁 困秦 咯扁俊 helper 屈侥栏肺 父甸绢 初澜.
	// 角力肺 UIDoc 俊 府家胶 饭欺繁胶甫 糕滚肺 持瘤 富巴!
	//UB2InGameHUDInfo_Player* PlayerHUDInfo = StaticGetRelevantHUDInfo(InPCClass);
	//return PlayerHUDInfo ? PlayerHUDInfo->GetPlayerDeathPortrait(WorldContextObject) : NULL;
	return NULL;
}

UTexture2D* UB2UIDocHero::GetPlayerLargePortraitTexture(EPCClass InPCClass)
{
	// 捞傈俊 UIDocHero 俊辑 檬惑拳 宏矾浆甫 流立 reference 窍带 利捞 乐绰单 弊扒 绝局绊 措脚俊 捞傈 荤侩阑 焊促 奖霸 措眉窍扁 困秦 咯扁俊 helper 屈侥栏肺 父甸绢 初澜.
	// 角力肺 UIDoc 俊 府家胶 饭欺繁胶甫 糕滚肺 持瘤 富巴!
	//UB2InGameHUDInfo_Player* PlayerHUDInfo = StaticGetRelevantHUDInfo(InPCClass);
	//return PlayerHUDInfo ? PlayerHUDInfo->GetPlayerLargePortrait() : NULL;
	return NULL;
}

void UB2UIDocHero::AddSkillDoc(UB2UIDocSkill* NewSkillDoc, int32 SkillInfoSkillID, int32 ToPlaceInput)
{
	if (NewSkillDoc)
	{
		NewSkillDoc->SetOwnerHeroDoc(this);
		NewSkillDoc->SetAndLoadFromSkillID(SkillInfoSkillID); // SkillDoc 俊绰 悼利栏肺 函拳公街茄 单捞磐绰 酒付 绝阑 芭扼 扁夯利牢 单捞磐甸篮 咯扁辑 甸绢哎 巴.

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

//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
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

	//SetAttackPower( // 傈捧吝俊 饭骇诀捞 绝促搁 荤角惑 InitialLevel 肺 荤侩.
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
