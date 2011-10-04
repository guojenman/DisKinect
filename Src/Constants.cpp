/*
 * Constants.cpp
 *
 *  Created on: Oct 1, 2011
 *      Author: mariogonzalez
 */
#include "cinder/Camera.h"
#include "Constants.h"
namespace Constants {
	cinder::CameraPersp* camera() {
		static cinder::CameraPersp* camera = new cinder::CameraPersp();
		return camera;
	}
}

