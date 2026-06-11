# XMRig Meson

Meson buildsystem for [XMRig](https://github.com/xmrig/xmrig).

install prerequisites(debian): `sudo apt install build-essential meson`

compile and install:
```
meson setup build --buildtype release --strip
meson compile -C build
sudo meson install -C build --tags runtime
```

Meson will download and compile required dependencies automatically for you.
