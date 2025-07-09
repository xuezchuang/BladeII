
#include "BladeII.h"

#include "FB2ErrorMessage.h"

namespace
{
	/**
	* Singleton to only create error title text if needed (and after localization system is in place)
	*/
	const FText& GetDefaultMessageTitle()
	{
		// Will be initialised on first call
		static FText DefaultMessageTitle(NSLOCTEXT("MessageDialog", "DefaultMessageTitle", "Message"));
		return DefaultMessageTitle;
	}
}

EAppReturnType::Type FB2ErrorMessage::Open(EAppMsgType::Type MessageType, const FText& Message, const FText* OptTitle)
{
	if (FApp::IsUnattended() != true)
	{
		FText Title = OptTitle ? *OptTitle : GetDefaultMessageTitle();
		if (GIsEditor && !IsRunningCommandlet())
		{
			if (FCoreDelegates::ModalErrorMessage.IsBound())
				return FCoreDelegates::ModalErrorMessage.Execute(MessageType, Message, Title);
			else
			{
				FMessageLog("BladeII").Error()
					->AddToken(FTextToken::Create(Message));
			}
		}
	}

	if (GWarn)
	{
		//GWarn->Logf(*Message.ToString());
	}

	switch (MessageType)
	{
	case EAppMsgType::Ok:
		return EAppReturnType::Ok;
	case EAppMsgType::YesNo:
		return EAppReturnType::No;
	case EAppMsgType::OkCancel:
		return EAppReturnType::Cancel;
	case EAppMsgType::YesNoCancel:
		return EAppReturnType::Cancel;
	case EAppMsgType::CancelRetryContinue:
		return EAppReturnType::Cancel;
	case EAppMsgType::YesNoYesAllNoAll:
		return EAppReturnType::No;
	case EAppMsgType::YesNoYesAllNoAllCancel:
	default:
		return EAppReturnType::Yes;
		break;
	}
}