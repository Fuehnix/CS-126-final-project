#pragma once
#ifndef PLAYER_H
#define PLAYER_H
//body shape inspired by https://github.com/yannickl/Box2D-Examples/blob/master/box2dcocos2dexamples/Classes/TestRagdoll.mm
//but as you can see through final version and my commit history, I went far beyond what they did
//really, the only thing that is the same at this point is the pixel coordinates
#include "ofxBox2d.h"
#include "ofxBox2dRevoluteJoint/ofxBox2dRevoluteJoint.h"
/*
Important:
Methods that return the vector of body parts should be used at times when you are doing something with
every body part and don't care about specifics. However, if for some reason you as a programmer
wish to retrieve the vector of body parts and know exactly which body part is at some index of an array,
the order that the vectors return values is consistent with the order that the body parts are listed in this
header.
*/

enum class Evans_face { normal, surprise, content, happy, noseless };

class Player {
private:
	const float pi = 3.14159265359;
	bool is_body_static;
	float current_density = 1.0f;
	float current_bounce = 0.2f;
	float current_friction = 0.4f;
	ofPtr<ofxBox2dPolygon> player_evans;
	Evans_face current_expression = Evans_face::normal;

	ofPtr<ofxBox2dPolygon> torso_1;
	ofPtr<ofxBox2dPolygon> torso_2;
	ofPtr<ofxBox2dPolygon> torso_3;
	ofPtr<ofxBox2dPolygon> upper_arm_L;
	ofPtr<ofxBox2dPolygon> upper_arm_R;
	ofPtr<ofxBox2dPolygon> lower_arm_L;
	ofPtr<ofxBox2dPolygon> lower_arm_R;
	ofPtr<ofxBox2dPolygon> upper_leg_L;
	ofPtr<ofxBox2dPolygon> upper_leg_R;
	ofPtr<ofxBox2dPolygon> lower_leg_L;
	ofPtr<ofxBox2dPolygon> lower_leg_R;
	vector<ofPtr<ofxBox2dPolygon>> body_parts;

	ofPtr<ofxBox2dRect> torso_1_s;
	ofPtr<ofxBox2dRect> torso_2_s;
	ofPtr<ofxBox2dRect> torso_3_s;
	ofPtr<ofxBox2dRect> upper_arm_L_s;
	ofPtr<ofxBox2dRect> upper_arm_R_s;
	ofPtr<ofxBox2dRect> lower_arm_L_s;
	ofPtr<ofxBox2dRect> lower_arm_R_s;
	ofPtr<ofxBox2dRect> upper_leg_L_s;
	ofPtr<ofxBox2dRect> upper_leg_R_s;
	ofPtr<ofxBox2dRect> lower_leg_L_s;
	ofPtr<ofxBox2dRect> lower_leg_R_s;
	vector<ofPtr<ofxBox2dRect>> body_parts_s;

	ofxBox2dRevoluteJoint head_shoulders;
	ofxBox2dRevoluteJoint shoulder_R;
	ofxBox2dRevoluteJoint shoulder_L;
	ofxBox2dRevoluteJoint spine_top_2_mid;
	ofxBox2dRevoluteJoint spine_mid_2_bottom;
	ofxBox2dRevoluteJoint elbow_L;
	ofxBox2dRevoluteJoint elbow_R;
	ofxBox2dRevoluteJoint knee_L;
	ofxBox2dRevoluteJoint knee_R;
	ofxBox2dRevoluteJoint hip_R;
	ofxBox2dRevoluteJoint hip_L;

public:
	void createEvans(b2World *world, float x, float y, bool is_static);
	void createEvans(b2World *world, float x, float y, bool is_static, float density, float bounce, float friction);
	void createEvans(b2World *world, float x, float y, bool is_static, float density, float bounce, float friction, float scale_factor);
	void createEvans(b2World *world, float x, float y, bool is_static, float density, float bounce, float friction, float scale_factor_x, float scale_factor_y);
	
	void setEvansBodyType(b2BodyType type);
	void setEvansStaticBodyType(b2BodyType type);
	void setEvansDensity(float density);
	void setEvansBounce(float bounce);
	void setEvansFriction(float friction);
	void setEvansPhysics(float density, float bounce, float friction);
	void setEvansFace(Evans_face expression);
	
	void createEvansBodyParts(b2World *world, float x, float y);
	void createEvansBodyParts(b2World *world, float x, float y, float scale_factor);
	void createEvansBodyParts(b2World *world, float x, float y, float scale_factor_x, float scale_factor_y);
	void createEvansBodyPart(ofPtr<ofxBox2dPolygon> shape, b2World *world, float pos_x, float pos_y, float width, float height);
	void createEvansStaticBodyParts(b2World *world, float pos_x, float pos_y);

	float ptm(float d);
	void initializeEvansBodyParts();
	void initializeEvansStaticBodyParts();
	void createEvansHead(b2World *world, float x, float y, float scale_factor_x, float scale_factor_y);

	vector<ofPtr<ofxBox2dPolygon>> getEvansBody();
	vector<ofPtr<ofxBox2dRect>> getEvansStaticBody();
	ofPtr<ofxBox2dPolygon> getEvansHead();
	ofVec2f getEvansVelocity();
	float getEvansSpeed();
	Evans_face getEvansFace();
	float getEvansFriction();
	float getEvansBounce();
	float getEvansDensity();
	bool isBodyStatic();
	b2Vec2 getPosition();

	void createStaticJoint(b2World *world, float x, float y, ofPtr<ofxBox2dRect> body1, ofPtr<ofxBox2dRect> body2, b2RevoluteJointDef &jd, b2Vec2 anchor);
	void createEvansStaticJoints(b2World *world, float x, float y);
	void createEvansBrokenJoints(b2World *world, float x, float y);
	void createEvansBrokenJoints(b2World *world, float x, float y, float scale_factor);
	void createEvansBrokenJoints(b2World *world, float x, float y, float scale_factor_x, float scale_factor_y);
	void Player::destroyJoints();
	void destroyEvans();
};

#endif /* PLAYER_H */