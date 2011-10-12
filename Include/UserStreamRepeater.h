/*
 * UserStreamRepeater.h
 *
 *  Created on: Oct 4, 2011
 *      Author: mariogonzalez
 *      Abstract:
 *      	This state is able to record user movement, then play it back to them.
 */

#ifndef USERSTREAMREPEATER_H_
#define USERSTREAMREPEATER_H_

#include "IUserStream.h"
#include "SkeletonStruct.h"
#include "json/json.h"
#include "boost/bind.hpp"
#include "boost/signals2.hpp"
#include <XnTypes.h>

// Forward declerations
namespace SKELETON {
	struct SKELETON;
	struct SKELETON_JOINT;
}
class WuCinderNITE;
class UserTracker;
//
namespace relay {
	class UserStreamRecorder;
	class UserStreamPlayer;

	class UserStreamRepeater : public IUserStream {
		public:
			enum REPEATER_STATE {
				WAITING_TO_RECORD,
				RECORDING,
				FIRST_PLAYBACK,
				PLAYBACK_LOOP
			};

			UserStreamRepeater();
			virtual ~UserStreamRepeater();

			void enter();
			void update();
			void draw();
			void exit();

			SKELETON::SKELETON getSkeleton();

		private:
			UserStreamRecorder* recorder;			// Records userstream
			UserStreamPlayer* player;				// Players userstream
			IUserStream* current;					// recorder/player

			REPEATER_STATE _state;					// Current state ( REPEATER_STATE )
			void setState( REPEATER_STATE aState );
			REPEATER_STATE getState() { return _state; };

			void startRecording();
			void stopRecording();

			Json::Value _recording;					// In memory json recording fed to 'player'
			int _numberOfFramesToRecord;

			float _cumalitiveDelta;
			int _frameCounter;
			bool canStartRecording();		// Atleast a user, and atleast some movement

			boost::signals2::connection	_signalConnectionNewUser;
			boost::signals2::connection	_signalConnectionLostUser;

			void onNewUser(XnUserID nId);
			void onLostUser(XnUserID nId);
	};
}
#endif /* USERSTREAMREPEATER_H_ */
