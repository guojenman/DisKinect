/*
 * UserTracker.cpp
 *
 *  Created on: Aug 28, 2011
 *      Author: guojianwu
 */

#include "UserTracker.h"
#include "SkeletonStruct.h"
#include "Constants.h"
#include "cinder/MayaCamUI.h"
#include <boost/lambda/lambda.hpp>


UserTracker* UserTracker::mInstance = NULL;
UserTracker* UserTracker::getInstance()
{
	if (mInstance == NULL) {
		mInstance = new UserTracker();
	}
	return mInstance;
}

UserTracker::UserTracker()
{
	ni = WuCinderNITE::getInstance();

	activeUserId = 0;
	activeMotionTolerance = 0.002f;
	activeTickTotlerance = 0.005f;
	activationZone = ci::Vec3f(0, 0, 2.5f);
	totalDist = 0;

	mSignalConnectionNewUser = ni->signalNewUser.connect( boost::bind(&UserTracker::onNewUser, this, boost::lambda::_1) );
	mSignalConnectionLostUser = ni->signalLostUser.connect( boost::bind(&UserTracker::onLostUser, this, boost::lambda::_1) );
}

UserTracker::~UserTracker()
{
	mSignalConnectionNewUser.disconnect();
	mSignalConnectionLostUser.disconnect();
}

void UserTracker::release()
{
	if (mInstance != NULL) {
		delete mInstance;
		mInstance = NULL;
	}
}

void UserTracker::onNewUser(XnUserID nId)
{
	onLostUser(nId);
	mUsers.push_back(UserInfo(nId));
}

void UserTracker::onLostUser(XnUserID nId)
{
	for(std::list<UserInfo>::iterator it = mUsers.begin(); it != mUsers.end();) {
		if (it->id == nId) {
			mUsers.erase(it);
			break;
		}
		it++;
	}
}

void UserTracker::update()
{
	totalDist = 0;
	float confidence = 0.5f;
	// measure distance of important joints have moved from the last position
	// and decide if the user is active or not - used for sorting, and gives us
	// the next active user, if user A stays still for too long (possible lost of user)
	for(std::list<UserInfo>::iterator it = mUsers.begin(); it != mUsers.end();) {
		SKELETON::SKELETON &skeleton = ni->skeletons[it->id];
		if (skeleton.isTracking) {

			ci::Vec3f &shoulderL = skeleton.joints[XN_SKEL_LEFT_SHOULDER].confidence > confidence
					? skeleton.joints[XN_SKEL_LEFT_SHOULDER].position : it->shoulderL;

			ci::Vec3f &handL = skeleton.joints[XN_SKEL_RIGHT_SHOULDER].confidence > confidence
					? skeleton.joints[XN_SKEL_RIGHT_SHOULDER].position : it->shoulderR;

			ci::Vec3f &kneeL = skeleton.joints[XN_SKEL_LEFT_HAND].confidence > confidence
					? skeleton.joints[XN_SKEL_LEFT_HAND].position : it->handL;

			ci::Vec3f &shoulderR = skeleton.joints[XN_SKEL_RIGHT_HAND].confidence > confidence
					? skeleton.joints[XN_SKEL_RIGHT_HAND].position : it->handR;

			ci::Vec3f &handR = skeleton.joints[XN_SKEL_LEFT_KNEE].confidence > confidence
					? skeleton.joints[XN_SKEL_LEFT_KNEE].position : it->kneeL;

			ci::Vec3f &kneeR = skeleton.joints[XN_SKEL_RIGHT_KNEE].confidence > confidence
					? skeleton.joints[XN_SKEL_RIGHT_KNEE].position : it->kneeR;

			ci::Vec3f &torso = skeleton.joints[XN_SKEL_TORSO].confidence > confidence
					? skeleton.joints[XN_SKEL_TORSO].position : it->torso;

			it->distanceFromActivationZone = torso.distance(activationZone);

			totalDist = 0;
			float distance;
			int validJoints = 0;
			float maxDistance = 10.0f;

			distance = shoulderL.distanceSquared(it->shoulderL);
			if( distance < maxDistance ) { totalDist += distance; ++validJoints; };

			distance = shoulderR.distanceSquared(it->shoulderR);
			if( distance < maxDistance ) { totalDist += distance; ++validJoints; };

			distance = handL.distanceSquared(it->handL);
			if( distance < maxDistance ) { totalDist += distance; ++validJoints; };

			distance = handR.distanceSquared(it->handR);
			if( distance < maxDistance ) { totalDist += distance; ++validJoints; };

			distance = kneeL.distanceSquared(it->kneeL);
			if( distance < maxDistance ) { totalDist += distance; ++validJoints; };

			distance = kneeR.distanceSquared(it->kneeR);
			if( distance < maxDistance ) { totalDist += distance; ++validJoints; };

//			std::cout << "TotalDist: " << totalDist << " ValidJoints: " << validJoints << std::endl;

			if (totalDist <= activeMotionTolerance) {
				if (++it->motionAtZeroDuration == activeTickTotlerance) {
					it->isActive = false;
				}
			} else {
				it->motionAtZeroDuration = 0;
				if (!it->isActive) {
					it->isActive = true;
				}
			}



//			std::cout << "Total Delta:"<< totalDist << std::endl;
//			std::cout << "Total Delta:"<< totalDist << std::endl;

			it->shoulderL = shoulderL;
			it->shoulderR = shoulderR;
			it->handL = handL;
			it->handR = handR;
			it->kneeL = kneeL;
			it->kneeR = kneeR;
		} else {
			it->isActive = false;
		}
		it++;
	}
	mUsers.sort();

	if (!mUsers.empty() && mUsers.begin()->isActive) {
		if (mUsers.begin()->id != activeUserId) {
			activeUserId = mUsers.begin()->id;
			ci::app::console() << mUsers.begin()->id << std::endl;
		}
	} else if (activeUserId != 0) {
		ci::app::console() << "no active user" << std::endl;
		activeUserId = 0;
	}
}

void UserTracker::draw()
{
	ci::gl::enableAlphaBlending();
	ci::gl::pushMatrices();
	ci::gl::setMatrices(Constants::mayaCam()->getCamera());
		ci::gl::color(ci::ColorA(1, 1, 1, 0.2f));
		ci::gl::pushModelView();
		ci::gl::translate(ci::Vec3f(0, ni->mFloor.ptPoint.Y / 1000.0f, 0));
		ci::gl::drawBillboard(activationZone, ci::Vec2f(1.0f, 1.0f), 0, ci::Vec3f::xAxis(), ci::Vec3f::zAxis());
		ci::gl::popModelView();
	ci::gl::popMatrices();
	ci::gl::disableAlphaBlending();
}

