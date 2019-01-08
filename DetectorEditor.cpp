#include "DetectorEditor.h"

#define SPACING 10
#define TOP_MARGIN 30
#define THREE_ELEMENT_SQUARED_SIZE 50

static int getNextX(Component *rInReference)
{
    int next_X = rInReference->getX() + rInReference->getWidth() + SPACING;
    return next_X;
}

static int getNextY(Component *rInReference)
{
    int next_Y = rInReference->getY() + rInReference->getHeight() + SPACING;
    return next_Y;
}

DetectorUi::DetectorUi()
{
    // Set the style of the GUI
    _defaultLookAndFeel = new LookAndFeel_V2();
    setLookAndFeel(_defaultLookAndFeel);

    // init available channels
    _channelSelection = new ComboBox();
    updateChannel(8);
    _channelSelection->addListener(this);
    addAndMakeVisible(_channelSelection);

    _outChannelSelection = new ComboBox();
    _outChannelSelection->addItem("1", 1);
    _outChannelSelection->addItem("2", 2);
    _outChannelSelection->addItem("3", 3);
    _outChannelSelection->addItem("4", 4);
    _outChannelSelection->addItem("5", 5);
    _outChannelSelection->addItem("6", 6);
    _outChannelSelection->addItem("7", 7);
    _outChannelSelection->addItem("8", 8);
    _outChannelSelection->setSelectedId(1);
    _outChannelSelection->addListener(this);
    addAndMakeVisible(_outChannelSelection);

    _thresholdAmplitude = new Slider("Threshold Amp");                         // Create the object and set its name
    _thresholdAmplitude->setRange(0, 1000, 0);                                 // Set the minium value, the max value and the increment interval
    _thresholdAmplitude->setSliderStyle(Slider::Rotary);                       // Set the style of the styler, horizontal or vertical
    _thresholdAmplitude->setTextBoxIsEditable(false);                          // Set the visual style of the slider
    _thresholdAmplitude->setTextBoxStyle(Slider::TextBoxRight, false, 80, 20); // Set the visual style of the slider
    _thresholdAmplitude->setTextValueSuffix(" ");                              // Set the sufix of the value shown at the left of the slider
    _thresholdAmplitude->addListener(this);                                    // Adding a listener, the class that will handle this slider
    addAndMakeVisible(_thresholdAmplitude);                                    // Adding the slider into this component for it to be visible

    _refractoryRmsSamples = new Slider("Refractory samples");                    // Create the object and set its name
    _refractoryRmsSamples->setRange(0, 1000, 0);                                 // Set the minium value, the max value and the increment interval
    _refractoryRmsSamples->setSliderStyle(Slider::Rotary);                       // Set the style of the styler, horizontal or vertical
    _refractoryRmsSamples->setTextBoxIsEditable(false);                          // Set the visual style of the slider
    _refractoryRmsSamples->setTextBoxStyle(Slider::TextBoxRight, false, 80, 20); // Set the visual style of the slider
    _refractoryRmsSamples->setTextValueSuffix(" ");                              // Set the sufix of the value shown at the left of the slider
    _refractoryRmsSamples->addListener(this);                                    // Adding a listener, the class that will handle this slider
    addAndMakeVisible(_refractoryRmsSamples);                                    // Adding the slider into this component for it to be visible

    _rmsSamples = new Slider("RMS samples");                           // Create the object and set its name
    _rmsSamples->setRange(0, 1000, 0);                                 // Set the minium value, the max value and the increment interval
    _rmsSamples->setSliderStyle(Slider::Rotary);                       // Set the style of the styler, horizontal or vertical
    _rmsSamples->setTextBoxIsEditable(false);                          // Set the visual style of the slider
    _rmsSamples->setTextBoxStyle(Slider::TextBoxRight, false, 80, 20); // Set the visual style of the slider
    _rmsSamples->setTextValueSuffix(" ");                              // Set the sufix of the value shown at the left of the slider
    _rmsSamples->addListener(this);                                    // Adding a listener, the class that will handle this slider
    addAndMakeVisible(_rmsSamples);                                    // Adding the slider into this component for it to be visible

    _inputChannelLabel = new Label("label", TRANS("Input _channel"));        // Create the object and set its name
    _inputChannelLabel->setFont(Font(15.10f, Font::plain));                  // Set the font size and style
    _inputChannelLabel->setJustificationType(Justification::centredLeft);    // Set the justification of the label
    _inputChannelLabel->setEditable(false, false, false);                    // Set if this label can be editable by the user or not
    _inputChannelLabel->setColour(TextEditor::textColourId, Colours::black); // Set the label colour
    addAndMakeVisible(_inputChannelLabel);                                   // Addint the label to the component and turning it visible

    _outChannelLabel = new Label("label", TRANS("Out _channel"));          // Create the object and set its name
    _outChannelLabel->setFont(Font(15.10f, Font::plain));                  // Set the font size and style
    _outChannelLabel->setJustificationType(Justification::centredLeft);    // Set the justification of the label
    _outChannelLabel->setEditable(false, false, false);                    // Set if this label can be editable by the user or not
    _outChannelLabel->setColour(TextEditor::textColourId, Colours::black); // Set the label colour
    addAndMakeVisible(_outChannelLabel);                                   // Addint the label to the component and turning it visible

    _thresholdAmpLabel = new Label("label", TRANS("T. Amp"));                // Create the object and set its name
    _thresholdAmpLabel->setFont(Font(15.10f, Font::plain));                  // Set the font size and style
    _thresholdAmpLabel->setJustificationType(Justification::centredLeft);    // Set the justification of the label
    _thresholdAmpLabel->setEditable(false, false, false);                    // Set if this label can be editable by the user or not
    _thresholdAmpLabel->setColour(TextEditor::textColourId, Colours::black); // Set the label colour
    addAndMakeVisible(_thresholdAmpLabel);                                   // Addint the label to the component and turning it visible

    _refractoryRmsSamplesLabel = new Label("label", TRANS("RMS Refractory"));        // Create the object and set its name
    _refractoryRmsSamplesLabel->setFont(Font(15.10f, Font::plain));                  // Set the font size and style
    _refractoryRmsSamplesLabel->setJustificationType(Justification::centredLeft);    // Set the justification of the label
    _refractoryRmsSamplesLabel->setEditable(false, false, false);                    // Set if this label can be editable by the user or not
    _refractoryRmsSamplesLabel->setColour(TextEditor::textColourId, Colours::black); // Set the label colour
    addAndMakeVisible(_refractoryRmsSamplesLabel);                                   // Addint the label to the component and turning it visible

    _rmsSamplesLabel = new Label("label", TRANS("RMS Samples"));           // Create the object and set its name
    _rmsSamplesLabel->setFont(Font(15.10f, Font::plain));                  // Set the font size and style
    _rmsSamplesLabel->setJustificationType(Justification::centredLeft);    // Set the justification of the label
    _rmsSamplesLabel->setEditable(false, false, false);                    // Set if this label can be editable by the user or not
    _rmsSamplesLabel->setColour(TextEditor::textColourId, Colours::black); // Set the label colour
    addAndMakeVisible(_rmsSamplesLabel);                                   // Addint the label to the component and turning it visible

	_calibrateButton = new TextButton("Calibrate"); // Create the object and set its name
	_calibrateButton->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom); // Set the style of the corners of this button
    _calibrateButton->setColour(TextButton::buttonNormal, Colour::fromRGBA(61, 78, 69, 255)); // Set colour according to its position inside the button, the position is represented by the ID
	_calibrateButton->addListener(this); // Adding a listener for this button, in this case it is this class
	addAndMakeVisible(_calibrateButton); // Adding the button into this component and turning it visible

    setSize(100, 100);
}

DetectorUi::~DetectorUi()
{
}

void DetectorUi::paint(Graphics &rInGraphics)
{
}

void DetectorUi::updateChannel(int channelCount)
{
    _channelSelection->clear();
    for (int channel = 1; channel <= channelCount; channel++)
    {
        _channelSelection->addItem(std::to_string(channel), channel);
    }
    _channelSelection->setSelectedId(1);
}

void DetectorUi::resized()
{
    // Set the limits of the UI_canvas
    setBounds(0, 0, getWidth(), getHeight());

    // Set the position, width and heigh of the elements - x, y, width, height
    int top_margin = 25;
    int left_margin = 5;

    // Set the limits, size, and location of each component for them to be visible
    _inputChannelLabel->setBounds(left_margin, top_margin, 100, 15);
    _channelSelection->setBounds(left_margin, getNextY(_inputChannelLabel), 100, 20);

    _outChannelLabel->setBounds(getNextX(_channelSelection), top_margin, 100, 15);
    _outChannelSelection->setBounds(getNextX(_channelSelection), getNextY(_inputChannelLabel), 100, 20);

    _calibrateButton->setBounds(getNextX(_outChannelSelection), getNextY(_inputChannelLabel), 100, 20);

    _thresholdAmpLabel->setBounds(left_margin, getNextY(_channelSelection), 150, 15);
    _thresholdAmplitude->setBounds(left_margin, getNextY(_thresholdAmpLabel), 100, 20);

    _refractoryRmsSamplesLabel->setBounds(getNextX(_thresholdAmplitude), getNextY(_channelSelection), 150, 15);
    _refractoryRmsSamples->setBounds(getNextX(_thresholdAmplitude), getNextY(_refractoryRmsSamplesLabel), 100, 20);

    _rmsSamplesLabel->setBounds(getNextX(_refractoryRmsSamples), getNextY(_channelSelection), 150, 15);
    _rmsSamples->setBounds(getNextX(_refractoryRmsSamples), getNextY(_rmsSamplesLabel), 100, 20);
}

void DetectorUi::buttonClicked(Button *pInButtonClicked)
{
    if(pInButtonClicked == _calibrateButton){
        _calibrate = true;
    }
}

void DetectorUi::sliderValueChanged(Slider *pInSliderChanged)
{
    if (pInSliderChanged == _thresholdAmplitude)
        _thresholdAmp = (unsigned int)pInSliderChanged->getValue();

    if (pInSliderChanged == _refractoryRmsSamples)
        _rmsRefractionCount = (unsigned int)pInSliderChanged->getValue();

    if (pInSliderChanged == _rmsSamples)
        _rmsSamplesCount = (unsigned int)pInSliderChanged->getValue();
}

void DetectorUi::comboBoxChanged(ComboBox *pInComboBoxChanged)
{
    if (pInComboBoxChanged == _channelSelection)
        _channel = pInComboBoxChanged->getSelectedId();

    if (pInComboBoxChanged == _outChannelSelection)
        _outChannel = pInComboBoxChanged->getSelectedId();
}

DetectorEditor::DetectorEditor(GenericProcessor *pInParentNode, bool useDefaultParameterEditors)
    : GenericEditor(pInParentNode, useDefaultParameterEditors)
{
    addAndMakeVisible(&_pluginUi);
    const int plugin_witdh = 350;
    setDesiredWidth(plugin_witdh);
}

DetectorEditor::~DetectorEditor()
{
}

void DetectorEditor::updateSettings()
{
    int channel_count = getProcessor()->getNumInputs();
    _pluginUi.updateChannel(channel_count);
}

void DetectorEditor::resized()
{
    GenericEditor::resized();

    const int x_pos_initial = 0;
    const int y_pos_inttial = 0;

    const int width = 850;
    const int height = 200;

    // Set the position and bounds of this group of elements inside the plugin UI
    _pluginUi.setBounds(x_pos_initial, y_pos_inttial, width, height);
}