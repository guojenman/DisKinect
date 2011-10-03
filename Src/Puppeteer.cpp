#include "cinder/app/App.h"
#include "cinder/Camera.h"
#include "cinder/PolyLine.h"
#include "cinder/Quaternion.h"
#include "cinder/Matrix22.h"
#include "cinder/Matrix33.h"
#include "cinder/Matrix44.h"
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
	poly.push_back(lShoulder - axisY * lLen);
	poly.push_back(lShoulder - axisY * lLen + axisX * lLen);
	poly.push_back(lShoulder + axisY * lLen + axisX * lLen);
	poly.push_back(lShoulder + axisY * lLen);
	poly.push_back(lShoulder - axisY * lLen);
	poly.push_back(lShoulder - axisY * lLen + axisZ * lLen); //
	poly.push_back(lShoulder - axisY * lLen + axisZ * lLen + axisX * lLen);
	poly.push_back(lShoulder + axisY * lLen + axisZ * lLen + axisX * lLen);
	poly.push_back(lShoulder + axisY * lLen + axisZ * lLen);
	poly.push_back(lShoulder - axisY * lLen + axisZ * lLen);

	Vec3f vec1 = axisY * lLen;
	Vec3f vec2 = Vec3f(0, lLen, 0);


	gl::pushMatrices();
	gl::setMatrices(mCam);

	// original bounding box
	glLineWidth(1.0f);
	gl::color(Color(1, 0, 0));
	gl::pushModelView();
	gl::draw(poly);
	gl::popModelView();

	// transformed bounding box
	gl::color(Color(0, 1, 0));
	gl::pushModelView();
	gl::scale(.2f, .2f, .2f);
	gl::rotate(Quatf(vec1.cross(vec2), -acos(vec1.dot(vec2))));
	gl::translate(-1.0f * lShoulder);
	gl::draw(poly);
	gl::popModelView();

	gl::popMatrices();

	gl::color(Color(1, 1, 1));

}

}
