/*
 * UserStreamStateManager.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
 */

#include "UserStreamStateManager.h"


#include "WuCinderNITE.h"
#include "UserTracker.h"
#include "SkeletonStruct.h"

namespace relay {

	UserStreamStateManager::UserStreamStateManager() {
		// TODO Auto-generated constructor stub
		currentState = NULL;
		previousState = NULL;
		nextState = NULL;
	}

	UserStreamStateManager::~UserStreamStateManager() {
		// TODO Auto-generated destructor stub
		delete currentState;
		delete previousState;
		delete nextState;

		std::cout << "UserStreamStateManager destructor!" << std::endl;
	}

	void UserStreamStateManager::setInitialState( IUserStream* aState ) {
		currentState = aState;
		currentState->enter();
	}

	void UserStreamStateManager::changeState( IUserStream* aState ) {

		if(aState != previousState)
			delete previousState;

		currentState->exit();
		previousState = currentState;

		currentState = aState;
		currentState->enter();
	}

	void UserStreamStateManager::gotoPreviousState() {
		changeState( previousState );
	}

	void UserStreamStateManager::gotoNextState()  {
		changeState( nextState );
	}

	void UserStreamStateManager::update() {

		// Not threaded... call update
		if( !WuCinderNITE::getInstance()->isThreaded() )
			WuCinderNITE::getInstance()->update();

		UserTracker::getInstance()->update();

		if( currentState ) {
			currentState->update();
		}
	}

	void UserStreamStateManager::draw() {
		if( currentState ) {
			currentState->draw();
		}
	}

	SKELETON::SKELETON UserStreamStateManager::getSkeleton() {
		return currentState->getSkeleton();
	}
}
