/*
 * UserStreamFrame.cpp
 *
 *  Created on: Sep 24, 2011
 *      Author: onedayitwillmake
 *      Abstract: This class represents a single frame of UserStream data.
 *      It contains a framenumber, and a WuCinderNITE::SKELETON struct.
 *      It is able to output itself as a jsonstring, or create itself from a json string
 */

#ifndef USERSTREAMFRAME_H_
#define USERSTREAMFRAME_H_

#include "json/value.h"
#include "SkeletonStruct.h"
#include <boost/shared_ptr.hpp>

// Forward declerations
namespace SKELETON {
	struct SKELETON;
	struct SKELETON_JOINT;
}
class WuCinderNITE;
class UserTracker;

namespace relay
{
	class UserStreamFrame
	{
		public:
			UserStreamFrame( uint32_t aFramenumber, SKELETON::SKELETON &aSkeleton );
			virtual ~UserStreamFrame();

			// Static creation of an instance
			static boost::shared_ptr<UserStreamFrame> fromJSON( Json::Value json  );
			// Outputs to JSON string
			Json::Value toJSON();

			// Sequenced framenumber of this recording
			uint32_t framenumber;

			// Skeleton data
			SKELETON::SKELETON skeleton;
	};
}

typedef boost::shared_ptr<relay::UserStreamFrame> UserStreamFrame_ptr;

#endif /* USERSTREAMFRAME_H_ */
