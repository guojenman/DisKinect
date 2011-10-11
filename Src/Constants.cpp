/*
 * Constants.cpp
 *
 *  Created on: Oct 1, 2011
 *      Author: mariogonzalez
 */
#include "Constants.h"
#include "cinder/Vector.h"
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Rand.h"
#include <iostream>

namespace Constants {
	cinder::MayaCamUI* mayaCam() {
		static cinder::MayaCamUI* mayaCam = new cinder::MayaCamUI();
		return mayaCam;
	}

	namespace relay {
		namespace player {
			std::map<std::string, int>* weightedGestures() {
				static bool wasSetup = false;
				static std::map<std::string, int> weights;

				if( !wasSetup ) {
					weights.insert( std::pair<std::string, int>("come here.json", 10) );
					weights.insert( std::pair<std::string, int>("attention.json", 20) );
					weights.insert( std::pair<std::string, int>("left arm up z.json", 10) );
					weights.insert( std::pair<std::string, int>("right arm up.json", 10) );
//					weights.insert( std::pair<std::string, int>("jsontest.json", 10) );
				}

				return &weights;
			}
		}
	}
}

