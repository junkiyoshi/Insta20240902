#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_MULTIPLY);
}

//--------------------------------------------------------------
void ofApp::update() {

	int base_radius = (ofGetFrameNum() * 2) % 420 + 60;

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		if (glm::length(this->log_list[i].back()) > 720) {

			this->log_list.erase(this->log_list.begin() + i);
			this->seed_list.erase(this->seed_list.begin() + i);
			this->size_list.erase(this->size_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);

			continue;
		}

		auto feature = this->velocity_list[i] * 100;
		auto deg = ofMap(ofNoise(this->seed_list[i], ofGetFrameNum() * 0.005), 0, 1, -360, 360);
		feature += glm::vec2(30 * cos(deg * DEG_TO_RAD), 30 * sin(deg * DEG_TO_RAD));
		auto next = this->log_list[i].back() + glm::normalize(feature) * 10;

		this->log_list[i].push_back(next);

		while (this->log_list[i].size() > 10) {
			this->log_list[i].erase(this->log_list[i].begin());
		}
	}

	ofColor color;
	for (int i = 0; i < 4; i++) {

		int deg = ofGetFrameNum() * 1.44 + (i % 4) * 90;
		int next_deg = deg + 1;
		float radius = ofRandom(145, 155);

		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		auto next = glm::vec2(radius * cos(next_deg * DEG_TO_RAD), radius * sin(next_deg * DEG_TO_RAD));

		vector<glm::vec2> log;
		log.push_back(location);
		this->log_list.push_back(log);

		this->seed_list.push_back(ofMap(ofNoise(location * 0.001), 0, 1, 0, 1000));
		this->size_list.push_back(5);

		this->velocity_list.push_back(glm::normalize(location - next));

		color.setHsb(ofRandom((i % 4) * 50, 50 + (i % 4) * 50), 180, 255);
		this->color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	for (int i = 0; i < this->log_list.size(); i++) {

		ofFill();
		ofSetColor(this->color_list[i], 200);
		ofDrawCircle(this->log_list[i].back(), this->size_list[i]);

		ofColor tmp_color;
		tmp_color.setHsb(this->color_list[i].getHue(), 255, 255);

		ofNoFill();
		ofSetColor(tmp_color);
		ofDrawCircle(this->log_list[i].back(), this->size_list[i]);

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	/*
	int start = 50;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}