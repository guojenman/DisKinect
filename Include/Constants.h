/*
 * Constants.h
 *
 *  Created on: Sep 25, 2011
 *      Author: onedayitwillmake
 */
#ifndef CONSTANTS_H_
#define CONSTANTS_H_
namespace cinder { class CameraPersp; class MayaCamUI; };
namespace Constants {
	extern cinder::CameraPersp* camera();
	extern cinder::MayaCamUI* mayaCam();

	namespace Debug {
		static bool DRAW_PUPPETEER_BOUNDS = true;
		static bool DRAW_SKELETON = true;
		static bool DRAW_DEPTHMAP = true;
		static bool USE_GUI = true;
	};
}

#endif /* CONSTANTS_H_ */
