#pragma once
#ifndef OFXBOX2DREVOLUTEJOINT_H
#define OFXBOX2DREVOLUTEJOINT_H

#include "ofxBox2d.h"

class ofxBox2dRevoluteJoint {

public:
	b2World * world;
	b2Joint * joint;
	bool alive;
	b2RevoluteJointDef jd;
	const float k_max_motor_torque = 30000; //default max motor torque. Determines how much torque is needed before the joint starts turning
	// ^ due to issues (probably related to the angle limits issue?) with the underlying box2d code, this number seems to make no observable difference, no matter what
	// value it is. This, combined with the angle limit issue, results in joints that seem incredibly loose. 

	//In the program At Your Command, these issues have been embraced with the "KNEES WEAK ARMS SPAGHETTI" command, which makes the player's body collapse
	//and his joints get "noodly"

	ofxBox2dRevoluteJoint();
	void setWorld(b2World * w);
	void destroyJoint();
	void addJoint(b2Body * a, b2Body * b);
	void addJoint(b2Body * a, b2Body * b, int Xoffset, int Yoffset);
	void addJoint(b2Body * a, b2Body * b, float lower_angle, float upper_angle, int Xoffset, int Yoffset);
	void draw();
	void draw(int alpha);
};

#endif /* OFXBOX2DREVOLUTEJOINT_H */