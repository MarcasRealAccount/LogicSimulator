# Premake5AppSystem
A premake5 system that allows apps with dependencies with more dependencies

# How it works
With normal premake projects you have a ```premake5.lua``` file that control how premake5 operates in that project.
To make sure that other projects can include another project a fix had to be made.
That's where this premake5 system comes in.

All you need is the ```premake/``` folder
and a ```premakeApp.lua``` file that creates an app and returns it.

To include another project place it in ```Third_Party/[ProjectName]/``` folder and in ```premakeApp.lua``` do
```lua
local [ProjectName] = globalApp.third_party_app("[ProjectName]", currentPath)
```
Be sure to remember to swap ```[ProjectName]``` with the actual project name.

To include a library place it in ```Third_Party/[LibraryName]/``` folder and make a ```Third_Party/[LibraryName].lua``` file where you create an app like is done in ```premakeApp.lua```, then in ```premakeApp.lua``` do
```lua
local [LibraryName] = globalApp.third_party_library("[LibraryName]", currentPath)
```
Be sure to remember to swap ```[LibraryName]``` with the actual library name.

Changing project properties can be done using variables in the app object. For help on what can be done visit the Wiki.

### Q. But how do I change premake stuff myself?<br>
A. Use State object and add that to the app with a filter of your choosing, making the filter = {} will make it work on anything.
