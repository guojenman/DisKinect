#pragma once
#ifndef PUPPETIER_H_
#define PUPPETIER_H_

#include "cinder/Matrix22.h"
#include "cinder/Matrix33.h"
#include "cinder/Matrix44.h"
#include "SkeletonStruct.h"

#include "ArduinoCommandInterface.h"

namespace puppeteer {

class Puppeteer {
public:
	Puppeteer();
	virtual ~Puppeteer();

	void update(SKELETON::SKELETON& skeleton);
	void draw();
private:
	ArduinoCommandInterface* arduino;
	double updateInterval;
	double lastUpdateTime;
	float arduinoUnit;

	float armLenL;
	float armLenR;
	ci::Matrix33<float> normalizationMatrix;
	ci::Vec3f shoulderL;
	ci::Vec3f shoulderR;
	ci::Vec3f handL;
	ci::Vec3f handR;
	ci::Vec3f axisHoriz;
	ci::Vec3f axisVert;
	ci::Vec3f normal;

	ci::Vec3f lastHandPosL;
	ci::Vec3f lastHandPosR;
	float lastLegPosL;
	float lastLegPosR;
};

} /* namespace puppetier */
#endif /* PUPPETIER_H_ */
