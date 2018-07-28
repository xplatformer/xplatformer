# Linking

The XSamples have a dependency on XGameLib. Rather than include a binary or the entire library, the code is included by symlinking.

This can either be done manually using the following:

```bash
ln -s /path/to/file /path/to/symlink
```

Or it can be done by running the `link.bash` script, that has the assumption that the `XGameLib` repository is on the same level as the `XSamples` repository.