# Never - Functional Programming Language
[![Build Status](https://travis-ci.org/never-lang/never.svg?branch=master)](https://travis-ci.org/never-lang/never)
[![Version](https://img.shields.io/github/release/never-lang/never.svg)](https://github.com/never-lang/never/releases)
[![Codecov](https://codecov.io/gh/never-lang/never/branch/master/graph/badge.svg)](https://codecov.io/gh/never-lang/never)

[Never](https://never-lang.readthedocs.io/en/latest/) is a simple functional 
programming language. Technically it may be classified as syntactically scoped,
strongly typed, call by value, functional programming language.

In practise Never offers basic data types, assignment, control flow, arrays,
first order functions and some mathematical functions to make it useful
to calculate expressions. Also it demonstrates how functions can be compiled,
invoked and passed as parameters or results between other functions.

## Getting Started
It is easy to download and compile Never language. Just clone repository
and make the compiler in a few basic steps. To read more visit
[readthedocs.io](https://never-lang.readthedocs.io/en/latest/) site.

### Prerequisites

Never requires basic Linux installation to be built. The following tools and
libraries are needed:
* gcc
* glibc
* bison / flex
* libmath, libdl and libffi

### Installing

Never can be installed in steps:
* clone repository
* ```cd never```
* ```mkdir build; cd build```
* ```cmake ..; make```

### asm.js wasm

To compile for asm.js or wasm targets use the following commands:

Get the latest sdk: git clone https://github.com/emscripten-core/emsdk.git

```bash
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

Compile to native asm.js

```bash
mkdir build && cd build
emcmake cmake .. -DJS_ONLY=ON 
make
```

Compile to native wasm

```bash
mkdir build && cd build
emcmake cmake .. -DWASM_ONLY=ON 
make
```

Run

Now you should see never.js file in your build directory. You can overwrite ../never.js and open never.html, or modify never.html to load build/never.js file or load the file in JS console and run:

```bash
python -m SimpleHTTPServer 8000
# http://localhost:8000/never.html
```

```js
var never = Module.cwrap('never','number',['string']);
never("func main() -> int { 123 }")
```


## Contributing
All help is welcome! Using it, reporting bugs, spreading the word, writing
code samples, blogs, submitting ideas, documentation, new features. Everyone
is invited to contribute.

## License
This project is licensed under the MIT License.



