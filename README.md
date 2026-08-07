# Little language

Is an imperative, C-like programming language developed using C++ and the LLVM Project

# Setup

I recommend running this on Linux or WSL
This project requires llc from LLVM, cmake and clang
Create a build folder, go to it and run:

> cmake ..

This will build your Makefile, which can then be used to run:

> make

Which should generate an executable called <em>little_compiler</em>

# Usage

Using the compiler is as simple as running:

> ./little_compiler <filename>

The compiler expects the file to have the <strong>.lln</strong> extension
After compilation, an executable file is generated which can be run
More information on the build process, compilation and syntax can be found in the attached <strong>LittleLanguageManual.pdf</strong> file