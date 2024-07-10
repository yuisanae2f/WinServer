#pragma once

#include "JsonMgr.h"
#ifndef QUERY_MGR_HPP
#define QUERY_MGR_HPP
	
namespace Query {
	void Parse(Json::iObj&, std::string&);
	void Parse(Json::iObj&, std::string&&);
}

#endif // !QUERY_MGR_HPP
