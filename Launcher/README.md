# DOOMdumper Launcher for DOOM Eternal (WIP)

## Purpose
Simplifies the process of receiving updates for DOOM Eternal.  

When DOOMdumper is updated for a new game version, the version number at https://sewsam.github.io/deternal_version will be incremented.  
The launcher will check for this when run (unless specified through a cmdline arg).   
If the remote version number is the same, the game will be launched normally as if nothing   happened.   
If it is not, the game launch will be interrupted by a messagebox notifying the user that there is an update. The user will then be prompted to delete the current installation, backup mods? (even though they may not work anymore), and then automatically taken to the store to reinstall the game.  
A registry value at `HKLM\SOFTWARE\DOOMdumper` will also be set to indicate that the game is no longer installed(??)

<br>
<br>

### todo:
remember to check certain things in doomdumper when dumping later 
such as:  
- if both `DOOMEternalx64vk.exe` and `appxmanifest.xml` (don't need `doom_dumper` anymore)
- hashes (??)
- if the registry indicates that DOOMEternal should be currently installed (??)
