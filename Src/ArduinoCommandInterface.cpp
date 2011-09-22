
#include "ArduinoCommandInterface.h"

void ArduinoCommandInterface::setup(string device, bool debug = false)
{
	isDebug = debug;
	if(isDebug) printDevices();	
	bindDevice(device);
}

void ArduinoCommandInterface::bindDevice(string device)
{
    try {
		Serial::Device dev = Serial::findDeviceByNameContains(device);
		serial = Serial( dev, 9600);
	}
	catch( ... ) {
		console() << "There was an error initializing the serial device!" << std::endl;
		exit( -1 );
	}
}

void ArduinoCommandInterface::printDevices()
{
    // print the devices
	const vector<Serial::Device> &devices( Serial::getDevices() );
	for( vector<Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
		console() << "Device: " << deviceIt->getName() << endl;
	}
}

void ArduinoCommandInterface::sendMessage(string message)
{
	serial.writeString(message);
}


