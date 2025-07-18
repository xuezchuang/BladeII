// Fill out your copyright notice in the Description page of Project Settings.
#include "B2PreRenderGameMode.h"
#include "B2PreRenderer.h"

#include "CommonStruct.h"
#include "Runtime/Engine/Public/Engine.h"


#include "Event.h"
//#include "B2PreLoadingScreen.h"
#include "B2GameLoadingProgress.h"
#include "B2DLCFrontGameMode.h"
#include "../BladeII/BladeIIUtil.h"

///////////////////////////////////////////////////////////////////////////////////////////
//
//	B2PreRenderGameMode
//

int32 AB2PreRenderGameMode::GetTotalContentTypePassNum() const
{ 
	// 2 ���� ContentPass �� �����δ� PCClassNum ��ŭ��.
	return (((int32)EPRGMContentTypePass::CTP_End - 3) + (GetMaxPCClassNum() * 2)); 
}

float AB2PreRenderGameMode::GetTotalWeightedPassNum() const
{
	// ����Ʈ �н� �� �ϳ��� ����ġ ���ؼ�..
	return (
		(GetFistLightPassProgressWeight() * (float)GetTotalContentTypePassNum()) +
		((GetTotalLightPassNum() - 1) * (float)GetTotalContentTypePassNum())
		);
}

AB2PreRenderGameMode::AB2PreRenderGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentLightPass = EPRGMLightPass::LP_None;
	CurrentContentTypePass = EPRGMContentTypePass::CTP_None;
	CurrentPCClassPass = EPCClass::EPC_End;
	PreRenderTimeScale = 1.f;
	SinglePassLeastTime = 1.f;
	TimeSinceLastPassBegin = 0.0f;
	WeightedPassSumSoFar = 0.0f;
	InternalLit2PassTickLimit = 3;
	InternalLit1PassTickLimit = 3;
	CurrentPassTickCount = 0;
}

void AB2PreRenderGameMode::PreBeginPlay()
{
	Super::PreBeginPlay();	

	if (GConfig)
	{
		GConfig->GetFloat(TEXT("PreRender"), TEXT("PreRenderGMTimeScale"), PreRenderTimeScale, GGameIni);
		GConfig->GetFloat(TEXT("PreRender"), TEXT("PreRenderGMSinglePassLeastTime"), SinglePassLeastTime, GGameIni);

		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderGMInternalLit2PassTickLimit"), InternalLit2PassTickLimit, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderGMInternalLit1PassTickLimit"), InternalLit1PassTickLimit, GGameIni);
	}
}

void AB2PreRenderGameMode::BeginPlay()
{
	//// ���⼭ ������� ǥ���ϴ� Progress ���� �� PreRender ������ �ε��ϴ� progress ���뼭 ����.
	//B2GMLoadingProgCollector::StepToMax();

	//Super::BeginPlay();
	//
	//// Virtual joystick ���ֱ�.
	//APlayerController* LocalPCon = UGameplayStatics::GetLocalPlayerController(this);
	//if (LocalPCon)
	//	LocalPCon->ActivateTouchInterface(nullptr);
}

void AB2PreRenderGameMode::SetupNextGameLoadingScreen()
{
	if (!AB2DLCFrontGameMode::GetSelectedFrontTitleMovieName().IsEmpty())
	{ // DLCFront �� ���� �Դٸ� �� ó�� �÷����ߴ� Ÿ��Ʋ �������� �ε� ȭ������ �ٽ� ����.
		AB2DLCFrontGameMode::TrySetupLoadingScreenWithInitialTitleMovie(this);
	}
	else
	{
		Super::SetupNextGameLoadingScreen();
	}
}

void AB2PreRenderGameMode::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	// UpdateForPreRenderObjects �� �����ܿ��� �θ��� ��.

	TimeSinceLastPassBegin += DeltaSecond;
	++CurrentPassTickCount;

	// �� ���� �н� �ȿ����� dynamic point light ������ �� �Ѵ�. ���⿡ ���� ���̴� ������ ������ �� �޴� ��.
	if (PreRenderer)
	{
		if (CurrentPassTickCount >= InternalLit1PassTickLimit + InternalLit2PassTickLimit)
		{
			if (CurrentPassTickCount == InternalLit1PassTickLimit + InternalLit2PassTickLimit)
			{
				// �� �� ��. PointLight 0 �� ���̴��� ���� �ʿ�..
				PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDynPL(false);
				// �÷��̾� ĳ���� Head Light �� ���� �ݰ��� ���� ���� ��Ƽ� ��ǻ� �������� �Ѵ�. �ƽθ� �� �������� �� �� �����ϰ�.. �Ѥ�;
				PreRenderer->MinimizePlayerPointLight();
			}
		}
		else if (CurrentPassTickCount > InternalLit2PassTickLimit)
		{
			PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDynPL(false); // �ϳ� �����
		}
	}
}

TArray<EPCClass> AB2PreRenderGameMode::GetPCClassesToPreLoad()
{
	TArray<EPCClass> RetArray;
	// �⺻ ��� ��Ʈ�� ���� ������ �ε����� �ʵ���.. �ʿ��� �� �׶��׶� �ε��� ��.
	/*for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RetArray.Add(IntToPCClass((PCI)));
	}*/
	return RetArray;
}

TArray<FCombinedPCSkillAnimID> AB2PreRenderGameMode::GetPCSkillAnimsToPreLoad()
{
	TArray<FCombinedPCSkillAnimID> RetArray;

	// �⺻ ��� ��Ʈ�� ���� ������ �ε����� �ʵ���.. �ʿ��� �� �׶��׶� �ε��� ��.

	return RetArray;
}

void AB2PreRenderGameMode::SetupPreRenderObjects()
{
	check(!PreRenderer);
	PreRenderer = NewObject<UB2PreRenderer>(this, NAME_None, RF_Transient);
	check(PreRenderer);
	PreRenderer->InitForPreRenderGameMode(this);

	UB2PreRenderer::LoadSettings_PreRenderGM(); // ���� ���� ���� �ε�. ������ �ǵ����� ��.
//
//	// Pre-render �¾� ù ������.
//
//	// ���� ù ����Ʈ �н����� �ؼ� ����.
//	SetupForLightPass(GetFirstLightPass());
//	TimeSinceLastPassBegin = 0.0f;
//	CurrentPassTickCount = 0;
//
//	OpenPreLoadingScreen(EPreLoadingScreenType::PLST_PreRender_First);
//	check(CreatedPreLoadingScreen_PreRender);
//	if (CreatedPreLoadingScreen_PreRender)
//	{ // PreRenderGameMode �� �ؽ�Ʈ ���� ����. ���൵�� �ؽ�Ʈ ���� ���α׷��� �ٷ� ǥ���� ����..
//		CreatedPreLoadingScreen_PreRender->SetupPreRenderProgress(true, false, true);
//	}
//
//	WeightedPassSumSoFar = 0.5f; // ó���� �ణ ä������ ���� ��
//	UpdateProgressDisplay();	
//
//	// �⺻ ���Ӹ�忡�� �����ϴ� prerender �� ���� ������ ������ �ʵ��� �̰� �ſ� �۰� �־�� �ϴµ� ���⼱ �׷� �ʿ䰡 ����.
//	// ������ ��ƼŬ ����Ʈ ���� �͵��� ���� ���÷��� �Ǿ �������� �ִ��� ���� �ǵ��� TimeScale �� �ణ Ű�� ����..
//	SetWholeGameTimeScale(
//#if PLATFORM_WINDOWS // ���� �ǹ̰� ���� �׳� ��� Ȯ�� �����̹Ƿ� Ÿ�� �������� ũ�� �־ ���� �Ѿ����
//		PreRenderTimeScale * 10.0f
//#else
//		PreRenderTimeScale
//#endif
//	);
	
	B2GMLoadingProgCollector::StepCustom(0.7f); // ���⼭ ������� ǥ���ϴ� Progress ���� �� PreRender ������ �ε��ϴ� progress.
}

bool AB2PreRenderGameMode::IsInPreRenderPhase() const
{
	return (CurrentLightPass != EPRGMLightPass::LP_None && CurrentLightPass != EPRGMLightPass::LP_End &&
		CurrentContentTypePass != EPRGMContentTypePass::CTP_None && CurrentContentTypePass != EPRGMContentTypePass::CTP_End);
	// ���⼭ CurrentPCClassPass �� üũ���� �ʴ´�.
}

void AB2PreRenderGameMode::UpdateForPreRenderObjects()
{	
	checkSlow(IsInPreRenderPhase() && PreRenderer);

	if (PreRenderer)
	{
		const bool LeastTimeMet = (TimeSinceLastPassBegin >= SinglePassLeastTime);
		// ���ο����� tick ī��Ʈ�� �ϰ� ���⼱ �Ѿ�� ���� �ּ� �ð��� ����
		PreRenderer->UpdateForPreRenderObjects(!LeastTimeMet);

		if (LeastTimeMet && !PreRenderer->HasPreRenderDummyToProcess())
		{
			ProceedToNextPass(); // ���� �ܰ�� �Ѿ���� Ȥ�� ����.
		}
	}
}

void AB2PreRenderGameMode::ProceedToNextPass()
{
	// �� Pass �� ���� ������ ���� Pass �� �Ѿ����, Ȥ�� �̹��� ���� ������ pass �̸� ����.
	// PreRenderPhase �� ���� �ʰ� ��� CurrentLightPass �� CurrentContentTypePass �� ���.

	CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);

	if (PreRenderer) {
		PreRenderer->EnsureSaveCaches(); // Ȥ���� �� ũ������ ����� ���̺�?
	}

	if (IsLastLightPass(CurrentLightPass) && IsLastContentTypePass(CurrentContentTypePass))
	{
		OnCompleteAll();
	}
	else if (IsLastContentTypePass(CurrentContentTypePass))
	{
		SetupForLightPass((EPRGMLightPass)((int32)CurrentLightPass + 1));
	}
	else
	{
		// ContentTypePass �� �Ѱܾ� �ϴ� ����ε� PC �� PCSkillAnim �� ��� ���� pass �� �� ������ �װ� üũ
		if (CurrentContentTypePass == EPRGMContentTypePass::CTP_PC && !IsLastPCClassPass(CurrentPCClassPass))
		{
			SetupForPCClassPass(IntToPCClass(PCClassToInt(CurrentPCClassPass) + 1));
		}
		else if (CurrentContentTypePass == EPRGMContentTypePass::CTP_PC_SkillAnim && !IsLastPCClassPass(CurrentPCClassPass))
		{
			SetupForPCSkillAnimPass(IntToPCClass(PCClassToInt(CurrentPCClassPass) + 1));
		}
		else
		{
			SetupForContentTypePass((EPRGMContentTypePass)((int32)CurrentContentTypePass + 1));
		}
	}

	// ��Ģ��ζ�� SetupForLightPass �� �Ϻΰ� �Ǿ�� �ϴµ� ���� ���̽��� �� �־.. �ٺ������� dynamic point light �¾��� ���⼭ �ٽ� �Ѥ�
	if (PreRenderer)
	{
		PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDynPL();
		PreRenderer->ConditionalSetupPreRenderLightEnvForDynPL(true);
	}

	TimeSinceLastPassBegin = 0.0f;
	CurrentPassTickCount = 0;
	
	// Pass �Ѿ �� ���൵ ������Ʈ
	WeightedPassSumSoFar += ((CurrentLightPass == GetFirstLightPass()) ? GetFistLightPassProgressWeight() : 1.0f);
	UpdateProgressDisplay();
}

void AB2PreRenderGameMode::SetupForLightPass(EPRGMLightPass InSetupLightPass)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("AB2PreRenderGameMode::SetupForLightPass %d"), (int32)InSetupLightPass));

	check(InSetupLightPass != EPRGMLightPass::LP_None && InSetupLightPass != EPRGMLightPass::LP_End);
	CurrentLightPass = InSetupLightPass;
	
	if (PreRenderer)
	{ // ������ �¾��ϱ� ���� ���º���.
		PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDynPL();
		PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDirL();
	}

	switch (CurrentLightPass)
	{
		// �ʹ� ���� ������ light pass �� ����.. ó���� ���⿡ �ʹ� ���� �ð��� ���̴� �͵� ������ �� �� �ְ�, � �͵��� �������� ȿ�뼺�� �ǽɵǱ⵵ �ϰ�..

	case EPRGMLightPass::LP_StationaryDirectionalLight:
		if (PreRenderer) {
			PreRenderer->ConditionalSetupPreRenderLightEnvForDirL(EComponentMobility::Stationary, true);
			PreRenderer->ConditionalSetupPreRenderLightEnvForDynPL(true); // DirectionalLight �н������� PointLight �� ���� �¾�.. �츮 ���� ���� ȯ������
		}
		break;
#if _PRE_RENDER_GM_MORE_LIGHT_PASS && !_PRE_RENDER_GM_MOVABLE_EXTRA_ONLY
	case EPRGMLightPass::LP_MovableDirectionalLight:
		if (PreRenderer) {
			PreRenderer->ConditionalSetupPreRenderLightEnvForDirL(EComponentMobility::Movable, true);
			PreRenderer->ConditionalSetupPreRenderLightEnvForDynPL(true); // DirectionalLight �н������� PointLight �� ���� �¾�.. �츮 ���� ���� ȯ������
		}
		break;
#endif
	}
	
	// LightPass �� ContentTypePass ���� �ٱ��̱� ������ ContentTypePass �� ó������ �����Ѵ�.
	SetupForContentTypePass(GetFirstContentTypePass());
}

void AB2PreRenderGameMode::SetupForContentTypePass(EPRGMContentTypePass InContentTypePass)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("AB2PreRenderGameMode::SetupForContentTypePass %d"), (int32)InContentTypePass));

	check(InContentTypePass != EPRGMContentTypePass::CTP_None && InContentTypePass != EPRGMContentTypePass::CTP_End);
	CurrentContentTypePass = InContentTypePass;

	switch (CurrentContentTypePass)
	{
	case EPRGMContentTypePass::CTP_DamageEffectInfo:
		if (PreRenderer) {
			PreRenderer->SetupForPreRenderGM_DamageEffectInfo();
		}
		break;
	case EPRGMContentTypePass::CTP_PC:
		// PC Pass �� ��� �� Ŭ���� �� ���� �н��� ������ �� ù PCClass �н��� ����.
		SetupForPCClassPass(GetFirstPCClassPass());

		break;
	case EPRGMContentTypePass::CTP_PC_SkillAnim:
		// PC SkillAnim Pass �� ��� �� Ŭ���� �� ���� �н� �� ù PCClass �н��� ����.
		SetupForPCSkillAnimPass(GetFirstPCClassPass());
		break;
	case EPRGMContentTypePass::CTP_Extra_NPC:
		if (PreRenderer) {
			PreRenderer->SetupForPreRenderGM_Extra(true, false, false);
		}
		break;
	case EPRGMContentTypePass::CTP_Extra_PC_NPC_CSM:
		SetupForExtraCSMPass();
		break;
	case EPRGMContentTypePass::CTP_Extra_FX:
		SetupForExtraFxAndOtherPass();
		break;
#if _PRE_RENDER_GM_MOVABLE_EXTRA_ONLY
	case EPRGMContentTypePass::CTP_Extra_MovableDirLight:
		SetupForMovableLightExtraOnlyPass();
		break;
#endif
	}

	if ((int32)CurrentContentTypePass >= (int32)EPRGMContentTypePass::CTP_Extra_NPC)
	{
		// ���� �������� �ƴϴ��� ������ �Ĺݺο��� �Ϸ� ��ũ.
		UB2PreRenderer::MarkPreRenderLevelGoneThrough();
		UB2PreRenderer::SaveCompleteState();
	}
}

void AB2PreRenderGameMode::SetupForPCClassPass(EPCClass InPCClass)
{
	// EPRGMContentTypePass �� CTP_PC ���� �н�

	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("AB2PreRenderGameMode::SetupForPCClass %d"), PCClassToInt(InPCClass)));

	check(InPCClass != EPCClass::EPC_End);
	CurrentPCClassPass = InPCClass;

	if (PreRenderer)
	{
		PreRenderer->SetupForPreRenderGM_PCClassPass(CurrentPCClassPass);
	}
}

void AB2PreRenderGameMode::SetupForPCSkillAnimPass(EPCClass InPCClass)
{
	// EPRGMContentTypePass �� CTP_PC_SkillAnim ���� �н�

	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("AB2PreRenderGameMode::SetupForPCSkillAnimPass %d"), PCClassToInt(InPCClass)));

	check(InPCClass != EPCClass::EPC_End);
	CurrentPCClassPass = InPCClass;

	if (PreRenderer)
	{
		PreRenderer->SetupForPreRenderGM_PCSkillAnimPass(CurrentPCClassPass);
	}
}

void AB2PreRenderGameMode::SetupForExtraCSMPass()
{
	if (PreRenderer 
		&& ShouldDoExtraCSMPass() // ��ŵ�ϰ� �Ǹ� �׳� �ƹ��͵� ���� ä�� �ð��� ������ ���� �н��� �Ѿ�� ��.
		)
	{ // LightPass ó�� ������ ȯ���� �¾��ϴµ� ContentTypePass �� �Ϻθ� ����ϹǷ� LightPass �� �ֱ⵵ �ָ��ϰ� �ؼ�..
		PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDynPL();
		PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDirL();
		PreRenderer->ConditionalSetupPreRenderLightEnvForDirL(EComponentMobility::Stationary, true, true);
		PreRenderer->ConditionalSetupPreRenderLightEnvForDynPL(true);

		// PCClass �ϳ��� NPCClass �¾�
		PreRenderer->SetupForPreRenderGM_PCClassPass(EPCClass::EPC_Assassin);
		PreRenderer->SetupForPreRenderGM_Extra(true, false, false);
	}
}

void AB2PreRenderGameMode::SetupForExtraFxAndOtherPass()
{
	check(CurrentContentTypePass == EPRGMContentTypePass::CTP_Extra_FX);

	//if (PreRenderer && ShouldDoExtraFxPass()/*ShouldDoExtraFxPass �� ExtraFX ���� ����. Others �� �׳� �ϰ� ��*/) 
	//{
	//	PreRenderer->SetupForPreRenderGM_Extra(false, true, true);
	//}

	//{
	//	// ù �������� ��½ ������ �����ϴ� DamageNum �ε� ���⿡ �Ż縮 ��������
	//	// BuffModeEffectInfo �� �ϸ� ��������
	//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (LocalB2Player)
	//	{
	//		CharacterTakeDamageClass<ABladeIICharacter*, float, bool>::GetInstance().Signal(LocalB2Player, 1000.0f, true);
	//	}

	//	// ��Ȯ�� Extra �н��� ���õ� �� �ƴѵ� ���뿡�� Pre-render ������ ��ġ�� ������Ʈ���� �������� ���� Ȯ���� ������ �ϱ� ���� �¾��� �� ��.
	//	for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
	//	{
	//		UPrimitiveComponent* ThisPrimComp = *It;
	//		if (ThisPrimComp &&
	//			(Cast<UMeshComponent>(ThisPrimComp) || Cast<UParticleSystemComponent>(ThisPrimComp))
	//			)
	//		{
	//			ThisPrimComp->SetVisibility(true);
	//			ThisPrimComp->SetHiddenInGame(false);
	//			ThisPrimComp->SetExcessiveRenderProcessing(true);
	//		}
	//	}
	//}
}

#if _PRE_RENDER_GM_MOVABLE_EXTRA_ONLY
void AB2PreRenderGameMode::SetupForMovableLightExtraOnlyPass()
{
	check(CurrentContentTypePass == EPRGMContentTypePass::CTP_Extra_MovableDirLight);

	// MovableDirLight �� ������ ���Ǵ� ȯ���� �κ���̴� ���� MovableDirLight �н��� ��ü������ ���� �ʰ� Ư�� ���鿡 ���ؼ��� �õ��ϴ� ��.
	if (PreRenderer && ShouldDoMovableLightExtraOnlyPass())
	{
		// ��, �� ��� ���⼭ ����Ʈ ȯ���� �ٲٹǷ�.. ContentTypePass ������ �������� �ʿ䰡 �ִ�. �� �׷� ����.
		check(IsLastContentTypePass(EPRGMContentTypePass::CTP_Extra_MovableDirLight));
		PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDirL();
		PreRenderer->ConditionalSetupPreRenderLightEnvForDirL(EComponentMobility::Movable, true);
				
		PreRenderer->SetupForPreRenderGM_Extra(false, false, true); // Extra Other ��..
	}
}
#endif

bool AB2PreRenderGameMode::ShouldDoExtraCSMPass() const
{
	// Scalability �����̾� ����� ���ÿ� ���� �ٲ� ���� ������ PreRenderGameMode �� ó�� ���࿡ ���Ǵ� ������ �����ϸ� �⺻�������� shadow �� ������ ���� ����翡�� ��ŵ.
	static IConsoleVariable* ShadowQualityCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShadowQuality"));
	check(ShadowQualityCVar);
	return (ShadowQualityCVar && ShadowQualityCVar->GetInt() > 0);
}
bool AB2PreRenderGameMode::ShouldDoExtraFxPass() const
{
	//// Effect quality ������ ���ٰ� �ؼ� ���̴� pre-compile �� �ʿ���� �� �ƴ����� pre-render �� �δ��� Ŭ �� �����Ƿ� ��ŵ
	//return (Scalability::GetQualityLevels_Effects() > 0);
	return true;
}

#if _PRE_RENDER_GM_MOVABLE_EXTRA_ONLY
bool AB2PreRenderGameMode::ShouldDoMovableLightExtraOnlyPass() const
{
	return ShouldDoExtraCSMPass(); // ������ �ý��� ��� �� ���鼭 ������ ������ �����Ϸ��� ��.
}
#endif

void AB2PreRenderGameMode::OnCompleteAll()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PreRenderGameMode::OnCompleteAll"));

	//check(PreRenderer);
	//if (PreRenderer) 
	//{
	//	// ���� �������� Pre-render �� �ѹ� �Ϸ�� ������ ���.. �ϴ� �� ���� �ǹ̴� ����. �����δ� �κ���̶� �� Ÿ�Ե鿡 ���� ��ũ�� �Ǿ�� �� �״�. �׷��� �� ���� ������ ����.
	//	PreRenderer->MarkCurrentWorldLevelPreRendered();
	//	PreRenderer->OnPreRenderComplete();
	//	PreRenderer->ConditionalBeginDestroy(); // ���� ���� ���� ��ũ�� �� �־�� �Ƚ��� �� ���� ���
	//}
	//PreRenderer = nullptr;

	//// ������ ��ġ�� �͵��̾� �̰� ���� ���ص� ������ ��Ʈ�� ������Ʈ�� �ְ� �ؼ�.. ���ϰ� ExcessiveRenderProcessing �� ��� ������ ������ �ٷ� �����ؾ�.
	//for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
	//{
	//	UPrimitiveComponent* ThisPrimComp = *It;
	//	if (ThisPrimComp)
	//	{
	//		ThisPrimComp->SetExcessiveRenderProcessing(false);
	//	}
	//}

	//CurrentLightPass = EPRGMLightPass::LP_End;
	//CurrentContentTypePass = EPRGMContentTypePass::CTP_End;

	//// ���⼭ �������� �ߴ��� �� ������ ��¦ �巯��. ������ �̷��� �� �ص� ���ŵ� ��.
	////ClosePreLoadingScreen(EPreLoadingScreenType::PLST_PreRender_First);

	//UB2PreRenderer::LoadSettings(); // PreRenderGM �������� ���� ���� �ε��ߴ� �� �ǵ��� 

	//// ��� �غ�� ������. ����, ���̵�II �� ��������.

	//// ���� ���� ��Ʈ�μ��� �̰� ��ġ �� ó�� ���ϰ� �� ��Ʈ������ �׷��ٰ� ���⼭ Ʃ�丮�� ���� �ε��ϸ� ��ĩ �ٸ� ��Ȳ���� ���� �� �ִ�.. 
	//// �ϴ� �κ�� ���ٰ� �ű⼭ Ʃ�丮�� ������ ���� �ɷ�.
	//OpenBladeIILobbyCommon(this);
}

void AB2PreRenderGameMode::UpdateProgressDisplay()
{
	//float ProgressScale = FMath::Clamp( WeightedPassSumSoFar / (float)GetTotalWeightedPassNum(), 0.0f, 1.0f);

	//if (CreatedPreLoadingScreen_PreRender)
	//{ // UB2PreLoadingScreen �ȿ����� ��ü �ִϸ��̼��� ���ư� ��.
	//	CreatedPreLoadingScreen_PreRender->UpdatePreRenderProgressScale(ProgressScale);
	//}
}

//================================================================

bool CheckBladeIIPreRenderGameAvailability()
{
	FString PreRenderMapName;

	if (!GConfig->GetString(TEXT("PreRender"), TEXT("BladeIIPreRenderMap"), PreRenderMapName, GGameIni)) {
		return false;
	}

	// ���� ���� ��ã���� �ٸ� ������ �ڵ鸵�� �ϵ��� false ����.
	if (!GEngine->MakeSureMapNameIsValid(PreRenderMapName)) {
		return false;
	}

	return true;
}

bool StartBladeIIPreRenderGame(class AGameMode* InCurrentGameMode)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIPreRenderGame"));

	FString PreRenderMapName;

	if (!GConfig->GetString(TEXT("PreRender"), TEXT("BladeIIPreRenderMap"), PreRenderMapName, GGameIni)) {
		return false;
	}

	// �̰� �ʿ��Ѱ�..
	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::Lobby);

	// ���� �� �ε����� ���� ���� ����.
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::PreRender);

	UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*PreRenderMapName), true);

	return true;
}