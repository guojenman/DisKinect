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

#include "WuCinderNITE.h"
#include "json/value.h"

namespace relay
{

	class UserStreamFrame
	{
		public:
			UserStreamFrame( uint32_t aFramenumber, WuCinderNITE::SKELETON &aSkeleton );
			virtual ~UserStreamFrame();

			// Static creation of an instance
			static UserStreamFrame* fromJSON( Json::Value json  );
			// Outputs to JSON string
			Json::Value toJSON();

			// Sequenced framenumber of this recording
			uint32_t framenumber;

			// Skeleton data
			WuCinderNITE::SKELETON skeleton;
	};

}

#endif /* USERSTREAMFRAME_H_ */
