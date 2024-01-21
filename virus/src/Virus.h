#pragma once
#include "ofMain.h"

using namespace glm;

class Virus {
public:
    // Constructor
    Virus(vec3 virusOnePos, vec3 virusTwoPos, ofColor _sphereColor, ofColor _sphereCageColor);
    
    // Copy constructor
    Virus(const Virus& v); // Const means we won't modify the original
    ~Virus(); // destructor
    void draw();

    // Update the Virus based on flocking behavior and other parameters
    void update(vector<Virus>& _sample, float speedlimit, float separationFactor, float cohesionFactor, float alignmentFactor, float checkdist, float boundradius, float bouncespeed, float redColour, float greenColour, float blueColour);

    // Get the position of the Virus (position of the first Virus)
    vec3 getPosition();

    // Get the velocity of the Virus
    vec3 getVelocity();

private:
    // Check if the Virus is within the boundary and adjust its position
    void checkBoundary(float speed, float size);

    // Implement flocking behavior for the Virus
    void flock(vector<Virus>& _sample, float separationFactor, float cohesionFactor, float alignmentFactor);

  
    vec3 velocity;
    float distToCheck;

    // Materials for coloring the Viruses
    ofMaterial virusOneMaterial;
    ofMaterial virusTwoMaterial;

    ofIcoSpherePrimitive virusOne;
    ofIcoSpherePrimitive virusTwo;

    ofColor virusOneColor;
    ofColor virusTwoColor;
};
