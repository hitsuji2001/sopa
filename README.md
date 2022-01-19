# Sopa

**A Stopwatch implement in C kinda like (or exactly like) [sowon](https://github.com/tsoding/sowon)**  
and it portable (i think)

## Dependencies
- [SDL2](https://www.libsdl.org/download-2.0.php)

## Demo

![demo](./demo.gif)

## Quick Start

### Linux

##### Install dependencies

```bash
$ sudo apt-get install libsdl2-dev
```

##### Compile and run the project

```bash
$ ./build.sh --release
$ ./sopa 
```

### Windows

##### Install dependencies

- **NOTE**: Assuming you run this in [Visual Studio Code](https://code.visualstudio.com/), have [git](https://git-scm.com/download/) installed and MinGW was installed to `C:\MinGW`

- Download and set up `SDL2`:  
    - Download the `SDL2-devel-2.0.20-mingw.tar.gz` from [here](https://www.libsdl.org/download-2.0.php):
    - Extract it with your tool of choice and go to folder `i686-w64-mingw32`
    - Copy all of the folder's contents to your `C:\MinGW` folder

- Download `pkg-config` for Windows:  
    - Go to [here](http://ftp.gnome.org/pub/gnome/binaries/win32/dependencies/)
    - Download the file `pkg-config_0.26-1_win32.zip`
    - Extract the file `bin/pkg-config.exe` to `C:\MinGW\bin`
    - Download the file `gettext-runtime_0.18.1.1-2_win32.zip`
    - Extract the file `bin/intl.dll` to `C:\MinGW\bin`
    - Go to [here](http://ftp.gnome.org/pub/gnome/binaries/win32/glib/2.28)
    - Download the file `glib_2.28.8-1_win32.zip`
    - Extract the file `bin/libglib-2.0-0.dll` to `C:\MinGW\bin`

- References: [Video tutorial for setting up SDL2](https://www.youtube.com/watch?v=DQ-NBjBFLJ4), [pkg-config Installation](https://stackoverflow.com/questions/1710922/how-to-install-pkg-config-in-windows/25605631)

##### Compile and run the project

```console
> cd path\to\sopa
> ./build.sh --release
> ./sopa
```

### MacOS

##### Install dependencies

- Install `SDL2`:

```bash
$ brew install sdl2
```

- Install `pkg-config`:

```bash
$ brew install pkg-config
```

##### Compile and run the project

```bash
$ ./build.sh --release
$ ./sopa
```

I don't know how, I'm not that rich, can't afford a MacBook or an apple. So I think the command for installation and running is correct.  
If it is incorrect please tell me how to do it :)

### Usage

It's a clock :)

### Syntax

```bash
./sopa [Flags] [Time]
```

#### Flags

| Flags        | Usage                                        |
| ------------ | -------------------------------------------- |
| -h or --help | Display help messages                        |
| -p           | Start the clock in pause state               |
| -r           | Start the clock by counting in reverse order |

The `-p` flag and `-r` flag can be combinded with each other to start the clock 
in pause state and counting backward.

#### Time format

| Time Format             | Usage                                                                   |
| ----------------------- | ----------------------------------------------------------------------- |
| 1) number               | Start the clock from `number` seconds                                   |
| 2) hh:mm:ss             | Start the clock from `hh` hours `mm` minutes and `ss` seconds           |
| 3) xx`d`xx`h`xx`m`xx`s` | Start the clock from `xx` days `xx` hours `xx` minutes and `xx` seconds |

The 2) format can have some arguments ommited.  
    `Eg`: 69:4 -> convert to 69:04:00, or even 69: -> convert to 69:00:00  
The 3) format can also have some arguments ommited, and can be in any order.  
    `Eg`: 2d, 2s9h3m, ...  
And every format has overflown protection which means you can input 69:420:96 and it will format it correctly  

### Examples

```bash
$ ./sopa 69420
$ ./sopa -r 2001s29h05m
$ ./sopa -rp 69:420:96
$ ./sopa -p 3h4m5s
$ ./sopa -r 2d
$ ./sopa -p 69420s
```

---

### Control

| Key                              | Usage                                 |
| -------------------------------- | ------------------------------------- |
| <kbd>SPACE</kbd>                 | Pause or unpause the clock            |
| <kbd>r</kbd>                     | Reset the clock to the original state |
| <kbd>q</kbd>                     | Quit                                  |
| <kbd>-</kbd>                     | Zoom out                              |
| <kbd>=</kbd>   or   <kbd>+</kbd> | Zoom in                               |
| <kbd>0</kbd>                     | Zoom back to the original             |
| <kbd>F11</kbd>                   | Toggle full screen                    |

## License
[MIT](./LICENSE)
