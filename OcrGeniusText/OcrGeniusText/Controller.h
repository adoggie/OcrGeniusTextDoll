#pragma once

#include <QCoreApplication>
#include <QWebSocket>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "websocket.h"

struct ControllerVars {
	int workers;
	std::string mxTask;
	std::string mxResult;
};

class Controller {
public:
	Controller();
	bool init(const ControllerVars& vars);
	void run();
	void stop();

protected:
	void worker_routine();
	bool initMx();
	bool initWebsocket();
private:
	ControllerVars vars_;
	void* context_;
	void* pub_sock_;
	WebSocketServer* webserver_;
};
