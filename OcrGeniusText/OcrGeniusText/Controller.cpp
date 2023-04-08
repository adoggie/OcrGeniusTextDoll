#include <QCoreApplication>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <zmq.h>
#include "Controller.h"
#include <thread>
#include "Task.h"
#include "websocket.h"

#define DEALER_ADDR "tcp://127.0.0.1:9911"


Controller::Controller()
{
}


/*
void Controller::worker_routine() {
	// Create a worker socket
	void* worker = zmq_socket(context_, ZMQ_PULL);
	zmq_connect(worker, DEALER_ADDR);
	printf("Worker started.\n");

	pv_ocrgeneralocr::OcrgeneralocrEngine engine;
	//ocr_init(engine);
	// Process tasks forever
	while (true) {
		// Receive a message from the worker socket
		zmq_msg_t message;
		zmq_msg_init(&message);
		zmq_msg_recv(&message, worker, 0);

		size_t size = zmq_msg_size(&message);
		std::vector<char> bytes(size + 1);
		char* data = (char*)zmq_msg_data(&message);
		bytes.assign(data, data + size);
		std::cout << bytes.data() << std::endl;
		QString str = bytes.data();
		QStringList fs = str.split(",");
		if (fs.size() < 4) {
			qWarning("message invalid. ", str);
			continue;
		}
		QString app_id, sequence, type, body;
		app_id = fs[0];
		sequence = fs[1];
		type = fs[2];
		body = fs[3];
		continue;
		QByteArray imageBytes = QByteArray::fromBase64(body.toUtf8());
		std::string result;
		//int retcode = ocr_image(imageBytes, result, engine);
		//if (retcode != 0) {
		//	break;
		//}

		// Send a message back to the worker socket
		zmq_msg_t reply;
		zmq_msg_init_size(&reply, 5);
		memcpy(zmq_msg_data(&reply), "OK", 2);
		zmq_msg_send(&reply, worker, 0);
	}

	//ocr_clean(engine);

	// We never get here
	zmq_close(worker);
}


bool  Controller::initMx() {
	std::cout << " initMx .." << std::endl;
	context_ = zmq_ctx_new();
	pub_sock_ = zmq_socket(context_, ZMQ_PUB);
	std::cout << "inited pub sock.." << std::endl;

	// Create a frontend socket to receive messages from clients
	void* frontend = zmq_socket(context_, ZMQ_ROUTER);
	zmq_bind(frontend, vars_.mxTask.c_str());

	// Create a backend socket to send messages to workers
	void* backend = zmq_socket(context_, ZMQ_DEALER);
	zmq_bind(backend, DEALER_ADDR);

	std::cout << "router/dealer binded .." << std::endl;

	// Create worker threads
	std::vector<std::thread> threads;
	int num_threads = vars_.workers;

	// Create the worker threads
	for (int i = 0; i < num_threads; i++) {
		threads.push_back(std::thread(&Controller::worker_routine, this));
	}

	std::cout << "Created " << num_threads << " worker threads." << std::endl;

	// Wait for the worker threads to finish
	for (auto& thread : threads) {
		thread.join();
	}

	std::cout << "Server started.\n";
	zmq_proxy(frontend, backend, NULL);
	zmq_close(frontend);
	zmq_close(backend);
	zmq_ctx_destroy(context_);
	return true;
}
*/

bool Controller::init(const ControllerVars& vars) {
	vars_ = vars;
	//initMx();
	webserver_ = new WebSocketServer(1944);
	return false;
}

void Controller::run() {

}

void Controller::stop() {

}


bool Controller::initWebsocket() {
	//QObject::connect(&webSocket_, &QWebSocket::connected, [&]() {
	//	qDebug() << "WebSocket connected";
	//	});

	//QObject::connect(&webSocket_, &QWebSocket::disconnected, [&]() {
	//	qDebug() << "WebSocket disconnected";
	//	});

	//QObject::connect(&webSocket_, &QWebSocket::textMessageReceived, [&](const QString& message) {
	//	qDebug() << "Received text message:" << message;
	//	});

	//QObject::connect(&webSocket_, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), [&](QAbstractSocket::SocketError error) {
	//	qDebug() << "WebSocket error:" << error;
	//	});

	//// Open a connection to the WebSocket server
	//QUrl url(QStringLiteral("ws://example.com:1234"));
	//webSocket_.open(url);

	return true;
}