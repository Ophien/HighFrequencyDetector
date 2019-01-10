/*MIT License

Copyright (c) 2019 Eliezyer Fermino de Oliveira,
                   Ikaro Jesus da Silva beraldo,
                   Alysson Ribeiro da Silva,
                   Cleiton Lopes Aguiar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#ifndef __DETECTOR_H
#define __DETECTOR_H

#include <ProcessorHeaders.h>
#include <stdio.h>			 
#include <iostream>			  
#include <string>			  
#include <vector>
#include <time.h>

#include "DetectorEditor.h" 

class Detector : public GenericProcessor
{
  public:
    Detector();
    ~Detector();
    void process(AudioSampleBuffer &rInContinuousBuffer) override;
    bool enable() override;
    void handleEvent(const EventChannel *rInChannelInfo, const MidiMessage &rInEvent, int sampleNum) override;
    AudioProcessorEditor *createEditor() override;
    void createEventChannels() override;
    void updateSettings() override;

    // calibration function
    void calibrate();
    void detectRipples(std::vector<double> &rInRmsBuffer);
    void sendTtlEvent(int rmsIndex, int val);
    double calculateRms(const float *rInBuffer, int index);

    // utilized _channel to check for high frequencies
    int _outputChannel;
    int _channel;
    int _rmsSize;

    // rms statistics
    std::vector<double> _calibrationRms;
    double _mean;
    double _standardDeviation;

    // detection _threshold
    double _threshold;
    double _thresholdAmp;

    // store processing time in seconds
    double _startTime;
    double _currentTime;
    double _calibrationTime;

    // calibrating flag
    bool _isCalibrating;

    // Event count
    bool _detected;
    bool _detectionEnabled;
    unsigned int _rmsSamplesSinceDetection;
    unsigned int _rmsRefractionCount;

    // event handle variables
    bool _isPluginEnabled;
    char *_pMessageString;
    int _messageSize;

    // ttl event _channel
    EventChannel *_pTtlEventChannel;

    // Editor
    DetectorEditor *_pDetectorEditor;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Detector);
};

#endif
