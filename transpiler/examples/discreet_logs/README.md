# README

This project uses the `bazel` build system of Google's `fully-homomorphic-encryption` library.  To build it, you should clone our fork of the project at https://github.com/sballan/fully-homomorphic-encryption.  As per Google's instructions, we have inserted the code you see here in the `/transpiler/examples/discreet_logs/` directory in order for it to build properly.

To install `bazel` and build this project, follow the instructions at [`/transpiler/README.md`](/transpiler/README.md).  We found it works best to install using Option A on an Intel Ubuntu system with at least 4 cores and 8GB of RAM.

From the root directory of the forked repository, you can then run `bazel run //transpiler/examples/discreet_logs:discreet_logs_client`.