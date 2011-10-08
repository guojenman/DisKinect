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

#include "Constants.h"


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
		if( recorder ) {
			recorder->exit();
			delete recorder; recorder = NULL;
		}

		if( player ) {
			player->exit();
			delete player; player = NULL;
		}

		current = NULL;
		ni = NULL;
		tracker = NULL;
	}

	void UserStreamRepeater::enter() {
		ni = WuCinderNITE::getInstance();
		tracker = UserTracker::getInstance();
		recorder = new UserStreamRecorder();
		recorder->enter();
		current = recorder;

		_cumalitiveDelta = 0;
		_state = WAITING_TO_RECORD;
	}

	void UserStreamRepeater::update() {

		static int frameCounter;
		if( getState() == WAITING_TO_RECORD && canStartRecording() ) {
			std::cout << "UserStreamRepeater: Starting recording!" << std::endl;
			recorder->startRecording();
			current = recorder;
			_numberOfFramesToRecord = ci::Rand::randInt( Constants::relay::repeater::MIN_FRAMES_TO_RECORD, Constants::relay::repeater::MAX_FRAMES_TO_RECORD );

			setState( RECORDING );
		}

		// Check if we've recorded enough
		if( getState() == RECORDING && ++frameCounter == _numberOfFramesToRecord) {

			std::cout << "UserStreamRepeater: Stopping recording!" << std::endl;

			// Stop recording, get json, and destroy recorder
			recorder->stopRecording();
			_recording = recorder->getRecordAsJSONValue();
			recorder->exit();

			// Start playback
			player = new UserStreamPlayer();
			player->setJson( &_recording );
			player->enter();
			current = player;

			setState( FIRST_PLAYBACK );
		}

		if(current)
			current->update();
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

	void UserStreamRepeater::setState( REPEATER_STATE aState ) {
		// Some fancy stuff goes here
		_state = aState;
	}

	bool UserStreamRepeater::canStartRecording() {
//		std::cout << "Current Delta: " << tracker->getTotalDist() << std::endl;
		_cumalitiveDelta += tracker->getTotalDist();
		return getSkeleton().isTracking && _cumalitiveDelta > Constants::relay::repeater::MIN_CUMALTIVE_DELTA_BEFORE_RECORDING;
	}
}



