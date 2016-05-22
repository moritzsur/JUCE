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

#ifndef JUCE_AUDIOPROCESSOREDITOR_H_INCLUDED
#define JUCE_AUDIOPROCESSOREDITOR_H_INCLUDED

class AudioProcessorEditorListener;
//==============================================================================
/**
    Base class for the component that acts as the GUI for an AudioProcessor.

    Derive your editor component from this class, and create an instance of it
    by overriding the AudioProcessor::createEditor() method.

    @see AudioProcessor, GenericAudioProcessorEditor
*/
class JUCE_API  AudioProcessorEditor  : public Component
{
protected:
    //==============================================================================
    /** Creates an editor for the specified processor. */
    AudioProcessorEditor (AudioProcessor&) noexcept;

    /** Creates an editor for the specified processor. */
    AudioProcessorEditor (AudioProcessor*) noexcept;

public:
    /** Destructor. */
    ~AudioProcessorEditor();


    //==============================================================================
    /** The AudioProcessor that this editor represents. */
    AudioProcessor& processor;

    /** Returns a pointer to the processor that this editor represents.
        This method is here to support legacy code, but it's easier to just use the
        AudioProcessorEditor::processor member variable directly to get this object.
    */
    AudioProcessor* getAudioProcessor() const noexcept        { return &processor; }

    //==============================================================================
    /** Used by the setParameterHighlighting() method. */
    struct ParameterControlHighlightInfo
    {
        int parameterIndex;
        bool isHighlighted;
        Colour suggestedColour;
    };

    /** Some types of plugin can call this to suggest that the control for a particular
        parameter should be highlighted.
        Currently only AAX plugins will call this, and implementing it is optional.
    */
    virtual void setControlHighlight (ParameterControlHighlightInfo);

    /** Called by certain plug-in wrappers to find out whether a component is used
        to control a parameter.

        If the given component represents a particular plugin parameter, then this
        method should return the index of that parameter. If not, it should return -1.
        Currently only AAX plugins will call this, and implementing it is optional.
    */
    virtual int getControlParameterIndex (Component&);

    //==============================================================================
    /** Make the editor resizable or fixed.

        @param shouldBeResizable   whether it's resizable at all
        @see setResizeLimits, isResizable
    */
    void setResizable (bool shouldBeResizable);

    /** Returns true if resizing is enabled.

        @see setResizable
    */
    bool isResizable() const noexcept;

    /** This sets the maximum and minimum sizes for the window.

        If the window's current size is outside these limits, it will be resized to
        make sure it's within them.

        A direct call to setBounds() will bypass any constraint checks, but when the
        window is dragged by the user or resized by other indirect means, the constrainer
        will limit the numbers involved.

        @see setResizable
    */
    void setResizeLimits (int newMinimumWidth,
                          int newMinimumHeight,
                          int newMaximumWidth,
                          int newMaximumHeight) noexcept;


    /** Returns the bounds constrainer object that this window is using.
        You can access this to change its properties.
    */
    ComponentBoundsConstrainer* getConstrainer() noexcept           { return constrainer; }

    /** Sets the bounds-constrainer object to use for resizing and dragging this window.

        A pointer to the object you pass in will be kept, but it won't be deleted
        by this object, so it's the caller's responsibility to manage it.

        If you pass a nullptr, then no contraints will be placed on the positioning of the window.
    */
    void setConstrainer (ComponentBoundsConstrainer* newConstrainer);

    /** Calls the window's setBounds method, after first checking these bounds
        with the current constrainer.

        @see setConstrainer
     */
    void setBoundsConstrained (Rectangle<int> newBounds);

    ScopedPointer<ResizableCornerComponent> resizableCorner;

private:
    //==============================================================================
    struct AudioProcessorEditorListener : ComponentListener
    {
        AudioProcessorEditorListener (AudioProcessorEditor* audioEditor) : e (audioEditor) {}
        void componentMovedOrResized (Component&, bool, bool wasResized) override   { e->editorResized (wasResized); }
        AudioProcessorEditor* e;
    };

    //==============================================================================
    void initialise();
    void editorResized (bool wasResized);

    //==============================================================================
    ScopedPointer<AudioProcessorEditorListener> resizeListener;
    ComponentBoundsConstrainer defaultConstrainer;
    ComponentBoundsConstrainer* constrainer;

    JUCE_DECLARE_NON_COPYABLE (AudioProcessorEditor)
};


#endif   // JUCE_AUDIOPROCESSOREDITOR_H_INCLUDED
