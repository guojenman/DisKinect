
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
    //unsigned char m_Test[20];    
    //std::strcpy( (char*) m_Test, message.c_str() );
    //serial.writeByte(m_Test[0]);

	serial.writeString(message);

	//if(isDebug) console() << message;
}

string ArduinoCommandInterface::fixMessage(string message)
{
	// remove return carriage (but doesn't remove returns)
	string::size_type pos = 0;
    while ( (pos = message.find("\r", pos)) != string::npos ) {
        message.replace( pos, 2, "" );
        pos++;
    }

	return message;
}

bool ArduinoCommandInterface::isValidCommand(string c)
{
	if(c == "1" || c == "2" || c == "3" || c == "4" || c == "5" || c == "6" || c == "q" || c == "w" || c =="e" || c =="r" || c == "t" || c == "y")
		return true;
	else
		return false;
}

