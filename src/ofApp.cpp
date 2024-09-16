#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
	cam.initGrabber(320, 240);//カメラ初期設定
	ofSetFrameRate(60);//フレームレート固定
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	gui.setup();
	gui.add(pyrScale.setup("pyrScale", .5, 0, 1));
	gui.add(levels.setup("levels", 4, 1, 8));
	gui.add(winSize.setup("winsize", 12, 4, 64));
	gui.add(iterations.setup("iterations", 2, 1, 8));
	gui.add(polyN.setup("polyN", 7, 5, 10));
	gui.add(polySigma.setup("polySigma", 1.5, 1.1, 2));
	curFlow = &farneback;

	gui.add(flowScale.setup("flowScale", 0.05, 0.0, 0.2));
	gui.add(particleFriction.setup("particleFriction", 0.01, 0.0, 0.5));

	for (int i = 0; i < NUM; i++) {
		Particle particle;
		particle.friction = 0.01;
		particle.position = glm::vec2(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
		particles.push_back(particle);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	cam.update();
	if (cam.isFrameNew()) {
		farneback.setPyramidScale(pyrScale);
		farneback.setNumLevels(levels);
		farneback.setWindowSize(winSize);
		farneback.setNumIterations(iterations);
		farneback.setPolyN(polyN);
		farneback.setPolySigma(polySigma);
		farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
		curFlow->calcOpticalFlow(cam);
	}

	for (int i = 0; i < particles.size(); i++) {
		glm::vec2 force = glm::vec2(0, 0);
		glm::vec2 pos;
		pos.x = particles[i].position.x * float(cam.getWidth()) / float(ofGetWidth());
		pos.y = particles[i].position.y * float(cam.getHeight()) / float(ofGetHeight());
		particles[i].friction = particleFriction;
		if (pos.x > 0 && pos.y > 0 && pos.x < cam.getWidth() && pos.y < cam.getHeight()) {
			force = farneback.getFlowOffset(pos.x, pos.y) * glm::vec1(flowScale);
		}

		particles[i].addForce(glm::vec2(force.x, force.y));
		particles[i].update();
		particles[i].throughOffWalls();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	ofSetColor(255);
	
	cam.draw(0, 0, ofGetWidth(), ofGetHeight());
	
	curFlow->draw(0, 0, ofGetWidth(), ofGetHeight());

	for (int i = 0; i < particles.size(); i++) {
		//各パーティクルの描画
		ofSetColor(52, 251, 254);
		ofCircle(particles[i].position.x, particles[i].position.y, particles[i].radius * 1.5);
	}

	gui.draw();
}

