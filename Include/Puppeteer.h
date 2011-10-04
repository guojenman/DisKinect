#pragma once
#ifndef PUPPETIER_H_
#define PUPPETIER_H_

#include "SkeletonStruct.h"

namespace puppeteer {

class Puppeteer {
public:
	Puppeteer();
	virtual ~Puppeteer();

	void update(SKELETON::SKELETON& skeleton);
};

} /* namespace puppetier */
#endif /* PUPPETIER_H_ */
