/*
 * UserStreamLive.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
 *      Abstract:
 *      	 This represents one of the states in the UserStreamStateManager
 *      	 It is responsible for retrieving live skeleton data from the kinect
 */

#ifndef USERSTREAMLIVE_H_
#define USERSTREAMLIVE_H_

#include "IUserStream.h"

// Forward declerations
namespace SKELETON {
	struct SKELETON;
	struct SKELETON_JOINT;
}
class WuCinderNITE;
class UserTracker;

namespace relay {

class UserStreamLive : public IUserStream  {
	public:
		UserStreamLive();
		virtual ~UserStreamLive();

		void enter();
		void update();
		void draw();
		void exit();
		SKELETON::SKELETON getSkeleton();
	};
}

#endif /* USERSTREAMLIVE_H_ */
