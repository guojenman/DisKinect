/*
 * UserStreamFrame.h
 *
 *  Created on: Sep 24, 2011
 *      Author: onedayitwillmake
 *      Abstract: Represents a single frame of a recording
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
			Json::Value toJSON();

			uint32_t framenumber;
			WuCinderNITE::SKELETON skeleton;
	};

}

#endif /* USERSTREAMFRAME_H_ */
