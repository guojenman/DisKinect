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
#include "cinder/Rand.h"

#include "boost/bind.hpp"
#include <boost/lambda/lambda.hpp>



namespace relay {

	UserStreamLive::UserStreamLive() {
		// TODO Auto-generated constructor stub
		std::cout << "UserStreamLive Constructor!" << std::endl;
	}

	UserStreamLive::~UserStreamLive() {
		// TODO Auto-generated destructor stub
		_signalConnectionLostUser.disconnect();
		_signalConnectionNewUser.disconnect();
		std::cout << "UserStreamLive destructor!" << std::endl;
	}

	void UserStreamLive::enter() {
		ni = WuCinderNITE::getInstance();
		tracker = UserTracker::getInstance();

		_framesActive = 0;
		_framesInactive = 0;
		_activeUserID = 0;

//		_signalConnectionLostUser = ni->signalLostUser.connect( boost::bind( &UserStreamLive::onLostUser, this, boost::lambda::_1 ) );
//		_signalConnectionNewUser = ni->signalNewUser.connect( boost::bind( &UserStreamLive::onNewUser, this, boost::lambda::_1 ) );
	}

	void UserStreamLive::update() {}
	void UserStreamLive::draw(){

	}

	SKELETON::SKELETON UserStreamLive::getSkeleton() {


		if( _activeUserID != tracker->activeUserId ) {
			onNewUser( tracker->activeUserId );
		}

		SKELETON::SKELETON aSkeleton;
		if( _activeUserID != 0 && ++_framesActive > Constants::relay::FRAMES_BEFORE_CONSIDERED_REAL_USER ) {
			aSkeleton = ni->skeletons[ _activeUserID ];
		} else { // is false by default?
			aSkeleton.isTracking = false;
			++_framesInactive;
		}

		return aSkeleton;
	}

	void UserStreamLive::exit() {
		ni = NULL;
		tracker = NULL;
	}

	void UserStreamLive::onNewUser(XnUserID id ) {
		std::cout << "UserStreamLive::newUser - newUserID " << id << "ActiveUserID" << tracker->activeUserId << std::endl;
		_activeUserID = tracker->activeUserId;

		_framesActive = 0;
		_framesInactive = 0;
	}

	void UserStreamLive::onLostUser(XnUserID id ) {

		std::cout << "UserStreamLive - lostUser!" << std::endl;
	}

	// not
	bool UserStreamLive::wantsToExit() {
		float chance = ci::Rand::randFloat();
		return _framesInactive >= Constants::relay::FRAMES_BEFORE_PLAYING_RECORDING
				&& chance <  Constants::relay::CHANCE_OF_PLAYING_RECORDING_WHEN_IDLE;
	};
}
