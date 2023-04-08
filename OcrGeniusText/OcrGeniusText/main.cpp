#include <QtCore/QCoreApplication>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

#include "Controller.h"

bool initEnv(ControllerVars& vars) {
	try {

		// 从文件中读取JSON
		QFile loadFile("c:/sdk-ocr/settings.json");
		if (!loadFile.open(QIODevice::ReadOnly)) {
			qDebug(" read settings.json error. ", loadFile.errorString());
			return false;
		}

		QJsonDocument loadDoc(QJsonDocument::fromJson(loadFile.readAll()));
		QJsonObject loadObj = loadDoc.object();

		// 打印加载的JSON
		qDebug() << "Loaded JSON:";
		qDebug() << loadObj["name"].toString() << loadObj["age"].toInt();
		vars.workers = loadObj["workers"].toInt();
		vars.mxTask = loadObj["mxTask"].toString().toStdString();
		vars.mxResult = loadObj["mxResult"].toString().toStdString();

		loadFile.close();
	}
	catch (...) {
		qDebug() << "Unknown error occurred.";
		return false;
	}
	return true;
}

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	ControllerVars vars;
	if (!initEnv(vars)) {
		return -1;
	}
	Controller controller;
	controller.init(vars);
	//controller.run();

	//test_one_image("c:/data/images/2.jpg");
	return a.exec();
	//return 0;
}
