/*
 * UserTracker.cpp
 *
 *  Created on: Aug 28, 2011
 *      Author: guojianwu
 */

#include "UserTracker.h"

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
	activeUserId = 0;
	activeMotionTolerance = 2.0f;
	activeTickTotlerance = 5;
	ni = WuCinderNITE::getInstance();
	mSignalConnectionNewUser = ni->signalNewUser.connect(boost::bind(&UserTracker::onNewUser, this, boost::lambda::_1));
	mSignalConnectionLostUser = ni->signalLostUser.connect(boost::bind(&UserTracker::onLostUser, this, boost::lambda::_1));
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
	for(list<UserInfo>::iterator it = mUsers.begin(); it != mUsers.end();) {
		if (it->id == nId) {
			mUsers.erase(it);
			break;
		}
		it++;
	}
}

void UserTracker::update()
{
	XnSkeletonJointPosition	jointShoulderL, jointHandL, jointKneeL;
	XnSkeletonJointPosition	jointShoulderR, jointHandR, jointKneeR;
	ci::Vec3f	shoulderL, handL, kneeL;
	ci::Vec3f	shoulderR, handR, kneeR;
	float totalDist;
	float confidence = 0.5f;
	// measure distance of important joints have moved from the last position
	// and decide if the user is active or not - used for sorting, and gives us
	// the next active user, if user A stays still for too long (possible lost of user)
	for(list<UserInfo>::iterator it = mUsers.begin(); it != mUsers.end();) {
		if (ni->mUserGen.GetSkeletonCap().IsTracking(it->id)) {
			ni->mUserGen.GetSkeletonCap().GetSkeletonJointPosition(it->id, XN_SKEL_LEFT_SHOULDER, jointShoulderL);
			ni->mUserGen.GetSkeletonCap().GetSkeletonJointPosition(it->id, XN_SKEL_LEFT_HAND, jointHandL);
			ni->mUserGen.GetSkeletonCap().GetSkeletonJointPosition(it->id, XN_SKEL_LEFT_KNEE, jointKneeL);

			ni->mUserGen.GetSkeletonCap().GetSkeletonJointPosition(it->id, XN_SKEL_RIGHT_SHOULDER, jointShoulderR);
			ni->mUserGen.GetSkeletonCap().GetSkeletonJointPosition(it->id, XN_SKEL_RIGHT_HAND, jointHandR);
			ni->mUserGen.GetSkeletonCap().GetSkeletonJointPosition(it->id, XN_SKEL_RIGHT_KNEE, jointKneeR);

			if (jointHandL.fConfidence > confidence) {
				shoulderL = WuCinderNITE::XnVector3DToVec3f(jointHandL.position);
			} else {
				shoulderL = it->shoulderL;
			}
			if (jointHandR.fConfidence > confidence) {
				shoulderR = WuCinderNITE::XnVector3DToVec3f(jointHandR.position);
			} else {
				shoulderR = it->shoulderR;
			}
			if (jointHandL.fConfidence > confidence) {
				handL = WuCinderNITE::XnVector3DToVec3f(jointHandL.position);
			} else {
				handL = it->handL;
			}
			if (jointHandR.fConfidence > confidence) {
				handR = WuCinderNITE::XnVector3DToVec3f(jointHandR.position);
			} else {
				handR = it->handR;
			}
			if (jointKneeL.fConfidence > confidence) {
				kneeL = WuCinderNITE::XnVector3DToVec3f(jointKneeL.position);
			} else {
				kneeL = it->kneeL;
			}
			if (jointKneeR.fConfidence > confidence) {
				kneeR = WuCinderNITE::XnVector3DToVec3f(jointKneeR.position);
			} else {
				kneeR = it->kneeR;
			}

			totalDist = 0;
			totalDist += shoulderL.distanceSquared(it->shoulderL);
			totalDist += shoulderR.distanceSquared(it->shoulderR);
			totalDist += handL.distanceSquared(it->handL);
			totalDist += handR.distanceSquared(it->handR);
			totalDist += kneeL.distanceSquared(it->kneeL);
			totalDist += kneeR.distanceSquared(it->kneeR);

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
			ci::app::console() << mUsers.begin()->id << endl;
		}
	} else if (activeUserId != 0) {
		ci::app::console() << "no active user" << endl;
		activeUserId = 0;
	}
}

