pokebot
==========

**pokebot** is a bot playing Pokemon Fire Red. It uses the libvbamcore, provided with vbam to emulate a Game Boy Advance.

Everything under the **vbam** directory is taken from the original vbam source code.


#Dependecies

##vbam

You will need to have `libgtkmm-2.4-dev`, `libgtkglextmm-x11-1.2-dev`, and `libsdl1.2-dev` installed first

```
$ git clone https://github.com/mirror/vbam.git
$ cd vbam/trunk
$ cmake .
$ make
$ mkdir lib; mv *.a lib
$ mv lib /path/to/pokebot/root/directory/
```

##pokebot

`liblua5.1-0-dev` is needed for the scripting interface of the pokebot.

##Pokemon Fire Red GBA ROM (U) (V1.0)

You can find it easily on the internet. Remember to get the V1.0 and **not** V1.1 as using this version causes the program to crash.

The md5sum of the right file is `e26ee0d44e809351c8ce2d73c7400cdd`

# Contribute

If you want to contribute you can send [pull requests](https://github.com/Ensiss/pokebot/pulls) or report [issues](https://github.com/Ensiss/pokebot/issues).

To learn how the project works, some ressources are available in the [wiki](https://github.com/Ensiss/pokebot/wiki).
