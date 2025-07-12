
#include "B2PreRenderer.h"
#include "BladeIIGameImpl.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/SkeletalMeshActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "BladeIIGameMode.h"
#include "B2LobbyGameMode.h"
#include "B2NPCClassInfoBox.h"
#include "B2PCClassInfoBox.h"
#include "B2ItemInfo.h"
#include "B2SkillInfo.h"
#include "BladeIICharacter.h"
#include "Global.h"
#include "B2DamageEffectInfo.h"
#include "B2StageEventDirector.h"
#include "B2StageGameMode.h"
#include "B2SomeInfo.h"
#include "B2PreRenderGameMode.h"
#include "B2MonsterSpawnPool.h"
//#include "B2PreLoadingScreen.h"

#if PLATFORM_ANDROID
#include "Android/AndroidMisc.h"
#endif

bool FB2GMPreRenderDummyInfo::UpdateDummy(bool bForceContinue /*= false*/)
{
	//if (!PreRenderDummy) {
	//	return false;
	//}

	bool bNotFinishedCondition = bForceContinue; // �ϴ� ForceContinue �� ������ �� ����.

	//// ��Ʈ ������Ʈ�� ���� �⺻ PreRender üũ..
	//USkeletalMeshComponent* ThisDummySKComp = PreRenderDummy->GetSkeletalMeshComponent();
	//if (ThisDummySKComp) {
	//	ThisDummySKComp->SetExcessiveRenderProcessing(true);// ������ ������Ʈ �ʿ� �� ���� �־���� �ʾ��� �� �����Ͻð� ������Ʈ �Ǳ淡 �Ź� ������ ����µ�.. ���� �ʿ���� ����.
	//}
	//if (!ThisDummySKComp || ThisDummySKComp->LastRenderTime <= 0.0f) {
	//	bNotFinishedCondition = true;
	//}
	//if (PreRenderedCount < UB2PreRenderer::MaxPreRenderCount) {
	//	++PreRenderedCount;
	//	bNotFinishedCondition = true;
	//}

	//// ��Ʈ SkeletalMeshComponent �� ���� �߰� ParticleSystemComponent ���� LastRenderTime �� üũ. �̵��� Ư�� EmitterDelay ������ ���� �����δ� �� ������ �Ǵ� ��쵵 �ִ�.
	//int32 NotRenderedPSCNum = 0;
	//TArray<UActorComponent*> AllPSC = PreRenderDummy->GetComponentsByClass(UParticleSystemComponent::StaticClass());
	//for (UActorComponent* ThisAC : AllPSC)
	//{
	//	UParticleSystemComponent* ThisPSC = Cast<UParticleSystemComponent>(ThisAC);
	//	if (ThisPSC)
	//	{
	//		if (ThisPSC->LastRenderTime <= 0.0f){
	//			++NotRenderedPSCNum;
	//		}
	//		else {
	//			//ThisPSC->SetVisibility(false); // �ѹ� �� �� �ٷ� visibility ���ַ��� �ߴµ�.. EmitterDelay ���� �� �־.. �ϴ� ���.
	//		}
	//		ThisPSC->SetExcessiveRenderProcessing(true); // ������ ������Ʈ �ʿ� �� ���� �־���� �ʾ��� �� �����Ͻð� ������Ʈ �Ǳ淡 �Ź� ������ ����µ�.. ���� �ʿ���� ����.
	//	}
	//}

	//if (NotRenderedPSCNum > 0)
	//{
	//	bNotFinishedCondition = true;
	//}

	return !bNotFinishedCondition;
}

void FPreRenderCompleteState::MarkPCClassPreRendered(EPCClass InClass)
{
	PreRenderedPCs.AddUnique(InClass);
}
bool FPreRenderCompleteState::IsPCClassPreRendered(EPCClass InClass) const
{
	return (PreRenderedPCs.Contains(InClass));
}

void FPreRenderCompleteState::MarkSkillAnimPreRendered(FCombinedPCSkillAnimID InCombinedPCSkillAnimId)
{
	bool& ValueRef = PreRenderedSkillAnims.FindOrAdd(InCombinedPCSkillAnimId);
	ValueRef = true; // Key �� ������ �ǹǷ� ���� �ʿ�� ������ �ǹ̻� �־���
}
bool FPreRenderCompleteState::IsSkillAnimPreRendered(FCombinedPCSkillAnimID InCombinedPCSkillAnimId) const
{
	return (PreRenderedSkillAnims.Find(InCombinedPCSkillAnimId) != nullptr);
}

void FPreRenderCompleteState::MarkNPCClassPreRendered(ENPCClass InClass, ENPCClassVariation InClassVari)
{
	MarkNPCClassPreRendered(FCombinedNPCClassID(InClass, InClassVari));
}
void FPreRenderCompleteState::MarkNPCClassPreRendered(FCombinedNPCClassID InCombinedNPCId)
{
	bool& ValueRef = PreRenderedNPCs.FindOrAdd(InCombinedNPCId);
	ValueRef = true; // Key �� ������ �ǹǷ� ���� �ʿ�� ������ �ǹ̻� �־���
}
bool FPreRenderCompleteState::IsNPCClassPreRendered(ENPCClass InClass, ENPCClassVariation InClassVari) const
{
	return IsNPCClassPreRendered(FCombinedNPCClassID(InClass, InClassVari));
}
bool FPreRenderCompleteState::IsNPCClassPreRendered(FCombinedNPCClassID InCombinedNPCId) const
{
	return (PreRenderedNPCs.Find(InCombinedNPCId) != nullptr);
}

void FPreRenderCompleteState::MarkWorldLevelPreRendered(class UWorld* InWorld)
{
	UPackage* WorldOuterMost = InWorld ? InWorld->GetOutermost() : nullptr;
	if (WorldOuterMost)
	{
		// ���� ������ P ������ �ش��ϴ� ���� �̸����� Pre-render ���θ� ��ũ.
		// ���� �����̸� �̸� ��ġ�� �� ����� ��.
		bool& ValueRef = PreRenderedLevels.FindOrAdd(WorldOuterMost->GetFName());
		ValueRef = true; // Key �� ������ �ǹǷ� ���� �ʿ�� ������ �ǹ̻� �־���
	}
}
bool FPreRenderCompleteState::IsWorldLevelPreRendered(class UWorld* InWorld) const
{
	UPackage* WorldOuterMost = InWorld ? InWorld->GetOutermost() : nullptr;
	if (WorldOuterMost)
	{
		return (PreRenderedLevels.Find(WorldOuterMost->GetFName()) != nullptr);
	}
	return false;
}

/**
 * ==================================================================
 * Pre-render �� ������� ���̴� ĳ���� r.UseProgramBinaryCache �� ���� ���� ����ȴٸ�
 * Pre-render ���¸� ���ÿ� ���� �� �ε������μ� ������ ����ÿ��� �ѹ� pre-render �� ��ģ ���� �ݺ����� �ʵ��� �Ѵ�.
 * ==================================================================
 */

bool FPreRenderCompleteState::CanSavePreRenderState() const
{
#if PLATFORM_ANDROID
	// ���̳ʸ� ���̴� ĳ�� ���� ���ΰ� �߿���.
	IConsoleVariable* UseProgramBinaryCacheVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.UseProgramBinaryCache"));
	return (FAndroidMisc::ShouldUseVulkan() || // Vulkan �� OpenGL �� ������ ����ǵ��� ó����.. FVulkanDynamicRHI::SavePipelineCache
		(UseProgramBinaryCacheVar && (UseProgramBinaryCacheVar->GetInt() != 0))
		//FOpenGLES2::SupportsProgramBinary() �̰� ���� include �� �� �ȵż�.. OpenGL ES �� SupportsProgramBinary �� ����ŭ ���� ����̽� �ƴϸ� �� ������ �ϴµ�.. �� ������ Ʋ�� ��� �÷��̸� ������ �ϴ� ���� ���̴� ĳ���� �Ǹ��� ������ ��. 
		// �� ������ �Ǹ� FOpenGLProgramBinaryCache::Initialize ���� ĳ�� �ߴ��� ���θ� ���������� �� ������..
		);
#elif PLATFORM_IOS
	// PreRender �Ϻθ� �ϰ� FShaderCache �� ���嵵 �� (FShaderCache::InitShaderCache)
	return UB2PreRenderer::ShouldDoPreRenderForIOS();
#elif PLATFORM_WINDOWS
	return true; // ���� �׽�Ʈ�� ����..
#else
	return false;
#endif
}

void FPreRenderCompleteState::SaveState()
{
	if (CanSavePreRenderState() && GConfig)
	{
		SavePCClassPreRenderState();
		SaveSkillAnimPreRenderState();
		SaveNPCClassPreRenderState();
		SaveWorldLevelPreRenderState();

		GConfig->SetBool(TEXT("Misc"), TEXT("PreRenderLevel"), bPreRenderLevelGoneThrough, GB2PreRenderSavedStateIni);
		GConfig->SetBool(TEXT("Misc"), TEXT("DamageEffectInfo"), bDamageEffectInfoPreRendered, GB2PreRenderSavedStateIni);
	}
}
void FPreRenderCompleteState::LoadState()
{
	if (CanSavePreRenderState() && GConfig)
	{
		LoadPCClassPreRenderState();
		LoadSkillAnimPreRenderState();
		LoadNPCClassPreRenderState();
		LoadWorldLevelPreRenderState();

		GConfig->GetBool(TEXT("Misc"), TEXT("PreRenderLevel"), bPreRenderLevelGoneThrough, GB2PreRenderSavedStateIni);
		GConfig->GetBool(TEXT("Misc"), TEXT("DamageEffectInfo"), bDamageEffectInfoPreRendered, GB2PreRenderSavedStateIni);
	}
}
void FPreRenderCompleteState::ClearState()
{
	// ���� �� ����� �ƴϰ� ���� �׽�Ʈ �� ����ε� Ȥ ���̴� ĳ�� ���� ���� ��Ȳ���� �ʿ��� ����� ������? 

	PreRenderedPCs.Empty();
	PreRenderedSkillAnims.Empty();
	PreRenderedNPCs.Empty();
	PreRenderedLevels.Empty();
	bPreRenderLevelGoneThrough = false;
	bDamageEffectInfoPreRendered = false;

	if (GConfig)
	{
		//GConfig->EmptyAll(GB2PreRenderSavedStateIni);
		GConfig->Flush(false, GB2PreRenderSavedStateIni);
	}
}

void FPreRenderCompleteState::SavePCClassPreRenderState()
{
	TArray<FString> SaveClassString;
	for (EPCClass ThisPCClass : PreRenderedPCs)
	{
		SaveClassString.Add(FString::Printf(TEXT("%d"), PCClassToInt(ThisPCClass)));
	}
	GConfig->SetArray(TEXT("PCClass"), TEXT("PCClass"), SaveClassString, GB2PreRenderSavedStateIni);
}
void FPreRenderCompleteState::LoadPCClassPreRenderState()
{
	PreRenderedPCs.Empty();

	TArray<FString> LoadedClassString;
	GConfig->GetArray(TEXT("PCClass"), TEXT("PCClass"), LoadedClassString, GB2PreRenderSavedStateIni);
	for (const FString& ThisClassStr : LoadedClassString)
	{
		PreRenderedPCs.Add(IntToPCClass(FCString::Atoi(*ThisClassStr)));
	}
}

void FPreRenderCompleteState::SaveSkillAnimPreRenderState()
{
	TArray<FString> SaveSkillAnimString;
	for (TMap<FCombinedPCSkillAnimID, bool>::TIterator SAIt(PreRenderedSkillAnims); SAIt; ++SAIt)
	{
		FCombinedPCSkillAnimID& ThisSkillAnimID = SAIt.Key();
		SaveSkillAnimString.Add(
			FString::Printf(TEXT("%d_%d"), PCClassToInt(ThisSkillAnimID.GetPCClassEnum()), (int32)ThisSkillAnimID.GetSkillAnimType())
		);
	}
	GConfig->SetArray(TEXT("SkillAnim"), TEXT("SkillAnim"), SaveSkillAnimString, GB2PreRenderSavedStateIni);
}
void FPreRenderCompleteState::LoadSkillAnimPreRenderState()
{
	PreRenderedSkillAnims.Empty();

	TArray<FString> LoadedSkillAnimString;
	GConfig->GetArray(TEXT("SkillAnim"), TEXT("SkillAnim"), LoadedSkillAnimString, GB2PreRenderSavedStateIni);
	for (const FString& ThisSkillAnimStr : LoadedSkillAnimString)
	{
		FString PCClassStr;
		FString SkillAnimTypeStr;
		if (ThisSkillAnimStr.Split(TEXT("_"), &PCClassStr, &SkillAnimTypeStr))
		{
			EPCClass ParsedPCClass = IntToPCClass(FCString::Atoi(*PCClassStr));
			ESkillAnimType SkillAnimType = (ESkillAnimType)(FCString::Atoi(*SkillAnimTypeStr));

			PreRenderedSkillAnims.Add(FCombinedPCSkillAnimID(ParsedPCClass, SkillAnimType), true);
		}
	}
}
void FPreRenderCompleteState::SaveNPCClassPreRenderState()
{
	TArray<FString> SavedNPCClassIDString;
	for (TMap<FCombinedNPCClassID, bool>::TIterator NPCIt(PreRenderedNPCs); NPCIt; ++NPCIt)
	{
		FCombinedNPCClassID& ThisNPCID = NPCIt.Key();
		SavedNPCClassIDString.Add(
			FString::Printf(TEXT("%d_%d"), (int32)ThisNPCID.GetNPCClassEnum(), (int32)ThisNPCID.GetNPCVariationEnum())
		);
	}
	GConfig->SetArray(TEXT("NPCClass"), TEXT("NPCClass"), SavedNPCClassIDString, GB2PreRenderSavedStateIni);
}
void FPreRenderCompleteState::LoadNPCClassPreRenderState()
{
	PreRenderedNPCs.Empty();

	TArray<FString> LoadedNPCIDString;
	GConfig->GetArray(TEXT("NPCClass"), TEXT("NPCClass"), LoadedNPCIDString, GB2PreRenderSavedStateIni);
	for (const FString& ThisNPCIDStr : LoadedNPCIDString)
	{
		FString NPCClassStr;
		FString NPCVariStr;
		if (ThisNPCIDStr.Split(TEXT("_"), &NPCClassStr, &NPCVariStr))
		{
			ENPCClass ParsedNPCClass = (ENPCClass)FCString::Atoi(*NPCClassStr);
			ENPCClassVariation NPCVariation = (ENPCClassVariation)(FCString::Atoi(*NPCVariStr));

			PreRenderedNPCs.Add(FCombinedNPCClassID(ParsedNPCClass, NPCVariation), true);
		}
	}
}
void FPreRenderCompleteState::SaveWorldLevelPreRenderState()
{
	TArray<FString> SavedLevelString;
	for (TMap<FName, bool>::TIterator PreRenderLevelIt(PreRenderedLevels); PreRenderLevelIt; ++PreRenderLevelIt)
	{
		SavedLevelString.Add(PreRenderLevelIt.Key().ToString());
	}
	GConfig->SetArray(TEXT("WorldLevel"), TEXT("WorldLevel"), SavedLevelString, GB2PreRenderSavedStateIni);
}
void FPreRenderCompleteState::LoadWorldLevelPreRenderState()
{
	PreRenderedLevels.Empty();

	TArray<FString> LoadedLevelString;
	GConfig->GetArray(TEXT("WorldLevel"), TEXT("WorldLevel"), LoadedLevelString, GB2PreRenderSavedStateIni);
	for (const FString& ThisLevelStr : LoadedLevelString)
	{
		PreRenderedLevels.Add(FName(*ThisLevelStr));
	}
}

void FPreRenderPerMaterialState::AddUsedMobility(EComponentMobility::Type InMobility)
{
	AllUsedMobilities.AddUnique(InMobility);
}
bool FPreRenderPerMaterialState::CheckMobilityUsed(EComponentMobility::Type InMobility) const
{
	return AllUsedMobilities.Contains(InMobility);
}
const FName FPreRenderMaterialCompleteState::GetMaterialKeyName(class UMaterialInterface* InCheckMtrl) const
{
	// ���̴� ���� �������� material �� �������ڸ� 
	// �츮�μ��� StaticSwitch �� �����ϰ� ��� ���ո��� ������ MasterMaterial �� ����ϴ� �����̶� �θ� material �� ���� �� ���� �ִµ�
	// Ȯ���� �ƴ� ���� �־ �ϴ� �׳� MaterialInterface ������ üũ
	// ��, MID �� �ΰ��� ���� �����̶� ���ܷ�..
	UMaterialInterface* RetMtrl = InCheckMtrl;
	UMaterialInstanceDynamic* CastedMID = Cast<UMaterialInstanceDynamic>(InCheckMtrl);
	if (CastedMID)
	{
		RetMtrl = CastedMID->Parent; // MIC �ų� Material �� ���� ���� üũ�� �ʿ������ ���� �� ����.
	}
	if (RetMtrl)
	{
		return FName(*RetMtrl->GetPathName());
	}
	return NAME_None;
}
void FPreRenderMaterialCompleteState::MarkPrimCompMaterials(class UPrimitiveComponent* InPrimComp)
{
	if (InPrimComp)
	{
		TArray<UMaterialInterface*> AllUsedMtrls;
		InPrimComp->GetUsedMaterials(AllUsedMtrls);

		for (UMaterialInterface* ThisOne : AllUsedMtrls)
		{
			const FName ThisMtrlName = GetMaterialKeyName(ThisOne);
			if (ThisMtrlName != NAME_None)
			{
				FPreRenderPerMaterialState& FoundOrAddPerMtrlState = AllStates.FindOrAdd(ThisMtrlName);
				FoundOrAddPerMtrlState.AddUsedMobility(InPrimComp->Mobility);
			}
		}
	}
}
bool FPreRenderMaterialCompleteState::ArePrimCompAllMaterialsPreRendered(class UPrimitiveComponent* InPrimComp) const
{ // InPrimComp �� ����ϴ� ��� Material �� ó���Ǿ����� üũ
	if (InPrimComp)
	{
		TArray<UMaterialInterface*> AllUsedMtrls;
		InPrimComp->GetUsedMaterials(AllUsedMtrls);

		bool bFoundNonRendered = false;
		for (UMaterialInterface* ThisOne : AllUsedMtrls)
		{
			const FName ThisMtrlName = GetMaterialKeyName(ThisOne);
			if (ThisMtrlName != NAME_None)
			{
				const FPreRenderPerMaterialState* FoundState = AllStates.Find(ThisMtrlName);
				// �� Material �� �ƿ� ó���� �� ���ų�, �ִ��� mobility �� �޶��ٸ�.. (�� ��� ���� ���̴��� �ٸ� Ȯ���� ����)
				if (!FoundState || !FoundState->CheckMobilityUsed(InPrimComp->Mobility))
				{
					bFoundNonRendered = true;
					break;
				}
			}
		}
		return !bFoundNonRendered; // ��, ���� material �� ���� ��� true ������ �ǰڴµ� �̰� ��� ������ �װ� �±� �ϰڴ�.
	}
	return false;
}
void FPreRenderMaterialCompleteState::ClearState()
{
	AllStates.Empty();
}

FPreRenderCompleteState UB2PreRenderer::TotalCompleteState;
FPreRenderMaterialCompleteState UB2PreRenderer::MaterialBasedCompleteState;

UB2PreRenderer::UB2PreRenderer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OwnerGameMode = nullptr;

	OverallTickCount = 0;
	bCanDoOverallCount = false;
	TickCountFromSetup = 0;
	PreRenderAllPrimTickCount = 0;

	bLightEnvironmentSetupForDynPointLight = false;
	bLocalPlayerHeadLightModified = false;
	LastCachedLocalPCLightRadius = 0.0f;
	LastCachedLocalPCLightIntensity = 0.0f;
	PreRenderDummyPointLight = nullptr;

	bLightEnvironmentSetupForDynDirLight = false;
	PreRenderDirLight = nullptr;

	bHadDoneAnyPreRender = false;
	bPendingForPreRenderGM = false;

	//CachedPreLoadingScreen = nullptr;
	EstimatedTotalProgressStep = 0;
	CurrPrerenderProgressStep = 0;
	ProgressStepCountForSelectedPhase = 0;
}

bool UB2PreRenderer::SetupPreRenderObjects(class ABladeIIGameMode* InGM) // ���̴� �������� �̸� �� ���� �ʴ� ����� �÷��������� ���̴� �������� ���� ���. ���⼭ ��ġ�� �� �ִٰ� ���� �۵� ���ϴ� �� �ƴϰ� �� �߰��� �ʿ��� �� �� �����Ÿ�.
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2PreRenderer::SetupPreRenderObjects"));
	B2_SCOPED_TIME_LOG(TEXT("UB2PreRenderer::SetupPreRenderObjects"));

	OwnerGameMode = InGM;
	check(OwnerGameMode);
	if (!OwnerGameMode)
	{
		return false;
	}

	// ��Ģ������ �� Material ���� Pre-render ���� Ʈ��ŷ�� ���带 �������� ������ �� �� �ִ� ��������
	// ������ ������ material �̴��� �ٸ� ���� ���� �Ͽ����� ���̴� ������ ó���� Ȯ������ �ʾ� �� �������� �����ϴ� �ɷ� ��.
	MaterialBasedCompleteState.ClearState();

	AllPreRenderDummy.Empty();
	UWorld* TheWorld = OwnerGameMode->GetWorld();

	if (GetB2GameModeType(OwnerGameMode) == EB2GameMode::Lobby)
	{
		// �κ�� Ư�������� ���� ���� ���� ó����. ��ǻ� ���� �����ϰ� �� ó�� �� �� ó���ϴ� �� �� ��.
		SetupPreRenderObjects_FirstLobby();
	}
	else
	{
#if PLATFORM_IOS
		if (ShouldDoPreRenderForIOS_Basic())
#endif
		{
			{
				// PC �⺻ �����ʹ� ��ǻ� ��κ��� PreRenderGameMode ���� ���� ��ġ �� ó�� �ѹ� ó���� �� ���̴�.
				TArray<EPCClass> PCClassesToLoad = OwnerGameMode->GetPCClassesToPreLoad();
				for (EPCClass ThisCharClass : PCClassesToLoad)
				{
					ConditionalSetupPreRenderObjectForPCClass(ThisCharClass);
				}

				// PreloadAnyNecessaryInfo �� �� �ܾ�� NPCClass ��� prerender ���� ����
				TArray<FCombinedNPCClassID> AllExpectedNPCs;
				AB2MonsterSpawnPool* ActiveSP = OwnerGameMode->GetActiveSpawnPool();
				if (ActiveSP)
				{
					ActiveSP->GetAllExpectedNPCIDs(AllExpectedNPCs);
				}
				for (FCombinedNPCClassID ThisMobId : AllExpectedNPCs)
				{
					ConditionalSetupPreRenderObjectForNPCClass(ThisMobId);
				}

				// PCClass ������ �� ��ġ�� ũ�� ���� PC ���� ���¿� ���� ���� ������ ������ Skill Anim ������..
				TArray<FCombinedPCSkillAnimID> SkillAnimsToPreLoad = OwnerGameMode->GetPCSkillAnimsToPreLoad();
				ConditionalSetupPreRenderObjectForPCSkillAnims(SkillAnimsToPreLoad);

				// DamageEffectInfo.. �ΰ��� ���ɿ� ������ �ϳ� ��ġ�� ���� ��鵵 pre-render �� ����. ��鵵 ó�� �ѹ��� �ʿ��� �� �ѵ�..
				ConditionalSetupPreRenderObjectForDamageEffectInfo();
			}

			// ���� �¾��� ���� PreRender �¾�. ���� ���忡 ���ؼ� ������ pre-render ���� ���� ������. 
			// ��� �Ʒ� StageEventDirector �� ���� �����̶� StageNumber �� ���� ��� ���ΰ� �ٸ� �� �־ �װͱ��� üũ�ϴ� �� Ȯ���ϱ� �ϴ�.
			// �����δ� �̰ͱ��� ������� pre-render �� ö���ϰ� �Ŵ� �� ũ�� ȿ���� �ִ� �� ������ �ʾƼ� StageNumber ������� �ѹ��� �ɾ��.
			if (!IsCurrentWorldLevelPreRendered())
			{
				ASkeletalMeshActor* ExtraDummySKActor = GetExtraPrerenderDummySKActor(); // ExtraDummy �� �ʿ��� ��Ȳ������ �����´�.
				if (ExtraDummySKActor)
				{
					for (AB2StageEventDirector* SED : OwnerGameMode->GetEventDirectorArray())
					{
						if (SED && SED->IsStageSupportedForGameMode(OwnerGameMode))
						{
							TArray<ASkeletalMeshActor*> ShowSettingUnboundSKActors;
							SED->SetupPreRenderSKMeshActor(ExtraDummySKActor, ShowSettingUnboundSKActors);

							for (ASkeletalMeshActor* NewSKActor : ShowSettingUnboundSKActors)
							{ // ShowSettings �� ������� ���� ���⿡�� ���Ǵ� SkeletalMeshActor �� ������ ��. ��鵵 PreRenderDummy ��..
								FB2GMPreRenderDummyInfo NewDummyInfo;
								NewDummyInfo.PreRenderDummy = NewSKActor;
								AllPreRenderDummy.Add(NewDummyInfo);
							}
						}
					}
				}
			}
		}
	}

	// �������� ��ó��
	PostPreRenderSeupProcess();

	// ������ ���� ������ ������ AllPrimitives �ɼ��� �ְų� ���� ���� ���� �����̸� ���� ������ �ϰ� �� �� ������ true ����
	if (AllPreRenderDummy.Num() > 0 || (bPreRenderAllPrimitives && !IsCurrentWorldLevelPreRendered()) || bPendingForPreRenderGM)
	{
		bHadDoneAnyPreRender = true; // ���� ������ �� �� �ƴ����� �� �÷��� ���� �� ���뿡�� true ��ũ�ص� ���ڴ�.

		if (!bPendingForPreRenderGM)
		{ // Pre-render �Ǵ� ���� ȭ�� ������ ����
			SetupDefaultPreRenderScreen();
		}

		return true;
	}

	return false; // ���� ���� ������ �˴� ������ Pre-render �� ���ƴٸ� �̷��� ���� �� ��. �ε� �ð� ���Ƽ ������ ���� ��.
}

void UB2PreRenderer::ConditionalSetupPreRenderObjectForPCClass(EPCClass InClass, bool bForceSetup)
{
	if ((!TotalCompleteState.IsPCClassPreRendered(InClass) || bForceSetup)
		&& OwnerGameMode)
	{
		UWorld* TheWorld = OwnerGameMode->GetWorld();

		FB2GMPreRenderDummyInfo ThisDummyInfo;

		// ���� ĳ������ ���� ���� ���� ������ ���� ���µ� �����ؼ��� ó���� �ѹ� ���� ���� �⺻ ���鸸 ����ϰ� �� ��. 
		// ���� Ŭ������ ���� ��� �޶� �������ϸ� �θ� material �̶� ���� �״� � ��� ������ ū ����� ���� �� ������.. ���ϰ� ���� �ǹ̴� ���� �ڵ� ��
		// ������ ���⼭ �߿��� �� �ִϸ��̼ǿ� ���� ����Ʈ���̰� �װ� Ŭ���� �����θ� ���� ��.
		TArray<FB2Item> ThisCharEquipment;
		FB2Wing ThisCharWing;
		BladeIIGameImpl::GetLocalCharacterData().GetEquippedItems(InClass, ThisCharEquipment);
		BladeIIGameImpl::GetLocalCharacterData().GetEquippedCostumeItems(InClass, ThisCharEquipment);
		bool bHasWing = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InClass, ThisCharWing);
		ICharacterDataStore::GetRenderItem(&BladeIIGameImpl::GetLocalCharacterData(), InClass, ThisCharEquipment);

		ThisDummyInfo.PreRenderDummy = SpawnPreRenderMeshCommon(TheWorld); // �⺻ SkeletalMeshActor Spawn

		if (ThisDummyInfo.PreRenderDummy)
		{
			// Mesh merge.. AnimBP ���õ� �� ����. 
			SetupDummyPCMeshForPreRender(InClass, NULL, ThisDummyInfo.PreRenderDummy, ThisCharEquipment, bHasWing ? &ThisCharWing : NULL);
			// AnimBP �� ���� �ܾ���� ParticleSystem �鵵 �ѷ��ش�.
			SetupDummyParticleSystemsForPreRender(ThisDummyInfo.PreRenderDummy);
		}

		TotalCompleteState.MarkPCClassPreRendered(InClass); // �¾��� �ϰ� �� �� �ɷ� ��ũ. ���� �� ó�� �� ���� ������ �����Ϸ��� ���̱⵵ �ϰ� �ʰ��ﰣ �¿�鼭 �׷� �ʿ���� ���� �� ������ �ʰ�..

		AllPreRenderDummy.Add(ThisDummyInfo);
	}
}
void UB2PreRenderer::ConditionalSetupPreRenderObjectForNPCClass(const FCombinedNPCClassID InClassID, bool bForceSetup)
{
	if ((!TotalCompleteState.IsNPCClassPreRendered(InClassID) || bForceSetup)
		&& OwnerGameMode)
	{
		UWorld* TheWorld = OwnerGameMode->GetWorld();

		FB2GMPreRenderDummyInfo ThisDummyInfo;
		ThisDummyInfo.PreRenderDummy = SpawnPreRenderMeshCommon(TheWorld); // �⺻ SkeletalMeshActor Spawn

		if (ThisDummyInfo.PreRenderDummy)
		{
			SetupDummyMobForPreRender(InClassID.GetNPCClassEnum(), InClassID.GetNPCVariationEnum(), NULL, ThisDummyInfo.PreRenderDummy);
			// AnimBP �� ���� �ܾ���� ParticleSystem �鵵 �ѷ��ش�.
			SetupDummyParticleSystemsForPreRender(ThisDummyInfo.PreRenderDummy);
		}

		TotalCompleteState.MarkNPCClassPreRendered(InClassID); // �¾��� �ϰ� �� �� �ɷ� ��ũ. ���� �� ó�� �� ���� ������ �����Ϸ��� ���̱⵵ �ϰ� �ʰ��ﰣ �¿�鼭 �׷� �ʿ���� ���� �� ������ �ʰ�..

		AllPreRenderDummy.Add(ThisDummyInfo);
	}
}
void UB2PreRenderer::ConditionalSetupPreRenderObjectForPCSkillAnims(const TArray<FCombinedPCSkillAnimID>& InSkillIDs, bool bForceSetup)
{
	// ��� ���� �� ��ų�ִϸ��̼� �ϳ����� �ƴϰ� �������� �޵��� ��.

	UB2SkillAnimInfo* SkillAnimInfo = StaticFindSkillAnimInfo(OwnerGameMode);
	if (!SkillAnimInfo)
	{
		return;
	}

	//TMap<FName, UParticleSystem*> GatheredPS;
	//for (const FCombinedPCSkillAnimID ThisSkillAnimID : InSkillIDs)
	//{
	//	if (!TotalCompleteState.IsSkillAnimPreRendered(ThisSkillAnimID) || bForceSetup)
	//	{
	//		// Async �ε��� �ϴ��� ��ǻ� ���⼭ �ʿ��ϸ� �ε��� ������ �� ���̴�.
	//		UAnimSequenceBase* ThisSkillAnim = SkillAnimInfo->GetSingleSkillAnim(ThisSkillAnimID);
	//		if (!ThisSkillAnim) {
	//			continue;
	//		}
	//		TryGatherAllBoundPSFromAnimSeq(ThisSkillAnim, GatheredPS); // �ߺ��Ǵ� �� ������ ���鼭 ����.
	//		// ������ ���⵵ ���� �¾��� �Ϸ������ �ʾ����� �� �� �ɷ� ��ũ.
	//		TotalCompleteState.MarkSkillAnimPreRendered(ThisSkillAnimID);
	//	}
	//}
	//if (GatheredPS.Num() > 0)
	//{
	//	// ExtraDummy �� �ʿ��� ��Ȳ������ �����;� �Ѵ�. �� �׷��� ������ Pre-render �� �ʿ���µ��� �� ������ ���� ���縷�� �������� ���� �ϰ� �� �� �ִ�.
	//	ASkeletalMeshActor* ExtraDummySKActor = GetExtraPrerenderDummySKActor();
	//	if (ExtraDummySKActor)
	//	{
	//		// ��� SkillAnimsToPreLoad ���� particle system ���� ���� ���Ŀ� Prerender �� component �� spawn ��Ų��.
	//		for (TMap<FName, UParticleSystem*>::TConstIterator PSIt(GatheredPS); PSIt; ++PSIt)
	//		{
	//			UParticleSystem* ThisPS = PSIt.Value();
	//			// DummySKActor �� attach ��Ű�� ���Ĵ� DummySKActor�� ��Ʈ��
	//			UParticleSystemComponent* ThisSpawnedFxComp = SpawnDummyPSCForPreRender(ThisPS, ExtraDummySKActor->GetRootComponent());
	//		}
	//	}
	//}
}
void UB2PreRenderer::ConditionalSetupPreRenderObjectForDamageEffectInfo(bool bForceSetup)
{
	UB2DamageEffectInfo* DmgFxInfo = StaticFindDamageEffectInfo(OwnerGameMode);
	if ((!TotalCompleteState.IsDamageEffectInfoPreRendered() || bForceSetup)
		&& DmgFxInfo)
	{
		TArray<ASkeletalMeshActor*> DummiesForMtrlOverride; // MaterialOverride �� ���� ���� SKActor �� �ʿ�..
		int32 DesiredDummyNum = DmgFxInfo->GetDesiredPreRenderDummyNumForMtrlOverride();
		for (int32 DI = 0; DI < DesiredDummyNum; ++DI)
		{
			DummiesForMtrlOverride.Add(GetAdditionalPrerenderDummySKActor());
		}
		DmgFxInfo->SetupPreRenderMaterialOverride(DummiesForMtrlOverride);

		ASkeletalMeshActor* ExtraDummySKActor = GetExtraPrerenderDummySKActor(); // �Ƹ��� �ٷ� ������ ���� ���� �� ù��° ���� �� ��.
		if (ExtraDummySKActor)
		{
			DmgFxInfo->SetupPreRenderSKMeshActorForFX(ExtraDummySKActor);
		}

		TotalCompleteState.MarkDamageEffectInfoPreRendered(); // �¾��� �ϰ� �� �� �ɷ� ��ũ.
	}
}
void UB2PreRenderer::PostPreRenderSeupProcess()
{
	//// AllPrim �� �ƴ� PreRenderDummy �� ����ϴ� �Ϲ� PreRender �� ���� �⺻ �¾� ���� ó��.
	//for (FB2GMPreRenderDummyInfo& ThisPreRenderInfo : AllPreRenderDummy)
	//{
	//	if (ThisPreRenderInfo.PreRenderDummy && ThisPreRenderInfo.PreRenderDummy->GetSkeletalMeshComponent())
	//	{
	//		ThisPreRenderInfo.PreRenderDummy->SetActorHiddenInGame(false);
	//		ThisPreRenderInfo.PreRenderDummy->GetSkeletalMeshComponent()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	//		ThisPreRenderInfo.PreRenderDummy->GetSkeletalMeshComponent()->TickAnimation(0.01f, false); // ��ƽ�� �������ϴ� ��츦 ���ؼ� �ѹ����� TickAnim �� �����ش�.
	//	}
	//	ThisPreRenderInfo.PreRenderedCount = 0;
	//}

	//OverallTickCount = 0;
	//bCanDoOverallCount = false;
	//TickCountFromSetup = 0;

	//FlushRenderingCommands(); // �̷��� �ϸ� �� ������ �Ƿ���...
}

void UB2PreRenderer::SetupPreRenderObjects_FirstLobby()
{
	// LobbyGameMode �� ����. LobbyGameMode ������ Ư���� ���� ��ġ �� ù ����� �ѹ� pre-rendering �� ���� ������Ʈ���� ó���Ѵ�. 
	// ���Ŀ� ������������ ���ƿ��ų� �� ������ ������� ����.

	check(OwnerGameMode && Cast<AB2LobbyGameMode>(OwnerGameMode));

	// ���� pre-render ���� ������ �ʿ��ϰ� ���� �������� üũ�� �Ѵ�.
	// ���������� FirstLobby �� �ǹ̷δ� �� ��Ʈ�� �⺻�� �� ��.
	if (!IsPreRenderLevelGoneThrough() && CheckBladeIIPreRenderGameAvailability()
#if PLATFORM_IOS
		&& ShouldDoPreRenderForIOS_PreRenderGM()
#endif
		)
	{
		bPendingForPreRenderGM = true;
	}

	// �κ�� ��ü�� ���µ� ����.
	if (IsCurrentWorldLevelPreRendered())
	{
		return;
	}

	B2_SCOPED_TRACK_LOG(TEXT("UB2PreRenderer::SetupPreRenderObjects_FirstLobby"));

	if (!bPendingForPreRenderGM && !IsPreRenderLevelGoneThrough()
#if PLATFORM_IOS
		// �̰͵� ��ǻ� PreRenderGM �� ����� �ǹ̶�� ShouldDoPreRenderForIOS_PreRenderGM ���� �ƽθ� ���ܿ��� �Ѿ���� �ϴ� �� �� ���� �ɼ��� �ִ�.
		&& ShouldDoPreRenderForIOS_Basic()
#endif
		)
	{
		// Pre-render ��ü�� �� �ϴ� Android �� ��� ���������� �� ��Ʈ�� ������ ���� ����� �� ��. ������ ������ġ�μ��� �ǹ� ���ΰ�..

		// �غ� �ȵǾ��ų� ���� ã�� ���߰ų�.. � ���� �κ񿡼� ���� �����Ѵ�.
		// �� ������ Extra �� �����ϴ� ��. �κ񿡼� ���� �͵��� ��ϵ� �� Extra ���̴�.
		SetupPreRenderObjects_ExtraNPCClass(); // ExtraNPCClass �� ���⼭ �� �ص� �� �ε� �ϸ鼭 �ϰ� ������ �׳� �̸� �� ����..
		SetupPreRenderObjects_ExtraFx();
		SetupPreRenderObjects_ExtraOthers();
	}

	// �κ� ���� pre-render �� ���� �Ǿ��ٴ� �ǹ� + AllPrim ó���� ���� ���ؼ��� Mark
	// PreRenderGame �� �ε��Ѵ� �ϴ��� ��ǻ� �κ񿡼� �Ϸ��� �� �ϴ� �Ŷ� ������ �Ǿ��ٰ� ��ũ�ؾ�.
	MarkCurrentWorldLevelPreRendered();
}

void UB2PreRenderer::SetupPreRenderObjects_ExtraFx()
{
	// Ư�� �з� ���� SomeInfo �� Pre-render ������ ����� ���� �ֵ�.
	// ����, ĳ����, Ȥ�� Ư�� InfoAsset �� ������ �ϰ������� ó���Ǵ� ��� Ư���ϰ� �� Pre-render ó���Ǵ� ����Ʈ ���ҽ� ���
	UB2SomeInfo* SomeInfo = StaticFindSomeInfo(OwnerGameMode);

	ASkeletalMeshActor* ExtraDummySKActor = GetExtraPrerenderDummySKActor();
	if (!ExtraDummySKActor)
	{
		return;
	}

	if (CachedExtraPreRenderPS.Num() == 0) // SomeInfo ���� ���⿡ ����� ����Ʈ�� LoadAnd"Consume" �� �ϴµ� ���ʿ����� PreRenderGameMode ���� �������� ���� �� �����Ƿ� ���� ĳ���� ����.
	{
		if (SomeInfo)
		{
			SomeInfo->LoadAndConsumeUniquePreRenderPS(CachedExtraPreRenderPS);
		}
	}
	for (UParticleSystem* ThisPS : CachedExtraPreRenderPS)
	{
		UParticleSystemComponent* ThisSpawnedFxComp = SpawnDummyPSCForPreRender(ThisPS, ExtraDummySKActor->GetRootComponent());
	}

	bHadDoneAnyPreRender = true; // ���� ������ �� �� �ƴ����� �� �÷��� ���� �� ���뿡�� true ��ũ�ص� ���ڴ�.
}
void UB2PreRenderer::SetupPreRenderObjects_ExtraNPCClass()
{
	// Ư�� �������� ó�� �÷��� �� ó�� �����ϴ� NPCClass �� ���ؼ� pre-render �� ����Ǵµ� ���⼭ �ϴ� �� �װŶ� ���������̴�. 
	// ��, �̷��� SomeInfo �� ����� ���� pre-render �� ���� ������ �� �ֵ��� �Ѱ� �̷� �� PreReder �������� �̸� �� ���� ó�� ������ �ε��� �� pre-render �� ���� ���Ƽ�� �� ���̰��� �ϱ� ����.
	// ��� NPCClass ���� �� �̰� ���� ó�������� ���Ѵ�. ������ ���..

	UB2SomeInfo* SomeInfo = StaticFindSomeInfo(OwnerGameMode);

	if (CachedExtraPreRenderNPCClassInfo.Num() == 0) // SomeInfo ���� ���⿡ ����� ����Ʈ�� LoadAnd"Consume" �� �ϴµ� ���ʿ����� PreRenderGameMode ���� �������� ���� �� �����Ƿ� ���� ĳ���� ����.
	{
		if (SomeInfo)
		{
			SomeInfo->LoadAndConsumeUniquePreRenderNPCClass(CachedExtraPreRenderNPCClassInfo);
		}
	}
	for (TSubclassOf<class UB2NPCSingleClassInfo> ThisInfo : CachedExtraPreRenderNPCClassInfo)
	{
		UB2NPCSingleClassInfo* ThisInfoObj = ThisInfo ? Cast<UB2NPCSingleClassInfo>(ThisInfo->GetDefaultObject()) : nullptr;
		if (ThisInfoObj)
		{
			ConditionalSetupPreRenderObjectForNPCClass(FCombinedNPCClassID(ThisInfoObj->NPCClassEnum, ThisInfoObj->ClassVariation), true);
		}
	}

	bHadDoneAnyPreRender = true; // ���� ������ �� �� �ƴ����� �� �÷��� ���� �� ���뿡�� true ��ũ�ص� ���ڴ�.
}

void UB2PreRenderer::SetupPreRenderObjects_ExtraOthers()
{
	// Ư�� �з� ���� SomeInfo �� Pre-render ������ ����� ���� �ֵ�.. �߿����� ������..
	UB2SomeInfo* SomeInfo = StaticFindSomeInfo(OwnerGameMode);

	if (CachedExtraPreRenderSKMesh.Num() == 0) // SomeInfo ���� ���⿡ ����� ����Ʈ�� LoadAnd"Consume" �� �ϴµ� ���ʿ����� PreRenderGameMode ���� �������� ���� �� �����Ƿ� ���� ĳ���� ����.
	{
		if (SomeInfo)
		{
			SomeInfo->LoadAndConsumeUniquePreRenderSKMesh(CachedExtraPreRenderSKMesh);
		}
	}
	for (USkeletalMesh* ThisMesh : CachedExtraPreRenderSKMesh)
	{
		FB2GMPreRenderDummyInfo NewExtraDummyInfo;
		NewExtraDummyInfo.PreRenderDummy = SpawnPreRenderMeshCommon(OwnerGameMode ? OwnerGameMode->GetWorld() : GetWorld());
		if (NewExtraDummyInfo.PreRenderDummy && NewExtraDummyInfo.PreRenderDummy->GetSkeletalMeshComponent())
		{
			NewExtraDummyInfo.PreRenderDummy->GetSkeletalMeshComponent()->SetSkeletalMesh(ThisMesh);
		}
		AllPreRenderDummy.Add(NewExtraDummyInfo);
	}

	bHadDoneAnyPreRender = true;
}

void UB2PreRenderer::ConditionalSetupPreRenderLightEnvForDynPL(bool bForceSetup)
{
	//const bool bShouldSetupDummyDynPointLight = (!bLightEnvironmentSetupForDynPointLight || bForceSetup);
	//const bool bShouldSetupPlayerHeadLight = (!bLocalPlayerHeadLightModified || bForceSetup);

	//if (!GIsEditor && (bShouldSetupDummyDynPointLight || bShouldSetupPlayerHeadLight))
	//{
	//	AB2StageGameModeBase* CastedSGM = Cast<AB2StageGameModeBase>(OwnerGameMode);
	//	if (CastedSGM)
	//	{ // ���� �̰� ������ ���⶧�� �������� �ǵ��ؼ� ��ġ�� ���� Dynamic PointLight ���� �ϰ������� �� ���� �����ε� StageGameMode BeginPlay �� ������ �ȴ�.
	//	  // ���⼭ �̸� ������ �� ��. ������ ��ġ�� PointLight �� ���⼭ �¾��ϴ� �ɷ� Ȯ���� �����ϱ� ����.
	//		// �ٵ� �̷��� �ص� �ᱹ PreRender ���ư��� ���߿� ������ ���۵Ǹ鼭 ���⿡�� ����ϴ� PointLight �� �Ѱ� �Ǵµ�.. AllPrim phase ���� �� �ɷ���.
	//		CastedSGM->DeactivateWorldMovablePointLight();
	//	}
	//}

	//if (bShouldSetupDummyDynPointLight)
	//{
	//	// DynamicPointLight �� �߰��� �ϳ� ����. �÷��̾� ĳ���� PointLight ���������δ� �� ���� ���� �ִ�.. �츮 ������ ����Ͽ��� �ΰ����� �����ϵ��� ������ �������� �̰ͱ���.
	//	UWorld* TheWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : GetWorld();
	//	if (TheWorld)
	//	{
	//		FActorSpawnParameters SpawnInfo;
	//		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//		SpawnInfo.ObjectFlags |= RF_Transient;
	//		FTransform SpawnTransform;
	//		SpawnTransform.SetLocation(GetPreRenderDummyDesiredLocation(TheWorld) + FVector(0.0f,0.0f,10000.0f)); // ��ġ�� �� ��������. Ȥ���� ���� ���̿����� ������� �ҷ���.. �Ѥ�;;
	//		PreRenderDummyPointLight = TheWorld->SpawnActor<APointLight>(APointLight::StaticClass(), SpawnTransform, SpawnInfo);
	//		if (PreRenderDummyPointLight)
	//		{
	//			PreRenderDummyPointLight->SetMobility(EComponentMobility::Movable);
	//			UPointLightComponent* PLComp = Cast<UPointLightComponent>(PreRenderDummyPointLight->GetLightComponent());
	//			if (PLComp)
	//			{
	//				PLComp->bAffectsWorld = true;
	//				PLComp->Activate();
	//				PLComp->RegisterComponent();
	//				PLComp->SetAttenuationRadius(1000000.0f);
	//				PLComp->SetIntensity(1000000.0f);
	//				PLComp->MarkRenderStateDirty();
	//			}

	//			bLightEnvironmentSetupForDynPointLight = true;
	//		}
	//	}
	//}

	//// bLightEnvironmentSetupForDynPointLight ���ʿ� ��ġ��ų ���� �ֱ� �ѵ�.. ���� ����..
	//if (bShouldSetupPlayerHeadLight)
	//{
	//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(OwnerGameMode));
	//	if (LocalB2Player && LocalB2Player->LightComponent)
	//	{
	//		// ���� �÷��̾� point light �� ������� �ʴ� ���Ӹ��� �ص� ���⼱ �Ҵ�. ĳ���� �� Pre-render �� �ѹ��� ���ư��� �Ǵµ� ���� ����Ʈ ���� ä�� ���ư��� ȿ���� �ݰ��Ǵ�..

	//		LastCachedLocalPCLightIntensity = LocalB2Player->LightComponent->Intensity;
	//		LastCachedLocalPCLightRadius = LocalB2Player->LightComponent->AttenuationRadius;

	//		// �ݰ��� ���� �а� �ؼ� Pre-render �� �ɷ��� ���� ������Ʈ�鿡 ������ ������ ��.
	//		LocalB2Player->LightComponent->SetAttenuationRadius(1000000.0f);
	//		LocalB2Player->LightComponent->SetIntensity(1000000.0f); // Intensity �� Ȥ�ó� �ؼ� ��

	//		// ���� ���� ���� ����. ��� ���� ������ ���� �ϳ��� spawn ��Ű�� �� ���� ��. 
	//		// ����Ͽ��� Dynamic Pointlight ���� ������ �ִ� ���� ���� �װ� �� �ȸ����� �׷��� ����.
	//		// FormalSetPointLightOn �� ��ġ�� �ʰ� ���� (���� ��½) �Ҵ�. �����ؼ� �ٷ�� ��.
	//		LocalB2Player->LightComponent->bAffectsWorld = true; // bHeadPointLightIsFormallyOn �� ���� ���� ������ ä�� bAffectsWorld �� ��
	//		LocalB2Player->LightComponent->Activate();
	//		LocalB2Player->LightComponent->RegisterComponent();
	//		LocalB2Player->LightComponent->MarkRenderStateDirty();

	//		bLocalPlayerHeadLightModified = true; // bLightEnvironmentSetupForDynPointLight ���� ������ ���¸� ��ũ�صд�. Ȥ���� Ÿ�̹� ������ �ٸ� ���� �����Ƿ�.
	//	}
	//}
}
void UB2PreRenderer::ConditionalRestoreFromPreRenderLightEnvForDynPL(bool bRestoreAll)
{
	//// �÷��̾� ĳ���� light ��������. �̰� RestoreAll �� �� ������, ��, RestoreAll �� ���� ������ ���̸� �������´�.
	//if (bLocalPlayerHeadLightModified && bRestoreAll)
	//{
	//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(OwnerGameMode));
	//	if (LocalB2Player && LocalB2Player->LightComponent)
	//	{
	//		LocalB2Player->LightComponent->SetIntensity(LastCachedLocalPCLightIntensity);
	//		LocalB2Player->LightComponent->SetAttenuationRadius(LastCachedLocalPCLightRadius);

	//		// ���� ��Ʈ�� ���� �ִ� ���� ���� ä ���ΰ� �ƴϸ� ��
	//		if (!LocalB2Player->IsHeadPointLightAllowed() || !LocalB2Player->IsHeadPointLightFormallyOn())
	//		{
	//			LocalB2Player->FormalSetPointLightOn(false);
	//		}
	//	}
	//	bLocalPlayerHeadLightModified = false;
	//}
	//// �߰� ���̵� ����
	//if (bLightEnvironmentSetupForDynPointLight)
	//{		
	//	if (PreRenderDummyPointLight && PreRenderDummyPointLight->GetLightComponent())
	//	{
	//		PreRenderDummyPointLight->GetLightComponent()->bAffectsWorld = false;
	//		PreRenderDummyPointLight->GetLightComponent()->UnregisterComponent();
	//		PreRenderDummyPointLight->GetLightComponent()->Deactivate();
	//		PreRenderDummyPointLight->GetLightComponent()->MarkRenderStateDirty();
	//		PreRenderDummyPointLight->Destroy();
	//		PreRenderDummyPointLight = nullptr;
	//	}

	//	bLightEnvironmentSetupForDynPointLight = false;
	//}
}

void UB2PreRenderer::MinimizePlayerPointLight()
{
	//// PointLight ȯ���� ��ǻ� 0 ���� ������� �� �� ����. 
	//// ConditionalRestoreFromPreRenderLightEnvForDynPL �� ������� �÷��̾� ĳ���� ����Ʈ�� ������ ������ �ʰ� �÷��̾� ĳ���ͷκ��� ���� �ݰ� �ȿ����� ������ �� ���� PointLight �� ���̰� �ȴ�.
	//// ConditionalSetupPreRenderLightEnvForDynPL �� ���� �̹� ���� �ݰ����� �¾��� �� ��Ȳ������ ����ϵ��� �Ѵ�.
	//// �׷��� ConditionalRestoreFromPreRenderLightEnvForDynPL ���� ������� ���ư� ���̴�.
	//if (bLocalPlayerHeadLightModified && LastCachedLocalPCLightRadius > 0.0f)
	//{
	//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(OwnerGameMode));
	//	if (LocalB2Player && LocalB2Player->LightComponent && LocalB2Player->LightComponent->bAffectsWorld)
	//	{
	//		// LastCachedLocalPCLightRadius �� �̹� �������� ��Ȳ������ ���� ����� ������ �����ϰ� ���⼭ ���� ĳ������ �ʴ´�.
	//		// �̰� ���̴� ��Ȳ������ AttenuationRadius �� �Ƹ� �ſ� ũ�� ���õ� ��Ȳ�� ���̴�.
	//		LocalB2Player->LightComponent->SetAttenuationRadius(0.01f);
	//	}
	//}
}

void UB2PreRenderer::ConditionalSetupPreRenderLightEnvForDirL(EComponentMobility::Type LightMobility, bool bForceSetup, bool bSetupForCSM)
{
	// DirectionalLight ȯ���� �ٲٴ� �� ��ǻ� PreRender ���� ���Ӹ�� �ƴϸ� �ٷ�� ��ٷο� �� ����. 
	// ��� ���� �������δ�..
	check(Cast<AB2PreRenderGameMode>(OwnerGameMode));

	if (!bLightEnvironmentSetupForDynDirLight || bForceSetup)
	{
		// �ϴ� ������ ��ġ�� �� �ִ������� ã�ƺ���.
		PreRenderDirLight = FindDirectionalLighOfMobility(LightMobility);
		// ��ġ�� �� ������ �ϳ� �����.
		if (!PreRenderDirLight)
		{
			UWorld* TheWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : GetWorld();
			if (TheWorld)
			{
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnInfo.ObjectFlags |= RF_Transient;
				// ���Ɽ�̴� ��ġ�� �� �������.. ��� ���⵵ ��Ե� ���� �ޱ⸸ �ϸ� �Ǵµ� �׷��� ������ ������ �Ʒ��� �� ����
				FTransform SpawnTransform(FRotator(-90.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f));
				PreRenderDirLight = TheWorld->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), SpawnTransform, SpawnInfo);
				if (PreRenderDirLight)
				{
					PreRenderDirLight->SetMobility(LightMobility);
					UDirectionalLightComponent* DLComp = Cast<UDirectionalLightComponent>(PreRenderDirLight->GetLightComponent());
					if (DLComp)
					{
						DLComp->SetIntensity(1000000.0f);
					}
				}
			}
		}

		if (PreRenderDirLight)
		{
			UDirectionalLightComponent* DLComp = Cast<UDirectionalLightComponent>(PreRenderDirLight->GetLightComponent());
			if (DLComp)
			{
				DLComp->bAffectsWorld = true;

				if (bSetupForCSM && LightMobility != EComponentMobility::Static)
				{ // ������ ���̴� �������� �ʿ��� CascadedShadowMap �� �¾�
					DLComp->bCastModulatedShadows = false;
					if (LightMobility == EComponentMobility::Stationary)
					{
						DLComp->DynamicShadowDistanceStationaryLight = 20000.0f;
					}
					else if (LightMobility == EComponentMobility::Movable)
					{
						DLComp->DynamicShadowDistanceMovableLight = 20000.0f;
					}
					DLComp->bUseInsetShadowsForMovableObjects = false;
				}
				else
				{ // �ƴ� ��� Modulated shadow ..
					DLComp->bCastModulatedShadows = true;
					DLComp->DynamicShadowDistanceStationaryLight = 0.0f;
					DLComp->DynamicShadowDistanceMovableLight = 0.0f;
				}
				DLComp->UnregisterComponent(); // ��� ȯ�濡 ���� �� ���� unregister �� �� �㿡 register �� �ؾ� Scene �� ����� add �Ǵ� ��찡 ����. �׷��� �ٺ� ��.

				DLComp->Activate();
				DLComp->RegisterComponent();
				DLComp->MarkRenderStateDirty();
			}

			bLightEnvironmentSetupForDynDirLight = true;
		}
	}
}
void UB2PreRenderer::ConditionalRestoreFromPreRenderLightEnvForDirL()
{
	// DirectionalLight ȯ���� �ٲٴ� �� ��ǻ� PreRender ���� ���Ӹ�� �ƴϸ� �ٷ�� ��ٷο� �� ����. 
	// ��� ���� �������δ�..
	check(Cast<AB2PreRenderGameMode>(OwnerGameMode));

	if (bLightEnvironmentSetupForDynDirLight)
	{
		if (PreRenderDirLight && PreRenderDirLight->GetLightComponent())
		{
			PreRenderDirLight->GetLightComponent()->bAffectsWorld = false;
			// ������ ��ġ�� ���� ���� ������ Destroy �� ���� ����.. �׷��� UnregisterComponent �� �� �־�� �ٷ� �ٸ� directional light �� �� �� ���������� Ÿ�̹� �� ������ ��������.
			PreRenderDirLight->GetLightComponent()->UnregisterComponent();
			PreRenderDirLight->GetLightComponent()->Deactivate();
			PreRenderDirLight->GetLightComponent()->MarkRenderStateDirty();

			PreRenderDirLight = nullptr;
		}

		bLightEnvironmentSetupForDynDirLight = false;
	}
}
ADirectionalLight* UB2PreRenderer::FindDirectionalLighOfMobility(EComponentMobility::Type LightMobility)
{
	// ���忡 ��ġ�� DirectionalLight �� ã�� ��. DirectionalLight �� �ʿ��� ������ �н����� ��ġ�� �� ���� ã�ƺ��� ������ spawn ��Ű����

	UWorld* TheWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : GetWorld();
	for (FActorIterator ActorIt(TheWorld); ActorIt; ++ActorIt)
	{
		ADirectionalLight* ThisDirL = Cast<ADirectionalLight>(*ActorIt);
		if (ThisDirL && ThisDirL->GetLightComponent() && ThisDirL->GetLightComponent()->Mobility == LightMobility)
		{
			return ThisDirL;
		}
	}
	return nullptr;
}

void UB2PreRenderer::CompletelyTurnOffAllLights()
{
	// Ȥ���� �̷� ���¿��� pre-render �� �ʿ�������..
	for (TObjectIterator<ULightComponent> It; It; ++It)
	{
		ULightComponent* ThisLightComp = *It;
		if (ThisLightComp)
		{
			ThisLightComp->bAffectsWorld = false;
			ThisLightComp->MarkRenderStateDirty();
		}
	}
}

ASkeletalMeshActor* UB2PreRenderer::SpawnPreRenderMeshCommon(UWorld* InSpawnWorld)
{
	//if (InSpawnWorld)
	//{
	//	FActorSpawnParameters SpawnInfo;
	//	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//	SpawnInfo.ObjectFlags |= RF_Transient;

	//	FTransform SpawnTransform;
	//	SpawnTransform.SetLocation(GetPreRenderDummyDesiredLocation(InSpawnWorld));

	//	ASkeletalMeshActor* SpawnedSKActor = InSpawnWorld->SpawnActor<ASkeletalMeshActor>(ASkeletalMeshActor::StaticClass(), SpawnTransform, SpawnInfo);
	//	if (SpawnedSKActor) { // �������� �� ���� ��ģ �ڸ��� �ٸ� �͵鵵 ������ spawn �� ���� collision �� Ȯ���� ������.
	//		SpawnedSKActor->SetActorEnableCollision(false);
	//		// Pre-render �� Ȯ���ϰ� �ǵ��� �ϱ� ���� ���������忡�� �ø����� �ʵ��� ����. �߰� ���̴� Ÿ�Կ� ���� ó���� ��.
	//		SpawnedSKActor->GetSkeletalMeshComponent()->SetExcessiveRenderProcessing(true);
	//	}
	//	return SpawnedSKActor;
	//}
	return NULL;
}


FVector UB2PreRenderer::GetPreRenderDummyDesiredLocation(UObject* WorldContextObject)
{
	//FVector ViewCenterWorldPos = UB2UnitedWidgetBase::GetWorldPosBeneathScreen(UGameplayStatics::GetLocalPlayerController(WorldContextObject), FVector2D(0.5f, 0.5f), PreRenderSpawnDistFromView);
	//// ��� �̴� �ָ� ����߷� ���� ȭ�� �������� ���� �������� �ſ��µ� ������ Pre-render �� ��ģ���� ������ ��Ƹ��� ���� TimeScale �� �۰� �ٰŴ� �׷� �ǹ̴� ���� ���� ��.
	//return bPreRenderAtFarAway ? (ViewCenterWorldPos + FVector(0.0f, 0.0f, -10000.0f)) : ViewCenterWorldPos;
	return FVector(0.0f, 0.0f, -10000.0f);
}

UParticleSystemComponent* UB2PreRenderer::SpawnDummyPSCForPreRender(UParticleSystem* InPS, USceneComponent* InRootComp)
{
	if (!InPS || !InRootComp)
	{
		return NULL;
	}

	//// UGameplayStatics::SpawnEmitterAttached ���ϱ� ���� �Ｎ���� SceneProxy �� ������ �ʾƼ� �� �ٸ� ������� spawn ��Ŵ
	//// �׷��ٰ� �Ϲ����� �����ڵ忡�� UGameplayStatics::SpawnEmitterAttached �� ���� �� ������ ����Ű���� �ʰ�, ���� ���⼱ �ٷ� SceneProxy �� ���� ��ȣ�� ��������

	//UParticleSystemComponent* CreatedComp = NewObject<UParticleSystemComponent>(InRootComp->GetOwner(), NAME_None, RF_Transient);
	//if (CreatedComp)
	//{
	//	CreatedComp->SetTemplate(InPS);
	//	CreatedComp->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	//	CreatedComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//	CreatedComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	//	CreatedComp->AttachToComponent(InRootComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	//	CreatedComp->RegisterComponent();
	//	CreatedComp->Activate(true);

	//	// Pre-render ���� �� delay �ִ��� ���� ���� ������ �ǵ��� �Ѵ�.
	//	// ��ƼŬ �ý��� ���� ���� �ǵ帮�� ���ϴ� �ִ��� �̷��Զ�..
	//	CreatedComp->EmitterDelay = 0.0f;
	//	for (FParticleEmitterInstance* EMI : CreatedComp->EmitterInstances)
	//	{
	//		if (EMI){
	//			EMI->SetupEmitterDuration();
	//			EMI->CurrentDelay = 0.0f;
	//		}
	//	}

	//	CreatedComp->SetExcessiveRenderProcessing(true); // ���������� InitView ������ SceneVisibility �� ����.

	//	return CreatedComp;
	//}
	return NULL;
}

FB2GMPreRenderDummyInfo& UB2PreRenderer::GetExtraPrerenderDummyInfo()
{
	// PCClass �� NPCClass ������ pre-render �� �� ������ �ش� Ŭ������ ����� pre-render dummy Actor �� ���� �ʿ��� ������Ʈ���� �ű⿡ ��������
	// �׷��� ���� ���� �̸� ������� ���� ������ �װ� ����ϴ��� �ƴϸ� ���̸� �ϳ� spawn ��Ų��.	
	if (AllPreRenderDummy.Num() == 0)
	{
		GetAdditionalPrerenderDummyInfo();
	}
	check(AllPreRenderDummy.Num() > 0);
	return AllPreRenderDummy[0];
}
ASkeletalMeshActor* UB2PreRenderer::GetExtraPrerenderDummySKActor()
{
	FB2GMPreRenderDummyInfo& ExtraDummyInfo = GetExtraPrerenderDummyInfo();
	check(ExtraDummyInfo.PreRenderDummy);
	return ExtraDummyInfo.PreRenderDummy;
}
FB2GMPreRenderDummyInfo& UB2PreRenderer::GetAdditionalPrerenderDummyInfo()
{
	FB2GMPreRenderDummyInfo NewExtraDummyInfo;
	// OwnerGameMode �� GetWorld �� �� Ȯ���� �� �ѵ�..
	NewExtraDummyInfo.PreRenderDummy = SpawnPreRenderMeshCommon(OwnerGameMode ? OwnerGameMode->GetWorld() : GetWorld());
	// Dummy �� ��� Ŭ������ ���� �⺻���� �ذ�ٰ��� ����� ��
	SetupDummyMobForPreRender(ENPCClass::ENC_Mob_Skell_Sword, ENPCClassVariation::ENCV_Normal, NULL, NewExtraDummyInfo.PreRenderDummy);
	int32 AddedIndex = AllPreRenderDummy.Add(NewExtraDummyInfo);
	return AllPreRenderDummy[AddedIndex];
}
ASkeletalMeshActor* UB2PreRenderer::GetAdditionalPrerenderDummySKActor()
{
	FB2GMPreRenderDummyInfo& AdditionalDummyInfo = GetAdditionalPrerenderDummyInfo();
	check(AdditionalDummyInfo.PreRenderDummy);
	return AdditionalDummyInfo.PreRenderDummy;
}

void UB2PreRenderer::SetupDummyParticleSystemsForPreRender(ASkeletalMeshActor* InDummySKActor)
{
	//if (!InDummySKActor) {
	//	return;
	//}
	//// AnimBP �� AnimNotify ���ؼ� ParticleSystem ���� �ִ��� �ܾ������.
	//TMap<FName, UParticleSystem*> GatheredPS;
	//TryGatherAllBoundPSFromSKComp(InDummySKActor->GetSkeletalMeshComponent(), GatheredPS);

	//for (TMap<FName, UParticleSystem*>::TConstIterator PSIt(GatheredPS); PSIt; ++PSIt)
	//{
	//	UParticleSystem* ThisPS = PSIt.Value();
	//	// DummySKActor �� attach ��Ű�� ���Ĵ� DummySKActor�� ��Ʈ��
	//	UParticleSystemComponent* ThisSpawnedFxComp = SpawnDummyPSCForPreRender(ThisPS, InDummySKActor->GetRootComponent());
	//}
}

void UB2PreRenderer::SetupDummyMobForPreRender(ENPCClass InNPCClass, ENPCClassVariation InVari, ABladeIICharacter* InOwnerChar, ASkeletalMeshActor* InDummySKActorForRender)
{
	if (InNPCClass == ENPCClass::ENC_End || !InDummySKActorForRender || !InDummySKActorForRender->GetSkeletalMeshComponent())
	{
		return;
	}

	UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox(OwnerGameMode);
	// MobClassInfo �� Async �ε��� ��׶���� ����ϰ� ���� ��Ȳ�̶�� �̰��� ��ȿȭ �� ��. �׳� Async �ε��� ��Ƽ�ھ� Ȱ�븸 ����ϰڴٸ� �𸣰�����.
	UB2NPCSingleClassInfo* ThisClassInfo = MobInfoBox ? MobInfoBox->GetNPCSingleClassInfo(InNPCClass, InVari) : nullptr;
	if (!ThisClassInfo)
	{
		return;
	}

	// ClassInfo ���� Dummy pre-render �� �ʼ����� ������ ����. Attach �ϴ� ���ҽ� �����鵵 ������ �״� ���� �� �ƴ�..
	InDummySKActorForRender->GetSkeletalMeshComponent()->SetSkeletalMesh(ThisClassInfo->MeshAssetOverride);
	if (ThisClassInfo->AnimBPOverride)
	{
		InDummySKActorForRender->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		InDummySKActorForRender->GetSkeletalMeshComponent()->SetAnimInstanceClass(ThisClassInfo->AnimBPOverride);
	}
}

void UB2PreRenderer::SetupDummyPCMeshForPreRender(EPCClass InCharClass, ABladeIIPlayer* InOwnerPlayer, ASkeletalMeshActor* InDummySKActorForRender, const TArray<FB2Item>& InEquippedItems, const FB2Wing* InWingData)
{
	if (InCharClass == EPCClass::EPC_End || !InDummySKActorForRender || !InDummySKActorForRender->GetSkeletalMeshComponent())
	{
		return;
	}

	//// PCClassInfo �� Async �ε��� ��׶���� ����ϰ� ���� ��Ȳ�̶�� �̰��� ��ȿȭ �� ��. �׳� Async �ε��� ��Ƽ�ھ� Ȱ�븸 ����ϰڴٸ� �𸣰�����.
	//UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(OwnerGameMode);
	//UB2PCClassInfo* ThisClassInfo = PCBox ? PCBox->GetSingleClassInfo(InCharClass) : NULL;
	//if (!ThisClassInfo) {
	//	return;
	//}

	//USkeletalMesh* PrebuiltMeshOrResult = NULL;
	//{ // PreRender dummy �����δ� �Ƹ��� InOwnerPlayer �� �� ���� �Ŷ� ��Ʈ��ũ ��� ������ ��� Ȯ�� �� ������ ���� �������� �� �� �ִ�.
	//  //PrebuiltMeshOrResult = OwnerGameMode->GetCachedCompositeMesh(InCharClass, InOwnerPlayer, InEquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, false);
	//}

	//// �κ񿡼��� InGameOnlyInfo �� �������� �ʵ��� �Ѵ�. �������� �޸� ����.
	//UB2InGameOnlyPCClassInfo* InGameOnlyInfo = (GetB2GameModeType(this) != EB2GameMode::Lobby) ? ThisClassInfo->GetInGameOnlyInfo(this) : NULL;

	//if (UB2PCClassInfo::SetupSKCompForParts(InCharClass, InDummySKActorForRender, InDummySKActorForRender->GetSkeletalMeshComponent(), PrebuiltMeshOrResult,
	//	ThisClassInfo->BaseMeshAsset, ThisClassInfo->DefaultPartsInfo, InEquippedItems, InWingData, (InGameOnlyInfo ? InGameOnlyInfo->AnimBPOverride : NULL), 
	//	false, false))
	//{
	//	{// PreRender dummy �����δ� �Ƹ��� InOwnerPlayer �� �� ���� �Ŷ� ��Ʈ��ũ ��� ������ ��� Ȯ�� �� ������ ���� ĳ���ϰ� �� �� �ִ�.
	//	 //OwnerGameMode->SetCachedCompositeMesh(InCharClass, InOwnerPlayer, PrebuiltMeshOrResult, InEquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, false);
	//	}
	//}
}

void UB2PreRenderer::UpdateForPreRenderObjects(bool bForceContinue /*= false*/) // Pre-render ���� ���� Tick �� �ش�
{
	checkSlow(OwnerGameMode && OwnerGameMode->IsInPreRenderPhase());
	//
	//	if (bPendingForPreRenderGM)
	//	{
	//		if (StartBladeIIPreRenderGame(OwnerGameMode)) // �κ񿡼� ���� pre-render �ϴ� ��� ���� ���� �ε��ϴ� ���
	//		{
	//			bPendingForPreRenderGM = false;
	//			return;
	//		}
	//	}
	//
	//	if (bCanDoOverallCount)
	//	{
	//		++OverallTickCount;
	//	}
	//	// �̰� true �� ���� ������ ���� ���� �� �ִ��� ����. ��� �� �������� �ȴٴ� ������ ������.. 
	//	const bool bOverallCountDone = (OverallTickCount >= OverallPreRenderLimit);
	//
	//	// �ƹ� ���� ���� �¾� ���ķ� Ư�� ƽ ������ Pre-render �� ���� ����. �̰� ������ safety �� ����.
	//	++TickCountFromSetup;
	//	// ������ ���⿡ �ɸ��� �� �ٶ����� ��Ȳ�� �ƴϴ�.
	//	const bool bHardLimitReached = (TickCountFromSetup >= PreRenderHardLimit);
	//
	//	const bool bShouldFinishDummiesBySomeLimit = (bOverallCountDone || bHardLimitReached) && !bForceContinue;
	//
	//	const FVector DummyDesiredLocation = GetPreRenderDummyDesiredLocation(OwnerGameMode); // ���� view ���� �������� �Ѵٸ� �Ź� �ٲ� ����.
	//
	//#if !UE_BUILD_SHIPPING
	//	int32 FinishBySomeLimitCount = 0;
	//	FString FinishBySomeLimitMsg = TEXT("");
	//#endif
	//
	//	// PreRender �� �� ƽ ���鼭 �� ��.
	//	for (int32 PRDI = 0; PRDI < AllPreRenderDummy.Num(); ++PRDI)
	//	{
	//		FB2GMPreRenderDummyInfo& ThisDummyInfo = AllPreRenderDummy[PRDI];
	//		// ��ü�� �� SkeletalMeshComponent �������� ���� �Ҳ��� �̰� �Ӹ��� �ƴ϶� ���⿡ ����Ʈ���� �ܶ� �پ� ���� �� �ִ�.
	//		USkeletalMeshComponent* ThisDummySKComp = ThisDummyInfo.PreRenderDummy->GetSkeletalMeshComponent();
	//
	//		// ���� ī��Ʈ �̻� ������ �Ǹ� ����.. 
	//		if (ThisDummyInfo.PreRenderDummy && !ThisDummyInfo.PreRenderDummy->bHidden)
	//		{
	//			if (ThisDummySKComp->LastRenderTime > 0.0f)
	//			{
	//				bCanDoOverallCount = true; // OverallCount �� ��� �ϳ��� ������ �� ���Ŀ� �����Ѵ�. 
	//			}
	//
	//			const bool bThisDummyDoneForNormalCondition = ThisDummyInfo.UpdateDummy(bForceContinue);
	//
	//			if ((bThisDummyDoneForNormalCondition || bShouldFinishDummiesBySomeLimit) && !bForceContinue)
	//			{
	//#if !UE_BUILD_SHIPPING // ���� ������ ��ȣ�� �� �Դµ� �׳� ������ ���� ������ �� �α��� ��
	//				if (!bThisDummyDoneForNormalCondition)
	//				{
	//					++FinishBySomeLimitCount;
	//					// PC ������ ������ �� ���� �ǹ��ִ� �̸��� �� ����������..
	//					FinishBySomeLimitMsg += FString::Printf(TEXT("%d. Root SkeletalMesh : %s\r\n"), FinishBySomeLimitCount, ThisDummySKComp->SkeletalMesh ? (*ThisDummySKComp->SkeletalMesh->GetName()) : TEXT("NULL"));
	//				}
	//#endif
	//
	//				ThisDummyInfo.PreRenderDummy->SetActorHiddenInGame(true);
	//				ThisDummySKComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	//				// ��ƼŬ �ý��۵� ���� �̻� �ı��� �ִ� �� ���ڴ�. �迭�� ����.. �׷��� �� ���ư����� Ȯ���ϱ⵵ ����.
	//				ThisDummyInfo.PreRenderDummy->Destroy();
	//				ThisDummyInfo.PreRenderDummy = NULL;
	//				AllPreRenderDummy.RemoveAt(PRDI);
	//				--PRDI;
	//			}
	//			else
	//			{ // ���� �������� ������ �ȵǾ��ų� ������ ����ϰų�.. ī�޶� ��ġ�� �ް��� �ٲ�� �� �� ������ �ٽñ� location �¾�.
	//				ThisDummyInfo.PreRenderDummy->SetActorLocation(DummyDesiredLocation);
	//			}
	//		}
	//	}
	//
	//	if (OwnerGameMode->GetPreRenderPhase() == EPreRenderPhase::EPRP_Selected && ProgressStepCountForSelectedPhase < MaxPreRenderCount)
	//	{ // �Ϲ� PreRender Selected phase �� ���൵ ����.
	//		// �⺻ Tick ���Ѹ�ŭ�� ���⼭ ī��Ʈ �Ѵ�. ��Ȯ������ ������ �׷��� ��Ȯ�� �ʿ������ ����.
	//		StepPreRenderScreenProgress();
	//		++ProgressStepCountForSelectedPhase;
	//	}
	//
	//	FlushRenderingCommands(); // ������ ������.
	//
	//	if (bShouldFinishDummiesBySomeLimit)
	//	{
	//		AllPreRenderDummy.Empty(); // Ȯ�λ��
	//
	//#if !UE_BUILD_SHIPPING
	//		FinishBySomeLimitMsg = FString::Printf(TEXT("[PreRender] %d Pre-render info are done by some limitation while not all rendered.\r\n"), FinishBySomeLimitCount) + FinishBySomeLimitMsg;
	//		UE_LOG(LogBladeII, Display, TEXT("%s"), *FinishBySomeLimitMsg);
	//#endif
	//	}
}

void UB2PreRenderer::SetupDefaultPreRenderScreen()
{
	// Ư�� Ÿ�̹��� �ε� ��ũ�� ����̾��µ� �̷� ������ ��Ȱ���ϴ±� ��
	//if (OwnerGameMode)
	//{
	//	check(!Cast<AB2PreRenderGameMode>(OwnerGameMode)); // �Ϲ� ���Ӹ�� ��. PreRender ���� ���Ӹ��� ������ ������� ������ ����
	//	OwnerGameMode->OpenPreLoadingScreen(EPreLoadingScreenType::PLST_PreRender_Default);
	//	CachedPreLoadingScreen = OwnerGameMode->GetCurrCreatedPreLoadingScreen(EPreLoadingScreenType::PLST_PreRender_Default);
	//	check(CachedPreLoadingScreen);
	//	if (CachedPreLoadingScreen)
	//	{
	//		// �Ϲ� PreRender ������ ���൵ �¾�.
	//		CachedPreLoadingScreen->SetupPreRenderProgress(false, false, true);

	//		EstimatedTotalProgressStep = 0;
	//		// Selected phase �� ���� ó�� ����Ʈ�� ���� ��� �⺻ ƽ ���Ѹ�ŭ ����
	//		if (AllPreRenderDummy.Num() > 0)
	//		{
	//			EstimatedTotalProgressStep += MaxPreRenderCount;
	//		}
	//		// AllPrim Phase ƽ ���Ѹ�ŭ�� ���� �߰�.
	//		if (bPreRenderAllPrimitives && !IsCurrentWorldLevelPreRendered())
	//		{
	//			EstimatedTotalProgressStep += GetTotalPreRenderAllPrimTickLimit();
	//		}

	//		CurrPrerenderProgressStep = 0;
	//		ProgressStepCountForSelectedPhase = 0;
	//		// �� ������ ���� ���� 1.0 ���൵�� ä��� ���� ���� �ϳ��� ��.
	//		EstimatedTotalProgressStep = FMath::Max(EstimatedTotalProgressStep - 1, 0);
	//	}
	//}
}

void UB2PreRenderer::StepPreRenderScreenProgress()
{
	check(OwnerGameMode && !Cast<AB2PreRenderGameMode>(OwnerGameMode));

	//if (CachedPreLoadingScreen && EstimatedTotalProgressStep > 0)
	//{
	//	// �Ϲ� �ʷε� progress �� PreRenderGameMode progress ���ٴ� �ܼ��ϰ� �̰� �� �Ҷ����� �ѽ��� �� ����.
	//	CurrPrerenderProgressStep = FMath::Min(CurrPrerenderProgressStep + 1, EstimatedTotalProgressStep);
	//	// ���� ������ �ε巴�� �귯������ ���� ���̹Ƿ� �ѹ� ������ �� �ٷ� Ÿ�� ���� ä�쵵�� �Ѵ�.
	//	CachedPreLoadingScreen->UpdatePreRenderProgressScale((float)CurrPrerenderProgressStep / (float)EstimatedTotalProgressStep, true);
	//}
}

bool UB2PreRenderer::ShouldRenderForAllPrimPhase(UPrimitiveComponent* InCompToCheck) const
{
	// PreRenderAll ������ ���忡 �� �ִ� ��� Primitive ������Ʈ���� ������ �������� �����µ� �̰ͱ��� �� ������ �� �־ 
	// ���⼱ ������ �߿��� material ���������� üũ�� �� �Ѵ�.
	if (MaterialBasedCompleteState.ArePrimCompAllMaterialsPreRendered(InCompToCheck))
	{
		return false;
	}

	// ���� Ÿ�� ���͸��� ���������� �ʾƼ�..
	return true; // Cast<UMeshComponent>(InCompToCheck) || Cast<UParticleSystemComponent>(InCompToCheck);
}

bool UB2PreRenderer::SetupPreRenderAllPrimPhase()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2PreRenderer::SetupPreRenderAllPrimPhase"));

	checkSlow(OwnerGameMode && OwnerGameMode->GetPreRenderPhase() == EPreRenderPhase::EPRP_Selected);

	PreRenderAllPrimVisbilityCache.Empty();
	PreRenderAllActorVisbilityCache.Empty();
	PreRenderAllPLStateCache.Empty();

	// ��� �� ����� ��ü�� ���輺�� �ִµ� ���⼭ �����ϰ� ������ ������ ���� �ڵ忡 ���� Ư�� ���ͳ� ������Ʈ�� visibility ���� ���ϰ� �Ǹ� �װ� ���������� �� ���̱� ����.
	// ��ü�� �̰� ����Ǵ� ������ ���� ������ �������� �Ŷ� ��Ƽ�� Ʈ���� ���� ���� �ܿ��� �׷� ���� ���� ���̰� Time Delta �� �ſ� �۰� �����ϸ� �Ͽ� ���ɼ��� ����� ���� �� ��.
//
//	if (!IsCurrentWorldLevelPreRendered()
//#if PLATFORM_IOS
//		&& ShouldDoPreRenderForIOS_AllPrim()
//#endif
//		)
//	{
//		for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
//		{
//			UPrimitiveComponent* ThisPrimComp = *It;
//			if (ThisPrimComp && ShouldRenderForAllPrimPhase(ThisPrimComp))
//			{
//				// ���� ������ �� ���� �ִµ� ��ĩ �߸� �ٷ�� ���� ���̴� ��Ȳ�� ��¤�� �� ����. �׷��� ���� ���� ����.
//				FPreRenderPrimCompVisibilityCache ThisPrimVisibleState;
//				ThisPrimVisibleState.bTotalVisible = ThisPrimComp->IsVisible();
//				ThisPrimVisibleState.bVisible = ThisPrimComp->bVisible;
//				ThisPrimVisibleState.bHiddenInGame = ThisPrimComp->bHiddenInGame;
//				PreRenderAllPrimVisbilityCache.Add(ThisPrimComp, ThisPrimVisibleState);
//
//				ThisPrimComp->SetVisibility(true);
//				ThisPrimComp->SetHiddenInGame(false);
//				ThisPrimComp->SetExcessiveRenderProcessing(true);
//
//				// ������ �� ������ ������ �Ϸᰡ �Ǳ� �� ���� ���뼭 ��ũ ��
//				MaterialBasedCompleteState.MarkPrimCompMaterials(ThisPrimComp);
//			}
//		}
//
//		// Dynamic(Movable)PointLight �� ������ �����ϱ� ������
//		// �����̸� ù ���� �����ϸ鼭 ������ ���� PointLight �� ���� ��. �װ͵� ����. ���´� �����ϰ�..
//		for (TObjectIterator<UPointLightComponent> It; It; ++It)
//		{
//			UPointLightComponent* ThisPL = *It;
//			if (ThisPL && ThisPL->Mobility == EComponentMobility::Movable && ThisPL->bAffectsWorld)
//			{
//				AActor* PLOwnerActor = ThisPL->GetOwner();
//				ABladeIIPlayer* PlayerOwner = Cast<ABladeIIPlayer>(PLOwnerActor);
//				APointLight* PLActorOwner = Cast<APointLight>(PLOwnerActor);
//								
//				if (
//					PLOwnerActor && 
//					!PlayerOwner &&  // PlayerOwner �� ������ ������ �����ϴ� �÷��̾� ��ܿ� �ٴ� ����Ʈ�̴� ����
//					!(PLActorOwner && PLActorOwner == PreRenderDummyPointLight) // �� ��쵵 ���⼭ Ư���� �����ϴ� ���̴� ����
//					)
//				{
//					// �̰� ������ �������� �ȴٴ� �� ����� ���� ��� ���д�.
//					PreRenderAllPLStateCache.Add(ThisPL, true);
//					ThisPL->bAffectsWorld = false;
//					ThisPL->MarkRenderStateDirty();
//				}
//
//			}
//		}
//
//		// Owner Actor �� �ִ� PrimitiveComponent ���� Actor �� ������ ������ �ҿ��� ���� ��. ���� ó��.
//		UWorld* TheWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : nullptr;
//		if (TheWorld)
//		{
//			for (FActorIterator ActorIt(TheWorld); ActorIt; ++ActorIt)
//			{
//				AActor* ThisActor = Cast<AActor>(*ActorIt);
//				if (ThisActor)
//				{
//					bool bIsVisible = !(ThisActor->bHidden);
//					PreRenderAllActorVisbilityCache.Add(ThisActor, bIsVisible);
//					ThisActor->SetActorHiddenInGame(false);
//				}
//			}
//		}
//
//		bHadDoneAnyPreRender = true; // ���� ������ �� �� �ƴ����� �� �÷��� ���� �� ���뿡�� true ��ũ�ص� ���ڴ�.
//	}
//
//	FlushRenderingCommands();
//	PreRenderAllPrimTickCount = 0;

	return (PreRenderAllPrimVisbilityCache.Num() > 0 || PreRenderAllActorVisbilityCache.Num() > 0);
}

bool UB2PreRenderer::CheckAndFinishPreRenderAllPrimPhase()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2PreRenderer::FinishPreRenderAllPrimPhase"));

	checkSlow(OwnerGameMode && OwnerGameMode->GetPreRenderPhase() == EPreRenderPhase::EPRP_AllPrim);

	//PreRenderAllPrimTickCount++;

	//StepPreRenderScreenProgress(); // Pre-render ���൵.

	//// Pre-render ���� ���ư��� ������ ���� ���� �ϴµ� 
	//// ���� Ʃ�丮�� ù ���⿡�� ī�޶��ũ�� ����Ǹ鼭 ���̴� ������ ������ ���ؼ� �̷� ���� �ϰ� �Ǿ�����
	//// ����� ȿ�����̾�� ���� �Ϲ������� ������ ��.
	//if (OwnerGameMode && GetTotalPreRenderAllPrimTickLimit() > 0)
	//{
	//	AB2StageEventDirector* CurrPlayingEvent = OwnerGameMode->GetCurrentPlayingStageEventDirector();
	//	if (CurrPlayingEvent)
	//	{ // ȭ���� ������ �÷����ϹǷ� �������� ������ �� ������ ���尡 �÷��� �� �� �ִµ�
	//		// PreRender �߿��� ���� Ʈ�� �� ���� (TemporaryDisableTracksForPreRender)
	//		CurrPlayingEvent->ForceSetMatineePlayingPosScale(
	//			(float)PreRenderAllPrimTickCount / (float)(GetTotalPreRenderAllPrimTickLimit()));
	//	}
	//}

	//if (PreRenderAllPrimTickCount < GetTotalPreRenderAllPrimTickLimit())
	//{
	//	// �̰� LastRenderTime ���� �� �� ���� �� ������ ƽ ���� ����. ��, �̰� ���ư��� ���� ���͵��� Visibility �� ������ �ʴ´ٴ� ������ �����Ƿ� �ʹ� ���� ���Ƽ� �� ��.
	//	// LastRenderTime ���� ���� �������� �Ǿ����� ������ ���� �������� ���� �ʴ� ������ ������Ʈ���� üũ��.

	//	// ������ ���� ����Ʈ ȯ�� �ٲ㼭�� ������.
	//	if (PreRenderAllPrimTickCount >= PreRenderAllPrimLit2PassTickLimit + PreRenderAllPrimLit1PassTickLimit)
	//	{// �� �� ��.
	//		ConditionalRestoreFromPreRenderLightEnvForDynPL(false);
	//		// �÷��̾� ĳ���� Head Light �� ���� �ݰ��� ���� ���� ��Ƽ� ��ǻ� �������� �Ѵ�. �̷��� �ص� ���⿡�� ������ PointLight �� ����Ѵٸ� PointLight �� �ϳ��� �ְ�����..
	//		MinimizePlayerPointLight();
	//	}
	//	else if (PreRenderAllPrimTickCount >= PreRenderAllPrimLit2PassTickLimit)
	//	{
	//		// 2 ���� dynamic point light �� �ϳ��� ��
	//		ConditionalRestoreFromPreRenderLightEnvForDynPL(false);
	//	}
	//	return false; 
	//}

	//// ��� ���̵��� �ߴ� visibility �� ���� ������ ���������ش�.

	//for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
	//{
	//	UPrimitiveComponent* ThisPrimComp = *It;
	//	if (ThisPrimComp)
	//	{
	//		// ���� ������ �� ���� �ִµ� ��ĩ �߸� �ٷ�� ���� ���̴� ��Ȳ�� ��¤�� �� ����.
	//		FPreRenderPrimCompVisibilityCache *ThisPrimVisibleState = PreRenderAllPrimVisbilityCache.Find(ThisPrimComp);
	//		if (ThisPrimVisibleState)
	//		{
	//			// �������� �������� ���� ������ �ΰ��� �ִٺ��� ���� ���°� ���̴� �� �ؼ� Ȯ���� ó���Ϸ���..
	//			if (!ThisPrimVisibleState->bTotalVisible) { // ������ � �����ε� visible ���� �ʾҴٰ� ���Դ� �ֵ鸸 ���� ����..
	//				ThisPrimComp->SetVisibility(ThisPrimVisibleState->bVisible);
	//				ThisPrimComp->SetHiddenInGame(ThisPrimVisibleState->bHiddenInGame);
	//			}
	//		}
	//		ThisPrimComp->SetExcessiveRenderProcessing(false); // �̰� ������ ������ ������ �׳� �θ� ���ɿ� �ǿ����� ��ġ�� �Ǵ� ������ ����.
	//	}
	//}

	//for (TMap<UPointLightComponent*, bool>::TIterator CachedPLIt(PreRenderAllPLStateCache); CachedPLIt; ++CachedPLIt)
	//{
	//	UPointLightComponent* ThisPL = CachedPLIt.Key();
	//	if (ThisPL)
	//	{
	//		ThisPL->bAffectsWorld = CachedPLIt.Value(); // �Ƹ��� true ������
	//		ThisPL->MarkRenderStateDirty();
	//	}
	//}

	//check(OwnerGameMode);
	//UWorld* TheWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : nullptr;
	//if (TheWorld)
	//{
	//	for (FActorIterator ActorIt(TheWorld); ActorIt; ++ActorIt)
	//	{
	//		AActor* ThisActor = Cast<AActor>(*ActorIt);
	//		if (ThisActor && ThisActor->IsValidObj())
	//		{
	//			bool* CachedVisibilityValue = PreRenderAllActorVisbilityCache.Find(ThisActor);
	//			if (CachedVisibilityValue && !(*CachedVisibilityValue)) {
	//				ThisActor->SetActorHiddenInGame(!(*CachedVisibilityValue)); // ���� �ִ� ��츸 �ٽ� ����
	//			}
	//		}
	//	}
	//}

	//PreRenderAllPrimVisbilityCache.Empty();
	//PreRenderAllActorVisbilityCache.Empty();
	//PreRenderAllPLStateCache.Empty();

	//FlushRenderingCommands();

	return true;
}

void UB2PreRenderer::OnPreRenderComplete()
{
	ConditionalRestoreFromPreRenderLightEnvForDynPL(true); // ���� �������� �ǵ���
	//CachedExtraPreRenderPS.Empty();
	//CachedExtraPreRenderSKMesh.Empty();
	//CachedExtraPreRenderNPCClassInfo.Empty();
	//CachedPreLoadingScreen = nullptr; // �̰� Close �� GameMode �ʼ�
	//
	//// ExcessiveRenderProcessing ���� Ȯ����.. AllPrimPhase ���� �� �������ִ� �ִ� �ɷδ� ������ �� �ִµ� 
	//// AllPrimPhase ���� �ٸ� pre-render �� �ϴ� ��쵵 �ְ�, �׷� ��쵵 component �� �ѵ��� �������� �� �ֱ� ����.
	//for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
	//{
	//	UPrimitiveComponent* ThisPrimComp = *It;
	//	if (ThisPrimComp)
	//	{
	//		ThisPrimComp->SetExcessiveRenderProcessing(false);
	//	}
	//}
	//
	//// �̹��� ������ pre-render �� �ߴٸ�.. ���̴� ĳ���� ������ �ǵ��� �Ѵ�.
	//if (bHadDoneAnyPreRender)
	//{
	//	EnsureSaveCaches();
	//}
}

void UB2PreRenderer::EnsureSaveCaches()
{
	//#if PLATFORM_ANDROID // Vulkan �� ����ε�.. �̰� �⺻�����δ� �� ����ÿ��� ������Ʈ �ǵ��� �� ���Ƽ� Ȥ���� ũ������ ������ PreRender ���´� ��������� ���̴� ĳ���� ���������� ��. �̷� �Ƚ��� ���¸� �����ϰ��� ���뼭�� ���̴� ĳ�� ������Ʈ.
	//	if (FAndroidMisc::ShouldUseVulkan())
	//	{
	//		const auto& OnSavePipelineCacheCallback = FAndroidMisc::GetOnSavePipelineCache();
	//		if (OnSavePipelineCacheCallback)
	//		{
	//			OnSavePipelineCacheCallback();
	//		}
	//	}
	//#endif
}

void UB2PreRenderer::MarkCurrentWorldLevelPreRendered()
{
	UWorld* CurrentWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : nullptr;
	TotalCompleteState.MarkWorldLevelPreRendered(CurrentWorld);
}

bool UB2PreRenderer::IsCurrentWorldLevelPreRendered() const
{
	UWorld* CurrentWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : nullptr;
	return TotalCompleteState.IsWorldLevelPreRendered(CurrentWorld);
}

void UB2PreRenderer::MarkPreRenderLevelGoneThrough()
{
	TotalCompleteState.MarkPreRenderLevelGoneThrough();
}
bool UB2PreRenderer::IsPreRenderLevelGoneThrough()
{
	return TotalCompleteState.IsPreRenderLevelGoneThrough();
}

///////////////////////////////////////////////////////////////////////////////////
// PreRenderGameMode �� ���� �������̽���.. �ٺ����� ����� ���� �⺻ SetupPreRenderObjects ���� �ϴ� �Ŷ� ��������. �ణ �ٸ��� ����� ��.

bool UB2PreRenderer::IsInPreRenderGameMode() const
{
	return OwnerGameMode && Cast<AB2PreRenderGameMode>(OwnerGameMode);
}
void UB2PreRenderer::InitForPreRenderGameMode(AB2PreRenderGameMode* InGM)
{
	OwnerGameMode = InGM;
	check(OwnerGameMode);

}
void UB2PreRenderer::SetupForPreRenderGM_PCClassPass(EPCClass InPCClass, bool bIncludeSkillAnims)
{
	checkSlow(AllPreRenderDummy.Num() == 0); // �� �н� ���� PreRenderDummy �� ��Ȱ���ؼ� ����ϴ� ���� �ȴٸ� �� check �� ���� �ʰ���..

	// Pre-render ���� ���� ���� ��忡�� ������ ���̹Ƿ� ForceSetup �� �ش�.
	// ���� �׷��� ���Ѵٰ� �ؼ� ������ �� ��������.. Ȥ���� ���̴� ĳ���� ���ư��ٰų� �ϴ� ��Ȳ���� �� PreRenderGameMode �� �ݺ��ؼ� ������ ���� ������ �����ؼ� ��
	ConditionalSetupPreRenderObjectForPCClass(InPCClass, true);

	if (bIncludeSkillAnims)
	{
		// ����� Ŭ������ SkillAnim ��. �� PCClass �� AnimBP �� �� �ִ� ���� �ִϸ��̼ǵ�� �ٺ������� �ٸ� �� ����. ���� �ε��Ǵ� ������ ���� �鿡�� ���� ���� �����ϴ� ��.
		const TArray<FCombinedPCSkillAnimID> SkillAnimsToPreLoad = ABladeIIGameMode::GetAllPCSkillAnimsOfClass(InPCClass);
		ConditionalSetupPreRenderObjectForPCSkillAnims(SkillAnimsToPreLoad, true);
	}

	PostPreRenderSeupProcess();
	if (AllPreRenderDummy.Num() > 0)
	{
		bHadDoneAnyPreRender = true; // ���� ������ �� �� �ƴ����� �� �÷��� ���� �� ���뼭 true ��ũ
	}
}

void UB2PreRenderer::SetupForPreRenderGM_PCSkillAnimPass(EPCClass InPCClass)
{
	checkSlow(AllPreRenderDummy.Num() == 0); // �� �н� ���� PreRenderDummy �� ��Ȱ���ؼ� ����ϴ� ���� �ȴٸ� �� check �� ���� �ʰ���..

	const TArray<FCombinedPCSkillAnimID> SkillAnimsToPreLoad = ABladeIIGameMode::GetAllPCSkillAnimsOfClass(InPCClass);
	ConditionalSetupPreRenderObjectForPCSkillAnims(SkillAnimsToPreLoad, true); // ���⵵ ForceSetup

	PostPreRenderSeupProcess();
	if (AllPreRenderDummy.Num() > 0)
	{
		bHadDoneAnyPreRender = true; // ���� ������ �� �� �ƴ����� �� �÷��� ���� �� ���뼭 true ��ũ
	}
}

void UB2PreRenderer::SetupForPreRenderGM_DamageEffectInfo()
{
	checkSlow(AllPreRenderDummy.Num() == 0); // �� �н� ���� PreRenderDummy �� ��Ȱ���ؼ� ����ϴ� ���� �ȴٸ� �� check �� ���� �ʰ���..

	ConditionalSetupPreRenderObjectForDamageEffectInfo(true);

	PostPreRenderSeupProcess();
	if (AllPreRenderDummy.Num() > 0)
	{
		bHadDoneAnyPreRender = true; // ���� ������ �� �� �ƴ����� �� �÷��� ���� �� ���뼭 true ��ũ
	}
}
void UB2PreRenderer::SetupForPreRenderGM_Extra(bool bForNPCClass, bool bForFx, bool bForOther)
{
	//checkSlow(AllPreRenderDummy.Num() == 0); �̰� �ٷ� �տ� �ٸ� PreRender �� �����ϴ� ��� ��ʰ� �־ �ɸ�. ���� �߿��� check �� �ƴ�.

	// ExtraNPCClass �� Other �� Fx ���� ���� �ϴµ� Fx �н� �¾��� ���� ���̰� ���ܼ� ���� �Ϻη� �ϳ� ������ �ʿ䰡 �������� �Ǳ� ����.
	if (bForNPCClass)
	{
		SetupPreRenderObjects_ExtraNPCClass();
	}
	if (bForOther)
	{
		SetupPreRenderObjects_ExtraOthers();
	}
	if (bForFx)
	{
		SetupPreRenderObjects_ExtraFx();
	}

	PostPreRenderSeupProcess();
	if (AllPreRenderDummy.Num() > 0)
	{
		bHadDoneAnyPreRender = true; // ���� ������ �� �� �ƴ����� �� �÷��� ���� �� ���뼭 true ��ũ
	}
}

#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] 
/*
 * iOS ���� ���̴� ������ ������ ���� ���̰� Pre-render �� �޸� �Ҹ� �� ������ �� �� �־ ���� ������ �Ǵ�.
 * ��Ȳ�� ���� �Ϻθ� ���ٰų� �� �� �ִ�.
 */
bool UB2PreRenderer::ShouldDoPreRenderForIOS()
{ // �̰� return true ��� �ؼ� ��� �� �Ȱ��� �ϰ� �Ǵ� �� �ƴϴ�.
	return ShouldDoPreRenderForIOS_Basic() ||
		ShouldDoPreRenderForIOS_AllPrim() ||
		ShouldDoPreRenderForIOS_PreRenderGM();
}
bool UB2PreRenderer::ShouldDoPreRenderForIOS_Basic()
{
	// �̸��� Basic ������ ShouldDoPreRenderForIOS_PreRenderGM �� false �̰� �̰� true �̸� ����� ū �δ����� �Ȱ��� �� ó���ϰ� �� �� �ִ�.
	// ��ǻ� �������� �ִ� �Ű� �ٸ� sub-condition �� true �� ���� �־ �̰� true �� �ϱ�� ����� ���̴�.
	return false;
}
bool UB2PreRenderer::ShouldDoPreRenderForIOS_AllPrim()
{
	return true; // �׽�Ʈ�� �� ���� �̰� ������ �ʿ��� �� ����.
}
bool UB2PreRenderer::ShouldDoPreRenderForIOS_PreRenderGM()
{
	return false;
}
#endif

///////////////////////////////////////////////////////////////////////////////////

int32 UB2PreRenderer::MaxPreRenderCount = 1; // 1���� ũ�� �� ������ ���� ����. 1 Ȥ�� 0.
int32 UB2PreRenderer::OverallPreRenderLimit = 10; // �ϳ��� ó�� �������� �� ���Ŀ� ������ �ֵ� ������ �� ������ ��ٸ��� ƽ ��.
int32 UB2PreRenderer::PreRenderHardLimit = 20;
float UB2PreRenderer::PreRenderSpawnDistFromView = 100.0f;
bool UB2PreRenderer::bPreRenderAtFarAway = false;
bool UB2PreRenderer::bPreRenderAllPrimitives = true;
int32 UB2PreRenderer::PreRenderAllPrimLit2PassTickLimit = 2;
int32 UB2PreRenderer::PreRenderAllPrimLit1PassTickLimit = 2;
int32 UB2PreRenderer::PreRenderAllPrimUnlitPassTickLimit = 2;

void UB2PreRenderer::LoadSettings()
{
	if (GConfig)
	{
		GConfig->GetInt(TEXT("PreRender"), TEXT("MaxPreRenderCount"), MaxPreRenderCount, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("OverallPreRenderLimit"), OverallPreRenderLimit, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderHardLimit"), PreRenderHardLimit, GGameIni);
		GConfig->GetFloat(TEXT("PreRender"), TEXT("PreRenderSpawnDistFromView"), PreRenderSpawnDistFromView, GGameIni);
		GConfig->GetBool(TEXT("PreRender"), TEXT("PreRenderAtFarAway"), bPreRenderAtFarAway, GGameIni);
		GConfig->GetBool(TEXT("PreRender"), TEXT("PreRenderAllPrimitives"), bPreRenderAllPrimitives, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderAllPrimLit2PassTickLimit"), PreRenderAllPrimLit2PassTickLimit, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderAllPrimLit1PassTickLimit"), PreRenderAllPrimLit1PassTickLimit, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderAllPrimUnlitPassTickLimit"), PreRenderAllPrimUnlitPassTickLimit, GGameIni);
	}
}
void UB2PreRenderer::LoadSettings_PreRenderGM()
{
	// Pre-render ���� ���Ӹ�忡�� ������ �ٸ��� ������ ��. Ư�� ����̹Ƿ� �װ� ����
	if (GConfig)
	{
		GConfig->GetInt(TEXT("PreRender"), TEXT("MaxPreRenderCount_PreRenderGM"), MaxPreRenderCount, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("OverallPreRenderLimit_PreRenderGM"), OverallPreRenderLimit, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderHardLimit_PreRenderGM"), PreRenderHardLimit, GGameIni);
	}
}

void UB2PreRenderer::SaveCompleteState()
{
	TotalCompleteState.SaveState();
	GConfig->Flush(false, GB2PreRenderSavedStateIni);
}
void UB2PreRenderer::LoadCompleteState()
{
	TotalCompleteState.LoadState();
}
void UB2PreRenderer::ClearCompleteState()
{
	TotalCompleteState.ClearState(); // ���� �� ����� �ƴϰ� ���� �׽�Ʈ �� ����ε� Ȥ ���̴� ĳ�� ���� ���� ��Ȳ���� �ʿ��� ����� ������? 
	MaterialBasedCompleteState.ClearState();
}