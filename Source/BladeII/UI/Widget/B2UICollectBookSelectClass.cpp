// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UICollectBookSelectClass.h"
#include "Event.h"
#include "Retailer.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UICollectBookMain.h"



#include "BladeIIGameImpl.h"

void UB2UICollectBookSelectClass::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2UICollectBookClassPart, UIP_ClassGladiator);
	if (UIP_ClassGladiator.IsValid())
	{
		UIP_ClassGladiator->Init();
		UIP_ClassGladiator->InitClassType(EPCClass::EPC_Gladiator);
	}
	GET_SLOT(UB2UICollectBookClassPart, UIP_ClassAssassin);
	if (UIP_ClassAssassin.IsValid())
	{
		UIP_ClassAssassin->Init();
		UIP_ClassAssassin->InitClassType(EPCClass::EPC_Assassin);
	}
	GET_SLOT(UB2UICollectBookClassPart, UIP_ClassWizard);
	if (UIP_ClassWizard.IsValid())
	{
		UIP_ClassWizard->Init();
		UIP_ClassWizard->InitClassType(EPCClass::EPC_Wizard);
	}
	GET_SLOT(UB2UICollectBookClassPart, UIP_ClassFighter);
	if (UIP_ClassFighter.IsValid())
	{
		UIP_ClassFighter->Init();
		UIP_ClassFighter->InitClassType(EPCClass::EPC_Fighter);
	}

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	GET_SLOT(UTextBlock, TB_Title)
}

void UB2UICollectBookSelectClass::BindDelegates()
{
	Super::BindDelegates();
}


void UB2UICollectBookSelectClass::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_SelectPartDescription")));

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("CollectBook"));
}

void UB2UICollectBookSelectClass::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("CollectBook"));
}

void UB2UICollectBookSelectClass::Init()
{
	Super::Init();
	DeliveryGetCollectionItemTicket = 0;
	SubscribeEvent();
}

void UB2UICollectBookSelectClass::DestroySelf(UB2UIManager* InUIManager)
{

	if(UIP_ClassGladiator.IsValid()) UIP_ClassGladiator->DestroySelf(InUIManager);
	if(UIP_ClassAssassin.IsValid()) UIP_ClassAssassin->DestroySelf(InUIManager);
	if(UIP_ClassWizard.IsValid()) UIP_ClassWizard->DestroySelf(InUIManager);
	if(UIP_ClassFighter.IsValid()) UIP_ClassFighter->DestroySelf(InUIManager);

	UnSubscribeEvent();
	Super::DestroySelf(InUIManager);
}

void UB2UICollectBookSelectClass::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UICollectBookSelectClass);

	DeliveryGetCollectionItemTicket = DeliveryGetCollectionItemClass<FB2CollectionItemPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetCollectionItem, FB2CollectionItemPtr CollectionItems)
		Capture->OnReceiveCollectionItems(CollectionItems);
	END_CAPTURE_OBJECT()
		);
}

void UB2UICollectBookSelectClass::UnSubscribeEvent()
{
	DeliveryGetCollectionItemClass<FB2CollectionItemPtr>::GetInstance().Unsubscribe(DeliveryGetCollectionItemTicket);
}



void UB2UICollectBookSelectClass::OnReceiveCollectionItems(FB2CollectionItemPtr CollectionItems)
{
	if (UIP_ClassGladiator.IsValid()) UIP_ClassGladiator->InitcollectionProgress();
	if (UIP_ClassAssassin.IsValid()) UIP_ClassAssassin->InitcollectionProgress();
	if (UIP_ClassWizard.IsValid()) UIP_ClassWizard->InitcollectionProgress();
	if (UIP_ClassFighter.IsValid()) UIP_ClassFighter->InitcollectionProgress();
}

///////////////////////////////////////////////////////////////////////////////////



void UB2UICollectBookClassPart::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UOverlay, O_CollectPerfect);
	GET_SLOT(UTextBlock, TB_CollectPerfect);

	GET_SLOT(UOverlay, O_CollectProgress);
	GET_SLOT(UProgressBar, PB_CollectRate);
	GET_SLOT(UTextBlock, TB_CollectRateLeft);
	GET_SLOT(UTextBlock, TB_CollectRateRight);

	GET_SLOT(UB2Image, IMG_Character);
	GET_SLOT(UTextBlock, TB_Character);
	GET_SLOT(UB2UIWidgetBase, IMG_NewIcon);
	GET_SLOT(UButton, BTN_Area);

	GET_SLOT(UTextBlock, TB_CollectRatePer);
}

void UB2UICollectBookClassPart::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UICollectBookClassPart::OnClickBtn_Area)
}

void UB2UICollectBookClassPart::Init()
{
	Super::Init();
}

void UB2UICollectBookClassPart::DestroySelf(UB2UIManager* InUIManager)
{


	Super::DestroySelf(InUIManager);
}

void UB2UICollectBookClassPart::SynchronizeProperties()
{
	Super::SynchronizeProperties();

#if WITH_EDITOR
	if (IMG_Character.IsValid() == false)
		CacheAssets();
	
	if (IMG_Character.IsValid() && ClassMaterial != nullptr)
		IMG_Character->SetBrushFromMaterial(ClassMaterial);

#endif
}


void UB2UICollectBookClassPart::InitClassType(EPCClass InClassType)
{
	ClassType = InClassType;

	FText ClassName = GetPCClassLocalizedText(InClassType);
	if (TB_Character.IsValid())
		TB_Character->SetText(ClassName);

	if (IMG_Character.IsValid() && ClassMaterial != nullptr)
		IMG_Character->SetBrushFromMaterial(ClassMaterial);

	InitcollectionProgress();
}


void UB2UICollectBookClassPart::InitcollectionProgress()
{
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();

	int32 ItemCount = 0;
	int32 CollectionCount = 0;

	const int32 CollectionBegin = static_cast<int32>(ECollectionType::Weapon);
	const int32 CollectionEnd = static_cast<int32>(ECollectionType::Accessory);

	for (int32 Idx = CollectionBegin; Idx <= CollectionEnd; Idx++)
	{
		ItemCount += GetTotalItemCount(ClassType, static_cast<ECollectionType>(Idx));
		CollectionCount += GetCollectionItemCount(ClassType, static_cast<ECollectionType>(Idx));
	}
	SetCollectionProgress(CollectionCount, ItemCount);


	bool bHasEnhancedItem = DataStore.HasEnhancedCollectionItem(ClassType);
	if (bHasEnhancedItem && IMG_NewIcon.IsValid())
	{
		IMG_NewIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UB2UICollectBookClassPart::SetCollectionProgress(int32 CurrentCount, int32 MaxCount)
{
	bool bCollectionPerfect = (MaxCount > 0 && CurrentCount == MaxCount);
	
	if (O_CollectPerfect.IsValid()) O_CollectPerfect->SetVisibility(bCollectionPerfect ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (O_CollectProgress.IsValid()) O_CollectProgress->SetVisibility(bCollectionPerfect ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	if (bCollectionPerfect == false && MaxCount > 0)
	{
		float Progress = static_cast<float>(CurrentCount) / static_cast<float>(MaxCount);
		if (PB_CollectRate.IsValid())
			PB_CollectRate->SetPercent(Progress);

		if (TB_CollectRateLeft.IsValid())
		{
			FText ProgressText = FText::Format(FText::FromString(TEXT(" {0}")), FText::AsNumber(CurrentCount));
			TB_CollectRateLeft->SetText(ProgressText);
		}
		if (TB_CollectRateRight.IsValid())
		{
			FText ProgressText = FText::Format(FText::FromString(TEXT("/ {0}")), FText::AsNumber(MaxCount));
			TB_CollectRateRight->SetText(ProgressText);
		}

		if (TB_CollectRatePer.IsValid())
		{
			int32 Percent = (float(CurrentCount) / float(MaxCount)) * 100;
			TB_CollectRatePer->SetText(FText::Format(FText::FromString(TEXT("{0}%")) ,FText::AsNumber(Percent)));
		}
	}
	else if (bCollectionPerfect)
	{
		if (TB_CollectPerfect.IsValid())
			TB_CollectPerfect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_CollectPerfect")));
	}
}

void UB2UICollectBookClassPart::OnClickBtn_Area()
{
	UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome)
		DocSome->SetCollectBookSelectClass(ClassType);

	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::CollectBookMain);
}
