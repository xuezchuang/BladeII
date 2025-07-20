// Fill out your copyright notice in the Description page of Project Settings.
#include "B2EtherData.h"
//#include "BladeII.h"

//#include "B2GameDefine.h"
//#include "BladeIIGameImpl.h"
//#include "B2CodeTable.h"
//#include "B2EtherManager.h"

FB2Ether::FB2Ether()
{
	ID = 0;
	OwnerPCClass = EPCClass::EPC_End;
	Star = 0;
	Tier = b2network::B2AetherTierType::SEAL;
	EnhanceLevel = 0;
	Part = ETHER::PART::MONE;
	IsLock = false;
	Type = b2network::B2AetherType::ALL;
	IsNew = false;
	MainOptionID = 0;
	//SetType = EtherSetType::OFF;
}

FB2Ether::FB2Ether(const b2network::B2AetherServerInfoPtr ptr)
{
	ID = ptr->id;
	Type = ptr->type;
	Part = ptr->slot;
	OwnerPCClass = SvrToCliPCClassType(ptr->character_type);
	EnhanceLevel = ptr->level;
	Star = ptr->grade;
	//tier는 기본 값 실제는 계산된 티어값을 사용한다
	Tier = ptr->calc_tier;
	MainOption.OptionType = SvrToCliOptionType(ptr->main_option_id);
	MainOptionID = ptr->main_option_id;
	//main_option_value 아이템의 기본 메인 값 사용 하지 않는다
	MainOption.RawOptionAmount = ptr->calc_main_option_value;

	EItemOption ThisOptionType = SvrToCliOptionType(ptr->sub_option_id1);
	if (ThisOptionType != EItemOption::EIO_End)
	{
		//sub_option_value1 마찬가지로 사용 안함
		SubOption.Add(FItemOption(ThisOptionType, ptr->calc_sub_option_value1));
	}

	ThisOptionType = SvrToCliOptionType(ptr->sub_option_id2);
	if (ThisOptionType != EItemOption::EIO_End)
	{
		//sub_option_value2 마찬가지로 사용 안함
		SubOption.Add(FItemOption(ThisOptionType, ptr->calc_sub_option_value2));
	}

	ThisOptionType = SvrToCliOptionType(ptr->sub_option_id3);
	if (ThisOptionType != EItemOption::EIO_End)
	{
		//sub_option_value3 마찬가지로 사용 안함
		SubOption.Add(FItemOption(ThisOptionType, ptr->calc_sub_option_value3));
	}

	IsLock = ptr->locked;
	IsNew = ptr->is_new;

	//b2network::B2mdAetherSetOptionInfoPtr SetOptionPtr = B2EtherManager::GetInstance().GetEtherSetOptionInfoPtrByType(Type, Tier);
	//if (SetOptionPtr != nullptr)
	//{
	//	if (SetOptionPtr->min_apply_count == EtherSetCount::DEFENCE) //방어 세트
	//	{
	//		SetType = EtherSetType::DEFENCE;
	//	}
	//	else if (SetOptionPtr->min_apply_count == EtherSetCount::OFFENCE) // 공격 세트
	//	{
	//		SetType = EtherSetType::OFFENCE;
	//	}
	//}
}

float FB2Ether::GetCombat()
{
	float Result = 0.0f;
	
	//Result += MainOption.RawOptionAmount * BladeIIGameImpl::GetClientDataStore().GetOptionWeight(MainOption.OptionType);

	//for (const FItemOption& ElemOption : SubOption)
	//{
	//	//Result += ElemOption.RawOptionAmount * BladeIIGameImpl::GetClientDataStore().GetOptionWeight(ElemOption.OptionType);
	//}

	return Result;
}

FB2EtherContainer::FB2EtherContainer()
{
	IsDirty = false;
}

void FB2EtherContainer::AddEther(const b2network::B2AetherServerInfoPtr EtherPtr)
{
	FB2Ether Ether(EtherPtr);
	if (Ether.OwnerPCClass == EPCClass::EPC_End)
	{
		StoredEthers.Add(Ether.ID, Ether);
	}
	else
	{
		if (EquippedEthers.Contains(Ether.OwnerPCClass) == false)
		{
			EquippedEthers.Add(Ether.OwnerPCClass, TMap<int64, FB2Ether>());
		}
		EquippedEthers[Ether.OwnerPCClass].Add(Ether.ID, Ether);
	}
}

void FB2EtherContainer::EquipEther(const FB2MountEtherPtr EtherPtr, const EPCClass PCClass)
{
	if (EquippedEthers.Contains(PCClass) == false)
	{
		EquippedEthers.Add(PCClass, TMap<int64, FB2Ether>());
	}
	//벗을거 있으면 벗고
	TakeOffEther(EtherPtr->unmounted_aether_ids, PCClass);

	//입힌다
	for (int64 EtherID : EtherPtr->mounted_aether_ids)
	{
		if (StoredEthers.Contains(EtherID))
		{
			StoredEthers[EtherID].IsNew = false;
			EquippedEthers[PCClass].Add(EtherID, StoredEthers[EtherID]);
			StoredEthers.Remove(EtherID);
		}
	}

	//SetDefenseSetIds(EtherPtr->defense_set_option_ids, PCClass);

	//BladeIIGameImpl::GetLocalCharacterData().SetEtherSetID(PCClass, EtherPtr->selected_offense_set_option_id, EtherPtr->selected_defense_set_option_id);

	////////////////////////////////////////////////////////////////////
	////액션 지표 로그 (에테르 장착)
	//B2ActionLogSender Sender;
	//Sender.setCategory(FString("CHARACTER"))
	//	.setAction(FString("AEHTER_EQUIP"))
	//	.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//////////////////////////////////////////////////////////////////
}

const FB2Ether* FB2EtherContainer::GetEther(int64 EtherID)
{
	FB2Ether* Result = nullptr;

	for (TPair<EPCClass, TMap<int64, FB2Ether>>& Elem : EquippedEthers)
	{
		if (Elem.Value.Contains(EtherID))
		{
			Result = &Elem.Value[EtherID];
			break;
		}
	}

	if (Result == nullptr && StoredEthers.Contains(EtherID))
	{
		Result = &StoredEthers[EtherID];
	}

	return Result;
}

void FB2EtherContainer::TakeOffEther(const TArray<int64>& IDs, const EPCClass PCClass)
{
	if (EquippedEthers.Contains(PCClass))
	{
		for (int64 ID : IDs)
		{
			if (EquippedEthers[PCClass].Contains(ID))
			{
				StoredEthers.Add(ID, EquippedEthers[PCClass][ID]);
				EquippedEthers[PCClass].Remove(ID);
			}
		}
	}
}

void FB2EtherContainer::TakeOffEther(const FB2UnmountEtherPtr EtherPtr, const EPCClass PCClass)
{
	TakeOffEther(EtherPtr->unmounted_aether_ids, PCClass);

	SetDefenseSetIds(EtherPtr->defense_set_option_ids, PCClass);

	//BladeIIGameImpl::GetLocalCharacterData().SetEtherSetID(PCClass, EtherPtr->selected_offense_set_option_id, EtherPtr->selected_defense_set_option_id);
}

void FB2EtherContainer::SetDefenseSetIds(const TArray<int32>& SetIds, const EPCClass PCClass)
{
	if (DefenseSetIds.Contains(PCClass) == false)
	{
		DefenseSetIds.Add(PCClass, TArray<int32>());
	}

	DefenseSetIds[PCClass].Empty();
	for (int32 Elem : SetIds)
	{
		DefenseSetIds[PCClass].Add(Elem);
	}
}

TArray<int32>& FB2EtherContainer::GetDefenseSetIds(const EPCClass PCClass)
{
	if (DefenseSetIds.Contains(PCClass) == false)
	{
		DefenseSetIds.Add(PCClass, TArray<int32>());
	}

	return DefenseSetIds[PCClass];
}

void FB2EtherContainer::MakeDirty()
{
	IsDirty = true;
}

bool FB2EtherContainer::GetDirty()
{
	return IsDirty;
}

void FB2EtherContainer::DeleteEther(int64 EtherID)
{
	bool IsDelete = false;

	for (TPair<EPCClass, TMap<int64, FB2Ether>>& Elem : EquippedEthers)
	{
		if (Elem.Value.Contains(EtherID))
		{
			Elem.Value.Remove(EtherID);
			IsDelete = true;
			break;
		}
	}

	if (IsDelete == false && StoredEthers.Contains(EtherID))
	{
		StoredEthers.Remove(EtherID);
	}
}

void FB2EtherContainer::ResetStoredEthers()
{
	EmptyScrollEthers();

	for (TPair<int64, FB2Ether>& Elem : StoredEthers)
	{
		if (ScrollEthers.Contains(Elem.Value.Type) == false)
		{
			ScrollEthers.Add(Elem.Value.Type, TArray<FB2Ether*>());
		}
		ScrollEthers[Elem.Value.Type].Add(&Elem.Value);

		if (ScrollEthers.Contains(b2network::B2AetherType::ALL) == false)
		{
			ScrollEthers.Add(b2network::B2AetherType::ALL, TArray<FB2Ether*>());
		}
		ScrollEthers[b2network::B2AetherType::ALL].Add(&Elem.Value);
	}

	SortStoredEthers();
}

void FB2EtherContainer::SortStoredEthers()
{
	for (TPair<int32, TArray<FB2Ether*>>& Elem : ScrollEthers)
	{
		Elem.Value.Sort([](const FB2Ether& A, const FB2Ether& B)
		{
			//뉴 > 잠금 >별등급 > 티어 > 타입(공격이 우선) > 포지션 > 강화 레벨 순
			bool Result = false;
			if (A.IsNew == true && B.IsNew == false)
			{
				Result = true;
			}
			else if (A.IsNew == B.IsNew)
			{
				if (A.IsLock == true && B.IsLock == false)
				{
					Result = true;
				}
				else if (A.IsLock == B.IsLock)
				{
					if (A.Star > B.Star)
					{
						Result = true;
					}
					else if (A.Star == B.Star)
					{
						if (A.Tier > B.Tier)
						{
							Result = true;
						}
						else if (A.Tier == B.Tier)
						{
							//if (A.SetType == EtherSetType::OFFENCE && B.SetType == EtherSetType::DEFENCE)
							//{
							//	Result = true;
							//}
							//else if(A.SetType == B.SetType)
							//{
							//	if (A.Type < B.Type)
							//	{
							//		Result = true;
							//	}
							//	else if (A.Type == B.Type)
							//	{
							//		if (A.Part < B.Part)
							//		{
							//			Result = true;
							//		}
							//		else if (A.Part == B.Part)
							//		{
							//			if (A.EnhanceLevel > B.EnhanceLevel)
							//			{
							//				Result = true;
							//			}
							//			else if (A.EnhanceLevel == B.EnhanceLevel)
							//			{
							//				if (A.ID > B.ID)
							//				{
							//					Result = true;
							//				}
							//			}
							//		}
							//	}
							//}
						}
					}
				}
			}
			return Result;
		});
	}
}

void FB2EtherContainer::EmptyScrollEthers()
{
	for (TPair<int32, TArray<FB2Ether*>>& Elem : ScrollEthers)
	{
		Elem.Value.Empty();
	}
	ScrollEthers.Empty();
}

void FB2EtherContainer::SetLock(int64 ID, bool InIsLock)
{
	bool Result = false;
	for (TPair<EPCClass, TMap<int64, FB2Ether>>& Elem : EquippedEthers)
	{
		if (Elem.Value.Contains(ID))
		{
			Result = true;
			Elem.Value[ID].IsLock = InIsLock;
			break;
		}
	}

	if (Result == false && StoredEthers.Contains(ID))
	{
		StoredEthers[ID].IsLock = InIsLock;
	}
}

void FB2EtherContainer::AllEtherNewFlagOff()
{
	//장착중인 에테르는 장착 할 때 new 지움
	for (TPair<int64, FB2Ether>& Elem : StoredEthers)
	{
		Elem.Value.IsNew = false;
	}
}

bool FB2EtherContainer::IsEtherNewFlag()
{
	bool Result = false;
	for (TPair<int64, FB2Ether>& Elem : StoredEthers)
	{
		if (Elem.Value.IsNew == true)
		{
			Result = true;
			break;
		}
	}
	return Result;
}

TArray<FB2Ether*>& FB2EtherContainer::GetScrollEther(int32 SetName)
{
	if (ScrollEthers.Contains(SetName) == false)
	{
		ScrollEthers.Add(SetName, TArray<FB2Ether*>());
	}

	return ScrollEthers[SetName];
}

void FB2EtherContainer::GetFilterEtherID(TArray<int64>& OutFilterEtherID, TArray<int32>& StarFilter, TArray<int32>& TierFilter, bool IsEnhance)
{
	// 차례대로 필터 적용
	TArray<FB2Ether> Result;
	StoredEthers.GenerateValueArray(Result);

	Result = Result.FilterByPredicate(
		[&StarFilter](FB2Ether CurrEther)
	{
		for (int Star : StarFilter)
		{
			if (CurrEther.Star == Star)
			{
				return true;
			}
		}
		return false;
	});

	Result = Result.FilterByPredicate(
		[&TierFilter](FB2Ether CurrEther)
	{
		for (int Tier : TierFilter)
		{
			if (CurrEther.Tier == Tier)
			{
				return true;
			}
		}
		return false;
	});

	if (!IsEnhance)
	{
		Result = Result.FilterByPredicate(
			[&TierFilter](FB2Ether CurrEther)
		{
			if (CurrEther.EnhanceLevel <= 0)
			{
				return true;
			}
			return false;
		});
	}

	for (FB2Ether& Elem : Result)
	{
		if (Elem.IsLock == false)
		{
			OutFilterEtherID.Add(Elem.ID);
		}
	}
}

TMap<int64, FB2Ether>& FB2EtherContainer::GetEquippedEthers(EPCClass PCClass)
{
	if (EquippedEthers.Contains(PCClass) == false)
	{
		EquippedEthers.Add(PCClass, TMap<int64, FB2Ether>());
	}

	return EquippedEthers[PCClass];
}

int FB2EtherContainer::GetEtherTotalCount()
{
	int Result = 0;

	Result += StoredEthers.Num();

	for (TPair<EPCClass, TMap<int64, FB2Ether>>& Elem : EquippedEthers)
	{
		Result += Elem.Value.Num();
	}
	return Result;
}

void FB2EtherContainer::GetEquippedEtherOption(TMap<EItemOption, float>& OutOption, EPCClass PCClass)
{
	if (EquippedEthers.Contains(PCClass))
	{
		for (TPair<int64, FB2Ether>& Elem : EquippedEthers[PCClass])
		{
			if (Elem.Value.MainOption.OptionType != EItemOption::EIO_End)
			{
				if (OutOption.Contains(Elem.Value.MainOption.OptionType) == false)
				{
					OutOption.Add(Elem.Value.MainOption.OptionType, Elem.Value.MainOption.RawOptionAmount);
				}
				else
				{
					OutOption[Elem.Value.MainOption.OptionType] += Elem.Value.MainOption.RawOptionAmount;
				}
			}

			for (int i = 0; i < Elem.Value.SubOption.Num(); ++i)
			{
				if (Elem.Value.SubOption[i].OptionType != EItemOption::EIO_End)
				{
					if (OutOption.Contains(Elem.Value.SubOption[i].OptionType) == false)
					{
						OutOption.Add(Elem.Value.SubOption[i].OptionType, Elem.Value.SubOption[i].RawOptionAmount);
					}
					else
					{
						OutOption[Elem.Value.SubOption[i].OptionType] += Elem.Value.SubOption[i].RawOptionAmount;
					}
				}
			}
		}

		OutOption.KeySort([](EItemOption A, EItemOption B)
		{
			bool Result = false;
			if ((uint8)A < (uint8)B)
			{
				Result = true;
			}
			return Result;
		}
		);
	}
}

void FB2EtherContainer::SetEquippedEther(const int32 InvenSlotCount, const TArray<b2network::B2AetherServerInfoPtr>& InEquippedEther)
{
	for (TPair<EPCClass, TMap<int64, FB2Ether>>& EquippedEther : EquippedEthers)
	{
		EquippedEther.Value.Empty();
	}
	EquippedEthers.Empty();

	for (b2network::B2AetherServerInfoPtr Elem : InEquippedEther)
	{
		AddEther(Elem);
	}
}

void FB2EtherContainer::SetAccountEther(const FB2GetAccountEtherPtr EtherPtr)
{
	IsDirty = false;

	for (TPair<EPCClass, TMap<int64, FB2Ether>>& EquippedEther : EquippedEthers)
	{
		EquippedEther.Value.Empty();
	}
	EquippedEthers.Empty();

	StoredEthers.Empty();

	for (b2network::B2AetherServerInfoPtr Elem : EtherPtr->aethers)
	{
		AddEther(Elem);
	}

	for (int i = 0; i < EtherPtr->current_character_set_option_Infos.Num(); ++i)
	{
		EPCClass PCClass = SvrToCliPCClassType(i + 1);
		SetDefenseSetIds(EtherPtr->current_character_set_option_Infos[i]->defense_set_option_ids, PCClass);
	}
}
