/*
 * iUserStream.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
 */

#ifndef IUSERSTREAM_H_
#define IUSERSTREAM_H_

#include "WuCinderNITE.h"
#include "UserTracker.h"

namespace relay {
	class IUserStream {
	public:
		virtual ~IUserStream(){};
		virtual void release() = 0;

		virtual void enter() = 0;
		virtual void update() = 0;
		virtual void exit() = 0;
		virtual WuCinderNITE::SKELETON getSkeleton() = 0;

	protected:
		WuCinderNITE* ni;
		UserTracker* tracker;
	};
}

#endif /* IUSERSTREAM_H_ */
