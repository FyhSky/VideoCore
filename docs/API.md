## Video Core

###参数详情
参数名|参数类型|含义|推流后是否可变|默认值|取值范围
--|--|--|--|--
videoSize|CGSize|视频分辨率|否|无(必须初始化)|宽高均大于0
bitrate|int|视频编码码率(单位为bps)|否|无(必须初始化)|大于100000
fps|int|视频采集帧率|否|无(必须初始化)|大于1，小于等于30
useInterfaceOrientation|BOOL|是否使用应用的竖直方向作为视频的竖直方向|否|NO|YES,NO
cameraState|VCCameraState|视频采集所用摄像头种类|是（调用switchCamera）|VCCameraStateBack|VCCameraStateFront, VCCameraStateBack
orientationLocked|BOOL|是否锁定视频正方向|是|YES|YES,NO
torch|BOOL|是否开启闪光灯|是|NO|YES,NO
videoZoomFactor|float|视频缩放比例|是|1.0|大于0， 小于等于1.0
audioChannelCount|int|音频采集编码过程中所使用的声道数|否|2|1,2
audioSampleRate|float|音频采集编码过程中所使用的采样率|否|44100.0|44100.0,22050.0
micGain|float|音量增益因子|是|1.0|大于等于0， 小于等于1.0
focusPointOfInterest|CGPoint|自动对焦时摄像头的对焦焦点|是|(0.5,0.5)|(0,0)到(1,1)， (0,0)表示左上角， (1,1)表示右下角
exposurePointOfInterest|CGPoint|摄像头的测光中心点|是|(0.5,0.5)|(0,0)到(1,1)， (0,0)表示左上角， (1,1)表示右下角
continuousAutofocus|BOOL|是否开启自动对焦|是|YES|YES,NO
continuousExposure|BOOL|是否开启动态测光|是|YES|YES,NO
useAdaptiveBitrate|BOOL|是否开启可变码率选项（暂不支持）|否|NO|YES,NO
aspectMode|VCAspectMode|视频画面对窗口的填充方式|是|VCAspectModeFit|VCAspectModeFit, VCAscpectModeFill
filter|VCFilter|实时滤镜效果|是|VCFilterNormal|VCFilterNormal, VCFilterGray, VCFilterInvertColors, VCFilterSepia, VCFilterFisheye, VCFilterGlow, VCFilterGussBlur