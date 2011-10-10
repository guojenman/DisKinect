/*
 * TimeLapseRGB.h
 *
 *  Created on: Oct 8, 2011
 *      Author: onedayitwillmake
 */

#ifndef TIMELAPSERGB_H_
#define TIMELAPSERGB_H_

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

class TimeLapseRGB {
public:
	TimeLapseRGB();
	virtual ~TimeLapseRGB();
	void stopThread();

	boost::mutex _mutex;
private:
	boost::thread _surfaceSaver;
	volatile bool _shouldStopThread;
	std::string _saveDirectory;	// Create and set the directory in the constructor to avoid cinder getHomeDirectory / createDirectories multi-threaded memory leak

	void saveImage();
	bool hasEnoughDiskSpace();
	int64_t getEpochTime();
};

#endif /* TIMELAPSERGB_H_ */
