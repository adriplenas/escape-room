#include "sombreroApp.h"
/*
* AUTHORS:
* Adrián López, Raúl Martín, Berta Martínez, Marta Navarro
*/
//--------------------------------------------------------------
void sombreroApp::setup() {
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	serial.setup("COM5", 9600);

	//system("monitorsombrero.bat");
	//serial.writeByte('S');
	show_help = false;
	for (int f = 0; f < 48; f++) {
		string aux = "images/ayuda/ayuda_" + ofToString(f) + ".png";
		_help[f].load(aux);
	}
	help_index = 0;
	tiempo_help1 = 0;
	tiempo_help2 = 0;
	//Pista inicial
	clue.load("images/pista.png");
	clue.resize(clue.getWidth() / 1.5, clue.getHeight() / 1.5);
	show_clue = true;
	//Modelos 3D
	if (true) {
		ofBackground(0, 0, 0);
		ofSetVerticalSync(true);
		ofEnableDepthTest();

		model.loadModel("habitacionsombreros.obj");
		model.setPosition(ofGetWidth() / 2 + 45, ofGetHeight() * 2.6 - 400, 0);
		model.enableColors();
		model.enableMaterials();
		model.enableTextures();
		mesh.load("habitacionsombreros.ply");
		model.setRotation(0, 90, 1, 0, 0);
		model.setRotation(0, 180, 0, 1, 0);
		model.setScale(8.8, 8.8, 8.8);

		// imagen del cuadro de fondo (no prueba)
		cuadrofondo.load("images/alice.png");
		cuadrofondo.resize(cuadrofondo.getWidth() / ratio, cuadrofondo.getHeight() / ratio);
		cuadrofondo.resize(cuadrofondo.getWidth() / 2.1, cuadrofondo.getHeight() / 2.1);
		volver.load("images/retroceso.png");
		volver.resize(ofGetWidth() / 9, ofGetHeight() / 9);
		posvolver.set(ofGetWidth() - 180, ofGetHeight() / 2 + 150);
		//light.enable();
		light.setPosition(0, 0, 1500);
		light2.setPosition(0, 0, 500);
	}
	//Comunicaciones
	sendtaz.setup(HOST, PORTST);
	rectaz.setup(PORTTS);
	sendcerr.setup(HOST, PORTSC);
	reccerr.setup(PORTCS);



	//Prueba cuadro
	if (true) {
		marco.load("images/marco.png");
		cuadro.load("images/alice.png");
		cuadro.resize(cuadro.getWidth() / ratio, cuadro.getHeight() / ratio);

		hatter.load("images/mad hatter.png");
		hatter.resize(hatter.getWidth() / ratio, hatter.getHeight() / ratio);
		// crop the Mad hatter's picture into 4 pieces
		images[1].cropFrom(hatter, 0, 0, hatter.getWidth() / 2, hatter.getHeight() / 2);
		images[2].cropFrom(hatter, hatter.getWidth() / 2, 0, hatter.getWidth() / 2, hatter.getHeight() / 2);
		images[3].cropFrom(hatter, 0, hatter.getHeight() / 2, hatter.getWidth() / 2, hatter.getHeight() / 2);
		images[4].cropFrom(hatter, hatter.getWidth() / 2, hatter.getHeight() / 2, hatter.getWidth() / 2, hatter.getHeight() / 2);

		queen.load("images/queen of hearts.png");
		queen.resize(queen.getWidth() / ratio, queen.getHeight() / ratio);
		// crop the Queen of Hearts' picture into 4 pieces
		images[5].cropFrom(queen, 0, 0, queen.getWidth() / 2, queen.getHeight() / 2);
		images[6].cropFrom(queen, queen.getWidth() / 2, 0, queen.getWidth() / 2, queen.getHeight() / 2);
		images[7].cropFrom(queen, 0, queen.getHeight() / 2, queen.getWidth() / 2, queen.getHeight() / 2);
		images[8].cropFrom(queen, queen.getWidth() / 2, queen.getHeight() / 2, queen.getWidth() / 2, queen.getHeight() / 2);

		image_name[0] = "aux";
		image_name[1] = "hatterUL";	image_name[2] = "hatterUR";
		image_name[3] = "hatterDL";	image_name[4] = "hatterDR";
		image_name[5] = "queenUL";	image_name[6] = "queenUR";
		image_name[7] = "queenDL";	image_name[8] = "queenDR";

		wmaxQH = max(max(images[5].getWidth(), images[6].getWidth()), max(images[7].getWidth(), images[8].getWidth()));
		hmaxQH = max(max(images[5].getHeight(), images[6].getHeight()), max(images[7].getHeight(), images[8].getHeight()));
		wmaxMH = max(max(images[1].getWidth(), images[2].getWidth()), max(images[3].getWidth(), images[4].getWidth()));
		hmaxMH = max(max(images[1].getHeight(), images[2].getHeight()), max(images[3].getHeight(), images[4].getHeight()));

		poscuadro.set(ofGetWidth() / 2 - cuadro.getWidth() / 2, ofGetHeight() / 2 - cuadro.getHeight() / 2 - 50);

		for (int i = 0; i <= 7; i++) {
			solution1[i] = i + 1;
			solution2[i] = ofWrap(i + 4, 0, 8) + 1;
			actual[i] = 0;
		}
		for (int i = 0; i <= 8; i++) {
			index[i] = i;
		}
		/*
		1- hatter UL	2- hatter UR
		3- hatter DL	4- hatter DR
		5- queen UL		6- queen UR
		7- queen DL		8- queen DR
		*/
		// coordenates and ranges for the Mad Hatter images

		xygoodMH[2].set(poscuadro.x + cuadro.getWidth()*0.412109, poscuadro.y + cuadro.getHeight()*0.387370);  // xy hatter DL
		xygoodMH[3].set(xygoodMH[2].x + images[2].getWidth(), xygoodMH[2].y);  // xy hatter DR
		xygoodMH[0].set(xygoodMH[2].x, xygoodMH[2].y - images[2].getHeight());  // xy hatter UL
		xygoodMH[1].set(xygoodMH[2].x + images[2].getWidth(), xygoodMH[2].y - images[2].getHeight());  // xy hatter UR

		xygoodMH[6].set(poscuadro.x + cuadro.getWidth()*0.055664, poscuadro.y + cuadro.getHeight()*0.443359);  // xy queen DL
		xygoodMH[7].set(xygoodMH[6].x + images[2].getWidth(), xygoodMH[6].y);  // xy queen DR 
		xygoodMH[4].set(xygoodMH[6].x, xygoodMH[6].y - images[2].getHeight());  // xy queen UL
		xygoodMH[5].set(xygoodMH[6].x + images[2].getWidth(), xygoodMH[6].y - images[2].getHeight());  // xy queen UR

		xyrangeMH[0].set(xygoodMH[0].x + images[1].getWidth() / 2 - range, xygoodMH[0].y + images[1].getHeight() / 2 - range, xygoodMH[0].x + images[1].getWidth() / 2 + range, xygoodMH[0].y + images[1].getHeight() / 2 + range);
		xyrangeMH[1].set(xygoodMH[1].x + images[1].getWidth() / 2 - range, xygoodMH[1].y + images[1].getHeight() / 2 - range, xygoodMH[1].x + images[1].getWidth() / 2 + range, xygoodMH[1].y + images[1].getHeight() / 2 + range);
		xyrangeMH[2].set(xygoodMH[2].x + images[1].getWidth() / 2 - range, xygoodMH[2].y + images[1].getHeight() / 2 - range, xygoodMH[2].x + images[1].getWidth() / 2 + range, xygoodMH[2].y + images[1].getHeight() / 2 + range);
		xyrangeMH[3].set(xygoodMH[3].x + images[1].getWidth() / 2 - range, xygoodMH[3].y + images[1].getHeight() / 2 - range, xygoodMH[3].x + images[1].getWidth() / 2 + range, xygoodMH[3].y + images[1].getHeight() / 2 + range);
		xyrangeMH[4].set(xygoodMH[4].x + images[1].getWidth() / 2 - range, xygoodMH[4].y + images[1].getHeight() / 2 - range, xygoodMH[4].x + images[1].getWidth() / 2 + range, xygoodMH[4].y + images[1].getHeight() / 2 + range);
		xyrangeMH[5].set(xygoodMH[5].x + images[1].getWidth() / 2 - range, xygoodMH[5].y + images[1].getHeight() / 2 - range, xygoodMH[5].x + images[1].getWidth() / 2 + range, xygoodMH[5].y + images[1].getHeight() / 2 + range);
		xyrangeMH[6].set(xygoodMH[6].x + images[1].getWidth() / 2 - range, xygoodMH[6].y + images[1].getHeight() / 2 - range, xygoodMH[6].x + images[1].getWidth() / 2 + range, xygoodMH[6].y + images[1].getHeight() / 2 + range);
		xyrangeMH[7].set(xygoodMH[7].x + images[1].getWidth() / 2 - range, xygoodMH[7].y + images[1].getHeight() / 2 - range, xygoodMH[7].x + images[1].getWidth() / 2 + range, xygoodMH[7].y + images[1].getHeight() / 2 + range);

		// coordenates and ranges for the Queen of Hearts images

		xygoodQH[2].set(poscuadro.x + cuadro.getWidth()*0.414551, poscuadro.y + cuadro.getHeight()*0.375000);  // xy hatter DL
		xygoodQH[3].set(xygoodQH[2].x + images[7].getWidth(), xygoodQH[2].y);  // xy hatter DR
		xygoodQH[0].set(xygoodQH[2].x, xygoodQH[2].y - images[7].getHeight());  // xy hatter UL
		xygoodQH[1].set(xygoodQH[2].x + images[7].getWidth(), xygoodQH[2].y - images[7].getHeight());  // xy hatter UR

		xygoodQH[6].set(poscuadro.x + cuadro.getWidth()*0.060059, poscuadro.y + cuadro.getHeight()*0.437500);  // xy queen DL
		xygoodQH[7].set(xygoodQH[6].x + images[7].getWidth(), xygoodQH[6].y);  // xy queen DR 
		xygoodQH[4].set(xygoodQH[6].x, xygoodQH[6].y - images[7].getHeight());  // xy queen UL
		xygoodQH[5].set(xygoodQH[6].x + images[7].getWidth(), xygoodQH[6].y - images[7].getHeight());  // xy queen UR

		xyrangeQH[0].set(xygoodQH[0].x + images[5].getWidth() / 2 - range, xygoodQH[0].y + images[5].getHeight() / 2 - range, xygoodQH[0].x + images[5].getWidth() / 2 + range, xygoodQH[0].y + images[5].getHeight() / 2 + range);
		xyrangeQH[1].set(xygoodQH[1].x + images[5].getWidth() / 2 - range, xygoodQH[1].y + images[5].getHeight() / 2 - range, xygoodQH[1].x + images[5].getWidth() / 2 + range, xygoodQH[1].y + images[5].getHeight() / 2 + range);
		xyrangeQH[2].set(xygoodQH[2].x + images[5].getWidth() / 2 - range, xygoodQH[2].y + images[5].getHeight() / 2 - range, xygoodQH[2].x + images[5].getWidth() / 2 + range, xygoodQH[2].y + images[5].getHeight() / 2 + range);
		xyrangeQH[3].set(xygoodQH[3].x + images[5].getWidth() / 2 - range, xygoodQH[3].y + images[5].getHeight() / 2 - range, xygoodQH[3].x + images[5].getWidth() / 2 + range, xygoodQH[3].y + images[5].getHeight() / 2 + range);
		xyrangeQH[4].set(xygoodQH[4].x + images[5].getWidth() / 2 - range, xygoodQH[4].y + images[5].getHeight() / 2 - range, xygoodQH[4].x + images[5].getWidth() / 2 + range, xygoodQH[4].y + images[5].getHeight() / 2 + range);
		xyrangeQH[5].set(xygoodQH[5].x + images[5].getWidth() / 2 - range, xygoodQH[5].y + images[5].getHeight() / 2 - range, xygoodQH[5].x + images[5].getWidth() / 2 + range, xygoodQH[5].y + images[5].getHeight() / 2 + range);
		xyrangeQH[6].set(xygoodQH[6].x + images[5].getWidth() / 2 - range, xygoodQH[6].y + images[5].getHeight() / 2 - range, xygoodQH[6].x + images[5].getWidth() / 2 + range, xygoodQH[6].y + images[5].getHeight() / 2 + range);
		xyrangeQH[7].set(xygoodQH[7].x + images[5].getWidth() / 2 - range, xygoodQH[7].y + images[5].getHeight() / 2 - range, xygoodQH[7].x + images[5].getWidth() / 2 + range, xygoodQH[7].y + images[5].getHeight() / 2 + range);

		//initial positions
		xy[1].set(poscuadro.x - sep - wmaxMH, poscuadro.y, poscuadro.x - sep - wmaxMH + images[1].getWidth(), poscuadro.y + images[1].getHeight());
		xy[2].set(xy[1].x, xy[1].w + sep, xy[1].x + images[2].getWidth(), xy[1].w + sep + images[2].getHeight());
		xy[3].set(xy[1].x, xy[2].w + sep, xy[1].x + images[3].getWidth(), xy[2].w + sep + images[3].getHeight());
		xy[4].set(xy[1].x, xy[3].w + sep, xy[1].x + images[4].getWidth(), xy[3].w + sep + images[4].getHeight());

		xy[5].set(poscuadro.x + cuadro.getWidth() + sep, poscuadro.y, poscuadro.x + cuadro.getWidth() + sep + images[5].getWidth(), poscuadro.y + images[5].getHeight());
		xy[6].set(xy[5].x, xy[5].w + sep, xy[5].x + images[6].getWidth(), xy[5].w + sep + images[6].getHeight());
		xy[7].set(xy[5].x, xy[6].w + sep, xy[5].x + images[7].getWidth(), xy[6].w + sep + images[7].getHeight());
		xy[8].set(xy[5].x, xy[7].w + sep, xy[5].x + images[8].getWidth(), xy[7].w + sep + images[8].getHeight());

		bubble.load("sounds/bubble.mp3");
		bubble.setVolume(0.3);
	}
	//Prueba ajedrez
	if (true) {
		//setup for step 2
		zelda_secret.load("sounds/zelda_secret.mp3"); zelda_secret.setVolume(0.5);
		mario_coin.load("sounds/mario_coin.mp3"); mario_coin.setVolume(0.5);
		zelda_item.load("sounds/zelda_item.mp3"); zelda_item.setVolume(0.5);
		head_cut.load("sounds/cabeza.wav"); head_cut.setVolume(0.5);

		chess_board.load("images/chess_board.jpg");
		chess_red_queen_real.load("images/chess_red_queen_real.png");
		chess_red_bishop_real.load("images/chess_red_bishop_real.png");
		chess_red_bishop.load("images/chess_red_bishop.png");
		chess_red_queen.load("images/chess_red_queen.png");

		chess_red_queen_real.resize(chess_red_queen_real.getWidth() / (ratio * 2), chess_red_queen_real.getHeight() / (ratio * 2));
		chess_red_bishop_real.resize(chess_red_bishop_real.getWidth() / (ratio*1.7), chess_red_bishop_real.getHeight() / (ratio*1.7));

		xychess_queen_real.x = poscuadro.x + cuadro.getWidth() + sep;
		xychess_queen_real.y = poscuadro.y + cuadro.getHeight() / 2;
		xychess_queen_real.z = xychess_queen_real.x + chess_red_queen_real.getWidth();
		xychess_queen_real.w = xychess_queen_real.y + chess_red_queen_real.getHeight();

		xychess_bishop_real.x = poscuadro.x - sep - chess_red_bishop_real.getWidth();
		xychess_bishop_real.y = poscuadro.y + cuadro.getHeight() / 2;
		xychess_bishop_real.z = xychess_bishop_real.x + chess_red_bishop_real.getWidth();
		xychess_bishop_real.w = xychess_bishop_real.y + chess_red_bishop_real.getHeight();

		//initial positions of chess board
		xychess_board.x = (ofGetWidth() - chess_board.getWidth()) / 2;
		xychess_board.y = (ofGetHeight() - chess_board.getHeight()) / 2;
		xychess_board.z = xychess_board.x + chess_board.getWidth();
		xychess_board.w = xychess_board.y + chess_board.getHeight();

		xychess_bishop.x = xychess_board.x + 13 * chess_board.getWidth() / 16 - chess_red_bishop.getWidth() / 2;
		xychess_bishop.y = xychess_board.y + 11 * chess_board.getHeight() / 16 - chess_red_bishop.getHeight() / 2;
		xychess_bishop.z = xychess_bishop.x + chess_red_bishop.getWidth();
		xychess_bishop.w = xychess_bishop.y + chess_red_bishop.getHeight();

		xychess_queen.x = xychess_board.x + 1 * chess_board.getWidth() / 16 - chess_red_queen.getWidth() / 2;
		xychess_queen.y = xychess_board.y + 9 * chess_board.getHeight() / 16 - chess_red_queen.getHeight() / 2;
		xychess_queen.z = xychess_queen.x + chess_red_queen.getWidth();
		xychess_queen.w = xychess_queen.y + chess_red_queen.getHeight();

		range2 = chess_board.getWidth() / (16 * 2);

		//positions of the moves of the queen
		//actual
		xypoints[0].set(xychess_board.x + 1 * chess_board.getWidth() / 16, xychess_board.y + 9 * chess_board.getHeight() / 16);
		//vertical moves
		xypoints[1].set(xychess_board.x + 1 * chess_board.getWidth() / 16, xychess_board.y + 1 * chess_board.getHeight() / 16);
		xypoints[2].set(xychess_board.x + 1 * chess_board.getWidth() / 16, xychess_board.y + 3 * chess_board.getHeight() / 16);
		xypoints[3].set(xychess_board.x + 1 * chess_board.getWidth() / 16, xychess_board.y + 5 * chess_board.getHeight() / 16);
		xypoints[4].set(xychess_board.x + 1 * chess_board.getWidth() / 16, xychess_board.y + 7 * chess_board.getHeight() / 16);
		xypoints[5].set(xychess_board.x + 1 * chess_board.getWidth() / 16, xychess_board.y + 11 * chess_board.getHeight() / 16);
		xypoints[6].set(xychess_board.x + 1 * chess_board.getWidth() / 16, xychess_board.y + 13 * chess_board.getHeight() / 16);
		xypoints[7].set(xychess_board.x + 1 * chess_board.getWidth() / 16, xychess_board.y + 15 * chess_board.getHeight() / 16);
		//horizontal moves
		xypoints[8].set(xychess_board.x + 3 * chess_board.getWidth() / 16, xychess_board.y + 9 * chess_board.getHeight() / 16);
		xypoints[9].set(xychess_board.x + 5 * chess_board.getWidth() / 16, xychess_board.y + 9 * chess_board.getHeight() / 16);
		xypoints[10].set(xychess_board.x + 7 * chess_board.getWidth() / 16, xychess_board.y + 9 * chess_board.getHeight() / 16);
		xypoints[11].set(xychess_board.x + 9 * chess_board.getWidth() / 16, xychess_board.y + 9 * chess_board.getHeight() / 16);
		xypoints[12].set(xychess_board.x + 11 * chess_board.getWidth() / 16, xychess_board.y + 9 * chess_board.getHeight() / 16);
		xypoints[13].set(xychess_board.x + 13 * chess_board.getWidth() / 16, xychess_board.y + 9 * chess_board.getHeight() / 16);
		xypoints[14].set(xychess_board.x + 15 * chess_board.getWidth() / 16, xychess_board.y + 9 * chess_board.getHeight() / 16);
		//vertical moves
		xypoints[15].set(xychess_board.x + 3 * chess_board.getWidth() / 16, xychess_board.y + 7 * chess_board.getHeight() / 16);
		xypoints[16].set(xychess_board.x + 5 * chess_board.getWidth() / 16, xychess_board.y + 5 * chess_board.getHeight() / 16);//correct
		xypoints[17].set(xychess_board.x + 7 * chess_board.getWidth() / 16, xychess_board.y + 3 * chess_board.getHeight() / 16);
		xypoints[18].set(xychess_board.x + 9 * chess_board.getWidth() / 16, xychess_board.y + 1 * chess_board.getHeight() / 16);
		xypoints[19].set(xychess_board.x + 3 * chess_board.getWidth() / 16, xychess_board.y + 11 * chess_board.getHeight() / 16);
		xypoints[20].set(xychess_board.x + 5 * chess_board.getWidth() / 16, xychess_board.y + 13 * chess_board.getHeight() / 16);
		xypoints[21].set(xychess_board.x + 7 * chess_board.getWidth() / 16, xychess_board.y + 15 * chess_board.getHeight() / 16);
	}

	//Pista papel
	if (true) {
		paper_big.load("images/papiro.png");
		paper_big.resize(paper_big.getWidth() / ratio, paper_big.getHeight() / ratio);
		paper_small.load("images/pergamino.png");
		paper_small.resize(paper_small.getWidth() / (2 * ratio), paper_small.getHeight() / (2 * ratio));
		xypaper_big.set((ofGetWidth() - paper_big.getWidth()) / 2, (ofGetHeight() - paper_big.getHeight()) / 2);
		xypaper_small.set((ofGetWidth() * 4 / 5), ofGetHeight() / 2 - paper_small.getHeight());
	}
	//Prueba comer-beber
	if (true) {
		drinkme.load("images/drinkme.jpg"); drinkme.resize(drinkme.getWidth() / 2, drinkme.getHeight() / 2);
		eatme.load("images/eatme.jpg"); eatme.resize(eatme.getWidth() / 2, eatme.getHeight() / 2);
		pacman.load("sounds/pacman_death.mp3");
		comerbeber.load("sounds/comerbeber.mp3");
		xyeatme.set(754, 352);
		xydrinkme.set(70, 396);
		embolo.load("sounds/embolo.wav"); embolo.setVolume(0.7);
	}
	show_help = true;
	pressed_ = false;
	pressed_1 = false;
	pressed_2 = false;

	tiempo_reset = 0;
	pared = 's';
	pruebas = 0.0;

	tiempo_start = 0;
	tiempo_juego = 0;
	seg = 0;
	min = 0;
	int part = 8;
	_signo.load("images/numsigno.png");
	_signoend.load("images/numsigno.png");
	_signoend.resize(_signoend.getWidth() / 2, _signoend.getHeight() / 2);
	_signo.resize(_signo.getWidth() / part, _signo.getHeight() / part);
	for (int f = 0; f < 10; f++) {
		string aux = "images/num" + ofToString(f) + ".png";
		_tiempos[f].load(aux);
	}
	show_time = false;
	for (int k = 0; k < 10; k++) {
		_tiemposfin[k] = _tiempos[k];
		_tiemposfin[k].resize(_tiemposfin[k].getWidth() / 2, _tiemposfin[k].getHeight() / 2);
		_tiempos[k].resize(_tiempos[k].getWidth() / part, _tiempos[k].getHeight() / part);
	}
	cartel.load("images/poster.png");
	cartel.resize(cartel.getWidth() / 2.5, cartel.getHeight() / 2.5);
	play = false;
	play_video = false;
	tiempo_video_on = ofGetSystemTime();
	//system("wmp.lnk");
}

//--------------------------------------------------------------
void sombreroApp::update() {
	//comunicaciones pruebas
	if (true) {
		while (reccerr.hasWaitingMessages()) {
			ofxOscMessage m;
			reccerr.getNextMessage(&m);
			if (m.getAddress() == "/cerradura/x") {
				pruebas = m.getArgAsFloat(0);
				pared = m.getArgAsChar(1);
				show_time = m.getArgAsBool(2);
				if (pruebas == 4.5 && bool_aux == true) {
					comerbeber.play();
					bool_aux = false;
				}
				if (pruebas == 6.5) {
					//end_music.play();
				}
				if (pared == 's') {
					system("monitorsombrero.bat");
				}
				serial.writeByte(pared);
			}
		}
		while (rectaz.hasWaitingMessages()) {
			ofxOscMessage m;
			rectaz.getNextMessage(&m);
			if (m.getAddress() == "/tazas/x") {
				pruebas = m.getArgAsFloat(0);
				pared = m.getArgAsChar(1);
				show_time = m.getArgAsBool(2);
				if (pruebas == 4.5) {
					comerbeber.play();
				}
				if (pared == 's') {
					system("monitorsombrero.bat");
				}
				serial.writeByte(pared);
			}
		}
		if (needsend == true) {
			ofxOscMessage * m;
			m = new ofxOscMessage();
			m->setAddress("/sombrero/x");
			m->addFloatArg(pruebas);
			m->addCharArg(pared);
			m->addBoolArg(show_time);
			m->addBoolArg(play);
			sendtaz.sendMessage(*m);
			sendcerr.sendMessage(*m);
			delete m;    // this calls the destructor where the messages are cleared.   
			needsend = false;
			serial.writeByte(pared);
		}
	}
	// prueba cuadro
	for (int i = 1; i <= 8; i++) {
		xy[i].z = xy[i].x + images[i].getWidth();
		xy[i].w = xy[i].y + images[i].getHeight();
	}
	xychess_queen.z = xychess_queen.x + chess_red_queen.getWidth();
	xychess_queen.w = xychess_queen.y + chess_red_queen.getHeight();

	ofSoundUpdate();

	if (endstep1 == false && pruebas == 0.0) {
		endstep1 = piece1 && piece2 && bishopToBoard == true && queenToBoard == true;
		if (endstep1 == true) {
			pruebas = 1.0;
		}
	}
	//Restart
	if (pruebas == 11.0) {
		system("off_music.bat");
		F = 0;
		show_picture = false;
		iniajedrez = false;
		images[1].cropFrom(hatter, 0, 0, hatter.getWidth() / 2, hatter.getHeight() / 2);
		images[2].cropFrom(hatter, hatter.getWidth() / 2, 0, hatter.getWidth() / 2, hatter.getHeight() / 2);
		images[3].cropFrom(hatter, 0, hatter.getHeight() / 2, hatter.getWidth() / 2, hatter.getHeight() / 2);
		images[4].cropFrom(hatter, hatter.getWidth() / 2, hatter.getHeight() / 2, hatter.getWidth() / 2, hatter.getHeight() / 2);
		images[5].cropFrom(queen, 0, 0, queen.getWidth() / 2, queen.getHeight() / 2);
		images[6].cropFrom(queen, queen.getWidth() / 2, 0, queen.getWidth() / 2, queen.getHeight() / 2);
		images[7].cropFrom(queen, 0, queen.getHeight() / 2, queen.getWidth() / 2, queen.getHeight() / 2);
		images[8].cropFrom(queen, queen.getWidth() / 2, queen.getHeight() / 2, queen.getWidth() / 2, queen.getHeight() / 2);
		
		image_name[0] = "aux";
		image_name[1] = "hatterUL";	image_name[2] = "hatterUR";
		image_name[3] = "hatterDL";	image_name[4] = "hatterDR";
		image_name[5] = "queenUL";	image_name[6] = "queenUR";
		image_name[7] = "queenDL";	image_name[8] = "queenDR";
		for (int i = 0; i <= 8; i++) {
			index[i] = i;
		}
		light.setPosition(0, 0, 1500);
		light2.setPosition(0, 0, 500);

		//initial positions
		xy[1].set(poscuadro.x - sep - wmaxMH, poscuadro.y, poscuadro.x - sep - wmaxMH + images[1].getWidth(), poscuadro.y + images[1].getHeight());
		xy[2].set(xy[1].x, xy[1].w + sep, xy[1].x + images[2].getWidth(), xy[1].w + sep + images[2].getHeight());
		xy[3].set(xy[1].x, xy[2].w + sep, xy[1].x + images[3].getWidth(), xy[2].w + sep + images[3].getHeight());
		xy[4].set(xy[1].x, xy[3].w + sep, xy[1].x + images[4].getWidth(), xy[3].w + sep + images[4].getHeight());

		xy[5].set(poscuadro.x + cuadro.getWidth() + sep, poscuadro.y, poscuadro.x + cuadro.getWidth() + sep + images[5].getWidth(), poscuadro.y + images[5].getHeight());
		xy[6].set(xy[5].x, xy[5].w + sep, xy[5].x + images[6].getWidth(), xy[5].w + sep + images[6].getHeight());
		xy[7].set(xy[5].x, xy[6].w + sep, xy[5].x + images[7].getWidth(), xy[6].w + sep + images[7].getHeight());
		xy[8].set(xy[5].x, xy[7].w + sep, xy[5].x + images[8].getWidth(), xy[7].w + sep + images[8].getHeight());
		
		xychess_queen.x = xychess_board.x + 1 * chess_board.getWidth() / 16 - chess_red_queen.getWidth() / 2;
		xychess_queen.y = xychess_board.y + 9 * chess_board.getHeight() / 16 - chess_red_queen.getHeight() / 2;
		xychess_queen.z = xychess_queen.x + chess_red_queen.getWidth();
		xychess_queen.w = xychess_queen.y + chess_red_queen.getHeight();

		for (int i = 0; i <= 7; i++) {
			solution1[i] = i + 1;
			solution2[i] = ofWrap(i + 4, 0, 8) + 1;
			actual[i] = 0;
		}
		endstep1 = false;
		endstep2 = false;
		picked = false;
		piece1 = false;
		piece2 = false;
		show_chess = false;
		queenToBoard = false;
		bishopToBoard = false;
		show_points = false;
		checkmate = false;
		show_paper = false;
		show_clue = true;
		piece1 = false;
		piece2 = false;

		pressed_ = false;
		pressed_1 = false;
		pressed_2 = false;
		tiempo_reset = 0;
		play = false;
		play_video = false;
		pruebas = 0.0;
		pared = 's';
		show_help = true;
		tiempo_start = 0;
		tiempo_juego = 0;
		seg = 0;
		min = 0;
		show_time = false;
		bool_aux = true;
		system("monitorsombrero.bat");
		serial.writeByte('S');
		play_video = false;
		tiempo_video_on = ofGetSystemTime();
		needsend = true;
		system("player.bat");
	}
	if (play_video == true && play == false && ofInRange(ofGetSystemTime() - tiempo_video_on,11000,12000)) {
		system("off_video.bat");
		play_video = false;
		play = true;
		show_time = false;
		tiempo_video_on = ofGetSystemTime();
		needsend = true;
	}
	if (play_video == false && play == true && ofInRange(ofGetSystemTime() - tiempo_video_on, 2000, 3000)) {
		play_video = true;
		system("on_music.bat");
	}

	if (pressed_2 == true && pressed_1 == true && pressed_ == true) {
		pruebas = 10.0;
		needsend = true;
	}
	if ((ofGetSystemTime() - tiempo_reset) < 6000) {
		if (pressed_2 == true && pressed_1 == true && pressed_ == true) {
			pressed_ = false;
			pressed_1 = false;
			pressed_2 = false;
			tiempo_reset = 0;
			pruebas = 10.0;
			needsend = true;
		}
	}
	else {
		pressed_ = false;
		pressed_1 = false;
		pressed_2 = false; 
		tiempo_reset = 0;
	}
	if(pruebas < 6.0){
		tiempo_juego = ofGetSystemTime() - tiempo_start;
	}
	if (pruebas == 6.0){
		show_time = false;
	}
	min = (tiempo_juego / 1000) / 60;
	seg = (tiempo_juego / 1000)-min*60;
	tiempo_help2 = (ofGetSystemTime()-tiempo_help1) / 1500;
	help_index = tiempo_help2- 48 * (tiempo_help2 / 48);
}

//--------------------------------------------------------------
void sombreroApp::draw() {
	light.enable();
	// draws the ply file loaded into the mesh is you pressed 6
	cam.begin();
	mesh.draw(); 
	cam.end();
	// draws all the other file types which are loaded into model.
	model.setPosition(ofGetWidth() / 2, ofGetHeight() * 2.6 - 400, 0);
	
	ofSetColor(255, 255, 255);
	if (play == true) {
			if (pruebas < 5.5) {
				model.setScale(8.8, 8.8, 8.8);
				model.drawFaces();
				cuadrofondo.draw(ofGetWidth() / 2 - 110, ofGetHeight() / 2 - 140, -20);
			}
			if (pruebas == 5.5) {
				if (F < 196) {
					model.setScale(8.8 + 50 * (F / 5) / 39, 8.8 + 50 * (F / 5) / 39, 8.8);
					model.drawFaces();
					F++;
				}
				if (F == 196) {
					model.setScale(8.8 + 50, 50 + 8.8, 8.8);
					model.drawFaces();
				}
			}
			if (5.5 < pruebas && pruebas < 10.0) {
				F = 196;
				model.setScale(8.8 + 50, 50 + 8.8, 8.8);
				model.drawFaces();
			}
			if (pruebas < 2.0 && (show_chess == true || show_picture == true)) {
				volver.draw(posvolver.x, posvolver.y, -21);
			}
			light.disable();
			light2.enable();
			// prueba cuadro
			if (pruebas < 2.0) {
				ofSetColor(255, 255, 255);
				if (show_picture == true) {
					cuadro.draw(poscuadro.x, poscuadro.y, -10);
					marco.resize(cuadro.getWidth() + 95, cuadro.getHeight() + 80);
					marco.draw(poscuadro.x - 45, poscuadro.y - 45, -10);
					int i = 1;
					for (i = 8; i >= 1; i--) {
						if (i == 1) {
							images[i].draw(xy[i].x, xy[i].y, -7.9);
						}
						else {
							images[i].draw(xy[i].x, xy[i].y, -8);
						}
					}
					//draw step 2
					if (piece1 == true && queenToBoard == false) {
						chess_red_queen_real.draw(xychess_queen_real.x, xychess_queen_real.y, -1);
					}
					if (piece2 == true && bishopToBoard == false) {
						chess_red_bishop_real.draw(xychess_bishop_real.x, xychess_bishop_real.y, -1);
					}
				}
				// ajedrez
				if (show_chess == true) {
					chess_board.draw(xychess_board.x, xychess_board.y, -1);
					if (queenToBoard == true) {
						chess_red_queen.draw(xychess_queen.x, xychess_queen.y, 0);
					}
					if (bishopToBoard == true) {
						chess_red_bishop.draw(xychess_bishop.x, xychess_bishop.y, 0);
					}
					if (show_points == true) {
						int j = 0;
						ofSetColor(255, 255, 0);
						for (j = 0; j < 22; j++) {
							ofDrawCircle(xypoints[j].x, xypoints[j].y, 1, 5);
						}
					}
				}
				// prueba ajedrez

				//
			}
			if (pruebas == 2.0 || pruebas == 2.5) {
				ofSetColor(255, 255, 255);
				if (show_paper == false) {
					paper_small.draw(xypaper_small.x, xypaper_small.y, 0);
				}
				else {
					if (show_clue == true && ofInRange(pruebas, 1.9, 2.6)) {
						ofSetColor(255, 255, 255);
						paper_big.draw(xypaper_big.x, xypaper_big.y, 0);
					}
				}
			}
			if (pruebas == 4.5) {
				drinkme.draw(xydrinkme.x, xydrinkme.y);
				eatme.draw(xyeatme.x, xyeatme.y);
			}

			ofHideCursor();
			if (show_clue == true && pruebas < 2.0 && show_picture == false && show_chess == false) {
				ofSetColor(255, 255, 255);
				clue.draw((ofGetWidth() - clue.getWidth()) / 2, (ofGetHeight() - clue.getHeight()) / 2, 0);
			}
			if (pared == 's') {
				if (picked == true) {
					ofSetColor(0, 255, 255);
					ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 1, 5);
				}
				else {
					ofSetColor(255, 0, 255);
					ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 1, 5);
				}
			}
			light.disable();
	}
	else {
		ofSetColor(0, 0, 0);
		ofDrawRectangle(0, 0, 1, ofGetWidth(), ofGetHeight());
	}
	//dibujar el tiempo
	if (show_help == true) {
		ofSetColor(255, 255, 255);
		_help[help_index].draw(0, 0, 8);
	}

	if (show_time == true) {	
		ofSetColor(255, 255, 255);
		int h_aux = 30;
		int w_aux = _tiempos[0].getWidth();
		if (min / 100 > 0 && tiempo_start != 0) {
			_tiempos[min / 100].draw(ofGetWidth() - 20 - 5 * w_aux - _signo.getWidth(), h_aux);
		}
		if (tiempo_start != 0) {
			_tiempos[min / 10].draw(ofGetWidth() - 20 - 4 * w_aux - _signo.getWidth(), h_aux);
			_tiempos[min - 10 * (min / 10)].draw(ofGetWidth() - 20 - 3 * w_aux - _signo.getWidth(), h_aux);
			_signo.draw(ofGetWidth() - 20 - 2 * w_aux - _signo.getWidth(), h_aux);
			_tiempos[seg / 10].draw(ofGetWidth() - 20 - 2 * w_aux, h_aux);
			_tiempos[seg - 10 * (seg / 10)].draw(ofGetWidth() - 20 - w_aux, h_aux);
		}
	}

	if (pruebas >= 6.0) {
		ofSetColor(0, 0, 0);
		ofDrawRectangle(0, 0, 7, ofGetWidth(), ofGetHeight());
		ofSetColor(255, 255, 255);
		int h_aux = (ofGetHeight() / 2) + 50;
		int w_aux = _tiemposfin[0].getWidth();
		int x_sig = (ofGetWidth() - _signoend.getWidth()) / 2;
		cartel.draw((ofGetWidth() - cartel.getWidth()) / 2, 5, 7.1);
		_tiemposfin[min / 10].draw(x_sig - 2 * w_aux, h_aux, 7.2);
		_tiemposfin[min - 10 * (min / 10)].draw(x_sig - w_aux, h_aux, 7.2);
		_signoend.draw(x_sig, h_aux, 7.2);
		_tiemposfin[seg / 10].draw(x_sig + _signoend.getWidth(), h_aux, 7.2);
		_tiemposfin[seg - 10 * (seg / 10)].draw(x_sig + _signoend.getWidth() + w_aux, h_aux, 7.2);
		cartel.draw((ofGetWidth() - cartel.getWidth()) / 2, 0, 7.2);
		string aux = "MNBMALRM";
		ofDrawBitmapString(aux, 20, ofGetHeight() - 20);
	}
}

//--------------------------------------------------------------
void sombreroApp::keyPressed(int key) {

	if (key == 'h'|| key == 'H') {
		if (tiempo_start == 0) {
			tiempo_start = ofGetSystemTime();
			tiempo_video_on = ofGetSystemTime();
			play_video = true;
			system("on_video.bat");
			needsend = true;
		}
		if (show_help == false) {
			tiempo_help1 = ofGetSystemTime();
		}
		show_help = !show_help;
	}
	if (key == '6') {
		pruebas = 2.0;
		needsend = true;
	}
	if (key == '4') {
		pruebas = 4.0;
		needsend = true;
	}
	if (key == 'f') {
		pruebas = 6.0;
		needsend = true;
	}

	if (key == 'r' || key == 'R') {
		if (tiempo_start > 0) {
			pruebas = 10.0;
			needsend = true;
		}
	}
	if (key == 'v') {
		play = !play;
		show_help = true;
	}
	if (play == true) {
		if (key == '+' && show_help == false) {
			show_clue = !show_clue;
		}
		if (key == '1') {
			pressed_1 = true;
			tiempo_reset = ofGetSystemTime();
		}
		if (key == '2') {
			pressed_2 = true;
		}
		if (key == 'm') {
			pressed_ = true;
		}
		if ((key == 'd' || key == 'D') && pared == 's') {
			pared = 't';
			needsend = true;
		}
		if ((key == 'a' || key == 'A') && pared == 's') {
			pared = 'c';
			needsend = true;
		}
		if (key == 'w' || key == 'W') {
			show_time = !show_time;
			needsend = true;
		}
	}
}

//--------------------------------------------------------------
void sombreroApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void sombreroApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void sombreroApp::mouseDragged(int x, int y, int button) {
	if (endstep1 == false) {
		if (button == 0) {
			if (picked == true) {
					xy[1].x = x - images[1].getWidth() / 2;
					xy[1].y = y - images[1].getHeight() / 2;
			}
		}
	}
	if (endstep1 == true && endstep2 == false) {
		if (button == 0) {
			if (picked == true) {
				xychess_queen.x = x - chess_red_queen.getWidth() / 2;
				xychess_queen.y = y - chess_red_queen.getHeight() / 2;
			}
		}
	}
}

//--------------------------------------------------------------
void sombreroApp::mousePressed(int x, int y, int button) {
	
	
	if (button == 1) {
		ofLog() << "pruebas: " << pruebas;
	}
	// comprobacion si estan sobre la prueba
	if (play == true && button == 0 && show_help == false && pared == 's') {
		if (pruebas == 0.0 && show_picture == true) {
			int i = 1;
			while ((i <= 8) && picked == false) {
				if (ofInRange(x, xy[i].x - range, xy[i].z + range) && ofInRange(y, xy[i].y - range, xy[i].w + range)) {
					xy[i].x = x - images[i].getWidth() / 2;
					xy[i].y = y - images[i].getHeight() / 2;
					picked = true;
					// reorder blocks to draw correctly
					// copy the first one into the aux one
					image_name[0] = image_name[1];
					xy[0] = xy[1];
					images[0] = images[1];
					index[0] = index[1];
					//move the selected image and props to first position
					image_name[1] = image_name[i];
					xy[1] = xy[i];
					images[1] = images[i];
					index[1] = index[i];
					//place the aux in the "free" space
					image_name[i] = image_name[0];
					xy[i] = xy[0];
					images[i] = images[0];
					index[i] = index[0];
					// check if the image was settled
					for (int j = 0; j < size(actual); j++) {
						if (actual[j] == index[1]) {
							actual[j] = 0;
						}
					}
				}
				i += 1;
			}
			if (show_chess == false) {
				if (piece1 == true) {
					if (queenToBoard == false && ofInRange(x, xychess_queen_real.x, xychess_queen_real.z) && ofInRange(y, xychess_queen_real.y, xychess_queen_real.w)) {
						queenToBoard = true;
						mario_coin.play();
					}
				}
				if (piece2 == true) {
					if (bishopToBoard == false && ofInRange(x, xychess_bishop_real.x, xychess_bishop_real.z) && ofInRange(y, xychess_bishop_real.y, xychess_bishop_real.w)) {
						bishopToBoard = true;
						mario_coin.play();
					}
				}
			}
		}
		//prueba del ajedrez
		if (pruebas == 1.0) {
			if (show_chess == false) {
				if (piece1 == true) {
					if (queenToBoard == false && ofInRange(x, xychess_queen_real.x, xychess_queen_real.z) && ofInRange(y, xychess_queen_real.y, xychess_queen_real.w)) {
						queenToBoard = true;
						mario_coin.play();
					}
				}
				if (piece2 == true) {
					if (bishopToBoard == false && ofInRange(x, xychess_bishop_real.x, xychess_bishop_real.z) && ofInRange(y, xychess_bishop_real.y, xychess_bishop_real.w)) {
						bishopToBoard = true;
						mario_coin.play();
					}
				}
			}
			if (show_chess == true && checkmate == false) {
				if (ofInRange(x, xychess_queen.x, xychess_queen.z) && ofInRange(y, xychess_queen.y, xychess_queen.w)) {
					show_points = true;
					picked = true;
				}
			}
		}
		if (pruebas == 2.0) {
			if (show_paper == false) {//small one is showed
				if (ofInRange(x, xypaper_small.x, xypaper_small.x + paper_small.getWidth()) && ofInRange(y, xypaper_small.y, xypaper_small.y + paper_small.getHeight())) {
					show_paper = !show_paper;
					show_clue = true;
					pruebas = 2.5;
					needsend = true;
				}
			}
		}
		// pruebas posicion de pruebas
		if (pruebas < 1.0 && show_picture == false && show_chess == false && show_clue == false) { // si la prueba se ha completado, la variable unicacuadro ha sido usada en la comunicacuadrocion, por lo que ya no entra aqui
			if (ofInRange(x, ofGetWidth() / 2 - 97, ofGetWidth() / 2 - 97 + cuadrofondo.getWidth())) { //posiciones del ajedrez en 3d
				if (ofInRange(y, ofGetHeight() / 2 - 140, ofGetHeight() / 2 - 140 + cuadrofondo.getHeight())) {
					show_picture = true;
				}
			}
		}
		if (pruebas < 2.0 && show_chess == false && show_picture == false && show_clue == false) { // si la prueba se ha completado, la variable unicacuadro ha sido usada en la comunicacuadrocion, por lo que ya no entra aqui
			if (ofInRange(x, 498, 637)&& ofInRange(y, 600, 688)) { //posiciones del cuadro en 3d
					show_chess = true;
			}
		}
		if (pruebas <2.0 && ofInRange(x, posvolver.x, posvolver.x + volver.getWidth()) && ofInRange(y, posvolver.y, posvolver.y + ofGetHeight())) {
			show_picture = false;
			show_chess = false;
			if (checkmate == true) {
				pruebas = 2.0;
				needsend = true;
			}
		}
		if (pruebas == 4.5 && comerbeber.getIsPlaying()==false) {
			if ((ofInRange(x,133,218)&&ofInRange(y,563,624))||(ofInRange(x, xydrinkme.x, xydrinkme.x+drinkme.getWidth()) && ofInRange(y, xydrinkme.y, xydrinkme.y + drinkme.getHeight()))) {
				pruebas = 5.0;
				needsend = true;
			}
			if ((ofInRange(x,760,903)&&ofInRange(y,520,624))||(ofInRange(x, xyeatme.x, xyeatme.x + eatme.getWidth()) && ofInRange(y, xyeatme.y, xyeatme.y + eatme.getHeight()))) {
				pacman.play();
			}
		}
	}
}

//--------------------------------------------------------------
void sombreroApp::mouseReleased(int x, int y, int button) {
	//	if (show_picture == true) {
	
	if (pruebas == 0.0) {
		if (picked == true) {
			picked = false;
			int i = 0;
			bool out = false;
			if (ofInRange(x, poscuadro.x - 45, poscuadro.x - 45 + marco.getWidth()) && ofInRange(y, poscuadro.y - 45, poscuadro.y - 45 + marco.getHeight())) {
				if (1 <= index[1] && index[1] <= 4) {//means user have picked a Mad Hatter image
					//check if its inside the picture
					while (i <= 7 && out == false) {
						if (ofInRange(x, xyrangeMH[i].x - range, xyrangeMH[i].z + range) && ofInRange(y, xyrangeMH[i].y - range, xyrangeMH[i].w + range)) {
							if (actual[i] == 0) {
								xy[1].x = xygoodMH[i].x;
								xy[1].y = xygoodMH[i].y;
								actual[i] = index[1];
							}
							else {
								xy[1].x = poscuadro.x;
								xy[1].y = poscuadro.y;
							}
							out = true;
							bubble.play();
						}
						i += 1;
					}
				}
				else {
					if (5 <= index[1] && index[1] <= 8) {//means user have picked a Queen of Hearts image
						while (i <= 7 && out == false) {
							if (ofInRange(x, xyrangeQH[i].x - range, xyrangeQH[i].z + range) && ofInRange(y, xyrangeQH[i].y - range, xyrangeQH[i].w + range)) {
								if (actual[i] == 0) {
									xy[1].x = xygoodQH[i].x;
									xy[1].y = xygoodQH[i].y;
									actual[i] = index[1];
								}
								else {
									xy[1].x = poscuadro.x;
									xy[1].y = poscuadro.y;
								}
								out = true;
								bubble.play();
							}
							i += 1;
						}
					}
				}
			}
			else {
				if (ofInRange(x, poscuadro.x - 45, poscuadro.x - 45 + marco.getWidth()) == false) {
					xy[1].x = poscuadro.x -45;
				}
				if (ofInRange(y, poscuadro.y - 45, poscuadro.y - 45 + marco.getHeight()) == false) {
					xy[1].y = poscuadro.y - 45;
				}
			}
		}
		if ((actual[0] == solution1[0]) && (actual[1] == solution1[1]) && (actual[2] == solution1[2]) && (actual[3] == solution1[3]) && (actual[4] == solution1[4]) && (actual[5] == solution1[5]) && (actual[6] == solution1[6]) && (actual[7] == solution1[7])) {
			if (piece1 == false) {
				piece1 = true;
				zelda_item.play();
			}
		}
		if ((actual[0] == solution2[0]) && (actual[1] == solution2[1]) && (actual[2] == solution2[2]) && (actual[3] == solution2[3]) && (actual[4] == solution2[4]) && (actual[5] == solution2[5]) && (actual[6] == solution2[6]) && (actual[7] == solution2[7])) {
			if (piece2 == false) {
				piece2 = true;
				zelda_secret.play();
			}

		}
	}
		
	if (pruebas == 1.0) {
		if (picked == true) {
			picked = false;
			show_points = false;
			int i = 0;
			bool out = false;
			while (i <= 21 && out == false) {
				if (ofInRange(x, xypoints[i].x - range2, xypoints[i].x + range2) && ofInRange(y, xypoints[i].y - range2, xypoints[i].y + range2)) {
					xychess_queen.x = xypoints[i].x - chess_red_queen.getWidth() / 2;
					xychess_queen.y = xypoints[i].y - chess_red_queen.getHeight() / 2;
					out = true;
					bubble.play();
					if (i == 16) {
						endstep2 = true;
						head_cut.play();
						checkmate = true;
					}
				}
				i += 1;
				if (i == 22 && out == false) {
					xychess_queen.x = xypoints[0].x - chess_red_queen.getWidth() / 2;;
					xychess_queen.y = xypoints[0].y - chess_red_queen.getHeight() / 2;;
				}
			}
		}
	}
}

//--------------------------------------------------------------
void sombreroApp::mouseEntered(int x, int y) {
	
}

//--------------------------------------------------------------
void sombreroApp::mouseExited(int x, int y) {
	picked = false;
}

//--------------------------------------------------------------
void sombreroApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void sombreroApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void sombreroApp::dragEvent(ofDragInfo dragInfo) {

}
