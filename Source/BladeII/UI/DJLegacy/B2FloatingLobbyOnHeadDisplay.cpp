// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "BladeIIGameMode.h"
#include "B2FloatingLobbyOnHeadDisplay.h"
#include "Animation/SkeletalMeshActor.h"
#include "B2UIDocHelper.h"
#include "Event.h"
#include "B2ClientDataStore.h"


#include "BladeIIGameImpl.h"

UB2FloatingLobbyOnHeadDisplay::UB2FloatingLobbyOnHeadDisplay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MasterType = ELobbyOnHeadDisplayType::LOHDT_LobbyMain;

	OwnerSKActor = NULL;
	LocOffsetFromOwner_LobbyMain = FVector(0.0f, 0.0f, 180.0f);
	LocOffsetFromOwner_CharObserve = FVector(0.0f, 0.0f, 150.0f);
	FinalScreenOffset = FVector2D(0.0f, 0.0f);
	OwnerPCClass = EPCClass::EPC_End;
	bWidgetProjectedPosBeingSet = false;
}

void UB2FloatingLobbyOnHeadDisplay::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_LevelAndOrName);
	GET_SLOT(UB2Button, BTN_SetNickName);
	GET_SLOT(UTextBlock, TB_Rank);
	GET_SLOT(UImage, IMG_RankMark);
	BIND_CLICK_FUNC_TO_BTN(BTN_SetNickName, &UB2FloatingLobbyOnHeadDisplay::OnClickSetNickNameBtn)
}

void UB2FloatingLobbyOnHeadDisplay::SetMasterType(ELobbyOnHeadDisplayType InType)
{
	MasterType = InType;

	UpdateOnMasterTypeChange();
	UpdatePosition(); // 매 틱 불리긴 함.
}

void UB2FloatingLobbyOnHeadDisplay::UpdateOnMasterTypeChange()
{
	int32 PCLevel = 1;

	if (OwnerPCClass != EPCClass::EPC_End)
	{
		PCLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(OwnerPCClass);
	}
		
	const FText ClassOrNickName = BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(OwnerPCClass);

	if (TB_LevelAndOrName.IsValid())
	{
		if (MasterType == ELobbyOnHeadDisplayType::LOHDT_LobbyMain)
		{
			TB_LevelAndOrName->SetText(
				FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyCharOnHeadLevelAndName"))), 
				FText::FromString(FString::FromInt(PCLevel)), ClassOrNickName)
				);
		}
		else
		{
			TB_LevelAndOrName->SetText(ClassOrNickName);
		}
	}

	if (BTN_SetNickName.IsValid())
	{
		BTN_SetNickName->SetVisibility(MasterType == ELobbyOnHeadDisplayType::LOHDT_LobbyMain ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}

	//계급 관련
	if (OwnerPCClass != EPCClass::EPC_End)
	{
		int32 BrevetRank = BladeIIGameImpl::GetLocalCharacterData().GetCharBrevetRank(OwnerPCClass);

		// 0계급일때 표기안함(테이블에 0계급에 대한 데이터는 있음 - 다른데 사용)
		if (BrevetRank == 0)
		{
			if (TB_Rank.IsValid())
				TB_Rank->SetVisibility(ESlateVisibility::Hidden);
		
			if (IMG_RankMark.IsValid())
				IMG_RankMark->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			// 계급명
			if (TB_Rank.IsValid())
			{
				FString BrevetRankNameKeyText = BladeIIGameImpl::GetClientDataStore().GetBrevetRankNameKey(BrevetRank);
				const FText BrevetRankName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, BrevetRankNameKeyText);

				TB_Rank->SetText(BrevetRankName);
			}

			// 계급 마크
			if (IMG_RankMark.IsValid())
			{
				UMaterialInterface* pMAT = BladeIIGameImpl::GetClientDataStore().GetBrevetRankIconMaterial(BrevetRank);

				IMG_RankMark->SetBrushFromMaterial(pMAT);
			}
		}
	}

	//////////////////////////////////////////////////
	// 현재 Observe 화면에서의 위치 세팅이 애매하여 Observe 화면에서는 걍 숨기기로 함. NickName 에디트하는 버튼은 UB2UICharacterObserve 에 둠.
	if (MasterType == ELobbyOnHeadDisplayType::LOHDT_CharObserve)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
	//////////////////////////////////////////////////
}

void UB2FloatingLobbyOnHeadDisplay::SetOwnerInfo(EPCClass InOwnerClass, class ASkeletalMeshActor* InOwner)
{
	OwnerPCClass = InOwnerClass;
	OwnerSKActor = InOwner; 

	UpdateOnMasterTypeChange();
	UpdatePosition(); // 처음 불릴 때는 대체로 렌더링 장면 전환 자체는 안되어 있어서 그런지 projection 이 실패하는 듯.
}

void UB2FloatingLobbyOnHeadDisplay::OnClickSetNickNameBtn()
{
	if (OwnerPCClass != EPCClass::EPC_End)
	{
		LobbyCharObservePopupNickEditClass<EPCClass>::GetInstance().Signal(OwnerPCClass);
	}
}

void UB2FloatingLobbyOnHeadDisplay::UpdatePosition()
{
	if (OwnerSKActor)
	{
		// Visibility settings here assumes this widget is for information visualization.

		ESlateVisibility SelfVisibility = ESlateVisibility::Hidden;

		if (/*!bWidgetProjectedPosBeingSet && */GetOwningPlayer())
		{
			// Follows this character position with specified offset.
			// LocOffsetFromOwner 을 Actor 위치에 그대로 적용할 수도 있고, 내지는 MeshComponent 의 바운드를 고려하여 일단 머리 위로 올린 다음에 추가 조절을 하는 식으로 적용할 수도 있다.
			// Actor 위치에 그대로 적용하면 SkeletalMeshActor 는 바닥이 원점이므로 캐릭터 크기에 상관없이 동일한 높이에 위치하게 될 것. 아마 그게 나을 거다.
			FVector WidgetWorldLocation = OwnerSKActor->GetActorLocation() + 
				(MasterType == ELobbyOnHeadDisplayType::LOHDT_LobbyMain ? LocOffsetFromOwner_LobbyMain : LocOffsetFromOwner_CharObserve);

			FVector2D ProjectedLocation;
			if (GetOwningPlayer()->ProjectWorldLocationToScreen(WidgetWorldLocation, ProjectedLocation))
			{
				ProjectedLocation += FinalScreenOffset;
				SetPositionInViewport(ProjectedLocation);
				SelfVisibility = ESlateVisibility::Visible;
				//bWidgetProjectedPosBeingSet = true; // 설령 Tick 에서 불리더라도 한번만 세팅하려고 했는데 뭔가 안먹힘 ㄷㄷ
			}
		}

		//////////////////////////////////////////////////
		// 현재 Observe 화면에서의 위치 세팅이 애매하여 Observe 화면에서는 걍 숨기기로 함. NickName 에디트하는 버튼은 UB2UICharacterObserve 에 둠.
		if (MasterType == ELobbyOnHeadDisplayType::LOHDT_CharObserve)
		{
			SelfVisibility = ESlateVisibility::Collapsed;
		}
		//////////////////////////////////////////////////

		// ForceHidden 인 경우 유지. 여기서 SetVisibility 를 직접 사용하는 대신 각 widget 별로 visibility 바인딩을 하는 게 좋을 것도 같다. 
		SetVisibility(IsForceHidden() ? ESlateVisibility::Hidden : SelfVisibility);
	}
}

void UB2FloatingLobbyOnHeadDisplay::DestroySelf()
{
	Super::DestroySelf();
	OwnerSKActor = NULL;
}