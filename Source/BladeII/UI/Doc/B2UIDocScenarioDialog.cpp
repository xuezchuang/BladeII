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
{ // ���� ��ε� ������ ����� �ǵ�.
	SpeakerPortrait = NULL;
}