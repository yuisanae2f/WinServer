#include "WinServer.h"

#if _Debug
std::ostringstream iErrStream::log;

bool __cErrStream<eLog_Console, 1>::dump() {
	std::cout << log.str();
	return true;
}

#if DbgTag(eLog_Save)
std::string __cErrStream<eLog_Save, 1>::file_path = Path_Log;
#endif // DbgTag(eLog_Save)

bool __cErrStream<eLog_Save, 1>::dump() {
#if DbgTag(eLog_Save)
	std::ofstream out(file_path, std::ios::app);
	if (out.is_open()) {
		out << log.str();
		out.close();
		return true;
	}
	else {
#if _Debug
		std::cout << "[Error] Critic error: file opening has failed.";
		return false;
#endif // Debug
	}
#endif // DbgTag(eLog_Save)

	return true;
}

__cErrStream<eLog_Save, 1>::__cErrStream() {
#if DbgTag(eLog_Save)
	std::ofstream out(file_path, std::ios::out);
	if (out.is_open()) {
		out.close();
	}
#endif // DbgTag(eLog_Save)
}

bool cErrStream::dump() {
	bool suc =
		__cErrStream<eLog_Save, scDebug::get(eLog_Save)>::dump() &&
		__cErrStream<eLog_Console, scDebug::get(eLog_Console)>::dump();
	if(suc) 
		log.str("");

	return suc;
}

cErrStream __errOut;
#endif // _Debug