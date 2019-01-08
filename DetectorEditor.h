/*MIT License

Copyright (c) 2019 Alysson Ribeiro da Silva,
                   Eliezyer Fermino de Oliveira,
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

#ifndef __DETECTOR_EDITOR_H
#define __DETECTOR_EDITOR_H

#include <EditorHeaders.h> 

class DetectorUi : public Component, public ButtonListener, public SliderListener, public ComboBoxListener
{
  public:
    DetectorUi();
    ~DetectorUi();
    void updateChannel(int channelCount);
    void updateSettings();
    void resized() override;
    void paint(Graphics &rInGraphics) override;
    void buttonClicked(Button *pInButtonClicked) override;
    void sliderValueChanged(Slider *pInSliderChanged) override;
    void comboBoxChanged(ComboBox *pInComboBoxChanged) override;

    LookAndFeel_V2 *_defaultLookAndFeel;
    ComboBox *_channelSelection;
    ComboBox *_outChannelSelection;
    Slider *_thresholdAmplitude;
    Slider *_refractoryRmsSamples;
    Slider *_rmsSamples;
    TextButton *_calibrateButton;

    // labels
    Label *_inputChannelLabel;
    Label *_outChannelLabel;
    Label *_thresholdAmpLabel;
    Label *_refractoryRmsSamplesLabel;
    Label *_rmsSamplesLabel;

    // facade
    int _channelCount;
    int _channel;
    int _outChannel;
    bool _calibrate;
    unsigned int _thresholdAmp;
    unsigned int _rmsRefractionCount;
    unsigned int _rmsSamplesCount;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DetectorUi);
};

class DetectorEditor : public GenericEditor
{
  public:
    DetectorEditor();
    virtual ~DetectorEditor();
    DetectorEditor(GenericProcessor *rInParentNode, bool useDefaultParameterEditors);
    void updateSettings() override;
    void resized() override;
    DetectorUi _pluginUi;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DetectorEditor);
};

#endif
