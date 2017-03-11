#include "tazasApp.h"
/*
* AUTHORS:
* Adrián López, Raúl Martín, Berta Martínez, Marta Navarro
*/
//--------------------------------------------------------------
void tazasApp::setup() {
	//Modelos 3D
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	// so the model isn't see through.
	ofEnableDepthTest();
	pista.loadImage("images/bateria.png");
	pista.resize(pista.getWidth() / 2.5, pista.getHeight() / 2.5);
	mac.load("sounds/mac_startup.mp3"); mac.setVolume(0.7);

	model.loadModel("habitaciontazas.obj");
	model.setPosition(ofGetWidth() / 2 + 45, ofGetHeight() * 2.6 + 200, 0);
	model.enableColors();
	model.enableMaterials();
	model.enableTextures();
	// this loads in the model directly into a mesh
	// ofMesh can only read in .plys that are in the ascii and not in the binary format.
	mesh.load("habitaciontazas.ply");

	//you can create as many rotations as you want
	//choose which axis you want it to effect
	//you can update these rotations later on
	// these rotation set the model so it is oriented correctly
	model.setRotation(0, 90, 1, 0, 0);
	model.setRotation(0, 0, 0, 1, 0);
	model.setScale(13, 13, 13);
	light.setPosition(1200, 500, 1500);

	reccerr.setup(PORTCT);
	sendcerr.setup(HOST, PORTTC);
	recsomb.setup(PORTST);
	sendsomb.setup(HOST, PORTTS);
	show_help = false;
	for (int f = 0; f < 48; f++) {
		string aux = "images/ayuda/ayuda_" + ofToString(f) + ".png";
		_help[f].load(aux);
	}
	help_index = 0;
	tiempo_help1 = 0;
	tiempo_help2 = 0;
	pared = 's';
	pruebas = 0.0;
	show_time = false;
	play = false;
}

//--------------------------------------------------------------
void tazasApp::update(){
	//Comunicaciones
	if (true) {
		while (reccerr.hasWaitingMessages()) {
			ofxOscMessage m;
			reccerr.getNextMessage(&m);
			if (m.getAddress() == "/cerradura/x") {
				pruebas = m.getArgAsFloat(0);
				pared = m.getArgAsChar(1);
				show_time = m.getArgAsBool(2);
				ofLog() << "t-c pared: " << pared;
				if (pared == 't') {
					system("monitortazas.bat");
				}
			}
		}
		while (recsomb.hasWaitingMessages()) {
			ofxOscMessage m;
			recsomb.getNextMessage(&m);
			if (m.getAddress() == "/sombrero/x") {
				pruebas = m.getArgAsFloat(0);
				pared = m.getArgAsChar(1);
				show_time = m.getArgAsBool(2);
				play = m.getArgAsBool(3);
				ofLog() << "t-s pared: " << pared;
				if (pared == 't') {
					system("monitortazas.bat");
				}
			}
		}
		// enviar
		if (needsend == true) {
			ofxOscMessage * m;
			m = new ofxOscMessage();
			m->setAddress("/tazas/x");
			m->addFloatArg(pruebas);
			m->addCharArg(pared);
			m->addBoolArg(show_time);
			sendcerr.sendMessage(*m);
			sendsomb.sendMessage(*m);
			delete m;    // this calls the destructor where the messages are cleared.   
			needsend = false;
		}
	}
	if (pruebas == 11.0) {
		show_help = false;
		i = 0;
		reslibro = false;
		pressed_ = false;
		pressed_1 = false;
		pressed_2 = false;

		pruebas = 12.0;
		ofLog() << "tazas reset ACK";
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
void tazasApp::draw(){

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
		//puerta.drawFaces();
		if (show_help == false) {
			if (pruebas == 5.5) {
				if (i < 196) {
					model.setScale(8.8 + 50 * (i / 5) / 39, 8.8 + 50 * (i / 5) / 39, 8.8);
					model.drawFaces();
					i++;
				}
				if (i == 196) {
					model.setScale(8.8 + 50, 50 + 8.8, 8.8);
					model.drawFaces();
				}
			}
			else if (5.5 < pruebas && pruebas < 10.0) {
				i = 196;
				model.setScale(8.8 + 50, 50 + 8.8, 8.8);
				model.drawFaces();
			}
			else {
				model.drawFaces();
			}
			if (reslibro && pruebas < 5.0) {
				pista.draw(448, 414);
			}
			ofHideCursor();
			if (pared == 't') {
				ofSetColor(255, 0, 255);
				ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 1, 5);
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
void tazasApp::keyPressed(int key){
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
		if ((key == 'a' || key == 'A') && pared == 't') {
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
void tazasApp::keyReleased(int key){

}

//--------------------------------------------------------------
void tazasApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void tazasApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void tazasApp::mousePressed(int x, int y, int button){
// pruebas posicion de pruebas
	if (button == 1.0) {
		ofLog() << x << "   " << y;
	}
	if (show_help == false && play == true) {
		if (pruebas == 2.5) { // si la prueba se ha completado, la variable unicacuadro ha sido usada en la comunicacuadrocion, por lo que ya no entra aqui
			if (ofInRange(x, 200, 260) && ofInRange(y, 360, 430)) {
				reslibro = true;
				mac.play();
				pruebas = 3.0;
				needsend = true;
				//system("monitorcerradura.bat");
				//serial.writeByte('C');
			}
		}
	}
}

//--------------------------------------------------------------
void tazasApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void tazasApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void tazasApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void tazasApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void tazasApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void tazasApp::dragEvent(ofDragInfo dragInfo){ 

}
