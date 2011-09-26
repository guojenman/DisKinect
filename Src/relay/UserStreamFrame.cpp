/*
 * UserStreamFrame.cpp
 *
 *  Created on: Sep 24, 2011
 *      Author: onedayitwillmake
 *      Abstract: This class represents a single frame of UserStream data.
 *      It contains a framenumber, and a WuCinderNITE::SKELETON struct.
 *      It is able to output itself as a jsonstring, or create itself from a json string
 */

#include "UserStreamFrame.h"
#include <iterator>


#define ZERO_IF_NAN(__X__) (__X__ != __X__) ? 0.0f : __X__
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
		value["framenumber"] = framenumber;

		Json::Value skeletonData;
		skeletonData["isTracking"] = skeleton.isTracking;

		// Store position data for each joint
		Json::Value joints;

		// iterate and get position/confidence data from each joint
		size_t length = sizeof(skeleton.joints) / sizeof(WuCinderNITE::SKELETON_JOINT);
		WuCinderNITE::SKELETON_JOINT *begin = &skeleton.joints[0];
		WuCinderNITE::SKELETON_JOINT *end = &skeleton.joints[length];
		for(WuCinderNITE::SKELETON_JOINT* i = begin; i != end; ++i){
			//std::cout << skeleton.joints[i].position << std::endl;
			WuCinderNITE::SKELETON_JOINT *singleJoint = &*i;

			Json::Value aJointJson;
			aJointJson["confidence"] = singleJoint->confidence;

			Json::Value positionJson;
			positionJson["x"] = ZERO_IF_NAN(singleJoint->position.x);
			positionJson["y"] = ZERO_IF_NAN(singleJoint->position.y);
			positionJson["z"] = ZERO_IF_NAN(singleJoint->position.z);
			aJointJson["position"] = positionJson;

			// Add to joints list
			joints.append( aJointJson );
		}

		skeletonData["joints"] = joints;
		value["skeletonData"] = skeletonData;
		return value;
	}
}
