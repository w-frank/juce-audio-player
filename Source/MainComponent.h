#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::ChangeListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:

    juce::AudioDeviceManager customDeviceManager;
    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioSettings;

    typedef enum
    {
        Stopped,
        Playing,
        Starting,
        Stopping
    }transportSourceState_t;

    transportSourceState_t transportSourceState;

    void openTextButtonClicked();
    void playAudioButtonClicked();
    void stopAudioButtonClicked();
    void transportSourceStateChanged(transportSourceState_t state);
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> playSource;
    juce::AudioTransportSource transportSource;

    juce::TextButton openFileButton;
    juce::TextButton playAudioButton;
    juce::TextButton stopAudioButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
