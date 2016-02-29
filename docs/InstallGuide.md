##使用VideoCore说明
###问题1
`pod install`**sample/SampleBroadcaster**项目时，采用cocoapods v0.38.2，更高的pod版本有问题。
###问题2
编译项目**sample/SampleBroadcaster**，target为SampleBroadcaster的时候提示：

```
Undefined symbols for architecture arm64:
"videocore::Apple::H264Encode::~H264Encode()",
...
ld: symbol(s) not found for architecture arm64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```
> 原因是：

工程中没有将`VideoCore/transforms/Apple/`目录下的`H264Encode.h`和`H264Encode.mm`添加进来。

> 解决方法：

手动将上述两个文件添加到`SampleBroadcaster.xcworkspace`的项目`Pods.xcodeproj`中的`Development Pods/VideoCore/transforms/Apple/`文件夹下。