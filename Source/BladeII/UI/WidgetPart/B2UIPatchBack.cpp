// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIPatchBack.h"
#include "B2UIManager.h"


UB2UIPatchBack::UB2UIPatchBack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIPatchBack::CacheAssets()
{	
	Super::CacheAssets();
	m_MovieBtnList.Empty();

	GET_SLOT(UButton, BTN_PVP);
	GET_SLOT(UTextBlock, TXT_PVP_Select);
	GET_SLOT(UTextBlock, TXT_PVP_UnSelect);
	m_TempMovieBtn.SelectedText = TXT_PVP_Select;
	m_TempMovieBtn.UnselectedText = TXT_PVP_UnSelect;
	m_MovieBtnList.Add(m_TempMovieBtn);

	GET_SLOT(UButton, BTN_Capture);
	GET_SLOT(UTextBlock, TXT_Capture_Select);
	GET_SLOT(UTextBlock, TXT_Capture_UnSelect);
	m_TempMovieBtn.SelectedText = TXT_Capture_Select;
	m_TempMovieBtn.UnselectedText = TXT_Capture_UnSelect;
	m_MovieBtnList.Add(m_TempMovieBtn);

	GET_SLOT(UButton, BTN_Raid);
	GET_SLOT(UTextBlock, TXT_Raid_Select);
	GET_SLOT(UTextBlock, TXT_Raid_UnSelect);
	m_TempMovieBtn.SelectedText = TXT_Raid_Select;
	m_TempMovieBtn.UnselectedText = TXT_Raid_UnSelect;
	m_MovieBtnList.Add(m_TempMovieBtn);

	GET_SLOT(UButton, BTN_TeamPvP);
	GET_SLOT(UTextBlock, TXT_TeamPvP_Select);
	GET_SLOT(UTextBlock, TXT_TeamPvP_UnSelect);
	m_TempMovieBtn.SelectedText = TXT_TeamPvP_Select;
	m_TempMovieBtn.UnselectedText = TXT_TeamPvP_UnSelect;
	m_MovieBtnList.Add(m_TempMovieBtn);	


	m_BackMovieList.Empty();
	GET_SLOT(UImage, MOV_PVP);
	m_BackMovieList.Add(MOV_PVP.Get());
	GET_SLOT(UImage, MOV_Capture);
	m_BackMovieList.Add(MOV_Capture.Get());
	GET_SLOT(UImage, MOV_Raid);
	m_BackMovieList.Add(MOV_Raid.Get());
	GET_SLOT(UImage, MOV_TeamPVP);
	m_BackMovieList.Add(MOV_TeamPVP.Get());

}

void UB2UIPatchBack::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_PVP, &UB2UIPatchBack::OnClickBTN_PVP);
	BIND_CLICK_FUNC_TO_BTN(BTN_Capture, &UB2UIPatchBack::OnClickBTN_Capture);
	BIND_CLICK_FUNC_TO_BTN(BTN_Raid, &UB2UIPatchBack::OnClickBTN_Raid);
	BIND_CLICK_FUNC_TO_BTN(BTN_TeamPvP, &UB2UIPatchBack::OnClickBTN_TeamPVP);
}

void UB2UIPatchBack::Init()
{
	Super::Init();
}

void UB2UIPatchBack::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

void UB2UIPatchBack::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UB2UIPatchBack::OnClickBTN_PVP()
{
	if (m_eCurrentBackMovie == EBackMovie::MOVIE_PVP)
	{
		return;
	}
	UB2UIPatchBack::SelectBtn(EBackMovie::MOVIE_PVP);
	PlayPVPMovie();
}

void UB2UIPatchBack::OnClickBTN_Capture()
{
	if (m_eCurrentBackMovie == EBackMovie::MOVIE_CAPTURE)
	{
		return;
	}
	UB2UIPatchBack::SelectBtn(EBackMovie::MOVIE_CAPTURE);
	PlayCaptureMovie();
}

void UB2UIPatchBack::OnClickBTN_Raid()
{
	if (m_eCurrentBackMovie == EBackMovie::MOVIE_RAID)
	{
		return;
	}
	UB2UIPatchBack::SelectBtn(EBackMovie::MOVIE_RAID);
	PlayRaidMovie();
}

void UB2UIPatchBack::OnClickBTN_TeamPVP()
{
	if (m_eCurrentBackMovie == EBackMovie::MOVIE_TeamPVP)
	{
		return;
	}
	UB2UIPatchBack::SelectBtn(EBackMovie::MOVIE_TeamPVP);
	PlayTeamPVPMovie();
}

void UB2UIPatchBack::StartPlayMovie()
{
	SelectBtn(EBackMovie::MOVIE_PVP);
	SetCurrentMovie(EBackMovie::MOVIE_PVP);
	PlayDownLoadBGM();
	PlayPVPMovie();
}

void UB2UIPatchBack::StopAllMovie()
{
	StopBackgroundMovie();
}

void UB2UIPatchBack::SelectBtn(EBackMovie eMovie)
{
	for (int32 i = 0; i < m_MovieBtnList.Num(); i++)
	{
		if (i == int32(eMovie))
		{
			m_MovieBtnList[i].SelectedText->SetVisibility(ESlateVisibility::Visible);
			m_MovieBtnList[i].UnselectedText->SetVisibility(ESlateVisibility::Hidden);
			m_BackMovieList[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			m_MovieBtnList[i].SelectedText->SetVisibility(ESlateVisibility::Hidden);
			m_MovieBtnList[i].UnselectedText->SetVisibility(ESlateVisibility::Visible);
			m_BackMovieList[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2UIPatchBack::NextMovie(EBackMovie eMovie)
{
	switch (eMovie)
	{
	case EBackMovie::MOVIE_PVP:
		PlayPVPMovie();
		break;
	case EBackMovie::MOVIE_CAPTURE:
		PlayCaptureMovie();
		break;
	case EBackMovie::MOVIE_RAID:
		PlayRaidMovie();
		break;
	case EBackMovie::MOVIE_TeamPVP:
		PlayTeamPVPMovie();
		break;
	}
}

void UB2UIPatchBack::SetCurrentMovie(EBackMovie eMoive)
{
	m_eCurrentBackMovie = eMoive;
}

EBackMovie UB2UIPatchBack::GetCurrentMovie()
{
	return m_eCurrentBackMovie;
}

void UB2UIPatchBack::SetLoopCheck(EBackMovie eMovie)
{
	m_eLoopCheckMovie = eMovie;
}
bool UB2UIPatchBack::GetLoopCheck(EBackMovie eMovie)
{
	if (eMovie == m_eLoopCheckMovie)
		return true;
	else
		return false;
}

