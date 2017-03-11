#include "ofMain.h"
#include "cerraduraApp.h"
#include "sombreroApp.h"
#include "tazasApp.h"
/*
* AUTHORS:
* Adrián López, Raúl Martín, Berta Martínez, Marta Navarro
*/
//========================================================================
int main() {

	ofGLWindowSettings settings;

	settings.width = 1024;
	settings.height = 768;
	settings.setPosition(ofVec2f(-1024, 0));
	shared_ptr<ofAppBaseWindow> cerraduraWindow = ofCreateWindow(settings);

	settings.width = 1024;
	settings.height = 768;
	settings.setPosition(ofVec2f(0, 0));//896
	shared_ptr<ofAppBaseWindow> sombreroWindow = ofCreateWindow(settings);

	settings.width = 1024;
	settings.height = 768;
	settings.setPosition(ofVec2f(1024, 0));//1920
	shared_ptr<ofAppBaseWindow> tazasWindow = ofCreateWindow(settings);
	
	shared_ptr<cerraduraApp> cerraduraApp(new cerraduraApp);
	shared_ptr<sombreroApp> sombreroApp(new sombreroApp);
	shared_ptr<tazasApp> tazasApp(new tazasApp);

	ofRunApp(cerraduraWindow, cerraduraApp);
	ofRunApp(tazasWindow, tazasApp);
	ofRunApp(sombreroWindow, sombreroApp);

	ofRunMainLoop();

}
