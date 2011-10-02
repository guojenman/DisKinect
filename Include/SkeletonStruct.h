
#pragma once
#ifndef SKELETONSTRUCT_H_
#define SKELETONSTRUCT_H_

#include "cinder/Vector.h"
namespace SKELETON {
	struct SKELETON_JOINT {
			SKELETON_JOINT():confidence(0.0){};
			float confidence;
			ci::Vec3f position;
	};
	struct SKELETON {
			bool isTracking;
			SKELETON_JOINT joints[25];
	};
}
#endif /* SKELETONSTRUCT_H_ */
