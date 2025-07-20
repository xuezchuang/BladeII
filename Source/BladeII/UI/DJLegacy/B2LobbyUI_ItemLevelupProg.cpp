// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_ItemLevelupProg.h"
#include "B2LobbyUI_ItemLevelupMain.h"
#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2UIManager_Lobby.h"
#include "B2DynItemIcon_ItemOpScene.h"
#include "Particles/ParticleSystemComponent.h"
#include "B2LobbyUI_ItemDetailPostOp.h"
#include "TutorialManager.h"

UB2LobbyUI_ItemLevelupProg::UB2LobbyUI_ItemLevelupProg(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedInventory = NULL;
	ItemIconWidgetClass = NULL;
	CreatedTargetItemIcon = NULL;
	TargetItemIconScreenPos = FVector2D(0.0f, 0.0f);
	IngredientIconPlacementMargin = 10.0f;
	IngredientIconNextRowOffset = FVector2D(0.0f, 20.0f);
	IngredientIconMaxSize = FVector2D(176.0f, 220.0f);

	OverallPlayRate = 1.0f;
	ItemSuckingTrailDepth = 5.0f; // 捞霸 农搁 粱 帮鄂. 2D 尔 3D 狼 甘俏苞 包访捞 登促焊聪.
	ItemSuckingStartTime = 0.5f;
	ItemSuckingInterval = 0.2f;
	ItemSuckingAccAcc = 100.0f;
	ItemSuckingBaseAcc = 200.0f;
	EffectiveItemSuckingAcc = 0.0f;
	ItemSuckingMaxSpeed = 400.0f;
	ItemSuckingTrailPS = NULL;
	ItemSuckingTrailScale = FVector(1.0f, 1.0f, 1.0f);
	ItemSuckingTrailLifeTime = 0.0f;
	bAllIngredSuckingFxCreated = false;
	
	OverallBGFxSetup.CreateDepth = 100.0f;
	CreatedOverallBGFx = NULL;
	EnhanceBlowupFxSetup.CreateDepth = 50.0f;
	CreatedEnhanceBlowupFx = NULL;
	TargetItemBGFxSetup.CreateDepth = 10.0f;
	CreatedTargetItemBGFx = NULL;
	FinalEnhanceSuccessFxSetup.CreateDepth = 5.0f;
	CreatedFinalEnhanceSuccessFx = NULL;

	FinalResultPageNRef = NULL;

	bUseCameraAdaptiveFOV = false;
	AdaptiveFOVScale = 0.6f;
}

void UB2LobbyUI_ItemLevelupProg::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, X_CP_TargetItemIconSet);
	GET_SLOT(UCanvasPanel, X_CP_EnhanceIngredIconPanel);
	//GET_SLOT(UWidgetAnimation, FinalResultAnimation);
}

void UB2LobbyUI_ItemLevelupProg::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	// 窜鉴洒 酒捞袍 沥焊父 掘绢坷妨搁 Inventory 牢胶畔胶 绝捞 static 栏肺 掘绢棵 荐 乐栏唱 碍拳 扁瓷篮 牢亥配府啊 栋 乐绰 惑怕俊辑 悼累.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);
	
	EffectiveItemSuckingAcc = ItemSuckingBaseAcc; // 殿啊加档(ItemSuckingAccAcc 0)俊辑绰 檬扁蔼捞 蜡瘤凳

	// 诀单捞飘 登扁 傈/饶 鸥百 酒捞袍 单捞磐啊 鞘夸 (UB2LobbyInventory::OnReqForItemOpCommon 俊辑 归诀) 
	bool bFailedToFindResultData = !CachedInventory->GetItemOPTargetItemData(NativeTargetLevelupItem_Before, true);
	if (bFailedToFindResultData || !IsSupportedOpMode(CachedInventory->GetItemOpMode()))
	{
		FinishAndProceedToResult(); // 父俊 窍唱 角菩茄促搁.. 绢驴 荐 绝瘤.
		return;
	}

	// 捞 矫痢俊 搬苞 单捞磐甫 啊廉坷瘤 给且 荐档 乐澜. 抛胶飘 目盖靛 荤侩捞芭唱 楷免 矫累 矫痢捞 粱 促福霸 登菌促芭唱..
	if (!CachedInventory->GetItemOPTargetItemData(NativeTargetLevelupItem_After, false))
		NativeTargetLevelupItem_After = NativeTargetLevelupItem_Before; // 角菩茄 版快 老窜篮 捞傈 单捞磐甫 持绢霖促.

	if (FinalResultPageNRef) // 捞扒 悼利 积己篮 酒聪促. 窜, 割啊瘤绰 流立 秦 林绢具.
	{
		FinalResultPageNRef->StartFromLobby(InUIManager, InGM);
		FinalResultPageNRef->UpdateItemData(NativeTargetLevelupItem_Before, NativeTargetLevelupItem_After);
		FinalResultPageNRef->ClosePopup(); // 贸澜俊绰 见败初澜. 楷免 弥辆 窜拌肺辑 焊咯柳促.
	}
		
	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, OverallPlayRate);

	ApplyCameraAdaptiveFOV();

	SetTargetItemIconData(NativeTargetLevelupItem_Before); // Before 肺 矫累.

	DestroyItemLevelupIngredientItemIcon();
	DestroySuckingFXComps();
	OnFinalIngredJustSucked();
}

void UB2LobbyUI_ItemLevelupProg::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// Sucking Fx 甸狼 框流烙 诀单捞飘.
	bool bAtLeastOneSuckingFxPresent = false;
	for (int32 FI = 0; FI < CreatedSuckingFxSet.Num(); ++FI)
	{
		if (CreatedSuckingFxSet[FI].FXComp){ // 档馒 饶俊绰 FXComp 啊 力芭登绊 NULL 捞 瞪 巴.
			bAtLeastOneSuckingFxPresent = true;
			UpdateSingleSuckingFxComp(FI, InDeltaTime);
		}
	}
	if (bAtLeastOneSuckingFxPresent)
	{
		EffectiveItemSuckingAcc += (ItemSuckingAccAcc * InDeltaTime); // 窍唱扼档 乐绢具 啊加档 刘啊.
	}
}

void UB2LobbyUI_ItemLevelupProg::DestroySelf()
{
	Super::DestroySelf();

	if (FinalResultPageNRef)
	{
		FinalResultPageNRef->DestroySelf();
	}

	DestroyTargetItemIcon();
	DestroyItemLevelupIngredientItemIcon();
	DestroySuckingFXComps();
	DestroyOverallBGFx();
	DestroyEnhanceBlowupFx();
	DestroyTargetItemBGFx();
	DestroyFinalEnhanceSuccessFxComp();
}

const FName UB2LobbyUI_ItemLevelupProg::Name_FxMID_ItemBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2LobbyUI_ItemLevelupProg::Name_FxMID_ItemIcon(TEXT("MAT_ItemIcon"));

UMaterialInstanceDynamic* UB2LobbyUI_ItemLevelupProg::GetFxMID_ItemBGPanel(class UParticleSystemComponent* InPSComp)
{
	return GetFxNamedMID(InPSComp, Name_FxMID_ItemBGPanel);
}

UMaterialInstanceDynamic* UB2LobbyUI_ItemLevelupProg::GetFxMID_ItemIcon(class UParticleSystemComponent* InPSComp)
{
	return GetFxNamedMID(InPSComp, Name_FxMID_ItemIcon);
}

void UB2LobbyUI_ItemLevelupProg::ApplyCameraAdaptiveFOV()
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

void UB2LobbyUI_ItemLevelupProg::SetTargetItemIconData(FB2Item& InItem)
{
	//DestroyTargetItemIcon();

	if (ItemIconWidgetClass == NULL || !X_CP_TargetItemIconSet.IsValid() || InItem.InstanceUID <= 0 || InItem.ItemRefID <= 0){
		return;
	}

	if (CreatedTargetItemIcon == nullptr)
	{
		// Anchor 汲沥俊 蝶扼 GetSize 绰 盔窍绰 蔼捞 救 唱棵 巴捞骨肺 林狼.
		UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(X_CP_TargetItemIconSet->Slot);
		FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);

		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

		// ItemIconPanelNRef 困俊 弥辆利牢 酒捞袍 酒捞能 widget 积己
		CreatedTargetItemIcon = Cast<UB2DynItemIcon_ItemOpScene>(DynCreateInCanvasPanel(
			ItemIconWidgetClass,
			this,
			X_CP_TargetItemIconSet.Get(),
			DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f),
			AllowedIconSize,
			FVector2D(0.0f, 0.0f),
			0,
			false));
	}

	if (CreatedTargetItemIcon)
	{
		// 酒捞能 吝缴 扁霖 胶农赴 傍埃 谅钎甫 掘绢柯促.
		// X_CP_TargetItemIconSet 狼 硅摹 炼扒篮 HACKGetWidgetScreenCoord 甫 曼绊		
		HACKGetWidgetScreenCoord(X_CP_TargetItemIconSet.Get(), GetOwningPlayer(), TargetItemIconScreenPos, true);
		CreatedTargetItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeProgTarget);
		CreatedTargetItemIcon->UpdateItemData(InItem);
	}
}

void UB2LobbyUI_ItemLevelupProg::SetIngredItemData(const TArray<FB2Item>& AllIngreds)
{
	DestroyItemLevelupIngredientItemIcon();
	DestroySuckingFXComps();
	
	if (ItemIconWidgetClass == NULL || !X_CP_EnhanceIngredIconPanel.IsValid() || AllIngreds.Num() == 0){
		return;
	}

	const FVector2D AllowedSingleSlotSize = GetAllowedSingleIngredientIconSize(); // Main panel 备己俊 蝶弗 倾侩 荤捞令
	const FVector2D CenterPos = GetIngredIconPanelCenterPos();
		
	const int32 MaxRowIndex = (AllIngreds.Num() - 1) / LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW;

	for (int32 II = 0; II < AllIngreds.Num(); ++II)
	{
		const FB2Item& ThisIngred = AllIngreds[II];

		////////////////////////////// 谅钎 拌魂 BEGIN
		const int32 RowIndex = II / LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW;
		// 捞锅 临俊辑 唱狼 鉴辑. 霉 临篮 捞 鉴辑措肺 啊肺 规氢 吝埃俊 啊滨霸 硅摹
		const int32 HPlacementIndex = II % LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW;

		float PosX = CenterPos.X;
		float PosY = CenterPos.Y;

		if (RowIndex == 0) // 霉锅掳 临篮 季磐俊辑 矫累秦辑 圈/娄 咯何 殿俊 蝶扼 谅钎 沥秦辑 谅快肺 欺廉唱皑.
		{
			// 捞锅 row 狼 霉锅掳 夸家 牢郸胶
			const int32 MyRowStartIndex = RowIndex * LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW;
			// 捞扒 捞锅 临捞 促 瞒瘤 臼绰促搁 硅凯 农扁啊 凳.
			const int32 NextRowStartIndex = FMath::Min(AllIngreds.Num(), MyRowStartIndex + LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW);
			// 捞锅 临俊 盲况龙 傈眉 酒捞能 俺荐
			const int32 TotalNumOfThisRow = NextRowStartIndex - MyRowStartIndex;
			const bool bRowHasEvenElem = (TotalNumOfThisRow % 2 == 0); // 圈荐/娄荐 牢瘤俊 蝶扼 硅摹 谅钎啊 官柴.
			const bool bAmIEvenIndex = (HPlacementIndex % 2 == 0);
			const bool bIsAtLeft = (bRowHasEvenElem == bAmIEvenIndex); // 圈荐老锭 0锅掳 夸家绰 角力肺绰 吝居

			if (bRowHasEvenElem)
			{
				float AbsDisplacement = ((float)((HPlacementIndex + 2) / 2) * (IngredientIconPlacementMargin + AllowedSingleSlotSize.X)) - (AllowedSingleSlotSize.X * 0.5f);
				PosX += (bAmIEvenIndex ? -1.0f : 1.0f) * AbsDisplacement;
			}
			else
			{
				if (HPlacementIndex != 0) // 圈荐俺 硅摹俊辑 贸澜 牢郸胶绰 季磐俊 官肺
				{
					float AbsDisplacement = ((float)((HPlacementIndex + 1) / 2) * (IngredientIconPlacementMargin + AllowedSingleSlotSize.X));
					PosX += (bAmIEvenIndex ? 1.0f : -1.0f) * AbsDisplacement;
				}
			}
		}
		else
		{ // 捞锅 临捞 圈娄牢瘤 咯何俊 惑包绝捞 霉锅掳 临俊 硅摹茄 巴阑 蝶扼啊具 且 巴. CreatedIngredIconsSet 沥纺等 霉掳临 鉴辑措肺
			if (HPlacementIndex < CreatedIngredIconsSet.Num())
			{
				PosX = CreatedIngredIconsSet[HPlacementIndex].RelativeCoord.X;
				PosY = CreatedIngredIconsSet[HPlacementIndex].RelativeCoord.Y;
			}

			PosX += (float)RowIndex * IngredientIconNextRowOffset.X; // 临付促 offset
			PosY += (float)RowIndex * IngredientIconNextRowOffset.Y;
		}

		FItemLevelupProgIngredItemIconSet NewIconSet;
		NewIconSet.RelativeCoord = FVector2D(PosX, PosY); // 谅惑窜 扁霖栏肺 弥辆 炼例登扁 傈俊 蝶肺 历厘秦 初澜. 颇萍努 积己矫俊 荤侩瞪荐档.

		PosX -= (AllowedSingleSlotSize.X * 0.5f); // 弥辆利栏肺 谅惑窜 扁霖 谅钎肺.
		PosY -= (AllowedSingleSlotSize.Y * 0.5f);
		////////////////////////////// 谅钎 拌魂 END

		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

		// MainPanelNativeRef 困俊 弥辆利牢 酒捞袍 酒捞能 widget 积己. RowIndex 付促 皑家窍绰 Z-order 甫 霖促. 菊俊波何磐 朝扼啊搁辑 瓤苞啊 焊捞档废 窍扁 困窃.
		UB2DynItemIcon_ItemOpScene* NewItemIcon = Cast<UB2DynItemIcon_ItemOpScene>(DynCreateInCanvasPanel(
			ItemIconWidgetClass, this, X_CP_EnhanceIngredIconPanel.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedSingleSlotSize,
			FVector2D(PosX, PosY), MaxRowIndex - RowIndex, false
			));
		if (NewItemIcon)
		{
			NewItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemLevelupMenuIngredient);
			NewItemIcon->UpdateItemData(ThisIngred); // 俺喊 酒捞袍 沥焊甫 逞败霖促.
						
			NewIconSet.DynIcon = NewItemIcon;
			CreatedIngredIconsSet.Add(NewIconSet);
			
			if (RowIndex == 0)
			{
				// 霉 临俊 茄秦辑 历厘等 谅钎 硅凯篮 RowIndex 啊 酒囱 X 谅钎 农扁措肺 沥纺茄促. 笛掳 临 捞饶何磐绰 捞 鉴辑措肺 瞒霸 瞪 巴. (郴瘤绰 笛掳 临 捞饶档 沥纺阑 且 荐档)
				struct FFirstRowIngredIconPosSorter
				{
					bool operator()(const FItemLevelupProgIngredItemIconSet& A, const FItemLevelupProgIngredItemIconSet& B) const { return A.RelativeCoord.X < B.RelativeCoord.X; }
				};
				CreatedIngredIconsSet.Sort(FFirstRowIngredIconPosSorter());
			}
		}
	}

	// 捞 矫痢俊 CreatedTargetItemIcon 捞 积己登绢 乐绢具 窃.
	BII_CHECK(CreatedTargetItemIcon);
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetTotalEnhanceIngredNum(AllIngreds.Num());
	}

	if (GetOwningPlayer() && CreatedIngredIconsSet.Num() > 0)
	{
		// 鉴辑措肺 sucking 瓤苞 矫累..
		if (ItemSuckingStartTime > 0.0f)
		{
			GetOwningPlayer()->GetWorldTimerManager().SetTimer(ItemSuckingIntervalTimer, FTimerDelegate::CreateUObject(this, &UB2LobbyUI_ItemLevelupProg::BeginIngredItemSucking, 0), ItemSuckingStartTime, false);
		}
		else
		{
			BeginIngredItemSucking(0);
		}
	}
}

int32 UB2LobbyUI_ItemLevelupProg::GetAllIngredNum()
{
	return CreatedIngredIconsSet.Num();
}

void UB2LobbyUI_ItemLevelupProg::UpdateStaticWidgets()
{
}

void UB2LobbyUI_ItemLevelupProg::DestroyTargetItemIcon()
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->DestroySelf();
		CreatedTargetItemIcon = NULL;
	}
}

void UB2LobbyUI_ItemLevelupProg::DestroyItemLevelupIngredientItemIcon()
{
	for (FItemLevelupProgIngredItemIconSet& CurrElem : CreatedIngredIconsSet)
	{
		if (CurrElem.DynIcon){
			CurrElem.DynIcon->DestroySelf();
		}
	}
	CreatedIngredIconsSet.Empty();
}

void UB2LobbyUI_ItemLevelupProg::DestroySuckingFXComps()
{
	for (FItemLevelupProgIngredSuckingFxSet& CurrFxSet : CreatedSuckingFxSet)
	{
		if (CurrFxSet.FXComp){
			CurrFxSet.FXComp->ConditionalBeginDestroy();
		}
	}
	CreatedSuckingFxSet.Empty();

	APlayerController* LocalOwningPlayer = GetOwningPlayer();
	//if (LocalOwningPlayer && LocalOwningPlayer->IsValidObj())
	{
		LocalOwningPlayer->GetWorldTimerManager().ClearTimer(ItemSuckingIntervalTimer);
	}
}

void UB2LobbyUI_ItemLevelupProg::DestroyOverallBGFx()
{
	if (CreatedOverallBGFx)
	{
		CreatedOverallBGFx->ConditionalBeginDestroy();
		CreatedOverallBGFx = NULL;
	}
}

void UB2LobbyUI_ItemLevelupProg::DestroyEnhanceBlowupFx()
{
	if (CreatedEnhanceBlowupFx)
	{
		CreatedEnhanceBlowupFx->ConditionalBeginDestroy();
		CreatedEnhanceBlowupFx = NULL;
	}
}

void UB2LobbyUI_ItemLevelupProg::DestroyTargetItemBGFx()
{
	if (CreatedTargetItemBGFx)
	{
		CreatedTargetItemBGFx->ConditionalBeginDestroy();
		CreatedTargetItemBGFx = NULL;
	}
}

void UB2LobbyUI_ItemLevelupProg::DestroyFinalEnhanceSuccessFxComp()
{
	if (CreatedFinalEnhanceSuccessFx)
	{
		CreatedFinalEnhanceSuccessFx->ConditionalBeginDestroy();
		CreatedFinalEnhanceSuccessFx = NULL;
	}
}

void UB2LobbyUI_ItemLevelupProg::BeginIngredItemSucking(int32 IngredIndex)
{
	if (CreatedIngredIconsSet.IsValidIndex(IngredIndex) && ItemSuckingTrailPS)
	{
		FVector FXCreateWorldPos = GetIngredIconDesiredWorldPos(IngredIndex); // 阿 酒捞能 胶农赴 谅钎俊 秦寸窍绰 岿靛 谅钎 掘绢咳. (FxCreatingDepth 父怒 酒贰)

		FTransform FXCreateTransform(FXCreateWorldPos);
		UParticleSystemComponent* CreatedSuckingFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ItemSuckingTrailPS, FXCreateTransform);

		if (CreatedSuckingFX)
		{
			CreatedSuckingFX->SetWorldScale3D(ItemSuckingTrailScale);

			FItemLevelupProgIngredSuckingFxSet NewFxSet;
			NewFxSet.FXComp = CreatedSuckingFX;

			// 鸥百鳖瘤 捞悼阑 困秦 格钎 芭府甫 持绢初澜. 捞 捞惑 捞悼窍搁 场牢 吧肺 秦辑 加档啊 狐福促绊 初磨 老 绝档废.
			NewFxSet.CurrentSpeed = 0.0f;
			NewFxSet.FlightDistSoFar = 0.0f;
			NewFxSet.FlightTimeSoFar = 0.0f;
			NewFxSet.TotalDistance = (GetTargetIconDesiredWorldPos(ItemSuckingTrailDepth) - CreatedSuckingFX->GetComponentLocation()).Size();

			UB2DynItemIcon_ItemOpScene* ThisIngredIcon = CreatedIngredIconsSet[IngredIndex].DynIcon;
			SetupNamedMIDForFxComp(NewFxSet.FXComp); // 鞘夸茄 MIC 甸俊辑 MID 甫 父甸绢辑 荤侩 啊瓷窍档废 技泼.
			// Fx 率俊 MID 鳖瘤 霖厚啊 登菌促搁 颇飘喊肺 颇扼固磐 蔼阑 啊廉棵 UI 率狼 MIC 甫 啊廉客辑 MID 俊 颇扼固磐甫 技泼.
			{
				UMaterialInstanceDynamic* ItemBGPanelMID = GetFxMID_ItemBGPanel(NewFxSet.FXComp);
				UMaterialInstanceConstant* ItemBGPanelMIC_Ref = ThisIngredIcon ? ThisIngredIcon->GetBGPanelMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(ItemBGPanelMID, ItemBGPanelMIC_Ref);
			}{
				UMaterialInstanceDynamic* ItemIconMID = GetFxMID_ItemIcon(NewFxSet.FXComp);
				UMaterialInstanceConstant* ItemIconMIC_Ref = ThisIngredIcon ? ThisIngredIcon->GetMainIconMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(ItemIconMID, ItemIconMIC_Ref);
			}
			
			OnSingleIngredBeginSucking(IngredIndex);

			CreatedSuckingFxSet.Add(NewFxSet);
		}

		if (IngredIndex == 0)
		{ // 霉锅掳 犁丰 矫累俊 措茄 捞亥飘.
			StartSfx_Phase2_FirstIngredient();
			OnFirstIngredBeginSucking_BP();
		}
	}	

	// 促澜 巴阑 积己窍扁 困茄 鸥捞赣
	APlayerController* LocalOwningPlayer = GetOwningPlayer();
	//if (LocalOwningPlayer && LocalOwningPlayer->IsValidObj())
	{
		LocalOwningPlayer->GetWorldTimerManager().ClearTimer(ItemSuckingIntervalTimer);
		if (CreatedIngredIconsSet.IsValidIndex(IngredIndex + 1))
		{
			LocalOwningPlayer->GetWorldTimerManager().SetTimer(ItemSuckingIntervalTimer, FTimerDelegate::CreateUObject(this, &UB2LobbyUI_ItemLevelupProg::BeginIngredItemSucking, IngredIndex + 1), FMath::Max(ItemSuckingInterval, 0.01f), false);
		}
		else
		{ // 葛滴 积己茄 芭. 捞力 辆丰 眉农啊 啊瓷窍档废 钎矫
			bAllIngredSuckingFxCreated = true;
		}
	}
}

void UB2LobbyUI_ItemLevelupProg::UpdateSingleSuckingFxComp(int32 IngredIndex, float InDeltaSec)
{
	if (CreatedSuckingFxSet.IsValidIndex(IngredIndex))
	{
		// 窜鉴茄 捞繁 啊加档, 加档 利侩栏肺 父练 救登搁 projectile 捞扼档.

		FItemLevelupProgIngredSuckingFxSet& CurrFxSet = CreatedSuckingFxSet[IngredIndex];
		if (CurrFxSet.FXComp)
		{
			FVector FXCompCurrPos = CurrFxSet.FXComp->GetComponentLocation();
			FVector ToTargetDir = (GetTargetIconDesiredWorldPos(ItemSuckingTrailDepth) - FXCompCurrPos);
			ToTargetDir.Normalize();

			// 如茄 dV = At, dS = Vt
			CurrFxSet.CurrentSpeed += (EffectiveItemSuckingAcc * InDeltaSec);
			CurrFxSet.CurrentSpeed = FMath::Min(ItemSuckingMaxSpeed, CurrFxSet.CurrentSpeed);
			FVector ThisMoveAmount = ToTargetDir * ((CurrFxSet.CurrentSpeed) * InDeltaSec);

			CurrFxSet.FXComp->SetWorldLocation(FXCompCurrPos + ThisMoveAmount);

			// 倔付唱 啊鳖况脸绰瘤 眉农窍绰 措脚 捞悼 芭府甫 歹秦辑 贸澜俊 魂沥茄 芭府甫 逞菌绰瘤 厚背. 呈公 弧府 捞悼秦档 救 初魔.
			CurrFxSet.FlightDistSoFar += ThisMoveAmount.Size();
			CurrFxSet.FlightTimeSoFar += InDeltaSec;

			const bool bDestroyThisOne = (
				(ItemSuckingTrailLifeTime > 0.0f && CurrFxSet.FlightTimeSoFar >= ItemSuckingTrailLifeTime) ||
				(CurrFxSet.FlightDistSoFar >= CurrFxSet.TotalDistance) // 矫埃阑 瘤沥窍瘤 臼疽促搁 档馒沁阑锭 颇鲍.
				); 

			if (bDestroyThisOne)
			{
				OnSingleIngredEndSucking(IngredIndex); // 窍唱 弧酒甸牢 巴俊 措茄 捞亥飘 せ

				CurrFxSet.FXComp->ConditionalBeginDestroy(); // 格钎 瘤痢 档馒 矫 FXComp 力芭. CurrFxSet 傈眉甫 CreatedSuckingFxSet 俊辑 力芭窍瘤绰 臼绰促. 眉农啊 粱 鞘夸秦辑.
				CurrFxSet.FXComp = NULL;

				CheckForAllFxSucked(); // 葛滴 弧酒甸看绰瘤 八荤.
			}
		}
	}
}

void UB2LobbyUI_ItemLevelupProg::CheckForAllFxSucked()
{
	// 葛滴 结欧 登菌绰瘤 八荤秦辑 捞亥飘甫 朝府绰单 
	// 窜鉴洒 IngredIndex == GetAllIngredNum() - 1 捞繁 侥栏肺 厚背秦辑 付瘤阜 结欧牢瘤 八荤秦档 奴 巩力 绝阑 芭 鞍瘤父 父俊 窍唱 菊 牢郸胶 巴捞 第词霸 档馒窍绰 荤怕啊 国绢龙 荐档 乐澜.
	if (bAllIngredSuckingFxCreated)
	{
		bool bAllSucked = true;
		for (FItemLevelupProgIngredSuckingFxSet& EndCheckFxSet : CreatedSuckingFxSet)
		{
			if (EndCheckFxSet.FXComp){
				bAllSucked = false;
				break;
			}
		}
		if (bAllSucked){
			OnFinalIngredJustSucked();
		}
	}
}

void UB2LobbyUI_ItemLevelupProg::OnFinalIngredJustSucked()
{
	// 付瘤阜 犁丰啊 软涝等 鉴埃何磐绰 绊沥等 鸥烙捞骨肺 傈开 瓤苞 鸥捞怪俊 亮阑 巴.
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->OnTargetItemLastSucking();
	}
	StartSfx_Phase3_FinalSucked(CachedInventory->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume);
	OnFinalIngredJustSucked_BP();
}

void UB2LobbyUI_ItemLevelupProg::UpdateTargetItemToNewData()
{
	FB2Item PreviewItem = NativeTargetLevelupItem_After;

	if (CachedInventory)
	{ 
		// 菊辑 角菩沁阑 荐档 乐栏骨肺 诀单捞飘 等 单捞磐 促矫 掘绢咳. 角菩茄 版快父 促矫 掘绢坷霸 且鳖..
		if (FinalResultPageNRef && CachedInventory->GetItemOPTargetItemData(NativeTargetLevelupItem_After, false))
			FinalResultPageNRef->UpdateItemData(NativeTargetLevelupItem_Before, NativeTargetLevelupItem_After);

		// 黎铺牢版快 扁措蔼栏肺 刚历 焊咯林绊 捞饶俊 角力 单捞磐肺 函版秦辑 焊咯淋
		if (CachedInventory->IsItemLevelupJackpot())
		{
			int32 OutLevel = 0;
			float OutExp = 0;
			TArray<FB2Item> CachedIngredItems;
			CachedInventory->GetAllGhostIngreds(CachedIngredItems);
			if (CachedIngredItems.Num() > 0)
				OutLevel = GetLevel_PreviewItemLevelup(OutExp, NativeTargetLevelupItem_Before, CachedIngredItems, true);

			PreviewItem.Level = OutLevel;
			PreviewItem.Exp = OutExp;
		}
	}
	
	SetTargetItemIconData(PreviewItem); // 固府 掘绢初篮 诀单捞飘 等 酒捞袍 单捞磐肺 技泼.
}

void UB2LobbyUI_ItemLevelupProg::ShowOverallBGFx()
{
	DestroyOverallBGFx();
	int32 ViewSizeX = (int32)DesignedRefViewSize.X;
	int32 ViewSizeY = (int32)DesignedRefViewSize.Y;
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetViewportSize(ViewSizeX, ViewSizeY); // 拳搁 吝居俊 积己
		CreatedOverallBGFx = OverallBGFxSetup.CreateUnderScreen(OwningPC, FVector2D((float)(ViewSizeX / 2), (float)(ViewSizeY / 2)));
	}	
}

void UB2LobbyUI_ItemLevelupProg::ShowEnhanceBlowupFx()
{
	DestroyEnhanceBlowupFx();
	int32 ViewSizeX = (int32)DesignedRefViewSize.X;
	int32 ViewSizeY = (int32)DesignedRefViewSize.Y;
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetViewportSize(ViewSizeX, ViewSizeY); // 拳搁 吝居俊 积己
		CreatedEnhanceBlowupFx = EnhanceBlowupFxSetup.CreateUnderScreen(OwningPC, FVector2D((float)(ViewSizeX / 2), (float)(ViewSizeY / 2)));

		if (CreatedEnhanceBlowupFx && CreatedTargetItemIcon)
		{
			SetupNamedMIDForFxComp(CreatedEnhanceBlowupFx); // 鞘夸茄 MIC 甸俊辑 MID 甫 父甸绢辑 荤侩 啊瓷窍档废 技泼.
			// Fx 率俊 MID 鳖瘤 霖厚啊 登菌促搁 颇飘喊肺 颇扼固磐 蔼阑 啊廉棵 UI 率狼 MIC 甫 啊廉客辑 MID 俊 颇扼固磐甫 技泼.
			{
				UMaterialInstanceDynamic* ItemBGPanelMID = GetFxMID_ItemBGPanel(CreatedEnhanceBlowupFx);
				UMaterialInstanceConstant* ItemBGPanelMIC_Ref = CreatedTargetItemIcon ? CreatedTargetItemIcon->GetBGPanelMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(ItemBGPanelMID, ItemBGPanelMIC_Ref);
			} {
				UMaterialInstanceDynamic* ItemIconMID = GetFxMID_ItemIcon(CreatedEnhanceBlowupFx);
				UMaterialInstanceConstant* ItemIconMIC_Ref = CreatedTargetItemIcon ? CreatedTargetItemIcon->GetMainIconMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(ItemIconMID, ItemIconMIC_Ref);
			}
		}
	}
}

void UB2LobbyUI_ItemLevelupProg::ShowTargetItemBGFx()
{
	DestroyTargetItemBGFx();
	CreatedTargetItemBGFx = TargetItemBGFxSetup.CreateUnderScreen(GetOwningPlayer(), TargetItemIconScreenPos);

	SetupNamedMIDForFxComp(CreatedTargetItemBGFx); // 鞘夸茄 MIC 甸俊辑 MID 甫 父甸绢辑 荤侩 啊瓷窍档废 技泼.
	// Fx 率俊 MID 鳖瘤 霖厚啊 登菌促搁 颇飘喊肺 颇扼固磐 蔼阑 啊廉棵 UI 率狼 MIC 甫 啊廉客辑 MID 俊 颇扼固磐甫 技泼.
	{
		UMaterialInstanceDynamic* ItemBGPanelMID = GetFxMID_ItemBGPanel(CreatedTargetItemBGFx);
		UMaterialInstanceConstant* ItemBGPanelMIC_Ref = CreatedTargetItemIcon ? CreatedTargetItemIcon->GetBGPanelMIC() : NULL;
		SetupMIDNonAtlasParamFromUIMIC(ItemBGPanelMID, ItemBGPanelMIC_Ref);
	}{
		UMaterialInstanceDynamic* ItemIconMID = GetFxMID_ItemIcon(CreatedTargetItemBGFx);
		UMaterialInstanceConstant* ItemIconMIC_Ref = CreatedTargetItemIcon ? CreatedTargetItemIcon->GetMainIconMIC() : NULL;
		SetupMIDNonAtlasParamFromUIMIC(ItemIconMID, ItemIconMIC_Ref);
	}
}

void UB2LobbyUI_ItemLevelupProg::ShowFinalEnhanceSuccessFx()
{
	DestroyFinalEnhanceSuccessFxComp();
	CreatedFinalEnhanceSuccessFx = FinalEnhanceSuccessFxSetup.CreateUnderScreen(GetOwningPlayer(), TargetItemIconScreenPos);
}

void UB2LobbyUI_ItemLevelupProg::SetTargetItemIconVisible(bool bVisible)
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
void UB2LobbyUI_ItemLevelupProg::SetTargetItemIconOverlayOnly(bool bOverlayOnly)
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetShowOverlayInfoOnly(!bOverlayOnly, true);
	}
}

void UB2LobbyUI_ItemLevelupProg::ShowFinalResultPage()
{
	if (FinalResultPageNRef)
	{
		FinalResultPageNRef->SetVisibility(ESlateVisibility::Visible);
		FinalResultPageNRef->OnVisibleUpdate();

		SetTargetItemIconVisible(false);
		FinalResultPageNRef->SetShowMainItemIcon(true);
	}
}

void UB2LobbyUI_ItemLevelupProg::ShowBattleScorePopup()
{  // 局聪皋捞记 楷免 辆丰矫 龋免登绰 傈捧仿 包访 扑诀 
	ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Signal(IntToPCClass(NativeTargetLevelupItem_After.AllowedPCClass));
}

void UB2LobbyUI_ItemLevelupProg::ShowCloseButton()
{
	if (FinalResultPageNRef)
	{
		FinalResultPageNRef->ShowBackButton();
	}
}

void UB2LobbyUI_ItemLevelupProg::OnSingleIngredBeginSucking(int32 IngredIndex)
{
	if (CreatedIngredIconsSet.IsValidIndex(IngredIndex))
	{
		FItemLevelupProgIngredItemIconSet& ThisSet = CreatedIngredIconsSet[IngredIndex];
		if (ThisSet.DynIcon)
		{
			ThisSet.DynIcon->OnIngredItemBeginSucking(IngredIndex); // 犁丰 酒捞袍 酒捞能俊 焊郴绰 捞亥飘
		}
	}
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->OnTargetItemBeginSucking(IngredIndex); // 鸥百 酒捞袍 酒捞能俊 焊郴绰 捞亥飘
	}

	StartSfx_Phase2N_EachIngredient();
}

void UB2LobbyUI_ItemLevelupProg::OnSingleIngredEndSucking(int32 IngredIndex)
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->OnTargetItemSuckedIngred(IngredIndex); // 鸥百 酒捞袍 酒捞能俊 焊郴绰 捞亥飘.
	}
}

FVector2D UB2LobbyUI_ItemLevelupProg::GetAllowedSingleIngredientIconSize()
{
	FVector2D ReturnSize(0.0f, 0.0f);

	// Anchor 汲沥俊 蝶扼 GetSize 绰 盔窍绰 蔼捞 救 唱棵 巴捞骨肺 林狼.
	UCanvasPanelSlot* MainPanelSlot = X_CP_EnhanceIngredIconPanel.IsValid() ? Cast<UCanvasPanelSlot>(X_CP_EnhanceIngredIconPanel->Slot) : NULL;
	if (MainPanelSlot)
	{
		FVector2D MainPanelSize = MainPanelSlot->GetSize();

		// MainPanelSize 客 Margin 俊 蝶扼..
		ReturnSize.X = ((MainPanelSize.X / (float)LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW) - IngredientIconPlacementMargin);
		ReturnSize.Y = MainPanelSize.Y;

		ReturnSize.X = FMath::Min(ReturnSize.X, IngredientIconMaxSize.X);
		ReturnSize.Y = FMath::Min(ReturnSize.Y, IngredientIconMaxSize.Y);
	}

	return ReturnSize;
}

FVector2D UB2LobbyUI_ItemLevelupProg::GetIngredIconPanelCenterPos()
{
	// Anchor 汲沥俊 蝶扼 GetSize 绰 盔窍绰 蔼捞 救 唱棵 巴捞骨肺 林狼.
	UCanvasPanelSlot* MainPanelSlot = X_CP_EnhanceIngredIconPanel.IsValid() ? Cast<UCanvasPanelSlot>(X_CP_EnhanceIngredIconPanel->Slot) : NULL;
	return MainPanelSlot ? (MainPanelSlot->GetSize() * FVector2D(0.5f, 0.5f)) : FVector2D(0.0f, 0.0f);
}

FVector2D UB2LobbyUI_ItemLevelupProg::GetIngredIconScreenPos(int32 IconIndex)
{
	if (CreatedIngredIconsSet.IsValidIndex(IconIndex))
	{
		FVector2D IngredPanelScreenCoord;
		
		// X_CP_EnhanceIngredIconPanel 狼 硅摹 炼扒篮 HACKGetWidgetScreenCoord 甫 曼绊
		if (HACKGetWidgetScreenCoord(X_CP_EnhanceIngredIconPanel.Get(), GetOwningPlayer(), IngredPanelScreenCoord, false, true))
		{			
			return ( // 菩澄 谅钎俊 Ingredient icon 甸捞 积己等 惑措 谅钎甫 歹窃. 
				IngredPanelScreenCoord +
				(CreatedIngredIconsSet[IconIndex].RelativeCoord * GetDesignToCurrentViewScale(GetOwningPlayer(), true)) 
				);
		}
	}
	return FVector2D(0.0f, 0.0f);
}

FVector UB2LobbyUI_ItemLevelupProg::GetIngredIconDesiredWorldPos(int32 IconIndex)
{
	FVector2D ScreenPos = GetIngredIconScreenPos(IconIndex);
	return GetWorldPosBeneathScreen(GetOwningPlayer(), ScreenPos, ItemSuckingTrailDepth);
}

FVector UB2LobbyUI_ItemLevelupProg::GetTargetIconDesiredWorldPos(float Depth)
{
	return GetWorldPosBeneathScreen(GetOwningPlayer(), TargetItemIconScreenPos, Depth);
}

void UB2LobbyUI_ItemLevelupProg::FinishAndProceedToResult()
{
	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, 1.0f); // OverallPlayRate 俊 蝶扼 函版沁带 芭 汗备.

	QuitItemOpModeClass<bool>::GetInstance().Signal(true); // 角力肺绰 搬苞芒捞 唱客具 窃. 芭扁辑 Quit 阑 窍霸 瞪 巴.
}