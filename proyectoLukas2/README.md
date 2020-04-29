## Prerequisites
### Building
You need meson and ninja in order to build the project and run the tests.
In Ubuntu and Debian:

```console
apt install meson ninja-build
```

Once installed

```console
cd VillagraCiancaglini
meson build
cd build
```

All building and testing should be done inside the `build` directory.
You can recompile with

```console
ninja
```

And run the tests with

```console
meson test
```

## Getting started
### Downloading the graphs
In the VillagraCiancaglini directory

```console
mkdir graphs
cd graphs
wget -i ../graphs.txt
```

will download them. Now you need to decompress some of them with

```console
# For tar.gz files
tar xvzf foo.tar.gz

# For just gz files
gzip --decompress foo.gz
```

### Coding
Code goes into `CangrejoEstelar`. All new `.c` files that aren't tests, add them
to the `sources` list in the `meson.build` file within that dir. If you need to
print debug info or errors, include the `log.h` header use the `log_debug` and similar
functions. Happy hacking.

### Testing
By default, meson will do a debug build without any optimizations.
For some intensive tests you may want to enable them.

```console
meson configure --buildtype release
```

You can run individual tests with the name of the test, for example

```console
meson test "graph"
```

Tests are grouped in suites - just labels to group related stuff together.
You can run a specific suite with the `--suite foo` flag, for example

```console
meson test --suite "unit test"
```
