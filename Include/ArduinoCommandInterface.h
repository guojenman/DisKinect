#include "cinder/app/AppBasic.h"
#include "cinder/Serial.h"
#include "cinder/Text.h"
#include "cinder/gl/Texture.h"

#include <sstream>
#include <map>
namespace mowa { namespace sgui { class Control; class LabelControl; class ButtonControl; class BoolVarControl; class IntVarControl; class SimpleGUI; }}

class ArduinoCommandInterface {
public:
	~ArduinoCommandInterface();
	void update();
	void draw();
	void setup(std::string device, bool debug);
    void printDevices();
    void bindDevice(std::string device);
	void sendMessage(std::string message);
	bool isDebug;
    ci::Serial* serial;

    // Debug stuff
    // GUI
    bool useGUI;
    bool ignoreExternalSendRequest;
private:
    void setupGUI();
    void sendMessageImp( std::string message, bool forceSend );
    bool onGuiButtonClicked( ci::app::MouseEvent event );

    mowa::sgui::SimpleGUI* _gui;		// SimpleGUI instance
    void addButton( std::string name, std::string command );
    std::map< mowa::sgui::ButtonControl*, std::string > _buttonMap; // Map buttons to Axis
};

/**
 * LeftArm X - UP / Down
LeftArm Y - UP / Down
LeftArm Z - UP / Down

RightArm X - UP / Down
RightArm Y - UP / Down
RightArm Z - UP / Down

LEFTLEG - UP/ DOWN
RIGHTLEG - UP/ DOWN

IGNORE_KINECT BOOL


 *
 */
