# qt5-beaglebone-black
QT5 binaries / source for beaglebone black with xcb and desktop opengl

You will need the crosscompile tools for armhf or armv7l (whichever debian names them at your current time).

The releases are gzipped tarballs available in the Release section.

You will need to download and mount the beaglebone system image to <bbb_sysroot> and use it in the lines below:

To compile, first configure: ./configure -device linux-beaglebone-g++ -device-option CROSS_COMPILE=arm-linux-gnueabihf- -sysroot <bbb_sysroot> -opensource -confirm-license -release -make libs -prefix /usr/local/qt5bbb -nomake examples -nomake tests -skip multimedia -skip webengine -opengl --recheck-all

Then you can make, make install, and copy over to the target.
