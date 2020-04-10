# References

This is the reference material that was used while implementing parts of the Beowolf engine.

## Math

* [Miller-Rabin Primality Test](https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test) - fast check for prime numbers
* [Hartmann & Gribbs Method](https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf) - fast way to extract the view frustum planes from an arbitrary projection matrix
* [fma-based lerp](https://devblogs.nvidia.com/lerp-faster-cuda/) - optimization for faster and more accurate lerps

## Font Rendering

* [Signed Distance Field](https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf) - improved textures for vector graphics, such as font glyphs
* [Multi-channel SDF](https://github.com/Chlumsky/msdfgen/files/3050967/thesis.pdf) - further improvement on the above that handles sharp edges significantly better