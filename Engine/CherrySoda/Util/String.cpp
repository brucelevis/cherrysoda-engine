#include <CherrySoda/Util/String.h>

#include <CherrySoda/Util/STL.h>

#include <cstdarg>

using cherrysoda::STL;
using cherrysoda::String;
using cherrysoda::StringID;
using cherrysoda::StringUtil;

const String StringUtil::Format(const char* format, ...)
{
	char buffer[64];
	va_list args;
	va_start(args, format);
	std::vsprintf(buffer, format, args);
	va_end(args);
	return String(buffer);
}
