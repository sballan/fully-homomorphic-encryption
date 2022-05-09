# README

This project uses the `bazel` build system of Google's `fully-homomorphic-encryption` library. To install `bazel` and build this project, follow the instructions at [`/transpiler/README.md`](/transpiler/README.md).  We found it works best to install using Option A on an Ubuntu system with at least 4 cores and 8GB of RAM.

You can then run it with `bazel run //transpiler/examples/discreet_logs:hangman_client`