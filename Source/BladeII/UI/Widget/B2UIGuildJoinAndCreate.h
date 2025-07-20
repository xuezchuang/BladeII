#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIGuildJoinAndCreate.generated.h"

/*
* Lobby Main UI
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildJoinAndCreate : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void BindDelegates() override;

public:
	UFUNCTION()
		void OnClickGuildCreate();
	UFUNCTION()
		void OnClickGuildJoin();

protected:
	TWeakObjectPtr<UB2Button>				BTN_GuildCreate;
	TWeakObjectPtr<UB2Button>				BTN_GuildJoin;


};