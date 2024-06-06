# Raspberry Pi 4 & 5 Open Glx Tutorial

A project that demonstrates how to create a X11 window on RP4 and execute OpenGL commands.

![RPI 4 & 5 GLX ES3.1](data/screenshot_2.png?raw=true "RPI 4 & 5 GLX ES3.1")

## Table of contents

- [Raspberry Pi 4 \& 5 Open Glx Tutorial](#raspberry-pi-4--5-open-glx-tutorial)
  - [Table of contents](#table-of-contents)
  - [Requirements](#requirements)
  - [Dependencies](#dependencies)
    - [X11](#x11)
    - [Mesa Utils](#mesa-utils)
  - [Resources](#resources)
  - [License](#license)

## Requirements

- A C++20 compliant compiler (GCC-13.2, Clang-17, etc.).
- Raspberry Pi 4

## Dependencies

### X11

For X11 windowing system install the x11 library:

```bash
sudo apt-get install libx11-dev
```

### Mesa Utils

It's also necessery to install mesa-utils:

```bash
sudo apt-get install mesa-utils
```

## Resources

<https://wiki.maemo.org/SimpleGL_example>
<http://manpages.ubuntu.com/manpages/bionic/en/man3/XCreateWindow.3.html>

## License

Copyright 2024 MrAviator93

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
