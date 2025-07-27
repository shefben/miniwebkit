# MiniWebKit

This repository contains a heavily stripped-down build of WebKit.

Only the core DOM and rendering components are included. The following
features have been removed:

- SVG support
- MathML support
- Inspector and debugging utilities
- Profiler and complex GUI modules

Generation scripts tied to the old build systems have also been deleted.

To build the project use `xmake` on a supported platform.
