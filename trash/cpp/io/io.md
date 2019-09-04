# io

``` C++
open(filename, mode);
```

|mode|usage|
|:--:|--|
ios::in|input
ios::out|output
ios::binary|open as binary file
ios::ate|open at the end of the file
ios::app|all output operations are performed at the end of the file
ios::trunc|delete previous content and replace by new one

iostream是控制窗口输入输出。
fstream是文件输入输出。
sstream是字符串输入输出（内存中）。