// Fill out your copyright notice in the Description page of Project Settings.
#include "B2FloatingDamageNum.h"
//#include "BladeII.h"

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
	// OwnerCharacter 甫 蝶扼啊瘤 臼绊 NotifyTakeDamage 寸矫 沥秦柳 胶农赴 谅钎甫 蜡瘤窍档废 窃.
	// -> 积己矫 搬沥等 3D谅钎俊 困摹矫难焊磊
		
	APlayerController* OwningPC = GetOwningPlayer();
	FVector2D vProjectedLocation;

	if (OwningPC && OwningPC->ProjectWorldLocationToScreen(m_vSpawnLocation, vProjectedLocation))
	{
		// 捞芭 秦具 荤捞令甫 舅荐乐匙..
		ForceLayoutPrepass();

		vProjectedLocation.X -= GetDesiredSize().X / 2;
		vProjectedLocation.Y -= GetDesiredSize().Y / 2;

		vProjectedLocation += FinalScreenOffset;

		SetPositionInViewport(vProjectedLocation);		
	}
	else
	{
		// 坷呈绝芭唱 橇肺璃记 角菩窍搁 见辫.
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	// ForceHidden 牢 版快 蜡瘤窍绰 巴父.
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
			// 货肺款 规侥.. 氢饶 捞巴父 巢辨 巴.
			SetupDigitsForNumber(RecentDamageNum,
				(RecentDamageNum > 0.0f) ? 
					( bInCritical ? FLinearColor(BaseModulateColor_Critical) :FLinearColor(BaseModulateColor_Damage) )
					: FLinearColor(BaseModulateColor_Healing)
				); 

			if (VisibilityTimerHandle.IsValid())
			{
				OwningPC->GetWorldTimerManager().ClearTimer(VisibilityTimerHandle);
			}
			// Set the timer to turn the visibility off. -> 捞扒 Manager 率俊辑 鞍篮 矫埃栏肺 力芭 鸥捞赣甫 汲摹窍聪 绝矩促. 
			//OwningPC->GetWorldTimerManager().SetTimer(VisibilityTimerHandle, this, &UB2FloatingDamageNum::TakeDamageTimerCB, VisibleDuration, false);

			SetupForCriticalDamage(bInCritical);
		}
		m_vSpawnLocation = OwnerCharacter->GetTopLocation();
		m_vSpawnLocation.Z += AdditionalZOffset + OwnerCharacter->GetScaledDamageNumZOffset(); // DamageNumZOffset 阑 眠啊肺. 某腐磐 辆幅 喊肺 粱 歹 炼例捞 鞘夸茄 局甸捞 乐绢辑.
		
		// 概锅 距埃究 促弗 困摹俊 谎府扁 困秦 SpawnLocation 阑 粱 炼沥.
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
	// 箭磊 钎矫绰 例措蔼栏肺. Damage 牢瘤 Heal 牢瘤绰 祸惑栏肺 钎矫且 芭
	const int32 FinalNumber = FMath::Abs( FMath::Clamp(InNumberToDisplay, -1 * NumberLimit, NumberLimit) );
	// 割磊府鳖瘤 钎矫 啊瓷茄瘤.
	const int32 DisplayDigitCount = (int32)FMath::LogX(10.0f, (float)FinalNumber) + 1; // 捞芭 迭 磊府荐 吧媚 乐绰 何辟俊辑 狼档措肺 唱坷妨唱.. 场捞 0 栏肺 场唱绰 扒 窍唱 歹秦辑 秦 临荐档..

	// 老窜 见败躇促啊
	for (UImage* ThisDigitImage : AllDigits)
	{
		if (ThisDigitImage){
			ThisDigitImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	for (int32 DI = 0; DI < DisplayDigitCount; ++DI)
	{
		// 捞锅 磊复荐.. ExtractNthDigitR 绰 袍敲复 惑荐 牢磊甫 罐酒具 秦辑.. Dynamic 滚傈栏肺
		const int32 ThisDigitNumber = ExtractNthDigitR_Dynamic(FinalNumber, DI + 1);
		UImage* ThisDigitImage = AllDigits.IsValidIndex(DI) ? AllDigits[DI] : NULL;
		if (ThisDigitImage) // 拱沸 汲沥苞 硅摹啊 力措肺 登菌促搁 捞霸 酒囱 版快绰 绝绢具..
		{
			ThisDigitImage->SetVisibility(ESlateVisibility::HitTestInvisible);

			// Image draw batching 荤侩阑 困秦 Material 捞 酒囱 PaperSprite 甫 荤侩茄促.
			UPaperSprite* DigitSprite = GetSpriteForDigit(ThisDigitNumber);
			if (DigitSprite)
			{
				SetImageBrushFromPaperSprite(ThisDigitImage, DigitSprite);
				// 捞 版快绰 UImage 俊辑 GetDynamicMaterial 肺 啊廉客档 家侩捞 绝澜.
				// 措脚 咯扁辑绰 UImage 磊眉俊 乐绰 扁瓷阑 荤侩秦辑 拿矾 喉坊爹鳖瘤绰 贸府.
				SetBlendColorForPaperSpriteBoundImage(ThisDigitImage, InBlendColor);
			}
		}
	}

	// 钎矫 磊复荐俊 蝶扼 荤捞令尔 困摹 炼例秦辑 啊款单 沥纺矫糯. WrapBox alignment 肺 窍妨聪 购啊 救冈囚辑..
	UCanvasPanelSlot* BasePanelSlot = WB_AllDisplaySet.IsValid() ? Cast<UCanvasPanelSlot>(WB_AllDisplaySet->Slot) : NULL;
	if (BasePanelSlot)
	{
		float DesiredWholeWidth = (float)DisplayDigitCount * SingleDigitWidth;
		BasePanelSlot->SetSize(FVector2D(DesiredWholeWidth, BasePanelSlot->GetSize().Y));
		// 吝居 巨目肺 啊沥.
		BasePanelSlot->SetPosition(FVector2D(DesiredWholeWidth * -0.5f, BasePanelSlot->GetPosition().Y));
	}
}

void UB2FloatingDamageNum::SetupForCriticalDamage(bool bIsCritical)
{
	// 酒流篮 咯扁俊 农府萍拿 单固瘤 沥焊甫 持菌促 沥档..

	if (PN_CriticalDamage.IsValid())
	{
		PN_CriticalDamage->SetVisibility(bIsCritical ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (IMG_CriticalDamageText.IsValid())
	{ // 捞扒 鞘夸绝阑 荐 乐澜. 扁夯利栏肺绰 箭磊俊 去钦窍妨绰 祸惑烙.
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