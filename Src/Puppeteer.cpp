#include "cinder/app/App.h"
#include "cinder/CinderMath.h"
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"
#include "cinder/PolyLine.h"
#include "cinder/Quaternion.h"
#include "cinder/Matrix22.h"
#include "cinder/Matrix33.h"
#include "cinder/Matrix44.h"
#include "Puppeteer.h"
#include "XnTypes.h"

#include "Constants.h"

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

	// calculate length for both left and right arms based on skeleton size
	float lLen = lShoulder.distance(skeleton.joints[XN_SKEL_LEFT_ELBOW].position)
			+ skeleton.joints[XN_SKEL_LEFT_ELBOW].position.distance(skeleton.joints[XN_SKEL_LEFT_HAND].position);
	float rLen = rShoulder.distance(skeleton.joints[XN_SKEL_RIGHT_ELBOW].position)
			+ skeleton.joints[XN_SKEL_RIGHT_ELBOW].position.distance(skeleton.joints[XN_SKEL_RIGHT_HAND].position);

	// get the 3 axis aligned to the body
	Vec3f axisX = (skeleton.joints[XN_SKEL_LEFT_SHOULDER].position - skeleton.joints[XN_SKEL_RIGHT_SHOULDER].position).normalized();
	Vec3f axisY = (skeleton.joints[XN_SKEL_NECK].position - skeleton.joints[XN_SKEL_TORSO].position).normalized();
	Vec3f normal = axisX.cross(axisY).normalized();

	Vec3f v1 = Vec3f::zAxis().normalized();
	Vec3f v2 = normal.normalized();
	// align rectangular region to z-axis
	Quatf q = Quatf( v1.cross(v2).normalized(), -acos(v1.dot(v2)) );
	Matrix33<float> m1 = Matrix33<float>::createRotation(q.getAxis(), q.getAngle());
	// normal aligned with z-axis but rectangular region is rotated around the z-axis, we need to undo this rotation
	Vec3f p = m1.transformVec(axisY);
	float theta = atan2(p.y, p.x);
	Matrix33<float> m2 = Matrix33<float>::createRotation(Vec3f::zAxis(), -theta + M_PI / 2.0f);
	Matrix33<float> m3 = m2 * m1;

	Vec3f lHand = m3.transformVec(skeleton.joints[XN_SKEL_LEFT_HAND].position - lShoulder);
	Vec3f rHand = m3.transformVec(skeleton.joints[XN_SKEL_RIGHT_HAND].position - rShoulder);

	if (Constants::Debug::DRAW_PUPPETEER_BOUNDS) {
		Vec3f origin = Vec3f::zero();
		PolyLine<Vec3f> lBounds;
		lBounds.push_back(origin - axisY * lLen);
		lBounds.push_back(origin - axisY * lLen + axisX * lLen);
		lBounds.push_back(origin + axisY * lLen + axisX * lLen);
		lBounds.push_back(origin + axisY * lLen);
		lBounds.push_back(origin - axisY * lLen);
		lBounds.push_back(origin - axisY * lLen - normal * lLen); //
		lBounds.push_back(origin - axisY * lLen - normal * lLen + axisX * lLen);
		lBounds.push_back(origin + axisY * lLen - normal * lLen + axisX * lLen);
		lBounds.push_back(origin + axisY * lLen - normal * lLen);
		lBounds.push_back(origin - axisY * lLen - normal * lLen);

		PolyLine<Vec3f> rBounds;
		rBounds.push_back(origin - axisY * rLen);
		rBounds.push_back(origin - axisY * rLen - axisX * rLen);
		rBounds.push_back(origin + axisY * rLen - axisX * rLen);
		rBounds.push_back(origin + axisY * rLen);
		rBounds.push_back(origin - axisY * rLen);
		rBounds.push_back(origin - axisY * rLen - normal * rLen); //
		rBounds.push_back(origin - axisY * rLen - normal * rLen - axisX * rLen);
		rBounds.push_back(origin + axisY * rLen - normal * rLen - axisX * rLen);
		rBounds.push_back(origin + axisY * rLen - normal * rLen);
		rBounds.push_back(origin - axisY * rLen - normal * rLen);

		gl::pushMatrices();

		MayaCamUI* mayaCam = Constants::mayaCam();
		gl::setMatrices(mayaCam->getCamera());

		float scale = .22f;
		// original bounding box
		gl::color(Color(1, 0, 0));
		gl::pushModelView();
		gl::translate(lShoulder);
		gl::draw(lBounds);
		gl::popModelView();

		gl::color(Color(1, 0, 0));
		gl::pushModelView();
		gl::translate(rShoulder);
		gl::draw(rBounds);
		gl::popModelView();

		gl::color(Color(0, 0, 1));
		gl::pushModelView();
		gl::scale(scale, scale, scale);
		gl::rotate( Quatf(m3) );
		gl::draw(lBounds);
		gl::popModelView();

		gl::pushModelView();
		gl::scale(scale, scale, scale);
		gl::rotate( Quatf(m3) );
		gl::draw(rBounds);
		gl::popModelView();

		gl::pushModelView();
		gl::scale(scale, scale, scale);
		gl::drawCube(lHand, Vec3f(100.0f, 100.0f, 100.0f));
		gl::popModelView();

		gl::pushModelView();
		gl::scale(scale, scale, scale);
		gl::drawCube(rHand, Vec3f(100.0f, 100.0f, 100.0f));
		gl::popModelView();

		gl::popMatrices();

		gl::color(Color(1, 1, 1));
	}


	lHand.x = math<float>::clamp(lHand.x / lLen, 0.0f, 1.0f);
	lHand.z = math<float>::clamp(lHand.z / lLen, 0.0f, 1.0f);
	lHand.y = math<float>::clamp((lHand.y + lLen) / lLen * 2.0f, 0.0f, 1.0f);

	rHand.x = math<float>::clamp(rHand.x / rLen * -1.0f, 0.0f, 1.0f);
	rHand.z = math<float>::clamp(rHand.z / rLen, 0.0f, 1.0f);
	rHand.y = math<float>::clamp((rHand.y + rLen) / rLen * 2.0f, 0.0f, 1.0f);

}

}
