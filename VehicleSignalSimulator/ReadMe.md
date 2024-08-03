# Console camera app
The Console camera app is not finished yet, this is the demo version. 

There are two executable files now , one is ConsoleInputProcessor used to receive cmd line input and convert the input to udp message and send to the server called MessagelProccessor, the other is MessagelProccessor used to receive udp messages from ConsoleInputProcessor or the Windows cmd app and interact with SvmApp through ACF interfaces(the ACF part is not implemented yet).

The two executable files will be combined into one later. Now two are retained for debugging convenience.
## build
```
cd ConsoleCameraApp
mkdir build
cd build
cmake ..
make
```

## run

### cmd message processor
`./MessagelProccessor/MessagelProccessor`
