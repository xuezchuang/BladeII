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
	
	// �� ����
	bool GetFairyInfo(EFairyType Fairy, FB2FairyStatusInfo &OutData);
	void GetFairyInfos(TArray<FB2FairyStatusInfo> &OutData);

	// �� �̸� ��������
	FText GetFairyName(EFairyType Type);

	// �ູ �̸� ��������
	FText GetBlessOptionName(int32 OptionId);
	
	// �� ���� �������� ���� ��������
	FText GetFairyOpenStageName(EFairyType Type);

	// ���� ���� �� ����
	bool GetFairyNextLevelInfo(EFairyType Fairy, FB2FairyStatusInfo &OutData);

	////////////////////////////////////////////////

	// �� �ִ� ����
	int32 GetFairyLevelUpMax();

	// �� ���� ���
	int32 GetFairyLevelUpCost(int32 Level);

	/*
	* ���̷� ���̺� ���� ��������
	*/
	const FFairyBelssInfo* GetFairyBlessInfo(EFairyType Target, int32 OptionId);

	/**
	* ����Ʈ�� ������ �ູ ��� ��������
	*
	*/
	bool GetFairyBlessSlotInfoAll(const EFairyType Target, TArray<FB2FairyBlessSlotInfo> &OutList);

	/**
	* ����Ʈ�� ������ ���µ� �ູ ���� ��������
	*
	*/
	bool GetFairyOpendBlessInfo(EFairyType Target, int32 OptionId, int32 FairyLevel, FMD_FairyInfo &OutData);

	// �ɼ� ID�� ��ġ ��� ��������
	bool GetFairyBlessInfoAll(EFairyType Target, int32 OptionId, TArray<FMD_FairyInfo> &OutList);

	// FairyLevel���� ū ���� ��������
	bool GetFairyNextBlessInfo(EFairyType Target,int32 FairyLevel, FMD_FairyInfo &OutData);

	// FairyLevel �� ���� ��������
	bool GetFairyTargetBlessInfo(EFairyType Target, int32 FairyLevel, FMD_FairyInfo &OutData);

	// �� ����A�� ����B�� �ູ�� ������ �˻� (true:����)
	bool IsSameFairyBlessInfo(EFairyType Target, int32 LevelA, int32 LevelB);

	/**
	* (��������) �� ���� ��ȸ
	*
	*/
	void RequestGetFairy();

	/**
	* (��������)�� ������ ��û
	*
	* @param TargetFairy ������ �� ���̷� Ÿ�� ����
	*/
	void RequestLevelUpFairy(const EFairyType TargetFairy);
	
	/**
	* (��������)�� �ູ ��û
	*
	* @param TargetFairy �ູ ���� �� ���̷� Ÿ�� ����
	* @param BlessOptionId ���� �� �ɼ� ���̵�
	*/
	void RequestBlessFairy(const EFairyType TargetFairy, const int32 BlessOptionId);

	/**
	* Ư�� �� ��� ���� �Ǵ��ϱ�
	*
	*/
	bool GetIsOpenedFairy(const EFairyType TargetFairy);

	/**
	* Ư�� �� ������ �������� ���� ��������
	*
	*/
	struct FSingleStageInfoData* GetFairyOpenStageInfo(const EFairyType TargetFairy);

	/**
	* Ư�� �� ������ �������� ���̵� ���� ��������
	*
	*/
	EStageDifficulty GetFairyOpenStageDifficulty(const EFairyType TargetFairy);

	
	/**
	* �� ���� ����
	*
	* @param Infos �� ���� ���� ����Ʈ
	*/
	void UpdateFairyStatus(const TArray<b2network::B2FairyStatusPtr> &Infos);

	TArray<FB2FairyStatusInfo> ModUpdateFairyStatus(const TArray<b2network::B2FairyStatusPtr> &Infairies);

	/**
	* �� ���� ����
	*
	*/
	void UpdateFairyStatus(const b2network::B2FairyStatusPtr &Info);
private:
	/**
	* Ư�� �� ���� ��������
	*
	* @param Target Ư�� �� Ÿ�� ����
	* @param OptionId Ư�� �� �ູ �ɼ� ���̵�
	* @param BlessLevel Ư�� �ູ �� ����
	* @param OutData �� ���� �Ѱ� ��
	*
	* @return ������ ��,�� ����
	*/
	bool GetMDFairyInfo(EFairyType Target, int32 OptionId, int32 BlessLevel, FMD_FairyInfo &OutData);

	//>>----------------- MasterData Wrapper
	/**
	* ��ü �� ������ ���� ��������
	*
	* @param OutList ��ü �� ������ ������  �Ѱ���
	*
	* @return ������ ��,�� ����
	*/
	bool GetMDFairyOpenInfoAll(TArray<FMD_FairyOpenInfo> &OutList);

	/**
	* Ư�� �� ������ ���� ��������
	*
	* @param Target Ư�� �� Ÿ�� ����
	* @param OutData �� ������ ���� �Ѱ� ��
	*
	* @return ������ ��,�� ����
	*/
	bool GetMDFairyOpenInfo(EFairyType Target, FMD_FairyOpenInfo &OutData);

	/**
	* ��ü �� ���� ��������
	*
	* @param Target Ư�� �� Ÿ�� ����
	* @param OutList ��ü �� ������  �Ѱ���
	*
	* @return ������ ��,�� ����
	*/
	bool GetMDFairyInfoAll(EFairyType Target, TArray<FMD_FairyInfo> &OutList);

	/**
	* ��ü �� ���� ID ��� ��������
	*
	* @param Target Ư�� �� Ÿ�� ����
	* @param OutList ��ü �� ���� ID ��� �Ѱ���
	*
	* @return ������ ��,�� ����
	*/
	bool GetMDFairyInfoOptionIdAll(EFairyType Target, TArray<int32> &OutList);

	/**
	* ��ü �� ������ ��� ���� ��������
	*
	* @param OutList ��ü �� ������ ��� ������  �Ѱ���
	*
	* @return ������ ��,�� ����
	*/
	bool GetMDFairyLevelUpCostAll(TArray<FMD_FairyLevelUpCost> &OutList);

	/**
	* Ư�� �� ������ ��� ���� ��������
	*
	* @param Target Ư�� �� Ÿ�� ����
	* @param OutData �� ������ ��� ���� �Ѱ� ��
	*
	* @return ������ ��,�� ����
	*/
	bool GetMDFairyLevelUpCost(int32 TargetLevel, FMD_FairyLevelUpCost &OutData);
	//<<----------------- MasterData Wrapper

	/**
	* (�������� ����) �� ���� ��ȸ
	*
	* @param data ���ŵ� �� ����
	*/
	void OnReceivedGetFairy(const FB2GetFairyPtr& data);
	
	/**
	* (�������� ����)�� �ູ �ݹ�
	*
	* @param data ���ŵ� �� ����
	*/
	void OnReceivedLevelUpFairy(const FB2LevelupFairyPtr& data);
	void OnErrorReceivedLevelUpFairy(const FB2LevelupFairyPtr& data);
	
	/**
	* (�������� ����)�� �ູ �ݹ�
	*
	* @param data ���ŵ� �� ����
	*/
	void OnReceivedBlessFairy(const FB2BlessFairyPtr& data);

	/**
	* �ູ ���� ���� �ູ ���� ��������
	*
	*/
	bool GetActivatedFairyBlessInfo(const EFairyType TargetFairy, int32 OptionId, int32 FairyLevel, FMD_FairyInfo &OutData);

	/**
	* �ູ ���� ���� �ູ ���� ��������
	*
	*/
	bool GetActivatedFairyBlessInfo(const EFairyType TargetFairy, FMD_FairyInfo &OutData);

	/**
	* ��� �� �ູ ���� ��������
	*
	*/
	bool GetActivatedFairyBlessInfoAll(TArray<FMD_FairyInfo> &OutList);

	//�� ��ȭ������ ���������� ����
private:
	void RefreshCombatInfo();


private:
	TMap<EFairyType, FB2FairyStatusInfo> CachedFairyInfoMap;

	bool bSubscribed;
	int32 CachedLevelUpMax;

	TArray<std::shared_ptr<issue>> Issues;

};