## README FOR LINUX USERS

This is one of my first projects, so the code may not be well readable. But it works!

Run Terminal on your device.

Type path to the directory with .c and .h files. 
```
cd home/yourusername/path/to/nand2tetris-virtual_machine-master 
```

Run 
```
make
```

Put .vm files into directory with executor:

1. To translate SimpleAdd.vm, StackTest.vm, BasicTest.vm, StaticTest.vm, PointerTest.vm, BasicLoop.vm, FibonacciSeries.vm, SimpleFunction.vm execute 
```
./VMtranslate file_to_read.vm file_to_convert.asm
```
- NOTE: the names of files .vm and .asm should be the same!

2. To translate multiple .vm files with Sys.init function execute 
```
./VMtranslate file_to_convert.asm
```
- NOTE: the name of file, that will be converted, should be the same as the name of file with .cmp extension!

