# <img src="https://user-images.githubusercontent.com/4319621/36906314-e3f99680-1e35-11e8-90fd-f959c9641f36.png" alt="Notepad" width="32" height="32" /> Notepad [![Build Status](https://travis-ci.com/notepadng/notepadng.svg?branch=master)](https://travis-ci.com/notepadng/notepadng) [![notepadng](https://snapcraft.io/notepadng/badge.svg)](https://snapcraft.io/notepadng)

> [!WARNING]  
> This project is not actively maintained anymore. New maintainers are welcome.
>
> It has been reported that with the most recent OS/Qt versions, the program can crash unexpectedly. Use this at your own risk.
> 
>  -- Daniele

### Links

* [What is it?](#what-is-it)
* [Build it yourself](#build-it-yourself)
* [Download it](#distribution-packages)

#### What is it?

Notepad is a text editor designed by developers, for developers. 

![screenshot_20180302_163505](https://notepadng.com/s/images/snapshot1.png)

Please visit our [Wiki](https://github.com/notepadng/notepadng/wiki) for more screenshots and details.

Build it yourself
-----

| Build dependencies    | Dependencies      |
|-----------------------|-------------------|
| Qt 5.6 or higher      | Qt 5.6 or higher  |
| qtwebengine5-dev      | qtwebengine5      |
| libqt5websockets5-dev | libqt5websockets5 |
| libqt5svg5-dev        | libqt5svg5        |
| qttools5-dev-tools    | coreutils         |
| libuchardet-dev       | libuchardet       |
| pkg-config            |                   |

#### Get the source

    $ git clone --recursive https://github.com/notepadng/notepadng.git
    $ cd notepadng

#### Build

    notepadng$ ./configure --prefix /usr
    notepadng$ make

If you encounter errors make sure to have the necessary libraries installed. For Ubuntu you can do that using apt-get:

    sudo apt-get install qttools5-dev-tools qtwebengine5-dev libqt5websockets5-dev libqt5svg5 libqt5svg5-dev libuchardet-dev pkg-config

For CentOS:

    sudo yum install -y qt5-qtbase-devel qt5-qttools-devel qt5-qtwebengine-devel qt5-qtwebsockets-devel qt5-qtsvg-devel uchardet qt5-qtwebchannel-devel pkgconfig

#### Install

You can run notepadng from its build output folder. If however you want to install it, first build it
by following the above steps, then run:

    notepadng$ sudo make install

#### Qt

If the newest version of Qt isn't available on your distribution, you can use the [online installer](http://www.qt.io/download-open-source) to get the latest libraries and install them into your home directory (`$HOME/Qt`). Notepad will automatically use them.

Distribution Packages
---------------------

#### Ubuntu, Debian, and others:

    sudo apt install notepadng

#### Snap

To install the latest stable version:

    sudo snap install notepadng

You don't have the `snap` command? Follow the instructions at https://docs.snapcraft.io/core/install and then install Notepad as shown above.

You can follow the unstable development releases from the "edge" channel.

#### Arch Linux (community-maintained)
Notepad is available from Arch's [community repositories](https://www.archlinux.org/packages/community/x86_64/notepadng/). To install using pacman:

    sudo pacman -S notepadng

Alternatively it can be found in the AUR:

 * Development (git version): [notepadng-git](https://aur.archlinux.org/packages/notepadng-git/)

#### OpenSUSE (community-maintained)
Notepad is avilable in OpenSUSE's main repository:

     sudo zypper in notepadng

#### Solus (community-maintained)
Notepad is available in the `shannon` (stable) repository:

     sudo eopkg it notepadng

#### Others
Use a package for a compatible distribution, or build from [source](https://github.com/notepadng/notepadng.git).
If you want to submit a package: https://github.com/notepadng/notepadng-packaging

#### Compiling on macOS
Instructions can be found [here](https://github.com/notepadng/notepadng/wiki/Compiling-Notepad-on-macOS).
