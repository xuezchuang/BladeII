//
#include "MyAssetStore.h"
//
////#include "B2UIDocHelper.h"
//#include "Event.h"
//
//
//#include "BladeIIGameImpl.h"
//
//FMyAssetStore::FMyAssetStore()
//{
//	bFreezeAdd = false;
//	InitialGoldAmount = 0;
//	CurrentTempGoldAmount = 0;
//}
//
//void FMyAssetStore::SetStartingGold(int32 NewAmount)
//{
//	InitialGoldAmount = CurrentTempGoldAmount = NewAmount;
//
//	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
//	if (DocUser)
//	{
//		DocUser->SetCombatStartGold(InitialGoldAmount);
//	}
//}
//
//void FMyAssetStore::SyncCurrentTempGoldToAccountInfo(bool bOptionalFreeze)
//{ // StageClear 시점에 싱크받은 골드 양을 여기로 맞춰주기 위함.
//	bFreezeAdd = bOptionalFreeze; // 그리고 이런 경우 아마도 더 더하길 원하지는 않을 것. 클리어 신호 이후에 보스 몹 골드를 sucking 할 수 있다.
//
//	CurrentTempGoldAmount = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();
//	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
//	if (DocUser)
//	{
//		DocUser->SetGold(CurrentTempGoldAmount);
//	}
//}
//
//void FMyAssetStore::AddGold(int32 AddAmount)
//{
//	//if (!bFreezeAdd)
//	//{
//	//	CurrentTempGoldAmount += AddAmount;
//
//	//	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
//	//	if (DocUser)
//	//	{
//	//		DocUser->SetGold(CurrentTempGoldAmount);
//	//	}
//
//	//	// UI 로 신호
//	//	PlayerSuckedGoldClass<int32>::GetInstance().Signal(AddAmount);
//	//}
//}