#pragma once
#include "bladeII.h"
#include "B2UnitedWidgetBase.h"
#include "CommonStruct.h"
#include "B2UITitleDesign.generated.h"

USTRUCT()
struct FB2UITitleMovieSetupLoadedCacher
{
	GENERATED_USTRUCT_BODY()

	FB2UITitleMovieSetupLoadedCacher()
	{
		LoadedMovieSource = nullptr;
		LoadedMoviePlayer = nullptr;
		LoadedMovieSoundCue = nullptr;
		LoadedFallbackSnapshotImage = nullptr;
	}

	UPROPERTY(Transient)
		class UMediaSource* LoadedMovieSource;
	UPROPERTY(Transient)
		class UMediaPlayer* LoadedMoviePlayer;
	UPROPERTY(Transient)
		class USoundCue* LoadedMovieSoundCue;
	UPROPERTY(Transient)
		class UTexture2D* LoadedFallbackSnapshotImage;
};

USTRUCT(BlueprintType)
struct FB2UITitleMovieSetup
{
	GENERATED_USTRUCT_BODY()

	FB2UITitleMovieSetup()
	{
		RepresentingClass = EPCClass::EPC_End;
	}

	UPROPERTY(EditAnywhere)
		EPCClass RepresentingClass;

protected:
	UPROPERTY(EditAnywhere)
		class UMediaSource* MovieSource;

	UPROPERTY(EditAnywhere)
		class UMediaPlayer* PlayerSetup;

	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<class USoundCue>	MoiveSoundCue;

	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<class UTexture2D> FallbackSnapshotImage;

public:
	class UMediaSource* GetMovieSource(FB2UITitleMovieSetupLoadedCacher& LoadedCacher, bool bLoadIfUnloaded = true);
	class UMediaPlayer* GetMoviePlayer(FB2UITitleMovieSetupLoadedCacher& LoadedCacher, bool bLoadIfUnloaded = true);
	class USoundCue*	GetMoiveSoundCue(FB2UITitleMovieSetupLoadedCacher& LoadedCacher, bool bLoadIfUnloaded = true);
	class UTexture2D*	GetFallbackSnapshotImage(FB2UITitleMovieSetupLoadedCacher& LoadedCacher, bool bLoadIfUnloaded = true);

	/** Each movie has different design that proper BI location must be different. */
	UPROPERTY(EditAnywhere)
		FVector2D BITranslation;

};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITitleDesign : public UB2UnitedWidgetBase // Must be based on B2UnitedWidgetBase! This is used by DLCFront mode too!
{
	GENERATED_BODY()

	/**
	Either DLCTitle or LobbyTitle is the owner. Not both of them can be valid
	*/

	UPROPERTY(Transient)
		class UB2UITitleDLC* OwnerDLCTitle;

	UPROPERTY(Transient)
		class UB2UITitleScreen* OwnerLobbyTitle;

public:
	UB2UITitleDesign(const FObjectInitializer& ObjectInitializer);

	void Init(class UB2UnitedWidgetBase* InOwnerWidget);
	void DestroySelf();
	void CacheAssets();

	void SetConnectingUI();

private:
	bool IsLimitedResourceAvailable() const;

	FB2UITitleMovieSetup* GetMovieSetup(bool bApkOnly, EPCClass InPCClass);
	FB2UITitleMovieSetup* GetMovieSetupSafe(bool bApkOnly, EPCClass InPCClass);
	TWeakObjectPtr<UImage> GetMovieBackImageOfSetup(FB2UITitleMovieSetup* InSetup);

public:
	void SetHiddenAllBackImages();

	/** For the first time call, after game launch from DLC Title. */
	void StartMovieRandomPick();

	void StartMovie(EPCClass InWantedClass);
	UFUNCTION(BlueprintImplementableEvent, Category = "title_dlc")
		void StartMovieBP();

	/** Ignores UI widget Anim, pop up the movie screen to show immediately */
	void StartMovieImmediate(EPCClass InWantedClass);
	UFUNCTION(BlueprintImplementableEvent, Category = "title_dlc")
		void StartMovieImmediateBP();
protected:
	bool StartMovieBySetupCommon(FB2UITitleMovieSetup& InMovieSetup, int32 InDesiredStartPosSec = 0);

	/** Repeatedly try StartMovie internal function, for some unknown cases. */
	int32 ReaptedStartMovieCountdown;
	FTimerHandle ReaptedStartMovieTH;
	void ReaptedStartMovieBySetupCommon(FB2UITitleMovieSetup InMovieSetup, int32 InDesiredStartPosSec = 0);

	void ShowTitleMovieFallbackImage(FB2UITitleMovieSetup& InMovieSetup);

public:
	void TurnOffStartMovies();
	void StartMovieSound(USoundCue* pSoundCue);
	void StopCurrentMovieSound();
	void SetShowOtherCIs(bool bShow);

public:
	void PlayTouchAnim();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "title_dlc")
		void PlayTouchAnimBP();

	/** Contains setup of movie source deployed as a part of Apk, available even without DLC download. */
	UPROPERTY(EditAnywhere)
		FB2UITitleMovieSetup MovieSetup_ApkOnly;

	/** Movie setup that can be used after DLC download. */
	UPROPERTY(EditAnywhere)
		TArray<FB2UITitleMovieSetup> MovieSetup_Normal;

	// Only one cache is enough
	UPROPERTY(Transient)
		FB2UITitleMovieSetupLoadedCacher CurrLoadedMovieResourceCache;

	UPROPERTY(Transient)
		class UAudioComponent*	CurrentMovieSound;

	TWeakObjectPtr<UImage> StartBackMovie_ApkOnly;
	TMap<EPCClass, TWeakObjectPtr<UImage>> StartMovieImagePerPC;

	TWeakObjectPtr<UPanelWidget> PN_BI;
	TWeakObjectPtr<UPanelWidget> P_StartTextSet;
	TWeakObjectPtr<UTextBlock> TB_Start;

	TWeakObjectPtr<UPanelWidget> P_OtherCIs;

	TWeakObjectPtr<UImage> IMG_FallbackMovieSnapshot;
	TWeakObjectPtr<UImage> IMG_Loading;
	TWeakObjectPtr<UWidgetAnimation> ANIM_Loading;
};
