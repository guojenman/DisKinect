/*
 * UserRelay.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
 *      Abstract:
 *      	This class contains a UserStreamStateMachine which can has the following or more states (classes):
 *      		* UserStreamLive
 *      		* UserStreamPlayback
 *      		* UserStreamRecorder
 *      	It determines which state the UserStreamStateMachine should be in,
 *      	and asks the UserStreamStateMachine for skeleton information each frame
 */

#include "UserRelay.h"
#include "cinder/app/App.h"
#include "UserStreamLive.h"
#include "UserStreamRecorder.h"

namespace relay {
	UserRelay::UserRelay( WuCinderNITE* t_ni, UserTracker* t_tracker ) {
		// TODO Auto-generated constructor stub
		this->ni = t_ni;
		this->tracker = t_tracker;

		// Create the FSM and set the initial state
		this->fsm = new relay::UserStreamStateManager();

		relay::UserStreamRecorder* live = new relay::UserStreamRecorder();
		this->fsm->setInitialState( live );

		mCamEye = ci::Vec3f(0, 0, -500.0f);
		mCamLookAt = ci::Vec3f::zero();
	}

	UserRelay::~UserRelay() {
		tracker->release();
		ni->shutdown();

		delete fsm;

		std::cout << "UserRelay destructor!" << std::endl;
	}

	void UserRelay::update() {
		WuCinderNITE::SKELETON aSkeleton;
		mCam.setPerspective(60.0f, cinder::app::App::get()->getWindowAspectRatio(), 1.0f, ni->maxDepth);
		mCam.lookAt(mCamEye, mCamLookAt);
		fsm->update();
	}

	void UserRelay::draw(){
		fsm->draw();
	}

	WuCinderNITE::SKELETON UserRelay::getSkeleton() {
		return fsm->getSkeleton();
	}

	void UserRelay::renderDepthMap() {
		ni->renderDepthMap( cinder::app::App::get()->getWindowBounds() );
	}

	void UserRelay::renderSkeleton() {
		WuCinderNITE::SKELETON aSkeleton = 	getSkeleton();

		ci::gl::pushMatrices();
		ci::gl::setMatrices(mCam);
			ni->renderSkeleton( aSkeleton , 0 );
		ci::gl::popMatrices();
	}
}
