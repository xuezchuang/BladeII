// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_ItemCusProgBase.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "Event.h"
#include "B2DynItemIcon_ItemOpScene.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "B2LobbyUI_ItemDetailPostOp.h"

float UB2LobbyUI_ItemCusProgBase::GetOverallPlayRateForStar(const FB2Item& InItem)
{
	if (auto* StarEffectInfo = GetStarEffectInfo(InItem))
	{
		return StarEffectInfo->OverallPlayRate;
	}

	return 0.0f;
}

float UB2LobbyUI_ItemCusProgBase::GetCreatingDepthForStar(const FB2Item& InItem)
{
	if (auto* StarEffectInfo = GetStarEffectInfo(InItem))
	{
		return StarEffectInfo->FxCreatingDepth;
	}

	return 0.0f;
}

float UB2LobbyUI_ItemCusProgBase::GetResultListingOffsetForStar(const FB2Item& InItem)
{
	if (auto* StarEffectInfo = GetStarEffectInfo(InItem))
	{
		return StarEffectInfo->ResultGradeStarListingOffset;
	}

	return 0.0f;
}

FVector UB2LobbyUI_ItemCusProgBase::GetStarFxScaleForStar(const FB2Item& InItem)
{
	if (auto* StarEffectInfo = GetStarEffectInfo(InItem))
	{
		return StarEffectInfo->ResultGradeStarFxScale;
	}

	return FVector(0.0f, 0.0f, 0.0f);
}

FVector2D UB2LobbyUI_ItemCusProgBase::GetResultStarCenterPosForStar(const FB2Item& InItem)
{
	if (auto* StarEffectInfo = GetStarEffectInfo(InItem))
	{
		return StarEffectInfo->ResultGradeStarCenterPos;
	}

	return FVector2D();
}

UParticleSystem* UB2LobbyUI_ItemCusProgBase::GetResultGradeStarPS(const FB2Item& InItem)
{
	if (auto* StarEffectInfo = GetStarEffectInfo(InItem))
	{
		return StarEffectInfo->ResultGradeStarPS;
	}

	return nullptr;
}

FStarEffectInfo* UB2LobbyUI_ItemCusProgBase::GetStarEffectInfo(const FB2Item& InItem)
{
	return GetStarEffectInfo(InItem.StarGrade);
}

FStarEffectInfo* UB2LobbyUI_ItemCusProgBase::GetStarEffectInfo(const int32 InGrade)
{
	if (InGrade > 0 && InGrade <= FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE
		&& StarsInfo.IsValidIndex(0))
	{
		return &StarsInfo[0];
	}
	else if (InGrade > FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE && InGrade <= FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE)
	{
		// 7己捞搁 1 8己捞搁2 9己捞搁 3...
		int32 StarContainerIndex = InGrade - FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE;
		if (StarsInfo.IsValidIndex(StarContainerIndex))
		{
			return &StarsInfo[StarContainerIndex];
		}
	}

	return nullptr;
}

UB2LobbyUI_ItemCusProgBase::UB2LobbyUI_ItemCusProgBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedInventory = NULL;
	
	ItemIconWidgetClass = NULL;
	CreatedItemIcon_Target = NULL;
	CreatedItemIcon_Result = NULL;

	bUseCameraAdaptiveFOV = false;
	AdaptiveFOVScale = 0.6f;
}

void UB2LobbyUI_ItemCusProgBase::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, X_CP_ItemIconSet_Target);
	GET_SLOT(UCanvasPanel, X_CP_ItemIconSet_Result);
	GET_SLOT(UB2LobbyUI_ItemDetailPostOp, UIP_FinalResultPageP)
}

void UB2LobbyUI_ItemCusProgBase::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	TargetOpMode = CachedInventory->GetItemOpMode();

	// 诀单捞飘 登扁 傈 鸥百 酒捞袍 单捞磐啊 鞘夸 (UB2LobbyInventory::OnReqForItemOpCommon 俊辑 归诀) 
	if (!CachedInventory->GetItemOPTargetItemData(NativeItemData_Target, true) || // Result 单捞磐绰 咯扁辑 啊廉坷瘤 臼绰促. 酒付 response 啊 柯 惑炔老 芭瘤父 趣 葛福聪..
		!IsSupportedOpMode(TargetOpMode))
	{
		FinishAndProceedToResult(); // 父俊 窍唱 角菩茄促搁.. 绢驴 荐 绝瘤.
		return;
	}

	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, GetOverallPlayRateForStar(NativeItemData_Target));

	ApplyCameraAdaptiveFOV();

	SetItemData_Target(NativeItemData_Target);

	// 诀单捞飘 等 搬苞 酒捞袍 单捞磐甫 啊廉坷扁 困窃. 酒流 酒捞能 技泼篮 窍瘤 臼绰促.
	// 酒流 response 啊 坷瘤 臼篮 鸥捞怪捞扼搁 鞘夸且 锭 促矫 啊廉棵 巴.
	if (!CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false)){
		// 绢恫 捞蜡肺电 搬苞甫 啊廉坷瘤 给茄促搁 request 捞傈狼 某教等 鸥百 单捞磐甫 持绰促.
		NativeItemData_Result = NativeItemData_Target;
	}

	InitFinalResultPage(InUIManager, InGM);

	UpdateStaticWidgets();
}

void UB2LobbyUI_ItemCusProgBase::DestroySelf()
{
	Super::DestroySelf();

	if (UIP_FinalResultPageP.IsValid())
	{
		UIP_FinalResultPageP->DestroySelf();
	}

	DestroyItemIcon_Target();
	DestroyItemIcon_Result();

	DestroyAllFx();
}

void UB2LobbyUI_ItemCusProgBase::UpdateStaticWidgets()
{
}

void UB2LobbyUI_ItemCusProgBase::StartResultGradeStarFx()
{
	DestroyResultGradeStarFx(); // 咯扁辑 DestroyAllFx 窍搁 救登瘤氛.

	if (!GetResultGradeStarPS(NativeItemData_Result)){
		return;
	}
	
	check(CachedInventory);
	CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false); // 被捞 鞘夸且鳖 酵变 窍瘤父.
	
	auto* StarEffectInfo = GetStarEffectInfo(NativeItemData_Result);
	auto* OwningPC = GetOwningPlayer();

	FVector FxCreateRefPos(0.0f,0.0f,0.0f); // 盖 贸澜 喊阑 积己窍绰 俩 谅螟 谅钎.
	FVector FxCreatePosOffsetDir(1.0f, 0.0f, 0.0f); // 岿靛 傍埃俊辑狼 拳搁 啊肺 规氢. 喊甸阑 积己且 锭 捞 规氢栏肺 可悸阑 临 巴.

	if (OwningPC && StarEffectInfo)
	{
		StarEffectInfo->ResultGradeStarCenterPos.X = FMath::Clamp(StarEffectInfo->ResultGradeStarCenterPos.X, 0.0f, 0.9f); // 捞芭焊促 距埃 坷弗率俊辑档 困摹甫 备且 芭扼 X 谅钎 力茄捞 粱 乐澜. 措眉肺 拳搁 吝居捞摆瘤父.

		// ResultGradeStarCenterPos 绰 0 ~ 1 荤捞 normalized 等 蔼栏肺 窍绊 胶农赴 荤捞令肺 胶纳老 秦辑 秦寸 胶农赴 困摹 酒贰 岿靛 谅钎甫 备茄促.

		int32 CurrViewSizeX, CurrViewSizeY;
		OwningPC->GetViewportSize(CurrViewSizeX, CurrViewSizeY);

		// Y 困摹 急沥俊 饭欺繁胶 秦惑档 厚啦 措厚 泅犁 秦惑档 厚啦阑 绊妨且 鞘夸..
		int32 RefViewSizeX = (int32)UB2UnitedWidgetBase::DesignedRefViewSize.X;
		int32 RefViewSizeY = (int32)UB2UnitedWidgetBase::DesignedRefViewSize.Y;
		// 扁霖 秦惑档 厚啦(酒付档 16:9) 措厚 倔付唱 辆染厚啊 奴瘤..
		const float RelativeAspectRatio =
			(CurrViewSizeX > 0 && CurrViewSizeY > 0 && RefViewSizeX > 0 && RefViewSizeY > 0) ?
			(((float)CurrViewSizeX / (float)CurrViewSizeY) / ((float)RefViewSizeX / (float)RefViewSizeY)) : 1.0f;

		float FinalGradeStarCenterPosX = StarEffectInfo->ResultGradeStarCenterPos.X;
		// 拳搁 吝居 扁霖栏肺 函券 -> 饭欺繁胶 秦惑档客 泅犁 秦惑档 厚啦父怒 炼沥 -> 促矫 拳搁 惑窜 扁霖栏肺
		float FinalGradeStarCenterPosY = ( (StarEffectInfo->ResultGradeStarCenterPos.Y - 0.5f) * RelativeAspectRatio ) + 0.5f;

		// 捞 矫痢俊辑绰 喊 6俺 父顶 谩阑 锭狼 吝缴 谅钎捞促.
		FxCreateRefPos = GetWorldPosBeneathScreen(OwningPC,
			FVector2D((float)CurrViewSizeX * FinalGradeStarCenterPosX, (float)CurrViewSizeY * FinalGradeStarCenterPosY),
			StarEffectInfo->FxCreatingDepth, true);

		// 距埃 坷弗率俊 积己秦辑 胶农赴 X 绵狼 岿靛 规氢阑 舅酒郴绰单俊 荤侩.
		FVector FxCreateOffsetPos = GetWorldPosBeneathScreen(OwningPC,
			FVector2D((float)CurrViewSizeX * FMath::Min(1.0f, FinalGradeStarCenterPosX + 0.1f), (float)CurrViewSizeY * FinalGradeStarCenterPosY), StarEffectInfo->FxCreatingDepth, true);

		FxCreatePosOffsetDir = (FxCreateOffsetPos - FxCreateRefPos);
		FxCreatePosOffsetDir.Normalize();
	}

	if (NativeItemData_Result.StarGrade > FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE)
	{
		// 俩 谅螟狼 霉 喊阑 积己且 困摹 登摆促.
		FxCreateRefPos = FxCreateRefPos - (FxCreatePosOffsetDir * StarEffectInfo->ResultGradeStarListingOffset * 0.5f);
		FxCreateRefPos.Y += 1.0f; // 困摹 焊沥蔼

		FTransform FXCreateTransform(FxCreateRefPos + FxCreatePosOffsetDir);
		UParticleSystemComponent* ThisFxComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetResultGradeStarPS(NativeItemData_Result), FXCreateTransform);
		if (ThisFxComp)
		{
			ThisFxComp->SetWorldScale3D(GetStarFxScaleForStar(NativeItemData_Result));
			CreatedResultGradeStarFx.Add(ThisFxComp);
		}
	}
	else
	{
		// 俩 谅螟狼 霉 喊阑 积己且 困摹 登摆促.
		FxCreateRefPos = FxCreateRefPos - (FxCreatePosOffsetDir * StarEffectInfo->ResultGradeStarListingOffset * (float)(/*FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE*/ NativeItemData_Result.StarGrade - 1) * 0.5f);

		// 喊殿鞭 俺荐父怒 谅螟何磐 秦辑 积己.
		for (int32 SI = 0; SI < NativeItemData_Result.StarGrade; ++SI)
		{
			FTransform FXCreateTransform(FxCreateRefPos + (FxCreatePosOffsetDir * GetResultListingOffsetForStar(NativeItemData_Result) * (float)SI));
			UParticleSystemComponent* ThisFxComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetResultGradeStarPS(NativeItemData_Result), FXCreateTransform);
			if (ThisFxComp)
			{
				ThisFxComp->SetWorldScale3D(GetStarFxScaleForStar(NativeItemData_Result));
				CreatedResultGradeStarFx.Add(ThisFxComp);
			}
		}
	}
}

void UB2LobbyUI_ItemCusProgBase::StartShowResultItemIcon()
{
	if (!CreatedItemIcon_Result)
	{
		CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false);
		SetItemData_Result(NativeItemData_Result);
	}
	if (CreatedItemIcon_Result)
	{
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Visible);
		CreatedItemIcon_Result->SetShowOverlayInfoOnly(true, true); // 窍瘤父 角力肺绰 捞抚苞 饭骇 沥档父 UI 窜俊辑 焊捞霸 窍绰 吧肺. 唱赣瘤绰 Fx 3D 皋浆肺.
		CreatedItemIcon_Result->ForceLayoutPrepass();
	}
}

void UB2LobbyUI_ItemCusProgBase::ShowFinalResultPage()
{
	if (UIP_FinalResultPageP.IsValid())
	{
		UIP_FinalResultPageP->SetVisibility(ESlateVisibility::Visible);
	}

	// 捞霸 弥辆 搬苞 拳搁俊档 荤侩登绰 版快啊 乐绰单 弊凡 订 缔何盒 3D 捞棋飘 绝捞 焊咯廉具 秦辑 Overlay 父 焊捞霸 茄 芭 秒家窃.
	if (CreatedItemIcon_Result)
	{
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Visible);
		CreatedItemIcon_Result->SetShowOverlayInfoOnly(true, true);
	}
}

void UB2LobbyUI_ItemCusProgBase::InitFinalResultPage(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	if (UIP_FinalResultPageP.IsValid()) // 捞扒 悼利 积己篮 酒聪促. 窜, 割啊瘤绰 流立 秦 林绢具.
	{
		UIP_FinalResultPageP->StartFromLobby(InUIManager, InGM);
		UIP_FinalResultPageP->UpdateItemData(NativeItemData_Target, NativeItemData_Result);	// 贸澜俊绰 府例飘啊 绝澜.
		UIP_FinalResultPageP->ClosePopup(); // 贸澜俊绰 见败初澜. 楷免 弥辆 窜拌肺辑 焊咯柳促.
	}
}

void UB2LobbyUI_ItemCusProgBase::ApplyCameraAdaptiveFOV()
{
	checkSlow(CachedLobbyGM && CachedLobbyGM->IsInItemOpDirectingView());

	APlayerController* OwningPC = GetOwningPlayer();
	// 捞 厘搁 悸诀俊辑 傈侩 墨皋扼 咀磐啊 ViewTarget 栏肺 技泼登绢 乐绢具 窃.
	ACameraActor* ViewTargetCam = OwningPC ? Cast<ACameraActor>(OwningPC->GetViewTarget()) : nullptr;
	checkSlow(ViewTargetCam);
	if (ViewTargetCam && ViewTargetCam->GetCameraComponent())
	{ // AspectRatioAdaptiveFOV 甫 荤侩窍搁 扁霖 秦惑档 厚啦焊促 奴 拳搁 厚啦俊辑 厘搁捞 肋府绰 吧 乔且 荐 乐瘤父
	  // 措脚 承绢柳 矫具肺 牢秦 皑眠菌带 霸 畴免登绰 老捞 惯积且 荐 乐促. 惑炔俊 蝶扼 利例窍霸..
		// 肚茄 2D-3D 夸家埃 沥纺捞 绢边唱霸 瞪 荐档 乐促. 捞扒 漂洒 促剧茄 秦惑档 厚啦俊辑 抛胶飘 鞘夸.
		//ViewTargetCam->GetCameraComponent()->SetAspectRatioAdaptiveFOV(bUseCameraAdaptiveFOV);
		//ViewTargetCam->GetCameraComponent()->SetAdaptiveFOVScale(AdaptiveFOVScale);
	}
}

FVector UB2LobbyUI_ItemCusProgBase::GetDesiredScreenCenterWorldPos()
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		int32 ViewSizeX, ViewSizeY;
		OwningPC->GetViewportSize(ViewSizeX, ViewSizeY);
		return GetWorldPosBeneathScreen(OwningPC, FVector2D((float)(ViewSizeX / 2), (float)(ViewSizeY / 2)), GetCreatingDepthForStar(NativeItemData_Result)); // NativeItemData_Result,NativeItemData_Target
	}
	return FVector(0.0f, 0.0f, 0.0f);
}

void UB2LobbyUI_ItemCusProgBase::SetItemData_Target(FB2Item& InItem)
{
	DestroyItemIcon_Target();

	NativeItemData_Target = InItem;
	CreatedItemIcon_Target = CreateItemIconCommon(X_CP_ItemIconSet_Target.Get(), NativeItemData_Target, ELobbyInvenItemIconUsage::EEIIT_ItemOpModeProgTarget);
}

void UB2LobbyUI_ItemCusProgBase::SetItemData_Result(FB2Item& InItem)
{
	DestroyItemIcon_Result();

	NativeItemData_Result = InItem;
	CreatedItemIcon_Result = CreateItemIconCommon(X_CP_ItemIconSet_Result.Get(), NativeItemData_Result, ELobbyInvenItemIconUsage::EEIIT_ItemOpModeProgTarget);

	if (UIP_FinalResultPageP.IsValid())
		UIP_FinalResultPageP->UpdateItemData(NativeItemData_Target, NativeItemData_Result);
}

UB2DynItemIcon_ItemOpScene* UB2LobbyUI_ItemCusProgBase::CreateItemIconCommon(UCanvasPanel* ParentCP, const FB2Item& InItemData, ELobbyInvenItemIconUsage IconUsage)
{
	if (!ItemIconWidgetClass || !ParentCP || InItemData.InstanceUID <= 0 || InItemData.ItemRefID <= 0){
		return NULL;
	}

	// Anchor 汲沥俊 蝶扼 GetSize 绰 盔窍绰 蔼捞 救 唱棵 巴捞骨肺 林狼.
	UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(ParentCP->Slot);
	FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);
	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());
	UB2DynItemIcon_ItemOpScene* CreatedItemIcon = Cast<UB2DynItemIcon_ItemOpScene>(DynCreateInCanvasPanel(
		ItemIconWidgetClass, this, ParentCP, DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
		FVector2D(0.0f, 0.0f), 0, false
	));

	if (CreatedItemIcon)
	{
		CreatedItemIcon->SetIconUsageType(IconUsage);
		CreatedItemIcon->UpdateItemData(InItemData);
		return CreatedItemIcon;
	}
	return NULL;
}

void UB2LobbyUI_ItemCusProgBase::DestroyItemIcon_Target()
{
	if (CreatedItemIcon_Target){
		CreatedItemIcon_Target->ConditionalBeginDestroy();
		CreatedItemIcon_Target = NULL;
	}
}

void UB2LobbyUI_ItemCusProgBase::DestroyItemIcon_Result()
{
	if (CreatedItemIcon_Result){
		CreatedItemIcon_Result->ConditionalBeginDestroy();
		CreatedItemIcon_Result = NULL;
	}
}

void UB2LobbyUI_ItemCusProgBase::DestroyAllFx()
{	
	DestroyResultGradeStarFx();
}

void UB2LobbyUI_ItemCusProgBase::DestroyResultGradeStarFx()
{
	for (UParticleSystemComponent* CurrComp : CreatedResultGradeStarFx)
	{
		if (CurrComp){
			CurrComp->ConditionalBeginDestroy();
		}
	}
	CreatedResultGradeStarFx.Empty();
}

void UB2LobbyUI_ItemCusProgBase::FinishAndProceedToResult()
{
	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, 1.0f); // OverallPlayRate 俊 蝶扼 函版沁带 芭 汗备.
	QuitItemOpModeClass<bool>::GetInstance().Signal(true); // 角力肺绰 搬苞芒捞 唱客具 窃. 芭扁辑 Quit 阑 窍霸 瞪 巴.
}

#if WITH_EDITOR
void UB2LobbyUI_ItemCusProgBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_ResultGradeStarCenterPos = GET_MEMBER_NAME_CHECKED(UB2LobbyUI_ItemCusProgBase, GetResultStarCenterPosForStar(NativeItemData_Result));
	
	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	//if (PropertyName == Name_ResultGradeStarCenterPos) // 捞 捞抚栏肺 救 甸绢坷绊 X, Y 肺 甸绢咳 せせ 傲 攫力唱 眉农
	{ // StartResultGradeStarFx 俊辑 ResultGradeStarCenterPos 距埃 坷弗率 困摹甫 荤侩窍绰 霸 乐绢辑 X 谅钎绰 1.0 捞 酒囱 0.9鳖瘤父 倾侩.

		auto* StarEffectInfo = GetStarEffectInfo(NativeItemData_Result);

		StarEffectInfo->ResultGradeStarCenterPos.X = FMath::Clamp(StarEffectInfo->ResultGradeStarCenterPos.X, 0.0f, 0.9f);
		StarEffectInfo->ResultGradeStarCenterPos.Y = FMath::Clamp(StarEffectInfo->ResultGradeStarCenterPos.Y, 0.0f, 1.0f);
	}
}
#endif