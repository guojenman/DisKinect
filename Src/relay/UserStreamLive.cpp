/*
 * UserStreamLive.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
 *      Abstract:
 *      	 This represents one of the states in the UserStreamStateManager
 *      	 It is responsible for retrieving live skeleton data from the kinect
 */

#include "UserStreamLive.h"

namespace relay {

	UserStreamLive::UserStreamLive() {
		// TODO Auto-generated constructor stub

	}

	UserStreamLive::~UserStreamLive() {
		// TODO Auto-generated destructor stub
	}

	void UserStreamLive::enter() {
		ni = WuCinderNITE::getInstance();
		tracker = UserTracker::getInstance();
	}

	void UserStreamLive::update() {

	}

	WuCinderNITE::SKELETON UserStreamLive::getSkeleton() {

		WuCinderNITE::SKELETON aSkeleton;
		if( tracker->activeUserId != 0 ) {
			aSkeleton = ni->skeletons[ tracker->activeUserId ];
		} else { // is false by default?
			aSkeleton.isTracking = false;
		}

		return aSkeleton;
	}

	void UserStreamLive::exit() {
		ni = NULL;
		tracker = NULL;
	}
}
