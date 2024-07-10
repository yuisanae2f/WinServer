#ifndef THREADFILE_HPP
#define THREADFILE_HPP

#if __cplusplus
#include <thread>
#include <string>
#include <unordered_map>
#include "JsonMgr.h"

// This class "tricks" that it does write and read contemporary.
// On a command read while operating "writing", this class will return the pre-read data in order to "trick" it.
class ThreadFIle {
	bool b; // is ready
public:
	Json::cReadMgr write_buff;
	// Safe-kind buffer for the file.
	// stores the data 
	Json::cReadMgr __stable;

	ThreadFIle();
	ThreadFIle(std::string __path, std::string buff);

	// return whether it is ready to read or write physically.
	constexpr bool& ready() { return b; }

	void Write(std::string __Path);
	void Write(std::string __Path, std::string _Buff);
	void WriteJSON_Sock(std::string __Path, SOCKET client, std::string rest);

	std::string Read();
	std::string Read(std::string _p);
};

#endif // __cplusplus
#endif