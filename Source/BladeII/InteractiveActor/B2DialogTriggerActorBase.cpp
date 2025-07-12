
#include "B2DialogTriggerActorBase.h"
#include "B2StageManager.h"
#include "B2StageGameMode.h"
#include "B2StageEventDirector.h"
#include "Event.h"
#include "Components/BillboardComponent.h"

AB2DialogTriggerActorBase::AB2DialogTriggerActorBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(DefaultRootComponent);

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	EditorSpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("DialogTriggerActorSprite"));
	if (!IsRunningCommandlet() && (EditorSpriteComponent != nullptr))
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture;
			FName ID_B2DialogTriggerActor;
			FText NAME_B2DialogTriggerActor;
			FConstructorStatics()
				: SpriteTexture(TEXT("/Engine/EditorResources/S_FTest")) // ������ �뺻 ���� ���..
				, ID_B2DialogTriggerActor(TEXT("B2DialogTriggerActor"))
				, NAME_B2DialogTriggerActor(FText::FromString(TEXT("B2DialogTriggerActorEditorSprite")))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;
		EditorSpriteComponent->Sprite = ConstructorStatics.SpriteTexture.Get();
		EditorSpriteComponent->SpriteInfo.Category = ConstructorStatics.ID_B2DialogTriggerActor;
		EditorSpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_B2DialogTriggerActor;
		EditorSpriteComponent->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		EditorSpriteComponent->bIsScreenSizeScaled = true;
		EditorSpriteComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
#endif

	TriggerType = EInteractiveTriggeringType::InteractByTriggerVolume;

	ManageMode = EStageEventManageMode::SEMM_Always;
	bMaintainBattleUIs = false;
}

void AB2DialogTriggerActorBase::InteractAction()
{
	OpenDialogClass<FName, bool>::GetInstance().Signal(DialogCodeName, (bool)bMaintainBattleUIs);

	Super::InteractAction();
}

bool AB2DialogTriggerActorBase::IsTriggering(AActor* Other, TEnumAsByte<EInteractiveTriggeringType::Type> InTriggerType)
{
	if (AB2StageEventDirector::CheckSkipForManageMode(this, ManageMode, false) != EStageEventSkipType::SEST_Play)
	{
		return false; // �ó����� ���, ��Ÿ �߰��� �� �ִ� Ư�� ���Ӹ�� ��Ȳ üũ
	}

	AB2StageGameMode* StageGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(this));
	AB2StageManager* StageMgr = StageGM ? StageGM->GetStageManager() : NULL;
	if (StageMgr)
	{
		// ���������� ���̵� �Ѵ� �´°�츸 �ߵ�
		if (IsStageSupported(StageMgr->GetCurrentClientStageId()) && IsDifficultySupported(StageMgr->GetStageDifficultyLevel()))
		{
			return Super::IsTriggering(Other, InTriggerType);
		}
	}
	
	return false;	
}

bool AB2DialogTriggerActorBase::IsStageSupported(int32 InClientStageId) const
{
	if (SupportedStages.Num() == 0)
	{
		return true; // ��� �ִ� ��� �⺻���� ��� �������� ����
	}
	for (int32 CurrStage : SupportedStages)
	{
		if (CurrStage == InClientStageId)
		{
			return true;
		}
	}
	return false;
}

bool AB2DialogTriggerActorBase::IsDifficultySupported(EStageDifficulty InStageDifficulty) const
{
	if (SupportedDifficulty.Num() == 0)
	{
		return true; // ��� �ִ� ��� �⺻���� ��� �������� ����
	}
	for (EStageDifficulty StageDifficulty : SupportedDifficulty)
	{
		if (StageDifficulty == InStageDifficulty)
		{
			return true;
		}
	}
	return false;
}

bool AB2DialogTriggerActorBase::IsExpectedForGameMode(class ABladeIIGameMode* InB2GM) const
{
	// �̹� ���Ӹ�� ������ �⺻ flow �󿡼� ���� �ߵ��� �ɷ� ����Ǵ���.
	AB2StageGameModeBase* CastedSGM = Cast<AB2StageGameModeBase>(InB2GM);
	AB2StageManager* StageManagerIfAny = CastedSGM ? CastedSGM->GetStageManager() : NULL;
	if (StageManagerIfAny)
	{
		return IsStageSupported(StageManagerIfAny->GetCurrentClientStageId()) &&
			(AB2StageEventDirector::CheckSkipForManageMode(CastedSGM, ManageMode, false) == EStageEventSkipType::SEST_Play);
	}
	return true; // StageGameMode �� �ƴ� ���� �⺻ �����Ǵ� �ɷ� ����. ���� �������� ������ ���� �⺻ ���� ����� ��.
}

#if WITH_EDITOR
void AB2DialogTriggerActorBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_SupportedStages = GET_MEMBER_NAME_CHECKED(AB2DialogTriggerActorBase, SupportedStages);

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	if (PropertyName == Name_SupportedStages)
	{
		for (int32 SI = 0; SI < SupportedStages.Num(); ++SI)
		{
			SupportedStages[SI] = FMath::Max(1, SupportedStages[SI]);
		}
	}
}
#endif