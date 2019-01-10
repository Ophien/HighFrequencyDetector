#include "Detector.h"

Detector::Detector()
    : GenericProcessor("Detector")
{
    _calibrationTime = 50000.0;
    _rmsSamplesSinceDetection = 0;
    _mean = 0.0;
    _standardDeviation = 0.0;
    _isPluginEnabled = true;
    _detectionEnabled = true;
    _isCalibrating = true;
    _startTime = getTimestamp(_channel);

    setProcessorType(PROCESSOR_TYPE_FILTER);
    createEventChannels();
}

Detector::~Detector()
{
}

bool Detector::enable()
{
    return true;
}

void Detector::createEventChannels()
{
    const DataChannel *in = getDataChannel(0);
    _pTtlEventChannel = new EventChannel(EventChannel::TTL, 8, 1, (in) ? in->getSampleRate() : CoreServices::getGlobalSampleRate(), this);
    MetaDataDescriptor md(MetaDataDescriptor::CHAR, 34, "High frequency detection type", "Description of the frequency", "channelInfo.extra");
    MetaDataValue mv(md);
    _pTtlEventChannel->addMetaData(md, mv);

    if (in)
    {
        md = MetaDataDescriptor(MetaDataDescriptor::UINT16,
                                3,
                                "Detection module",
                                "Index at its source, Source processor ID and Sub Processor index of the channel that triggers this event",
                                "source.channel.identifier.full");
        mv = MetaDataValue(md);
        uint16 source_info[3];
        source_info[0] = in->getSourceIndex();
        source_info[1] = in->getSourceNodeID();
        source_info[2] = in->getSubProcessorIdx();
        mv.setValue(static_cast<const uint16 *>(source_info));
        _pTtlEventChannel->addMetaData(md, mv);
    }

    eventChannelArray.add(_pTtlEventChannel);
}

void Detector::updateSettings()
{
    const DataChannel *in = getDataChannel(0);
    _pTtlEventChannel = new EventChannel(EventChannel::TTL, 8, 1, (in) ? in->getSampleRate() : CoreServices::getGlobalSampleRate(), this);
    MetaDataDescriptor md(MetaDataDescriptor::CHAR, 34, "High frequency detection type", "Description of the frequency", "channelInfo.extra");
    MetaDataValue mv(md);
    _pTtlEventChannel->addMetaData(md, mv);

    if (in)
    {
        md = MetaDataDescriptor(MetaDataDescriptor::UINT16,
                                3,
                                "Detection module",
                                "Index at its source, Source processor ID and Sub Processor index of the channel that triggers this event",
                                "source.channel.identifier.full");
        mv = MetaDataValue(md);
        uint16 source_info[3];
        source_info[0] = in->getSourceIndex();
        source_info[1] = in->getSourceNodeID();
        source_info[2] = in->getSubProcessorIdx();
        mv.setValue(static_cast<const uint16 *>(source_info));
        _pTtlEventChannel->addMetaData(md, mv);
    }

    eventChannelArray.add(_pTtlEventChannel);
}

AudioProcessorEditor *Detector::createEditor()
{
    _pDetectorEditor = new DetectorEditor(this, true);
    editor = _pDetectorEditor;
    return editor;
}

void Detector::sendTtlEvent(int rmsIndex, int val)
{
    // send event only when the animal is not moving
    if (!_isPluginEnabled)
        return;

    // timestamp for this sample
    uint64 time_stamp = getTimestamp(_channel) + rmsIndex;

    uint8 ttlData;
    uint8 output_event_channel = _outputChannel;
    ttlData = val << _outputChannel;
    TTLEventPtr ttl = TTLEvent::createTTLEvent(_pTtlEventChannel, time_stamp, &ttlData, sizeof(uint8), output_event_channel);
    addEvent(_pTtlEventChannel, ttl, rmsIndex);
}

void Detector::detect(std::vector<double> &rInRmsBuffer)
{
    for (unsigned int rms_sample = 0; rms_sample < rInRmsBuffer.size(); rms_sample++)
    {
        double sample = rInRmsBuffer[rms_sample];

        if (_detectionEnabled && sample > _threshold)
        {
            sendTtlEvent(rms_sample, 1);
            _detected = true;
            _detectionEnabled = false;
        }

        // count rms samples since last detection
        if (_detected)
        {
            _rmsSamplesSinceDetection += 1;
        }

        // enable detection again
        if (_rmsSamplesSinceDetection > _rmsRefractionCount)
        {
            _detected = false;
            _detectionEnabled = true;
            _rmsSamplesSinceDetection = 0;

            sendTtlEvent(rms_sample, 0);
        }
    }
}

void Detector::calibrate()
{
    if (_currentTime > _calibrationTime)
    {
        // set flag to false to end the calibration period
        _isCalibrating = false;

        // calculate statistics
        _mean = _mean / (double)_calibrationRms.size();

        // calculate standard deviation
        for (unsigned int rms_sample = 0; rms_sample < _calibrationRms.size(); rms_sample++)
        {
            _standardDeviation += pow(_calibrationRms[rms_sample] - _mean, 2.0);
        }
        _standardDeviation = sqrt(_standardDeviation / ((double)_calibrationRms.size() - 1));
    }
}

double Detector::calculateRms(const float *rInBuffer, int index)
{
    double sum = 0.0;

    for (int cnt = 0; cnt < _rmsSize; cnt++)
    {
        sum += pow(rInBuffer[index + cnt], 2.0);
    }

    double rms = sqrt(sum / _rmsSize);

    return rms;
}

void Detector::process(AudioSampleBuffer &rInBuffer)
{
    // update parameters according to UI
    _outputChannel = _pDetectorEditor->_pluginUi._outChannel - 1;
    _channel = _pDetectorEditor->_pluginUi._channel - 1;
    _thresholdAmp = _pDetectorEditor->_pluginUi._thresholdAmp;
    _rmsRefractionCount = _pDetectorEditor->_pluginUi._rmsRefractionCount;
    _rmsSize = _pDetectorEditor->_pluginUi._rmsSamplesCount;
    
    // define _threshold
    _threshold = _mean + _thresholdAmp * _standardDeviation;

    if (_pDetectorEditor->_pluginUi._calibrate == true)
    {
        printf("recalibrating...\n");
        _pDetectorEditor->_pluginUi._calibrate = false;
        _isCalibrating = true;
        _currentTime = 0.0;
    }

    // Get accelerometer raw data
    const float *rSamples = rInBuffer.getReadPointer(_channel);

    // RMS buffer
    std::vector<double> local_rms;

    // Generate RMS buffer
    for (int rms_index = 0; rms_index < rInBuffer.getNumSamples(); rms_index += _rmsSize)
    {
        if (rms_index + _rmsSize > rInBuffer.getNumSamples())
        {
            break;
        }

        // RMS calculation
        double rms = calculateRms(rSamples, rms_index);

        // Calculate average between RMSs to determine baseline _threshold
        if (_isCalibrating)
        {
            // Add variables to be used as a calibration basis
            _calibrationRms.push_back(rms);
            _mean += rms;
        }

        // Set buffer value
        local_rms.push_back(rms);
    }

    // check if plugin needs to stop calibration and calculate its statistics for _threshold estimation
    if (_isCalibrating)
    {
        calibrate();
    }
    else
    {
        detect(local_rms);
    }

    // calculate performing time
    _currentTime += getTimestamp(_channel) - _startTime;

    // Check if any event ocurred and tells this plugin to call the handleEventsFunction()
    checkForEvents();
}

void Detector::handleEvent(const EventChannel *rInEventInfo, const MidiMessage &rInEvent, int samplePosition)
{
    // handle plugin control events
    _isPluginEnabled = true;

    if (Event::getEventType(rInEvent) == EventChannel::TEXT)
    {
        TextEventPtr text_event = TextEvent::deserializeFromMessage(rInEvent, rInEventInfo);

        std::string msg = std::string((char *)text_event->getRawDataPointer());

        printf("sample: %Ld message: %s\n", text_event->getTimestamp(), msg.c_str());

        if (strcmp(msg.c_str(), "movement_detected") == 0)
        {
            _isPluginEnabled = false;
        }
    }
}
