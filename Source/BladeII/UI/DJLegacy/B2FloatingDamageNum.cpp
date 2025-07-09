// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2FloatingDamageNum.h"
#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"
#include "PaperSprite.h"
#include "BladeIIUtil.h"

static FName DamageNumMID_DamageNum_ParamName(TEXT("DamageNum"));
static FName DamageNumMID_FadingProgress_ParamName(TEXT("FadingProgress"));
static FName DamageNumMID_BaseModulateColor_ParamName(TEXT("BaseModulateColor"));

UB2FloatingDamageNum::UB2FloatingDamageNum(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	VisibleDuration = 1.0f;
	AdditionalZOffset = 100.0f;
	RandomPlaceXDistanceFromOwner = 0.0f;
	RandomPlaceYDistanceFromOwner = 0.0f;
	
	SingleDigitWidth = 40.0f;
	MaxDigitNum = 5;
	DigitImageNamePrefix = TEXT("NumDigit");


	BaseModulateColor_Damage = FColor(255, 0, 0);
	BaseModulateColor_Healing = FColor(0, 255, 0);
	BaseModulateColor_Critical = FColor(255, 127, 0);

	bIsVisible = false;
	FadingProgress = 0.0f;
	RecentDamageNum = 0;

	InitialProjectedLocation = FVector2D(0.0f, 0.0f);
	m_vSpawnLocation = FVector(0.0f, 0.0f, 0.0f);
}

void UB2FloatingDamageNum::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWrapBox, WB_AllDisplaySet);

	AllDigits.Empty();
	for (int32 DI = 1; DI <= MaxDigitNum; ++DI) // Lower one first.
	{
		const FString& ThisImageName = FString::Printf(TEXT("%s_%d"), *DigitImageNamePrefix, DI);
		UImage* ThisFoundElem = GetSlot<UImage>(FName(*ThisImageName));
		if (ThisFoundElem)
		{
			AllDigits.Add(ThisFoundElem);
		}
	}

	GET_SLOT(UPanelWidget, PN_CriticalDamage);
	GET_SLOT(UImage, IMG_CriticalDamageText);
}

void UB2FloatingDamageNum::SetOwnerCharacter(class ABladeIICharacter* InOwner)
{
	Super::SetOwnerCharacter(InOwner);

	UpdateFloatingOffset();
}

void UB2FloatingDamageNum::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UB2FloatingDamageNum::UpdatePosition()
{
	// OwnerCharacter 를 따라가지 않고 NotifyTakeDamage 당시 정해진 스크린 좌표를 유지하도록 함.
	// -> 생성시 결정된 3D좌표에 위치시켜보자
		
	APlayerController* OwningPC = GetOwningPlayer();
	FVector2D vProjectedLocation;

	if (OwningPC && OwningPC->ProjectWorldLocationToScreen(m_vSpawnLocation, vProjectedLocation))
	{
		// 이거 해야 사이즈를 알수있네..
		ForceLayoutPrepass();

		vProjectedLocation.X -= GetDesiredSize().X / 2;
		vProjectedLocation.Y -= GetDesiredSize().Y / 2;

		vProjectedLocation += FinalScreenOffset;

		SetPositionInViewport(vProjectedLocation);		
	}
	else
	{
		// 오너없거나 프로젝션 실패하면 숨김.
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	// ForceHidden 인 경우 유지하는 것만.
	SetVisibility((IsForceHidden() || ShouldBeHiddenAsPooledObject()) ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
}

void UB2FloatingDamageNum::NotifyTakeDamage(float Damage, bool bInCritical)
{
	if (FMath::Abs<float>(Damage) < 1.0f)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	APlayerController* OwningPC = GetOwningPlayer();
	if (OwnerCharacter && OwningPC)
	{
		bIsVisible = true;
		FadingProgress = 0.0f;
		RecentDamageNum = Damage;

		if (RecentDamageNum != 0.0f) // Let's count for negative damage, assuming that would be healing.
		{
			// 새로운 방식.. 향후 이것만 남길 것.
			SetupDigitsForNumber(RecentDamageNum,
				(RecentDamageNum > 0.0f) ? 
					( bInCritical ? FLinearColor(BaseModulateColor_Critical) :FLinearColor(BaseModulateColor_Damage) )
					: FLinearColor(BaseModulateColor_Healing)
				); 

			if (VisibilityTimerHandle.IsValid())
			{
				OwningPC->GetWorldTimerManager().ClearTimer(VisibilityTimerHandle);
			}
			// Set the timer to turn the visibility off. -> 이건 Manager 쪽에서 같은 시간으로 제거 타이머를 설치하니 없앤다. 
			//OwningPC->GetWorldTimerManager().SetTimer(VisibilityTimerHandle, this, &UB2FloatingDamageNum::TakeDamageTimerCB, VisibleDuration, false);

			SetupForCriticalDamage(bInCritical);
		}
		m_vSpawnLocation = OwnerCharacter->GetTopLocation();
		m_vSpawnLocation.Z += AdditionalZOffset + OwnerCharacter->GetScaledDamageNumZOffset(); // DamageNumZOffset 을 추가로. 캐릭터 종류 별로 좀 더 조절이 필요한 애들이 있어서.
		
		// 매번 약간씩 다른 위치에 뿌리기 위해 SpawnLocation 을 좀 조정.
		float RandDirX = FMath::FRandRange(-1.0f, 1.0f);
		float RandDirY = FMath::FRandRange(-1.0f, 1.0f);
		FVector2D RandDir(RandDirX, RandDirY);
		RandDir.Normalize();
		
		m_vSpawnLocation.X += RandomPlaceXDistanceFromOwner * RandDir.X;
		m_vSpawnLocation.Y += RandomPlaceYDistanceFromOwner * RandDir.Y;
		
		UpdatePosition();
	}

	ReceiveTakeDamage(Damage, bInCritical); // Call the BP event too.
}

void UB2FloatingDamageNum::SetupDigitsForNumber(int32 InNumberToDisplay, const FLinearColor& InBlendColor)
{
	const int32 NumberLimit = GetMaxDisplayableNum();
	// 숫자 표시는 절대값으로. Damage 인지 Heal 인지는 색상으로 표시할 거
	const int32 FinalNumber = FMath::Abs( FMath::Clamp(InNumberToDisplay, -1 * NumberLimit, NumberLimit) );
	// 몇자리까지 표시 가능한지.
	const int32 DisplayDigitCount = (int32)FMath::LogX(10.0f, (float)FinalNumber) + 1; // 이거 딱 자리수 걸쳐 있는 부근에서 의도대로 나오려나.. 끝이 0 으로 끝나는 건 하나 더해서 해 줄수도..

	// 일단 숨겨놨다가
	for (UImage* ThisDigitImage : AllDigits)
	{
		if (ThisDigitImage){
			ThisDigitImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	for (int32 DI = 0; DI < DisplayDigitCount; ++DI)
	{
		// 이번 자릿수.. ExtractNthDigitR 는 템플릿 상수 인자를 받아야 해서.. Dynamic 버전으로
		const int32 ThisDigitNumber = ExtractNthDigitR_Dynamic(FinalNumber, DI + 1);
		UImage* ThisDigitImage = AllDigits.IsValidIndex(DI) ? AllDigits[DI] : NULL;
		if (ThisDigitImage) // 물론 설정과 배치가 제대로 되었다면 이게 아닌 경우는 없어야..
		{
			ThisDigitImage->SetVisibility(ESlateVisibility::HitTestInvisible);

			// Image draw batching 사용을 위해 Material 이 아닌 PaperSprite 를 사용한다.
			UPaperSprite* DigitSprite = GetSpriteForDigit(ThisDigitNumber);
			if (DigitSprite)
			{
				SetImageBrushFromPaperSprite(ThisDigitImage, DigitSprite);
				// 이 경우는 UImage 에서 GetDynamicMaterial 로 가져와도 소용이 없음.
				// 대신 여기서는 UImage 자체에 있는 기능을 사용해서 컬러 블렌딩까지는 처리.
				SetBlendColorForPaperSpriteBoundImage(ThisDigitImage, InBlendColor);
			}
		}
	}

	// 표시 자릿수에 따라 사이즈랑 위치 조절해서 가운데 정렬시킴. WrapBox alignment 로 하려니 뭔가 안먹혀서..
	UCanvasPanelSlot* BasePanelSlot = WB_AllDisplaySet.IsValid() ? Cast<UCanvasPanelSlot>(WB_AllDisplaySet->Slot) : NULL;
	if (BasePanelSlot)
	{
		float DesiredWholeWidth = (float)DisplayDigitCount * SingleDigitWidth;
		BasePanelSlot->SetSize(FVector2D(DesiredWholeWidth, BasePanelSlot->GetSize().Y));
		// 중앙 앵커로 가정.
		BasePanelSlot->SetPosition(FVector2D(DesiredWholeWidth * -0.5f, BasePanelSlot->GetPosition().Y));
	}
}

void UB2FloatingDamageNum::SetupForCriticalDamage(bool bIsCritical)
{
	// 아직은 여기에 크리티컬 데미지 정보를 넣었다 정도..

	if (PN_CriticalDamage.IsValid())
	{
		PN_CriticalDamage->SetVisibility(bIsCritical ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (IMG_CriticalDamageText.IsValid())
	{ // 이건 필요없을 수 있음. 기본적으로는 숫자에 혼합하려는 색상임.
		IMG_CriticalDamageText->SetColorAndOpacity(BaseModulateColor_Critical);
	}
}

void UB2FloatingDamageNum::TakeDamageTimerCB()
{
	bIsVisible = false;

	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		// Not sure if this is dead necessary.
		OwningPC->GetWorldTimerManager().ClearTimer(VisibilityTimerHandle);
	}

	RecentDamageNum = 0;
}

ESlateVisibility UB2FloatingDamageNum::GetDamageNumVisibility() const
{
	return (bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

bool UB2FloatingDamageNum::IsOwnerPC()
{
	return (Cast<ABladeIIPlayer>(OwnerCharacter) != NULL);
}

void UB2FloatingDamageNum::UpdateFloatingOffset()
{
	// Calculate the offset based on the character's bound size
	if (OwnerCharacter)
	{
		if (USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh())
		{
			// Leave LocOffsetFromOwner X and Y. We can still configure it.
			LocOffsetFromOwner.Z = (OwnerMesh->Bounds.BoxExtent.Z * OwnerMesh->BoundsScale) + AdditionalZOffset; // Apply additional offset variable.
		}
	}
}