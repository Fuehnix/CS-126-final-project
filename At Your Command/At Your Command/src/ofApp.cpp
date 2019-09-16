#include "ofApp.h"
#include "ofxBox2d.h"
#include "player.h"
#include "ofxBox2dRevoluteJoint/ofxBox2dRevoluteJoint.h"
#include <math.h> 

//--------------------------------------------------------------
void ofApp::setup() {
	loadImages();

	//  window
	ofSetWindowShape(1920, 1080);
	ofSetVerticalSync(true);

	//  box2d
	world2d.init();
	world2d.setFPS(60.0f);
	world2d.createGround(0, 1080, 1920, 1080);
	world2d.registerGrabbing(); // allows user to click, grab, drag, and throw box2d objects using left click (which is why line drawing is binded to right click)

	//create player evans
	player_evans.createEvans(world2d.getWorld(), 500, 0, true);

	//load font
	font.load("Teko-Regular.ttf", k_font_size);

	//load party song
	soundPlayer.load("SpongeBob Production Music Stadium Rave A.mp3"); //yes, thats the official song name, don't hate
}

//--------------------------------------------------------------
void ofApp::update() {
	world2d.update();
	color_counter++;
	if (party_mode && !soundPlayer.getIsPlaying()) {
		soundPlayer.play();
	}
	updateEmotion();
	updatePlayerNausea();
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (color_counter % k_party_mode_freq == 0) {
		random_R = std::rand() % 255;
		random_G = std::rand() % 255;
		random_B = std::rand() % 255;
	}
	ofSetColor(random_R, random_G, random_B, 255);
	if (!party_mode) {
		ofSetColor(255);
	}
	background.draw(0, 0);
	drawText();
	drawPlayer();
	drawGrades();
	drawUserLines();
	drawComics();
}

void ofApp::drawGrades() {
	//draw the grade letter images and line them up with their polygon physics hitboxes
	for (int i = 0; i < grades.size(); i++) {
		ofPushMatrix();
		a_letter.bind();

		if (!party_mode) {
			ofSetColor(255);
		}
		grades[i].get()->draw();
		ofPoint pos = grades[i].get()->getPosition();
		glTranslatef(pos.x, pos.y, pos.z);
		float current_rotation = grades[i].get()->getRotation();
		ofRotateDeg(current_rotation);
		a_letter.draw(0, 0);

		a_letter.unbind();
		ofPopMatrix();
	}
}

void ofApp::drawText() {
	ofPushMatrix();
	font.drawString(input, 50, 50);
	font.drawString("Paint Remaining for the level: " + to_string(level_paint), 50, 100);
	font.drawString("FPS: " + ofToString(ofGetFrameRate()), 50, 150);
	if (player_evans.getEvansHead().get()->alive) {
		//note, speed is given in m/s^2.  Also, due to limitations in the underlying box2d library,
		//objects cannot exceed a speed of 120 m/s, but this is acceptable for our game, considering
		//that 120 m/s is actually 268mph
		font.drawString("Evans Speed: " + ofToString(player_evans.getEvansSpeed()), 50, 200);
		font.drawString("Evans Highest G-force : " + ofToString(max_acceleration/9.8), 50, 250);
	}
	ofPopMatrix();
}

void ofApp::drawPlayer() {
	if (!player_evans.getEvansHead().get()->alive) {
		return;
	}
	drawPlayerBody();
	drawPlayerFace();
	if (player_evans.getEvansFace() == Evans_face::noseless) {
		drawNose();
	}
}

void ofApp::drawPlayerFace() {
	ofPushMatrix();

	ofImage evans_face = determineEvansFace();
	evans_face.bind();
	if (!party_mode) {
		ofSetColor(255);
	}
	player_evans.getEvansHead().get()->draw();
	ofPoint pos = player_evans.getEvansHead()->getPosition();
	glTranslatef(pos.x, pos.y, pos.z);
	float current_rotation = player_evans.getEvansHead()->getRotation();
	ofRotateDeg(current_rotation);
	evans_face.setAnchorPercent(0.5, 0.5);
	evans_face.draw(0, 0);
	evans_face.unbind();

	ofPopMatrix();
}

void ofApp::drawPlayerBody() {
	if (player_evans.isBodyStatic()) {
		for (int i = 0; i < player_evans.getEvansStaticBody().size(); i++) {
			player_evans.getEvansStaticBody()[i].get()->draw();
		}
	}
	else {
		for (int i = 0; i < player_evans.getEvansBody().size(); i++) {
			player_evans.getEvansBody()[i].get()->draw();
		}
	}
}

void ofApp::drawNose() {
	ofPushMatrix();

	evans_nose.bind();
	if (!party_mode) {
		ofSetColor(255);
	}
	evans_nose_poly.get()->draw();
	ofPoint pos = evans_nose_poly.get()->getPosition();
	glTranslatef(pos.x, pos.y, pos.z);
	float current_rotation = evans_nose_poly.get()->getRotation();
	ofRotateDeg(current_rotation);
	evans_nose.setAnchorPercent(0.5, 0.5);
	evans_nose.draw(0, 0);
	evans_nose.unbind();

	ofPopMatrix();
}

void ofApp::drawUserLines() {

	for (int i = 0; i < lines.size(); i++) {
		lines[i].draw();
	}
	for (int i = 0; i < edges.size(); i++) {
		edges[i].get()->draw();
	}
}

void ofApp::drawComics() {
	for (int i = 0; i < comics.size(); i++) {
		ofPushMatrix();
		xkcd.bind();

		if (!party_mode) {
			ofSetColor(255);
		}
		comics[i].get()->draw();
		ofPoint pos = comics[i].get()->getPosition();
		glTranslatef(pos.x, pos.y, pos.z);
		float current_rotation = comics[i].get()->getRotation();
		ofRotateDeg(current_rotation);
		xkcd.setAnchorPercent(0.5, 0.5);
		xkcd.draw(0, 0);

		xkcd.unbind();
		ofPopMatrix();
	}
}

ofImage ofApp::determineEvansFace() {
	Evans_face current_expression = player_evans.getEvansFace();
	if (current_expression == Evans_face::noseless) {
		return evans_noseless;
	}
	if (current_expression == Evans_face::normal) {
		return evans_default;
	}
	if (current_expression == Evans_face::surprise) {
		return evans_surprise;
	}
	if (current_expression == Evans_face::content) {
		return evans_content;
	}
	if (current_expression == Evans_face::happy) {
		return evans_happy;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (isValidInputKey(key)) {
		//adds a valid key to the private string input
		input += (char)key;
		return;
	}
	if (key == OF_KEY_BACKSPACE) {
		if (input.size() != 0) {
			//resizes the input string so that it is 1 character shorter.
			//the character that it removes will always be the last character in the string
			//which is the character one would expect backspace to remove, so this code is a logical, simple workaround for
			//having a way of getting user input
			input.resize(input.size() - 1);
		}
		return;
	}
	if (key == OF_KEY_RETURN) {
		readInput();
		return;
	}
	if (key == OF_KEY_DEL) {
		//erase all mouse drawn lines
		lines.clear();
		edges.clear();
		level_paint = 300;
		return;
	}
	if (key == '-') {
		createXKCD();
		return;
	}
	if (key == '+') {
		createALetterGrade(mouseX,mouseY);
		return;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (level_paint > 0 && button == OF_MOUSE_BUTTON_RIGHT) {
		lines.back().addVertex(x, y);
		level_paint--;
		return;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	lines.push_back(ofPolyline());
	lines.back().addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	//code is from example Edges
	//https://github.com/vanderlin/ofxBox2d/blob/master/example-Edges/src/ofApp.cpp
	auto edge = std::make_shared<ofxBox2dEdge>();
	lines.back().simplify();

	for (auto i = 0; i < lines.back().size(); i++) {
		edge.get()->addVertex(lines.back()[i]);
	}

	edge.get()->create(world2d.getWorld());
	edges.push_back(edge);

	//end cited code
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::loadImages() {
	//load files for ofImage variables (this is acceptable format, so long as the file is local, i.e., in the bin/data folder)
	//note, this method must be called in setup. ofImage files cannot be loaded in while the box2d world is active
	a_letter.loadImage("A.png");
	background.loadImage("MorrowPlotsBackground.jpg");
	evans_default.loadImage("EvansDefault.png");
	evans_surprise.loadImage("EvansSurprise.png");
	evans_content.loadImage("EvansContent.png");
	evans_happy.loadImage("EvansHappy.png");
	evans_nose.loadImage("EvansNose.png");
	evans_noseless.loadImage("EvansNoseless.png");
	xkcd.loadImage("xkcd.png");

	//set anchors for the images
	a_letter.setAnchorPercent(0.5, 0.5);
	background.setAnchorPercent(0, 0);
	evans_default.setAnchorPercent(0.5, 0.5);
	evans_surprise.setAnchorPercent(0.5, 0.5);
	evans_content.setAnchorPercent(0.5, 0.5);
	evans_happy.setAnchorPercent(0.5, 0.5);
	evans_nose.setAnchorPercent(0.5, 0.5);
	evans_noseless.setAnchorPercent(0.5, 0.5);
	xkcd.setAnchorPercent(0.5, 0.5);
}

void ofApp::readInput() {
	string caps_input;
	for (char c : input) {
		caps_input += std::toupper(c);
	}
	if (caps_input == "RESET") {
		player_evans.destroyEvans();
		destroyComics();
		destroyGrades();
		player_evans.createEvans(world2d.getWorld(), 500, 0, true);
		input = "";
		return;
	}
	if (caps_input == "GRAVITY LEFT") {
		world2d.setGravity(-gravity, 0.0f);
		input = "";
		return;
	}
	if (caps_input == "GRAVITY RIGHT") {
		world2d.setGravity(gravity, 0.0f);
		input = "";
		return;
	}
	if (caps_input == "GRAVITY DOWN") {
		world2d.setGravity(0.0f, gravity);
		input = "";
		return;
	}
	if (caps_input == "GRAVITY UP") {
		world2d.setGravity(0.0f, -gravity);
		input = "";
		return;
	}
	if (caps_input == "SHATTER") {
		if (player_evans.isBodyStatic()) {
			makePlayerFlexible();
		}
		player_evans.destroyJoints();
		emotion_meter += k_shatter_happiness;
		input = "";
		return;
	}
	if (caps_input == "DECREASE GRAVITY") {
		reduceGravity();
		input = "";
		return;
	}
	if (caps_input == "INCREASE GRAVITY") {
		increaseGravity();
		input = "";
		return;
	}
	if (caps_input == "INCREASE FRICTION") {
		float current_friction = player_evans.getEvansFriction();
		player_evans.setEvansFriction(current_friction * 2);
		input = "";
		return;
	}
	if (caps_input == "DECREASE FRICTION") {
		float current_friction = player_evans.getEvansFriction();
		player_evans.setEvansFriction(current_friction / 2);
		input = "";
		return;
	}
	if (caps_input == "INCREASE DENSITY") {
		float current_density = player_evans.getEvansDensity();
		player_evans.setEvansDensity(current_density * 2);
		input = "";
		return;
	}
	if (caps_input == "DECREASE DENSITY") {
		float current_density = player_evans.getEvansDensity();
		player_evans.setEvansDensity(current_density / 2);
		input = "";
		return;
	}
	if (caps_input == "INCREASE BOUNCE") {
		float current_bounce = player_evans.getEvansBounce();
		player_evans.setEvansBounce(current_bounce * 2);
		input = "";
		return;
	}
	if (caps_input == "DECREASE BOUNCE") {
		float current_bounce = player_evans.getEvansBounce();
		player_evans.setEvansBounce(current_bounce / 2);
		input = "";
		return;
	}
	if (caps_input == "PARTY MODE") {
		party_mode = true;
		if (!soundPlayer.getIsPlaying()) {
			soundPlayer.play();
			emotion_meter += k_party_mode_happiness;
		}
		input = "";
		return;
	}
	if (caps_input == "BORING MODE") {
		party_mode = false;
		emotion_meter += k_boring_mode_happiness;
		soundPlayer.stop();
		input = "";
		return;
	}
	if (caps_input == "GIVE ME AN A") {
		evansGivesGrade();
		input = "";
		return;
	}
	if (caps_input == "KNEES WEAK ARMS SPAGHETTI") {
		makePlayerFlexible();
		input = "";
		return;
	}
	if (caps_input == "GOT YOUR NOSE") {
		takeEvansNose();
		emotion_meter += k_taking_nose_happiness;
		input = "";
		return;
	}
	if (caps_input == "GIVE NOSE BACK") {
		if (evans_nose_poly.get()) {
			evans_nose_poly.get()->destroy();
			emotion_meter += k_giving_nose_happiness;
			player_evans.setEvansFace(Evans_face::surprise);
		}
		input = "";
		return;
	}
}

bool ofApp::isValidInputKey(int key) {
	if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z')) {
		return true;
	}
	if (key == ' ') {
		return true;
	}
	return false;
}

void ofApp::createALetterGrade(float x, float y) {
	ofPtr<ofxBox2dPolygon> current_grade = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	current_grade.get()->addVertex(x - -0.010, y - 25.985);
	current_grade.get()->addVertex(x - -23.010, y - -20.015);
	current_grade.get()->addVertex(x - 23.990, y - -20.015);
	current_grade.get()->close();
	current_grade.get()->setPhysics(1.0, 0.5, 0.1);
	current_grade.get()->create(world2d.getWorld());
	grades.push_back(current_grade);
}

void ofApp::createXKCD() {
	ofPtr<ofxBox2dRect> current_comic = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	current_comic.get()->setPhysics(1.0, 0.5, 0.1);
	current_comic.get()->setup(world2d.getWorld(), mouseX, mouseY, 500.0f, 179.0f);
	comics.push_back(current_comic);
	emotion_meter += k_xkcd_happiness;
}

float ofApp::ptm(float d){
	//Box2d uses pixels to meters, but the template of the ragdoll body I am using uses pixels
	//so conversion is necessary.
	float PTM_RATIO = 32;
	return d / PTM_RATIO;
}

void ofApp::evansGivesGrade() {
	b2Vec2 position = player_evans.getPosition();
	createALetterGrade(position.x, position.y);
	emotion_meter += k_giving_A_grade_happiness;
}

void ofApp::reduceGravity() {
	float grav_x = world2d.getGravity().x;
	float grav_y = world2d.getGravity().y;
	world2d.setGravity(grav_x / 2, grav_y / 2);
	gravity = gravity / 2; //make sure gravity corresponds to the change
}

void ofApp::increaseGravity() {
	float grav_x = world2d.getGravity().x;
	float grav_y = world2d.getGravity().y;
	world2d.setGravity(grav_x * 2, grav_y * 2);
	gravity = gravity * 2; //make sure gravity corresponds to the change
}

void ofApp::takeEvansNose() {
	player_evans.setEvansFace(Evans_face::noseless);
	b2Vec2 position = player_evans.getPosition();
	createNose(position.x, position.y);
}

void ofApp::createNose(float x, float y) {
	evans_nose_poly = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	evans_nose_poly.get()->addVertex(21, 7);
	evans_nose_poly.get()->addVertex(4, 57);
	evans_nose_poly.get()->addVertex(40, 57);
	evans_nose_poly.get()->close();
	evans_nose_poly.get()->setPhysics(1.0, 0.5, 0.1);
	evans_nose_poly.get()->create(world2d.getWorld());
	evans_nose_poly.get()->setPosition(x, y);
}

void ofApp::updateEmotion() {
	if (emotion_meter > 0) {
		emotion_meter--;
	}
	else if (emotion_meter < 0) {
		emotion_meter++;
	}
	if (player_evans.getEvansFace() == Evans_face::noseless) {
		return;
	}
	if (happy_lower_bounds < emotion_meter) {
		player_evans.setEvansFace(Evans_face::happy);
		return;
	}
	if ((emotion_meter < happy_lower_bounds) && (emotion_meter > content_lower_bounds)) {
		player_evans.setEvansFace(Evans_face::content);
		return;
	}
	if ((emotion_meter < content_lower_bounds) && (emotion_meter > normal_lower_bounds)) {
		player_evans.setEvansFace(Evans_face::normal);
		return;
	}
	if (emotion_meter < normal_lower_bounds) {
		player_evans.setEvansFace(Evans_face::surprise);
		return;
	}
	return;
}

float ofApp::getAcceleration() {
	//must be called only after points update, if called twice in a row, it will return 0, because it hasn't moved technically
	float x = player_evans.getEvansVelocity().x;
	float y = player_evans.getEvansVelocity().y;

	//using formula for getting the speed vector from the x y velocity vectors
	//note, speed is different from velocity, as velocity includes direction and speed is just how fast you go
	player_speed_curr = sqrt(pow(x, 2) + pow(y, 2));
	time_curr = ofGetElapsedTimef();

	float acceleration = (player_speed_curr - player_speed_prev) / (time_curr - time_prev);

	player_speed_prev = player_speed_curr;
	time_prev = time_curr;
	
	return acceleration;
}

void ofApp::updateMaxAcceleration(float acceleration) {
	if (acceleration > max_acceleration) {
		//also note, box2d will give max_acceleration back in terms of m/s^2
		max_acceleration = acceleration;
	}
}

void ofApp::updatePlayerNausea() {
	float current_acceleration = getAcceleration();
	updateMaxAcceleration(current_acceleration);

	float current_g_force = current_acceleration / 9.8f;

	if (current_g_force > k_g_force_deadly) {
		emotion_meter += k_deadly_physics_happiness;
		return;
	}
	if (current_g_force > k_g_force_danger) {
		emotion_meter += k_dangerous_physics_happiness;
		return;
	}
	if (current_g_force > k_g_force_nausea) {
		emotion_meter += k_scary_physics_happiness;
		return;
	}
}

void ofApp::destroyComics() {
	for (int i = 0; i < comics.size(); i++) {
		comics[i].get()->destroy();
	}
	comics.clear();
}

void ofApp::destroyGrades() {
	for (int i = 0; i < grades.size(); i++) {
		grades[i].get()->destroy();
	}
	grades.clear();
}

void ofApp::makePlayerFlexible() {
	b2Vec2 position = player_evans.getPosition();
	player_evans.destroyEvans();
	player_evans.createEvans(world2d.getWorld(), position.x, position.y, false);
}