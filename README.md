# **SEDU Board Configuration Tool**

## Description

With the seducfg tool you can setup your sedu board controller easily via gui.

The current version supports the following features:

* check function, try out various settings, save configuration
* multi platform (Windows, Linux, MacOS)
* multi language (English and German at the moment, translators welcome)

##### Hint for OSX users

If you get timeout errors you should modify your FTDI driver:

* [download and install latest driver](http://www.ftdichip.com/Drivers/VCP.htm)
* open "/Library/Extensions/FTDIUSBSerialDriver.kext/Contents/Info.plist" with your preferred editor as root
* locate sedu entry (e.g. idVendor 1027, idProduct 24577) and add the following (or 250000)

		<key>ConfigData</key>
		<dict>
			<key>BaudRates</key>
			<dict>
				<key>BAUDALL</key>
				<integer>500000</integer>
			</dict>
		</dict>
* disconnect device
* run "sudo kextunload -b com.FTDI.driver.FTDIUSBSerialDriver" and "sudo kextload -b com.FTDI.driver.FTDIUSBSerialDriver"
* reconnect device

##### Hint for Linux users

If you get permission denied errors you should create a udev rule:

* disconnect device
* create file (e.g. "/etc/udev/rules.d/99-sedu.rules") as root add the following line

		KERNEL=="ttyUSB*", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001", MODE="0666"

* reconnect device

## Screenshots

The main window...

![main window](https://raw.github.com/LazyT/seducfg/gh-pages/screenshots/screenshot1.png)

Some infos about the program and possible interactions...

![about dialog](https://raw.github.com/LazyT/seducfg/gh-pages/screenshots/screenshot2.png)

If you like it don't forget to donate some beer! ;-)

The installer...

![setup dialog](https://raw.github.com/LazyT/seducfg/gh-pages/screenshots/screenshot3.png)

## Download Installer

Download and install the latest version for your operating system:

* [Windows](https://github.com/LazyT/seducfg/releases/download/1.0.0/SEDUCFG-1.0.0-win.exe)
* [Linux](https://github.com/LazyT/seducfg/releases/download/1.0.0/SEDUCFG-1.0.0-lin.run)
* [MacOS](https://github.com/LazyT/seducfg/releases/download/1.0.0/SEDUCFG-1.0.0-mac.dmg)

## Build from Source

If the installer binaries doesn't work for you build it yourself:

1) checkout the sourcecode

		git clone https://github.com/LazyT/seducfg

2) change into the new seducfg directory and generate the Makefile

		cd seducfg && qmake

3) compile the sourcecode

		make

GUI fans just install the [Qt-Environment](http://www.qt.io/download-open-source), open the "seducfg.pro" project file and click on the build button.

## Dependencies

SEDUCFG is based on

* [Qt](http://www.qt.io)

Thanks for this great software!

Have a nice day and always an optimal sedulight...


