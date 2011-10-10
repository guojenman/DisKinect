/*
 * WuCinderNITE.cpp
 *
 *  Created on: Jul 23, 2011
 *      Author: guojianwu
 */

#include "WuCinderNITE.h"
#include "SkeletonStruct.h"
#include <OpenGL.framework/Headers/gl.h>
using namespace std;

XnUInt32 mNITENumNITEUserColors = 10;
XnFloat mNITEUserColors[][3] = {
	{0, 0, 1},
	{1, 0.3,0.2},
	{0,1,0},
	{1,1,0},
	{1,0,0},
	{1,.5,0},
	{.5,1,0},
	{0,.5,1},
	{.5,0,1},
	{1,1,.5},
	{1,1,1}
};


WuCinderNITE* WuCinderNITE::mInstance = NULL;
WuCinderNITE* WuCinderNITE::getInstance()
{
	if (mInstance == NULL) {
		mInstance = new WuCinderNITE();
	}
	return mInstance;
}

WuCinderNITE::WuCinderNITE() {
	useSingleCalibrationMode = true;
	waitForTrackingToSingalNewUser = true;
	mNeedPoseForCalibration = false;
	mIsCalibrated = false;
	mRunUpdates = false;
}

WuCinderNITE::~WuCinderNITE() {
	unregisterCallbacks();
	if (mRunUpdates) {
		stopUpdating();
	}
	mContext.Shutdown();
	mContext.Release();
	mDepthGen.Release();
	mUserGen.Release();
	mImageGen.Release();
	mSceneAnalyzer.Release();
	signalNewUser.disconnect_all_slots();
	signalLostUser.disconnect_all_slots();
}

void WuCinderNITE::shutdown()
{
	if (mInstance != NULL) {
		delete mInstance;
		mInstance = NULL;
	}
}

void WuCinderNITE::setup(string onipath)
{
	mMapMode.nXRes = 0;
	XnStatus status = XN_STATUS_OK;
	xn::EnumerationErrors errors;
	status = mContext.Init();
	CHECK_RC(status, "Init", true);

	status = mContext.OpenFileRecording(onipath.c_str());
	CHECK_RC(status, "Recording", true);

	status = mContext.FindExistingNode(XN_NODE_TYPE_DEPTH, mDepthGen);
	if (status == XN_STATUS_OK) {
		mUseDepthMap = true;

		status = mDepthGen.GetMapOutputMode(mMapMode);
		CHECK_RC(status, "Retrieving XnMapOutputMode", true);

		mDepthSurface = ci::Surface8u(mMapMode.nXRes, mMapMode.nYRes, false);
		mDrawArea = ci::Area(0, 0, mMapMode.nXRes, mMapMode.nYRes);
		maxDepth = mDepthGen.GetDeviceMaxDepth() / 1000.0f;
	} else {
		mUseDepthMap = false;
		maxDepth = 0;
	}

	status = mContext.FindExistingNode(XN_NODE_TYPE_IMAGE, mImageGen);
	if (status != XN_STATUS_OK) {
		mUseColorImage = true;
		status = mImageGen.Create(mContext);
		CHECK_RC(status, "Image Gen", true);

		if (mMapMode.nXRes == 0) {
			status = mImageGen.GetMapOutputMode(mMapMode);
			CHECK_RC(status, "Retrieving XnMapOutputMode", true);
		}

		mImageSurface = ci::Surface8u(mMapMode.nXRes, mMapMode.nYRes, false);
	} else {
		mUseColorImage = false;
	}

	status = mContext.FindExistingNode(XN_NODE_TYPE_USER, mUserGen);
	if (status != XN_STATUS_OK) {
		status = mUserGen.Create(mContext);
		CHECK_RC(status, "User Gen", true);
	}
	mUserGen.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

	if (!mUserGen.IsCapabilitySupported((const char*)XN_CAPABILITY_SKELETON)) {
		ci::app::console() << "Supplied user generator doesn't support skeleton" << endl;
		exit(-1);
	}

	status = mContext.FindExistingNode(XN_NODE_TYPE_SCENE, mSceneAnalyzer);
	if (status != XN_STATUS_OK) {
		status = mSceneAnalyzer.Create(mContext);
		CHECK_RC(status, "Scene Analyzer", true);
	}

	registerCallbacks();
}
void WuCinderNITE::setup(string xmlpath, XnMapOutputMode mapMode, bool useDepthMap, bool useColorImage)
{
	mMapMode = mapMode;
	mUseDepthMap = useDepthMap;
	mUseColorImage = useColorImage;
	mDrawArea = ci::Area(0, 0, mMapMode.nXRes, mMapMode.nYRes);

	XnStatus status = XN_STATUS_OK;
	xn::EnumerationErrors errors;
	status = mContext.InitFromXmlFile(xmlpath.c_str(), &errors);
	CHECK_RC(status, "Init", true);


	if (mUseDepthMap) {
		status = mContext.FindExistingNode(XN_NODE_TYPE_DEPTH, mDepthGen);
		if (status != XN_STATUS_OK) {
			status = mDepthGen.Create(mContext);
			CHECK_RC(status, "Depth Creating", true);
		}
		status = mDepthGen.SetMapOutputMode(mMapMode);
		CHECK_RC(status, "Depth Settings", true);
		mDepthSurface = ci::Surface8u(mMapMode.nXRes, mMapMode.nYRes, false);
		maxDepth = mDepthGen.GetDeviceMaxDepth() / 1000.0f;
	} else {
		maxDepth = 0;
	}

	if (mUseColorImage) {
		status = mContext.FindExistingNode(XN_NODE_TYPE_IMAGE, mImageGen);
		if (status != XN_STATUS_OK) {
			status = mImageGen.Create(mContext);
			CHECK_RC(status, "Image Gen", true);
		}
		status = mImageGen.SetMapOutputMode(mMapMode);
		CHECK_RC(status, "Image Settings", true);
		mImageSurface = ci::Surface8u(mMapMode.nXRes, mMapMode.nYRes, false);
	}

	status = mContext.FindExistingNode(XN_NODE_TYPE_USER, mUserGen);
	if (status != XN_STATUS_OK) {
		status = mUserGen.Create(mContext);
		CHECK_RC(status, "User Gen", true);
	}
	mUserGen.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

	if (!mUserGen.IsCapabilitySupported((const char*)XN_CAPABILITY_SKELETON)) {
		ci::app::console() << "Supplied user generator doesn't support skeleton" << endl;
		exit(-1);
	}

	status = mContext.FindExistingNode(XN_NODE_TYPE_SCENE, mSceneAnalyzer);
	if (status != XN_STATUS_OK) {
		status = mSceneAnalyzer.Create(mContext);
		CHECK_RC(status, "Scene Analyzer", true);
	}

	registerCallbacks();
}

void WuCinderNITE::useCalibrationFile(string filepath)
{
	mCalibrationFile = filepath;
}

void WuCinderNITE::startGenerating()
{
	mContext.StartGeneratingAll();
}
void WuCinderNITE::stopGenerating()
{
	if( mThread ) {
		stopUpdating();
	}
	mContext.StopGeneratingAll();
}

void WuCinderNITE::startUpdating()
{
	if (mRunUpdates) {
		return;
	}
	mRunUpdates = true;
	mContext.StartGeneratingAll();
	mThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&WuCinderNITE::updateLoop, this)));
}

void WuCinderNITE::stopUpdating()
{
	if (!mRunUpdates) {
		return;
	}
	mRunUpdates = false;

	mThread->join();
	mContext.StopGeneratingAll();
}

void WuCinderNITE::updateLoop() {
	while (mRunUpdates) {
		update();
	}
}

void WuCinderNITE::update()
{

	XnStatus status = XN_STATUS_OK;
	mMutex.lock();
		status = mContext.WaitAndUpdateAll();
		if( status != XN_STATUS_OK ) {
			ci::app::console() << "no update" << endl;
			return;
		}
		if( !mUserGen ) {
			ci::app::console() << "No user generator" << endl;
			return;
		}
		status = mContext.FindExistingNode(XN_NODE_TYPE_DEPTH, mDepthGen);
		if( status != XN_STATUS_OK ) {
			ci::app::console() << xnGetStatusString(status) << endl;
			return;
		}

		mSceneAnalyzer.GetFloor(mFloor);

		mUserGen.GetUserPixels(0, mSceneMeta);
		if (mUseDepthMap) {
			mDepthGen.GetMetaData(mDepthMeta);
			updateDepthSurface();
		}
		if (mUseColorImage) {
			mImageGen.GetMetaData(mImageMeta);
			updateImageSurface();
		}

		XnSkeletonJointTransformation joint;
		for(int i = 1; i < MAX_USERS; i++) {
			skeletons[i].isTracking = mUserGen.GetSkeletonCap().IsTracking(i);
			if (skeletons[i].isTracking) {
				for(int j = 1; j < MAX_JOINTS; j++) {
					mUserGen.GetSkeletonCap().GetSkeletonJoint(i, (XnSkeletonJoint)j, joint);
					skeletons[i].joints[j].confidence = joint.position.fConfidence;
					skeletons[i].joints[j].position.x = joint.position.position.X / 1000.0f;
					skeletons[i].joints[j].position.y = joint.position.position.Y / 1000.0f;
					skeletons[i].joints[j].position.z = joint.position.position.Z / 1000.0f;
				}
			} else {
				for(int j = 1; j < MAX_JOINTS; j++) {
					skeletons[i].joints[j].confidence = 0;
				}
			}
		}
	mMutex.unlock();
}

void WuCinderNITE::updateDepthSurface()
{
	int w = mDepthMeta.XRes();
	int h = mDepthMeta.YRes();
	unsigned int numPoints = 0;
	unsigned int nValue = 0;
	unsigned int nHistValue = 0;

	const XnDepthPixel* pDepth = mDepthMeta.Data();
	const XnLabel* pLabels = mSceneMeta.Data();
	bool hasSceneData = pLabels != 0;

	// set all items of the array to 0
	memset(mDepthHistogram, 0, MAX_DEPTH*sizeof(float));

	// histogram logic from NiSimpleViewer.cpp
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			if (*pDepth != 0) {
				mDepthHistogram[*pDepth]++;
				numPoints++;
			}
			pDepth++;
		}
	}
	for (int nIndex=1; nIndex < MAX_DEPTH; nIndex++) {
		mDepthHistogram[nIndex] += mDepthHistogram[nIndex-1];
	}
	if (numPoints) {
		for (int nIndex = 1; nIndex < MAX_DEPTH; nIndex++) {
			mDepthHistogram[nIndex] = (unsigned int)(256 * (1.0f - (mDepthHistogram[nIndex] / numPoints)));
		}
	}

	pDepth = mDepthMeta.Data();
	ci::Surface::Iter iter = mDepthSurface.getIter( mDrawArea );
	while( iter.line() ) {
		while( iter.pixel() ) {
			iter.r() = 0;
			iter.g() = 0;
			iter.b() = 0;

			if ( hasSceneData && *pLabels != 0) {
				nValue = *pDepth;
				XnLabel label = *pLabels;
				XnUInt32 nColorID = label % mNITENumNITEUserColors;
				if (label == 0) {
					nColorID = mNITENumNITEUserColors;
				}
				if (nValue != 0) {
					nHistValue = mDepthHistogram[nValue];

					iter.r() = nHistValue * mNITEUserColors[nColorID][0];
					iter.g() = nHistValue * mNITEUserColors[nColorID][1];
					iter.b() = nHistValue * mNITEUserColors[nColorID][2];
				}
			} else {
				nValue = *pDepth;
				if (nValue != 0) {
					nHistValue = mDepthHistogram[nValue];
					iter.r() = nHistValue;
					iter.g() = nHistValue;
					iter.b() = nHistValue;
				}
			}
			pDepth++;
			pLabels++;
		}
	}
}

void WuCinderNITE::updateImageSurface()
{
	mMutexImageSurface.lock();
		int w = mImageMeta.XRes();
		const XnRGB24Pixel* pImage = mImageMeta.RGB24Data();
		ci::Surface::Iter iter = mImageSurface.getIter( mDrawArea );
		while( iter.line() ) {
			pImage += w;
			while( iter.pixel() ) {
				iter.r() = pImage->nRed;
				iter.g() = pImage->nGreen;
				iter.b() = pImage->nBlue;
				pImage--; // image is flipped, let's read backwards
			}
			pImage += w;
		}
	mMutexImageSurface.unlock();
}

ci::Surface8u WuCinderNITE::getDepthSurface()
{
	return mDepthSurface;
}

ci::Surface8u WuCinderNITE::getImageSurface()
{
	return mImageSurface;
}

XnMapOutputMode WuCinderNITE::getMapMode()
{
	return mMapMode;
}

void WuCinderNITE::renderDepthMap(ci::Area area)
{
	if (mUseDepthMap) {
		ci::gl::pushMatrices();
		ci::gl::draw( ci::gl::Texture(mDepthSurface, ci::gl::Texture::Format::Format() ), area );
		ci::gl::popMatrices();
	}
}

void WuCinderNITE::renderColor(ci::Area area)
{
	if(mUseColorImage) {
		ci::gl::draw( ci::gl::Texture(mImageSurface, ci::gl::Texture::Format::Format() ), area );
	}
}

void WuCinderNITE::renderSkeleton(XnUserID nId)
{
	if (nId == 0) {
		for (int nUser = 1; nUser < MAX_USERS; nUser++) {
			renderSkeleton(skeletons[nUser], nUser);
		}
	} else {
		renderSkeleton(skeletons[nId], nId);
	}
}

void WuCinderNITE::renderSkeleton(SKELETON::SKELETON &skeleton, XnUserID nId)
{
	if (skeleton.isTracking) {
		glLineWidth(3);
		int colorIndex = (nId) % mNITENumNITEUserColors;
		ci::gl::color(mNITEUserColors[colorIndex][0],
					  mNITEUserColors[colorIndex][1],
					  mNITEUserColors[colorIndex][2], 1);

		// HEAD TO NECK
		renderLimb(skeleton, XN_SKEL_HEAD, XN_SKEL_NECK);

		// Left Arm
		renderLimb(skeleton, XN_SKEL_NECK, XN_SKEL_LEFT_SHOULDER);
		renderLimb(skeleton, XN_SKEL_LEFT_SHOULDER, XN_SKEL_LEFT_ELBOW);
		renderLimb(skeleton, XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_HAND);
		renderLimb(skeleton, XN_SKEL_LEFT_HAND, XN_SKEL_LEFT_FINGERTIP);

		// RIGHT ARM
		renderLimb(skeleton, XN_SKEL_NECK, XN_SKEL_RIGHT_SHOULDER);
		renderLimb(skeleton, XN_SKEL_RIGHT_SHOULDER, XN_SKEL_RIGHT_ELBOW);
		renderLimb(skeleton, XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_HAND);
		renderLimb(skeleton, XN_SKEL_RIGHT_HAND, XN_SKEL_RIGHT_FINGERTIP);
		// TORSO
		renderLimb(skeleton, XN_SKEL_LEFT_SHOULDER, XN_SKEL_TORSO);
		renderLimb(skeleton, XN_SKEL_RIGHT_SHOULDER, XN_SKEL_TORSO);

		// LEFT LEG
		renderLimb(skeleton, XN_SKEL_TORSO, XN_SKEL_LEFT_HIP);
		renderLimb(skeleton, XN_SKEL_LEFT_HIP, XN_SKEL_LEFT_KNEE);
		renderLimb(skeleton, XN_SKEL_LEFT_KNEE, XN_SKEL_LEFT_FOOT);

		// RIGHT LEG
		renderLimb(skeleton, XN_SKEL_TORSO, XN_SKEL_RIGHT_HIP);
		renderLimb(skeleton, XN_SKEL_RIGHT_HIP, XN_SKEL_RIGHT_KNEE);
		renderLimb(skeleton, XN_SKEL_RIGHT_KNEE, XN_SKEL_RIGHT_FOOT);
		// PELVIS
		renderLimb(skeleton, XN_SKEL_LEFT_HIP, XN_SKEL_RIGHT_HIP);
		glLineWidth(1);
	}
	ci::gl::color(1, 1, 1, 1);
}

void WuCinderNITE::renderLimb(SKELETON::SKELETON &skeleton, XnSkeletonJoint eJoint1, XnSkeletonJoint eJoint2, float confidence)
{
	if (!skeleton.isTracking) {
	//	ci::app::console() << "user not tracked!" << endl;
		return;
	}
	if (skeleton.joints[eJoint1].confidence < confidence || skeleton.joints[eJoint2].confidence < confidence) {
		return;
	}
	if (true) {
		// 3D - arbitrary scale
		ci::gl::drawLine(skeleton.joints[eJoint1].position,skeleton.joints[eJoint2].position);

	} else {
		// 2D
		XnPoint3D pt[2];
		pt[0].X = skeleton.joints[eJoint1].position.x;
		pt[0].Y = skeleton.joints[eJoint1].position.y;
		pt[0].Z = skeleton.joints[eJoint1].position.z;
		pt[1].X = skeleton.joints[eJoint2].position.x;
		pt[2].Y = skeleton.joints[eJoint2].position.y;
		pt[3].Z = skeleton.joints[eJoint2].position.z;
		mDepthGen.ConvertRealWorldToProjective(2, pt, pt);
		ci::gl::drawLine(ci::Vec2f(pt[0].X, pt[0].Y), ci::Vec2f(pt[1].X, pt[1].Y));
	}
}

void WuCinderNITE::startTracking(XnUserID nId)
{
	mUserGen.GetSkeletonCap().StartTracking(nId);
	mUserGen.GetSkeletonCap().SetSmoothing(0.5f);
	if (waitForTrackingToSingalNewUser) {
		signalNewUser(nId);
	}
}

void WuCinderNITE::registerCallbacks()
{
	XnStatus status = XN_STATUS_OK;
	status = mUserGen.RegisterUserCallbacks(mInstance->CB_NewUser, mInstance->CB_LostUser, NULL, hUserCBs);
	CHECK_RC(status, "User callbacks", true);

	status = mUserGen.GetSkeletonCap().RegisterCalibrationCallbacks(mInstance->CB_CalibrationStart, mInstance->CB_CalibrationEnd, NULL, hCalibrationPhasesCBs);
	CHECK_RC(status, "Calibrations callbacks 1", true);

	status = mUserGen.GetSkeletonCap().RegisterToCalibrationComplete(mInstance->CB_CalibrationComplete, NULL, hCalibrationCompleteCBs);
	CHECK_RC(status, "Calibrations callbacks 2", true);

	if (mUserGen.GetSkeletonCap().NeedPoseForCalibration()) {
		mNeedPoseForCalibration = true;
		if (!mUserGen.IsCapabilitySupported((const char*)XN_CAPABILITY_POSE_DETECTION)) {
			ci::app::console() << "Need pose for calibration but device does not support it" << endl;
			exit(-1);
		}

		status = mUserGen.GetPoseDetectionCap().RegisterToPoseCallbacks(mInstance->CB_PoseDetected, NULL, NULL, hPoseCBs);
		CHECK_RC(status, "Pose callbacks", true);

		mUserGen.GetSkeletonCap().GetCalibrationPose(mCalibrationPose);
	}
}

void WuCinderNITE::unregisterCallbacks()
{
	mUserGen.UnregisterUserCallbacks(hUserCBs);
	mUserGen.GetSkeletonCap().UnregisterCalibrationCallbacks(hCalibrationPhasesCBs);
	mUserGen.GetSkeletonCap().UnregisterFromCalibrationComplete(hCalibrationCompleteCBs);
	if (mUserGen.GetSkeletonCap().NeedPoseForCalibration()) {
		mUserGen.GetPoseDetectionCap().UnregisterFromPoseCallbacks(hPoseCBs);
	}
}

void XN_CALLBACK_TYPE WuCinderNITE::CB_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	ci::app::console() << "new user " << nId << endl;
	if (!mInstance->mCalibrationFile.empty()) {
		mInstance->mUserGen.GetSkeletonCap().LoadCalibrationDataFromFile(nId, mInstance->mCalibrationFile.c_str());
		mInstance->startTracking(nId);
	}
	else if (mInstance->mNeedPoseForCalibration) {
		if ((mInstance->useSingleCalibrationMode && mInstance->mIsCalibrated) || mInstance->mUserGen.GetSkeletonCap().IsCalibrated(nId)) {
			mInstance->mUserGen.GetSkeletonCap().LoadCalibrationData(nId, mInstance->useSingleCalibrationMode ? 0 : nId);
			mInstance->startTracking(nId);
		} else {
			mInstance->mUserGen.GetPoseDetectionCap().StartPoseDetection(mInstance->mCalibrationPose, nId);
		}
	}
	else {
		if ((mInstance->useSingleCalibrationMode && mInstance->mIsCalibrated) || mInstance->mUserGen.GetSkeletonCap().IsCalibrated(nId)) {
			mInstance->mUserGen.GetSkeletonCap().LoadCalibrationData(nId, mInstance->useSingleCalibrationMode ? 0 : nId);
			mInstance->startTracking(nId);
		} else {
			mInstance->mUserGen.GetSkeletonCap().RequestCalibration(nId, TRUE);
		}
	}
	if (!mInstance->waitForTrackingToSingalNewUser) {
		mInstance->signalNewUser(nId);
	}
}
void XN_CALLBACK_TYPE WuCinderNITE::CB_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	ci::app::console() << "lost user " << nId << endl;
	mInstance->signalLostUser(nId);
}
void XN_CALLBACK_TYPE WuCinderNITE::CB_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
	// nothing
}
void XN_CALLBACK_TYPE WuCinderNITE::CB_CalibrationEnd(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie)
{
	ci::app::console() << "CB_CalibrationEnd " << nId << (bSuccess ? " success" : " failed")  << endl;
	if (!bSuccess) {
		if (mInstance->mNeedPoseForCalibration) {
			mInstance->mUserGen.GetPoseDetectionCap().StartPoseDetection(mInstance->mCalibrationPose, nId);
		} else {
			mInstance->mUserGen.GetSkeletonCap().RequestCalibration(nId, TRUE);
		}
	}
}
void XN_CALLBACK_TYPE WuCinderNITE::CB_CalibrationComplete(xn::SkeletonCapability& skeleton, XnUserID nId, XnCalibrationStatus eStatus, void* cxt)
{
	ci::app::console() << "calibration completed for user " << nId << (eStatus == XN_CALIBRATION_STATUS_OK ? " success" : " failed") << endl;
	if (eStatus == XN_CALIBRATION_STATUS_OK) {
		if (!mInstance->mIsCalibrated) {
			if (mInstance->useSingleCalibrationMode) {
				mInstance->mIsCalibrated = TRUE;
				mInstance->mUserGen.GetSkeletonCap().SaveCalibrationData(nId, 0);
			} else {
				mInstance->mUserGen.GetSkeletonCap().SaveCalibrationData(nId, nId);
			}
			mInstance->startTracking(nId);
		}
		mInstance->mUserGen.GetPoseDetectionCap().StopPoseDetection(nId);
	}
}
void XN_CALLBACK_TYPE WuCinderNITE::CB_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	ci::app::console() << "pose detected for user " << nId << endl;
	mInstance->mUserGen.GetPoseDetectionCap().StopPoseDetection(nId);
	mInstance->mUserGen.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

