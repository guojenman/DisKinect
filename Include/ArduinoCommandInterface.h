#include "cinder/app/AppBasic.h"
#include "cinder/Serial.h"
#include "cinder/Text.h"
#include "cinder/gl/Texture.h"

#include <sstream>

class ArduinoCommandInterface {
public:
	void update();
	void setup(std::string device, bool debug);
    void printDevices();
    void bindDevice(std::string device);
	void sendMessage(std::string message);
	bool isDebug;
    ci::Serial serial;
};
