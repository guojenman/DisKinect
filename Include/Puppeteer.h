#pragma once
#ifndef PUPPETIER_H_
#define PUPPETIER_H_

#include "SkeletonStruct.h"

#include "ArduinoCommandInterface.h"

namespace puppeteer {

class Puppeteer {
public:
	Puppeteer();
	virtual ~Puppeteer();

	void update(SKELETON::SKELETON& skeleton);

private:
	ArduinoCommandInterface* arduino;
	double updateInterval;
	double lastUpdateTime;
	float arduinoUnit;

};

} /* namespace puppetier */
#endif /* PUPPETIER_H_ */
