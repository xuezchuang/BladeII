
#ifndef __FB2ERRORMESSAGE_H__
#define __FB2ERRORMESSAGE_H__
//#include "BladeII.h"

struct FB2ErrorMessage
{
	/**
	* Open a modal message box dialog
	* @param MessageType Controls buttons dialog should have
	* @param Message Text of message to show
	* @param OptTitle Optional title to use (defaults to "Message")
	*/
	static EAppReturnType::Type Open(EAppMsgType::Type MessageType, const FText& Message, const FText* OptTitle = nullptr);
};

#define FErrorMessage FB2ErrorMessage

#endif