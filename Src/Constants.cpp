/*
 * Constants.cpp
 *
 *  Created on: Oct 1, 1011
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
					weights.insert( std::pair<std::string, int>("attention.json", 15) );
					weights.insert( std::pair<std::string, int>("both_arm_raise.json", 10) );
					weights.insert( std::pair<std::string, int>("left_arm_up_y.json", 10) );
					weights.insert( std::pair<std::string, int>("left_arm_up_z.json", 10) );
					weights.insert( std::pair<std::string, int>("right_arm_up_y.json", 10) );
					weights.insert( std::pair<std::string, int>("towards.json", 10) );
					weights.insert( std::pair<std::string, int>("towardsmore.json", 15) );
					weights.insert( std::pair<std::string, int>("wave.json", 25) );
				}

				return &weights;
			}
		}
	}
}

