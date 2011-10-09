/*
 * TimeLapseRGB.h
 *
 *  Created on: Oct 8, 2011
 *      Author: onedayitwillmake
 *      Abstract:
 *      	Records an image of whatever the kinect is looking at every N seconds
 */

#include "TimeLapseRGB.h"
#include "cinder/Filesystem.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/DataTarget.h"

#include "WuCinderNITE.h"
#include "Constants.h"

#include <boost/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <sstream>

TimeLapseRGB::TimeLapseRGB() {

	_saveDirectory = ci::getHomeDirectory() + Constants::TimeLapse::DIRECTORY_NAME + "/";
	ci::createDirectories( _saveDirectory );

	_shouldStopThread = false;
	_surfaceSaver = boost::thread( boost::bind( &TimeLapseRGB::saveImage, this ) );
}

TimeLapseRGB::~TimeLapseRGB() {
	stopThread();
}

void TimeLapseRGB::saveImage() {
	static int counter = 0;

	_mutex.lock();
		while( !_shouldStopThread ) {
			WuCinderNITE::getInstance()->mMutexImageSurface.lock();
				counter++;

				// Get psuedo epochtime
				static boost::posix_time::ptime epoch( boost::gregorian::date(2011, 1, 1) );
				boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();
				boost::posix_time::time_duration diff(now - epoch);
				int64_t secondsSinceEpoch = diff.ticks() / diff.ticks_per_second();

				std::stringstream filename;
				filename << _saveDirectory << "RGB_" << secondsSinceEpoch << ".png";

				// Create fake image if kinects 'mImageSurface' is not available ( .oni file or failure )
				ci::Surface8u imageSurface;
				if( WuCinderNITE::getInstance()->mImageSurface == 0 ) imageSurface = ci::Surface8u( 640, 480, false );
				else imageSurface = WuCinderNITE::getInstance()->mImageSurface;

				// Note we're writing using the alternative siganture of writeImage because letting Cinder create the directories...
				// ...while in a thread causes a std::string leak
				ci::writeImage( ci::writeFile( filename.str(), false ), imageSurface, ci::ImageTarget::Options(), "png" );

			WuCinderNITE::getInstance()->mMutexImageSurface.unlock();
			boost::this_thread::sleep( boost::posix_time::seconds( Constants::TimeLapse::SECONDS_BETWEEN_SNAPSHOT ) );
		}
	_mutex.unlock();
}

void TimeLapseRGB::stopThread() {
	std::cout << "TimeLapseRGB - Stopping thread..." << std::endl;
	_shouldStopThread = true;
	_surfaceSaver.join();
}
