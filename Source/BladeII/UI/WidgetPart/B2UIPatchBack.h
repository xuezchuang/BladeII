// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIPatchBack.generated.h"

UENUM(BlueprintType)
enum class EBackMovie : uint8
{
	MOVIE_PVP = 0,
	MOVIE_CAPTURE,
	MOVIE_RAID,
	MOVIE_TeamPVP,
	MOVIE_NONE,
	MOVIE_Max
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPatchBack : public UB2UIWidget
{
	GENERATED_BODY()	

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	UB2UIPatchBack(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	UFUNCTION(BlueprintImplementableEvent, Category = PatchBack)
		void PlayPVPMovie();
	UFUNCTION(BlueprintImplementableEvent, Category = PatchBack)
		void PlayCaptureMovie();
	UFUNCTION(BlueprintImplementableEvent, Category = PatchBack)
		void PlayRaidMovie();
	UFUNCTION(BlueprintImplementableEvent, Category = PatchBack)
		void PlayTeamPVPMovie();
	UFUNCTION(BlueprintImplementableEvent, Category = PatchBack)
		void StopBackgroundMovie();
	UFUNCTION(BlueprintCallable, Category = PatchBack)
		void SelectBtn(EBackMovie eMovie);
	UFUNCTION(BlueprintCallable, Category = PatchBack)
		EBackMovie GetCurrentMovie();
	UFUNCTION(BlueprintCallable, Category = PatchBack)
		void SetCurrentMovie(EBackMovie eMoive);

	UFUNCTION(BlueprintCallable, Category = PatchBack)
		void SetLoopCheck(EBackMovie eMovie);
	UFUNCTION(BlueprintCallable, Category = PatchBack)
		bool GetLoopCheck(EBackMovie eMovie);
	UFUNCTION(BlueprintCallable, Category = PatchBack)
		void NextMovie(EBackMovie eMovie);

	UFUNCTION(BlueprintImplementableEvent, Category = PatchBack)
		void PlayDownLoadBGM();
	
	
	void StartPlayMovie();
	void StopAllMovie();

protected:
	UFUNCTION()
		void OnClickBTN_PVP();
	UFUNCTION()
		void OnClickBTN_Capture();
	UFUNCTION()
		void OnClickBTN_Raid();
	UFUNCTION()
		void OnClickBTN_TeamPVP();

private:

	struct MovieBtn
	{
		TWeakObjectPtr<UTextBlock>	SelectedText;
		TWeakObjectPtr<UTextBlock>	UnselectedText;
	};

	TWeakObjectPtr<UButton> BTN_PVP;
	TWeakObjectPtr<UTextBlock> TXT_PVP_Select;
	TWeakObjectPtr<UTextBlock> TXT_PVP_UnSelect;

	TWeakObjectPtr<UButton> BTN_Capture;	
	TWeakObjectPtr<UTextBlock> TXT_Capture_Select;
	TWeakObjectPtr<UTextBlock> TXT_Capture_UnSelect;

	TWeakObjectPtr<UButton> BTN_Raid;
	TWeakObjectPtr<UTextBlock> TXT_Raid_Select;
	TWeakObjectPtr<UTextBlock> TXT_Raid_UnSelect;

	TWeakObjectPtr<UButton> BTN_TeamPvP;	
	TWeakObjectPtr<UTextBlock> TXT_TeamPvP_Select;
	TWeakObjectPtr<UTextBlock> TXT_TeamPvP_UnSelect;

	TArray<UTextBlock*> m_PatchBackBtnTXTList;

	TWeakObjectPtr<UImage> MOV_PVP;
	TWeakObjectPtr<UImage> MOV_Capture;
	TWeakObjectPtr<UImage> MOV_Raid;
	TWeakObjectPtr<UImage> MOV_TeamPVP;
	TArray<UImage*> m_BackMovieList;

	//void SelectBtn(EBackMovie eMovie);

	TArray<MovieBtn> m_MovieBtnList;
	MovieBtn m_TempMovieBtn;
	EBackMovie m_eCurrentBackMovie;

	EBackMovie m_eLoopCheckMovie;
};
