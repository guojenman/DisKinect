/*
 * UserStreamRecorder.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
 *      Abstract:
 *      	 This represents one of the states in the UserStreamStateManager
 *      	 It is able to record a user stream from the Kinect and outputing it as a JSON string
 *      	 It relies on a UserStreamLive instance for kinect data
 */

#include "UserStreamRecorder.h"
#include "cinder/app/App.h"
namespace relay {

	UserStreamRecorder::UserStreamRecorder() {
		_livestream = NULL;
		_gui = NULL;
		_label = NULL;
	}
	UserStreamRecorder::~UserStreamRecorder() {
		delete _livestream; _livestream = NULL;
		delete _gui; _gui = NULL;
		_label = NULL;
		std::cout << "UserStreamRecorder destructor!" << std::endl;
	}

	void UserStreamRecorder::enter() {
		ni = WuCinderNITE::getInstance();
		tracker = UserTracker::getInstance();

		_livestream = new UserStreamLive();
		_livestream->enter();

		_gui = new mowa::sgui::SimpleGUI( ci::app::App::get() );
		_gui->addLabel("RECORDER");
		_gui->addSeparator();
		_label = _gui->addLabel("Idle");
		_gui->addSeparator();
		_toggle = _gui->addButton("START");
		_toggle->registerClick( this, &UserStreamRecorder::onToggleRecordingClicked );

		_framenumber = 0;
		_state = NOT_RECORDERING;
		_recording.clear();
	}

	void UserStreamRecorder::update() {
		_livestream->update();

		if( _state == RECORDING ) {
			recordState();
		}
	}

	void UserStreamRecorder::draw() {
		_livestream->draw();
		_gui->draw();
	}

	WuCinderNITE::SKELETON UserStreamRecorder::getSkeleton() {
		return _livestream->getSkeleton();
	}

	void UserStreamRecorder::exit() {
		ni = NULL;
		tracker = NULL;

		delete _livestream; _livestream = NULL;
		delete _gui; _gui = NULL;
		_label = NULL;
	}



	void UserStreamRecorder::startRecording() {
		if( _state == RECORDING ) return;
		setState( RECORDING );
		_recording.clear();
	}

	void UserStreamRecorder::stopRecording() {
		if( _state == NOT_RECORDERING ) return;
		setState( NOT_RECORDERING );

		ci::app::MouseEvent fakeEvent;
		onSaveClicked( fakeEvent );
	}

	void UserStreamRecorder::recordState() {

		WuCinderNITE::SKELETON aSkeleton = _livestream->getSkeleton();
		UserStreamFrame* aFrame = new UserStreamFrame( _framenumber, aSkeleton );
		_recording.push_back( aFrame );
		_framenumber++;
	}

	/**
	 * Set the state
	 */
	void UserStreamRecorder::setState( RecorderState aState ) {
		_state = aState;
		_label->setText( _state == NOT_RECORDERING ? "Idle" : "Recording!" );
		_toggle->name = _state == NOT_RECORDERING ? "START" : "STOP";
	}
	// SimpleGUI Callbacks
	bool UserStreamRecorder::onToggleRecordingClicked( ci::app::MouseEvent event ) {
		if( _state == NOT_RECORDERING ) startRecording();
		else stopRecording();

		return true;
	}
	bool UserStreamRecorder::onSaveClicked( ci::app::MouseEvent event ) {
		stopRecording();

		Json::Value json;
		for( std::vector<UserStreamFrame*>::iterator i = _recording.begin();
				i != _recording.end();
				++i) {
			UserStreamFrame* aFrame = *i;
			json.append( aFrame->toJSON() );
		}


		std::cout << json.toStyledString() << std::endl;
	}
}
