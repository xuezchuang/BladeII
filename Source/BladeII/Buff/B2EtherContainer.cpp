// Fill out your copyright notice in the Description page of Project Settings.


#include "B2EtherContainer.h"
#include "B2EtherManager.h"
#include "BladeIIUtil.h"
#include "B2UIDocHelper.h"
#include "BladeIIUtil.h"
#include "B2EtherInfo.h"

TSubclassOf<UB2EtherSetEffect> UB2EtherSetInfoTable::GetEtherSetEffectClass(int32 EtherSetID)
{
	if (TSubclassOf<UB2EtherSetEffect>* EtherClassBP = EtherSetClasses.Find(EtherSetID))
		return *EtherClassBP;

	return nullptr;
}

void UB2EtherContainer::InitializeContainer(class ABladeIIPlayer* EtherOwner)
{
	/*if (EtherOwner && EtherOwner->IsValidObj())
	{
		const EPCClass OwnerClass = EtherOwner->GetCurrentPlayerClass();
		if (OwnerClass != EPCClass::EPC_End)
		{
			OffenseEther = InitEtherSetEffect(EtherOwner, EEtherSetType::Offense);
			if (OffenseEther && EtherOwner->IsLocalPlayer())
			{
				if (auto* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(OwnerClass)))
				{
					float testets = DocHero->GetCurOffenseEtherCoolTime();
					OffenseEther->SetEtherCondition(DocHero->GetCurOffenseEtherCoolTime());
				}
			}

			DefenseEther = InitEtherSetEffect(EtherOwner, EEtherSetType::Defense);
			if (DefenseEther && EtherOwner->IsLocalPlayer())
			{
				if (auto* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(OwnerClass)))
				{
					DefenseEther->SetEtherCondition(DocHero->GetCurDefenseEtherCoolTime());
				}
			}
		}

		if(EtherOwner->IsLocalPlayer())
			InitializeDocBattle(UB2UIDocHelper::GetDocBattle());

		SetOwnerPlayer(EtherOwner);
	}*/
}

void UB2EtherContainer::InitializeDocBattle(UB2UIDocBattle* DocBattle)
{
	//if (DocBattle && StaticFindEtherInfoTable())
	//{
	//	// Offense Ether
	//	if (auto* OffenseEtherObj = GetOffenseEtherSet())
	//	{
	//		DocBattle->SetOffenseSetID(StaticFindEtherInfoTable()->GetTypeID(OffenseEtherObj->GetEtherSetID()));
	//		DocBattle->SetIsOffenseEther(true);
	//	}
	//	else
	//	{
	//		DocBattle->SetOffenseSetID(INDEX_NONE);
	//		DocBattle->SetIsOffenseEther(false);
	//	}

	//	// Defense Ether
	//	if (auto* DefenseEtherObj = GetDefenseEtherSet())
	//	{
	//		DocBattle->SetDefenseSetID(StaticFindEtherInfoTable()->GetTypeID(DefenseEtherObj->GetEtherSetID()));
	//		DocBattle->SetIsDefenseEther(true);
	//	}
	//	else
	//	{
	//		DocBattle->SetDefenseSetID(INDEX_NONE);
	//		DocBattle->SetIsDefenseEther(false);
	//	}
	//}
}

void UB2EtherContainer::SetOwnerPlayer(class ABladeIIPlayer* EtherOwner)
{
	OwnerPlayer = EtherOwner;
}

UB2EtherSetEffect* UB2EtherContainer::GetEtherSetEffect(EEtherSetType EtherSetType)
{
	switch (EtherSetType)
	{
	case EEtherSetType::Offense: return OffenseEther;
	case EEtherSetType::Defense: return DefenseEther;
	}

	return nullptr;
}

 UB2EtherSetEffect* UB2EtherContainer::GetEtherSetEffect(const int32 EtherSetdID)
{
	if (OffenseEther && OffenseEther->GetEtherSetID() == EtherSetdID) return OffenseEther;
	if (DefenseEther && DefenseEther->GetEtherSetID() == EtherSetdID) return DefenseEther;

	return nullptr;
}

bool UB2EtherContainer::AddEtherSetEffect(const int32 EtherSetID, float TriggerRate, float CoolTime)
{
	//TSubclassOf<UB2EtherSetEffect> DefaultClass = StaticFindEtherSetEffectInfo()->GetEtherSetEffectClass(EtherSetID);
	//if (UB2EtherSetEffect* DefaultObj = DefaultClass.GetDefaultObject())
	//{
	//	EEtherSetType SetType = DefaultObj->GetEtherSetType();
	//	
	//	RemoveEtherSetEffect(SetType); // 일단 지우고

	//	UB2EtherSetEffect* NewEtherSetObject = CreateEtherSetEffect(SetType, EtherSetID);
	//	if (NewEtherSetObject)
	//	{
	//		NewEtherSetObject->Initialize(OwnerPlayer, TriggerRate, CoolTime);

	//		if (SetType == EEtherSetType::Offense)		OffenseEther = NewEtherSetObject;
	//		else if (SetType == EEtherSetType::Defense)	DefenseEther = NewEtherSetObject;

	//		
	//		return true;
	//	}
	//}

	return false;
}

UB2EtherSetEffect* UB2EtherContainer::InitEtherSetEffect(ABladeIIPlayer* EtherOwner, EEtherSetType EtherSetType)
{
	ICharacterDataStore* OwnerInfo = EtherOwner ? EtherOwner->GetCharacterDataStore() : nullptr;
	if(OwnerInfo)
	{
		const EPCClass OwnerClass = EtherOwner->GetCurrentPlayerClass();
		TMap<int64, FB2Ether> OwnerEthers;
		OwnerInfo->GetEquippedEthers(OwnerClass, OwnerEthers);

		int32 EtherSetID = 0;
		if (EtherSetType == EEtherSetType::Offense)			EtherSetID = OwnerInfo->GetEtherOffenseSetID(OwnerClass);
		else if (EtherSetType == EEtherSetType::Defense)	EtherSetID = OwnerInfo->GetEtherDefenceSetID(OwnerClass);

		//test
		//if (EtherSetType == EEtherSetType::Offense)			EtherSetID = b2network::B2AetherSetOptionType::SKILL_GAGE;
		//else if (EtherSetType == EEtherSetType::Defense)	EtherSetID = b2network::B2AetherSetOptionType::DAMAGE_REFLECT;


		UB2EtherSetEffect* EtherSetObject = CreateEtherSetEffect(EtherSetType, EtherSetID);
		if (EtherSetObject)
		{
			B2EtherManager& EtherManager = B2EtherManager::GetInstance();
			const float TriggerRate = EtherManager.GetEtherSetOptionTriggerRate(EtherSetID, OwnerEthers);
			const float EtherCoolDown = EtherManager.GetEtherSetOptionCoolDown(EtherSetID, OwnerEthers);

			// test
			//const float TriggerRate = 1.0f;
			//const float EtherCoolDown = 5.0f;
			// test

			EtherSetObject->Initialize(EtherOwner, TriggerRate, EtherCoolDown);
			return EtherSetObject;
		}
	}

	return nullptr;
}

UB2EtherSetEffect* UB2EtherContainer::CreateEtherSetEffect(EEtherSetType EtherSetType, int32 EtherID)
{
	//TSubclassOf<UB2EtherSetEffect> DefaultClass = StaticFindEtherSetEffectInfo()->GetEtherSetEffectClass(EtherID);
	//if (DefaultClass.Get() != nullptr)
	//{
	//	TSubclassOf<UB2EtherSetEffect> EtherSetClass = *DefaultClass;
	//	UB2EtherSetEffect* EtherSetObject = NewObject<UB2EtherSetEffect>(this, EtherSetClass);

	//	return EtherSetObject;
	//}

	return nullptr;
}

void UB2EtherContainer::RemoveEtherSetEffect(EEtherSetType EtherSetType)
{
	switch (EtherSetType)
	{
	case EEtherSetType::Offense: if (OffenseEther) OffenseEther = nullptr; break;
	case EEtherSetType::Defense: if (DefenseEther) DefenseEther = nullptr; break;
	}
}

void UB2EtherContainer::RealtimeUpdateUIDoc(UB2UIDocBattle* DocBattle)
{
	if (!DocBattle)
		return;

	if (OffenseEther)
	{
		DocBattle->SetOffenseEtherCoolTime(OffenseEther->GetCurrentCoolTime());
		//DocBattle->SetOffenseEtherMaxCoolTime(OffenseEther->GetMaxCoolTime());
		//DocBattle->SetEnableOffenseEther(OffenseEther->GetEnableCoolTime());
		//DocBattle->SetIsOffenseEther(true);
	}
	else
	{
		if(DocBattle->GetIsOffenseEther())
			DocBattle->SetIsOffenseEther(false);
	}

	if (DefenseEther)
	{
		DocBattle->SetDefenseEtherCoolTime(DefenseEther->GetCurrentCoolTime());
		//DocBattle->SetDefenseEtherMaxCoolTime(DefenseEther->GetMaxCoolTime());
		//DocBattle->SetEnableDefenseEther(DefenseEther->GetEnableCoolTime());
		//DocBattle->SetIsDefenseEther(true);
	}
	else 
	{
		if(DocBattle->GetIsDefenseEther())
			DocBattle->SetIsDefenseEther(false);
	}
}

void UB2EtherContainer::UpdateCoolTime(float DeltaSeconds)
{
	if (OffenseEther)
	{
		OffenseEther->UpdateCoolDown(DeltaSeconds);
	}
	
	if (DefenseEther)
	{
		DefenseEther->UpdateCoolDown(DeltaSeconds);
	}
}
