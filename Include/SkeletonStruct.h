
#pragma once
#ifndef SKELETONSTRUCT_H_
#define SKELETONSTRUCT_H_

#include "cinder/Vector.h"
#include "cinder/Matrix33.h"
namespace SKELETON {
	struct SKELETON_JOINT {
			SKELETON_JOINT():confidence(0.0){};
			float confidence;
			ci::Vec3f position;
			ci::Matrix33<float> orientation;
	};
	struct SKELETON {
			bool isTracking;
			SKELETON_JOINT joints[25];
	};
}
#endif /* SKELETONSTRUCT_H_ */
