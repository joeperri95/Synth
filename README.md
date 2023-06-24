# Synth
An audio processing program
Most dependencies are installed via conan
You need to have portaudio installed manually.

## GUI

### ControlWidget
These are imgui widgets that control and individual pipeline node

### NodeWidget
These live in the nodeEditorWidget
They have input and outputs 
Graphical representation of a pipeline

## PipelineController
This is the bridge between the graphical nodewidgets and the actual audio pipeline

## Pipeline

Each pipeline element inherits from the Node class.
Elements are added to a pipeline object



Future design

Plugin based
pipeline will load a .so file containing an audio node
gui will construct the visual node based on a config file
gui will optionally load a control panel ui element contained in a .so file
Tooling to generate this subproject
