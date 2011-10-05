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

private:
    void setupGUI();
    void sendMessageImp( std::string message, bool forceSend );

    // DEBUG
    bool useGUI;
	bool ignoreExternalSendRequest;

	// SimpleGUI
    mowa::sgui::SimpleGUI* _gui;		// SimpleGUI instance
    void addButton( std::string name, std::string command );		// Adds a button to the gui, registers the clickevent and stores it in a std::map

    bool onGuiButtonClicked( ci::app::MouseEvent event );			//
    bool onResetClicked( ci::app::MouseEvent event );			//

    std::map< mowa::sgui::ButtonControl*, std::string > _buttonMap; // Map each button to a string
};
