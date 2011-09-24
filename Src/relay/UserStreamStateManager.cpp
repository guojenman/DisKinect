/*
 * UserStreamStateManager.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
 */

#include "UserStreamStateManager.h"


namespace relay {

	UserStreamStateManager::UserStreamStateManager() {
		// TODO Auto-generated constructor stub
	}

	UserStreamStateManager::~UserStreamStateManager() {
		// TODO Auto-generated destructor stub
		delete currentState;
		delete previousState;
		delete nextState;
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

	WuCinderNITE::SKELETON UserStreamStateManager::getSkeleton() {
		return currentState->getSkeleton();
	}
}
