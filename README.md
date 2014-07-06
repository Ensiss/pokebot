pokebot
==========

**pokebot** is a bot playing Pokemon Fire Red. It uses the libvbamcore, provided with vbam to emulate a Game Boy Advance.

Everything under the **vbam** directory is taken from the original vbam source code.


#Dependecies

##vbam

You will need to have `libgtkmm-2.4-dev`, `libgtkglextmm-x11-1.2-dev`, and `libsdl1.2-dev` installed first

```
$ git clone git@github.com:mirror/vbam.git
$ cd vbam/trunk
$ cmake .
$ make
$ mkdir lib; mv *.a lib
```

Then move the `lib/` directory in the root directory of the pokebot

##Pokemon Fire Red GBA ROM

Found on [CoolRom](http://coolrom.com/roms/gba/14488/Pokemon_FireRed.php)

# Contribute

If you want to contribute you can send [pull requests](https://github.com/Ensiss/pokebot/pulls) or report [issues](https://github.com/Ensiss/pokebot/issues).

To learn how the project works, some ressources are available in the [wiki](https://github.com/Ensiss/pokebot/wiki).
