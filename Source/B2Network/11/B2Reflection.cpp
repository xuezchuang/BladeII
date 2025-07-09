//#include "B2NetworkPlugin.h"
#include "B2Reflection.h"

namespace reflection
{
	ReflectionHandler& ReflectionHandler::instance()
	{
		static ReflectionHandler inst;
		return inst;
	}

	const ProtoClass* ProtoClass::from_name(std::string name)
	{
		return ReflectionHandler::instance().get_class(name);
	}
}