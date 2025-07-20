// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "B2MessageInfoCommonStruct.h"
#include "CommonStruct.h"
/**
 * 
 */


namespace ETHER
{
	namespace PART
	{
		const int32 MONE = 1;
		const int32 MTWO = 2;
		const int32 MTHREE = 3;
		const int32 MFOUR = 4;
		const int32 MFIVE = 5;
		const int32 MSIX = 6;
	}
}

struct FB2Ether
{
	FB2Ether();
	FB2Ether(const b2network::B2AetherServerInfoPtr ptr);
	float GetCombat();

	int64 ID;
	EPCClass OwnerPCClass;
	int32 Star;
	int32 Tier;
	int32 EnhanceLevel;
	int32 Part;
	bool IsLock;
	bool IsNew;
	int32 Type;
	FItemOption MainOption;
	int32 MainOptionID;
	TArray<FItemOption> SubOption;
	int32 SetType;
};

struct FB2EtherContainer
{
	TMap<int64, FB2Ether>										StoredEthers;
	TMap<int32, TArray<FB2Ether*>>								ScrollEthers;
	TMap<EPCClass, TMap<int64, FB2Ether>>						EquippedEthers;
	bool														IsDirty;
	TMap<EPCClass, TArray<int32>>								DefenseSetIds;

	FB2EtherContainer();
	void AddEther(const b2network::B2AetherServerInfoPtr EtherPtr);
	const FB2Ether* GetEther(int64 EtherID);
	void ResetStoredEthers();
	void SortStoredEthers();
	TArray<FB2Ether*>& GetScrollEther(int32 SetName);
	void GetFilterEtherID(TArray<int64>& OutFilterEtherID, TArray<int32>& StarFilter, TArray<int32>& TierFilter, bool IsEnhance);
	TMap<int64, FB2Ether>& GetEquippedEthers(EPCClass PCClass);
	int GetEtherTotalCount();
	void GetEquippedEtherOption(TMap<EItemOption, float>& OutOption, EPCClass PCClass);
	void EquipEther(const FB2MountEtherPtr EtherPtr, const EPCClass PCClass);
	void TakeOffEther(const FB2UnmountEtherPtr EtherPtr, const EPCClass PCClass);
	void TakeOffEther(const TArray<int64>& IDs, const EPCClass PCClass);
	void SetDefenseSetIds(const TArray<int32>& SetIds, const EPCClass PCClass);
	TArray<int32>& GetDefenseSetIds(const EPCClass PCClass);
	void MakeDirty();
	bool GetDirty();
	void DeleteEther(int64 EtherID);
	void SetEquippedEther(const int32 InvenSlotCount, const TArray<b2network::B2AetherServerInfoPtr>& InEquippedEther);
	void SetAccountEther(const FB2GetAccountEtherPtr EtherPtr);
	void EmptyScrollEthers();
	void SetLock(int64 ID, bool InIsLock);
	void AllEtherNewFlagOff();
	bool IsEtherNewFlag();
};