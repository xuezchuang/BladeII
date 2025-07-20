
#include "MyAssetStore.h"

#include "B2UIDocHelper.h"
#include "Event.h"


#include "BladeIIGameImpl.h"

FMyAssetStore::FMyAssetStore()
{
	bFreezeAdd = false;
	InitialGoldAmount = 0;
	CurrentTempGoldAmount = 0;
}

void FMyAssetStore::SetStartingGold(int32 NewAmount)
{
	InitialGoldAmount = CurrentTempGoldAmount = NewAmount;

	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
	if (DocUser)
	{
		DocUser->SetCombatStartGold(InitialGoldAmount);
	}
}

void FMyAssetStore::SyncCurrentTempGoldToAccountInfo(bool bOptionalFreeze)
{ // StageClear 矫痢俊 教农罐篮 榜靛 剧阑 咯扁肺 嘎苗林扁 困窃.
	bFreezeAdd = bOptionalFreeze; // 弊府绊 捞繁 版快 酒付档 歹 歹窍辨 盔窍瘤绰 臼阑 巴. 努府绢 脚龋 捞饶俊 焊胶 各 榜靛甫 sucking 且 荐 乐促.

	CurrentTempGoldAmount = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();
	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
	if (DocUser)
	{
		DocUser->SetGold(CurrentTempGoldAmount);
	}
}

void FMyAssetStore::AddGold(int32 AddAmount)
{
	if (!bFreezeAdd)
	{
		CurrentTempGoldAmount += AddAmount;

		UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
		if (DocUser)
		{
			DocUser->SetGold(CurrentTempGoldAmount);
		}

		// UI 肺 脚龋
		PlayerSuckedGoldClass<int32>::GetInstance().Signal(AddAmount);
	}
}