/*
 * UserStreamFrame.cpp
 *
 *  Created on: Sep 24, 2011
 *      Author: onedayitwillmake
 */

#include "UserStreamFrame.h"

namespace relay
{

	UserStreamFrame::UserStreamFrame( uint32_t aFramenumber, WuCinderNITE::SKELETON &aSkeleton ) {
		framenumber = aFramenumber;
		skeleton = aSkeleton;
	}
	UserStreamFrame::~UserStreamFrame() {}

	UserStreamFrame* UserStreamFrame::fromJSON( Json::Value json  ) {
		WuCinderNITE::SKELETON aSkeleton;
		return new UserStreamFrame(1, aSkeleton);;
	}

	Json::Value UserStreamFrame::toJSON() {
		Json::Value value;
		value["framenumber"] = 4;
		return value;
	}
}
