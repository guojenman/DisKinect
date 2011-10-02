#include "cinder/app/App.h"
#include "cinder/Camera.h"
#include "cinder/PolyLine.h"
#include "Puppeteer.h"
#include "XnTypes.h"

#include "WuCinderNITE.h"

using namespace ci;

namespace puppeteer {

Puppeteer::Puppeteer() {
}

Puppeteer::~Puppeteer() {
}

void Puppeteer::update(SKELETON::SKELETON& skeleton)
{
	if ( skeleton.joints[XN_SKEL_LEFT_SHOULDER].confidence == 0 || skeleton.joints[XN_SKEL_RIGHT_SHOULDER].confidence == 0 ) {
		return;
	}

	Vec3f &lShoulder = skeleton.joints[XN_SKEL_LEFT_SHOULDER].position;
	Vec3f &rShoulder = skeleton.joints[XN_SKEL_RIGHT_SHOULDER].position;
	Vec3f &neck = skeleton.joints[XN_SKEL_NECK].position;

	// calculate length for both left and right arms based on skeleton size
	float lLen = lShoulder.distance(skeleton.joints[XN_SKEL_LEFT_ELBOW].position)
			+ skeleton.joints[XN_SKEL_LEFT_ELBOW].position.distance(skeleton.joints[XN_SKEL_LEFT_HAND].position);
	float rLen = rShoulder.distance(skeleton.joints[XN_SKEL_RIGHT_ELBOW].position)
			+ skeleton.joints[XN_SKEL_RIGHT_ELBOW].position.distance(skeleton.joints[XN_SKEL_RIGHT_HAND].position);

	// get the 3 axis aligned to the body
	Vec3f axisX = (skeleton.joints[XN_SKEL_LEFT_SHOULDER].position - skeleton.joints[XN_SKEL_RIGHT_SHOULDER].position).normalized();
	Vec3f axisY = (skeleton.joints[XN_SKEL_NECK].position - skeleton.joints[XN_SKEL_TORSO].position).normalized();
	Vec3f axisZ = axisX.cross(axisY).normalized();


	// totally dirty test code below

	ci::CameraPersp mCam;
	mCam.setPerspective(60.0f, cinder::app::App::get()->getWindowAspectRatio(), 1.0f, WuCinderNITE::getInstance()->maxDepth);
	mCam.lookAt(ci::Vec3f(0, 0, -500.0f), ci::Vec3f::zero());

	PolyLine<Vec3f> poly;
	poly.setClosed(true);
	poly.push_back(lShoulder - axisY * lLen);
	poly.push_back(lShoulder - axisY * lLen + axisX * lLen);
	poly.push_back(lShoulder + axisY * lLen + axisX * lLen);
	poly.push_back(lShoulder + axisY * lLen);


	gl::pushMatrices();
	gl::setMatrices(mCam);
	glLineWidth(1.0f);
	gl::draw(poly);
	gl::popMatrices();

}

}
