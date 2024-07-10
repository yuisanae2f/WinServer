#include "WinServer.h"
#include "JsonMgr.h"

int main() {
	Json::cReadMgr read("{\"Hello World!\" : 15}");
	std::cout << read.Union().classDictionary->dic["Hello World!"]->String();

	return 0;
}