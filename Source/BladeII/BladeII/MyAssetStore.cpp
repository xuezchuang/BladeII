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
//{ // StageClear ������ ��ũ���� ��� ���� ����� �����ֱ� ����.
//	bFreezeAdd = bOptionalFreeze; // �׸��� �̷� ��� �Ƹ��� �� ���ϱ� �������� ���� ��. Ŭ���� ��ȣ ���Ŀ� ���� �� ��带 sucking �� �� �ִ�.
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
//	//	// UI �� ��ȣ
//	//	PlayerSuckedGoldClass<int32>::GetInstance().Signal(AddAmount);
//	//}
//}