---

# Sopa
**A Stopwatch implement in C kinda like (or exactly like) [sowon](https://github.com/tsoding/sowon)**

---

## Dependencies
- [SDL2](https://www.libsdl.org/download-2.0.php)

- [SDL_image.h](https://www.libsdl.org/projects/SDL_image/)

---

## Quick Start

### Linux

##### Install dependencies

---

```bash
$ sudo apt-get install libsdl2-dev
$ sudo apt-get install libsdl2-image-dev
```

##### Compile and run the project

---

```bash
$ ./build.sh --release
$ ./sopa 
```

---

### Windows

---

### MacOS

---

I don't know how, I'm not that rich to buy a MacBook or something.

---

### Syntax

---

```bash
./sopa **[Flags]** **[Time]**
```

---

#### Flags

---
| Flags        | Usage                                        |
| ------------ | -------------------------------------------- |
| -h or --help | Display help messages                        |
| -p           | Start the clock in pause state               |
| -r           | Start the clock by counting in reverse order |

The `-p` flag and `-r` flag can be combinded with each other to start the clock 
in pause state and counting backward.

---

#### Time format

---

| Time Format             | Usage                                                                   |
| ----------------------- | ----------------------------------------------------------------------- |
| 1) number               | Start the clock from `number` seconds                                   |
| 2) hh:mm:ss             | Start the clock from `hh` hours `mm` minutes and `ss` seconds           |
| 3) xx`d`xx`h`xx`m`xx`s` | Start the clock from `xx` days `xx` hours `xx` minutes and `xx` seconds |

The 2) format can have some arguments ommited.
    Eg: 69:4 -> convert to 69:04:00, or even 69: -> convert to 69:00:00
The 3) format can also have some arguments ommited, and can be in any order.
    Eg: 2d, 2s9h3m, ...
And every format has overflown protection which means you can input 69:420:96 and it will format it correctly

---

### Examples

---

```bash
$ ./sopa 69420
```
```bash
$ ./sopa -r 2001s29h05m
```
```bash
$ ./sopa -rp 69:420:96
```

---

### Control

---

| Key                          | Usage                      |
| ---------------------------- | -------------------------- |
| <kbd>SPACE</kbd>             | Pause or unpause the clock |
| <kbd>r</kbd>                 | Reset the clock            |
| <kbd>q</kbd>                 | Quit                       |
| <kbd>-</kbd>                 | Zoom out                   |
| <kbd>=</kbd> or <kbd>+</kbd> | Zoom in                    |
| <kbd>0</kbd> or <kbd>+</kbd> | Zoom back to the original  |
| <kbd>F11</kbd>               | Toggle full screen         |

---

## License
[MIT](./LICENSE)
