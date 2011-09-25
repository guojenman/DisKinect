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

#include <vector>
#include "IUserStream.h"
#include "UserStreamLive.h"
#include "UserStreamFrame.h"
#include "simplegui/SimpleGUI.h"
#include "cinder/app/Event.h"

namespace relay {
class UserStreamRecorder : public IUserStream  {

	enum RecorderState { NOT_RECORDERING, RECORDING };

	public:
		UserStreamRecorder();
		virtual ~UserStreamRecorder();

		void enter();
		void update();
		void draw();
		void exit();
		WuCinderNITE::SKELETON getSkeleton();

		// Callbacks
		bool onToggleRecordingClicked( ci::app::MouseEvent event );
		bool onSaveClicked( ci::app::MouseEvent event );

	private:
		void setState( RecorderState aState );

		UserStreamLive *_livestream;	// When queried for skeleton data we just send whatever our livestream has

		// Gui
		mowa::sgui::LabelControl* _label;
		mowa::sgui::ButtonControl* _toggle;
		mowa::sgui::SimpleGUI* _gui;

		RecorderState _state;
		uint32_t _framenumber; // Current frame number of recording, set to zero on start

		std::vector<UserStreamFrame*> _recording;


		// recording
		void startRecording();
		void recordState();
		void stopRecording();
	};
}

#endif /* UserStreamRecorder_H_ */
