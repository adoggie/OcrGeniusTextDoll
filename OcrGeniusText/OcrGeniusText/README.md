
https://github.com/KHeresy/GenLibFromDll
dumpbin libzmq.dll /exports /out:libzmq.def

enter vs command 
.\GenLibFromDll.exe .\libzmq.dll
