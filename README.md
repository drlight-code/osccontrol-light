oscsend-light - An audio plugin that speaks OSC
===============================================

This software enables dispatching of Open Sound Control (OSC) messages
from audio plugin host environments such as digital audio workstations
(DAW). 

It features a plugin or standalone UI that enables prototyping
of control presets by quick switching and reloading of preset
definitions.  To integrate into a host environment with automatable
controls, the plugin can run in headless mode that loads a specific
preset upon construction. 

Control presets are defined via a concise
YAML-based textual markup format.


Usage
-----

To run the standalone UI application, execute the `oscsend-light`
application binary. To run the UI inside a plugin host, make the
`oscsend-light.{so,dll}` file available to the host environment and
load the plugin.

The UI elements are illustrated in the following.

<img src="Documentation/Images/ui-overview.png" width="512" title="oscsend-light UI overview">

To create a headless instance of the plugin that provides automatable
controls inside an audio plugin host environment, create a copy (or
symbolic link) of the `oscsend-light.{so,dll}` file with the preset
name as `oscsend-light-<name>.{so,dll}`. This enables
registration of the controls at plugin initialization time, which is
necessary for most hosts.

Environment variables:
- `OSCSEND_SCALE_FACTOR`: Set UI scale factor for high DPI displays.
- `OSCSEND_PRESET_PATH`: Set directory with preset definitions.

The directory specified via the UI or `OSCSEND_PRESET_PATH` will
be searched recursively for `*.yaml` preset definition files.

**Please note: setting `OSCSEND_PRESET_PATH` in the environment is
  mandatory for headless mode!**


Preset Definitions
------------------

Control presets are defined in a YAML-based configuration format as
follows. Refer to the [Presets](Presets) folder for more examples.


```
network:
  host: 127.0.0.1
  port: 6789

controls:
  - name: frequency
    type: float
    range: [100, 2000]
    default: 440
    message: /synth/freq

  - name: amplitude
    type: float
    range: [0, 1]
    default: 0.8
    message: /synth/amplitude
```

Currently, only continuous (float) controls are supported. Additional
control types, such as toggle buttons or parameter groups will be
added in upcoming releases.


Platform Support
----------------

Different plugin SDKs are supported via the [JUCE](https://juce.com/)
library (VST, VST3, AU, RTAS and AAX), as well as all major operating
systems (GNU/Linux, MacOS, Microsoft Windows).


Compilation
-----------

Build the standalone application and plugin for your platform by
exporting the project files for your favorite IDE using the
[Projucer](https://juce.com/discover/projucer) application that ships
with JUCE.


Licensing
---------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version. 

See the [LICENSE](LICENSE) file for more details.
