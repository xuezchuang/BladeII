#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIStoreBanner.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStoreBannerButton : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	void ToggleImage(bool ToggleOn);
	void SetBannerIndex(int32 ToSet);
	void SetBannerMain(class UB2UIStoreBanner* ToSet);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
		void OnClickedButton();

private:
	TWeakObjectPtr<UImage> IMG_On;
	TWeakObjectPtr<UImage> IMG_Off;
	TWeakObjectPtr<UB2Button> BTN_Area;

	int32 BannerIndex;

	class UB2UIStoreBanner* BannerMain;
};

UENUM(BlueprintType)
enum class EBannerStoreToGo : uint8
{
	SummonItem_GemBox,
	SummonItem_FriendBox,
	SummonItem_Costume,
	Store_BladePoint,
	Store_Gem,
	Store_Gold,
	Store_DarkCrystal,
	Store_Package,
	DoNotGoAnywhere,
};

USTRUCT(BlueprintType)
struct FBannerData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UMaterialInstance* BannerImage;

	UPROPERTY(EditAnywhere)
	EBannerStoreToGo WhereToGo;

	TWeakObjectPtr<UB2UIStoreBannerButton> BannerButton;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2BannerDataInfos : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TArray<FBannerData> BannerData;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStoreBanner : public UB2UIWidgetBase
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ShowBanner(int32 ToShowIndex, bool bForce);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	void AutoScroll(float InDeltaTime);

	void OnBeginScroll(float ScrolledDelta);
	void ChangeBanner(int32 CenterBannerIndex);

	float GetBannerWidth();

	UFUNCTION()
		void OnClickedBanner();

protected:
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSubclassOf<UB2UIStoreBannerButton> BannerButtonClass;	

	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSubclassOf<UB2BannerDataInfos> BannerDataInfos;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	float NextBannerScrollTime;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	float AutoScrollTime;

	float BannerImageWidth;

	TArray<FBannerData> BannerImages;

private:
	TWeakObjectPtr<UB2ScrollBox> BannerScrollBox;

	TWeakObjectPtr<UHorizontalBox> HB_BannerBox;
	TWeakObjectPtr<UImage> IMG_BannerBackward;
	TWeakObjectPtr<UImage> IMG_BannerCurrent;
	TWeakObjectPtr<UImage> IMG_BannerForward;

	TWeakObjectPtr<UHorizontalBox> HB_ButtonBox;

	TWeakObjectPtr<UB2Button> BTN_Banner;

	int32 CurrentViewBannerIndex;
	int32 DesiredPosition;

	float ElapsedNextBannerScrollTime;

	float WidgetScale;	
};