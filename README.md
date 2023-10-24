# RP4GLX - Raspberry Pi 4 OpenGLX

A project that demonstrates how to create a X11 window on RP4 and execute OpenGL commands.

![RPI4 GLX ES3.1](data/screenshot.png?raw=true "RPI4 GLX ES3.1")

## Table of contents

- [RP4GLX - Raspberry Pi 4 OpenGLX](#rp4glx---raspberry-pi-4-openglx)
  - [Table of contents](#table-of-contents)
  - [Requirements](#requirements)
    - [Boost](#boost)
    - [Fmt](#fmt)
    - [Date](#date)
    - [X11](#x11)
    - [Glm](#glm)
    - [Mesa Utils](#mesa-utils)
  - [License](#license)

## Requirements

A C++ compiler that supports C++ 17 revision, Boost C++ libraries and fmt formatting library.

### Boost

Use the following command to install boost libraries:

```bash
sudo apt-get install libboost*.**-dev
```

For latest version of boost libraries see <https://www.boost.org/>

### Fmt

For fmtlib, clone the following repository <https://github.com/fmtlib/fmt> or install using package manager

```bash
sudo apt-get install libfmt-dev
```

### Date

For date library, clone the following repository <https://github.com/HowardHinnant/date>

### X11

For X11 windowing system install the x11 library:

```bash
sudo apt-get install libx11-dev
```

### Glm

Additionaly, it won't hurt to install glm library for any math's related operations:

```bash
sudo apt-get install libglm-dev
```

### Mesa Utils

It's also necessery to install mesa-utils:

```bash
sudo apt-get install mesa-utils
```

## License

Copyright 2021 Arturs Kalnins

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
