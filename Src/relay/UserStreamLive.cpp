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

#include "WuCinderNITE.h"
#include "UserTracker.h"
#include "SkeletonStruct.h"


namespace relay {

	UserStreamLive::UserStreamLive() {
		// TODO Auto-generated constructor stub

	}

	UserStreamLive::~UserStreamLive() {
		// TODO Auto-generated destructor stub
		std::cout << "UserStreamLive destructor!" << std::endl;
	}

	void UserStreamLive::enter() {
		ni = WuCinderNITE::getInstance();
		tracker = UserTracker::getInstance();
	}

	void UserStreamLive::update() {}
	void UserStreamLive::draw(){

	}

	SKELETON::SKELETON UserStreamLive::getSkeleton() {

		SKELETON::SKELETON aSkeleton;
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
