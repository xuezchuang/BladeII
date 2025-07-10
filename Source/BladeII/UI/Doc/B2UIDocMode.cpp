#include "B2UIDocMode.h"
#include "Event.h"


UB2UIDocMode::UB2UIDocMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ContentsModeInfos.Empty();

	CAPTURE_UOBJECT(UB2UIDocMode);
	DeliveryResponseGetContentsModeStateTicket = DeliveryResponseGetContentsModeStateClass<FB2ResponseGetContentsModeStatePtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryResponseGetContentsModeState, const FB2ResponseGetContentsModeStatePtr& msgPtr)
		Capture->ReceiveGetContentsModeState(msgPtr->mode_infos);
	END_CAPTURE_OBJECT()
		);
}

UB2UIDocMode::~UB2UIDocMode()
{
	DeliveryResponseGetContentsModeStateClass<FB2ResponseGetContentsModeStatePtr>::GetInstance().Unsubscribe(DeliveryResponseGetContentsModeStateTicket);
}

void UB2UIDocMode::ReceiveGetContentsModeState(TArray<b2network::B2ContentsModeInfoPtr>& ModeInfos)
{
	for (b2network::B2ContentsModeInfoPtr& ModeInfoPtr : ModeInfos)
	{
		ReceiveSingleGetContentsModeState(ModeInfoPtr->mode, ModeInfoPtr->state);
	}
}

void UB2UIDocMode::ReceiveSingleGetContentsModeState(int32 ContentsMode, int32 ContentsModeState)
{
	if (ContentsModeInfos.Contains(ContentsMode))
	{
		ContentsModeInfos[ContentsMode] = ContentsModeState;
	}
	else
	{
		ContentsModeInfos.Add(ContentsMode, ContentsModeState);
	}
}

// ContentsMode - b2network::B2ContentsMode
int32 UB2UIDocMode::GetContentsModeState(int32 ContentsMode)
{
	if (ContentsModeInfos.Contains(ContentsMode))
	{
		// Value : b2network::B2ContentsModeState
		return ContentsModeInfos[ContentsMode];
	}

	return b2network::B2ContentsModeState::NONE;
}
