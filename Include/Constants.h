/*
 * Constants.h
 *
 *  Created on: Sep 25, 2011
 *      Author: onedayitwillmake
 */
#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include <iostream>

namespace cinder { class MayaCamUI; };
namespace Constants {
	extern cinder::MayaCamUI* mayaCam();

	namespace Debug {
		static bool DRAW_PUPPETEER_BOUNDS = true;
		static bool DRAW_SKELETON = true;
		static bool DRAW_DEPTHMAP = true;
		static bool USE_GUI = true;
		static bool CREATE_TIMELAPSE = true;
	};

	namespace relay {
		namespace repeater {
			static float MIN_CUMALTIVE_DELTA_BEFORE_RECORDING = 1e05;	// At least this much movement has to accumulate while a user is active before we start recording

			// Record somewhere between this many frames - arbitary numbers
			// TODO: instead, stop recording after if delta decreases?
			static float MIN_FRAMES_TO_RECORD = 50;
			static float MAX_FRAMES_TO_RECORD = 200;
		}
	}

	namespace TimeLapse {
		static std::string DIRECTORY_NAME = "Diskinect";
		static int SECONDS_BETWEEN_SNAPSHOT = 5;
	}
}

#endif /* CONSTANTS_H_ */
