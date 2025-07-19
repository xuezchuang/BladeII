
#include "B2Reflection.h"
#include "B2NetworkPlugin.h"
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