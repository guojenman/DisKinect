/*
 * UserTracker.h
 *
 *  Created on: Aug 28, 2011
 *      Author: guojianwu
 */

#ifndef USERTRACKER_H_
#define USERTRACKER_H_

#pragma once

#include "cinder/Vector.h"
#include "cinder/Text.h"
#include <boost/signals2.hpp>
#include <list>
#include <stddef.h>
#include <XnTypes.h>

#include "WuCinderNITE.h"

class UserTracker {
public:
	static UserTracker* getInstance();
	virtual ~UserTracker();
	void update();
	void draw();
	void release();

	XnUserID	activeUserId;
	float		activeMotionTolerance;
	unsigned int	activeTickTotlerance;
	ci::Vec3f	activationZone;

	float totalDist;
	float getTotalDist() { return totalDist > 1.5e+05 ? 0 : totalDist; }; // Temp fix to prevent it from taking NaN values into account causing bad readouts

private:
	struct UserInfo {
		UserInfo(XnUserID nId):id(nId),isActive(false),motionAtZeroDuration(0){};
		XnUserID	id;
		bool		isActive;
		float		distanceFromActivationZone;

		unsigned int	motionAtZeroDuration; // ticks that the user has not moved
		ci::Vec3f	shoulderL, handL, kneeL;
		ci::Vec3f	shoulderR, handR, kneeR;
		ci::Vec3f	torso;

		bool operator<(const UserInfo& other) {
//			if (distanceFromActivationZone < 0.25f) {
//				return true;
//			}
//			return distanceFromActivationZone < other.distanceFromActivationZone ? true : isActive > other.isActive;

			return isActive > other.isActive;;
		}
	};

	UserTracker();
	static UserTracker* mInstance;

	void onNewUser(XnUserID nId);
	void onLostUser(XnUserID nId);

	ci::Font	mFont;

	WuCinderNITE*		ni;
	std::list<UserInfo>	mUsers;
	boost::signals2::connection	mSignalConnectionNewUser;
	boost::signals2::connection	mSignalConnectionLostUser;
	boost::mutex	mMutex;

};

#endif /* USERTRACKER_H_ */
