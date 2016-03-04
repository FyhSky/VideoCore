## 使用方法
### 添加SDK
#### 方法一：使用CocoaPods

* 在**Podfile**中添加

```
platform:ios, '7.0'
pod 'VideoCore', :git=> 'https://github.com/leyleo/VideoCore.git'
```
* 执行`pod install`
* 打开xcworkspace文件

#### 方法二：使用lib库文件
Todo

### Quick Start
*  引入头文件

```
#import "VCSimpleSession.h"
```
*  初始化

```
self.session = [[VCSimpleSession alloc] initWithCurrentStatus]; // 基于当前网络状况和设备朝向自动初始化
self.session.delegate = self; // 设置代理VCSessionDelegate，用于监听推流连接状态
self.session.previewView.frame = CGRectMake(0, 0, self.session.videoSize.width, self.session.videoSize.height);
[self.view addSubview:self.session.previewView]; // 添加预览视图
```
*  开始推流

```
self.pushUrl = @"rtmp://xxxxx";
[self.session startRtmpSessionWithURL:self.pushUrl];
```
* 结束推流

```
[self.session endRtmpSession];
```
* 监听连接状态

需要添加`VCSessionDelegate`

```
- (void) connectionStatusChanged: (VCSessionState) sessionState
{
    switch(sessionState) {
        case VCSessionStateStarting:
        case VCSessionStateStarted:
        case VCSessionStatePreviewStarted:
        case VCSessionStateEnded:
        case VCSessionStateError:
        case VCSessionStateNone:
    }
}
```
* 切换摄像头

```
self.session.cameraState = VCCameraStateFront;
self.session.cameraState = VCCameraStateBack;
```
* 添加滤镜效果

```
[self.session setFilter: VCFilterNormal];
[self.session setFilter: VCFilterGray];
[self.session setFilter: VCFilterInvertColors];
...
```
## 详细API说明
`VCSimpleSession`类完成了音视频采集，并通过RTMP协议上推到服务端的功能。
#### 初始化
有六种初始化方法，用来设置视频的分辨率、相关品质及其他相关参数：

```
方法一：
/** 
 根据当前网络状态和状态栏的朝向自动初始化，推荐该种使用方式，默认配置了不同分辨率下的品质：
 	当前网络为Wifi条件时，视频为720p，当isPortrait=YES时采用 VCVideoQuality720x1280，NO时采用 VCVideoQuality1280x720；
 	当前为移动网络时，视频为480p，当isPortrait=YES时采用 VCVideoQuality480x640，NO时采用 VCVideoQuality640x480；
 	其他网络状态时，视频为360p，当isPortrait=YES时采用 VCVideoQuality360x480，NO时采用VCVideoQuality480x360.
 */
-(id)initWithCurrentStatus;
```
```
方法二：
/** 
 初始化推流Session，默认配置了不同分辨率下的品质。
 @param quality 视频大小，包括以下几个选项
	VCVideoQuality1280x720,
	VCVideoQuality720x1280,
	VCVideoQuality640x480,
	VCVideoQuality480x640,
	VCVideoQuality480x360,
	VCVideoQuality360x480
 */
-(instancetype) initWithQuality:(VCVideoQuality)quality;
```
```
方法三：
/**
 初始化推流Session
 @param videoSize 视频分辨率大小
 @param fps 帧速率
 @param bps 比特率
 @return VCSimpleSession实例
 */
- (instancetype) initWithVideoSize:(CGSize)videoSize
                         frameRate:(int)fps
                           bitrate:(int)bps;
```
```
方法四：
/**
 初始化推流Session
 @param videoSize 视频分辨率大小
 @param fps 帧速率
 @param bps 比特率
 @param useInterfaceOrientation 是否采用应用的竖朝向作为视频的竖朝向
 @return VCSimpleSession实例
 */
- (instancetype) initWithVideoSize:(CGSize)videoSize
                         frameRate:(int)fps
                           bitrate:(int)bps
           useInterfaceOrientation:(BOOL)useInterfaceOrientation;
```
```
方法五：
/**
 初始化推流Session
 @param videoSize 视频分辨率大小
 @param fps 帧速率
 @param bps 比特率
 @param useInterfaceOrientation 是否采用应用的竖朝向作为视频的竖朝向
 @param cameraState 摄像头类型
 @return VCSimpleSession实例
 */
- (instancetype) initWithVideoSize:(CGSize)videoSize
                         frameRate:(int)fps
                           bitrate:(int)bps
           useInterfaceOrientation:(BOOL)useInterfaceOrientation
                       cameraState:(VCCameraState) cameraState;
```
```
方法六：
/**
 初始化推流Session
 @param videoSize 视频分辨率大小
 @param fps 帧速率
 @param bps 比特率
 @param useInterfaceOrientation 是否采用应用的竖朝向作为视频的竖朝向
 @param cameraState 摄像头类型
 @param aspectMode 画面填充模式
 @return VCSimpleSession实例
 */
- (instancetype) initWithVideoSize:(CGSize)videoSize
                         frameRate:(int)fps
                           bitrate:(int)bps
           useInterfaceOrientation:(BOOL)useInterfaceOrientation
                       cameraState:(VCCameraState) cameraState
                        aspectMode:(VCAspectMode) aspectMode;
```                        
#### 本地预览
通过**VCSimpleSession**的只读属性`UIView *previewView`进行本地摄像头效果预览。类似下面代码：

```
self.session = [[VCSimpleSession alloc] initWithQuality: VCVideoQuality720x1280];
self.session.previewView.frame = self.view.bounds;
[self.view addSubview:self.session.previewView];
```

#### 上推
将流媒体数据以rtmp协议推送到服务器，需要设置推送地址Url。开始推送有两个方法：

```
方法一：
/**
 开始推送RTMP流
 @param rtmpUrl 上推地址，rtmp:// 协议
 @param streamKey 秘钥
 */
- (void) startRtmpSessionWithURL:(NSString*) rtmpUrl
                    andStreamKey:(NSString*) streamKey;
```
```
方法二：
/**
 开始推送RTMP流
 @param rtmpUrl 上推地址，rtmp:// 协议
 */
- (void) startRtmpSessionWithURL:(NSString *)rtmpUrl;
```

#### 结束上推
通过下面方法结束上推流媒体。

```
/** 结束RTMP流推送 */
- (void) endRtmpSession;
```

#### 上推状态检测
通过**VCSessionDelegate**的代理方法完成推送状态的检测，以便更新UI。

```
/** 推流的运行状态改变时的回调方法
 @param sessionState VCSessionState 有以下几种状态
		VCSessionStateNone,
		VCSessionStatePreviewStarted,
		VCSessionStateStarting,
		VCSessionStateStarted,
		VCSessionStateEnded,
		VCSessionStateError
 */
- (void) connectionStatusChanged: (VCSessionState) sessionState;
```

> 注意：在使用上述方法更新UI时，请确保已切换到主线程。

#### 属性参数

|参数名|参数类型|含义|推流后是否可变|默认值|取值范围|
|---|---|---|---|---|
|videoSize|CGSize|视频分辨率|否|无(必须初始化)|宽高均大于0|
|bitrate|int|视频编码码率(单位为bps)|否|无(必须初始化)|大于100000|
|fps|int|视频采集帧率|否|无(必须初始化)|大于1，小于等于30|
|useInterfaceOrientation|BOOL|视频的朝向|否|NO|YES: 以应用的竖直方向为视频的竖直方向，NO: 以设备的竖直方向为视频的竖直方向。|
|cameraState|VCCameraState|视频采集所用摄像头种类|是|VCCameraStateBack|VCCameraStateFront, VCCameraStateBack|
|orientationLocked|BOOL|是否锁定视频方向|否|NO|YES,NO|
|torch|BOOL|是否开启闪光灯|是|NO|YES,NO|
|videoZoomFactor|float|视频缩放比例|是|1.0|大于0， 小于等于1.0|
|audioChannelCount|int|音频采集编码过程中所使用的声道数|否|2|1,2|
|audioSampleRate|float|音频采集编码过程中所使用的采样率|否|44100.0|44100.0,22050.0|
|micGain|float|音量增益因子|是|1.0|大于等于0， 小于等于1.0|
|focusPointOfInterest|CGPoint|自动对焦时摄像头的对焦焦点|是|(0.5,0.5)|(0,0)到(1,1)， (0,0)表示左上角， (1,1)表示右下角|
|exposurePointOfInterest|CGPoint|摄像头的测光中心点|是|(0.5,0.5)|(0,0)到(1,1)， (0,0)表示左上角， (1,1)表示右下角|
|continuousAutofocus|BOOL|是否开启自动对焦|是|YES|YES,NO|
|continuousExposure|BOOL|是否开启动态测光|是|YES|YES,NO|
|aspectMode|VCAspectMode|视频画面对窗口的填充方式|是|VCAspectModeFit|VCAspectModeFit, VCAscpectModeFill|
|filter|VCFilter|实时滤镜效果|是|VCFilterNormal|VCFilterNormal, VCFilterGray, VCFilterInvertColors, VCFilterSepia, VCFilterFisheye, VCFilterGlow|

### Demo

完整见`sample/SampleBroadcaster`项目。

### 相关问题
#### 问题1
`pod install`**sample/SampleBroadcaster**项目时，采用cocoapods v0.38.2，更高的pod版本有问题。
> 解决方法：

```
sudo gem install cocoapods -v 0.38.2 // 安装指定版本pod
pod _0.38.2_ install // 用指定版本的pod创建工程
```

#### 问题2
编译项目**sample/SampleBroadcaster**，target为SampleBroadcaster的时候提示：

```
Undefined symbols for architecture arm64:
"videocore::Apple::H264Encode::~H264Encode()",
...
ld: symbol(s) not found for architecture arm64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```
> 原因：

工程中没有将`VideoCore/transforms/Apple/`目录下的`H264Encode.h`和`H264Encode.mm`添加进来。

> 解决方法：

手动将上述两个文件添加到`SampleBroadcaster.xcworkspace`的项目`Pods.xcodeproj`中的`Development Pods/VideoCore/transforms/Apple/`文件夹下。
