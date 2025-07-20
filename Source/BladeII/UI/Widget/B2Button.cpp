// Fill out your copyright notice in the Description page of Project Settings.


#include "B2Button.h"
//#include "B2UnitedWidgetBase.h"
#include "BladeIIGameMode.h"
#include "Event.h"
#include "B2UnitedWidgetBase.h"
#include "Materials/MaterialInstanceDynamic.h"
//#include "B2CommonSoundInfo.h"
//#include "TutorialManager.h"
//#include "B2GameInstance.h"

#define LOCTEXT_NAMESPACE "UMG"
#define ON_TOUCH_IMPULSE_RING_MAT_ANIM_PARAM_NAME TEXT("RingScale")

//================================================================================

bool FImpluseRingInstInfo::Init(class UB2UnitedWidgetBase* InOwnerUnitedWidget, const FName& InImageWidgetName, float InWholeAnimTime, float InBeginMIDParam, float InEndMIDParam)
{
	if (InOwnerUnitedWidget && InImageWidgetName != NAME_None)
	{
		return Init(InOwnerUnitedWidget, InOwnerUnitedWidget->GetSlot<UImage>(InImageWidgetName), InWholeAnimTime, InBeginMIDParam, InEndMIDParam);
	}
	return false;
}
bool FImpluseRingInstInfo::Init(class UB2UnitedWidgetBase* InOwnerUnitedWidget, UImage* InImageWidgetPtr, float InWholeAnimTime, float InBeginMIDParam, float InEndMIDParam)
{
	if (InOwnerUnitedWidget && InImageWidgetPtr)
	{
		ImpulseRingImage = InImageWidgetPtr;

		if (ImpulseRingImage.IsValid())
		{
			WholeAnimTime = InWholeAnimTime; // 颇扼固磐 傈崔.
			MIDParamRange_Begin = InBeginMIDParam;
			MIDParamRange_End = InEndMIDParam;

			ImpulseRingImage->SetVisibility(ESlateVisibility::Hidden); // 老窜 见败具摆瘤
			bIsCurrentOn = false;
			ImpulseRingMID = NULL;
			return true;
		}
	}

	return false;
}
void FImpluseRingInstInfo::Clear()
{
	Stop(); // 歹 鞘夸茄啊?
}

void FImpluseRingInstInfo::Play()
{
	if (ImpulseRingImage.IsValid())
	{
		ImpulseRingImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		ImpulseRingMID = ImpulseRingImage->GetDynamicMaterial();
		if (ImpulseRingMID)
		{ // Material Anim progress 府悸
			CurrentAnimTime = 0.0f;
			ApplyMIDParam();
			bIsCurrentOn = true;
		}
	}
}
void FImpluseRingInstInfo::Stop()
{
	if (ImpulseRingImage.IsValid())
	{
		ImpulseRingImage->SetVisibility(ESlateVisibility::Hidden);
	}
	ImpulseRingMID = NULL;
	bIsCurrentOn = false;
}

bool FImpluseRingInstInfo::Tick(float DeltaSecond, bool bAutoRepeatOnEnd)
{
	bool bRetVal = false;
	if (bIsCurrentOn)
	{
		// 0 ~ WholeAnimTime 荤捞肺 颇扼固磐 局聪皋捞记 矫糯
		CurrentAnimTime += DeltaSecond;
		if (CurrentAnimTime > WholeAnimTime)
		{
			CurrentAnimTime = 0.0f;
			bRetVal = true; // 促矫 贸澜何磐 敲饭捞 窍带瘤, stop 窍带瘤, 观俊辑 舅酒辑 窍档废
			if (bAutoRepeatOnEnd) // 弊矾唱 bAutoRepeatOnEnd 牢 版快绰 观俊辑 歹 脚版镜 鞘夸 绝摆促.
			{
				Play();
			}
		}
		ApplyMIDParam();
	}
	return bRetVal;
}

void FImpluseRingInstInfo::ApplyMIDParam()
{
	if (ImpulseRingMID)
	{
		// CurrentAnimTime 捞 WholeAnimTime 鳖瘤父 刘啊窍绰单 弥辆利牢 颇扼固磐 利侩篮 MIDParamRange_Begin ~ MIDParamRange_End 荤捞肺.
		const float ParamRange = MIDParamRange_End - MIDParamRange_Begin;
		const float AnimScale = (WholeAnimTime > KINDA_SMALL_NUMBER) ? (ParamRange / WholeAnimTime) : 1.0f;
		const float FinalParameter = MIDParamRange_Begin + (AnimScale * CurrentAnimTime);

		ImpulseRingMID->SetScalarParameterValue(FName(ON_TOUCH_IMPULSE_RING_MAT_ANIM_PARAM_NAME), FinalParameter);
	}
}

void FImpluseRingInstInfo::SetImageCenterPosition(const FVector2D& InPosUpperLeftOrigin)
{ // InPosUpperLeftOrigin 篮 拳搁 谅惑窜 扁霖狼 UMG 饭欺繁胶 秦惑档 扁霖 谅钎肺 模促. UB2UnitedWidgetBase::HACKGetWidgetScreenCoord 曼绊
	//if (ImpulseRingImage.IsValid())
	//{
	//	UCanvasPanelSlot* CPSlot = Cast<UCanvasPanelSlot>(ImpulseRingImage->Slot); // 傈眉 康开 农扁狼 CanvasPanel 俊 硅摹茄 版快父 冈鳃 巴烙.
	//	if (CPSlot)
	//	{
	//		const FAnchors& SlotAnchor = CPSlot->LayoutData.Anchors;
	//		if (!SlotAnchor.IsStretchedVertical() && !SlotAnchor.IsStretchedHorizontal())
	//		{
	//			const FVector2D RefScaledAnchor = SlotAnchor.Minimum * UB2UnitedWidgetBase::DesignedRefViewSize;
	//			FVector2D FinalAdjusted = InPosUpperLeftOrigin;
	//			FinalAdjusted.X += RefScaledAnchor.X;
	//			FinalAdjusted.Y += RefScaledAnchor.Y;

	//			const FVector2D& ImageSize = CPSlot->GetSize();
	//			// 甸绢柯 谅钎甫 捞固瘤 吝缴 谅钎肺 
	//			FinalAdjusted.X -= (ImageSize.X * 0.5f);
	//			FinalAdjusted.Y -= (ImageSize.Y * 0.5f);

	//			CPSlot->SetPosition(FinalAdjusted);
	//		}
	//	}
	//}
}

//================================================================================

UB2Button::UB2Button(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//bIsEnabledByNormalUsage = bIsEnabled;
	//bIsPressedForImpulseRing = false;

	//CachedOwnerUnitedWidget = NULL;

	//DefaultOnClickSoundIndex = 0;
	//bPlayDefaultOnClickSoundOnPress = false;

	//bReleaseOnFocusLost = true;

	///*
	// Click/TouchMethod 汲沥捞 MouseDown/PreciseTap 老 锭 滚瓢 Click 矫弊澄篮 Press 矫痢俊 惯积.
	// Pressed 客 Click 官牢爹捞 荤角惑 付蛮啊瘤啊 等促.
	// 捞绰 滚瓢 磐摹 馆览捞 矫盔矫盔窍绊 弧扼具 茄促绰 夸备荤亲俊 狼茄 巴牢单 (眠啊肺 厚搅茄 公菲俊 滚瓢 磐摹 康开 犬厘档 备泅凳)
	// 酒付 寸矫鳖瘤狼 备泅俊辑 喊 积阿绝捞 Click 俊 官牢爹茄 绢瘤埃茄 滚瓢甸 葛滴 磐摹 溜矫 馆览阑 盔沁带 巴 鞍促.
	// 滚瓢 侩档俊 蝶扼 捞巴捞 何累侩阑 老栏懦 荐 乐绰单 父老 捞 扁夯蔼阑 浚柳 扁夯牢 DownAndUp 栏肺 登倒副 矫 
	// 扁粮俊 Click 俊 官牢爹茄 滚瓢甸 吝 溜阿利牢 馆览捞 鞘夸茄 滚瓢甸篮 流立 汲沥阑 函版窍芭唱 Pressed 俊 官牢爹窍芭唱..
	// 捞俊 蝶扼 霸烙 皋春 炼累狼 傈馆利牢 磐摹皑捞 崔扼龙 荐 乐促.
	//*/
	//ClickMethod = EButtonClickMethod::MouseDown;
	//TouchMethod = EButtonTouchMethod::PreciseTap;

	//OnTouchImpulseRingAnimTime = 0.2f;
	//OnTouchImpulseRingParamRange_Begin = 0.5f;
	//OnTouchImpulseRingParamRange_End = 1.5f;
	//bLoopOnTouchImpulseRing = false;

	//StunDuration = 0.0f;
	//LastStunnedTime = 0.0f;
	//bIsStunned = false;
}

void UB2Button::SetIsEnabled(bool bInIsEnabled)
{
	bIsEnabledByNormalUsage = bInIsEnabled; // 促弗 扁瓷俊 狼秦 Enable/Disable 且 锭俊 盔贰 惑怕蔼阑 犬牢窍绰 单俊 镜 巴.
	Super::SetIsEnabled(bInIsEnabled);
}

//////////////////////////////////////////////////
// 漂沥茄 荤侩 寇俊 老馆利牢 侩档肺 荤侩窍瘤 富 巴.
void UB2Button::StunForDuration()
{ // bIsEnabledByNormalUsage 篮 蜡瘤茄 盲 惑窜 SetIsEnabled 甫 龋免.
	bIsStunned = true;
	LastStunnedTime = static_cast<float>(FPlatformTime::Seconds());
	Super::SetIsEnabled(false);
}
bool UB2Button::TryRestoreFromStun()
{
	if (bIsStunned)
	{
		// 矫埃 眉农..  StunDuration 捞 0 捞窍搁 官肺 汗备
		const float CurrTime = static_cast<float>(FPlatformTime::Seconds());
		if (ShouldStunSingleTickOnly() || (CurrTime - LastStunnedTime >= StunDuration))
		{
			Super::SetIsEnabled(bIsEnabledByNormalUsage);
			bIsStunned = false; // 汲飞 咯傈洒 disable 捞歹扼档 stun 篮 钱赴 巴.
			return true; // 角力 stun 捞菌促啊 钱赴 版快父
		}		
	}
	return false;
}
//////////////////////////////////////////////////

bool UB2Button::IsTickRequired() const
{ // Owning user widget 积己 矫 阂府绰 巴烙. 荤傈俊 汲沥 啊瓷茄 蔼栏肺父 魄沥秦具 窃.
	return (OnTouchImpulseRingWidgetName != NAME_None); // Name 汲沥捞 登绢 乐栏搁 OnTouchImpulseRingImage 甫 掘绢棵 啊瓷己捞 乐栏骨肺 tick 阑 捣促.
}

void UB2Button::ManualTick(float DeltaSecond)
{
	if (OnTouchImpulseRingInst.IsCurrentOn())
	{
		if (OnTouchImpulseRingInst.Tick(DeltaSecond))
		{
			OnOnTouchImpulseRingAnimTimeCycle(); // 茄 荤捞努 促 倒搁 促矫 贸澜何磐 敲饭捞 窍带瘤, stop 登带瘤.
		}
	}
}

void UB2Button::RegisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget)
{
	//CachedOwnerUnitedWidget = InOwnerUnitedWidget;
	//bIsEnabledByNormalUsage = bIsEnabled; // 捞 矫痢俊辑绰 捞霸 鞍酒具 茄促. 捞霸 崔扼瘤绰 版快绰 CachedOwnerUnitedWidget 阑 烹秦 悼累窍骨肺

	//OnClicked.AddUniqueDynamic(this, &UB2Button::ExtraOnClick);
	//OnPressed.AddUniqueDynamic(this, &UB2Button::ExtraOnPress);
	//OnReleased.AddUniqueDynamic(this, &UB2Button::ExtraOnRelease);
	//OnHovered.AddUniqueDynamic(this, &UB2Button::ExtraOnHovered);

	//OnTouchImpulseRingInst.Init(InOwnerUnitedWidget, OnTouchImpulseRingWidgetName, OnTouchImpulseRingAnimTime, OnTouchImpulseRingParamRange_Begin, OnTouchImpulseRingParamRange_End);
}

void UB2Button::UnregisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget)
{
	OnClicked.RemoveDynamic(this, &UB2Button::ExtraOnClick);
	OnPressed.RemoveDynamic(this, &UB2Button::ExtraOnPress);
	OnReleased.RemoveDynamic(this, &UB2Button::ExtraOnRelease);
	OnHovered.RemoveDynamic(this, &UB2Button::ExtraOnHovered);
	OnTouchImpulseRingInst.Clear();
}

void UB2Button::ExtraOnClick()
{
	if (!bPlayDefaultOnClickSoundOnPress){
		PlayOnClickDefaultSound();
	}

	TutorialButtonSignal();
}

void UB2Button::ExtraOnPress()
{
	//if (CachedOwnerUnitedWidget) {
	//	// 酒公唱 喘啡栏搁 官肺 促弗 滚瓢甸 stun 阑.. 汲沥俊 蝶扼 茄 平 趣篮 老沥 矫埃 蜡瘤. 磊脚狼 StunDuration 捞 0 焊促 农搁 磊脚档 stun
	//	// 辟单 捞巴 磊眉啊 茄 平俊 悼矫俊 甸绢哎 荐档..?
	//	CachedOwnerUnitedWidget->SetButtonsStunForDuration(this);
	//}

	//bIsPressedForImpulseRing = true;
	//
	//if (!OnTouchImpulseRingInst.IsCurrentOn()){
	//	PlayOnTouchImpulseRing(); // 狐弗 磐摹 矫 概锅 货肺 敲饭捞 登瘤 臼档废 难廉 乐绰 悼救俊绰 鸥捞赣俊 狼秦辑父 促矫 敲饭捞.
	//}

	//if (bPlayDefaultOnClickSoundOnPress){
	//	PlayOnClickDefaultSound();
	//}
}

void UB2Button::ExtraOnRelease()
{
	bIsPressedForImpulseRing = false;

	// OnTouchImpulseRing 阑 咯扁辑 迭洒 stop 矫虐瘤绰 臼绰促.
}

void UB2Button::ExtraOnHovered()
{
	//[@AKi, 170329] 漂喊洒 荤侩篮 救窍唱 趣矫 隔扼辑 父甸绢 初澜.
}

void UB2Button::PlayOnClickDefaultSound()
{
	//if (DefaultOnClickSoundIndex >= 0)
	//{
	//	UWorld* TheWorld = GetWorld();
	//	if (TheWorld)
	//	{
	//		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(TheWorld));
	//		UB2CommonSoundInfo* CSI = B2GI ? B2GI->GetCommonSoundInfo() : NULL;
	//		USoundCue* WantedSoundCue = CSI ? CSI->GetButtonClickDefSound(DefaultOnClickSoundIndex) : NULL;
	//		if (WantedSoundCue)
	//		{
	//			UGameplayStatics::PlaySound2D(TheWorld, WantedSoundCue);
	//		}
	//	}
	//}
}

void UB2Button::PlayOnTouchImpulseRing()
{
	OnTouchImpulseRingInst.Play();
}

void UB2Button::StopOnTouchImpulseRing()
{
	OnTouchImpulseRingInst.Stop();
}

void UB2Button::OnOnTouchImpulseRingAnimTimeCycle()
{
	if (bIsPressedForImpulseRing && bLoopOnTouchImpulseRing){
		PlayOnTouchImpulseRing(); // 促矫 贸澜何磐 矫累
	}
	else{
		StopOnTouchImpulseRing(); // 酒聪搁 捞力 弊父
	}
}

void UB2Button::CustomStopOnTouchImpulseRing()
{
	bIsPressedForImpulseRing = false; // 敲饭捞 登绊 乐绰 惑炔捞扼搁 OnOnTouchImpulseRingAnimTimeCycle 俊辑 侨诀且 巴.
}

void UB2Button::TutorialButtonSignal()
{
	/*if (TutorialManager::GetInstance().HasAnyTutorial())
	{
		NotifyUIButtonClickedClass<UButton*>::GetInstance().Signal(this);
	}*/
}

void UB2Button::OnChangedClickMethod()
{	
	//if (MyButton.IsValid())
	//{
	//	MyButton->ChangeClickMethod_Dynamic(ClickMethod);
	//	MyButton->ChangeTouchMethod_Dynamic(TouchMethod);
	//}
}

#if WITH_EDITOR
const FText UB2Button::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common"); // Original UBtton palette category
}
#endif

#undef LOCTEXT_NAMESPACE