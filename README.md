# SillyMUD World Conversion Tool

This tool converts SillyMUD formatted world files into `JSON` `objects`, `mobiles`, `rooms`, and `zones` that should correctly cross-reference itself.

The importation code was extracted from the newest copy of SillyMUD that I had. The world files are taken from the same release.

## Building

### Pre-requisites

You must have the `jansson` `JSON` library installed. For macOS, you can:

```shell
brew install jansson
```

```shell
make
```

This builds the `export` tool.

### Running

```shell
./export
```
