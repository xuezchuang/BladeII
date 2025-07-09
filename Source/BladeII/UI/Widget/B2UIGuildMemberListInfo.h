#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIGuildMemberListInfo.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildMemberListInfo : public UB2UIWidget
{
	GENERATED_BODY()

public:
	~UB2UIGuildMemberListInfo();
	virtual void OnOpen(bool RightNow = false) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

private:
	void SetEventSubscribe();
	void SetGuildMemberList();
	bool IsLongTermUnconnect(int64 InMemberID);
	void SetGuildInfo(FB2ResponseGuildInfoPtr InGuildInfo, TArray<b2network::B2GuildMemberPtr> InGuildMemberInfo);

public:
	UFUNCTION()
		void SetScondPopup(int32 GuildId, int32 Rank, int32 Index, int32 YPos);

	UFUNCTION()
		void OnClickClose();

private:
	FVector2D							MemberSlotSize;
	int64								MemberID; //±æµå¿ø accountid

	uint32								AppointSubGuildMasterTicket;
	uint32								DismissSubGuildMasterTicket;
	uint32								KickGuildMemberTicket;

	TArray<std::shared_ptr<class issue>> Issues;

protected:
	TWeakObjectPtr<UB2Button>			BTN_Close;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_GuildMemberCount;
	TWeakObjectPtr<UVerticalBox>		VB_List;

	TWeakObjectPtr<class UB2UIGuildInfoMemberPart1>	UIP_Profile;
	TWeakObjectPtr<class UB2UIGuildInfoMemberPart1>	UIP_SecondMaster;
	TWeakObjectPtr<class UB2UIGuildInfoMemberPart1>	UIP_MemberOut;
	TWeakObjectPtr<class UB2UIGuildInfoMemberPart1>	UIP_Master;
	TWeakObjectPtr<UB2ScrollBox>					SB_ScrollBox;
	TWeakObjectPtr<UCanvasPanel>					CP_Popup;
	TWeakObjectPtr<UOverlay>						OV_PopupBG;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TSubclassOf<class UB2UIGuildMemberSlot>		UIP_GuildMemberInfoSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		float PopupXPos;

	UFUNCTION()
		void SendMemberProfile();
	UFUNCTION()
		void SendSecondMasterDismissal();
	UFUNCTION()
		void SendSecondMasterEntrust();
	UFUNCTION()
		void SendMemberOut();

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
};