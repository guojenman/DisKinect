/*
 * UserStreamLive.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
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

		/*
		 * if (tracker->activeUserId != 0) {
			ni->renderSkeleton(tracker->activeUserId);
		}
		 */
		WuCinderNITE::SKELETON aSkeleton;
		aSkeleton.isTracking = false;
		return aSkeleton;
	}

	void UserStreamLive::exit() {
		ni = NULL;
		tracker = NULL;
	}
}
