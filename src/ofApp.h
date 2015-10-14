#pragma once

#include "ofMain.h"

#include "ofxFft.h"    //FFT functionality!
#include "chuck_fft.h" // Chuck FFT functionality?

#include <math.h>

//using boost::tuple;
//typedef vector< tuple<int,int> > tuple_list;

//-----------------------------------------------------------------------------
// Preprocessor definitions
//-----------------------------------------------------------------------------
#define MY_SRATE         44100            // sample rate
#define MY_CHANNELS      2                // number of channels
#define MY_BUFFERHISTORY 100               // number of buffers to save
#define MY_BUFFERSIZE    512              // number of frames in a buffer
#define MY_NBUFFERS      2                // number of buffers latency
#define MY_PIE           3.14159265358979 // for convenience
#define HISTORY_LENGTH   50               // length of history shown on visualizer
#define MAX_SPARKLES     100              // maximum number of sparkles displayed for any one sample
#define NUM_RUBIES       30               // number of ruby frames in the ruby gif

//-----------------------------------------------------------------------------
// My application
//-----------------------------------------------------------------------------
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    
        // Get buffer samples
        void audioIn(float * input, int bufferSize, int nChannels);

        // Our sound stream object
		ofSoundStream soundStream;
    
        vector<float> left;
        vector<float> leftFFT;
    
        float currAvg;
    
    unsigned int sampleNum;
    
        vector <vector<vector<int> > > smooches;
    
        ofImage sparkle;
        ofImage lips;
        ofImage rubies[NUM_RUBIES];
        ofVideoPlayer myPlayer;
    
       vector <vector<float> > wavy;
           int wavyNum;
    
        ofSpherePrimitive earth;
        float earthSomethingAngle;
    
        // FFT stuff from the ofcFFT addon
        ofxFft* fftLeft;      // Our FFT object
        float*  fftLeftOutput;
        ofxFft* fftRight;      // Our FFT object
        float*  fftRightOutput; // FFT output
//
        // FFT stuff for Chuck_FFT
        complex fftBuffer[MY_BUFFERSIZE/2];
        float my_window[MY_BUFFERSIZE]; // window to use with FFT
    
};
