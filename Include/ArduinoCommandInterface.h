#include "cinder/app/AppBasic.h"
#include "cinder/Serial.h"
#include "cinder/Text.h"
#include "cinder/gl/Texture.h"

#include <sstream>

using namespace ci;
using namespace ci::app;
using namespace std;

class ArduinoCommandInterface {
public:
	void update();
	void setup(string device, bool debug);
    void printDevices();
    void bindDevice(string device);
	void sendMessage(string message);
	bool isDebug;
    Serial serial;
};
