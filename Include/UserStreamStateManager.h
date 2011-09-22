/*
 * UserStreamStateManager.h
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
 */

#ifndef USERSTREAMSTATEMANAGER_H_
#define USERSTREAMSTATEMANAGER_H_

#include "IUserStream.h"

namespace relay {

class UserStreamStateManager {
	public:
		UserStreamStateManager();
		virtual ~UserStreamStateManager();

		void setInitialState( IUserStream* aState );
		void changeState( IUserStream* aState );
		void gotoPreviousState();
		void gotoNextState();

		void update();
		WuCinderNITE::SKELETON getSkeleton();

	private:
		IUserStream* currentState;
		IUserStream* previousState;
		IUserStream* nextState;
	};
};

#endif /* USERSTREAMSTATEMANAGER_H_ */
