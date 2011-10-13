/*
 * iUserStream.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: mariogonzalez
 */

#ifndef IUSERSTREAM_H_
#define IUSERSTREAM_H_

// Forward declerations
namespace SKELETON {
	struct SKELETON;
	struct SKELETON_JOINT;
}
class WuCinderNITE;
class UserTracker;

namespace relay {
	class IUserStream {
	public:
		virtual ~IUserStream(){};

		virtual void enter() = 0;
		virtual void update() = 0;
		virtual void exit() = 0;
		virtual void draw() = 0;
		virtual bool wantsToExit() = 0;
		virtual SKELETON::SKELETON getSkeleton() = 0;

	protected:
		WuCinderNITE* ni;
		UserTracker* tracker;
	};
}

#endif /* IUSERSTREAM_H_ */
