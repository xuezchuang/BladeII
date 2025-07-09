#include "BladeII.h"
#include "B2UIDocSome.h"
#include "Event.h"

//
// 뭔가 UI 에 넣을 데이터가 있는데 어따 넣어야할지도 모르겠고, 새로 UIDoc 을 만들기도 뭐하고 할 때 사용할
//
// "썸닥"
//

UB2UIDocSome::UB2UIDocSome(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DocType = EUIDocType::Some;	
	OnClickUserToHallOf = false;
	OnClickHallOfFame = false;
	CurrentBestCharacter = EPCClass::EPC_Assassin;
	HallOfFameMode = EHallOfFameMode::PVP;
	OpenedMailTab = MailTabIndex::EMailTab_Gift;
	OutAutoSellGradeFilter = -1;
}

UB2UIDocSome::~UB2UIDocSome()
{
	
}

void UB2UIDocSome::Init()
{
	Super::Init();
}

void UB2UIDocSome::Destroy()
{
	Super::Destroy();
}

void UB2UIDocSome::SetPraiseInfo(const TArray<b2network::B2HallOfFamePraisePtr>& PraiseInfo)
{
	if (PraiseInfo.Num() >= 3)
	{
		EnablePraise.Add(static_cast<EHallOfFameMode>(PraiseInfo[0]->mode), PraiseInfo[0]->praise_count < PraiseInfo[0]->max_count);
		EnablePraise.Add(static_cast<EHallOfFameMode>(PraiseInfo[1]->mode), PraiseInfo[1]->praise_count < PraiseInfo[1]->max_count);
		EnablePraise.Add(static_cast<EHallOfFameMode>(PraiseInfo[2]->mode), PraiseInfo[2]->praise_count < PraiseInfo[2]->max_count);
	}
}

void UB2UIDocSome::SetPraiseInfo(const b2network::B2HallOfFamePraisePtr& PraiseInfo)
{
	if (PraiseInfo)
	{
		EHallOfFameMode InHallOfFameMode = static_cast<EHallOfFameMode>(PraiseInfo->mode);

		if (EnablePraise.Contains(HallOfFameMode))
		{
			EnablePraise[HallOfFameMode] = PraiseInfo->praise_count < PraiseInfo->max_count;
		}
	}
}

bool UB2UIDocSome::GetPraiseInfo(EHallOfFameMode InHallOfFame) const
{
	return EnablePraise.Contains(InHallOfFame) && EnablePraise[InHallOfFame];
}
