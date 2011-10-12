
#include "ArduinoCommandInterface.h"
#include "Constants.h"

void ArduinoCommandInterface::setup(std::string device, bool debug = false)
{
	isDebug = debug;
	if(isDebug) printDevices();	
	bindDevice(device);
}

void ArduinoCommandInterface::bindDevice(std::string device)
{
    try {
		ci::Serial::Device dev = ci::Serial::findDeviceByNameContains(device);
		serial = ci::Serial( dev, 9600);
	}
	catch( ... ) {
		ci::app::console() << "There was an error initializing the serial device!" << std::endl;
		exit( -1 );
	}
}

void ArduinoCommandInterface::printDevices()
{
    // print the devices
	const std::vector<ci::Serial::Device> &devices( ci::Serial::getDevices() );
	for( std::vector<ci::Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
		ci::app::console() << "Device: " << deviceIt->getName() << std::endl;
	}
}

void ArduinoCommandInterface::sendMessage(std::string message)
{
	serial.writeString(message);
}


