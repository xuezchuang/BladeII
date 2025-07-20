

#include "FairyManager.h"
#include "Event.h"
#include "Retailer.h"
#include "OnlineWholesaler.h"
#include "B2StageDataStore.h"
#include "B2StageManager.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"

#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "FB2ErrorMessage.h"
#include "../InfoAsset/B2StageInfo.h"
//#include "../InfoAsset/B2StageInfo.h"
//#include "../Lobby/B2Inventory.h"
//#include "../Common/CommonStruct.h"

UTexture2D* FFairyBelssInfo::GetFairySkillIconTexture() const
{
	return SkillIconTexture.LoadSynchronous();
}

const FFairyBelssInfo* UB2FairyInfo::GetFairyBelssInfo(int32 FairyType, int32 OptionID)
{
	if (FairyBlessInfoTable == nullptr)
		return nullptr;

	if (FairyBlessInfoTable)
	{
		FString RowString = FString::Printf(TEXT("FairyBless_%d%d"), (FairyType * 10), OptionID);
		FName FindRowName = FName(*RowString);
		return FairyBlessInfoTable->FindRow<FFairyBelssInfo>(FindRowName, TEXT(""));
	}

	return nullptr;
}

void UB2FairyInfo::SetDataTable(UDataTable* InDataTable)
{
	if (InDataTable == NULL)
		return;

	FairyBlessInfoTable = InDataTable;

#if WITH_EDITOR
	CheckInfoDataIntegrity();
#endif
}

#if WITH_EDITOR
void UB2FairyInfo::CheckInfoDataIntegrity()
{
#if !PLATFORM_MAC
	if (!FairyBlessInfoTable)
	{
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("FairyBlessInfoTable 单捞磐 绝澜. 哪腔磐啊 梆 气惯茄促."))
		));
		return;
	}
#endif
}

void UB2FairyInfo::EditorLoadAll()
{
	if (FairyBlessInfoTable)
	{
		TArray<FName> RowKeys = FairyBlessInfoTable->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			FFairyBelssInfo* FoundData = FairyBlessInfoTable->FindRow<FFairyBelssInfo>(RowKeys[RI], TEXT(""));
			if (FoundData)
			{
				FoundData->SkillIconTexture.LoadSynchronous();
			}
		}
	}
}
#endif

void FB2FairyBlessSlotInfo::UpdateInfo(const FMD_FairyInfo& Info, const FB2FairyStatusInfo& Status)
{
	FairyType = Info.FairyType;
	OptionId = Info.OptionId;
	BlessLevel = Info.BlessLevel;
	OpenLevel = Info.Level;

	bOpendBless = Status.bClearStage ? Info.Level <= Status.Level : false;
	bEquiped = Info.OptionId == Status.ActivedOptoinId;
	OptionValue = Info.OptionValue;
}

FB2FairyStatusInfo::FB2FairyStatusInfo(const b2network::B2FairyStatusPtr & ServerStruct)
{
	*this = ServerStruct;
}

FB2FairyStatusInfo& FB2FairyStatusInfo::operator=(const b2network::B2FairyStatusPtr& ServerStruct)
{
	if (ServerStruct)
	{
		FairyType = SvrToCliFairyType(ServerStruct->fairy_type);
		//bClearStage = FairyManager::GetInstance().GetIsOpenedFairy(FairyType);
		Level = ServerStruct->level;
		EnhancePoint = ServerStruct->enhance_point;
		ActivedOptoinId = ServerStruct->activated_bless_option_id;
	}
	return *this;
}

FairyManager::FairyManager()
		: bSubscribed(false)
		, CachedLevelUpMax(-1)
{
}

FairyManager::~FairyManager()
{
}

FairyManager& FairyManager::GetInstance()
{
	static FairyManager Instance;
	return Instance;
}

void FairyManager::Init()
{
	SubscribeEvents();

	if (bSubscribed == false)
	{
		bSubscribed = true;
	}
}

void FairyManager::SubscribeEvents()
{
	UnsubscribeEvents();

	//Issues.Add(DeliveryGetFairyClass<FB2GetFairyPtr>::GetInstance().Subscribe2(
	//	[this](const FB2GetFairyPtr& data)
	//	{
	//		OnReceivedGetFairy(data);
	//	}
	//));
	//Issues.Add(DeliveryLevelupFairyClass<FB2LevelupFairyPtr>::GetInstance().Subscribe2(
	//	[this](const FB2LevelupFairyPtr& data)
	//	{
	//		OnReceivedLevelUpFairy(data);
	//	}
	//));

	//Issues.Add(DeliveryLevelupFairyErrorClass<FB2LevelupFairyPtr>::GetInstance().Subscribe2(
	//	[this](const FB2LevelupFairyPtr& data)
	//	{
	//		OnErrorReceivedLevelUpFairy(data);
	//	}
	//));

	//Issues.Add(DeliveryBlessFairyClass<FB2BlessFairyPtr>::GetInstance().Subscribe2(
	//	[this](const FB2BlessFairyPtr& data)
	//	{
	//		OnReceivedBlessFairy(data);
	//	}
	//));
}

void FairyManager::UnsubscribeEvents()
{
	Issues.Empty();
}

void FairyManager::UpdateFairyStatus(const TArray<b2network::B2FairyStatusPtr> &Infos)
{
	CachedFairyInfoMap.Empty();

	for (int32 i = CliToSvrFairyType(EFairyType::FAIRY_NIECE); i < CliToSvrFairyType(EFairyType::FAIRY_MAX); ++i)
	{
		EFairyType FairyType = SvrToCliFairyType(i);

		FB2FairyStatusInfo StatusInfo(FairyType);
		StatusInfo.MaxLevel = GetFairyLevelUpMax();
		StatusInfo.OptionValue = 0.0f;
		//StatusInfo.OpenStageName;

		CachedFairyInfoMap.Add(FairyType, StatusInfo);
	}

	for (const auto& Elem : Infos)
	{
		UpdateFairyStatus(Elem);
	}
}

void FairyManager::UpdateFairyStatus(const b2network::B2FairyStatusPtr& Info)
{
	EFairyType FairyType = SvrToCliFairyType(Info->fairy_type);
	if (CachedFairyInfoMap.Contains(FairyType))
	{
		FB2FairyStatusInfo* Status = CachedFairyInfoMap.Find(FairyType);
		*Status = Info;
		Status->OptionValue = 0.0f;
		Status->bClearStage = GetIsOpenedFairy(FairyType);

		FMD_FairyInfo FairyInfo;

		if (GetFairyOpendBlessInfo(FairyType, Status->ActivedOptoinId, Status->Level, FairyInfo))
		{
			Status->OptionValue = FairyInfo.OptionValue;
		}
	}
}

TArray<FB2FairyStatusInfo> FairyManager::ModUpdateFairyStatus(const TArray<b2network::B2FairyStatusPtr>& Infairies)
{
	TArray<FB2FairyStatusInfo> OutFairyStatus;

	for (const b2network::B2FairyStatusPtr& Status : Infairies)
	{
		int32 OutFairytIndex = OutFairyStatus.Emplace(Status);

		EFairyType FairyType = SvrToCliFairyType(Status->fairy_type);
		if (CachedFairyInfoMap.Contains(FairyType))
		{
			FMD_FairyInfo FairyInfo;

			if (GetFairyOpendBlessInfo(FairyType, Status->activated_bless_option_id, Status->level, FairyInfo))
			{
				OutFairyStatus[OutFairytIndex].OptionValue = FairyInfo.OptionValue;
			}
		}
	}

	return OutFairyStatus;
}

bool FairyManager::GetFairyInfo(EFairyType Fairy, FB2FairyStatusInfo& OutData)
{
	if (CachedFairyInfoMap.Contains(Fairy))
	{
		OutData = CachedFairyInfoMap[Fairy];
		return true;
	}
	return false;
}

void FairyManager::GetFairyInfos(TArray<FB2FairyStatusInfo>& OutData)
{
	for(const auto & FairyInfo : CachedFairyInfoMap)
	{
		OutData.Add(FairyInfo.Value);
	}
}

bool FairyManager::GetFairyNextLevelInfo(EFairyType Fairy, FB2FairyStatusInfo &OutData)
{
	if (CachedFairyInfoMap.Contains(Fairy))
	{
		FB2FairyStatusInfo Temp = CachedFairyInfoMap[Fairy];

		FMD_FairyInfo FairyInfo;
		if (GetFairyNextBlessInfo(Fairy, Temp.Level + 1, FairyInfo))
		{
			Temp.Level = FairyInfo.Level;
			Temp.ActivedOptoinId = FairyInfo.OptionId;
			Temp.OptionValue = FairyInfo.OptionValue;

			OutData = Temp;
			return true;
		}

		return false;
	}

	return false;
}

FText FairyManager::GetFairyName(EFairyType Type)
{
	switch (Type)
	{
	case EFairyType::FAIRY_NIECE:
		return BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("NPCClass_FairyNice"));
	case EFairyType::FAIRY_PANG:
		return BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("NPCClass_FairyPan"));
	case EFairyType::FAIRY_RIO:
		return BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("NPCClass_FairyRio"));
	}
	return FText();
}

FText FairyManager::GetBlessOptionName(int32 OptionId)
{
	EItemOption CurrOption = SvrToCliOptionType(OptionId);
	return GetLOCTextOfItemOption(CurrOption, EPCClass::EPC_End);
}

FText FairyManager::GetFairyOpenStageName(EFairyType Type)
{
	FText DiffText;
	EStageDifficulty Difficulty = GetFairyOpenStageDifficulty(Type);
	switch (Difficulty)
	{
	case EStageDifficulty::ESD_Normal:
		DiffText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_DifficultyEasy"));
		break;
	case EStageDifficulty::ESD_Hard:
		DiffText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_DifficultyHard"));
		break;
	case EStageDifficulty::ESD_VeryHard:
		DiffText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_DifficultyVeryHard"));
		break;
	case EStageDifficulty::ESD_Hell:
		DiffText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_DifficultyHell"));
		break;
	}

	int32 ChapterNum = 0;
	int32 SubNum = 0;
	if (FSingleStageInfoData* StageInfo = GetFairyOpenStageInfo(Type))
	{
		ChapterNum = StageInfo->ChapterNum;
		SubNum = StageInfo->SubChapterNum;
	}

	return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_OpenStage")), DiffText, FText::AsNumber(ChapterNum), FText::AsNumber(SubNum));
}

const FFairyBelssInfo* FairyManager::GetFairyBlessInfo(EFairyType Target, int32 OptionId)
{
	int32 FairyType = CliToSvrFairyType(Target);
	UB2FairyInfo* FairyInfo = StaticFindFairyInfo();
	if (FairyInfo)
	{
		return FairyInfo->GetFairyBelssInfo(FairyType, OptionId);
	}
	return nullptr;
}

void FairyManager::RequestGetFairy()
{
	data_trader::Retailer::GetInstance().RequestGetFairy();
}

void FairyManager::RequestLevelUpFairy(const EFairyType TargetFairy)
{
	const int32 Fairy = CliToSvrFairyType(TargetFairy);
	data_trader::Retailer::GetInstance().RequestLevelupFairy(Fairy);
}

void FairyManager::RequestBlessFairy(const EFairyType TargetFairy, const int32 BlessOptionId)
{
	const int32 Fairy = CliToSvrFairyType(TargetFairy);
	data_trader::Retailer::GetInstance().RequestBlessFairy(Fairy, BlessOptionId);
}

void FairyManager::OnReceivedGetFairy(const FB2GetFairyPtr& data)
{
	UpdateFairyStatus(data->fairies);

	UpdateFairyStatusClass<>::GetInstance().Signal();
}

void FairyManager::OnReceivedLevelUpFairy(const FB2LevelupFairyPtr& data)
{
	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	if (data->deleted_item_id > 0)
	{
		BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().RemoveItem(data->deleted_item_id);
	}
	else
	{
		if (data->changed_material_item)
			BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountIncrease(data->changed_material_item->template_id, data->changed_material_item);
	}

	UpdateFairyStatus(data->updated_fairy);

	EFairyType FairyType = SvrToCliFairyType(data->updated_fairy->fairy_type);
	bool IsLevelUp = data->is_levelup;
	UpdateFairyLevelUpClass<EFairyType, bool>::GetInstance().Signal(FairyType, IsLevelUp);

	RefreshCombatInfo();
}

void FairyManager::OnErrorReceivedLevelUpFairy(const FB2LevelupFairyPtr& data)
{
	//if (data->deleted_item_id > 0)
	//{
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().RemoveItem(data->deleted_item_id);
	//}
	//else
	//{
	//	if (data->changed_material_item)
	//		BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountIncrease(data->changed_material_item->template_id, data->changed_material_item);
	//}
	//UpdateFairyLevelUpErrorClass<int32>::GetInstance().Signal(data->GetErrorCode());
}

void FairyManager::OnReceivedBlessFairy(const FB2BlessFairyPtr& data)
{
	UpdateFairyStatus(data->updated_fairy);

	EFairyType FairyType = SvrToCliFairyType(data->updated_fairy->fairy_type);
	UpdateFairyBlessClass<EFairyType>::GetInstance().Signal(FairyType);

	RefreshCombatInfo();
}

bool FairyManager::GetMDFairyOpenInfoAll(TArray<FMD_FairyOpenInfo>& OutList)
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	return CliDataStore.GetFairyOpenInfoAll(OutList);
}

bool FairyManager::GetMDFairyOpenInfo(EFairyType Target, FMD_FairyOpenInfo& OutData)
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	return CliDataStore.GetFairyOpenInfo(Target, OutData);
}

bool FairyManager::GetMDFairyInfoAll(EFairyType Target, TArray<FMD_FairyInfo>& OutList)
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	return CliDataStore.GetFairyInfoAll(Target, OutList);
}

//bool FairyManager::GetMDFairyInfoOptionIdAll(EFairyType Target, TArray<int32> &OutList)
//{
//	struct FB2FairyOptionIdSorter
//	{
//		bool operator()(const FMD_FairyInfo& A, const FMD_FairyInfo& B) const
//		{
//			return A.OptionId > B.OptionId;
//		}
//	};
//
//	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
//	TArray<FMD_FairyInfo> TempList;
//	if (CliDataStore.GetFairyInfoAll(Target, TempList))
//	{
//		TempList.Sort(FB2FairyOptionIdSorter());
//
//		int32 PrevOptionId = -1;
//		if (TempList.Num() > 0)
//		{
//			PrevOptionId = TempList[0].OptionId;
//			OutList.Add(PrevOptionId);
//		}
//		for (FMD_FairyInfo &Each : TempList)
//		{
//			if (PrevOptionId != Each.OptionId)
//			{
//				PrevOptionId = Each.OptionId;
//				OutList.Add(PrevOptionId);
//			}
//		}
//		return OutList.Num() > 0;
//	}
//	return false;
//}

bool FairyManager::GetMDFairyInfo(EFairyType Target, int32 OptionId, int32 BlessLevel, FMD_FairyInfo& OutData)
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	return CliDataStore.GetFairyInfo(Target, OptionId, BlessLevel, OutData);
}

bool FairyManager::GetMDFairyLevelUpCostAll(TArray<FMD_FairyLevelUpCost>& OutList)
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	return CliDataStore.GetFairyLevelUpCostAll(OutList);
}

//bool FairyManager::GetMDFairyLevelUpCost(int32 TargetLevel, FMD_FairyLevelUpCost &OutData)
//{
//	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
//	return CliDataStore.GetFairyLevelUpCost(TargetLevel, OutData);
//}
//
bool FairyManager::GetIsOpenedFairy(const EFairyType TargetFairy)
{
	FMD_FairyOpenInfo FairyInfo;

	//if (GetMDFairyOpenInfo(TargetFairy, FairyInfo))
	//{
	//	FServerStageID ServerStageID = FServerStageID(FairyInfo.StageId);

	//	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	//	UB2StageInfo* StageInfoTable = StaticFindStageInfo();

	//	const FServerStageID LastClearServerStageID = StageDataStore.GetLastClearServerStageIdByLastPlayedStageDifficulty();
	//	const int32 LastClearClientStageID = StageDataStore.GetClientStageId(LastClearServerStageID);

	//	FServerStageID RequestServerStageID = (ServerStageID.IsEmpty() == false) ? ServerStageID : LastClearServerStageID;
	//	int32 RequestClientStageID = StageDataStore.GetClientStageId(RequestServerStageID);

	//	int32 LastDifficulty = static_cast<int32>(StageDataStore.GetStageDifficulty(LastClearServerStageID));

	//	int32 RequestDifficuly = static_cast<int32>(StageDataStore.GetStageDifficulty(RequestServerStageID));

	//	int32 RequestActNumber = StageDataStore.GetActIdByClientStageId(RequestClientStageID);
	//	int32 LastClearStageActNumber = StageDataStore.GetActIdByClientStageId(LastClearClientStageID);
	//	EStageDifficulty Difficulty = StageDataStore.GetStageDifficulty(RequestServerStageID);

	//	// 矮 6厘 柳涝 饶 胶抛捞瘤 努府绢矫 抄捞档啊 1肺 檬扁拳登扁 锭巩俊 Act 甫 烹秦 惑困 咀飘甫 柳青吝捞扼搁 凯赴 巴栏肺 牢瘤茄促.
	//	if (LastClearStageActNumber > RequestActNumber)
	//	{
	//		return true;
	//	}

	//	bool bSixChapterOrMore = false;
	//	if (RequestActNumber > StageInfoTable->GetStartAdventureModeChapterNum())
	//	{
	//		bSixChapterOrMore = !StageDataStore.GetNewChapterOpenState(RequestActNumber);
	//	}

	//	if (((RequestDifficuly > LastDifficulty)
	//		|| (RequestDifficuly == LastDifficulty && RequestClientStageID > LastClearClientStageID))
	//		|| bSixChapterOrMore)
	//	{
	//		return false;
	//	}

	//	return true;
	//}
	//else
	{
		return false;
	}
}

EStageDifficulty FairyManager::GetFairyOpenStageDifficulty(const EFairyType TargetFairy)
{
	FMD_FairyOpenInfo FairyInfo;

	if (GetMDFairyOpenInfo(TargetFairy, FairyInfo))
	{
		UB2StageInfo* StageInfoTable = StaticFindStageInfo();

		auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
		FServerStageID ServerId = FServerStageID(FairyInfo.StageId);

		return StageDataStore.GetStageDifficulty(ServerId);
	}
	else
	{
		return EStageDifficulty::ESD_None;
	}
}

FSingleStageInfoData* FairyManager::GetFairyOpenStageInfo(const EFairyType TargetFairy)
{
	FMD_FairyOpenInfo FairyInfo;

	if (GetMDFairyOpenInfo(TargetFairy, FairyInfo))
	{
		UB2StageInfo* StageInfoTable = StaticFindStageInfo();

		auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
		FServerStageID ServerId = FServerStageID(FairyInfo.StageId);

		const int32 ClientStageId = StageDataStore.GetClientStageId(ServerId);
		return StageInfoTable->GetInfoData(ClientStageId);
	}
	else
	{
		return nullptr;
	}
}

bool FairyManager::GetActivatedFairyBlessInfo(const EFairyType TargetFairy, int32 OptionId, int32 FairyLevel, FMD_FairyInfo& OutData)
{
	return GetMDFairyInfo(TargetFairy, OptionId, FairyLevel, OutData);
}

bool FairyManager::GetActivatedFairyBlessInfo(const EFairyType TargetFairy, FMD_FairyInfo& OutData)
{
	FB2FairyStatusInfo StatusInfo;
	if (GetFairyInfo(TargetFairy, StatusInfo))
	{
		return GetMDFairyInfo(TargetFairy, StatusInfo.ActivedOptoinId, StatusInfo.Level, OutData);
	}

	return false;
}

bool FairyManager::GetActivatedFairyBlessInfoAll(TArray<FMD_FairyInfo>& OutList)
{
	FMD_FairyInfo OutData;

	OutList.Empty();

	for (int32 Target = CliToSvrFairyType(EFairyType::FAIRY_NIECE); Target < CliToSvrFairyType(EFairyType::FAIRY_MAX); ++Target)
	{
		EFairyType FairyType = SvrToCliFairyType(Target);
		if (GetActivatedFairyBlessInfo(FairyType, OutData))
		{
			OutList.Add(OutData);
		}
	}

	return OutList.Num() > 0;
}

void FairyManager::RefreshCombatInfo()
{
	for (int32 i = 0; i < static_cast<int32>(EPCClass::EPC_End); ++i)
	{
		UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(IntToPCClass(i), false);

		//auto* DocHero = UB2UIDocHelper::GetDocHero(i);
		//if (DocHero)
		//{
		//	//DocHero->SetChangedCombatPower(true);
		//}
	}
}

bool FairyManager::GetFairyBlessSlotInfoAll(const EFairyType Target, TArray<FB2FairyBlessSlotInfo> &OutList)
{
	struct FB2FairyOptionIdSorter
	{
		bool operator()(const FMD_FairyInfo& A, const FMD_FairyInfo& B) const
		{
			if (A.OptionId == B.OptionId)
				return A.Level < B.Level;
			else 
				return A.OptionId > B.OptionId;
		}
	};

	if (OutList.Num() > 0)
		OutList.Empty();

	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	TArray<FMD_FairyInfo> TempList;
	if (CliDataStore.GetFairyInfoAll(Target, TempList))
	{
		int32 FairyLevel = 1;
		FB2FairyStatusInfo StatusInfo;
		if (GetFairyInfo(Target, StatusInfo))
			FairyLevel = StatusInfo.Level;

		TempList.Sort(FB2FairyOptionIdSorter());

		int32 PrevOptionId = TempList.Num()  > 0 ? TempList[0].OptionId : -1;
		int32 MinIndex = 0;
		for (int32 i = 0; i < TempList.Num(); ++i)//(FMD_FairyInfo &Each : TempList)
		{
			FMD_FairyInfo &Each = TempList[i];
			if (PrevOptionId != Each.OptionId)
			{
				FB2FairyBlessSlotInfo SlotInfo;
				SlotInfo.UpdateInfo(TempList[MinIndex], StatusInfo);
				OutList.Add(SlotInfo);
				
				PrevOptionId = Each.OptionId;
				MinIndex = i;
			}
			else
			{
				if (Each.Level <= FairyLevel)
				{
					MinIndex = i;
				}
			}
		}

		FB2FairyBlessSlotInfo SlotInfo;
		SlotInfo.UpdateInfo(TempList[MinIndex], StatusInfo);
		OutList.Add(SlotInfo);

		struct FB2FairyOptionLevelSorter
		{
			bool operator()(const FB2FairyBlessSlotInfo& A, const FB2FairyBlessSlotInfo& B) const
			{
				return A.OpenLevel < B.OpenLevel;
			}
		};
		OutList.Sort(FB2FairyOptionLevelSorter());

		return OutList.Num() > 0;
	}

	return false;
}

bool FairyManager::GetFairyTargetBlessInfo(EFairyType Target, int32 FairyLevel, FMD_FairyInfo& OutData)
{
	struct FB2FairyLevelSorter
	{
		bool operator()(const FMD_FairyInfo& A, const FMD_FairyInfo& B) const
		{
			return A.Level < B.Level;
		}
	};
	TArray<FMD_FairyInfo> FairyInfoList;
	if (GetMDFairyInfoAll(Target, FairyInfoList))
	{
		FairyInfoList.Sort(FB2FairyLevelSorter());

		for (FMD_FairyInfo& Each : FairyInfoList)
		{
			if (Each.Level == FairyLevel)
			{
				//UE_LOG(LogBladeII, Log, TEXT("@@@@@@@@@@@ GetFairyTargetBlessInfo  %d >> %d : %d == %d"), (int32)Each.FairyType, Each.OptionId, Each.Level, Each.BlessLevel);
				OutData = Each;
				return true;
			}
		}

	}
	return false;
}

bool FairyManager::IsSameFairyBlessInfo(EFairyType Target, int32 LevelA, int32 LevelB)
{
	FMD_FairyInfo CurFairyInfo;
	FMD_FairyInfo NextFairyInfo;
	GetFairyTargetBlessInfo(Target, LevelA, CurFairyInfo);
	GetFairyTargetBlessInfo(Target, LevelB, NextFairyInfo);

	return CurFairyInfo.OptionId == NextFairyInfo.OptionId;
}

bool FairyManager::GetFairyNextBlessInfo(EFairyType Target, int32 FairyLevel, FMD_FairyInfo& OutData)
{
	struct FB2FairyLevelSorter
	{
		bool operator()(const FMD_FairyInfo& A, const FMD_FairyInfo& B) const
		{
			return A.Level < B.Level;
		}
	};
	TArray<FMD_FairyInfo> FairyInfoList;
	if (GetMDFairyInfoAll(Target, FairyInfoList))
	{
		FairyInfoList.Sort(FB2FairyLevelSorter());

		for (FMD_FairyInfo& Each : FairyInfoList)
		{
			if (Each.Level > FairyLevel)
			{
				//UE_LOG(LogBladeII, Log, TEXT("@@@@@@@@@@@ GetFairyNextBlessInfo  %d >> %d : %d == %d"), (int32)Each.FairyType, Each.OptionId, Each.Level, Each.BlessLevel);
				OutData = Each;
				return true;
			}
		}

	}
	return false;
}

bool FairyManager::GetFairyBlessInfoAll(EFairyType Target, int32 OptionId, TArray<FMD_FairyInfo>& OutList)
{
	struct FB2FairyLevelSorter
	{
		bool operator()(const FMD_FairyInfo& A, const FMD_FairyInfo& B) const
		{
			return A.Level < B.Level;
		}
	};

	TArray<FMD_FairyInfo> FairyInfoList;
	if (GetMDFairyInfoAll(Target, FairyInfoList))
	{
		for (FMD_FairyInfo& Each : FairyInfoList)
		{
			if (Each.OptionId == OptionId)
			{
				//UE_LOG(LogBladeII, Log, TEXT("@@@@@@@@@@@ GetMDFairyInfoAll  %d >> %d : %d == %d"), (int32)Each.FairyType, Each.OptionId, Each.Level, Each.BlessLevel);
				OutList.Add(Each);
			}
		}

		OutList.Sort(FB2FairyLevelSorter());
		return OutList.Num() > 0;
	}
	return false;
}

bool FairyManager::GetFairyOpendBlessInfo(EFairyType Target, int32 OptionId, int32 FairyLevel, FMD_FairyInfo &OutData)
{
	TArray<FMD_FairyInfo> FairyInfoList;
	if (GetMDFairyInfoAll(Target, FairyInfoList))
	{
		int32 MinIndex = -1;
		for (int32 i = 0; i < FairyInfoList.Num(); ++i)//(FMD_FairyInfo &Each : FairyInfoList)
		{
			FMD_FairyInfo &Each = FairyInfoList[i];
			if (Each.OptionId == OptionId && Each.Level <= FairyLevel)
			{
				MinIndex = i;
				//UE_LOG(LogBladeII, Log, TEXT("@@@@@@@@@@@ GetMDFairyInfoAll  %d >> %d : %d == %d"), (int32)Each.FairyType, Each.OptionId, Each.Level, Each.BlessLevel);
			}
		}

		if (MinIndex > -1)
		{
			OutData = FairyInfoList[MinIndex];
			return true;
		}
	}
	return false;
}

int32 FairyManager::GetFairyLevelUpMax()
{
	if (CachedLevelUpMax > 0)
		return CachedLevelUpMax;

	struct FB2FairyLevelSorter
	{
		bool operator()(const FMD_FairyLevelUpCost& A, const FMD_FairyLevelUpCost& B) const
		{
			return A.Level < B.Level;
		}
	};
	TArray<FMD_FairyLevelUpCost> CostList;
	if (GetMDFairyLevelUpCostAll(CostList) && CostList.Num() > 0)
	{
		CachedLevelUpMax = CostList[CostList.Num() - 1].Level;
		return CachedLevelUpMax;
	}
	return CachedLevelUpMax;
}

int32 FairyManager::GetFairyLevelUpCost(int32 Level)
{
	struct FB2FairyLevelSorter
	{
		bool operator()(const FMD_FairyLevelUpCost& A, const FMD_FairyLevelUpCost& B) const
		{
			return A.Level < B.Level;
		}
	};
	TArray<FMD_FairyLevelUpCost> CostList;
	if (GetMDFairyLevelUpCostAll(CostList) && CostList.Num() > 0)
	{
		for (FMD_FairyLevelUpCost& Each : CostList)
		{
			if (Each.Level == Level)
			{
				return Each.MaterialCost;
			}
		}
	}
	return 0;
}