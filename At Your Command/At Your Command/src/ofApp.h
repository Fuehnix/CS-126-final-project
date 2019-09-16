#pragma once

#include "ofMain.h"
#include <string>
#include "ofxBox2d.h"
#include <vector>
#include "player.h"


using std::string;
using std::vector;

class ofApp : public ofBaseApp{
	private:
		bool party_mode = false; //seizure warning for when true ... you've been warned
		int color_counter = 0;
		const int k_party_mode_freq = 5; //the higher the number, the less often colors change
		// ^ adjust this value to make party mode less 'seizure prone' and less ridiculous. default value of 5 should be acceptable, but it is changeable as needed
		int random_R;
		int random_G;
		int random_B;

		float gravity = 10.0f;
		ofTrueTypeFont font;
		const int k_font_size = 48;
		int level_paint = 300;
		string input;
		ofxBox2d world2d;

		ofImage a_letter;
		ofImage background;
		ofImage evans_default;
		ofImage evans_surprise;
		ofImage evans_content;
		ofImage evans_happy;
		ofImage xkcd;
		ofImage evans_noseless;
		ofImage evans_nose;

		const int k_xkcd_happiness = 300;
		const int k_scary_physics_happiness = -3; //note that this value is updated very rapidly, so it is cumulatively significant
		const int k_dangerous_physics_happiness = -10; //note that this value is updated very rapidly, so it is cumulatively significant
		const int k_deadly_physics_happiness = -500; //note that this value is updated very rapidly, so it is cumulatively significant
		const int k_party_mode_happiness = 3000;
		const int k_boring_mode_happiness = -1000;
		const int k_shatter_happiness = -1000;
		const int k_limp_joint_happiness = -500;
		const int k_giving_A_grade_happiness = 300;
		const int k_taking_nose_happiness = -300;
		const int k_giving_nose_happiness = 200;
		
		//due to box2d scaling issues, we're just gonna pretend that evans is a superhuman astronaut, as, when put at realistic values
		//it is very difficult to not terrify/kill him
		const int k_g_force_nausea = 8;
		const int k_g_force_danger = 15;
		const int k_g_force_deadly = 30;

		const int happy_lower_bounds = 100;
		const int content_lower_bounds = 50;
		const int normal_lower_bounds = -50;

		Player player_evans;
		int emotion_meter = 500;
		float player_speed_prev = 0;
		float player_speed_curr = 0;
		float time_prev = 0;
		float time_curr = 0;
		float max_acceleration;


		vector <ofPtr<ofxBox2dPolygon>> grades;
		vector <ofPtr<ofxBox2dRect>> comics;
		vector <ofPolyline> lines;
		vector <shared_ptr<ofxBox2dEdge>> edges;
		ofPtr<ofxBox2dPolygon> evans_nose_poly;

		ofSoundPlayer soundPlayer;

	public:
		void setup();
		void update();

		void draw();
		void drawGrades();
		void drawText();
		void drawPlayer();
		void drawUserLines();
		void drawComics();
		void drawPlayerFace();
		void drawPlayerBody();
		void drawNose();

		ofImage determineEvansFace();

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

		void loadImages();
		void readInput();
		bool isValidInputKey(int key);
		void createALetterGrade(float x, float y);
		void createXKCD();
		void createNose(float x, float y);
		float ptm(float d);

		void evansGivesGrade();
		void reduceGravity();
		void increaseGravity();
		void takeEvansNose();

		void updateEmotion();
		float getAcceleration();
		void updateMaxAcceleration(float acceleration);
		void updatePlayerNausea();

		void destroyComics();
		void destroyGrades();
		void makePlayerFlexible();
};
