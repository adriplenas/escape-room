#include "cerraduraApp.h"
/*
* AUTHORS:
* Adrián López, Raúl Martín, Berta Martínez, Marta Navarro
*/
//--------------------------------------------------------------
void cerraduraApp::setup(){
	//Modelos 3D
	if (true) {
		ofBackground(0, 0, 0);
		ofSetVerticalSync(true);
		ofEnableDepthTest();	// so the model isn't see through.

		model.loadModel("habitacioncerradura.obj");
		model.setPosition(ofGetWidth() / 2, ofGetHeight() * 2.6 - 400, 0);
		model.enableColors();
		model.enableMaterials();
		model.enableTextures();
		// puerta fin de pruebas
		puerta.loadModel("puerta.obj");
		puerta.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, -200);
		puerta.enableColors();
		puerta.enableMaterials();
		puerta.enableTextures();
		puerta.setRotation(0, 270, 0, 1, 0);
		puerta.setScale(13, 13, 13);
		mpuerta.load("puerta.ply");
		// this loads in the model directly into a mesh
		// ofMesh can only read in .plys that are in the ascii and not in the binary format.
		mesh.load("habitacioncerradura.ply");
		puerta_amp.load("puerta_mod.png");

		//you can create as many rotations as you want
		//choose which axis you want it to effect
		//you can update these rotations later on
		// these rotation set the model so it is oriented correctly
		//model.setRotation(0, 90, 1, 0, 0);
		model.setRotation(0, 270, 0, 1, 0);
		model.setScale(8.8, 8.8, 8.8);

		light.setPosition(0, 0, 1500);
	}
	//comunicaciones
	recsomb.setup(PORTSC);
	sendsomb.setup(HOST, PORTCS);
	rectaz.setup(PORTTC);
	sendtaz.setup(HOST, PORTCT);

	_papel.load("images/ppartitura.png");
	_notas.load("images/Captura.png");
	_notasV.load("images/Capturav.png");

	_A.load("images/letra C1.PNG");
	_B.load("images/letra B.PNG");
	_C.load("images/letra C2.PNG");
	_D.load("images/letra P.PNG");
	_E.load("images/letra C3.PNG");

	_drinkme.load("images/beber.png");//_drinkme.resize(_drinkme.getWidth()/2,_drinkme.getHeight()/2);
	_eatme.load("images/eatme.jpg"); _eatme.resize(_eatme.getWidth() / 2, _eatme.getHeight() / 2);

	darriba.load("sounds/darriba.wav");
	dabajo.load("sounds/dabajo.wav");
	iarriba.load("sounds/iarriba.wav");
	iabajo.load("sounds/iabajo.wav");
	bombo.load("sounds/bombo.wav");
	plato1.load("sounds/plato1.wav");
	plato2.load("sounds/plato2.wav");
	platillo.load("sounds/platillo.wav");
	_queen.load("sounds/queen.wav");
	_champions.load("sounds/Alice.wav");
	_queen.setVolume(_queen.getVolume()/2);
	_champions.setVolume(_champions.getVolume()/2);
	embolo.load("sounds/embolo.wav"); embolo.setVolume(0.5);
	bubble.load("sounds/bubble.mp3");

	show_help = false;
	for (int f = 0; f < 48; f++) {
		string aux = "images/ayuda/ayuda_" + ofToString(f) + ".png";
		_help[f].load(aux);
	}
	help_index = 0;
	tiempo_help1 = 0;
	tiempo_help2 = 0;

	pruebas = 0.0;
	pared = 's';
	show_time = false;
	play = false;
	puerta_mov = false;
}

//--------------------------------------------------------------
void cerraduraApp::update(){
	// rotates the model in a circle
	puerta.setRotation(1, 270 + ofGetElapsedTimef() * 60, 0, 1, 0);
	//Comunicaciones
	if (true) {
		while (recsomb.hasWaitingMessages()) {
			ofxOscMessage m;
			recsomb.getNextMessage(&m);
			if (m.getAddress() == "/sombrero/x") {
				pruebas = m.getArgAsFloat(0);
				pared = m.getArgAsChar(1);
				show_time = m.getArgAsBool(2);
				play = m.getArgAsBool(3);
				ofLog() << "c-s pared: " << pared;
				if (pared == 'c') {
					system("monitorcerradura.bat");
				}
			}
		}
		while (rectaz.hasWaitingMessages()) {
			ofxOscMessage m;
			rectaz.getNextMessage(&m);
			if (m.getAddress() == "/tazas/x") {
				pruebas = m.getArgAsFloat(0);
				pared = m.getArgAsChar(1);
				show_time = m.getArgAsBool(2);
				ofLog() << "c-t pared: " << pared;
				if (pared == 'c') {
					system("monitorcerradura.bat");
				}
			}
		}
		if (needsend == true) {
			ofxOscMessage * m;
			m = new ofxOscMessage();
			m->setAddress("/cerradura/x");
			m->addFloatArg(pruebas);
			m->addCharArg(pared);
			m->addBoolArg(show_time);
			sendtaz.sendMessage(*m);
			sendsomb.sendMessage(*m);
			delete m;    // this calls the destructor where the messages are cleared.   
			needsend = false;
		}
	}
	if (pruebas == 4.0) {
		if (_queen.isPlaying() == false) {
			mostrarPartitura = false;
			pruebas = 4.5;
			needsend = true;
			//system("monitorsombrero.bat");
			//serial.writeByte('S');
		}
	}
	if (pruebas == 5.0){
		embolo.play();
		embolo.setMultiPlay(false);
		pruebas = 5.5;
		needsend = true;
	}
	if (pruebas == 6.0) {
		if (_champions.isPlaying() == false) {
			pruebas = 6.5;
			system("monitorsombrero.bat");
			needsend = true;
		}
	}
	if (pruebas == 10.0) {
		float ti = ofGetSystemTime();
		show_help = false;
		for (int aux = 0; aux < 12; aux++) {
			notas[aux] = 0;
		}
		pos = 0;
		mostrarPartitura = false;
		pressed_ = false;
		pressed_1 = false;
		pressed_2 = false;
		embolo.stop(); embolo.setPosition(0);
		_queen.stop(); _queen.setPosition(0);
		_champions.stop(); _champions.setPosition(0);
		i = 0;
		pruebas = 11.0;
		nota_bien = 0;
		ofLog() << "Cerradura reset ACK";
		needsend = true;

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
	tiempo_help2 = (ofGetSystemTime() - tiempo_help1) / 1500;
	help_index = tiempo_help2 - 48 * (tiempo_help2 / 48);
}

//--------------------------------------------------------------
void cerraduraApp::draw() {
	light.enable();
	// draws the ply file loaded into the mesh is you pressed 6
	cam.begin();
	mesh.draw();
	//mpuerta.draw();
	cam.end();
	// draws all the other file types which are loaded into model.
	model.setScale(8.8, 8.8, 8.8);

	model.setPosition(ofGetWidth() / 2, ofGetHeight() * 2.6 - 400, 0);
	ofSetColor(255, 255, 255);
	if (play == true) {
		if (show_help == false) {
			if (pruebas == 5.5) {
				if (i < 196) {
					model.setScale(8.8, 8.8 + 50 * (i / 5) / 19.5, 8.8 + 50 * (i / 5) / 19.5);
					model.drawFaces();
					puerta_amp.draw((ofGetWidth() - 10.4 * i / 5) / 2, ofGetHeight() - 16.45 * i / 5 - 81 - (39 * i / 195), 1, 10.4 * i / 5, 16.45 * i / 5);
					if (i == 195) {
						embolo.stop();
						bubble.play();
					}
					i++;
				}
				if (i == 196) {
					model.setScale(8.8, 50 + 8.8, 50 + 8.8);
					model.drawFaces();
					puerta_amp.draw((ofGetWidth() - 10.4 * i / 5) / 2, ofGetHeight() - 16.45 * i / 5 - 81 - (39 * i / 195), 1, 10.4 * i / 5, 16.45 * i / 5);
					pruebas = 5.7;
					needsend = true;
					//system("monitorcerradura.bat");
					//serial.writeByte('C');
				}
			}
			if (5.5 < pruebas && pruebas < 10.0) {
				i = 196;
				model.setScale(8.8, 50 + 8.8, 50 + 8.8);
				model.drawFaces();
				puerta_amp.draw((ofGetWidth() - 10.4 * i / 5) / 2, ofGetHeight() - 16.45 * i / 5 - 81 - (39 * i / 195), 1, 10.4 * i / 5, 16.45 * i / 5);
			}
			else {
				if (i < 3) {
					model.drawFaces();
					puerta_amp.draw((ofGetWidth() - 416 / 80) / 2, ofGetHeight() - 658 / 40 - 81, 10, 416 / 40, 658 / 40);
				}
			}

			//
			if (pruebas == 3.0 || pruebas == 4.0) {
				if (mostrarPartitura) {
					/*_partitura.draw(ofGetWidth() / 6, ofGetHeight() / 2 - _partitura.getHeight() + 40, 0);
					if (nota_bien > 0) {
						_partitura2.drawSubsection(ofGetWidth() / 6, ofGetHeight() / 2 - _partitura.getHeight() + 40, 0.1, 155 + nota_bien * 53, _partitura2.getHeight() / 2, 0, 0);
					}if (nota_bien > 6) {
						_partitura2.drawSubsection(ofGetWidth() / 6, ofGetHeight() / 2 - _partitura.getHeight()/2 + 40, 0.1, 155 + (nota_bien -6) * 53, _partitura2.getHeight()/2, 0,_partitura2.getHeight()/2);
					}*/
					ofVec2f xy_partitura;
					xy_partitura.set(ofGetWidth() / 6, ofGetHeight() / 2 - _papel.getHeight() + 40);
					_papel.draw(xy_partitura.x, xy_partitura.y, 0);
					_notas.draw(xy_partitura.x + 140, xy_partitura.y + 80, 0.1);
					if (nota_bien > 0) {
						_notasV.drawSubsection(xy_partitura.x + 140, xy_partitura.y + 80, 0.2, 105 + nota_bien * 42, 113, 0, 0);
					}if (nota_bien > 6) {
						_notasV.drawSubsection(xy_partitura.x + 140, xy_partitura.y + 194, 0.3, 56 + (nota_bien - 6) * 51, 97, 0, 114);
					}
					_A.draw(393 - _A.getWidth() / 46, 629 - _A.getHeight() / 44, -10, ofGetWidth() / 23, ofGetHeight() / 22);
					_B.draw(254 - _B.getWidth() / 46, 694 - _B.getHeight() / 44, -10, ofGetWidth() / 23, ofGetHeight() / 22);
					_C.draw(326 - _C.getWidth() / 46, 576 - _C.getHeight() / 44, -10, ofGetWidth() / 23, ofGetHeight() / 22);
					_D.draw(114 - _D.getWidth() / 46, 532 - _D.getHeight() / 44, -10, ofGetWidth() / 23, ofGetHeight() / 22);
					_E.draw(225 - _E.getWidth() / 46, 573 - _E.getHeight() / 44, -10, ofGetWidth() / 23, ofGetHeight() / 22);

				}
			}
			ofHideCursor();
			if (pared == 'c') {
				ofSetColor(255, 0, 255);
				ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 1.1, 5);
			}

			light.disable();
		}
		else {
			ofSetColor(255, 255, 255);
			_help[help_index].draw(0, 0, 8);
		}
	}
	else {
		ofSetColor(0, 0, 0);
		ofDrawRectangle(0, 0, 1, ofGetWidth(), ofGetHeight());
	}
}

//--------------------------------------------------------------
void cerraduraApp::keyPressed(int key){
	if (play == true) {
		if (key == 'h' || key == 'H') {
			if (show_help == false) {
				tiempo_help1 = ofGetSystemTime();
			}
			show_help = !show_help;
		}
		if (key == 'r' || key == 'R') {
			pruebas = 10.0;
			needsend = true;
		}
		if (key == '1') {
			pressed_1 = true;
			tiempo_reset = ofGetSystemTime();
		}
		if (key == '2') {
			pressed_2 = true;
		}
		if (key == '-') {
			pressed_ = true;
		}
		if (key == '3') {
			pruebas = 3.0;
			needsend = true;
		}
		if ((key == 'd' || 'D') && pared == 'c') {
			pared = 's';
			needsend = true;
		}
		if (key == 'w' || key == 'W') {
			show_time = !show_time;
			needsend = true;
		}
	}
}

//--------------------------------------------------------------
void cerraduraApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void cerraduraApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void cerraduraApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void cerraduraApp::mousePressed(int x, int y, int button){

	if (button == 1) {
		ofLog() << "pruebas: " << pruebas;
	}
	if (show_help == false && play == true) {
		if ((!mostrarPartitura) && (pruebas == 3.0)) {
			if ((ofInRange(x, 120, 450)) && (ofInRange(y, 470, 740))) {
				mostrarPartitura = true;
			}
		}
		else {
			if (pruebas == 3.0) {
				int a = 0;
				//Letra D
				if ((ofInRange(x, 151, 234) && ofInRange(y, 503, 525)) || (ofInRange(x, 146, 195) && ofInRange(y, 525, 565))) { // 1 que es el platillo grande
					notas[pos] = 1;
					//tiempo[pos] = ofGetSystemTimeMicros() / 1000;//abs(double(ofGetSystemTimeMicros()));
					pos++;
					platillo.play();
				}
				//Letra A
				else if (ofInRange(x, 367, 435) && ofInRange(y, 568, 669)) { // 2 que es el tambor gordo
					notas[pos] = 2;
					//tiempo[pos] = ofGetSystemTimeMicros() / 1000;//abs(double(ofGetSystemTimeMicros()));
					pos++;
					dabajo.play();
				}
				// Letra C
				else if (ofInRange(x, 293, 362) && ofInRange(y, 554, 601)) { // 5 que es el derecha arriba
					notas[pos] = 5;
					//tiempo[pos] = ofGetSystemTimeMicros() / 1000;//abs(double(ofGetSystemTimeMicros()));
					pos++;
					darriba.play();
				}
				//Letra B
				else if (ofInRange(x, 183, 313) && ofInRange(y, 633, 752)) { // bombo
					notas[pos] = 6;
					//tiempo[pos] = ofGetSystemTimeMicros() / 1000;//abs(double(ofGetSystemTimeMicros()));
					pos++;
					bombo.play();
				}
				//Letra E
				else if (ofInRange(x, 191, 260) && ofInRange(y, 552, 593)) { // iarriba
					notas[pos] = 6;
					//tiempo[pos] = ofGetSystemTimeMicros() / 1000;//abs(double(ofGetSystemTimeMicros()));
					pos++;
					iarriba.play();
				}

				//ofLog() << "notas " << notas[0] << notas[1] << notas[2] << notas[3] << notas[4] << notas[5] << notas[6] << notas[7] << notas[8] << notas[9] << notas[10] << notas[11];
				bool ritmo = true;
				while ((notas[a] == queen[a]) && (a < 12) && ritmo) {
					if ((a == 3) || (a == 6) || (a == 9)) {
						if ((tiempo[a] - tiempo[a - 1]) > 4000) {
							ritmo = true;
						}
					}
					else if (a > 0) {
						if ((tiempo[a] - tiempo[a - 1]) > 2000) {
							ritmo = true;;
						}
					}
					a++;
				}
				if ((a == 12) && ritmo) {
					pruebas = 4.0;
					needsend = true;
					_queen.play();
				}
				else if ((notas[a] != 0) || (!ritmo)) {
					pos = 0;
					for (int g = 0; g <= 11; g++) {
						notas[g] = 0;
					}
				}
				nota_bien = pos;
			}
		}

		if (pruebas > 5.5 && pruebas < 6.0) {
			if (ofInRange(x, 291, 711) && ofInRange(y, 10, 625)) {
				system("off_music.bat");
				_champions.play();
				pruebas = 6.0;
				needsend = true;
			}
		}
	}
}

//--------------------------------------------------------------
void cerraduraApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void cerraduraApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void cerraduraApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void cerraduraApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void cerraduraApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void cerraduraApp::dragEvent(ofDragInfo dragInfo){ 

}
