# <img src="https://raw.githubusercontent.com/kravich/notepadng/master/images/notepadng.svg" alt="Notepadng" width="32" height="32" /> Notepadng

Notepadng is a Notepad++-like editor for programmers and power users.

Notepadng is fork of a [Notepadqq](https://github.com/notepadqq/notepadqq) editor with Web-based editor component replaced by Scintilla for better user experience, stability and maintainability.
It also incorporates original color themes from [Notepad++](github.com/notepad-plus-plus/notepad-plus-plus) itself to replicate brand Notepad++ look and feel.

## Manual build

| Dependency            | Debian/Ubuntu package  |
|-----------------------|------------------------|
| CMake                 | cmake                  |
| pkg-config            | pkgconf                |
| Qt6 Core              | qt6-base-dev           |
| Qt6 Gui               |                        |
| Qt6 Widgets           |                        |
| Qt6 PrintSupport      |                        |
| Qt6 DBus              |                        |
| Qt6 Network           |                        |
| Qt6 Xml               |                        |
| Qt6 Svg               | qt6-svg-dev            |
| Qt6 Core5Compat       | qt6-5compat-dev        |
| Qt6 LinguistTools     | qt6-l10n-tools         |
| QScintilla            | libqscintilla2-qt6-dev |
| uchardet              | libuchardet-dev        |

### Get the source

    $ git clone https://github.com/kravich/notepadng.git
    $ cd notepadng

### Build

    notepadng$ cmake -DCMAKE_INSTALL_PREFIX=/usr .
    notepadng$ make

If you encounter errors make sure to have the necessary libraries installed. For Ubuntu you can do that using apt:

    sudo apt install cmake qt6-base-dev qt6-svg-dev qt6-5compat-dev qt6-l10n-tools libqscintilla2-qt6-dev libuchardet-dev

### Install

You can run notepadng from its build output folder. If however you want to install it, first build it
by following the above steps, then run:

    notepadng$ sudo make install

## Distribution Packages

> Not yet available. The plans are to provide native Ubuntu, Debian and Arch packages as well as Snap and Flatpack distributions
