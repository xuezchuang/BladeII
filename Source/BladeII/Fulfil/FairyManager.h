#pragma once
//#include "BladeII.h"
//#include "UObject/Object.h"
#include "Event.h"
#include "EventSubsystem.h"
#include "CommonStruct.h"
#include "Engine/DataTable.h"
#include "B2ClientDataStore.h"
#include "FairyManager.generated.h"


USTRUCT()
struct FFairyBelssInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EFairyType FairyType = EFairyType::FAIRY_NIECE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 OptionID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UTexture2D> SkillIconTexture;

	UTexture2D* GetFairySkillIconTexture() const;
};

UCLASS()
class BLADEII_API UB2FairyInfo : public UObject
{
	GENERATED_BODY()

public:
	const FFairyBelssInfo*	GetFairyBelssInfo(int32 FairyType, int32 OptionID);

	void SetDataTable(UDataTable* InDataTable);

	int32 GetKey(int32 InGrade, int32 InLevel) { return ((InGrade * 100) + InLevel); }

#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
	void EditorLoadAll();
#endif

private:

	UPROPERTY()
	UDataTable* FairyBlessInfoTable;
};

struct FB2FairyStatusInfo
{
	FB2FairyStatusInfo()
		: FairyType(EFairyType::FAIRY_NONE)
		, bClearStage(false)
		, Level(1)
		, MaxLevel(120)
		, ActivedOptoinId(-1)
		, EnhancePoint(0)
		, OptionValue(0.0f)
	{}

	FB2FairyStatusInfo(EFairyType Fairy)
		: FairyType(Fairy)
		, bClearStage(false)
		, Level(1)
		, MaxLevel(120)
		, ActivedOptoinId(-1)
		, EnhancePoint(0)
		, OptionValue(0.0f)
	{}	  
	FB2FairyStatusInfo(const b2network::B2FairyStatusPtr& ServerStruct);

	EFairyType FairyType;
	bool bClearStage;

	int32 Level;
	int32 MaxLevel;

	int32 ActivedOptoinId;
	int32 EnhancePoint;
	float OptionValue;
	
	FB2FairyStatusInfo& operator=(const b2network::B2FairyStatusPtr& ServerStruct);
};

struct FB2FairyBlessSlotInfo
{
	EFairyType FairyType;
	int32 OptionId;
	int32 BlessLevel;
	int32 OpenLevel;
	bool bOpendBless;
	bool bEquiped;
	float OptionValue;

	FB2FairyBlessSlotInfo()
		: FairyType(EFairyType::FAIRY_NONE)
		, OptionId(0)
		, BlessLevel(0)
		, OpenLevel(0)
		, bOpendBless(false)
		, bEquiped(false)
		, OptionValue(0.0f)
	{}
	void UpdateInfo(const FMD_FairyInfo& Info, const FB2FairyStatusInfo &Status);
};

class BLADEII_API FairyManager
{
public:
	FairyManager();
	~FairyManager();

	static FairyManager&	GetInstance();
	void Init();

	void SubscribeEvents();
	void UnsubscribeEvents();

	////////////////////////////////////////////////
	
	// 其绢府 沥焊
	bool GetFairyInfo(EFairyType Fairy, FB2FairyStatusInfo &OutData);
	void GetFairyInfos(TArray<FB2FairyStatusInfo> &OutData);

	// 其绢府 捞抚 啊廉坷扁
	FText GetFairyName(EFairyType Type);

	// 绵汗 捞抚 啊廉坷扁
	FText GetBlessOptionName(int32 OptionId);
	
	// 其绢府 坷锹 胶抛捞瘤 沥焊 啊廉坷扁
	FText GetFairyOpenStageName(EFairyType Type);

	// 促澜 饭骇 其绢府 沥焊
	bool GetFairyNextLevelInfo(EFairyType Fairy, FB2FairyStatusInfo &OutData);

	////////////////////////////////////////////////

	// 其绢府 弥措 饭骇
	int32 GetFairyLevelUpMax();

	// 其绢府 饭骇 厚侩
	int32 GetFairyLevelUpCost(int32 Level);

	/*
	* 其捞矾 抛捞喉 沥焊 啊廉坷扁
	*/
	const FFairyBelssInfo* GetFairyBlessInfo(EFairyType Target, int32 OptionId);

	/**
	* 府胶飘俊 焊咯临 绵汗 格废 啊廉坷扁
	*
	*/
	bool GetFairyBlessSlotInfoAll(const EFairyType Target, TArray<FB2FairyBlessSlotInfo> &OutList);

	/**
	* 府胶飘俊 焊咯临 坷锹等 绵汗 沥焊 啊廉坷扁
	*
	*/
	bool GetFairyOpendBlessInfo(EFairyType Target, int32 OptionId, int32 FairyLevel, FMD_FairyInfo &OutData);

	// 可记 ID狼 荐摹 格废 啊廉坷扁
	bool GetFairyBlessInfoAll(EFairyType Target, int32 OptionId, TArray<FMD_FairyInfo> &OutList);

	// FairyLevel焊促 奴 沥焊 啊廉坷扁
	bool GetFairyNextBlessInfo(EFairyType Target,int32 FairyLevel, FMD_FairyInfo &OutData);

	// FairyLevel 狼 沥焊 啊廉坷扁
	bool GetFairyTargetBlessInfo(EFairyType Target, int32 FairyLevel, FMD_FairyInfo &OutData);

	// 其绢府 饭骇A客 饭骇B狼 绵汗捞 鞍篮瘤 八荤 (true:鞍澜)
	bool IsSameFairyBlessInfo(EFairyType Target, int32 LevelA, int32 LevelB);

	/**
	* (橇肺配妮) 其绢府 沥焊 炼雀
	*
	*/
	void RequestGetFairy();

	/**
	* (橇肺配妮)其绢府 饭骇诀 夸没
	*
	* @param TargetFairy 饭骇诀 且 其捞矾 鸥涝 沥焊
	*/
	void RequestLevelUpFairy(const EFairyType TargetFairy);
	
	/**
	* (橇肺配妮)其绢府 绵汗 夸没
	*
	* @param TargetFairy 绵汗 利侩 且 其捞矾 鸥涝 沥焊
	* @param BlessOptionId 利侩 且 可记 酒捞叼
	*/
	void RequestBlessFairy(const EFairyType TargetFairy, const int32 BlessOptionId);

	/**
	* 漂沥 其绢府 荤侩 啊瓷 魄窜窍扁
	*
	*/
	bool GetIsOpenedFairy(const EFairyType TargetFairy);

	/**
	* 漂沥 其绢府 凯府绰 胶抛捞瘤 沥焊 啊廉坷扁
	*
	*/
	struct FSingleStageInfoData* GetFairyOpenStageInfo(const EFairyType TargetFairy);

	/**
	* 漂沥 其绢府 凯府绰 胶抛捞瘤 抄捞档 沥焊 啊廉坷扁
	*
	*/
	EStageDifficulty GetFairyOpenStageDifficulty(const EFairyType TargetFairy);

	
	/**
	* 其绢府 惑怕 盎脚
	*
	* @param Infos 其绢府 惑怕 沥焊 府胶飘
	*/
	void UpdateFairyStatus(const TArray<b2network::B2FairyStatusPtr> &Infos);

	TArray<FB2FairyStatusInfo> ModUpdateFairyStatus(const TArray<b2network::B2FairyStatusPtr> &Infairies);

	/**
	* 其绢府 惑怕 盎脚
	*
	*/
	void UpdateFairyStatus(const b2network::B2FairyStatusPtr &Info);
private:
	/**
	* 漂沥 其绢府 沥焊 啊廉坷扁
	*
	* @param Target 漂沥 其绢府 鸥涝 沥焊
	* @param OptionId 漂沥 其绢府 绵汗 可记 酒捞叼
	* @param BlessLevel 漂沥 绵汗 其绢府 饭骇
	* @param OutData 其绢府 沥焊 逞败 淋
	*
	* @return 单捞磐 蜡,公 咯何
	*/
	bool GetMDFairyInfo(EFairyType Target, int32 OptionId, int32 BlessLevel, FMD_FairyInfo &OutData);

	//>>----------------- MasterData Wrapper
	/**
	* 傈眉 其绢府 凯府绰 沥焊 啊廉坷扁
	*
	* @param OutList 傈眉 其绢府 凯府绰 沥焊甫  逞败淋
	*
	* @return 单捞磐 蜡,公 咯何
	*/
	bool GetMDFairyOpenInfoAll(TArray<FMD_FairyOpenInfo> &OutList);

	/**
	* 漂沥 其绢府 凯府绰 沥焊 啊廉坷扁
	*
	* @param Target 漂沥 其绢府 鸥涝 沥焊
	* @param OutData 其绢府 凯府绰 沥焊 逞败 淋
	*
	* @return 单捞磐 蜡,公 咯何
	*/
	bool GetMDFairyOpenInfo(EFairyType Target, FMD_FairyOpenInfo &OutData);

	/**
	* 傈眉 其绢府 沥焊 啊廉坷扁
	*
	* @param Target 漂沥 其绢府 鸥涝 沥焊
	* @param OutList 傈眉 其绢府 沥焊甫  逞败淋
	*
	* @return 单捞磐 蜡,公 咯何
	*/
	bool GetMDFairyInfoAll(EFairyType Target, TArray<FMD_FairyInfo> &OutList);

	/**
	* 傈眉 其绢府 喉饭胶 ID 格废 啊廉坷扁
	*
	* @param Target 漂沥 其绢府 鸥涝 沥焊
	* @param OutList 傈眉 其绢府 喉饭胶 ID 格废 逞败淋
	*
	* @return 单捞磐 蜡,公 咯何
	*/
	bool GetMDFairyInfoOptionIdAll(EFairyType Target, TArray<int32> &OutList);

	/**
	* 傈眉 其绢府 饭骇诀 厚侩 沥焊 啊廉坷扁
	*
	* @param OutList 傈眉 其绢府 饭骇诀 厚侩 沥焊甫  逞败淋
	*
	* @return 单捞磐 蜡,公 咯何
	*/
	bool GetMDFairyLevelUpCostAll(TArray<FMD_FairyLevelUpCost> &OutList);

	/**
	* 漂沥 其绢府 饭骇诀 厚侩 沥焊 啊廉坷扁
	*
	* @param Target 漂沥 其绢府 鸥涝 沥焊
	* @param OutData 其绢府 饭骇诀 厚侩 沥焊 逞败 淋
	*
	* @return 单捞磐 蜡,公 咯何
	*/
	bool GetMDFairyLevelUpCost(int32 TargetLevel, FMD_FairyLevelUpCost &OutData);
	//<<----------------- MasterData Wrapper

	/**
	* (橇肺配妮 览翠) 其绢府 沥焊 炼雀
	*
	* @param data 盎脚等 其绢府 沥焊
	*/
	void OnReceivedGetFairy(const FB2GetFairyPtr& data);
	
	/**
	* (橇肺配妮 览翠)其绢府 绵汗 妮归
	*
	* @param data 盎脚等 其绢府 沥焊
	*/
	void OnReceivedLevelUpFairy(const FB2LevelupFairyPtr& data);
	void OnErrorReceivedLevelUpFairy(const FB2LevelupFairyPtr& data);
	
	/**
	* (橇肺配妮 览翠)其绢府 绵汗 妮归
	*
	* @param data 盎脚等 其绢府 沥焊
	*/
	void OnReceivedBlessFairy(const FB2BlessFairyPtr& data);

	/**
	* 绵汗 吝牢 其绢府狼 绵汗 沥焊 啊廉坷扁
	*
	*/
	bool GetActivatedFairyBlessInfo(const EFairyType TargetFairy, int32 OptionId, int32 FairyLevel, FMD_FairyInfo &OutData);

	/**
	* 绵汗 吝牢 其绢府狼 绵汗 沥焊 啊廉坷扁
	*
	*/
	bool GetActivatedFairyBlessInfo(const EFairyType TargetFairy, FMD_FairyInfo &OutData);

	/**
	* 葛电 其绢府 绵汗 沥焊 啊廉坷扁
	*
	*/
	bool GetActivatedFairyBlessInfoAll(TArray<FMD_FairyInfo> &OutList);

	//其绢府 函拳乐阑矫 傈捧仿沥焊 荐沥
private:
	void RefreshCombatInfo();


private:
	TMap<EFairyType, FB2FairyStatusInfo> CachedFairyInfoMap;

	bool bSubscribed;
	int32 CachedLevelUpMax;

	TArray<std::shared_ptr<issue>> Issues;

};