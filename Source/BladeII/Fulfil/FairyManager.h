#pragma once

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
	
	// 페어리 정보
	bool GetFairyInfo(EFairyType Fairy, FB2FairyStatusInfo &OutData);
	void GetFairyInfos(TArray<FB2FairyStatusInfo> &OutData);

	// 페어리 이름 가져오기
	FText GetFairyName(EFairyType Type);

	// 축복 이름 가져오기
	FText GetBlessOptionName(int32 OptionId);
	
	// 페어리 오픈 스테이지 정보 가져오기
	FText GetFairyOpenStageName(EFairyType Type);

	// 다음 레벨 페어리 정보
	bool GetFairyNextLevelInfo(EFairyType Fairy, FB2FairyStatusInfo &OutData);

	////////////////////////////////////////////////

	// 페어리 최대 레벨
	int32 GetFairyLevelUpMax();

	// 페어리 레벨 비용
	int32 GetFairyLevelUpCost(int32 Level);

	/*
	* 페이러 테이블 정보 가져오기
	*/
	const FFairyBelssInfo* GetFairyBlessInfo(EFairyType Target, int32 OptionId);

	/**
	* 리스트에 보여줄 축복 목록 가져오기
	*
	*/
	bool GetFairyBlessSlotInfoAll(const EFairyType Target, TArray<FB2FairyBlessSlotInfo> &OutList);

	/**
	* 리스트에 보여줄 오픈된 축복 정보 가져오기
	*
	*/
	bool GetFairyOpendBlessInfo(EFairyType Target, int32 OptionId, int32 FairyLevel, FMD_FairyInfo &OutData);

	// 옵션 ID의 수치 목록 가져오기
	bool GetFairyBlessInfoAll(EFairyType Target, int32 OptionId, TArray<FMD_FairyInfo> &OutList);

	// FairyLevel보다 큰 정보 가져오기
	bool GetFairyNextBlessInfo(EFairyType Target,int32 FairyLevel, FMD_FairyInfo &OutData);

	// FairyLevel 의 정보 가져오기
	bool GetFairyTargetBlessInfo(EFairyType Target, int32 FairyLevel, FMD_FairyInfo &OutData);

	// 페어리 레벨A와 레벨B의 축복이 같은지 검사 (true:같음)
	bool IsSameFairyBlessInfo(EFairyType Target, int32 LevelA, int32 LevelB);

	/**
	* (프로토콜) 페어리 정보 조회
	*
	*/
	void RequestGetFairy();

	/**
	* (프로토콜)페어리 레벨업 요청
	*
	* @param TargetFairy 레벨업 할 페이러 타입 정보
	*/
	void RequestLevelUpFairy(const EFairyType TargetFairy);
	
	/**
	* (프로토콜)페어리 축복 요청
	*
	* @param TargetFairy 축복 적용 할 페이러 타입 정보
	* @param BlessOptionId 적용 할 옵션 아이디
	*/
	void RequestBlessFairy(const EFairyType TargetFairy, const int32 BlessOptionId);

	/**
	* 특정 페어리 사용 가능 판단하기
	*
	*/
	bool GetIsOpenedFairy(const EFairyType TargetFairy);

	/**
	* 특정 페어리 열리는 스테이지 정보 가져오기
	*
	*/
	struct FSingleStageInfoData* GetFairyOpenStageInfo(const EFairyType TargetFairy);

	/**
	* 특정 페어리 열리는 스테이지 난이도 정보 가져오기
	*
	*/
	EStageDifficulty GetFairyOpenStageDifficulty(const EFairyType TargetFairy);

	
	/**
	* 페어리 상태 갱신
	*
	* @param Infos 페어리 상태 정보 리스트
	*/
	void UpdateFairyStatus(const TArray<b2network::B2FairyStatusPtr> &Infos);

	TArray<FB2FairyStatusInfo> ModUpdateFairyStatus(const TArray<b2network::B2FairyStatusPtr> &Infairies);

	/**
	* 페어리 상태 갱신
	*
	*/
	void UpdateFairyStatus(const b2network::B2FairyStatusPtr &Info);
private:
	/**
	* 특정 페어리 정보 가져오기
	*
	* @param Target 특정 페어리 타입 정보
	* @param OptionId 특정 페어리 축복 옵션 아이디
	* @param BlessLevel 특정 축복 페어리 레벨
	* @param OutData 페어리 정보 넘겨 줌
	*
	* @return 데이터 유,무 여부
	*/
	bool GetMDFairyInfo(EFairyType Target, int32 OptionId, int32 BlessLevel, FMD_FairyInfo &OutData);

	//>>----------------- MasterData Wrapper
	/**
	* 전체 페어리 열리는 정보 가져오기
	*
	* @param OutList 전체 페어리 열리는 정보를  넘겨줌
	*
	* @return 데이터 유,무 여부
	*/
	bool GetMDFairyOpenInfoAll(TArray<FMD_FairyOpenInfo> &OutList);

	/**
	* 특정 페어리 열리는 정보 가져오기
	*
	* @param Target 특정 페어리 타입 정보
	* @param OutData 페어리 열리는 정보 넘겨 줌
	*
	* @return 데이터 유,무 여부
	*/
	bool GetMDFairyOpenInfo(EFairyType Target, FMD_FairyOpenInfo &OutData);

	/**
	* 전체 페어리 정보 가져오기
	*
	* @param Target 특정 페어리 타입 정보
	* @param OutList 전체 페어리 정보를  넘겨줌
	*
	* @return 데이터 유,무 여부
	*/
	bool GetMDFairyInfoAll(EFairyType Target, TArray<FMD_FairyInfo> &OutList);

	/**
	* 전체 페어리 블레스 ID 목록 가져오기
	*
	* @param Target 특정 페어리 타입 정보
	* @param OutList 전체 페어리 블레스 ID 목록 넘겨줌
	*
	* @return 데이터 유,무 여부
	*/
	bool GetMDFairyInfoOptionIdAll(EFairyType Target, TArray<int32> &OutList);

	/**
	* 전체 페어리 레벨업 비용 정보 가져오기
	*
	* @param OutList 전체 페어리 레벨업 비용 정보를  넘겨줌
	*
	* @return 데이터 유,무 여부
	*/
	bool GetMDFairyLevelUpCostAll(TArray<FMD_FairyLevelUpCost> &OutList);

	/**
	* 특정 페어리 레벨업 비용 정보 가져오기
	*
	* @param Target 특정 페어리 타입 정보
	* @param OutData 페어리 레벨업 비용 정보 넘겨 줌
	*
	* @return 데이터 유,무 여부
	*/
	bool GetMDFairyLevelUpCost(int32 TargetLevel, FMD_FairyLevelUpCost &OutData);
	//<<----------------- MasterData Wrapper

	/**
	* (프로토콜 응답) 페어리 정보 조회
	*
	* @param data 갱신된 페어리 정보
	*/
	void OnReceivedGetFairy(const FB2GetFairyPtr& data);
	
	/**
	* (프로토콜 응답)페어리 축복 콜백
	*
	* @param data 갱신된 페어리 정보
	*/
	void OnReceivedLevelUpFairy(const FB2LevelupFairyPtr& data);
	void OnErrorReceivedLevelUpFairy(const FB2LevelupFairyPtr& data);
	
	/**
	* (프로토콜 응답)페어리 축복 콜백
	*
	* @param data 갱신된 페어리 정보
	*/
	void OnReceivedBlessFairy(const FB2BlessFairyPtr& data);

	/**
	* 축복 중인 페어리의 축복 정보 가져오기
	*
	*/
	bool GetActivatedFairyBlessInfo(const EFairyType TargetFairy, int32 OptionId, int32 FairyLevel, FMD_FairyInfo &OutData);

	/**
	* 축복 중인 페어리의 축복 정보 가져오기
	*
	*/
	bool GetActivatedFairyBlessInfo(const EFairyType TargetFairy, FMD_FairyInfo &OutData);

	/**
	* 모든 페어리 축복 정보 가져오기
	*
	*/
	bool GetActivatedFairyBlessInfoAll(TArray<FMD_FairyInfo> &OutList);

	//페어리 변화있을시 전투력정보 수정
private:
	void RefreshCombatInfo();


private:
	TMap<EFairyType, FB2FairyStatusInfo> CachedFairyInfoMap;

	bool bSubscribed;
	int32 CachedLevelUpMax;

	TArray<std::shared_ptr<issue>> Issues;

};