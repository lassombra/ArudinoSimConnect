#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <locale>
#include <codecvt>
#include "Serial.h"

class MyFrame;
class ConnectionManager
{
public:
	ConnectionManager();
	void processTimerEvent();
	void setFrame(MyFrame*);
	void startConnection();
	void endConnection();
	void sendMessage(const char*);
private:
	void startConnection(int, CEnumerateSerial::CPortAndNamesArray, std::vector<std::shared_ptr<Serial>>);
	std::shared_ptr<Serial> connection;
	std::function<void()> pendingFunc;
	void tryConnection(std::vector<std::shared_ptr<Serial>>);
	void tryConnection(std::vector<std::shared_ptr<Serial>>, short);
	void readStatus();
	MyFrame* frame;
	std::string message;
};

template<typename ... Args>
std::wstring string_format(const std::string& format, Args ... args)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	std::unique_ptr<char[]> buf(new char[size]);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return converter.from_bytes(
		std::string(buf.get(), buf.get() + size - 1)
	);
}
