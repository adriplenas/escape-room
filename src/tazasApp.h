#pragma once
/*
* AUTHORS:
* Adrián López, Raúl Martín, Berta Martínez, Marta Navarro
*/
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxOsc.h"

#define PORTCT 12344  // cerradura - tazas
#define PORTCS 12345  // cerradura - sombrero
#define PORTSC 12346  // sombrero - cerradura
#define PORTST 12347  // sombrero - tazas
#define PORTTC 12348  // tazas - cerradura
#define PORTTS 12349  // tazas - sombrero

#define HOST "localhost"
class tazasApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);



	// variables de control
	float pruebas = 0.0;  // contador de pruebas superadas
	bool reslibro = false;

						  // modelo 3d habitacion
	ofxAssimpModelLoader model;
	ofLight	light;
	ofLight	light2;
	ofImage cuadrofondo;
	ofVboMesh mesh;
	ofEasyCam cam;
	int i = 0;
	// comunicaciones
	ofxOscReceiver reccerr;
	ofxOscSender sendcerr;
	ofxOscReceiver recsomb;
	ofxOscSender sendsomb;
	bool needsend = false;
	ofSoundPlayer mac;
	ofImage pista;

	bool show_help;
	ofImage _help[48];
	int help_index;
	int tiempo_help1;
	int tiempo_help2;

	bool pressed_1;
	bool pressed_2;
	bool pressed_;
	int pared;
	double tiempo_reset;
	bool show_time;
	bool play;
};

