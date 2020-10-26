#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : juce::AudioAppComponent(customDeviceManager),
                                    openFileButton("Open File"), playAudioButton("Play"), 
                                    stopAudioButton("Stop"), transportSourceState(Stopped)
{

    customDeviceManager.initialise(2, 2, nullptr, true);
    audioSettings.reset(new juce::AudioDeviceSelectorComponent(customDeviceManager,
                                                                0, 2, 0, 2,
                                                                true, true, true, true));
    addAndMakeVisible(audioSettings.get());

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    openFileButton.onClick = [this] {openTextButtonClicked(); };
    addAndMakeVisible(&openFileButton);

    playAudioButton.onClick = [this] {playAudioButtonClicked(); };
    playAudioButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playAudioButton.setEnabled(true);
    addAndMakeVisible(&playAudioButton);

    stopAudioButton.onClick = [this] {stopAudioButtonClicked(); };
    stopAudioButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopAudioButton.setEnabled(false);
    addAndMakeVisible(&stopAudioButton);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (400, 600);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::openTextButtonClicked()
{

    juce::FileChooser chooser ("Open a Wav or AIFF file", juce::File::getSpecialLocation(juce::File::userHomeDirectory),
                                "*.wav;*.aiff;*.mp3");

    if (chooser.browseForFileToOpen())
    {
        juce::File audioFile;
        audioFile = chooser.getResult();

        juce::AudioFormatReader* reader = formatManager.createReaderFor(audioFile);

        if (reader != nullptr)
        {
            std::unique_ptr<juce::AudioFormatReaderSource> tempSource (new juce::AudioFormatReaderSource (reader, true));
            transportSource.setSource(tempSource.get());
            transportSourceStateChanged(Stopped);
            playSource.reset(tempSource.release());
        }
        
    }

}

void MainComponent::playAudioButtonClicked()
{
    transportSourceStateChanged(Starting);
}

void MainComponent::stopAudioButtonClicked()
{
    transportSourceStateChanged(Stopping);
}

void MainComponent::transportSourceStateChanged(transportSourceState_t state)
{
    if (state != transportSourceState)
    {
        transportSourceState = state;

        switch(transportSourceState)
        {
            case Stopped:
                playAudioButton.setEnabled(true);
                transportSource.setPosition(0.0);
                break;
            case Playing:
                playAudioButton.setEnabled(true);
                break;
            case Starting:
                playAudioButton.setEnabled(false);
                stopAudioButton.setEnabled(true);
                transportSource.start();
                break;
            case Stopping:
                playAudioButton.setEnabled(true);
                stopAudioButton.setEnabled(false);
                transportSource.stop();
                break;
        }
    }
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
        {
            transportSourceStateChanged(Playing);
        }
        else
        {
            transportSourceStateChanged(Stopped);
        }
        
    }
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    transportSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    openFileButton.setBounds(10, 10, getWidth() - 20, 30);
    playAudioButton.setBounds(10, 50, getWidth() - 20, 30);
    stopAudioButton.setBounds(10, 90, getWidth() - 20, 30);
    audioSettings->setBounds(10, 130, getWidth()- 20, 100);
}
