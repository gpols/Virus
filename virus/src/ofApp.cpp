#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    // background sound
    noise.load("noise.wav");
    noise.play();
    noise.setVolume(2);
    
    bubbles.load("bubbles.wav");
    warning.load("warning.wav");
    explosion.load("explosion.wav");
    spectrogram.load("spectrogram.wav");
    siren.load("siren.wav");
    
    warningSoundPlaying = false;
    explosionSoundPlaying = false;
    bubbleSoundPlaying = false;
    spectrogramSoundPlaying = false;
    gameOverSoundPlayed = false;
    gameOver = false;
    
    // GUI
    gui.setup();
    gui.setup("Chemical Reagents", "settings.xml", 100, ofGetHeight() - 420);
    
    font.load("neuropol.otf", 13);
    smallFont.load("BlockKie.ttf", 15);
    
    // A higher value leads to stronger separation, causing viruses to maintain more distance from each other.
    gui.add(slider_separation.setup("Sodium Chloride", 0.8, 0, 5, 200, 20));
    
    // Colour blue channel
    gui.add(slider_bChannel.setup("Benzene", 0.2, 0, 2, 200, 20));
    
    // Higher values result in faster movement.
    gui.add(slider_speedlimit.setup("Potassium Nitrate", 2.375, 1, 20, 200, 20));
    
    // Higher values lead to stronger cohesion, causing viruses to move towards the average position of their neighbors.
    gui.add(slider_cohesion.setup("Hydrogen Peroxide", 0.57, 0.5, 1, 200, 20));
    
    // Colour green channel
    gui.add(slider_gChannel.setup("Chloroform", 0.3, 0, 1, 200, 20));
    
    // Higher values lead to more alignment, causing viruses to move in the same direction as their neighbors.
    gui.add(slider_alignment.setup("Sulfuric Acid", 0.815, 0, 1, 200, 20));
    
    // A higher value increases the range of interaction between viruses.
    gui.add(slider_checkdist.setup("Ammonium Nitrate", 54, 10, 60, 200, 20));
    
    // The visual impact depends on how the R channel is utilized in the rendering or behavior of viruses.
    gui.add(slider_rChannel.setup("Acetone", 0.5, 0, 1, 200, 20));
    
    // The simulation checks if the virus has spread beyond this radius for determining neutralization.
    gui.add(slider_radius.setup("Ethanol", 443, 50, 500, 200, 20));
    
    // Higher values result in more pronounced bouncing.
    gui.add(slider_bounce.setup("Methanol", 14.086, 0.1, 100, 200, 20));
    
    
    // set 3d environment
    ofSetBackgroundColor(0);
    ofEnableNormalizedTexCoords();
    light.setPointLight();
    light.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
    light.setAmbientColor(ofFloatColor(0.3, 0.3, 0.3));
    light.setPosition(ofGetWidth() * 0.5, ofGetHeight() * 0.5, 500);
    
    for (int i = 0; i < 100; i++) {
        vec3 virusOnePos = vec3(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10));
        vec3 virusTwoPos = vec3(ofRandom(-5, 5), ofRandom(-5, 5), ofRandom(-5, 5));
        ofColor virusOneColor = ofColor(slider_rChannel * 100, slider_gChannel * 50, slider_bChannel * 50);
        ofColor virusTwoColor = ofColor(slider_rChannel * 50, slider_gChannel * 100, slider_bChannel * 100);
        Virus v{virusOnePos, virusTwoPos, virusOneColor, virusTwoColor};
        sample.push_back(v);
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    if (gameStarted && !virusNeutralized) {
        
        for (int i = 0; i < sample.size(); i++) {
            sample[i].update(sample, slider_speedlimit, slider_separation, slider_cohesion, slider_alignment, slider_checkdist, slider_radius, slider_bounce, slider_rChannel, slider_gChannel, slider_bChannel);
            
            
        }
        
        drawAlertMessages();
        
        checkAndPlayWarningSound();
        
        checkAndPlayBubbleSound();
        
        checkAndPlaySpectrogramSound();
        
        countToExplosionSound();
    }
    
    checkGameOver();
    
    if (slider_radius > 450) {
        virusNeutralized = true;
        noise.stop();
        bubbles.stop();
        warning.stop();
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    // Draw the 3D scene
    ofEnableDepthTest();
    cam.begin();
    ofEnableLighting();
    light.enable();
    
    // Do 3D drawing here
    for (int i = 0; i < sample.size(); i++) {
        sample[i].draw();
    }
    
    light.disable();
    ofDisableLighting();
    cam.end();
    ofDisableDepthTest();
    
    if (!gameStarted) {
        
        drawInitialMessage();
        
    } else if (gameStarted) {
        
        ofSetBackgroundAuto(false);
        gui.draw();
    }
    
    if (virusNeutralized) {
        ofSetColor(0, 0, 255);
        font.drawString("Virus Neutralized", 570, 420);
        
    }
    
    if (gameOver) {
        ofSetColor(0, 0, 255);
        font.drawString("You killed everyone", 570, 420);
    }
    cout << to_string(ofGetFrameRate()) << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == ' ') {
        gameStarted = true;
    }
}


void ofApp::checkAndPlayWarningSound() {
    
    // Check if speed limit is higher than 8 and the warning sound is not already playing
    if (slider_speedlimit >= 8 && !warningSoundPlaying) {
        // Play warning sound in a loop
        warning.setLoop(true);
        warning.play();
        // Set the flag to true to indicate that the sound is now playing
        warningSoundPlaying = true;
        
    } else if (slider_speedlimit < 6) {
        // Stop warning sound if speed limit goes below 4
        warning.stop();
        // Reset the flag to indicate that the sound is not playing
        warningSoundPlaying = false;
    }
}

void ofApp::checkAndPlayBubbleSound(){
    
    if ((slider_gChannel > 0.4  || slider_bChannel > 0.3 || slider_checkdist > 55 || slider_radius > 500 )&& !bubbleSoundPlaying) {
        bubbles.play();
        bubbleSoundPlaying = true;
        
    }
}
void ofApp::checkAndPlaySpectrogramSound(){
    
    if((slider_separation > 1 || slider_rChannel < 0.5 || slider_cohesion < 0.5) && !spectrogramSoundPlaying){
        
        spectrogram.play();
        spectrogramSoundPlaying = true;
    }
}
void ofApp::countToExplosionSound(){
    
    if (ofGetElapsedTimeMillis() > 50000 && !explosionSoundPlaying && gameStarted) {
        // Play explosion sound and stop other sounds
        explosion.play();
        explosion.setVolume(0.2);  // Adjust the starting volume as needed
        // Set the flag to true to indicate that the explosion sound is now playing
        explosionSoundPlaying = true;
    }
    
    if (ofGetElapsedTimeMillis() > 55000 && gameStarted) {
        gameOver = true;
    }
    
}

void ofApp::checkGameOver() {
    
    // Check if the game over sound has not been played
    if (gameOver && !gameOverSoundPlayed) {
        // Stop background sound and play the game over sound
        noise.stop();
        siren.play();
        
        // Set the flag to true to indicate that the game over sound has been played
        gameOverSoundPlayed = true;
    }
}
void ofApp::drawAlertMessages(){
    
    ofSetColor(0, 10);
    ofDrawRectangle(40, 120, 380, 500);
    
    if (gameStarted){
        
        speedAlert = "The clock is ticking!";
        ofSetColor(100);
        font.drawString(speedAlert, 100, 150);
        
    }
    
    if(slider_bChannel > 0.4){
        
        benzeneAlert = "Caution: High levels of benzene detected";
        ofSetColor(100);
        smallFont.drawString(benzeneAlert , 50, 200);
        
    } else {
        
        benzeneAlert = "Caution: High levels of benzene detected";
        ofSetColor(0);
        smallFont.drawString(benzeneAlert , 50, 200);
        
    }
    
    if (slider_radius < 400){
        
        ethanolAlert = "The virus is showing signs of vulnerability";
        ofSetColor(100);
        smallFont.drawString(ethanolAlert, 50, 250);
        
    } else {
        
        ethanolAlert = "The virus is showing signs of vulnerability";
        ofSetColor(0);
        smallFont.drawString(ethanolAlert, 50, 250);
    }
    
    if (slider_speedlimit >= 8){
        
        potassiumNitrateAlert = "Emergency: Virus spreading rapidly";
        ofSetColor(100);
        smallFont.drawString(potassiumNitrateAlert, 50, 300);
        
    } else {
        
        potassiumNitrateAlert = "Emergency: Virus spreading rapidly.";
        ofSetColor(0);
        smallFont.drawString(potassiumNitrateAlert, 50, 300);
    }
    
}

void ofApp::drawInitialMessage(){
    
    // Display narrative message in the center of the window
    ofSetColor(0);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    alertMessage = "Emergency Alert";
    message = "Alien Virus Outbreak on Spaceship!\n\n";
    message += "As the ship's alarms wail, an alien virus threatens humanity's existence.\n\n";
    message += "You, the virologist, are our last hope.\n\n";
    message += "To neutralize the virus, find the right chemical solution.\n\n";
    message += "Time is short, survival is in your hands...\n\n";
    pressBar = "Press space bar to start";
    
    alertBox = font.getStringBoundingBox(alertMessage, 0, 0);
    x = ofGetWidth() * 0.5 - alertBox.width * 0.5;
    y = ofGetHeight() * 0.5 - alertBox.height * 0.5;
    
    ofSetColor(0, 0, 200);
    font.drawString(alertMessage, x - 320, y - 120);
    
    messageBox = font.getStringBoundingBox(message, 0, 0);
    x = ofGetWidth() * 0.5 - messageBox.width * 0.5;
    y = ofGetHeight() * 0.5 - messageBox.height * 0.5;
    
    ofSetColor(255);
    font.drawString(message, x, y);
    
    pressBarBox = font.getStringBoundingBox(pressBar, 0, 0);
    x = ofGetWidth() * 0.5 - pressBarBox.width * 0.5;
    y = ofGetHeight() * 0.5 - pressBarBox.height * 0.5;
    
    ofSetColor(0, 0, ofRandom(255));
    font.drawString(pressBar, x, y + 180);
}
