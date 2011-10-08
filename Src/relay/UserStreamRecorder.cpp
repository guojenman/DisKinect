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
#include <boost/date_time/posix_time/posix_time.hpp>

#include "cinder/app/App.h"
#include "cinder/Utilities.h"
#include "cinder/Color.h"

#include "json/writer.h"

#include "WuCinderNITE.h"
#include "UserTracker.h"
#include "SkeletonStruct.h"

#include "simplegui/SimpleGUI.h"
#include "Constants.h"

namespace relay {

	UserStreamRecorder::UserStreamRecorder() {
		_livestream = NULL;
		_gui = NULL;
		_label = NULL;
	}
	UserStreamRecorder::~UserStreamRecorder() {
		delete _livestream; _livestream = NULL;
		delete _gui; _gui = NULL;
		delete _label; _label = NULL;
		std::cout << "UserStreamRecorder destructor!" << std::endl;
	}

	void UserStreamRecorder::enter() {
		ni = WuCinderNITE::getInstance();
		tracker = UserTracker::getInstance();

		_livestream = new UserStreamLive();
		_livestream->enter();

		if( Constants::Debug::USE_GUI ) {
			_gui = new mowa::sgui::SimpleGUI( ci::app::App::get() );
			_gui->textColor = ColorA(1,1,1,1);
			_gui->lightColor = ColorA(1, 0, 1, 1);
			_gui->darkColor = ColorA(0.05,0.05,0.05, 1);
			_gui->bgColor = ColorA(0.15, 0.15, 0.15, 1.0);
			_gui->addLabel("RECORDER");
			_gui->addSeparator();
			_label = _gui->addLabel("Idle");
			_gui->addSeparator();
			_toggle = _gui->addButton("START");
			_toggle->registerClick( this, &UserStreamRecorder::onToggleRecordingClicked );
		}

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

		if( _gui )
			_gui->draw();
	}

	SKELETON::SKELETON UserStreamRecorder::getSkeleton() {
		return _livestream->getSkeleton();
	}

	void UserStreamRecorder::exit() {
		ni = NULL;
		tracker = NULL;

		delete _livestream; _livestream = NULL;
		delete _gui; _gui = NULL;
		delete _label; _label = NULL;
	}



	void UserStreamRecorder::startRecording() {
		if( _state == RECORDING ) return;
		setState( RECORDING );
		_recording.clear();
	}

	void UserStreamRecorder::stopRecording() {
		if( _state == NOT_RECORDERING ) return;
		setState( NOT_RECORDERING );
	}

	void UserStreamRecorder::recordState() {
		SKELETON::SKELETON aSkeleton = _livestream->getSkeleton();
		UserStreamFrame* aFrame = new UserStreamFrame( _framenumber, aSkeleton );
		_recording.push_back( aFrame );
		_framenumber++;

		std::cout << "Recording frame:" << _framenumber << std::endl;
	}

	/**
	 * Set the state
	 */
	void UserStreamRecorder::setState( RecorderState aState ) {
		_state = aState;

		if( _gui ) {
			_label->setText( _state == NOT_RECORDERING ? "Idle" : "Recording!" );
			_toggle->name = _state == NOT_RECORDERING ? "START" : "STOP";
		}
	}

	// SimpleGUI Callbacks
	bool UserStreamRecorder::onToggleRecordingClicked( ci::app::MouseEvent event ) {
		if( _state == NOT_RECORDERING ) {
			startRecording();
		} else {
			stopRecording();
			ci::app::MouseEvent fakeEvent;
			onSaveClicked( fakeEvent );
		}

		return true;
	}

	Json::Value UserStreamRecorder::getRecordAsJSONValue() {
		stopRecording();

		Json::Value json;
		Json::Value root;
		for (std::vector<UserStreamFrame*>::iterator i = _recording.begin(); i != _recording.end(); ++i) {
			UserStreamFrame* aFrame = *i;
			root.append( aFrame->toJSON() );
		}

		json["root"] = root;
		return json;
	}

	bool UserStreamRecorder::onSaveClicked( ci::app::MouseEvent event ) {
		Json::Value json = getRecordAsJSONValue();

		// Write to console for debug
//		std::cout << json.toStyledString() << std::endl;

		// Save nice version to disk
		saveToDisk( json.toStyledString() );
		return true;
	}

	void UserStreamRecorder::saveToDisk( std::string jsonString ) {
		// Create a timestamp
		using namespace boost::posix_time;
	    using namespace boost::gregorian;

		// Use boost to figure out the current time on local machine
		ptime now = second_clock::local_time();

		std::stringstream ss;
		ss << now;
		//ss << now.date().month() <<  now.date().day() << "_" << ci::app::App::get()->getElapsedFrames();
		std::string timeStamp = ss.str();

		// Write the file to the documents directory
		std::string fileName = ci::getDocumentsDirectory() + "/Diskinect/Recording_" + timeStamp;
		ci::OStreamFileRef oStream = ci::writeFileStream( fileName + ".json", true );
		oStream->writeData( jsonString.data(), jsonString.length() );
	}
}
