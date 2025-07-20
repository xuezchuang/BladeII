// Fill out your copyright notice in the Description page of Project Settings.
#include "B2FloatingLobbyOnHeadDisplay.h"
//#include "BladeII.h"
#include "BladeIIGameMode.h"

#include "Animation/SkeletalMeshActor.h"
#include "B2UIDocHelper.h"
#include "Event.h"
#include "B2ClientDataStore.h"


#include "BladeIIGameImpl.h"
#include "../../BladeII/BladeIILocText.h"

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
	UpdatePosition(); // 概 平 阂府变 窃.
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

	//拌鞭 包访
	if (OwnerPCClass != EPCClass::EPC_End)
	{
		int32 BrevetRank = BladeIIGameImpl::GetLocalCharacterData().GetCharBrevetRank(OwnerPCClass);

		// 0拌鞭老锭 钎扁救窃(抛捞喉俊 0拌鞭俊 措茄 单捞磐绰 乐澜 - 促弗单 荤侩)
		if (BrevetRank == 0)
		{
			if (TB_Rank.IsValid())
				TB_Rank->SetVisibility(ESlateVisibility::Hidden);
		
			if (IMG_RankMark.IsValid())
				IMG_RankMark->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			// 拌鞭疙
			if (TB_Rank.IsValid())
			{
				FString BrevetRankNameKeyText = BladeIIGameImpl::GetClientDataStore().GetBrevetRankNameKey(BrevetRank);
				const FText BrevetRankName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, BrevetRankNameKeyText);

				TB_Rank->SetText(BrevetRankName);
			}

			// 拌鞭 付农
			if (IMG_RankMark.IsValid())
			{
				UMaterialInterface* pMAT = BladeIIGameImpl::GetClientDataStore().GetBrevetRankIconMaterial(BrevetRank);

				IMG_RankMark->SetBrushFromMaterial(pMAT);
			}
		}
	}

	//////////////////////////////////////////////////
	// 泅犁 Observe 拳搁俊辑狼 困摹 技泼捞 局概窍咯 Observe 拳搁俊辑绰 傲 见扁扁肺 窃. NickName 俊叼飘窍绰 滚瓢篮 UB2UICharacterObserve 俊 狄.
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
	UpdatePosition(); // 贸澜 阂副 锭绰 措眉肺 坊歹傅 厘搁 傈券 磊眉绰 救登绢 乐绢辑 弊繁瘤 projection 捞 角菩窍绰 淀.
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
			// LocOffsetFromOwner 阑 Actor 困摹俊 弊措肺 利侩且 荐档 乐绊, 郴瘤绰 MeshComponent 狼 官款靛甫 绊妨窍咯 老窜 赣府 困肺 棵赴 促澜俊 眠啊 炼例阑 窍绰 侥栏肺 利侩且 荐档 乐促.
			// Actor 困摹俊 弊措肺 利侩窍搁 SkeletalMeshActor 绰 官蹿捞 盔痢捞骨肺 某腐磐 农扁俊 惑包绝捞 悼老茄 臭捞俊 困摹窍霸 瞪 巴. 酒付 弊霸 唱阑 芭促.
			FVector WidgetWorldLocation = OwnerSKActor->GetActorLocation() + 
				(MasterType == ELobbyOnHeadDisplayType::LOHDT_LobbyMain ? LocOffsetFromOwner_LobbyMain : LocOffsetFromOwner_CharObserve);

			FVector2D ProjectedLocation;
			if (GetOwningPlayer()->ProjectWorldLocationToScreen(WidgetWorldLocation, ProjectedLocation))
			{
				ProjectedLocation += FinalScreenOffset;
				SetPositionInViewport(ProjectedLocation);
				SelfVisibility = ESlateVisibility::Visible;
				//bWidgetProjectedPosBeingSet = true; // 汲飞 Tick 俊辑 阂府歹扼档 茄锅父 技泼窍妨绊 沁绰单 购啊 救冈塞 ぇぇ
			}
		}

		//////////////////////////////////////////////////
		// 泅犁 Observe 拳搁俊辑狼 困摹 技泼捞 局概窍咯 Observe 拳搁俊辑绰 傲 见扁扁肺 窃. NickName 俊叼飘窍绰 滚瓢篮 UB2UICharacterObserve 俊 狄.
		if (MasterType == ELobbyOnHeadDisplayType::LOHDT_CharObserve)
		{
			SelfVisibility = ESlateVisibility::Collapsed;
		}
		//////////////////////////////////////////////////

		// ForceHidden 牢 版快 蜡瘤. 咯扁辑 SetVisibility 甫 流立 荤侩窍绰 措脚 阿 widget 喊肺 visibility 官牢爹阑 窍绰 霸 亮阑 巴档 鞍促. 
		SetVisibility(IsForceHidden() ? ESlateVisibility::Hidden : SelfVisibility);
	}
}

void UB2FloatingLobbyOnHeadDisplay::DestroySelf()
{
	Super::DestroySelf();
	OwnerSKActor = NULL;
}