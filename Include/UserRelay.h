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
 *      	and ask the UserStreamStateMachine for skeleton information each frame
 */

#ifndef USERRELAY_H_
#define USERRELAY_H_



#include "UserStreamStateManager.h"
#include "cinder/Vector.h"
#include "json/json.h"
#include "cinder/app/MouseEvent.h"

// Forward declerations
namespace SKELETON {
	struct SKELETON;
	struct SKELETON_JOINT;
}
class WuCinderNITE;
class UserTracker;
namespace mowa { namespace sgui { class SimpleGUI; } }

namespace relay {
	class UserRelay {
	public:
		UserRelay( WuCinderNITE* t_ni, UserTracker* t_tracker );
		virtual ~UserRelay();

		void setupDebug();
		void update();
		SKELETON::SKELETON getSkeleton();

		void renderDepthMap();
		void renderSkeleton();
		void renderGUI();
		void draw();

	private:
		WuCinderNITE* ni;
		UserTracker* tracker;
		relay::UserStreamStateManager* fsm;

		// Debug
		mowa::sgui::SimpleGUI* _debugGUI;

		void debugDraw();
		bool setStateLive( ci::app::MouseEvent event );
		bool setStateRecorder( ci::app::MouseEvent event );
		bool setStatePlayback( ci::app::MouseEvent event );
	};

} /* relay */
#endif /* USERRELAY_H_ */
