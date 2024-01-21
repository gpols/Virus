#include "Virus.h"

// Constructor
Virus::Virus(vec3 virusOnePos, vec3 virusTwoPos, ofColor _virusOneColor, ofColor _virusTwoColor) {
    cout << "Constructed a Virus" << endl;

    virusOneColor = _virusOneColor;
    virusTwoColor = _virusTwoColor;

    // the small number for of random position is for a closer drawing of the sphere
    virusOne.set(ofRandom(0.5, 5.0), ofRandom(0.5, 5.0));
    virusOne.setPosition(virusOnePos);
    virusTwo.setPosition(virusTwoPos);
    virusTwo.set(ofRandom(0.5, 5.0), ofRandom(0.5, 5.0));
}

// Copy constructor
Virus::Virus(const Virus& v) {
    cout << "Copied a Virus" << endl;

    velocity = v.velocity;
    virusOne = v.virusOne;
    virusTwo = v.virusTwo;
    virusOneColor = v.virusOneColor;
    virusTwoColor = v.virusTwoColor;
    virusOneMaterial = v.virusOneMaterial;
    virusTwoMaterial = v.virusTwoMaterial;
}

// Destructor
Virus::~Virus() {
    cout << "Destructed a Virus" << endl;
}

// Update function for each Virus
void Virus::update(vector<Virus>& _sample, float speedlimit, float separationFactor, float cohesionFactor, float alignmentFactor, float checkdist, float boundradius, float bouncespeed, float redColour, float greenColour, float blueColour) {
    // Set the base distance for flocking nearest neighbors
    distToCheck = checkdist;

    // Keep Viruses in a small area by checking the boundary
    checkBoundary(bouncespeed, boundradius);

    // Add flocking behavior
    flock(_sample, separationFactor, cohesionFactor, alignmentFactor);

    // Limit how fast a Virus can go
    if (length(velocity) > speedlimit) {
        velocity = normalize(velocity) * speedlimit;
    }

    // Move the Viruses
    virusOne.move(velocity);
    virusTwo.move(velocity);

    // Normalize color values to the range [0, 1]
    float r = ofClamp(redColour, 0, 1);
    float g = ofClamp(greenColour, 0, 1);
    float b = ofClamp(blueColour, 0, 1);

    // Set the colors with the normalized values
    virusOneColor = ofColor(r * 150, g * 100, b * 100, ofRandom(30, 150));
    virusTwoColor = ofColor(r * 100, g * 150, b * 100, ofRandom(30, 150));

    // Apply the new colors to the materials
    virusOneMaterial.setDiffuseColor(virusOneColor);
    virusTwoMaterial.setDiffuseColor(virusTwoColor);
}

// Draw function for each Virus
void Virus::draw() {
    // Begin drawing for the first Virus
    virusOneMaterial.begin();
    virusOne.draw();
    virusOneMaterial.end();

    // Begin drawing for the second Virus
    virusTwoMaterial.begin();
    virusTwo.draw();
    virusTwoMaterial.end();
}

// Flocking behavior for each Virus
void Virus::flock(vector<Virus>& _sample, float separationFactor, float cohesionFactor, float alignmentFactor) {
    vec3 separation = vec3(0, 0, 0);
    vec3 cohesion = vec3(0, 0, 0);
    vec3 align = vec3(0, 0, 0);
    int numCohesionNeighbours = 0;
    int numAlignNeighbours = 0;

    // Query the entire flock
    for (int i = 0; i < _sample.size(); i++) {
        float dist = distance(getPosition(), _sample[i].getPosition());

        if (dist < distToCheck) {
            // It's close enough >> add a difference vector
            separation -= (_sample[i].getPosition() - getPosition());
        }

        if (dist < distToCheck + 20 && dist > 10) {
            // It's close enough >> add its position to the average
            cohesion += _sample[i].getPosition();
            numCohesionNeighbours++;
        }

        if (dist < distToCheck + 10 && dist > 10) {
            // It's close enough >> add its velocity to the average
            align += _sample[i].getVelocity();
            numAlignNeighbours++;
        }
    }

    // Calculate separation force
    separation *= separationFactor;
    velocity += separation;

    // Calculate cohesion force
    if (numCohesionNeighbours == 0) {
        cohesion = vec3(0, 0, 0);
    } else {
        cohesion /= numCohesionNeighbours;
        cohesion -= getPosition();
        cohesion *= cohesionFactor;
    }

    velocity += cohesion;

    // Calculate alignment force
    if (numAlignNeighbours == 0) {
        align = vec3(0, 0, 0);
    } else {
        align /= numAlignNeighbours;
        align *= alignmentFactor;

        // This will point the Virus in the direction of the alignment vector
        vec3 lookAhead = normalize(align) * 50;
        virusOne.lookAt(lookAhead);
        virusTwo.lookAt(lookAhead);
    }

    velocity += align;
}

// Check if the distance between the Virus and the center is greater than the radius
// If so, reverse the direction of the Virus
void Virus::checkBoundary(float bouncespeed, float size) {
    vec3 position = getPosition();
    vec3 desired = vec3(0, 0, 0) - position;

    // The size variable is the radius of a Virus around the origin
    if (length(desired) > size) {
        desired = normalize(desired) * bouncespeed;
        velocity = desired - velocity;
    }
}

// Get the position of the Virus (position of the first Virus)
vec3 Virus::getPosition() {
    return virusOne.getPosition();
}

// Get the velocity of the Virus
vec3 Virus::getVelocity() {
    return velocity;
}
