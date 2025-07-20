#include "B2UIDocSome.h"
#include "Event.h"

//
// 购啊 UI 俊 持阑 单捞磐啊 乐绰单 绢蝶 持绢具且瘤档 葛福摆绊, 货肺 UIDoc 阑 父甸扁档 构窍绊 且 锭 荤侩且
//
// "芥蹿"
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
