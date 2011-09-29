/*
 * UserStreamPlayer.h
 *
 *  Created on: Sep 26, 2011
 *      Author: mariogonzalez
 *      Abstract:
 *      	 This represents one of the states in the UserStreamStateManager
 *      	 It is responsible for playing a json skeleton recording
 */

#ifndef USERSTREAMPLAYER_H_
#define USERSTREAMPLAYER_H_

#include "IUserStream.h"
#include "UserStreamFrame.h"
#include "json/value.h"


// Forward declerations
namespace SKELETON {
	struct SKELETON;
	struct SKELETON_JOINT;
}
class WuCinderNITE;
class UserTracker;


namespace relay {
	class UserStreamPlayer : public IUserStream {
		enum PlayerState { PLAYING, PAUSED };

	public:
		UserStreamPlayer();
		virtual ~UserStreamPlayer();

		void enter();
		void update();
		void draw();
		void exit();
		SKELETON::SKELETON getSkeleton();

		void play();
		void pause();
		void restart();

		///// ACCESSORS
		void setJson( const std::string &aPath );
		void setJson( Json::Value *aJsonValue );

	private:
		PlayerState _state;
		int _currentFrame;
		int _totalframes;
		std::vector<UserStreamFrame*> _recording;

		Json::Value _json;
		std::string _jsonString;
		void parseJson( Json::Value* aJsonValue );
	};
}

#endif /* USERSTREAMPLAYER_H_ */
