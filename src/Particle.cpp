#include "Particle.h"

Particle::Particle() {
	radius = 2.0;
	friction = 0.01;
	mass = 1.0;
	position = glm::vec2(ofGetWidth() / 2.0, ofGetHeight() / 2.0);
	velocity = glm::vec2(0, 0);
	minX = 0;
	minY = 0;
	maxX = ofGetWidth();
	maxY = ofGetHeight();
}

void Particle::update() {
	acceleration -= velocity * friction;
	velocity += acceleration;
	position += velocity;
	acceleration = glm::vec2(0, 0);
}

void Particle::draw() {
	ofDrawCircle(position.x, position.y, radius);
}

void Particle::addForce(glm::vec2 _force) {
	acceleration += _force / mass;
}
void Particle::addForce(float forceX, float forceY) {
	acceleration += glm::vec2(forceX, forceY) / mass;
}

void Particle::bounceOffWalls() {
	if (position.x > maxX) {
		position.x = maxX;
		velocity.x *= -1;
	}
	if (position.x < minX) {
		position.x = minX;
		velocity.x *= -1;
	}
	if (position.y > maxY) {
		position.y = maxY;
		velocity.y *= -1;
	}
	if (position.y < minY) {
		position.y = minY;
		velocity.y *= -1;
	}
}

void Particle::throughOffWalls() {
	if (position.x < minX) {
		position.x = maxX;
	}
	if (position.y < minY) {
		position.y = maxY;
	}
	if (position.x > maxX) {
		position.x = minX;
	}
	if (position.y > maxY) {
		position.y = minY;
	}
}

void Particle::addAttractionForce(float x, float y, float radius, float scale) {
	glm::vec2 posOfForce;
	posOfForce = glm::vec2(x, y);
	glm::vec2 diff = position - posOfForce;
	float length = diff.length();
	bool bAmCloseEnough = true;
	if (radius > 0) {
		if (length > radius) {
			bAmCloseEnough = false;
		}
	}
	if (bAmCloseEnough == true) {
		float pct = 1 - (length / radius);
		normalize(diff);
		acceleration = acceleration - diff * scale * pct;
	}
}

void Particle::addAttractionForce(Particle& p, float radius, float scale) {
	glm::vec2 posOfForce;
	posOfForce = glm::vec2(p.position.x, p.position.y);
	glm::vec2 diff = position - posOfForce;
	float length = diff.length();
	bool bAmCloseEnough = true;
	if (radius > 0) {
		if (length > radius) {
			bAmCloseEnough = false;
		}
	}
	if (bAmCloseEnough == true) {
		float pct = 1 - (length / radius);
		normalize(diff);
		acceleration = acceleration - diff * scale * pct;
	}
}

void Particle::addRepulsionForce(float x, float y, float radius, float scale) {
	glm::vec2 posOfForce;
	posOfForce = glm::vec2(x, y);
	glm::vec2 diff = position - posOfForce;
	float length = diff.length();
	bool bAmCloseEnough = true;
	if (radius > 0) {
		if (length > radius) {
			bAmCloseEnough = false;
		}
	}
	if (bAmCloseEnough == true) {
		float pct = 1 - (length / radius);
		normalize(diff);
		acceleration = acceleration + diff * scale * pct;
	}
}

void Particle::addRepulsionForce(Particle& p, float radius, float scale) {
	glm::vec2 posOfForce;
	posOfForce = glm::vec2(p.position.x, p.position.y);
	glm::vec2 diff = position - posOfForce;
	float length = diff.length();
	bool bAmCloseEnough = true;
	if (radius > 0) {
		if (length > radius) {
			bAmCloseEnough = false;
		}
	}
	if (bAmCloseEnough == true) {
		float pct = 1 - (length / radius);
		normalize(diff);
		acceleration = acceleration + diff * scale * pct;
	}
}