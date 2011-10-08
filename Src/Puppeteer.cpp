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
	//arduino.setup("");
}

Puppeteer::~Puppeteer() {
}

void Puppeteer::update(SKELETON::SKELETON& skeleton)
{
	if ( skeleton.joints[XN_SKEL_LEFT_SHOULDER].confidence == 0 || skeleton.joints[XN_SKEL_RIGHT_SHOULDER].confidence == 0 ) {
		return;
	}
	// ----------------------------legs

	Vec3f &lHip = skeleton.joints[XN_SKEL_LEFT_HIP].position;
	Vec3f &rHip = skeleton.joints[XN_SKEL_RIGHT_HIP].position;
	Vec3f &lKnee = skeleton.joints[XN_SKEL_LEFT_KNEE].position;
	Vec3f &rKnee = skeleton.joints[XN_SKEL_RIGHT_KNEE].position;
	float lLegLen = lKnee.distance(lHip);
	float rLegLen = rKnee.distance(rHip);
	float lLegPos = math<float>::clamp(1.0f - (lHip.y - lKnee.y + lLegLen * .25f) / (lLegLen * 1.25f), 0.0f, 1.0f);
	float rLegPos = math<float>::clamp(1.0f - (rHip.y - rKnee.y + rLegLen * .25f) / (rLegLen * 1.25f), 0.0f, 1.0f);

	// ----------------------------hands
	Vec3f &lShoulder = skeleton.joints[XN_SKEL_LEFT_SHOULDER].position;
	Vec3f &rShoulder = skeleton.joints[XN_SKEL_RIGHT_SHOULDER].position;

	// calculate length for both left and right arms based on skeleton size
	float lArmLen = lShoulder.distance(skeleton.joints[XN_SKEL_LEFT_ELBOW].position)
			+ skeleton.joints[XN_SKEL_LEFT_ELBOW].position.distance(skeleton.joints[XN_SKEL_LEFT_HAND].position);
	float rArmLen = rShoulder.distance(skeleton.joints[XN_SKEL_RIGHT_ELBOW].position)
			+ skeleton.joints[XN_SKEL_RIGHT_ELBOW].position.distance(skeleton.joints[XN_SKEL_RIGHT_HAND].position);

	// get the 3 axis aligned to the body
	Vec3f axisX = (skeleton.joints[XN_SKEL_LEFT_SHOULDER].position - skeleton.joints[XN_SKEL_RIGHT_SHOULDER].position).normalized();
	Vec3f axisY = (skeleton.joints[XN_SKEL_NECK].position - skeleton.joints[XN_SKEL_TORSO].position).normalized();
	Vec3f normal = axisX.cross(axisY).normalized();

	Vec3f v1 = Vec3f(0, 0, 1).normalized();
	Vec3f v2 = normal;
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


	// ----------------------------send to arduino
	Vec3f lHandPos = Vec3f(
		math<float>::clamp(lHand.x / lArmLen, 0.0f, 1.0f),
		math<float>::clamp(lHand.z / lArmLen, 0.0f, 1.0f),
		math<float>::clamp((lHand.y + lArmLen) / lArmLen * 2.0f, 0.0f, 1.0f)
	);
	Vec3f rHandPos = Vec3f(
		math<float>::clamp(rHand.x / rArmLen * -1.0f, 0.0f, 1.0f),
		math<float>::clamp(rHand.z / rArmLen, 0.0f, 1.0f),
		math<float>::clamp((rHand.y + rArmLen) / rArmLen * 2.0f, 0.0f, 1.0f)
	);

	//arduino.sendMessage();

	// ----------------------------debug

	if (Constants::Debug::DRAW_PUPPETEER_BOUNDS) {
		Vec3f origin = Vec3f::zero();
		PolyLine<Vec3f> lBounds;
		lBounds.push_back(origin - axisY * lArmLen);
		lBounds.push_back(origin - axisY * lArmLen + axisX * lArmLen);
		lBounds.push_back(origin + axisY * lArmLen + axisX * lArmLen);
		lBounds.push_back(origin + axisY * lArmLen);
		lBounds.push_back(origin - axisY * lArmLen);
		lBounds.push_back(origin - axisY * lArmLen - normal * lArmLen); //
		lBounds.push_back(origin - axisY * lArmLen - normal * lArmLen + axisX * lArmLen);
		lBounds.push_back(origin + axisY * lArmLen - normal * lArmLen + axisX * lArmLen);
		lBounds.push_back(origin + axisY * lArmLen - normal * lArmLen);
		lBounds.push_back(origin - axisY * lArmLen - normal * lArmLen);

		PolyLine<Vec3f> rBounds;
		rBounds.push_back(origin - axisY * rArmLen);
		rBounds.push_back(origin - axisY * rArmLen - axisX * rArmLen);
		rBounds.push_back(origin + axisY * rArmLen - axisX * rArmLen);
		rBounds.push_back(origin + axisY * rArmLen);
		rBounds.push_back(origin - axisY * rArmLen);
		rBounds.push_back(origin - axisY * rArmLen - normal * rArmLen); //
		rBounds.push_back(origin - axisY * rArmLen - normal * rArmLen - axisX * rArmLen);
		rBounds.push_back(origin + axisY * rArmLen - normal * rArmLen - axisX * rArmLen);
		rBounds.push_back(origin + axisY * rArmLen - normal * rArmLen);
		rBounds.push_back(origin - axisY * rArmLen - normal * rArmLen);

		gl::pushMatrices();

		MayaCamUI* mayaCam = Constants::mayaCam();
		gl::setMatrices(mayaCam->getCamera());

		float scale = .22f;
		// original bounding box
		gl::color(Color(0.5f, 0.5f, 0.5f));
		gl::pushModelView();
		gl::translate(lShoulder);
		gl::draw(lBounds);
		gl::popModelView();

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
		gl::drawCube(lHand, Vec3f(100.0f, 100.0f, 100.0f));
		gl::popModelView();

		gl::color(Color(0, 1, 0));
		gl::pushModelView();
		gl::scale(scale, scale, scale);
		gl::rotate( Quatf(m3) );
		gl::draw(rBounds);
		gl::popModelView();

		gl::pushModelView();
		gl::scale(scale, scale, scale);
		gl::drawCube(rHand, Vec3f(100.0f, 100.0f, 100.0f));
		gl::popModelView();

		gl::popMatrices();

		gl::color(Color(1, 1, 1));
	}


}

}
