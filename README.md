# cuteui (WIP)

A simple GUI framework with GTK-like layouting and some cool features such as Mica support on Windows 11

## Features

- [ ] GTK-like layouting (WIP)
- [x] Small binary size (around 300 KiB without any compression)
- [x] Hardware-accelerated rendering
- [x] Alpha channel support through composition, so you can make translucent windows (for example, with blur or Mica effect)
- [x] All objects are reference counted using custom intrusive RcPtr
- [ ] Theming based on 9-patch images (WIP)
- [ ] Text rendering using only system libraries and optional custom ttf fonts
- [ ] Custom rendering viewports (for example, OpenGL / Direct3D view with 3D scene)
- [ ] Docking
- [ ] Custom header bars that properly implemented and integrated with WM
- [ ] Linear space rendering with gamma correction

## Backends

- Win32 (Direct3D 11 + DirectComposition rendering)