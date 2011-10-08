/*
 * Constants.cpp
 *
 *  Created on: Oct 1, 2011
 *      Author: mariogonzalez
 */
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"
#include "Constants.h"
namespace Constants {
	cinder::MayaCamUI* mayaCam() {
		static cinder::MayaCamUI* mayaCam = new cinder::MayaCamUI();
		return mayaCam;
	}
}

