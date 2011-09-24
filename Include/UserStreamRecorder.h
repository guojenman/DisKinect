/*
 * UserStreamRecorder.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
 *      Abstract:
 *      	 This represents one of the states in the UserStreamStateManager
 *      	 It is responsible for retrieving live skeleton data from the kinect
 */

#ifndef UserStreamRecorder_H_
#define UserStreamRecorder_H_

#include "IUserStream.h"
#include "UserStreamLive.h"

namespace relay {

class UserStreamRecorder : public IUserStream  {
	public:
		UserStreamRecorder();
		virtual ~UserStreamRecorder();

		void enter();
		void update();
		void draw();
		void exit();
		WuCinderNITE::SKELETON getSkeleton();

	private:
		UserStreamLive *_livestream;	// When queried for skeleton data we just send whatever our livestream has
	};
}

#endif /* UserStreamRecorder_H_ */
