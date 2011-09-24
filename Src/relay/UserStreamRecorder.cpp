/*
 * UserStreamRecorder.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
 *      Abstract:
 *      	 This represents one of the states in the UserStreamStateManager
 *      	 It is responsible for retrieving live skeleton data from the kinect
 */

#include "UserStreamRecorder.h"

namespace relay {

	UserStreamRecorder::UserStreamRecorder() {}
	UserStreamRecorder::~UserStreamRecorder() {
		delete _livestream;
	}

	void UserStreamRecorder::enter() {
		ni = WuCinderNITE::getInstance();
		tracker = UserTracker::getInstance();

		_livestream = new UserStreamLive();
		_livestream->enter();
	}

	void UserStreamRecorder::update() {
		_livestream->update();
	}

	void UserStreamRecorder::draw() {
		_livestream->draw();
	}

	WuCinderNITE::SKELETON UserStreamRecorder::getSkeleton() {
		return _livestream->getSkeleton();
	}

	void UserStreamRecorder::exit() {
		ni = NULL;
		tracker = NULL;

		delete _livestream;
	}
}
