#include "cinder/app/AppBasic.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"

#include "UserRelay.h"
#include "Puppeteer.h"

#include <OpenGL.framework/Headers/gl.h>
#include <OpenGL.framework/Headers/glext.h>

#include "WuCinderNITE.h"
#include "UserTracker.h"
#include "SkeletonStruct.h"
#include "Constants.h"
#include "TimeLapseRGB.h"

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
	void mouseDown( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void keyUp(KeyEvent event);

	UserTracker* userTracker;
	relay::UserRelay* userRelay;
	puppeteer::Puppeteer* puppetier;
	TimeLapseRGB* rgbSaver;
};

void DisKinect::prepareSettings( AppBasic::Settings *settings )
{
	settings->setWindowSize( 640, 480 );
}

void DisKinect::setup()
{
	WuCinderNITE* aNi = WuCinderNITE::getInstance();
	if (Constants::Debug::USE_RECORDED_ONI) {
		aNi->setup(getResourcePath("SkeletonRec.oni"));
//		aNi->setup(getResourcePath("SkeletonRec.oni"));
	} else {
		XnMapOutputMode mapMode;
		mapMode.nFPS = 30;
		mapMode.nXRes = 640;
		mapMode.nYRes = 480;
		aNi->setup(getResourcePath("Sample-User.xml"), mapMode, true, true);
	}

	aNi->useCalibrationFile(getResourcePath("calibration.dat"));
//	aNi->startUpdating();
	aNi->startGenerating();


	float qDepth = WuCinderNITE::getInstance()->maxDepth * 0.25f;
	ci::CameraPersp cam;
	cam.setPerspective(60.0f, cinder::app::App::get()->getWindowAspectRatio(), 1.0f, WuCinderNITE::getInstance()->maxDepth * 2.0f);
	cam.lookAt(ci::Vec3f(qDepth, qDepth, -qDepth), Vec3f(0, 0, qDepth));
//	cam.lookAt(ci::Vec3f(0, 0, -qDepth), ci::Vec3f::zero());
	Constants::mayaCam()->setCurrentCam( cam );

	userTracker = UserTracker::getInstance();
	userTracker->activationZone = Constants::UserTracker::ACTIVATION_ZONE;
	userRelay = new relay::UserRelay( aNi, userTracker );
	puppetier = new puppeteer::Puppeteer();

	if( Constants::Debug::CREATE_TIMELAPSE ) rgbSaver = new TimeLapseRGB();
	else rgbSaver = NULL;
}


void DisKinect::update()
{
	userRelay->update();

	SKELETON::SKELETON skeleton = userRelay->getSkeleton();
	puppetier->update(skeleton);
}

void DisKinect::draw()
{
	gl::clear( ColorA::black(), true);

	userRelay->draw();
	userTracker->draw();
	puppetier->draw();

}

void DisKinect::shutdown()
{
	console() << "quitting..." << std::endl;
	WuCinderNITE::getInstance()->stopGenerating();

	delete userRelay;
	delete puppetier;
	delete rgbSaver; rgbSaver = NULL;
}

void DisKinect::keyUp(KeyEvent event)
{
	if (event.getChar() == KeyEvent::KEY_q) {
		quit();
	}
}
void DisKinect::mouseDown( MouseEvent event )
{
	MayaCamUI* mayaCam = Constants::mayaCam();
	mayaCam->mouseDown(	event.getPos());
}
void DisKinect::mouseDrag( MouseEvent event )
{
	MayaCamUI* mayaCam = Constants::mayaCam();
	mayaCam->mouseDrag(event.getPos(), event.isControlDown(), event.isAltDown(), event.isShiftDown());
}

CINDER_APP_BASIC( DisKinect, RendererGl )
