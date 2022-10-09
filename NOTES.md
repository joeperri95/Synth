
Nodes kinda like Gstreamer

Pipeline -> container for all of these nodes
    ensures there are enough sources and sinks
    manages the metadata 

Node.hpp -> virtual base class
    has update() periodic method

Sources.hpp -> Virtual base class. Provide single audio source
Sink.hpp -> Virtual base class. Provides single audio sink
FilterNode.hpp -> Virtual base class. Has at least one input and output


AudioBuffer -> What the nodes use to pass data around.
AudioDevice -> Extends Sink. Portaudio wrapper for playback.
FileWriter -> Extends Sink. Writes to file.
FileReader -> Extends Source. Reads from file.
