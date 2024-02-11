#include <sndfile.hh>
#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    const int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    const int channels = 2;
    const int sampleRate = 48000;
    const char* outfilename = "B3.wav";
    const int frequency1 = 1350;
    const int frequency2 = 1600;
    const int frequency3 = 1850;
    const int frequencyFront1 = 2300;
    const int frequencyFront2 = 2400;
    const int frequencyFront3 = 2500;
    const int frequencyBack1 = 2000;
    const int frequencyBack2 = 2100;
    const int frequencyBack3 = 2200;

    cout << "wav_writer..." << endl;

    SndfileHandle outfile(outfilename, SFM_WRITE, format, channels, sampleRate);
    if (not outfile) return -1;

    const int size = sampleRate * 1;
    float sample[size * channels];  
    float factor = 1.0;

    for (int i = 0; i < size; i++) {
        // frequency is changed based on project requirements

        // sample[2 * i] = 0.0; // Left channel to zero
        // sample[2 * i + 1] = sin(float(i) / size * 3.14 * frequency1) * factor;  // Right channel freq1
        // sample[2 * i + 1] = 0.0; // Right channel to zero
        // sample[2 * i] = sin(float(i) / size * 3.14 * frequency1) * factor;  // Left channel freq1
        
        // sample[2 * i] = 0.0; // Left channel to zero
        // sample[2 * i + 1] = sin(float(i) / size * 3.14 * frequency2) * factor;  // Right channel freq2
        // sample[2 * i + 1] = 0.0; // Right channel to zero
        // sample[2 * i] = sin(float(i) / size * 3.14 * frequency2) * factor;  // Left channel freq2

        // sample[2 * i] = 0.0; // Left channel to zero
        // sample[2 * i + 1] = sin(float(i) / size * 3.14 * frequency3) * factor;  // Right channel freq3
        // sample[2 * i + 1] = 0.0; // Right channel to zero
        // sample[2 * i] = sin(float(i) / size * 3.14 * frequency3) * factor;  // Left channel freq3

        // sample[i] = sin(float(i) / size * 3.14 * frequencyFront1) * factor; // Both sides front1
        // sample[i] = sin(float(i) / size * 3.14 * frequencyBack1) * factor; // Both sides back1

        // sample[i] = sin(float(i) / size * 3.14 * frequencyFront2) * factor; // Both sides front2
        // sample[i] = sin(float(i) / size * 3.14 * frequencyBack2) * factor; // Both sides back2

        // sample[i] = sin(float(i) / size * 3.14 * frequencyFront3) * factor; // Both sides front3
        // sample[i] = sin(float(i) / size * 3.14 * frequencyBack3) * factor; // Both sides back3
    }

    outfile.write(&sample[0], size * channels);

    cout << "done!" << endl;
    return 0;
}
