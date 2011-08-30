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
#include <stddef.h>
#include <XnTypes.h>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <list>
#include "XnCppWrapper.h"
#include "WuCinderNITE.h"

struct UserInfo {
	UserInfo(XnUserID nId):id(nId),isActive(false),motionAtZeroDuration(0){};
	XnUserID	id;
	bool		isActive;

	unsigned int	motionAtZeroDuration; // ticks that the user has not moved
	ci::Vec3f	shoulderL, handL, kneeL;
	ci::Vec3f	shoulderR, handR, kneeR;

	bool operator<(UserInfo &other) {
		return isActive ? isActive : other.isActive;
	};
};

class UserTracker {
public:
	static UserTracker* getInstance();
	virtual ~UserTracker();

	void release();
private:
	UserTracker();

	static UserTracker* mInstance;

	void onNewUser(XnUserID nId);
	void onLostUser(XnUserID nId);
	void onUpdate();

	WuCinderNITE*		ni;
	list<UserInfo>		mUsers;
	unsigned char		mUsersTracking;
	boost::signals2::connection	mSignalConnectionNewUser;
	boost::signals2::connection	mSignalConnectionLostUser;
	boost::signals2::connection mSignalConnectionUpdate;
};

#endif /* USERTRACKER_H_ */
