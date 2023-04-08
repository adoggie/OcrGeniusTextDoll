#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QThreadPool>
#include <QtCore/QRunnable>
#include <QtCore/QDebug>

#include "ocrgeneralocr_engine.h"
#include "ocrgeneralocr_response.h"
#include "ocrgeneralocr_status.h"
#include "ocrgeneralocr_license.h"
#include <opencv2/opencv.hpp>


int ocr_init(pv_ocrgeneralocr::OcrgeneralocrEngine& engine);
int ocr_clean(pv_ocrgeneralocr::OcrgeneralocrEngine& engine);
int ocr_image(const QByteArray& image, std::string& result, pv_ocrgeneralocr::OcrgeneralocrEngine& engine);