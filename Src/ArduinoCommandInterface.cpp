
#include "ArduinoCommandInterface.h"
#include "simplegui/SimpleGUI.h"
#include "cinder/Color.h"
#include "cinder/app/App.h"
#include "Constants.h"

ArduinoCommandInterface::~ArduinoCommandInterface() {
	delete _gui; _gui = NULL;
	delete serial; serial = NULL;
}

void ArduinoCommandInterface::setup(std::string device, bool debug = false)
{
	isDebug = debug;
	useGUI = true;
	_gui = NULL;
	serial = NULL;
	ignoreExternalSendRequest = false;

	if(isDebug) printDevices();	
	bindDevice(device);

	setupGUI();
}

void ArduinoCommandInterface::setupGUI() {
	if( !useGUI ) return;

	using namespace mowa::sgui;
	_gui = new SimpleGUI( ci::app::App::get() );
	_gui->textColor = ci::ColorA(1,1,1,1);
	_gui->lightColor = ci::ColorA(1, 0, 1, 1);
	_gui->darkColor = ci::ColorA(0.05,0.05,0.05, 1);
	_gui->bgColor = ci::ColorA(0.15, 0.15, 0.15, 1.0);

	int yPadding = 145;
	_gui->addColumn( (SimpleGUI::labelSize.x + SimpleGUI::spacing) * 0, yPadding);
	_gui->addParam("IgnoreKinect", &ignoreExternalSendRequest, ignoreExternalSendRequest);
	mowa::sgui::ButtonControl* button = _gui->addButton( "Set As Zero" );
	button->registerClick( this, &ArduinoCommandInterface::onResetClicked );

	_gui->addColumn( (SimpleGUI::labelSize.x + SimpleGUI::spacing) * 1, yPadding);
	addButton( "LeftArmX_up", "q" );
	addButton( "LeftArmX_down", "w" );

	addButton( "LeftArmZ_up", "a" );
	addButton( "LeftArmZ_down", "s" );

	addButton( "LeftArmY_up", "z" );
	addButton( "LeftArmY_down", "x" );

	_gui->addColumn( (SimpleGUI::labelSize.x + SimpleGUI::spacing) * 2, yPadding);
	addButton( "RightArmX_up", "o" );
	addButton( "RightArmX_down", "p" );

	addButton( "RightArmZ_up", "k" );
	addButton( "RightArmZ_down", "l" );

	addButton( "RightArmY_up", "n" );
	addButton( "RightArmY_down", "m" );

	_gui->addColumn( (SimpleGUI::labelSize.x + SimpleGUI::spacing) * 3, yPadding);
	addButton( "LeftLeg_up", "d" );
	addButton( "LeftLeg_down", "f" );

	_gui->addColumn( (SimpleGUI::labelSize.x + SimpleGUI::spacing) * 4, yPadding);
	addButton( "RightLeg_up", "h" );
	addButton( "RightLeg_down", "j" );
}

void ArduinoCommandInterface::addButton( std::string name, std::string command ) {
	mowa::sgui::ButtonControl* button = _gui->addButton( name );
	button->registerClick( this, &ArduinoCommandInterface::onGuiButtonClicked );

	_buttonMap.insert( std::pair< mowa::sgui::ButtonControl*, std::string >(button, command) );
}

void ArduinoCommandInterface::bindDevice(std::string device)
{
    try {
		ci::Serial::Device dev = ci::Serial::findDeviceByNameContains(device);
		serial = new ci::Serial( dev, 9600);
	}
	catch( ... ) {
//		ci::app::console() << "There was an error initializing the serial device!" << std::endl;
//		exit( -1 );
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

// This is called externally, - if ignoreExternalSendRequest is true - don't actually send
void ArduinoCommandInterface::sendMessage(std::string message) {
	sendMessageImp( message, false );
}

void ArduinoCommandInterface::sendMessageImp(std::string message, bool forceSend ) {
	if( forceSend || !ignoreExternalSendRequest ) {
		std::cout << "Sending message!" << message << std::endl;

		if( serial )
			serial->writeString(message);
	}
}

void ArduinoCommandInterface::draw() {
	if( _gui )
		_gui->draw();
}

bool ArduinoCommandInterface::onGuiButtonClicked( ci::app::MouseEvent event ) {
	std::map< mowa::sgui::ButtonControl*, std::string >::iterator iter = _buttonMap.find( (mowa::sgui::ButtonControl*) _gui->getSelectedControl() );
	if ( iter == _buttonMap.end() ) {
		std::cout << " Could not locate button in buttonmap " << std::endl;
	}

	std::cout << "ForceSend!" << iter->second << std::endl;
	sendMessageImp( iter->second, true );
}

bool ArduinoCommandInterface::onResetClicked( ci::app::MouseEvent event ) {
	sendMessageImp( "r", true );
}

