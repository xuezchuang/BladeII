#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIMailContent.generated.h"

USTRUCT()
struct FB2MailSenderPortraitTemplate : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	MailSenderType			NPC_Type;

	UPROPERTY(EditAnywhere)
	UMaterialInterface*		Portrait;
};

USTRUCT()
struct FB2MailGiftPortraitTemplate : public FTableRowBase{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	MailGiftType			Gift_Type;

	UPROPERTY(EditAnywhere)
	EGeneralLotteryGrade Lottery_Grade;

	UPROPERTY(EditAnywhere)
	int32 Lottery_ItemStarGrade;

	UPROPERTY(EditAnywhere)
	ELotteryItemCategory Lottery_ItemCategory;
	
	UPROPERTY(EditAnywhere)
	int32			Id;

	//UPROPERTY(EditAnywhere)
	//int32 IteminfoDataIdx;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface*		Portrait;
};

UCLASS()
class UB2UIMailContent : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	UPROPERTY(EditAnywhere, Category="Time Color")
		FSlateColor	LeftTimeTextColor_ShortTime;
	UPROPERTY(EditAnywhere, Category = "Time Color")
		FSlateColor	LeftTimeTextColor_MidTime;
	UPROPERTY(EditAnywhere, Category = "Time Color")
		FSlateColor	LeftTimeTextColor_LongTime;
	
	UPROPERTY(EditAnywhere, Category = "NPC Portrait")
		UDataTable* NPCPortraitDataTable;

public:
	void SetMailContent(FB2MailContentPtr MailContent);
	int64 GetMailId() const { return MailId; }
	MailGiftType GetGiftType() const { return ToReceiveGift; }
	int32 GetItemRefId() const { return ToReceiveItemRefId; }
	void OpenedMail(const FB2ResponseOpenMailPtr& MailInfo);
	TArray<int32> GetSelectItemRefIDs();
	void SetMailBox(class UB2UIMail* InMailBox) 
	{
		MailBox = InMailBox;
	}

	UB2UIMail* GetMailBox() { return MailBox; }

	UFUNCTION()
		void OnClickedReceiveBTN();


	class UMaterialInterface* GetItemIcon(MailGiftType GiftType, int32 ItemTemplateId = 0);
	FText GetItemNumText(MailGiftType GiftType, int32 Num, int32 ItemTemplateId) { return GetText_WhatGift(GiftType, Num, ItemTemplateId); }

private:
	enum class ELeftTimeType : uint8
	{
		Expiration,
		Sec,
		Min_Sec,
		Hour_Min_Sec,
		Day_Hour_Min,
		Unlimited,
	};

	ELeftTimeType CurrentLeftTimeType;

	TWeakObjectPtr<UImage>		IMG_Portrait;
	TWeakObjectPtr<UTextBlock>	TB_NickName;
	TWeakObjectPtr<UTextBlock>	TB_Name;
	TWeakObjectPtr<UB2RichTextBlock> RTB_MailContent;

	TWeakObjectPtr<UImage>		IMG_GiftPortrait;
	TWeakObjectPtr<UTextBlock>	TB_NumGift;
	TWeakObjectPtr<UImage>		IMG_Clock_01;
	TWeakObjectPtr<UImage>		IMG_Clock_02;
	TWeakObjectPtr<UTextBlock>	TB_LeftTime;
	TWeakObjectPtr<UTextBlock>	TB_Receive;

	TWeakObjectPtr<UButton>		BTN_Receive;
	TWeakObjectPtr<UB2ButtonGoodInfoToolTip>	BTN_Goods;

	TArray<FB2MailSenderPortraitTemplate*>	SenderPortraits;
	//TArray<FB2MailGiftPortraitTemplate*>	GiftPortraits;

private:
	ELeftTimeType GetLeftTimeType(int64 TimeSeconds);
	void SetTime(int64 TimeSeconds);
	void SetSender(MailSenderType SenderType, const FString& FriendName);
	void SetGift(int32 ItemTemplateId, int32 Num);
	void SetLotteryTicketIcon(int32 ItemTemplateId);
	void SetContentText(const FText& Cause, const FText& Gift);

	void SetPortraitIcon(int32 _ItemTemplateId);
	void SetGiftNumText(int32 _Num);

	void GetEquipTypeText(int32 InEquipType , FText& InText, int32& InParam);
	bool IsCostumeByEquipCategory(int32 InEquipCategory);

	bool CheckAdminMail(MailSenderType SenderType, const FString& Message);
	void SetContentAdminText(const FText& Message);

	FText GetText_WhatCause(MailWhatCause Cause);
	FText GetText_PackageName(int32 PackageId);
	FText GetText_WhatGift(MailGiftType GiftType, int32 Num, int32 ItemTemplateId);

	int64 MailId;
	FDateTime ExpireTime;

	MailGiftType ToReceiveGift;
	int32 ToReceiveGiftNum;
	int32 ToReceiveItemRefId;

	FString CachedSenderName;

	TArray<int32> ToSelectItemRefIds;
	TArray<int32> ToSelectItemNums;

	class UB2UIMail* MailBox;
};