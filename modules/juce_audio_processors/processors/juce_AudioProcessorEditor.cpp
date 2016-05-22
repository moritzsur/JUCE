/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

AudioProcessorEditor::AudioProcessorEditor (AudioProcessor& p) noexcept  : processor (p), constrainer (nullptr)
{
    initialise();
}

AudioProcessorEditor::AudioProcessorEditor (AudioProcessor* p) noexcept  : processor (*p), constrainer (nullptr)
{
    // the filter must be valid..
    jassert (p != nullptr);
    initialise();
}

AudioProcessorEditor::~AudioProcessorEditor()
{
    // if this fails, then the wrapper hasn't called editorBeingDeleted() on the
    // filter for some reason..
    jassert (processor.getActiveEditor() != this);
    removeComponentListener (resizeListener);
}

void AudioProcessorEditor::setControlHighlight (ParameterControlHighlightInfo) {}
int AudioProcessorEditor::getControlParameterIndex (Component&)  { return -1; }

void AudioProcessorEditor::initialise()
{
    setConstrainer (&defaultConstrainer);
    addComponentListener (resizeListener = new AudioProcessorEditorListener (this));
}

//==============================================================================
void AudioProcessorEditor::setResizable (const bool shouldBeResizable)
{
    if (shouldBeResizable)
    {
        if (resizableCorner == nullptr)
        {
            Component::addChildComponent (resizableCorner = new ResizableCornerComponent (this, constrainer));
            resizableCorner->setAlwaysOnTop (true);
        }
    }
    else
    {
        setConstrainer (&defaultConstrainer);
        resizableCorner = nullptr;

        if (getWidth() > 0 && getHeight() > 0)
        {
            defaultConstrainer.setSizeLimits (getWidth(), getHeight(), getWidth(), getHeight());
            resized();
        }
    }
}

bool AudioProcessorEditor::isResizable() const noexcept
{
    return resizableCorner != nullptr;
}

void AudioProcessorEditor::setResizeLimits (const int newMinimumWidth,
                                            const int newMinimumHeight,
                                            const int newMaximumWidth,
                                            const int newMaximumHeight) noexcept
{
    // if you've set up a custom constrainer then these settings won't have any effect..
    jassert (constrainer == &defaultConstrainer || constrainer == nullptr);

    setResizable (newMinimumWidth != newMaximumWidth || newMinimumHeight != newMaximumHeight);

    if (constrainer == nullptr)
        setConstrainer (&defaultConstrainer);

    defaultConstrainer.setSizeLimits (newMinimumWidth, newMinimumHeight,
                                      newMaximumWidth, newMaximumHeight);

    setBoundsConstrained (getBounds());
}

void AudioProcessorEditor::setConstrainer (ComponentBoundsConstrainer* newConstrainer)
{
    if (constrainer != newConstrainer)
    {
        constrainer = newConstrainer;

        const bool shouldBeResizable = resizableCorner != nullptr;

        resizableCorner = nullptr;

        setResizable (shouldBeResizable);

        if (isOnDesktop())
            if (ComponentPeer* const peer = getPeer())
                peer->setConstrainer (constrainer);
    }
}

void AudioProcessorEditor::setBoundsConstrained (Rectangle<int> newBounds)
{
    if (constrainer != nullptr)
        constrainer->setBoundsForComponent (this, newBounds, false, false, false, false);
    else
        setBounds (newBounds);
}

void AudioProcessorEditor::editorResized (bool wasResized)
{
    if (wasResized)
    {
        bool resizerHidden = false;

        if (ComponentPeer* peer = getPeer())
            resizerHidden = peer->isFullScreen() || peer->isKioskMode();

        if (resizableCorner != nullptr)
        {
            resizableCorner->setVisible (! resizerHidden);

            const int resizerSize = 18;
            resizableCorner->setBounds (getWidth() - resizerSize,
                                        getHeight() - resizerSize,
                                        resizerSize, resizerSize);
        }
        else
        {
            if (getWidth() > 0 && getHeight() > 0)
                defaultConstrainer.setSizeLimits (getWidth(), getHeight(),
                                                  getWidth(), getHeight());
        }
    }
}
