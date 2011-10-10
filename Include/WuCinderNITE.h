/*
 * WuCinderNITE.h
 *
 *  Created on: Jul 23, 2011
 *      Author: guojianwu
 */

#pragma once
#ifndef WUCINDERNITE_H_
#define WUCINDERNITE_H_

#include "cinder/Cinder.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#include "string.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/signals.hpp>

#include <XnCppWrapper.h>
#include <XnCodecIDs.h>
#include <XnStatusCodes.h>
#include <XnTypes.h>


// Forward decleration
#include "SkeletonStruct.h"

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
	static const int	MAX_JOINTS = 25;
	static const int	MAX_USERS = 11;

	static WuCinderNITE* getInstance();
	static ci::Vec3f XnVector3DToVec3f(XnVector3D &pos) {
		return ci::Vec3f(pos.X, pos.Y, pos.Z);
	};

	virtual ~WuCinderNITE();

	void setup(std::string xmlpath, XnMapOutputMode mapMode, bool useDepthMap = true, bool useColorImage = true);
	void setup(std::string onipath);
	void useCalibrationFile(std::string filepath);
	void update();
	void startUpdating();
	void stopUpdating();
	void shutdown();

	// for use in single threaded applications
	void startGenerating();
	void stopGenerating();

	ci::Surface8u getDepthSurface();
	ci::Surface8u getImageSurface();
	XnMapOutputMode getMapMode();

	void renderDepthMap(ci::Area area);
	void renderSkeleton(SKELETON::SKELETON &skeleton, XnUserID nId = 0);
	void renderSkeleton(XnUserID nId = 0);
	void renderLimb(SKELETON::SKELETON &skeleton, XnSkeletonJoint eJoint1, XnSkeletonJoint eJoint2, float confidence = 0.75f);
	void renderColor(ci::Area area);
	void debugNodeTypes();
	void startTracking(XnUserID nId);

	WuCinderNITESingalUser		signalNewUser;
	WuCinderNITESingalUser		signalLostUser;

	/**
	 * Options
	 */
	bool				useSingleCalibrationMode;	// default true
	bool				waitForTrackingToSingalNewUser;	// default true

	SKELETON::SKELETON	skeletons[MAX_USERS];

	unsigned short		maxDepth;
	XnMapOutputMode		mMapMode;
	XnPlane3D			mFloor;


	boost::mutex mMutex;
	boost::mutex		mMutexImageSurface;
	ci::Surface8u		mImageSurface;

protected:
	WuCinderNITE();

	void updateLoop();
	void updateDepthSurface();
	void updateImageSurface();
	void registerCallbacks();
	void unregisterCallbacks();

	static WuCinderNITE* mInstance;

	volatile bool		mRunUpdates; // exits update thread if false
	boost::shared_ptr<boost::thread>	mThread;


	bool				mNeedPoseForCalibration;
	bool				mIsCalibrated;
	bool				mUseColorImage;
	bool				mUseDepthMap;
	std::string			mCalibrationFile;

	xn::Context			mContext;
	xn::DepthGenerator	mDepthGen;
	xn::UserGenerator	mUserGen;
	xn::ImageGenerator	mImageGen;
	xn::SceneAnalyzer	mSceneAnalyzer;
	xn::SceneMetaData	mSceneMeta;
	xn::DepthMetaData	mDepthMeta;
	xn::ImageMetaData	mImageMeta;


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
