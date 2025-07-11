#include "B2UIDocEquip.h"
#include "B2UIDocHero.h"

UB2UIDocEquip::UB2UIDocEquip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bMultiple = true;
	bAnonymous = true;
	DocType = EUIDocType::Equip;
	EquippedItemId = INDEX_NONE;
}

UB2UIDocEquip::~UB2UIDocEquip()
{

}

void UB2UIDocEquip::DoEquip(const FB2Item& InEquipItem)
{
	if ((int32)InEquipItem.EquipPlace == EquipPlace) // �ٸ� ����� ��ǻ�Ͱ� �����Ѵ�.
	{
		SetEquipped(true);
		SetEquippedItemId(InEquipItem.InstanceUID);
		SetEquippedItem(InEquipItem);
	}
}

void UB2UIDocEquip::RemoveEquip()
{
	SetEquipped(false);
}

void UB2UIDocEquip::UpdateData(const FB2Item& InNewData)
{ 
	if ((int32)InNewData.EquipPlace == EquipPlace) // �ٸ� ����� ��ǻ�Ͱ� �����Ѵ�.
	{
		// GetEquipped üũ�� �� �ʿ䰡 ��������..
		SetEquippedItemId(InNewData.InstanceUID);
		SetEquippedItem(InNewData);
	}
}