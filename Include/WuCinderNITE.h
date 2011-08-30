/*
 * WuCinderNITE.h
 *
 *  Created on: Jul 23, 2011
 *      Author: guojianwu
 */
#ifndef WUCINDERNITE_H_
#define WUCINDERNITE_H_

#pragma once

#include "cinder/Cinder.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>

#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnCodecIDs.h>

using namespace std;

#define MAX_DEPTH 10000
#define CHECK_RC(status, what, isFatal) \
if (status != XN_STATUS_OK) \
{ \
	ci::app::console() << "failed:" << what << ":" << xnGetStatusString(status) << std::endl; \
	if(isFatal) { exit(-1); } \
}

class WuCinderNITE {
public:
	typedef boost::signals2::signal<void (XnUserID)> WuCinderNITESingalUser;
	typedef boost::signals2::signal<void ()> WuCinderNITESingalUpdate;

	static WuCinderNITE* getInstance();
	static ci::Vec3f XnVector3DToVec3f(XnVector3D &pos) {
		return ci::Vec3f(pos.X, pos.Y, pos.Z);
	};

	virtual ~WuCinderNITE();

	void setup(string xmlpath, XnMapOutputMode mapMode, bool useDepthMap = true, bool useColorImage = true);
	void setup(string onipath);
	void startUpdating();
	void stopUpdating();
	void shutdown();

	ci::Surface8u getDepthSurface();
	ci::Surface8u getImageSurface();
	XnMapOutputMode getMapMode();

	void renderDepthMap(ci::Area area);
	void renderSkeleton();
	void renderLimb(XnUserID player, XnSkeletonJoint eJoint1, XnSkeletonJoint eJoint2, float confidence = 0.75f);
	void renderColor(ci::Area area);
	void debugNodeTypes();

	WuCinderNITESingalUser		signalNewUser;
	WuCinderNITESingalUser		signalLostUser;
	WuCinderNITESingalUpdate	signalUpdate;

	bool				useSingleCalibrationMode;
	unsigned short		maxDepth;
	XnMapOutputMode		mMapMode;
	xn::Context			mContext;
	xn::DepthGenerator	mDepthGen;
	xn::UserGenerator	mUserGen;
	xn::ImageGenerator	mImageGen;
	xn::SceneAnalyzer	mSceneAnalyzer;
	xn::SceneMetaData	mSceneMeta;
	xn::DepthMetaData	mDepthMeta;
	xn::ImageMetaData	mImageMeta;
	XnPlane3D			mFloor;
	XnUserID			mUsers[15];
	XnUInt16			mNumUsers;


protected:
	WuCinderNITE();

	void update();
	void updateDepthSurface();
	void updateImageSurface();
	void registerCallbacks();
	void unregisterCallbacks();
	void findUsers();

	static WuCinderNITE* mInstance;

	volatile bool		mRunUpdates; // exits update thread if false
	boost::shared_ptr<boost::thread>	mThread;

	bool				mNeedPoseForCalibration;
	bool				mIsCalibrated;
	bool				mUseColorImage;
	bool				mUseDepthMap;
	ci::Surface8u		mImageSurface;
	ci::Area			mDrawArea;
	ci::Surface8u		mDepthSurface;
	float 				mDepthHistogram[MAX_DEPTH];

	XnChar				mCalibrationPose[20];
	XnCallbackHandle	hUserCBs, hCalibrationPhasesCBs, hCalibrationCompleteCBs, hPoseCBs;

	void static XN_CALLBACK_TYPE CB_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
	void static XN_CALLBACK_TYPE CB_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
	void static XN_CALLBACK_TYPE CB_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie);
	void static XN_CALLBACK_TYPE CB_CalibrationEnd(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie);
	void static XN_CALLBACK_TYPE CB_CalibrationComplete(xn::SkeletonCapability& skeleton, XnUserID nId, XnCalibrationStatus eStatus, void* cxt);
	void static XN_CALLBACK_TYPE CB_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie);

};

#endif /* WUCINDERNITE_H_ */
