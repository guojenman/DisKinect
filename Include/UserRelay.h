/*
 * UserRelay.h
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
 */

#ifndef USERRELAY_H_
#define USERRELAY_H_

#include "WuCinderNITE.h"
#include "UserTracker.h"
#include "UserStreamStateManager.h"
#include "cinder/Camera.h"
#include "cinder/Vector.h"

namespace relay {
	class UserRelay {
	public:
		UserRelay( WuCinderNITE* t_ni, UserTracker* t_tracker );
		virtual ~UserRelay();

		void update();
		WuCinderNITE::SKELETON getSkeleton();

		void renderDepthMap();
		void renderSkeleton();
		void draw();

	private:
		WuCinderNITE* ni;
		UserTracker* tracker;

		relay::UserStreamStateManager* fsm;

		// Debug
		ci::CameraPersp mCam;
		ci::Vec3f mCamEye;
		ci::Vec3f mCamLookAt;
	};

} /* relay */
#endif /* USERRELAY_H_ */
