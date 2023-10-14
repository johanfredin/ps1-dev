# About #

Collection of hello world projects for the ps1 running a "modern" version of the [psy-q sdk](https://www.retroreversing.com/official-playstation-1-software-development-kit-(psyq)/) originally developed by Psygnosis in 1993. Migrated from initial 
[psx-dev](https://github.com/johanfredin/psx-dev) project. The old project ran the old psyq setup with the original 16bit binaries and (without a big headache) required either a 32 bit windows or using third party libs to be able to run on a 64bit os. Not the most user friendly experience.

This project runs a more modern setup (courtesy of the [psx.dev community](https://www.psx.dev/)) and no longer requires the old 16bit libraries, or windows
to compile! 

This repo is a trial and error work in progress where I mess around with the ps1 and learning C at the same time. I have been messing around with the ps1 on and off now for a couple of years as a fun side project and figured I might 
as well share some of it with whoever might find it useful. Here is the structure:  

* All projects are found under the (duh) [projects](gs_projects) section. They contain the basic drawing of sprites, primitives, dialog boxes and playing audio. 
There is also a parser for the [Tiled](https://www.mapeditor.org/) map editor that is far from perfect, but gets the job done. The main project is the from-scratch project that is like an ugly 2d zelda clone.
* Common project specific libraries under the [lib](lib) section is a common work in progress where once I find something useful I add to. Not sure if I will keep using LIBDS and/or LIBGS in the future but thats what im running now.
* The [tools](tools) section contains a windows .exe version of mkpsxiso. If you'd rather use that you can update the make files inside the projects to use that one instead. The dlls are a requirement for that mkpsxiso version. Besides that are files required for building a ps1 cd.
* The [target](target) section is where the iso files end up. 

## Installation ##

These projects all generate a PS1 ISO file. Those ISOs are generated using [mkpsxiso](https://github.com/Lameguy64/mkpsxiso) which is an awesome tool
by Lameguy64.

The laziest way to get the required dependencies on windows and linux is to just 
Get the PSX.dev vscode extension, launch the panel and get the required(or recommended) dependencies
for psy-q under the TEMPLATES section. Then create a new psy-q example project. This project
will then contain the required psyq libraries under the `third-party` folder required for this project. Simply drag and drop it into this 
projects' [third-party](third_party) section. There is an awesome [youtube](https://www.youtube.com/watch?v=KbAv-Ao7lzU) video
explaining the required steps. Once again courtesy of the courtesy of the [psx.dev community](https://www.psx.dev/)

### Linux ### 

If you don't want to use the vs-code extension here are the steps for linux (Ubuntu flavours):

Besides gcc and make that should already be installed, you need to install the `gcc-mipsel` tools
```shell
 sudo apt-get install gcc-mipsel-linux-gnu g++-mipsel-linux-gnu binutils-mipsel-linux-gnu
 ```
Download [mkpsxiso](https://github.com/Lameguy64/mkpsxiso) and run the .deb file. 

Now download an emulator. Both [no$psx](https://problemkaputt.de/psx.htm) (windows only) and [pcsx-redux](https://pcsx-redux.consoledev.net/) have been working fine for me.

### Honorable mentions ###

Here are some awesome places/people you should look into to get you started with psx development
* [psx.dev](https://www.psx.dev/) - not sure how long they have been around, but they have a great discord channel where people are more than willing to help out. They are also behind the new pcsx-redux emulator and the very helpful vs-code extension. I'd suggest you start there! They also have a new sdk called psyqo that lets you make ps1 games with a more modern c++ setup. 
* [psxdev.net](http://www.psxdev.net/) - has been around a long time and are rather quick to help you out. You can also get the psyq sdk here and there are many good examples
* [Lameguy64](https://github.com/Lameguy64) - the creator of another popular sdk NoobSDK. Knows a ton about the playstation and have a couple of great tools (like mkpsxiso) and a newer version of the TIMEdit software for making ps1 sprites.
* [Wituz youtube series](https://www.youtube.com/watch?v=RFu0iYazIeo&list=PLAQybJIBW2UtXJITyUTJipPFxC61Tt-Xv) - this was where I initially started. Tutorials are quite old and use the older psyq setup. But the examples work just fine and is a great way to start out.
* [psx.arthus.net](https://psx.arthus.net/starting.html) - holds links to many great places (like a couple of the ones above)
* [orionsoft](http://onorisoft.free.fr/retro.htm?psx/psx.htm) - has his own library with some great examples. 


