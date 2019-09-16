#include "player.h"
#include <cstdlib>
#include "ofxBox2dRevoluteJoint/ofxBox2dRevoluteJoint.h"

void Player::createEvans(b2World *world, float x, float y, bool is_static) {
	createEvans(world, x, y, is_static, current_density, current_bounce, current_friction);
}

void Player::createEvans(b2World *world, float x, float y, bool is_static, float density, float bounce, float friction) {
	createEvans(world, x, y, is_static, density, bounce, friction, 1.0f);
}

void Player::createEvans(b2World *world, float x, float y, bool is_static, float density, float bounce, float friction, float scale_factor) {
	createEvans(world, x, y, is_static, density, bounce, friction, scale_factor, scale_factor);
}

void Player::createEvans(b2World *world, float x, float y, bool is_static, float density, float bounce, float friction, float scale_factor_x, float scale_factor_y) {
	is_body_static = is_static;
	if (is_static) {
		initializeEvansStaticBodyParts();
		createEvansHead(world, x, y, scale_factor_x, scale_factor_y);
		createEvansStaticBodyParts(world, x, y);
		setEvansPhysics(density, bounce, friction);
		setEvansStaticBodyType(b2_dynamicBody);
		createEvansStaticJoints(world, x, y);
	}
	initializeEvansBodyParts();
	createEvansHead(world, x, y, scale_factor_x, scale_factor_y);
	createEvansBodyParts(world, x, y, scale_factor_x, scale_factor_y);
	setEvansPhysics(density, bounce, friction);
	setEvansBodyType(b2_dynamicBody);
	createEvansBrokenJoints(world, x, y, scale_factor_x, scale_factor_y);
}

void Player::createEvansStaticJoints(b2World *world, float x, float y) {
	b2RevoluteJointDef jd;
	jd.enableLimit = true;

	
	// Head to shoulders
	jd.Initialize(torso_1_s.get()->body, player_evans.get()->body, b2Vec2(ptm(x), ptm(y - 70.0f)));
	world->CreateJoint(&jd);

	// Upper arm to shoulders -- L to R
	createStaticJoint(world, x, y, torso_1_s, upper_arm_L_s, jd, b2Vec2(ptm(x - 18.0f), ptm(y + 20.0f)));
	createStaticJoint(world, x, y, torso_1_s, upper_arm_R_s, jd, b2Vec2(ptm(x + 18.0f), ptm(y + 20.0f)));

	// Lower arm to shoulders -- L to R
	createStaticJoint(world, x, y, upper_arm_L_s, lower_arm_L_s, jd, b2Vec2(ptm(x - 45.0f), ptm(y + 20.0f)));
	createStaticJoint(world, x, y, upper_arm_R_s, lower_arm_R_s, jd, b2Vec2(ptm(x + 45.0f), ptm(y + 20.0f)));

	// Shoulders / stomach ----- 
	createStaticJoint(world, x, y, torso_1_s, torso_2_s, jd, b2Vec2(ptm(x), ptm(y + 35.0f)));

	// Stomach / hips
	createStaticJoint(world, x, y, torso_2_s, torso_3_s, jd, b2Vec2(ptm(x), ptm(y + 50.0f)));

	// Torso to upper leg ------ L to R
	createStaticJoint(world, x, y, torso_3_s, upper_leg_L_s, jd, b2Vec2(ptm(x - 8), ptm(y + 72.0f)));
	createStaticJoint(world, x, y, torso_3_s, upper_leg_R_s, jd, b2Vec2(ptm(x + 8), ptm(y + 72.0f)));

	// Upper leg to lower leg -- L to R
	createStaticJoint(world, x, y, upper_leg_L_s, lower_leg_L_s, jd, b2Vec2(ptm(x - 8), ptm(y + 105.0f)));
	createStaticJoint(world, x, y, upper_leg_R_s, lower_leg_R_s, jd, b2Vec2(ptm(x + 8), ptm(y + 105.0f)));
}

void Player::createStaticJoint(b2World *world, float x, float y, ofPtr<ofxBox2dRect> body1, ofPtr<ofxBox2dRect> body2, b2RevoluteJointDef &jd, b2Vec2 anchor){
	jd.Initialize(body1.get()->body, body2.get()->body, anchor);
	world->CreateJoint(&jd);
}
void Player::createEvansBrokenJoints(b2World *world, float x, float y) {
	createEvansBrokenJoints(world, x, y, 2.0f);
}

void Player::createEvansBrokenJoints(b2World *world, float x, float y, float scale_factor) {
	createEvansBrokenJoints(world, x, y, scale_factor, scale_factor);
}

void Player::createEvansBrokenJoints(b2World *world, float x, float y, float scale_factor_x, float scale_factor_y) {
	b2RevoluteJointDef jd;
	jd.enableLimit = true;

	//create joints (believe it or not, this is the cleanest possible way to do this)
	head_shoulders.setWorld(world);
	head_shoulders.addJoint(torso_1.get()->body, player_evans.get()->body, -40.0f * (180.0f / pi), 40.0f * (180.0f / pi), 
		ptm(scale_factor_x * x), ptm(scale_factor_x * (y - 70.0f)));

	shoulder_L.setWorld(world);
	shoulder_L.addJoint(torso_1.get()->body, upper_arm_L.get()->body, -85.0f * (180.0f / pi), 85.0f * (180.0f / pi), 
		ptm(scale_factor_x * (x - 18.0f)), ptm(scale_factor_y * (y + 20.0f)));

	shoulder_R.setWorld(world);
	shoulder_R.addJoint(torso_1.get()->body, upper_arm_R.get()->body, -85.0f * (180.0f / pi), -85.0f * (180.0f / pi), 
		ptm(scale_factor_x * (x - 18.0f)), ptm(scale_factor_y * (y + 20.0f)));

	spine_top_2_mid.setWorld(world);
	spine_top_2_mid.addJoint(torso_1.get()->body, torso_2.get()->body, -15.0f * (180.0f / pi), 15.0f * (180.0f / pi), 
		ptm(scale_factor_x * x), ptm(scale_factor_y * (y + 35.0f)));

	spine_mid_2_bottom.setWorld(world);
	spine_mid_2_bottom.addJoint(torso_2.get()->body, torso_3.get()->body, -5.0f * (180.0f / pi), 5.0f * (180.0f / pi), 
		ptm(scale_factor_x * x), ptm(scale_factor_y * (y + 50.0f)));

	elbow_L.setWorld(world);
	elbow_L.addJoint(upper_arm_L.get()->body, lower_arm_L.get()->body, -70.0f * (180.0f / pi), 70.0f * (180.0f / pi), 
		ptm(scale_factor_x * (x - 45.0f)), ptm(scale_factor_y * (y + 20.0f)));

	elbow_R.setWorld(world);
	elbow_R.addJoint(upper_arm_R.get()->body, lower_arm_R.get()->body, -70.0f * (180.0f / pi), 70.0f * (180.0f / pi), 
		ptm(scale_factor_x * (x + 45.0f)), ptm(scale_factor_y * (y + 20.0f)));

	knee_L.setWorld(world);
	knee_L.addJoint(torso_3.get()->body, upper_leg_L.get()->body, -35.0f * (180.0f / pi), 35.0f * (180.0f / pi), 
		ptm(scale_factor_x * (x - 8.0f)), ptm(scale_factor_y * (y + 72.0f)));;

	knee_R.setWorld(world);
	knee_R.addJoint(torso_3.get()->body, upper_leg_R.get()->body, -35.0f * (180.0f / pi), -35.0f * (180.0f / pi), 
		ptm(scale_factor_x * (x + 8.0f)), ptm(scale_factor_y * (y + 72.0f)));;

	hip_R.setWorld(world);
	hip_R.addJoint(upper_leg_R.get()->body, lower_leg_R.get()->body, -70.0f * (180.0f / pi), 70.0f * (180.0f / pi), 
		ptm(scale_factor_x * (x - 8.0f)), ptm(scale_factor_y * (y + 105.0f)));;

	hip_L.setWorld(world);
	hip_L.addJoint(upper_leg_L.get()->body, lower_leg_L.get()->body, -70.0f * (180.0f / pi), 70.0f * (180.0f / pi), 
		ptm(scale_factor_x * (x + 8.0f)), ptm(scale_factor_y * (y + 105.0f)));;
}

void Player::setEvansBodyType(b2BodyType type) {
	player_evans.get()->body->SetType(type);
	for (int i = 0; i < body_parts.size(); i++) {
		body_parts[i].get()->body->SetType(type);
	}
}

void Player::setEvansStaticBodyType(b2BodyType type) {
	player_evans.get()->body->SetType(type);
	for (int i = 0; i < body_parts_s.size(); i++) {
		body_parts_s[i].get()->body->SetType(type);
	}
}

void Player::createEvansHead(b2World *world, float x, float y, float scale_factor_x, float scale_factor_y) {
	//vertices were acquired by tracing points around the image of evan's head
	//with help of an image editor
	player_evans.get()->addVertex((scale_factor_x * 18.753), (scale_factor_y * 92.380));
	player_evans.get()->addVertex((scale_factor_x * -23.247), (scale_factor_y * 88.380));
	player_evans.get()->addVertex((scale_factor_x * -66.247), (scale_factor_y * 53.380));
	player_evans.get()->addVertex((scale_factor_x * -50.247), (scale_factor_y * -70.620));
	player_evans.get()->addVertex((scale_factor_x * -6.247), (scale_factor_y * -99.620));
	player_evans.get()->addVertex((scale_factor_x * 48.753), (scale_factor_y * -69.620));
	player_evans.get()->addVertex((scale_factor_x * -66.247), (scale_factor_y * 53.380));
	player_evans.get()->addVertex((scale_factor_x * 61.753), (scale_factor_y * 22.380));
	player_evans.get()->addVertex((scale_factor_x * 55.753), (scale_factor_y * 62.380));
	player_evans.get()->addVertex((scale_factor_x * -66.247), (scale_factor_y * 53.380));
	player_evans.get()->rotateDeg(150, { 1, 0, 0 });
	player_evans.get()->setPhysics(0.5,0.2,0.4);
	player_evans.get()->create(world);
	player_evans.get()->setPosition(scale_factor_x * (x - 20), scale_factor_y * (y - 80));
}

void Player::initializeEvansBodyParts(){
	player_evans = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	torso_1 = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	torso_2 = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	torso_3 = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	upper_arm_L= ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	upper_arm_R = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	lower_arm_L = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	lower_arm_R = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	upper_leg_L = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	upper_leg_R = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	lower_leg_L = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	lower_leg_R = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
}

void Player::initializeEvansStaticBodyParts() {
	player_evans = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
	torso_1_s = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	torso_2_s = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	torso_3_s = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	upper_arm_L_s = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	upper_arm_R_s = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	lower_arm_L_s = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	lower_arm_R_s = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	upper_leg_L_s = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	upper_leg_R_s = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	lower_leg_L_s = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	lower_leg_R_s = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
}

void Player::createEvansBodyParts(b2World *world, float x, float y) {
	createEvansBodyParts(world, x, y, 1.0f);
}

void Player::createEvansBodyParts(b2World *world, float x, float y, float scale_factor) {
	createEvansBodyParts(world, x, y, scale_factor, scale_factor);
}

void Player::createEvansBodyParts(b2World *world, float x, float y, float scale_factor_x, float scale_factor_y) {
	body_parts.clear();
	createEvansBodyPart(torso_1, world, scale_factor_x * x, scale_factor_y * (y + 25.0f), scale_factor_x *15.0f, scale_factor_y * 10.0f);
	createEvansBodyPart(torso_2, world, scale_factor_x * x, scale_factor_y * (y + 43.0f), scale_factor_x * 15.0f, scale_factor_y * 10.0f);
	createEvansBodyPart(torso_3, world, scale_factor_x * x, scale_factor_y * (y + 58.0f), scale_factor_x *15.0f, scale_factor_y * 10.0f);

	createEvansBodyPart(upper_arm_L, world, scale_factor_x * (x - 30.0f), scale_factor_y * (y + 20.0f), scale_factor_x * 18.0f, scale_factor_y * 6.5f);
	createEvansBodyPart(upper_arm_R, world, scale_factor_x * (x + 30.0f), scale_factor_y * (y + 20.0f), scale_factor_x * 18.0f, scale_factor_y * 6.5f);
	createEvansBodyPart(lower_arm_L, world, scale_factor_x * (x - 57.0f), scale_factor_y * (y + 20.0f), scale_factor_x * 17.0f, scale_factor_y * 6.0f);
	createEvansBodyPart(lower_arm_R, world, scale_factor_x * (x + 57.0f), scale_factor_y * (y + 20.0f), scale_factor_x * 17.0f, scale_factor_y * 6.0f);

	createEvansBodyPart(upper_leg_L, world, scale_factor_x * (x - 8.0f), scale_factor_y * (y + 85.0f), scale_factor_x * 7.5f, scale_factor_y * 22.0f);
	createEvansBodyPart(upper_leg_R, world, scale_factor_x * (x + 8.0f), scale_factor_y * (y + 85.0f), scale_factor_x * 7.5f, scale_factor_y * 22.0f);
	createEvansBodyPart(lower_leg_L, world, scale_factor_x * (x - 8.0f), scale_factor_y * (y + 120.0f), scale_factor_x * 6.0f, scale_factor_y * 20.0f);
	createEvansBodyPart(lower_leg_R, world, scale_factor_x * (x + 8.0f), scale_factor_y * (y + 120.0f), scale_factor_x * 6.0f, scale_factor_y * 20.0f);
}
void Player::createEvansBodyPart(ofPtr<ofxBox2dPolygon> shape, b2World *world, float pos_x, float pos_y, float width, float height) {
	shape.get()->addVertex(width, height);
	shape.get()->addVertex(0, height);
	shape.get()->addVertex(0, 0);
	shape.get()->addVertex(width, 0);
	shape.get()->create(world);
	shape.get()->setPhysics(0.5, 0.2, 0.4);
	shape.get()->create(world);
	shape.get()->setPosition(pos_x, pos_y);
	body_parts.push_back(shape);
}

void Player::createEvansStaticBodyParts(b2World *world, float pos_x, float pos_y) {
	torso_1_s.get()->setup(world, pos_x, pos_y + 25.0f, 15.0f, 10.0f);
	torso_2_s.get()->setup(world, pos_x, pos_y + 43.0f, 15.0f, 10.0f);
	torso_3_s.get()->setup(world, pos_x, pos_y + 58.0f, 15.0f, 10.0f);
	upper_arm_L_s.get()->setup(world, pos_x - 30.0f, pos_y + 20.0f, 18.0f, 6.5f);
	upper_arm_R_s.get()->setup(world, pos_x + 30.0f, pos_y + 20.0f, 18.0f, 6.5f);
	lower_arm_L_s.get()->setup(world, pos_x - 57.0f, pos_y + 20.0f, 17.0f, 6.0f);
	lower_arm_R_s.get()->setup(world, pos_x + 57.0f, pos_y + 20.0f, 18.0f, 6.5f);
	upper_leg_L_s.get()->setup(world, pos_x - 8.0f, pos_y + 85.0f, 7.5f, 22.0f);
	upper_leg_R_s.get()->setup(world, pos_x + 8.0f, pos_y + 85.0f, 7.5f, 22.0f);
	lower_leg_L_s.get()->setup(world, pos_x - 8.0f, pos_y + 120.0f, 6.0f, 20.0f);
	lower_leg_R_s.get()->setup(world, pos_x + 8.0f, pos_y + 120.0f, 6.0f, 20.0f);

	//pushback body parts to a vector for the times when it doesn't matter which body part you use
	//and you just want to modify them all with a for loop
	body_parts_s.clear();
	body_parts_s.push_back(torso_1_s);
	body_parts_s.push_back(torso_2_s);
	body_parts_s.push_back(torso_3_s);
	body_parts_s.push_back(upper_arm_L_s);
	body_parts_s.push_back(upper_arm_R_s);
	body_parts_s.push_back(lower_arm_L_s);
	body_parts_s.push_back(lower_arm_R_s);
	body_parts_s.push_back(upper_leg_L_s);
	body_parts_s.push_back(upper_leg_R_s);
	body_parts_s.push_back(lower_leg_L_s);
	body_parts_s.push_back(lower_leg_R_s);
}

void Player::setEvansPhysics(float density, float bounce, float friction) {
	setEvansDensity(density);
	setEvansBounce(bounce);
	setEvansFriction(friction);
}

void Player::setEvansDensity(float density) {
	player_evans.get()->setDensity(density);
	for (int i = 0; i < body_parts.size(); i++) {
		body_parts[i].get()->setDensity(density);
	}
	current_density = density;
}

void Player::setEvansFriction(float friction) {
	player_evans.get()->setFriction(friction);
	for (int i = 0; i < body_parts.size(); i++) {
		body_parts[i].get()->setFriction(friction);
	}
	current_friction = friction;
}

void Player::setEvansBounce(float bounce) {
	player_evans.get()->setBounce(bounce);
	for (int i = 0; i < body_parts.size(); i++) {
		body_parts[i].get()->setBounce(bounce);
	}
	current_bounce = bounce;
}

void Player::setEvansFace(Evans_face expression) {
	current_expression = expression;
}

float Player::ptm(float d) {
	//Box2d uses pixels to meters, but the template of the ragdoll body I am using uses pixels
	//so conversion is necessary.
	float PTM_RATIO = 32;
	return d / PTM_RATIO;
}

vector<ofPtr<ofxBox2dPolygon>> Player::getEvansBody() {
	return body_parts;
}

vector<ofPtr<ofxBox2dRect>> Player::getEvansStaticBody() {
	return body_parts_s;
}

ofPtr<ofxBox2dPolygon> Player::getEvansHead() {
	return player_evans;
}

ofVec2f Player::getEvansVelocity() {
	return player_evans.get()->getVelocity();
}

float Player::getEvansSpeed() {
	float x = player_evans.get()->getVelocity().x;
	float y = player_evans.get()->getVelocity().y;
	return sqrt(pow(x, 2) + pow(y, 2));
}

Evans_face Player::getEvansFace(){
	return current_expression;
}

float Player::getEvansFriction() {
	return current_friction;
}

float Player::getEvansBounce() {
	return current_bounce;
}

float Player::getEvansDensity() {
	return current_density;
}

bool Player::isBodyStatic() {
	return is_body_static;
}

b2Vec2 Player::getPosition() {
	//torso_1_s is used as an arbitrary body part to get the position
	if (is_body_static) {
		float pos_x = torso_1_s.get()->getPosition().x;
		float pos_y = torso_1_s.get()->getPosition().y;
		return b2Vec2(pos_x, pos_y);
	}
	float pos_x = torso_1.get()->getPosition().x;
	float pos_y = torso_1.get()->getPosition().y;
	return b2Vec2(pos_x, pos_y);
}

void Player::destroyJoints() {
	if (is_body_static) {
		is_body_static = false;
		return;
	}
	head_shoulders.destroyJoint();
	shoulder_R.destroyJoint();
	shoulder_L.destroyJoint();
	spine_top_2_mid.destroyJoint();
	spine_mid_2_bottom.destroyJoint();
	elbow_L.destroyJoint();
	elbow_R.destroyJoint();
	knee_L.destroyJoint();
	knee_R.destroyJoint();
	hip_R.destroyJoint();
	hip_L.destroyJoint();
}

void Player::destroyEvans() {
	player_evans.get()->destroy();
	if (is_body_static) {
		for (int i = 0; i < body_parts_s.size(); i++) {
			body_parts_s[i].get()->destroy();
		}
	} else {
		for (int i = 0; i < body_parts.size(); i++) {
			body_parts[i].get()->destroy();
		}
	}
}