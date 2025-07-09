#include "BladeII.h"
#include "B2UIDocChapter.h"
#include "B2ChapterInfo.h"

UB2UIDocChapter::UB2UIDocChapter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SelectHardStageDifficulty = EStageDifficulty::ESD_None;
	MaxOpenedStageDifficulty = EStageDifficulty::ESD_None;
	bMultiple = false;
	bAnonymous = false;
	DocType = EUIDocType::Chapter;
}

UB2UIDocChapter::~UB2UIDocChapter()
{

}
