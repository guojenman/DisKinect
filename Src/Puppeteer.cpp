#include "cinder/app/App.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Matrix22.h"
#include "Puppeteer.h"
#include "XnTypes.h"

using namespace ci;

namespace puppeteer {

MayaCamUI cam;

Puppeteer::Puppeteer() {
}

Puppeteer::~Puppeteer() {
}

void Puppeteer::update(SKELETON::SKELETON& skeleton)
{
	Vec3f torso = skeleton.joints[XN_SKEL_LEFT_HIP].position - skeleton.joints[XN_SKEL_LEFT_SHOULDER].position;
	Vec3f lElbow = skeleton.joints[XN_SKEL_LEFT_ELBOW].position - skeleton.joints[XN_SKEL_NECK].position;
	Vec3f lHand = skeleton.joints[XN_SKEL_LEFT_HAND].position - skeleton.joints[XN_SKEL_LEFT_ELBOW].position;
	Vec3f lKnee = skeleton.joints[XN_SKEL_LEFT_KNEE].position - skeleton.joints[XN_SKEL_LEFT_HIP].position;
	Vec3f rHand = skeleton.joints[XN_SKEL_RIGHT_HAND].position - skeleton.joints[XN_SKEL_RIGHT_ELBOW].position;
	Vec3f rKnee = skeleton.joints[XN_SKEL_RIGHT_KNEE].position - skeleton.joints[XN_SKEL_RIGHT_HIP].position;

	float thetaXZ = acos(torso.xz().normalized().dot(lElbow.xz().normalized()));
	Vec2f vecXZ = Matrix22<float>::createRotation(thetaXZ) * Vec2f(0.0f, -5.0f);

	float thetaYZ = acos(torso.yz().normalized().dot(lElbow.yz().normalized()));
	Vec2f vecYZ = Matrix22<float>::createRotation(thetaYZ) * Vec2f(0.0f, -5.0f);

	glLineWidth(3.0f);

	gl::pushMatrices();
	gl::setMatrices(cam.getCamera());
	gl::color(Color(1.0f, 0, 0));
	gl::drawLine(Vec2f::zero(), vecXZ);

	gl::color(Color(0, 1.0f, 0));
	gl::drawLine(Vec2f::zero(), vecYZ);
	gl::popMatrices();

}

}
