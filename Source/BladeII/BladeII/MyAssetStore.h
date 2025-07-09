#pragma once

class FMyAssetStore
{
	/** Initial amount at stage starting moment. Final gold will reset to this value if player abandon current level. */
	int32 InitialGoldAmount;

	/** InitialGoldAmount + Acquired gold while playing the stage. It is not the final yet. Still shown at UI */
	int32 CurrentTempGoldAmount;

	uint32 bFreezeAdd : 1; // To be set after sync by the actual amount, then no more add by client scene.

public:
	FMyAssetStore();

	void SetStartingGold(int32 NewAmount);
	void AddGold(int32 AddAmount);

	void SyncCurrentTempGoldToAccountInfo(bool bOptionalFreeze = false); // Use this at special condition only.

	/** Get the current gold amount total. Final if player complete the statge. */
	FORCEINLINE int32 GetCurrentTempGoldAmount() { return CurrentTempGoldAmount; }
	/** Final gold if player quit the match. */
	FORCEINLINE int32 GetInitialGoldAmount() { return InitialGoldAmount; }
	/** Acquired gold so far for this stage. */
	FORCEINLINE int32 GetAcquiredGoldDelta() { return CurrentTempGoldAmount - InitialGoldAmount; }
};