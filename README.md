# DOOMdumper
Utility that streamlines the process of sideloading the UWP Version of DOOM Eternal for modding.

## What does it do?
_**Basically, it makes modding DOOM Eternal on gamepass much easier**_
1. Using `C++/WinRT`, _DOOMdumper_ checks the Package Status of DOOM Eternal to make sure the game isn't unnecessarily dumped.
2. Prompts the User for an installation path, and verifies it.
3. Initiates [UWPDumper](https://github.com/Wunkolo/UWPDumper)/[UWPdumper-lib](https://github.com/SEWsam/UWPDumper-lib) to dump the game, with appropriate error handling.
4. Extracts the latest included `EternalModInjector-UWP`

## Compilation
This project is currently compiled within Visual Studio 2019. It requires `zlib`, and the latest `Windows 10 SDK`. (You may also need to install the `WinRT NuGet Package`)

This project also relies on [UWPdumper-lib](https://github.com/SEWsam/UWPDumper-lib) and [zipper](https://github.com/sebastiandev/zipper), which are included as submodules.  
```
git clone --recurse-submodules https://github.com/SEWsam/DOOMdumper.git
```

The Visual Studio Solution should have everything setup from here, but just make sure that the Build Order has UWPInjector/Dumper and zipper above DOOMdumper.

### Note for Zipper with Visual Studio: 
You may have to add `CDirEntry.h` and `CDirEntry.cpp` to the zipper project, then change 
```cpp 
#include <CDirEntry.h>
```  
to  
```cpp 
#include "CDirEntry.h"
```
You will also need to change `zipper - tools::currentPath()` to use `_getcwd()` instead of `getcwd()`.


---
---
## This vs. UWP-DOOMdumper (old)
This is a C++ rewrite of the original tool-- pain.
### Rationale
Although a Python based tool may be easier to maintain, I feel this rewrite is _(big somewhat)_ justified. The purpose of rewriting this was simple: reduce the invokation of command line tool processes that `DOOMdumper` has no control of; reduce the frequency of issues related to uncaught errors.

##### also partially just because I wanted a starting project as I just recently learned c++

