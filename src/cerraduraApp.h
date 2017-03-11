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
class cerraduraApp : public ofBaseApp {

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
	// modelo 3d habitacion
	ofxAssimpModelLoader model;
	ofxAssimpModelLoader puerta;
	ofLight	light;
	ofLight	light2;
	ofImage cuadrofondo;
	ofVboMesh mesh;
	ofVboMesh mpuerta;
	ofEasyCam cam;
	int i = 0;
	// comunicaciones
	ofxOscReceiver recsomb;
	ofxOscSender sendsomb;
	ofxOscReceiver rectaz;
	ofxOscSender sendtaz;
	bool needsend = false;
	ofImage puerta_amp;
	// prueba 
	ofImage _partitura;
	ofImage _partitura2;
	ofImage _notas;
	ofImage _notasV;
	ofImage _papel;
	ofImage _A;
	ofImage _B;
	ofImage _C;
	ofImage _D;
	ofImage _E;
	ofImage _drinkme;
	ofImage _eatme;

	int nota_bien = 0;
	int notas[12]={ 0,0,0,0,0,0,0,0,0,0,0,0 };
	double tiempo[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	double t = 0;
	const int queen[12] = { 2, 2, 5, 2, 2, 5, 2, 2, 5, 2, 2, 1 };
	int pos = 0;
	bool mostrarPartitura = false;

	ofSoundPlayer darriba;
	ofSoundPlayer dabajo;
	ofSoundPlayer iarriba;
	ofSoundPlayer iabajo;
	ofSoundPlayer bombo;
	ofSoundPlayer platillo;
	ofSoundPlayer plato1;
	ofSoundPlayer plato2;
	ofSoundPlayer _queen;
	ofSoundPlayer _champions;

	ofSoundPlayer pacman;
	ofSoundPlayer embolo;
	ofSoundPlayer bubble;

	bool show_help;
	ofImage _help[48];
	int help_index;
	int tiempo_help1;
	int tiempo_help2;

	bool pressed_1;
	bool pressed_2;
	bool pressed_;

	double tiempo_reset;
	int pared;
	bool show_time;
	bool play;
	bool puerta_mov;
};

