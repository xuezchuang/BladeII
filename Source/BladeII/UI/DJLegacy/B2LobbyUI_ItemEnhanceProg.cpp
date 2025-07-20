// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_ItemEnhanceProg.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "Event.h"
#include "B2DynItemIcon_ItemOpScene.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "B2LobbyUI_ItemDetailPostOp.h"
#include "TutorialManager.h"

// 酒贰 捞抚甸篮 漂沥 Particle System 府家胶狼 Named Material Slots 俊 沥狼登哥 肚茄 Required 葛碘狼 Named Material Override 俊辑 荤侩.
const FName UB2LobbyUI_ItemEnhanceProg::Name_FxMID_TargetBGPanel(TEXT("MAT_ItemBGPanel_Left"));
const FName UB2LobbyUI_ItemEnhanceProg::Name_FxMID_TargetItemIcon(TEXT("MAT_ItemIcon_Left"));
const FName UB2LobbyUI_ItemEnhanceProg::Name_FxMID_IngredBGPanel(TEXT("MAT_ItemBGPanel_Right"));
const FName UB2LobbyUI_ItemEnhanceProg::Name_FxMID_IngredItemIcon(TEXT("MAT_ItemIcon_Right"));
const FName UB2LobbyUI_ItemEnhanceProg::Name_FxMID_UnitingBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2LobbyUI_ItemEnhanceProg::Name_FxMID_UnitingItemIcon(TEXT("MAT_ItemIcon"));

UB2LobbyUI_ItemEnhanceProg::UB2LobbyUI_ItemEnhanceProg(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedInventory = NULL;
	CreatedItemIcon_Ingred = NULL;
	GettingClosePS = NULL;
	GettingCloseFxScale = FVector(1.0f, 1.0f, 1.0f);
	CreatedGettingCloseFx = NULL;
	EnhanceSuccessUnitingPS = NULL;
	EnhanceSuccessUnitingFxScale = FVector(1.0f, 1.0f, 1.0f);
	CreatedUnitingFx = NULL;
}

void UB2LobbyUI_ItemEnhanceProg::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, X_CP_ItemIconSet_Ingred);
}

void UB2LobbyUI_ItemEnhanceProg::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	SetItemData_Target(NativeItemData_Target);

	TArray<FB2Item> AllIngreds; // 犁丰绰 窍唱咯具 窃.
	CachedInventory->GetAllGhostIngreds(AllIngreds);
	if (AllIngreds.Num() > 0)
		SetItemData_Ingred(AllIngreds[0]);

	// 楷免俊辑 敬 捞镑狼 鸥百 酒捞袍 酒捞能阑 搬苞 拳搁俊辑档 楷厘秦辑 焊咯临 巴烙. 搬苞 其捞瘤俊 乐绰 扒 公矫窃.
	if (UIP_FinalResultPageP.IsValid())
	{
		UIP_FinalResultPageP->SetShowMainItemIcon(false);
	}

	PlayResultSound(CachedInventory->IsSuccessEnhanceItemForResultPresent());


	TUTORIAL // Tutorial Hardcoding
	if (TutorialManager::GetInstance().IsCurrentTutorial(TutorialID_ItemEnhance, 7))
	{
		TutorialManager::GetInstance().OnItemEnhancedClicked();
	}
}

void UB2LobbyUI_ItemEnhanceProg::DestroySelf()
{
	Super::DestroySelf();

	DestroyItemIcon_Ingred();
}

void UB2LobbyUI_ItemEnhanceProg::UpdateStaticWidgets()
{
	Super::UpdateStaticWidgets();
}

void UB2LobbyUI_ItemEnhanceProg::StartGettingCloseScene()
{
	DestroyAllFx(); // 父老 捞傈巴捞 巢酒乐绢具 茄促搁 官曹 巴.

	if (!GettingClosePS) {
		return;
	}

	FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
	CreatedGettingCloseFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GettingClosePS, FXCreateTransform);
	if (CreatedGettingCloseFx)
	{
		CreatedGettingCloseFx->SetWorldScale3D(GettingCloseFxScale);

		// 捞窍 RefID 俊 蝶扼 利例茄 赣萍府倔捞 利侩登绰 UI 酒捞能苞 付蛮啊瘤 寇包捞 唱坷档废 窃.

		SetupNamedMIDForFxComp(CreatedGettingCloseFx); // 鞘夸茄 MIC 甸俊辑 MID 甫 父甸绢辑 荤侩 啊瓷窍档废 技泼.
													   // Fx 率俊 MID 鳖瘤 霖厚啊 登菌促搁 颇飘喊肺 颇扼固磐 蔼阑 啊廉棵 UI 率狼 MIC 甫 啊廉客辑 MID 俊 颇扼固磐甫 技泼.
		{
			UMaterialInstanceDynamic* TargetBGPanelMID = GetFxNamedMID(CreatedGettingCloseFx, Name_FxMID_TargetBGPanel);
			UMaterialInstanceConstant* TargetBGPanelMIC_Ref = CreatedItemIcon_Target ? CreatedItemIcon_Target->GetBGPanelMIC() : NULL;
			SetupMIDNonAtlasParamFromUIMIC(TargetBGPanelMID, TargetBGPanelMIC_Ref);
		} {
			UMaterialInstanceDynamic* TargetItemIconMID = GetFxNamedMID(CreatedGettingCloseFx, Name_FxMID_TargetItemIcon);
			UMaterialInstanceConstant* TargetItemIconMIC_Ref = CreatedItemIcon_Target ? CreatedItemIcon_Target->GetMainIconMIC() : NULL;
			SetupMIDNonAtlasParamFromUIMIC(TargetItemIconMID, TargetItemIconMIC_Ref);
		} {
			UMaterialInstanceDynamic* IngredBGPanelMID = GetFxNamedMID(CreatedGettingCloseFx, Name_FxMID_IngredBGPanel);
			UMaterialInstanceConstant* IngredBGPanelMIC_Ref = CreatedItemIcon_Ingred ? CreatedItemIcon_Ingred->GetBGPanelMIC() : NULL;
			SetupMIDNonAtlasParamFromUIMIC(IngredBGPanelMID, IngredBGPanelMIC_Ref);
		} {
			UMaterialInstanceDynamic* IngredItemIconMID = GetFxNamedMID(CreatedGettingCloseFx, Name_FxMID_IngredItemIcon);
			UMaterialInstanceConstant* IngredItemIconMIC_Ref = CreatedItemIcon_Ingred ? CreatedItemIcon_Ingred->GetMainIconMIC() : NULL;
			SetupMIDNonAtlasParamFromUIMIC(IngredItemIconMID, IngredItemIconMIC_Ref);
		}
	}
}

void UB2LobbyUI_ItemEnhanceProg::StartUnitingScene()
{
	DestroyAllFx(); // 父老 捞傈巴捞 巢酒乐绢具 茄促搁 官曹 巴.

	check(CachedInventory);
	CachedInventory->GetItemOPTargetItemData(NativeItemData_Result, false); // 搬苞 酒捞袍 单捞磐甫 促矫 茄锅 啊廉咳.

	SetItemData_Result(NativeItemData_Result); // 搬苞 酒捞袍 酒捞能阑 泅犁 焊捞扒 救 焊捞扒 老窜 积己阑 秦具 窃. 弊贰具 官肺 酒贰俊辑 UI material 啊廉客辑 Fx 率俊辑 颇扼固磐甫 技泼茄促.
	if (CreatedItemIcon_Result)
		CreatedItemIcon_Result->SetVisibility(ESlateVisibility::Hidden); // 绢恫 版快电 老窜 见败初霸 瞪 巴捞促. 瘤陛篮 3D 瓤苞啊 唱客具 且 矫痢.
	
	UParticleSystem* CurUnitingPS = CachedInventory->IsSuccessEnhanceItemForResultPresent() ? EnhanceSuccessUnitingPS : EnhanceFailUnitingPS;
	if (CurUnitingPS)
	{
		FTransform FXCreateTransform(GetDesiredScreenCenterWorldPos());
		CreatedUnitingFx = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurUnitingPS, FXCreateTransform);
		if (CreatedUnitingFx)
		{
			CreatedUnitingFx->SetWorldScale3D(EnhanceSuccessUnitingFxScale);

			SetupNamedMIDForFxComp(CreatedUnitingFx); // 鞘夸茄 MIC 甸俊辑 MID 甫 父甸绢辑 荤侩 啊瓷窍档废 技泼.

			// Fx 率俊 MID 鳖瘤 霖厚啊 登菌促搁 颇飘喊肺 颇扼固磐 蔼阑 啊廉棵 UI 率狼 MIC 甫 啊廉客辑 MID 俊 颇扼固磐甫 技泼.
			{
				UMaterialInstanceDynamic* UnitingPSBGPanelMID = GetFxNamedMID(CreatedUnitingFx, Name_FxMID_UnitingBGPanel);
				UMaterialInstanceConstant* UnitingPSBGPanelMIC_Ref = CreatedItemIcon_Result ? CreatedItemIcon_Result->GetBGPanelMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(UnitingPSBGPanelMID, UnitingPSBGPanelMIC_Ref);
			} {
				UMaterialInstanceDynamic* UnitingPSItemIconMID = GetFxNamedMID(CreatedUnitingFx, Name_FxMID_UnitingItemIcon);
				UMaterialInstanceConstant* UnitingPSItemIconMIC_Ref = CreatedItemIcon_Result ? CreatedItemIcon_Result->GetMainIconMIC() : NULL;
				SetupMIDNonAtlasParamFromUIMIC(UnitingPSItemIconMID, UnitingPSItemIconMIC_Ref);
			}
		}
	}
}

void UB2LobbyUI_ItemEnhanceProg::SetItemData_Ingred(FB2Item& InItem)
{
	DestroyItemIcon_Ingred();

	NativeItemData_Ingred = InItem;
	CreatedItemIcon_Ingred = CreateItemIconCommon(X_CP_ItemIconSet_Ingred.Get(), NativeItemData_Ingred, ELobbyInvenItemIconUsage::EEIIT_ItemOpModeProgTarget);
}

void UB2LobbyUI_ItemEnhanceProg::DestroyItemIcon_Ingred()
{
	if (CreatedItemIcon_Ingred) {
		CreatedItemIcon_Ingred->ConditionalBeginDestroy();
		CreatedItemIcon_Ingred = NULL;
	}
}

void UB2LobbyUI_ItemEnhanceProg::DestroyAllFx()
{
	Super::DestroyAllFx();
	DestroyGettingCloseFx();
	DestroyUnitingFx();
}

void UB2LobbyUI_ItemEnhanceProg::DestroyGettingCloseFx()
{
	if (CreatedGettingCloseFx) {
		CreatedGettingCloseFx->ConditionalBeginDestroy();
		CreatedGettingCloseFx = NULL;
	}
}

void UB2LobbyUI_ItemEnhanceProg::DestroyUnitingFx()
{
	if (CreatedUnitingFx) {
		CreatedUnitingFx->ConditionalBeginDestroy();
		CreatedUnitingFx = NULL;
	}
}

void UB2LobbyUI_ItemEnhanceProg::ShowBattleScorePopup()
{// 局聪皋捞记 楷免 辆丰矫 龋免登绰 傈捧仿 包访 扑诀 
	ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Signal(IntToPCClass(NativeItemData_Target.AllowedPCClass));
}

void UB2LobbyUI_ItemEnhanceProg::ShowCloseButton()
{
	if (UIP_FinalResultPageP.IsValid())
	{
		UIP_FinalResultPageP->ShowBackButton();
	}
}

void UB2LobbyUI_ItemEnhanceProg::StartShowResultItemIcon()
{
	Super::StartShowResultItemIcon();

	// Note : 厘厚 碍拳绰 喊殿鞭捞 函版登绰巴篮 酒聪骨肺 殿鞭篮 楷免捞 酒囱 弊成 UI惑栏肺 钎矫窍档废 窍磊.
	if (CreatedItemIcon_Result)	
		CreatedItemIcon_Result->SetShowOverlayInfoOnly(false, true);
}
