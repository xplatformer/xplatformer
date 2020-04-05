# XSamples

## Summary

XSamples provides a collection of *XWindows* starter code that works with the XGameLib library. The XGameLib library powers the XPlatformer project. All the examples make use of the XLib API (XOrg) and focus on code that was developed in the XPlatformer project (or planned to be used). The point of the examples is to demonstrate how to use the XGameLib library.

## Getting started

To make ("compile and link") an example, use the following bazel commands to build:

```bash
bazel build //samples:<sample>
```

Then, to run:

```bash
bazel run //samples:<sample>
```

## Samples

|Name|Description|Build cmd|Run cmd|
|---|---|---|---|
|Animation|Demonstrates animation with shapes.|`bazel build //samples:xanimation`|`bazel run //samples:xanimation`|
|Clipping|Demonstrates using clipping to limit screen visibility.|`bazel build //samples:xclip`|`bazel run //samples:xclip`|
|Drawing|Demonstrates drawing shapes.|`bazel build //samples:xdraw`|`bazel run //samples:xdraw`|
|Input|Demonstrates input from the mouse & keyboard.|`bazel build //samples:xinput`|`bazel run //samples:xinput`|
|Null Check|Verify that the project successfully compiles.|`bazel build //samples:xnull`|`bazel run //samples:xnull`|
|Sprites|Demonstrates the usage of spritesheets for animation.|`bazel build //samples:xsprites`|`bazel run //samples:xsprites`|
|Windows|Demonstrates the `XInfo` class for XWindows commands.|`bazel build //samples:xwindows`|`bazel run //samples:xwindows`|

## Acknowledgements

The project icon is retrieved from [kenney.nl](docs/icon/icon.json). The original source material has been altered for the purposes of the project. The icon is used under the terms of the [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/).

The project icon uses assets by [Kenney from kenney.nl/](http://kenney.nl/assets/platformer-art-deluxe).
