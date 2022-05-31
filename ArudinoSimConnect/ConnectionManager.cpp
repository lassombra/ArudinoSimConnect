#include "ConnectionManager.h"
#include "MyFrame.h"
#include <sstream>

std::function<void()> noop = []() {};
void ConnectionManager::processTimerEvent()
{
    auto currentFunc = this->pendingFunc;
    this->pendingFunc = noop;
    currentFunc();
}

ConnectionManager::ConnectionManager()
{
    this->pendingFunc = noop;
}

void ConnectionManager::setFrame(MyFrame* frame)
{
	this->frame = frame;
}

void ConnectionManager::startConnection()
{
    auto ports = selectComPort();
    std::vector<std::shared_ptr<Serial>> connections;
    this->pendingFunc = [this, connections, ports]() {this->startConnection(0, ports, connections);};
}

void ConnectionManager::startConnection(int idx, CEnumerateSerial::CPortAndNamesArray ports, std::vector<std::shared_ptr<Serial>> connections) {
    if (idx >= ports.size()) {
        this->pendingFunc = [connections, this]() {
            this->tryConnection(connections);
        };
    }
    else {
        auto portName = string_format("\\\\.\\COM%u", ports[idx].first);
        auto buf = std::wstringstream();
        std::shared_ptr<Serial> port(new Serial(portName.c_str()));
        if (port->IsConnected()) {
            if (connections.size() > 0) {
                buf << "\n";
            }
            buf << string_format("COM%u <%ls>", ports[idx].first, ports[idx].second.c_str());
            connections.push_back(port);
        }
        this->pendingFunc = [this, idx, ports, connections]() {
            this->startConnection(idx + 1, ports, connections);
        };
    }
}

void ConnectionManager::endConnection()
{
    if (this->connection && this->connection->IsConnected()) {
        this->connection = std::shared_ptr<Serial>();
    }
    this->frame->setLabel("Disconnected");
    this->frame->SetStatusText("Disconnected");
}

void ConnectionManager::sendMessage(const char* message) {
    if (this->connection && this->connection->IsConnected()) {
        auto string = std::string(message);
        string += "|";
        this->connection->WriteData(string.c_str(), string.length());
    }
}

void ConnectionManager::readStatus()
{
    if (!connection || !connection->IsConnected()) {
        this->frame->setLabel("Disconnected");
        this->frame->SetStatusText("Disconnected");
        return;
    }
    char buff[255];
    auto read = connection->ReadData(buff, 254);
    buff[read] = '\0';
    if (read > 0) {
        std::string string(buff);
        auto idx = string.find_last_of('|');
        if (idx == string.size() - 1) idx = string.find_last_of('|', idx-1);
        if (idx != (string.size() - 1) && idx != string.npos) string = string.substr(idx +1);
        message += string;
        idx = message.find_last_of('|');
        if (idx == message.size() - 1) {
            frame->setLabel(message.substr(0, message.size()-1));
            message = std::string();
        }
    }
    this->pendingFunc = [this]() {this->readStatus();};
}

void ConnectionManager::tryConnection(std::vector<std::shared_ptr<Serial>> list, short count)
{
    auto it = list.begin();
    if (it == list.end()) return;
    auto conn = *it;
    char buff[256];
    auto read = conn->ReadData(buff, 255);
    if (read > 0) {
        buff[read] = 0;
        std::string string(buff);
        if (string.find("2560") != std::string::npos) {
            this->connection = conn;
            this->frame->setLabel("Connected");
            this->frame->SetStatusText("Cconnected");
            this->sendMessage("status");
            this->pendingFunc = [this]() {
                this->readStatus();
            };
        }
    }
    else {
        count++;
        if (count > 10) {
            it = list.erase(it);
            if (it != list.end()) {
                this->pendingFunc = [list, this]() {
                    this->tryConnection(list);
                };
            }
        }
        else {
            this->pendingFunc = [list, count, this]() {
                this->tryConnection(list, count);
            };
        }
    }
}

void ConnectionManager::tryConnection(std::vector<std::shared_ptr<Serial>> list)
{
    auto it = list.begin();
    if (it == list.end()) return;
    auto conn = *it;
    char buff[256];
    while (conn->ReadData(buff, 255) > 0) {}
    conn->WriteData("hello", 5);
    tryConnection(list, 0);
}
