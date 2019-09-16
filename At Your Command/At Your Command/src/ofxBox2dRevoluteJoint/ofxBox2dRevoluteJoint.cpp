#pragma once  
//based on nay's ofxBox2dRevoluteJoint which was based on Todd Vanderlin's ofxBox2dJoint class  
//class comes from 
//https://forum.openframeworks.cc/t/ofxbox2drevolutejoint/3411?fbclid=IwAR2tHBm9kYZkaeZVzNo1hhZQQWnbTUtM8WYSjo8iYhia6kGMTgir91nUwIw
//class was modified by Jacob Fuehne so that it is in a header and cpp file to be used by other classes
//also, a number of overloaded addJoint methods were added

//NOTE:
//there is a known, seemingly unresolveable issue with box2d revolute joints where they do not enforce their assigned angle limits
//Using the overloaded method I made that takes in these angle limits will work, but it simply won't enforce the limits due to issues with the
//underlying box2d code.

//Due to my (Jacob Fuehne) project time constraints, I was unable to determine how to change the original b2RevoluteJointDef's and related files' code
//so that angle limits would be enforced, or if this was even possible to do (as this has apparently been a known issue since 2010, there must be some reason nobody has fixed
//it yet). In order to escape the "sunk cost fallacy" I encountered with implementing a ragdoll using ofxBox2d, I had to adopt a 
//"it's not a bug, it's a feature" philosophy and embrace my ragdoll's spaghetti joints.

#include "ofxBox2d.h"
#include "ofxBox2dRevoluteJoint/ofxBox2dRevoluteJoint.h"

	ofxBox2dRevoluteJoint::ofxBox2dRevoluteJoint() {
		world = NULL;
		alive = false;
	}

	void ofxBox2dRevoluteJoint::setWorld(b2World * w) {
		if (w == NULL) {
			ofLog(OF_LOG_NOTICE, "- box2d world needed -");
			return;
		}
		world = w;
		alive = true;
	}

	void ofxBox2dRevoluteJoint::destroyJoint() {
		world->DestroyJoint(joint);
		joint = NULL;
	}

	void ofxBox2dRevoluteJoint::addJoint(b2Body * a, b2Body * b) {
		addJoint(a, b, -0.25 * TWO_PI, 0.25 * TWO_PI, 0, 0);
	}

	void ofxBox2dRevoluteJoint::addJoint(b2Body * a, b2Body * b, int Xoffset, int Yoffset) {
		addJoint(a, b, -0.25 * TWO_PI, 0.25 * TWO_PI, Xoffset, Yoffset);
	}

	void ofxBox2dRevoluteJoint::addJoint(b2Body * a, b2Body * b, float lower_angle, float upper_angle, int Xoffset, int Yoffset) {
		if (world == NULL) {
			ofLog(OF_LOG_NOTICE, "- no joint added - box2d world needed -");
			return;
		}

		b2Vec2 anchor = a->GetWorldCenter();
		jd.Initialize(a, b, anchor);
		//need to override anchor set in Initialize() if you don't want the anchor to be in b2Body a's center  
		//(due to reasons I can't figure out!)  
		jd.localAnchorA.Set(Xoffset / OFX_BOX2D_SCALE, Yoffset / OFX_BOX2D_SCALE);

		jd.collideConnected = false;
		jd.lowerAngle = lower_angle;
		jd.upperAngle = upper_angle;
		jd.enableLimit = true;
		jd.maxMotorTorque = k_max_motor_torque;
		//jd.motorSpeed = 0.0;
		//jd.enableMotor = true;

		joint = world->CreateJoint(&jd);
	}

	void ofxBox2dRevoluteJoint::draw() {
		draw(200);
	}

	void ofxBox2dRevoluteJoint::draw(int alpha) {

		if (!alive) return;

		//Unsure what AlphaBlending actually DOES, but it messes up ofImage() and makes no meaningful 
		//contribution to my functionality, so I commented it out
		//ofEnableAlphaBlending(); 
		ofSetColor(255, 0, 255, alpha);

		b2Vec2 p1 = joint->GetAnchorA();
		b2Vec2 p2 = joint->GetAnchorB();

		p1 *= OFX_BOX2D_SCALE;
		p2 *= OFX_BOX2D_SCALE;
		ofLine(p1.x, p1.y, p2.x, p2.y);
		//ofDisableAlphaBlending();
		
	}
