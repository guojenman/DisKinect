/*
 * Constants.h
 *
 *  Created on: Sep 25, 2011
 *      Author: onedayitwillmake
 */
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "cinder/Vector.h"
#include <iosfwd>
#include <map>

namespace cinder { class MayaCamUI; };
namespace Constants {
	extern cinder::MayaCamUI* mayaCam();

	namespace Debug {
		static bool DRAW_PUPPETEER_BOUNDS = true;
		static bool DRAW_SKELETON = true;
		static bool DRAW_DEPTHMAP = true;
		static bool USE_GUI = true;
		static bool CREATE_TIMELAPSE = true;
		static bool USE_ARDUINO = false;
		static bool USE_RECORDED_ONI = false;
	};

	namespace relay {
		namespace repeater {
			static float MIN_CUMALTIVE_DELTA_BEFORE_RECORDING = 25.0f;	// At least this much movement has to accumulate while a user is active before we start recording

			// Record somewhere between this many frames - arbitary numbers
			// TODO: instead, stop recording after if delta decreases?
			static float MIN_FRAMES_TO_RECORD = 50;
			static float MAX_FRAMES_TO_RECORD = 200;
		}

		namespace player {
			extern std::map<std::string, int>* weightedGestures();
		}
	}

	namespace TimeLapse {
		static std::string DIRECTORY_NAME = "Diskinect";
		static unsigned int MINIMUM_GIGABYTES_BEFORE_SAVE = 2;
		static int SECONDS_BETWEEN_SNAPSHOT = 5;
	}

	namespace UserTracker {
		static ci::Vec3f ACTIVATION_ZONE = ci::Vec3f(0.0f, 0.0f, 2.5f);
	}

	namespace Puppeteer {
		static std::string USB_COM = "tty.usbmodemfd131";
	}
}

#endif /* CONSTANTS_H_ */
