#include "cinder/app/AppBasic.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"
#include "cinder/Camera.h"

#include "WuCinderNITE.h"
#include "UserTracker.h"

using namespace ci;
using namespace app;
using namespace std;

class DisKinect : public AppBasic {
public:
	void prepareSettings( AppBasic::Settings *settings );
	void setup();
	void update();
	void draw();
	void shutdown();

	void keyUp(KeyEvent event);

	WuCinderNITE* ni;
	UserTracker* tracker;

	CameraPersp mCam;
	Vec3f mCamEye;
	Vec3f mCamLookAt;
	Vec4f lightPosition;
};

void DisKinect::prepareSettings( AppBasic::Settings *settings )
{
	settings->setWindowSize( 640, 480 );
}

void DisKinect::setup()
{
	XnMapOutputMode mapMode;
	mapMode.nFPS = 30;
	mapMode.nXRes = 640;
	mapMode.nYRes = 480;

	ni = WuCinderNITE::getInstance();
	ni->setup("Resources/Captured.oni");
	//ni->setup("Resources/Sample-User.xml", mapMode, true, true);
	ni->startUpdating();

	tracker = UserTracker::getInstance();

	mCamEye = Vec3f(0, 0, -500.0f);
	mCamLookAt = Vec3f::zero();
}


void DisKinect::update()
{
	mCam.setPerspective(60.0f, getWindowAspectRatio(), 1.0f, ni->maxDepth);
	mCam.lookAt(mCamEye, mCamLookAt);
}

void DisKinect::draw()
{
	gl::clear(ColorA(0, 0, 0, 0), true);
	ni->renderColor(getWindowBounds());

	gl::pushMatrices();
	gl::setMatrices(mCam);
	ni->renderSkeleton();
	gl::popMatrices();
}

void DisKinect::shutdown()
{
	console() << "quitting..." << std::endl;
	tracker->release();
	ni->shutdown();
}

void DisKinect::keyUp(KeyEvent event)
{
	if (event.getChar() == KeyEvent::KEY_q) {
		quit();
	}
}

CINDER_APP_BASIC( DisKinect, RendererGl )
