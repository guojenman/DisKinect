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
#include "cinder/app/MouseEvent.h"
#include "cinder/app/FileDropEvent.h"
#include <boost/signals2.hpp>

// Forward declerations
namespace SKELETON { struct SKELETON; struct SKELETON_JOINT; }
namespace mowa { namespace sgui { class LabelControl; class ButtonControl; class BoolVarControl; class IntVarControl; class SimpleGUI; }}
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
		bool fileDrop( ci::app::FileDropEvent event );

		SKELETON::SKELETON getSkeleton();

		void play();
		void pause();
		void restart();

		///// CALLBACKS
		bool onToggleRecordingClicked( ci::app::MouseEvent event );
		int filedropCallbackId;

		///// ACCESSORS
		void setJson( const std::string &aPath );
		void setJson( Json::Value *aJsonValue );

		int getCurrentFrame() { return _currentFrame; }
		int getTotalFrames() { return _totalframes; }

	private:
		PlayerState _state;
		bool _shouldLoop;
		int _currentFrame;
		int _totalframes;
		std::vector< UserStreamFrame_ptr > _recording;

		// GUI
		mowa::sgui::LabelControl* _label;	// Label that displays crrent state
		mowa::sgui::ButtonControl* _toggle;	// Toggle button
		mowa::sgui::BoolVarControl* _loopButton;	// Toggle button
		mowa::sgui::IntVarControl* _frameSlider;
		mowa::sgui::SimpleGUI* _gui;		// SimpleGUI instance

		Json::Value _json;
		std::string _jsonString;
		void parseJson( Json::Value* aJsonValue );
	};
}

#endif /* USERSTREAMPLAYER_H_ */
