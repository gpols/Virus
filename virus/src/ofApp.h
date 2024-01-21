#pragma once

#include "ofMain.h"
#include "Virus.h"
#include "ofxGui.h"
using namespace glm;

class ofApp : public ofBaseApp {
public:
    // Standard OpenFrameworks methods
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

private:
    // Custom methods for better organization
    void checkAndPlayWarningSound();       // Check and play the warning sound
    void checkAndPlayBubbleSound();        // Check and play the bubble sound
    void countToExplosionSound();          // Count 50 sec and play the explosion sound
    void checkAndPlaySpectrogramSound();
    void drawInitialMessage();            // draw story and instructions
    void drawAlertMessages();
    void checkGameOver();
  
    
    // 3D rendering elements
    ofLight light;
    ofEasyCam cam;
    vector<Virus> sample;
    
    // GUI elements
    ofxPanel gui;
    ofxToggle toggle_axis;
    ofxToggle toggle_grid;
    
    // Sliders for simulation parameters
    ofxFloatSlider slider_speedlimit;
    ofxFloatSlider slider_separation;
    ofxFloatSlider slider_cohesion;
    ofxFloatSlider slider_alignment;
    
    ofxFloatSlider slider_radius;
    ofxFloatSlider slider_checkdist;
    ofxFloatSlider slider_bounce;
    
    ofxFloatSlider slider_rChannel;
    ofxFloatSlider slider_gChannel;
    ofxFloatSlider slider_bChannel;
    
    // Booleans for game state and sound status
    bool gameStarted;
    bool gameOver;
    bool virusNeutralized;
    bool warningSoundPlaying;
    bool explosionSoundPlaying;
    bool bubbleSoundPlaying;
    bool spectrogramSoundPlaying;
    bool spaceBarPressed;
    bool gameOverSoundPlayed;
    
    // Font for text rendering
    ofTrueTypeFont font;
    ofTrueTypeFont smallFont;
    
    // Sound players for different game sounds
    ofSoundPlayer noise;
    ofSoundPlayer warning;
    ofSoundPlayer bubbles;
    ofSoundPlayer explosion;
    ofSoundPlayer spectrogram;
    ofSoundPlayer siren;
    
    // Strings for in-game messages
    string alertMessage;
    string message;
    string pressBar;
    string speedAlert;
    string benzeneAlert;
    string ethanolAlert;
    string potassiumNitrateAlert;
    
    // Rectangles for positioning text
    ofRectangle messageBox;
    ofRectangle alertBox;
    ofRectangle pressBarBox;
    
    // Floats for positioning text
    float x;
    float y;   
};
