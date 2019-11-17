# libqgit2e

A Qt/C++ wrapper for libgit2

A C++11 capable compiler is required to compile libqgit2.

Note! This is a fork of libqgit2, for original library look for https://github.com/KDE/libqgit2

Easy subproject with QtCreator, git submodule/subtree

Should compile out of box on modern linux distro with everthing (gcc, qt, libgit2) from official distro repositories. Ubuntu 18.x, 19.x, Linux Mint 19.x now supported.

## Versioning (until relase)

The versioning schema is "vNUM1.NUM2.NUM3". The first part of the version, given by "NUM1.NUM2" follows the libgit2 supported version (so all 0.22.x versions of libqgit2(e) compile against libgit2 0.22.x). The third number increases with maintenance versions of libqgit2(e). Between maintenance versions only libqgit2(e) fixes are allowed (as long as libgit2 maintains its versioning schema).

Each libgit2 version has its own libqgit2 branch, where multiple versions may be tagged, according to the maintenance work done on that release.

Branches are named after the libgit2 release without prefixes (e.g. '0.21', '0.22'), while versions are soft tagged with the 'v' prefix starting with NUM3 equal to 0 (e.g. 'v0.21.0', '0.21.1').

So the decision chart is:

* Did you change the code to compile against a new version of libgit2? Create a new 'NUM1.NUM2' branch and create there a 'NUM1.NUM2.0' tag.
* Did you fix libqgit2 code, solved CMake or compilation issues, changed comments or documentation? Just commit inside the 'NUM1.NUM2' branch you are working in, and create a new maintenance tag increasing NUM3.

The version number follows that of libgit2: libqgit2 0.21.x uses libgit2
0.21.x and libqgit2 0.22.x uses libgit2 0.22.x and so on.

## Versioning (planning after relase)

New version schema will stop follow to libgit2 version, instead it will conditional compiling to support different libgit2 versions
