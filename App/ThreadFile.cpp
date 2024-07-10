#include "WinServer.h"
#include "ThreadFIle.h"
#include <fstream>
#include <sstream>

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <param name="b"> is ready</param>
/// <param name="buff_w"></param>
static void Fun(std::string a, bool* b, Json::cReadMgr* buff_w) {
	Trial("[Writing] Writing Trial\n");
	LogDump();
	while (!b[0]);
	b[0] = false;
	std::ofstream ofs(a);

	std::string buff = buff_w->String();

	if (ofs.is_open()) {
		ofs.write(buff.c_str(), buff.size());
		ofs.close();
	}
	b[0] = true;
	Trial("[Wrote] Writing has done\n");
	LogDump();
	return;
}

static size_t StringToCount(std::string& str, size_t& i) {
	std::string v;
	std::istringstream(std::string(str.begin() + i, str.end())) >> v;
	i += v.size() + 1;

	return std::stoull(v);
}

static std::string StringRead(std::string& str, size_t& i) {
	std::string v;
	std::istringstream(std::string(str.begin() + i, str.end())) >> v;
	i += v.size() + 1;
	return v;
}
enum eMETHOD {
	Set = '0',
	ListAppend,
	Remove,
	Null
};
static void Fun2(std::string __Path, SOCKET clientSocket, std::string BodyWithHead, bool* ready, Json::iObj* JSON) {
	Trial("[SockWrite] Writing Trial\n");
	LogDump();
	while (!ready[0]);
	ready[0] = false;
	__Buffer buff;
	do {
		buff.clear();
		buff.s = recv(clientSocket, buff.raw, SizeOnTrial, 0);
		if (buff.s != -1) {
			BodyWithHead += std::string(buff.raw, buff.s);
		}
	} while (buff.s && buff.s != -1);

	size_t idxBody = 0; uint8_t __Method = 0;
	for (; BodyWithHead[idxBody] != '!'; idxBody++);

	__Method = BodyWithHead[idxBody + 1];
	BodyWithHead = std::string(BodyWithHead.c_str() + idxBody + 2);
	idxBody = 0;
	
	for (size_t i = StringToCount(BodyWithHead, idxBody); i > 1; i--) {
		std::string v = StringRead(BodyWithHead, idxBody);

		switch (JSON->Type()) {
		case Json::Dic: {
			JSON = JSON->Union().classDictionary->dic[v];
		} break;
		case Json::List: {
			JSON = JSON->Union().classList->vector[std::stoull(v)];
		} default: break;
		}
	}

	std::string __V = StringRead(BodyWithHead, idxBody);
	switch (__Method) {
	case eMETHOD::ListAppend: {
		BodyWithHead = std::string(BodyWithHead.c_str() + idxBody);
		JSON->Union().classList->vector.push_back(Json::cReadMgr(BodyWithHead).Abandon());
	} break;
	case eMETHOD::Remove: {
		
	} break;
	case eMETHOD::Set: {
		std::string key = StringRead(BodyWithHead, idxBody);
		BodyWithHead = std::string(BodyWithHead.c_str() + idxBody);
		switch (JSON->Type()) {
		case Json::List: {
			size_t _keyIdx = std::stoull(key);
			delete JSON->Union().classList->vector[_keyIdx];
			JSON->Union().classList->vector[_keyIdx] = Json::cReadMgr(BodyWithHead).Abandon();
		} break;
		case Json::Dic: {
			delete JSON->Union().classDictionary->dic[key];
			JSON->Union().classDictionary->dic[key] = Json::cReadMgr(BodyWithHead).Abandon();
		}
		}
	} break;
	default: break;
	}

	std::ofstream ofs(__Path);
	if (ofs.is_open()) {
		std::string __ = JSON->String();
		ofs.write(__.c_str(), __.size());
		ofs.close();
	}
	else {
	}

	std::string go = httpValidHeader(0);
	send(clientSocket, go.c_str(), go.size(), 0);

	closesocket(clientSocket);
	ready[0] = true;

	Trial("[SockWrite] Writing has done\n");
	LogDump();
}

ThreadFIle::ThreadFIle() : b(1), write_buff() {}
ThreadFIle::ThreadFIle(std::string a, std::string buff) : b(0), write_buff(buff) {
	this->Write(a);
}

void ThreadFIle::Write(std::string path) {
	std::thread(Fun, path, &this->b, &this->write_buff).detach();
}

void ThreadFIle::Write(std::string path, std::string Buff) {
	std::thread(Fun, path, &this->b, &this->write_buff).detach();
}

void ThreadFIle::WriteJSON_Sock(std::string __Path, SOCKET clientSocket, std::string BodyWithHead) {
	std::thread(Fun2, __Path, clientSocket, BodyWithHead, &this->b, this->write_buff.Union()._interface).detach();
}

std::string ThreadFIle::Read() {
	return this->__stable.String();
}

std::string ThreadFIle::Read(std::string path) {
	Trial("[Reading] Reading Trial\n");
	LogDump();
	if (!this->b) {
		return this->Read();
	}
	this->b = 0;
	this->__stable.Kill();

	std::ifstream ifs(path);
	if (ifs.is_open()) {
		this->__stable.Again(std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()));
		ifs.close();
	}
	else {
		std::ofstream ofs(path);
		if (ofs.is_open()) {
			ofs.write("{}", 2);
			ofs.close();
		}

		this->__stable.Again("{}");
	}

	Trial("[Reading] Done!\n" << this->__stable.String());
	LogDump();
	this->b = 1;
	return this->__stable.String();
}