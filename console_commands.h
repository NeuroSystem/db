#pragma once
#include <iostream>

bool isExit(std::string command) { return command == "exit"; }

bool isPort(std::string command) { return command == "port"; }

bool isStart(std::string command) { return command == "start"; }

bool isStop(std::string command) { return command == "stop"; }

bool isSql(std::string command) { return command == "/sql"; }

bool isHelp(std::string command) { return command == "/?"; }

bool isTestConnections(std::string command) { return command == "test_connections"; }