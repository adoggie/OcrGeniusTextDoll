#pragma once

#include <QtCore>
#include <QtWebSockets>
#include "Task.h"

class WebSocketServer : public QObject
{
	Q_OBJECT

public:
	explicit WebSocketServer(quint16 port, QObject* parent = nullptr)
		: QObject(parent)
	{
		server_ = new QWebSocketServer(QStringLiteral("WebSocket Server"),
			QWebSocketServer::NonSecureMode,
			this);
		if (!server_->listen(QHostAddress::Any, port)) {
			qWarning() << "Failed to start server:" << server_->errorString();
			return;
		}
		qDebug() << "Server started on port:" << port;

		connect(server_, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
		ocr_init(engine_);
	}

	~WebSocketServer()
	{
		server_->close();
		qDeleteAll(clients_);
	}

private slots:
	void onNewConnection()
	{
		QWebSocket* clientSocket = server_->nextPendingConnection();
		qDebug() << "New client connected:" << clientSocket->peerAddress().toString();

		connect(clientSocket, &QWebSocket::disconnected, this, &WebSocketServer::onClientDisconnected);

		clients_.insert(clientSocket);
		connect(clientSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::onTextMessageReceived);
		connect(clientSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::onBinaryMessageReceived);
	}

	void onTextMessageReceived(const QString& message)
	{
		QWebSocket* clientSocket = qobject_cast<QWebSocket*>(sender());
		qDebug() << "Received text message:" << message << "from client:" << clientSocket->peerAddress().toString();
		size_t size;

		QStringList fs = message.split(",");
		if (fs.size() < 4) {
			qWarning("message invalid. ");
			return;
		}
		QString app_id, sequence, type, body;
		app_id = fs[0];
		sequence = fs[1];
		type = fs[2];
		body = fs[3];

		QByteArray imageBytes = QByteArray::fromBase64(body.toUtf8());
		std::string result;

		int retcode = ocr_image(imageBytes, result, engine_);
		if (retcode != 0) {
			qErrnoWarning("ocr_image error:", retcode);
			result = "error";
		}

		// Echo the message back to the client
		if (clientSocket) {
			QString fmt = "%1,%2,%3,%4";
			QString b64encode = QByteArray(result.c_str(), result.size()).toBase64();
			QString content = fmt.arg(app_id).arg(sequence).arg(type).arg(b64encode);
			clientSocket->sendTextMessage(content);
		}
	}



	void onBinaryMessageReceived(const QByteArray& message)
	{
		QWebSocket* clientSocket = qobject_cast<QWebSocket*>(sender());
		qDebug() << "Received binary message with size:" << message.size() << "from client:" << clientSocket->peerAddress().toString();

		// Echo the message back to the client
		if (clientSocket) {
			clientSocket->sendBinaryMessage(message);
		}
	}

	void onClientDisconnected()
	{
		QWebSocket* clientSocket = qobject_cast<QWebSocket*>(sender());
		qDebug() << "Client disconnected:" << clientSocket->peerAddress().toString();

		if (clientSocket) {
			clients_.remove(clientSocket);
			clientSocket->deleteLater();
		}
	}

private:
	QWebSocketServer* server_;
	QSet<QWebSocket*> clients_;
	pv_ocrgeneralocr::OcrgeneralocrEngine engine_;
};

//int main(int argc, char* argv[])
//{
//    QCoreApplication app(argc, argv);
//
//    WebSocketServer server(8080);
//
//    return app.exec();
//}

//#include "main.moc"
