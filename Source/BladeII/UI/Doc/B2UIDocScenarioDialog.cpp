#include "B2UIDocScenarioDialog.h"
#include "B2PCClassInfoBox.h"
#include "B2PCClassInfo.h"
#include "BladeIICharacter.h"
//#include "BladeIIUtil.h"

UB2UIDocScenarioDialog::UB2UIDocScenarioDialog(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bMultiple = false;
	bAnonymous = false;
	DocType = EUIDocType::ScenarioDialog;
}

UB2UIDocScenarioDialog::~UB2UIDocScenarioDialog()
{

}

void UB2UIDocScenarioDialog::Init()
{
	Super::Init();

	//auto ClassInfoBox = StaticFindPCClassInfoBox();
	//check(ClassInfoBox);

}

void UB2UIDocScenarioDialog::ReleaseResources()
{ // 饭骇 攫肺爹 矫痢俊 荤侩且 狼档.
	SpeakerPortrait = NULL;
}