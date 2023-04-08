#include "Task.h"

#include "ocrgeneralocr_engine.h"
#include "ocrgeneralocr_response.h"
#include "ocrgeneralocr_status.h"
#include "ocrgeneralocr_license.h"
#include <opencv2/opencv.hpp>
#include "time_now.hpp"
#include <iostream>

#define STOP_APPLICATION(status) do {char c = 0; std::cin>>c; return status;} while(0)
#include "Windows.h"
#include <tchar.h>
#include <direct.h>
#define CHECK_RET(ret) do{if (!ret) {char c; std::cin >> c; return 0;}} while(false)
//#define MAX_PATH 1024 //最长路径长度 


pv_ocrgeneralocr::OcrgeneralocrEngine g_ocrgeneralocr_engine;
int test_iamge(std::string image);
void write_file(std::string file, std::string content);



void write_file(std::string file, std::string content) {
	FILE* fp = fopen(file.c_str(), "wb");
	if (!fp) {
		return;
	}
	fputs(content.c_str(), fp);
	fclose(fp);
}
long long total_time = 0;
int times = 0;


int test_iamge(std::string image_name) {
	times++;
	long long time_now = TimeNow::get_mil_time_now();
	std::string image = image_name;
	cv::Mat img = cv::imread(image);
	std::vector<pv_ocrgeneralocr::PVResponse> responses;
	pv_ocrgeneralocr::PVStatus ret = g_ocrgeneralocr_engine.process(img, 0, responses);
	if (ret != pv_ocrgeneralocr::PVStatus::SUCCESS) {
		std::cout << "ret: " << ret << std::endl;
		//STOP_APPLICATION(-1);
	}
	total_time += (TimeNow::get_mil_time_now() - time_now);
	std::cout << "test_avg_cost_time=" << total_time / times << "ms" << std::endl;
	std::string result = image_name + "\n";
	for (auto it = responses.begin(); it != responses.end(); ++it) {
		std::cout << "识别结果 :" << it->result << std::endl;
		result = result +
			std::to_string(it->x1) + "\t" + std::to_string(it->y1) + "\t" +
			std::to_string(it->x2) + "\t" + std::to_string(it->y2) + "\t" +
			std::to_string(it->x3) + "\t" + std::to_string(it->y3) + "\t" +
			std::to_string(it->x4) + "\t" + std::to_string(it->y4) + "\t" +
			it->result + "\n";
	}
	std::cout << "result: " << result << std::endl;
	write_file(image_name + ".txt", result);
	return 0;
}

bool init_license(std::string license_folder) {
	vis_license::BDLicenseLocalInfo local_info = pv_ocrgeneralocr::powervision_auth_get_local_info();
	std::cout << "license_sdk_version: " << local_info._license_sdk_version << std::endl;
	std::cout << "device_id: " << local_info._device_id << std::endl;

	vis_license::ErrorCode license_ret = pv_ocrgeneralocr::powervision_auth_from_folder(license_folder, true);
	if (license_ret != vis_license::ErrorCode::SUCCESS) {
		std::cout << pv_ocrgeneralocr::powervision_auth_get_error_msg() << std::endl;
		STOP_APPLICATION(-1);
	}
	return true;
}

int ocr_init(pv_ocrgeneralocr::OcrgeneralocrEngine& engine) {
	static bool once = false;
	if (!once) {
		once = true;
	}
	else {
		return 0;
	}
	std::string license_folder = "C:\\sdk-ocr\\license";
	std::string model_folder = "C:/sdk-ocr/ocrgeneralocr-model";
	std::string type = "image"; // argv[3];
	bool auth_ret = init_license(license_folder);
	if (!auth_ret) {
		qDebug("init license failed!");
		return  -1;
	}
	pv_ocrgeneralocr::PVStatus ret = engine.init(model_folder, 4, 0);
	if (ret != pv_ocrgeneralocr::PVStatus::SUCCESS) {
		std::cout << "ret: " << ret << std::endl;
		qDebug("g_ocrgeneralocr_engine.init failed!");
		return  -1;
	}
	return 0;
}

int ocr_clean(pv_ocrgeneralocr::OcrgeneralocrEngine& engine) {
	engine.uninit();
	return 0;
}

int ocr_image(const QByteArray& image, std::string& result, pv_ocrgeneralocr::OcrgeneralocrEngine& engine) {

	times++;
	long long time_now = TimeNow::get_mil_time_now();
	//std::string image = image_name;
	//cv::Mat img = cv::imread(image);
	std::vector<char> bytes(image.constData(), image.constData() + image.size());

	cv::Mat img = cv::imdecode(bytes, cv::IMREAD_COLOR);

	std::vector<pv_ocrgeneralocr::PVResponse> responses;
	pv_ocrgeneralocr::PVStatus ret = engine.process(img, 0, responses);

	if (ret != pv_ocrgeneralocr::PVStatus::SUCCESS) {
		std::cout << "ret: " << ret << std::endl;
		return -1;
	}
	for (auto it = responses.begin(); it != responses.end(); ++it) {
		std::cout << "识别结果 :" << it->result << std::endl;
		result = result + it->result + "\n";

	}
	std::cout << "result: " << result << std::endl;

	return 0;

}