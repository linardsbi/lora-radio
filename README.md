# LORA RADIO
If by chance you stumble upon this repo, be warned that to run this code, you need a compiler that supports features from C++17 and C++20 concepts (e.g. GCC 10.1 and later).

For an ESP8266 using PlatformIO this is quite tedious. I have not tested if other Arduino compilers can run it.
You must first compile a compatible toolchain from [the ESP open SDK](https://github.com/pfalcon/esp-open-sdk). To do this, add these lines to *crosstool-config-overrides* 

```
CT_LIBC_NEWLIB_ENABLE_TARGET_OPTSPACE=y
CT_GDB_CROSS_EXTRA_CONFIG_ARRAY="--with-guile=guile-2.0"
CT_CC_GCC_VERSION="10.1.0"
CT_CC_GCC_4_8_or_later=y
CT_ISL_VERSION="0.15"
CT_GDB_V_7_10=n
CT_GDB_VERSION="9.2"
```

and then replace the old xtensa toolchain in your PlatformIO installation.

Next, disable compilation with C++11 by removing the build flag `-std=c++11` in 
`.platformio/packages/framework-arduinoespressif8266/tools/platformio-build.py`

Then you're going to have to define a delete operator anywhere in the project like so: 
```C++
inline void operator delete(void *ptr, unsigned int) { operator delete(ptr); }
```

Lastly, add `-std=c++2a` as a build flag to the project, and everything should work.
