#include "JsonMgr.h"
#include <sstream>
#include "WinServer.h"

using namespace Json;

#pragma region iObj
void cElement<Interface>::Kill() {}

template<>
cElement<Interface>::~cElement() {
	this->Kill();
}
#pragma endregion
#pragma region cObj
std::string cElement<Obj>::String() {
	return this->raw;
}
eType cElement<Obj>::Type() {
	return Obj;
}
void cElement<Obj>::Kill() {}
#pragma endregion
#pragma region cStr
std::string cElement<Str>::String() {
	return std::string("\"") + cObj::String() + "\"";
}

eType cElement<Str>::Type() {
	return Str;
}
#pragma endregion
#pragma region cList
eType cElement<List>::Type() {
	return List;
}
std::string cElement<List>::String() {
	if (!this->vector.size()) return "[]";
	std::string Return = "[";
	for (size_t i = 0; i < this->vector.size() - 1; i++)
		Return += vector[i]->String() + ",";
	Return += this->vector[this->vector.size() - 1]->String();
	return (Return + "]");
}
void cElement<List>::Kill() {
	for (auto& el : this->vector) {
		delete el;
	}
}
#pragma endregion
#pragma region cDic
eType cDic::Type() {
	return Dic;
}

std::string cElement<Dic>::String() {
	if (!dic.size()) {
		return "{}";
	}
	std::string rtn = "{";

	for (auto it = this->dic.begin(); it != this->dic.end(); it++) {
		rtn += "\"" + it->first + "\":" + it->second->String() + ',';
	}
	rtn.end()[-1] = '}';

	return rtn;
}

void cElement<Dic>::Kill() {
	for (auto& pair : this->dic) {
		delete pair.second;
	}
}
#pragma endregion
#pragma region cReadMgr

cReadMgr::~cReadMgr() {
	this->Kill();
}

static void Parse_LIST(std::string& a, size_t& idx, cList& list);
static void Parse_DIC(std::string& a, size_t& idx, cDic& dic);
static void Parse_STR(std::string& a, size_t& idx, cStr& obj) {
	Trial("Parse_str: " << "\n");
	LogDump();

	for (idx++; idx < a.size() && a[idx] != '"'; idx++) {
		obj.raw.push_back(a[idx]);
	}

	Trial("parse str end: " << obj.String() << "\n");
	LogDump();
	return;
}
static void Parse_DIC(std::string& a, size_t& idx, cDic& dic) {
	Trial("Parse dictionary: " << "\n");
	LogDump();
	std::string buff = "";

	for (idx++; idx < a.size() && a[idx] != '}'; idx++) {
		switch (a[idx]) {
		case ' ': case '\n':
		case '\r': case '\t':
		case ',': case ':':
			break;
		case '{': {
			if (buff.size()) {
				cDic* obj = new cDic;
				dic.dic[buff] = obj;
				Parse_DIC(a, idx, obj[0]);
				buff = "";
			}
		} break;
		case '[': {
			if (buff.size()) {
				cList* obj = new cList;
				dic.dic[buff] = obj;
				Parse_LIST(a, idx, obj[0]);
				buff = "";
			}
		} break;
		case '"': {
			if (buff.size()) {
				// value getting
				cStr* obj = new cStr;
				dic.dic[buff] = obj;
				Parse_STR(a, idx, obj[0]);
				buff = "";
			}
			else {
				Trial("Parsing Key: " << "\n");
				LogDump();
				cStr obj;
				Parse_STR(a, idx, obj);
				buff = obj.raw;
			}
		} break;
		default: {
			if (buff.size()) {
				cObj* obj = new cObj;
				std::getline(std::istringstream(a.c_str() + idx), obj->raw, ',');
				std::getline(std::istringstream(obj->raw), obj->raw, '}');
				std::getline(std::istringstream(obj->raw), obj->raw, ']');
				std::getline(std::istringstream(obj->raw), obj->raw, ' ');
				std::getline(std::istringstream(obj->raw), obj->raw, '\r');
				std::getline(std::istringstream(obj->raw), obj->raw, '\n');
				std::getline(std::istringstream(obj->raw), obj->raw, '\t');
				std::getline(std::istringstream(obj->raw), obj->raw, ':');
				dic.dic[buff] = obj;
				idx += obj->raw.size();
				idx--;
				buff = "";
			}
		}
		}
	}

	Trial("Parse dic end: " << dic.String() << "\n");
	LogDump();
}
static void Parse_LIST(std::string& a, size_t& idx, cList& list) {
	Trial("Parse_list: " << "\n");
	LogDump();
	for (idx++; idx < a.size() && a[idx] != ']'; idx++) {
		switch (a[idx]) {
		case ' ': case '\n':
		case '\r': case '\t':
		case ',': case ':':
			break;

		case '[': {
			cList* obj = new cList;
			list.vector.push_back(obj);
			Parse_LIST(a, idx, obj[0]);
		} break;

		case '{': {
			cDic* obj = new cDic;
			list.vector.push_back(obj);
			Parse_DIC(a, idx, obj[0]);
		} break;

		case '"': {
			cStr* obj = new cStr;
			list.vector.push_back(obj);
			Parse_STR(a, idx, obj[0]);
		} break;

		default: {
			cObj* obj = new cObj;
			std::getline(std::istringstream(a.c_str() + idx), obj->raw, ',');
			std::getline(std::istringstream(obj->raw), obj->raw, '}');
			std::getline(std::istringstream(obj->raw), obj->raw, ']');
			std::getline(std::istringstream(obj->raw), obj->raw, ' ');
			std::getline(std::istringstream(obj->raw), obj->raw, '\r');
			std::getline(std::istringstream(obj->raw), obj->raw, '\n');
			std::getline(std::istringstream(obj->raw), obj->raw, '\t');
			std::getline(std::istringstream(obj->raw), obj->raw, ':');
			list.vector.push_back(obj);
			idx += obj->raw.size() - 1;
		} break;
		}
	}

	Trial("Parse list end: " << list.String() << "\n");
	LogDump();
}



iObj* cReadMgr::Parse(std::string a) {
	uALL obj = { 0 };
	size_t idx = 0;

#pragma region init
	for (bool go = 1; go && idx < a.size(); idx++) {
		switch (a[idx]) {
		case ' ': case '\n':
		case '\r': case '\t':
		case ',': case ':':
			break;
		default:
			go = 0;
			idx--;
		}
	}

	switch (a[idx]) {
	case '[': {
		obj.classList = new cList;
		Parse_LIST(a, idx, obj.classList[0]);
	} break;
	case '{': {
		obj.classDictionary = new cDic;
		Parse_DIC(a, idx, obj.classDictionary[0]);
	} break;
	case '"': {
		obj.classString = new cStr;
		Parse_STR(a, idx, obj.classString[0]);
	} break;
	default: {
		obj.classObject = new cObj;
		std::istringstream(a.c_str() + idx) >> obj.classObject->raw;
	} break;
	}
#pragma endregion

	return obj._interface;
}
cReadMgr::cReadMgr(std::string a) : raw(cReadMgr::Parse(a)) {}
void cReadMgr::Kill() {
	if (this->raw) {
		delete this->raw;
		this->raw = 0;
	}
}
void cReadMgr::Again(std::string a) {
	this->Kill();
	this->raw = this->Parse(a);
}

std::string cReadMgr::String() {
	return this->raw->String();
}
#pragma endregion
