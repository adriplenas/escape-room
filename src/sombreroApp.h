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

class sombreroApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		
	// variables de control
	bool play = false;
	int F = 0;
	
	bool show_picture = false;
	bool iniajedrez = false;
	float pruebas;  // contador de pruebas superadas

	// modelo 3d habitacion

	ofVec2f posvolver;
	ofImage volver;
	ofxAssimpModelLoader model;
	ofLight	light;
	ofLight	light2;
	ofImage cuadrofondo;
	ofVboMesh mesh;
	ofEasyCam cam;

	// comunicaciones
	ofxOscReceiver reccerr;
	ofxOscSender sendcerr;
	ofxOscReceiver rectaz;
	ofxOscSender sendtaz;
	bool needsend = false;

	// prueba cuadro
	ofImage marco;
	ofImage cuadro;
	ofImage teacups;

	ofImage hatter;
	ofImage queen;

	float ratio = 1.75;

	ofImage images[9];//contains de 8 pieces of the images and one extra for for resize
	int index[9];
	string image_name[9];
	ofVec4f xy[9]; // contains de coordenates of the pieces
				   //    X1 Y1 X2 Y2 = x y z w
				   //     X1          X2
				   //	Y1 --------------	
				   //     |				|
				   //     |				|
				   //     |				|
				   //     |				|
				   //  Y2 ---------------

	ofVec2f xygoodMH[8];  // definitive xy for every Mad Hatter image
	ofVec2f xygoodQH[8];  // definitive xy for every Queen of Hearts image
	ofVec4f xyrangeMH[8];	// xy releasing range for every Mad Hatter image
	ofVec4f xyrangeQH[8];	// xy releasing range for every Queen of Hearts image

	ofVec2f poscuadro;
	int sep = 5;
	int range = 10;
	int wmaxQH;		int hmaxQH;
	int wmaxMH;		int hmaxMH;

	int solution1[8];
	int solution2[8];
	int actual[8];

	float propHeight;
	float propWidth;

	bool picked = false;
	bool piece1 = false;
	bool piece2 = false;
	bool endstep1 = piece1 && piece2;

	// ajederz
	bool endstep2 = false;
	ofSoundPlayer bubble;

	//variables for step 2
	ofImage chess_board; ofVec4f xychess_board;
	ofImage chess_red_queen; ofVec4f xychess_queen;
	ofImage chess_red_bishop; ofVec4f xychess_bishop;
	ofImage chess_red_queen_real; ofVec4f xychess_queen_real;
	ofImage chess_red_bishop_real; ofVec4f xychess_bishop_real;
		
	ofVec2f xypoints[22];
	bool show_chess = false;
	bool queenToBoard = false;
	bool bishopToBoard = false;
	bool show_points = false;
	bool checkmate = false;
	int range2 = 10;

	ofSoundPlayer zelda_secret;
	ofSoundPlayer zelda_item;
	ofSoundPlayer mario_coin;
	ofSoundPlayer head_cut;
	bool show_paper = false;
	ofImage paper_big; ofVec2f xypaper_big;
	ofImage paper_small; ofVec2f xypaper_small;
	ofImage eatme; ofVec2f xyeatme;
	ofImage drinkme; ofVec2f xydrinkme;
	ofSoundPlayer pacman;
	ofSoundPlayer comerbeber;
	ofSoundPlayer embolo;

	ofImage clue;
	bool show_clue;
	bool show_help;
	ofImage _help[48];
	int help_index;
	
	int tiempo_help1;
	int tiempo_help2;

	double tiempo_reset;
	double tiempo_juego;
	double tiempo_start;
	bool pressed_1;
	bool pressed_2;
	bool pressed_;

	ofSerial serial;

	char pared;
	int min;
	int seg;
	ofImage _tiempos[10];
	ofImage _tiemposfin[10];
	ofImage _signo;
	ofImage _signoend;
	bool show_time;
	bool bool_aux = true;
	ofImage cartel;
	double tiempo_video_on;
	bool play_video;

};

