/*
 * UserStreamPlayer.h
 *
 *  Created on: Sep 26, 2011
 *      Author: mariogonzalez
 *      Abstract:
 *      	 This represents one of the states in the UserStreamStateManager
 *      	 It is responsible for playing a json skeleton recording
 */

#include "UserStreamPlayer.h"
#include <iostream>
#include <fstream>

#include "UserStreamLive.h"
#include "UserStreamRecorder.h"
#include "UserStreamPlayer.h"


#include "WuCinderNITE.h"
#include "UserTracker.h"
#include "SkeletonStruct.h"

#include "json/reader.h"

namespace relay {
	UserStreamPlayer::UserStreamPlayer() {
	}

	UserStreamPlayer::~UserStreamPlayer() {
		std::cout << "UserStreamPlayer destructor!" << std::endl;
	}

	void UserStreamPlayer::enter() {
		if( _recording.size() == 0 ) {
			std::cout << "UserStreamPlayer Error: You must call 'setJsonFromString', or 'setJsonFromValue' before this state is entered" << std::endl;
			return;
		}

		ni = WuCinderNITE::getInstance();
		tracker = UserTracker::getInstance();

		_currentFrame = 0;
		_totalframes = 100;
	}

	void UserStreamPlayer::update() {
		_currentFrame++;
	}

	void UserStreamPlayer::draw() {

	}

	void UserStreamPlayer::exit() {

	}

	void UserStreamPlayer::play() {
		_state = PLAYING;
	}

	void UserStreamPlayer::pause() {
		_state = PAUSED;
	}
	void UserStreamPlayer::restart() {
		_currentFrame = 0;
		_state = PLAYING;
	}

	SKELETON::SKELETON UserStreamPlayer::getSkeleton() {
		uint frameToPlay = (uint)ci::math<float>::min( _currentFrame, _totalframes-1 );

		// To avoid throwing an exception - just return an empty skeleton struct if the current frame is greater than our size
		// This can happen do to a race condition if update is called just before the file is loaded
		if( frameToPlay < _recording.size() ) {
			return _recording.at( frameToPlay )->skeleton;
		} else {
			SKELETON::SKELETON skeleton;
			return skeleton;
		}

	}


	// SAVE LOAD JSON
	void UserStreamPlayer::setJson( const std::string &aPath ) {
		Json::Value *aJsonValue = new Json::Value();
		Json::Reader reader;

		// Read the filestream
		ifstream filestream;

		// load from resources directory so we can modify it while we work
		filestream.open( ci::app::App::get()->getResourcePath(aPath).c_str(), ifstream::in);

		// Parse the json file or retrieve errors
		bool parsingSuccessful = reader.parse( filestream, *aJsonValue );
		if ( !parsingSuccessful ) {
			// report to the user the failure and their locations in the document.
			std::cout  << "Failed to parse configuration\n" << reader.getFormatedErrorMessages() << std::endl;
			return;
		}

		parseJson ( aJsonValue );
	}

	void UserStreamPlayer::setJson( Json::Value *aJsonValue ) {
		_json = aJsonValue;
		parseJson( aJsonValue );
	}

	void UserStreamPlayer::parseJson( Json::Value *aJsonValue ) {
		// Create a userframe for each json object
		Json::Value root = (*aJsonValue)["root"];

		UserStreamFrame *userFrame;
        for( Json::ValueIterator itr = root.begin() ; itr != root.end() ; itr++ ) {
        	userFrame = UserStreamFrame::fromJSON( (*itr) );
			_totalframes = userFrame->framenumber + 1;
			_recording.push_back( userFrame );
        }

        std::cout << "Playback stream created with '" << userFrame->framenumber << "' frames" << std::endl;
	}
}
