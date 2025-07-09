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
	// OwnerCharacter �� ������ �ʰ� NotifyTakeDamage ��� ������ ��ũ�� ��ǥ�� �����ϵ��� ��.
	// -> ������ ������ 3D��ǥ�� ��ġ���Ѻ���
		
	APlayerController* OwningPC = GetOwningPlayer();
	FVector2D vProjectedLocation;

	if (OwningPC && OwningPC->ProjectWorldLocationToScreen(m_vSpawnLocation, vProjectedLocation))
	{
		// �̰� �ؾ� ����� �˼��ֳ�..
		ForceLayoutPrepass();

		vProjectedLocation.X -= GetDesiredSize().X / 2;
		vProjectedLocation.Y -= GetDesiredSize().Y / 2;

		vProjectedLocation += FinalScreenOffset;

		SetPositionInViewport(vProjectedLocation);		
	}
	else
	{
		// ���ʾ��ų� �������� �����ϸ� ����.
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	// ForceHidden �� ��� �����ϴ� �͸�.
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
			// ���ο� ���.. ���� �̰͸� ���� ��.
			SetupDigitsForNumber(RecentDamageNum,
				(RecentDamageNum > 0.0f) ? 
					( bInCritical ? FLinearColor(BaseModulateColor_Critical) :FLinearColor(BaseModulateColor_Damage) )
					: FLinearColor(BaseModulateColor_Healing)
				); 

			if (VisibilityTimerHandle.IsValid())
			{
				OwningPC->GetWorldTimerManager().ClearTimer(VisibilityTimerHandle);
			}
			// Set the timer to turn the visibility off. -> �̰� Manager �ʿ��� ���� �ð����� ���� Ÿ�̸Ӹ� ��ġ�ϴ� ���ش�. 
			//OwningPC->GetWorldTimerManager().SetTimer(VisibilityTimerHandle, this, &UB2FloatingDamageNum::TakeDamageTimerCB, VisibleDuration, false);

			SetupForCriticalDamage(bInCritical);
		}
		m_vSpawnLocation = OwnerCharacter->GetTopLocation();
		m_vSpawnLocation.Z += AdditionalZOffset + OwnerCharacter->GetScaledDamageNumZOffset(); // DamageNumZOffset �� �߰���. ĳ���� ���� ���� �� �� ������ �ʿ��� �ֵ��� �־.
		
		// �Ź� �ణ�� �ٸ� ��ġ�� �Ѹ��� ���� SpawnLocation �� �� ����.
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
	// ���� ǥ�ô� ���밪����. Damage ���� Heal ������ �������� ǥ���� ��
	const int32 FinalNumber = FMath::Abs( FMath::Clamp(InNumberToDisplay, -1 * NumberLimit, NumberLimit) );
	// ���ڸ����� ǥ�� ��������.
	const int32 DisplayDigitCount = (int32)FMath::LogX(10.0f, (float)FinalNumber) + 1; // �̰� �� �ڸ��� ���� �ִ� �αٿ��� �ǵ���� ��������.. ���� 0 ���� ������ �� �ϳ� ���ؼ� �� �ټ���..

	// �ϴ� ���ܳ��ٰ�
	for (UImage* ThisDigitImage : AllDigits)
	{
		if (ThisDigitImage){
			ThisDigitImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	for (int32 DI = 0; DI < DisplayDigitCount; ++DI)
	{
		// �̹� �ڸ���.. ExtractNthDigitR �� ���ø� ��� ���ڸ� �޾ƾ� �ؼ�.. Dynamic ��������
		const int32 ThisDigitNumber = ExtractNthDigitR_Dynamic(FinalNumber, DI + 1);
		UImage* ThisDigitImage = AllDigits.IsValidIndex(DI) ? AllDigits[DI] : NULL;
		if (ThisDigitImage) // ���� ������ ��ġ�� ����� �Ǿ��ٸ� �̰� �ƴ� ���� �����..
		{
			ThisDigitImage->SetVisibility(ESlateVisibility::HitTestInvisible);

			// Image draw batching ����� ���� Material �� �ƴ� PaperSprite �� ����Ѵ�.
			UPaperSprite* DigitSprite = GetSpriteForDigit(ThisDigitNumber);
			if (DigitSprite)
			{
				SetImageBrushFromPaperSprite(ThisDigitImage, DigitSprite);
				// �� ���� UImage ���� GetDynamicMaterial �� �����͵� �ҿ��� ����.
				// ��� ���⼭�� UImage ��ü�� �ִ� ����� ����ؼ� �÷� ���������� ó��.
				SetBlendColorForPaperSpriteBoundImage(ThisDigitImage, InBlendColor);
			}
		}
	}

	// ǥ�� �ڸ����� ���� ������� ��ġ �����ؼ� ��� ���Ľ�Ŵ. WrapBox alignment �� �Ϸ��� ���� �ȸ�����..
	UCanvasPanelSlot* BasePanelSlot = WB_AllDisplaySet.IsValid() ? Cast<UCanvasPanelSlot>(WB_AllDisplaySet->Slot) : NULL;
	if (BasePanelSlot)
	{
		float DesiredWholeWidth = (float)DisplayDigitCount * SingleDigitWidth;
		BasePanelSlot->SetSize(FVector2D(DesiredWholeWidth, BasePanelSlot->GetSize().Y));
		// �߾� ��Ŀ�� ����.
		BasePanelSlot->SetPosition(FVector2D(DesiredWholeWidth * -0.5f, BasePanelSlot->GetPosition().Y));
	}
}

void UB2FloatingDamageNum::SetupForCriticalDamage(bool bIsCritical)
{
	// ������ ���⿡ ũ��Ƽ�� ������ ������ �־��� ����..

	if (PN_CriticalDamage.IsValid())
	{
		PN_CriticalDamage->SetVisibility(bIsCritical ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (IMG_CriticalDamageText.IsValid())
	{ // �̰� �ʿ���� �� ����. �⺻�����δ� ���ڿ� ȥ���Ϸ��� ������.
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