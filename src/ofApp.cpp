#include "ofApp.h"

//--------------------------------------------------------------
// setup -- Setup our application
//--------------------------------------------------------------
void ofApp::setup(){
    // AUDIO...
    
    sparkle.loadImage("sparkle.gif");
    lips.loadImage("lips.png");
    
    for (int i = 0; i < NUM_RUBIES; i++){
        string filename = "rubies/r-"+ofToString(i)+".png";
        cout << ofToString(filename), 10, 15+(i*30);
        ofImage r;
        r.loadImage(filename);
        rubies[i] = r;
    }

    sampleNum = 0;
    
    wavyNum = 0;
    wavy.resize(HISTORY_LENGTH, vector<float>(MY_BUFFERSIZE,0));
    smooches.resize(10, vector <vector<int> >(10, vector<int>(2,0)));
    
    //smooches.resize(HISTORY_LENGTH, vector<int>(2, 0));
    
    // Check out the available audio devices
    soundStream.listDevices();
    
    // Setup the sound stream
    soundStream.setup(this, 0, MY_CHANNELS, MY_SRATE, MY_BUFFERSIZE, MY_NBUFFERS);
    
    // Resize and initialize left and right buffers...
    left.resize( MY_BUFFERSIZE, 0 );
    leftFFT.resize( MY_BUFFERSIZE, 0 );
    
    // FFT! using ofxFFT addon
    fftLeft = ofxFft::create(MY_BUFFERSIZE, OF_FFT_WINDOW_HANN);
    fftLeftOutput = new float[fftLeft->getBinSize()];
    fftRight = ofxFft::create(MY_BUFFERSIZE, OF_FFT_WINDOW_HANN);
    fftRightOutput = new float[fftRight->getBinSize()];
    
    // FFT stuff for Chuck_FFT

    // To begin, set the FFT buffer to zeros
    memset(fftBuffer, 0, sizeof(complex)*MY_BUFFERSIZE/2 );
    
    // Create a (mono) window buffer, for use before FFT
    hamming( my_window, MY_BUFFERSIZE);
    
    // VIDEO...

    // Sync video refresh rate
    ofSetVerticalSync(true);
    
    // Set background color
    ofBackground(0x000000);

    // Smooth edges
    ofEnableSmoothing();
    
}

// star screen saver effect, each star matched to a bin in the fft
// time domain same as sndpeak

//--------------------------------------------------------------
void ofApp::update(){
    
    fftLeft->setSignal(leftFFT);
    memcpy(fftLeftOutput, fftLeft->getAmplitude(), sizeof(float) * fftLeft->getBinSize());
    
    //fftRight->setSignal(right);
    //memcpy(fftRightOutput, fftRight->getAmplitude(), sizeof(float) * fftRight->getBinSize());
    
    // Update the ChucK_FFT -- left channel only
    
    apply_window( &leftFFT[0], my_window, MY_BUFFERSIZE ); // Window the buffer
    
    rfft( &leftFFT[0], MY_BUFFERSIZE/2, FFT_FORWARD ); // Compute the FFT in place
    
    memcpy( fftBuffer, &leftFFT[0], sizeof(complex)*MY_BUFFERSIZE/2); // Copy it
    
    vector<float> curr(fftLeftOutput, fftLeftOutput + (MY_BUFFERSIZE/2));
    wavy.push_back(curr);
    wavy.erase(wavy.begin());
//    
//    float sum = 0;
//    for (unsigned int i = 0; i < fftLeft->getBinSize(); i++){
//        sum += 500*fftLeftOutput[i];
//    }
//    currAvg = sum / left.size();
//    
//    int avgFloor = (int)currAvg / 1;
//    for (int i = 0; i < avgFloor; i++){
//        float origx = rand();
//        float origy = rand();
//        //sparkle.draw(origx, origy);
//        vector<int> vec(2);
//        vec[0] = origx;
//        vec[1] = origy;
//        if (i < MAX_SPARKLES) {
//            sparkles.push_back(vec);
//        }
//    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    int wh = ofGetWindowHeight();
    int ww = ofGetWindowWidth();
    
    float sum = 0;
    float avg = 0;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ofPushStyle();
    ofPushMatrix();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    ofTranslate(-300, -600, -1200);
    
    ofPushStyle();
    ofPushMatrix();
        lips.draw(20,20);
    ofPopMatrix();
    ofPopStyle();
    
    ofTranslate(200, 400, 0);
    
    //img.resize(ww*4,wh*4);
    
    float waveform_amp   = 0.5 * wh;
    float waveform_width = 0.5 * ww;
    
    //ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 15);
    
    int fftBinSize = fftLeft->getBinSize();
    
//    //Draw waveform for left channel
//    ofPushStyle();
//        ofPushMatrix();
//    
//        ofTranslate(ww*0.1, wh * 0.5, 0);
//    
//    ofSetHexColor(0xe5e5e5);
//        ofSetLineWidth(3);
//        ofNoFill();
//    
//        ofBeginShape();
//        for (unsigned int i = 0; i < left.size(); i++){
//            ofVertex(i*waveform_width/left.size(), waveform_amp * left[i], 0);
//            //earth.setRadius(waveform_amp * left[i]+40);
//        }
//        ofEndShape(false); // End line. Don't connect first and last points.
//    
//        ofPopMatrix();
//    ofPopStyle();
    
    // PLOT THE FFT WE CALCULATED FROM ofxFFT ADDON
    
    int colors[] = {0xfbeaf0,0xf8d5e1,0xf5c0d2,0xf1abc3,0xee96b4,
        0xeb81a5,0xe76c96,0xe45687,0xe14278,0xde2d69};
    
        ofPushStyle();
        ofPushMatrix();
        for (int i = (HISTORY_LENGTH-1); i >=0; i--){
            ofSetLineWidth(1);
            ofNoFill();
            
            int binSize = (int)HISTORY_LENGTH/(int)10;
            int bin = i/binSize;
        
            ofSetHexColor(colors[bin]);
            ofTranslate(20, 20, (1200/HISTORY_LENGTH));
            //ofTranslate(50, 50, -100);
            ofBeginShape();
            for (unsigned int j = 0; j < fftBinSize; j++){
                ofVertex( (j/(float)fftBinSize) * 400, -10.0*wh * wavy[i][j], 0);
                sum += wh*fftLeftOutput[i];
            }
            avg = sum / left.size();
            ofEndShape(false); // End line. Don't connect first and last points.

        }
        ofPopMatrix();
        ofPopStyle();
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ofPopMatrix();
    ofPopStyle();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
//    int bin = (int)sampleNum/(int)5;
//    int frame = bin % NUM_RUBIES;
//    //ofDrawBitmapString(ofToString(frame), 10, 15);
//    double width = (double)(avg+50) * 1.87;
//    double height = (double)(avg+50) * 1.42;
//    //double height = (avg+100) *2.83;
//    //ofTranslate(0, 0, z);
//    //rubies[frame].draw(ww*.25-(374/2),wh*.75-(283/2));
//    //rubies[frame].resize(width, height);
//    int xCoordinate = ww*.2;
//    int yCoordinate = wh*.65;
//    rubies[frame].draw(xCoordinate,yCoordinate);
    
    int avgFloor = (int)avg / 1;
    
    for (int i = 0; i < avgFloor; i++){
        ofPushStyle();
        ofPushMatrix();
        
        float origz = rand() % 700;
        
        ofTranslate(0, 0, origz);
        
        float origx = rand() % ww;
        float origy = rand() % wh;
        sparkle.draw(origx, origy);
        //int curr[2] = {origx, origy};
        //smooches.push_back(curr);
        //smooches.erase(smooches.begin());
        
        ofPopMatrix();
        ofPopStyle();
    }
    
//    vector <vector<int> > sparkles(MAX_SPARKLES);
//    for (int i = 0; i < avgFloor; i++){
//        float origx = rand() % ww;
//        float origy = rand() % wh;
//        sparkle.draw(origx, origy);
////        vector<int> vec(2);
////        vec[0] = origx;
////        vec[1] = origy;
////        if (i < MAX_SPARKLES) {
////            sparkles.push_back(vec);
////        }
//    }
//    //smooches.push_back(sparkles);
    sampleNum++;
}


//--------------------------------------------------------------
// audioIn -- deal with incoming audio buffer
//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){

    // Write incoming audio to buffer. Note: samples are interleaved.
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        leftFFT[i]	= input[i*2]*0.5;
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
