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
	UpdatePosition(); // �� ƽ �Ҹ��� ��.
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

	//��� ����
	if (OwnerPCClass != EPCClass::EPC_End)
	{
		int32 BrevetRank = BladeIIGameImpl::GetLocalCharacterData().GetCharBrevetRank(OwnerPCClass);

		// 0����϶� ǥ�����(���̺� 0��޿� ���� �����ʹ� ���� - �ٸ��� ���)
		if (BrevetRank == 0)
		{
			if (TB_Rank.IsValid())
				TB_Rank->SetVisibility(ESlateVisibility::Hidden);
		
			if (IMG_RankMark.IsValid())
				IMG_RankMark->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			// ��޸�
			if (TB_Rank.IsValid())
			{
				FString BrevetRankNameKeyText = BladeIIGameImpl::GetClientDataStore().GetBrevetRankNameKey(BrevetRank);
				const FText BrevetRankName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, BrevetRankNameKeyText);

				TB_Rank->SetText(BrevetRankName);
			}

			// ��� ��ũ
			if (IMG_RankMark.IsValid())
			{
				UMaterialInterface* pMAT = BladeIIGameImpl::GetClientDataStore().GetBrevetRankIconMaterial(BrevetRank);

				IMG_RankMark->SetBrushFromMaterial(pMAT);
			}
		}
	}

	//////////////////////////////////////////////////
	// ���� Observe ȭ�鿡���� ��ġ ������ �ָ��Ͽ� Observe ȭ�鿡���� �� ������ ��. NickName ����Ʈ�ϴ� ��ư�� UB2UICharacterObserve �� ��.
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
	UpdatePosition(); // ó�� �Ҹ� ���� ��ü�� ������ ��� ��ȯ ��ü�� �ȵǾ� �־ �׷��� projection �� �����ϴ� ��.
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
			// LocOffsetFromOwner �� Actor ��ġ�� �״�� ������ ���� �ְ�, ������ MeshComponent �� �ٿ�带 ����Ͽ� �ϴ� �Ӹ� ���� �ø� ������ �߰� ������ �ϴ� ������ ������ ���� �ִ�.
			// Actor ��ġ�� �״�� �����ϸ� SkeletalMeshActor �� �ٴ��� �����̹Ƿ� ĳ���� ũ�⿡ ������� ������ ���̿� ��ġ�ϰ� �� ��. �Ƹ� �װ� ���� �Ŵ�.
			FVector WidgetWorldLocation = OwnerSKActor->GetActorLocation() + 
				(MasterType == ELobbyOnHeadDisplayType::LOHDT_LobbyMain ? LocOffsetFromOwner_LobbyMain : LocOffsetFromOwner_CharObserve);

			FVector2D ProjectedLocation;
			if (GetOwningPlayer()->ProjectWorldLocationToScreen(WidgetWorldLocation, ProjectedLocation))
			{
				ProjectedLocation += FinalScreenOffset;
				SetPositionInViewport(ProjectedLocation);
				SelfVisibility = ESlateVisibility::Visible;
				//bWidgetProjectedPosBeingSet = true; // ���� Tick ���� �Ҹ����� �ѹ��� �����Ϸ��� �ߴµ� ���� �ȸ��� ����
			}
		}

		//////////////////////////////////////////////////
		// ���� Observe ȭ�鿡���� ��ġ ������ �ָ��Ͽ� Observe ȭ�鿡���� �� ������ ��. NickName ����Ʈ�ϴ� ��ư�� UB2UICharacterObserve �� ��.
		if (MasterType == ELobbyOnHeadDisplayType::LOHDT_CharObserve)
		{
			SelfVisibility = ESlateVisibility::Collapsed;
		}
		//////////////////////////////////////////////////

		// ForceHidden �� ��� ����. ���⼭ SetVisibility �� ���� ����ϴ� ��� �� widget ���� visibility ���ε��� �ϴ� �� ���� �͵� ����. 
		SetVisibility(IsForceHidden() ? ESlateVisibility::Hidden : SelfVisibility);
	}
}

void UB2FloatingLobbyOnHeadDisplay::DestroySelf()
{
	Super::DestroySelf();
	OwnerSKActor = NULL;
}