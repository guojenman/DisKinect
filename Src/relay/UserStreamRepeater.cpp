/*
 * UserStreamRepeater.cpp
 *
 *  Created on: Oct 4, 2011
 *      Author: mariogonzalez
 */

#include "UserStreamRepeater.h"

#include "WuCinderNITE.h"
#include "UserTracker.h"
#include "SkeletonStruct.h"

#include "UserStreamRecorder.h"
#include "UserStreamPlayer.h"


namespace relay {
	UserStreamRepeater::UserStreamRepeater() {
		// TODO Auto-generated constructor stub
		player = NULL;
		recorder = NULL;
		current = NULL;
		ni = NULL;
		tracker = NULL;
	}

	UserStreamRepeater::~UserStreamRepeater() {
		// TODO Auto-generated destructor stub
		deallocStates();
		ni = NULL;
		tracker = NULL;
	}

	void UserStreamRepeater::enter() {
		ni = WuCinderNITE::getInstance();
		tracker = UserTracker::getInstance();
		recorder = new UserStreamRecorder();
		recorder->enter();
		current = recorder;
		_didStartRecording = false;
//
//		_didFinishRecording = _didMakeRecording = false;
	}

	void UserStreamRepeater::update() {

		static int frameCounter;
		if( getSkeleton().isTracking && !_didStartRecording) {
			std::cout << "Starting recording" << std::endl;
			recorder->startRecording();
			current = recorder;
			_didStartRecording = true;
		}

		if( recorder->getState() == UserStreamRecorder::RECORDING && ++frameCounter == 75) {
			std::cout << "Stop recording!" << std::endl;
			recorder->stopRecording();
		}
	}

	void UserStreamRepeater::draw() {
		if( current ) current->draw();
	}

	SKELETON::SKELETON UserStreamRepeater::getSkeleton() {
		if( current ) {
			return current->getSkeleton();
		}

		SKELETON::SKELETON aSkeleton;
		if( tracker->activeUserId != 0 ) {
			aSkeleton = ni->skeletons[ tracker->activeUserId ];
		} else { // is false by default?
			aSkeleton.isTracking = false;
		}

		return aSkeleton;
	}

	void UserStreamRepeater::exit() {
		ni = NULL;
		tracker = NULL;
	}

	// Clean up
	void UserStreamRepeater::deallocStates() {
		if( recorder ) {
			recorder->exit();
			delete recorder; recorder = NULL;
		}

		if( player ) {
			player->exit();
			delete player; player = NULL;
		}

		current = NULL;
	}
}



