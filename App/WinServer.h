// WinServer.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.

#pragma once

#include "config.h"

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <thread>
#include <sstream>
#include "gen8.h"
#include "multibit.hpp"
#include "JsonMgr.h"
#include "ThreadFIle.h"

#if _Debug

// Debug mode on
#define _Debug 1

using eLog_t = unsigned char;

// Write a log on console.
#define eLog_Console 0
// Include a trial log on Debugging.
#define eLog_Trial 1
// Saves the log in a txt file.
#define eLog_Save 2

// [eLog_t] m: index
#define DbgTag(m) ((Debug >> (m)) & _Debug)

struct scDebug {
	constexpr static bool get(eLog_t e) {
		return DbgTag(e);
	}
};

// interface
struct iErrStream {
	// log 넣어 두는 곳
	static std::ostringstream log;

protected:
	// Dumping function
	virtual bool dump() { return true; }
}; 


// template programming
template<eLog_t, bool>
struct __cErrStream : iErrStream {};

template<>
struct __cErrStream<eLog_Console, 1> : iErrStream
{
protected:
	virtual bool dump() override;
};

template<>
struct __cErrStream<eLog_Save, 1> : iErrStream
{
protected:
#if DbgTag(eLog_Save)
	static std::string file_path;
#endif // DbgTag(eLog_Save)
	virtual bool dump() override;
	__cErrStream();
};

extern struct cErrStream :
	public iErrStream,
	public __cErrStream<eLog_Console, scDebug::get(eLog_Console)>,
	public __cErrStream<eLog_Save, scDebug::get(eLog_Save)>
{
	virtual bool dump() override;
} __errOut;

#define Err(...) cErrStream::log << __VA_ARGS__
#define LogDump(...) __errOut.dump(__VA_ARGS__)
#if DbgTag(eLog_Trial)
#define Trial(...) cErrStream::log << __VA_ARGS__
#else
#define Trial(...)
#endif
#else
#define DbgTag(...) 0
#define Trial(...)
#define LogDump(...)
#define Err(...)
#endif // _Debug

#define SizeOnTrial 1024

struct __Buffer {
	char raw[SizeOnTrial];
	size_t s;

	constexpr bool full() {
		return SizeOnTrial == s;
	}

	void clear();

	constexpr __Buffer() : raw{ 0, }, s(0) {}
};

// Hard-coded version of valid http response header.
std::string httpValidHeader(
	// The size of the following body.
	size_t content_size
);

// When it starts, the whole programme (thread) would be made
// and run eventually until it runs over the stack(scope).
class WinServer {
	bool __Run;		// buffer that I command the server to keep running
	bool Running;	// buffer that server replies to me that "I am running"
public:
	static std::unordered_map<std::string, ThreadFIle> Files;
	inline void terminate() { __Run = 0; while (Running); }
	constexpr WinServer() : __Run(0), Running(0) {}

	void Main(
#if !lURL
		// The endpoint (ip, or domain) to deploy.
		const char* endpoint,
#endif // lURL
#if !lPORT
		// The port number you will add.
		const char* port,
#endif // !lPORT
		// Decide whether to detach the thread and not wait until the server is down or not.
		bool toDetach = 1
	);
	inline ~WinServer() { terminate(); }
	constexpr bool isRunning() { return this->Running; }
};

// Writes a file on 
// [string] a: file_path
// [string] b: buffer
#define FILE_WRITE(a, b) WinServer::Files[a].Write(a, b)
// 
// [string] a: file_path
#define FILE_READ(a) WinServer::Files[a].Read(a)