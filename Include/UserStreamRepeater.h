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

// Forward declerations
namespace SKELETON {
	struct SKELETON;
	struct SKELETON_JOINT;
}
class WuCinderNITE;
class UserTracker;

namespace relay {
	class UserStreamRecorder;
	class UserStreamPlayer;

	class UserStreamRepeater : public IUserStream {
		public:
			UserStreamRepeater();
			virtual ~UserStreamRepeater();

			void enter();
			void update();
			void draw();
			void exit();
			SKELETON::SKELETON getSkeleton();

		private:
			void deallocStates();
			UserStreamRecorder* recorder;
			UserStreamPlayer* player;
			IUserStream* current;

			bool _didStartRecording;
			bool _didFinishRecording;
	};
}
#endif /* USERSTREAMREPEATER_H_ */
