/*

 Video Core
 Copyright (c) 2014 James G. Hurley

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 */

/*!
 *  A simple Objective-C Session API that will create an RTMP session using the
 *  device's camera(s) and microphone.
 *
 */

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

@class VCSimpleSession;

/** 推流运行状态 */
typedef NS_ENUM(NSInteger, VCSessionState)
{
    VCSessionStateNone,
    VCSessionStatePreviewStarted,
    VCSessionStateStarting,
    VCSessionStateStarted,
    VCSessionStateEnded,
    VCSessionStateError

};
/** 摄像头类型：VCCameraStateFront, VCCameraStateBack*/
typedef NS_ENUM(NSInteger, VCCameraState)
{
    VCCameraStateFront, // 前置
    VCCameraStateBack // 后置
};
/** 视频画面的填充方式*/
typedef NS_ENUM(NSInteger, VCAspectMode)
{
    VCAspectModeFit, // 保持画面比例，宽高自适应
    VCAscpectModeFill // 填满整个视频窗口
};

/** 滤镜类型*/
typedef NS_ENUM(NSInteger, VCFilter) {
    VCFilterNormal, // 正常模式
    VCFilterGray, // 灰色调
    VCFilterInvertColors, // 反色
    VCFilterSepia, // 怀旧
    VCFilterFisheye, // 鱼眼
    VCFilterGlow // 发光
};

@protocol VCSessionDelegate <NSObject>
@required
/** 推流的运行状态改变时的回调方法
 @param sessionState VCSessionState
 */
- (void) connectionStatusChanged: (VCSessionState) sessionState;
@optional
/** 添加视频源时的回调方法
 @param session VCSimpleSession
 */
- (void) didAddCameraSource:(VCSimpleSession*)session;

- (void) detectedThroughput: (NSInteger) throughputInBytesPerSecond videoRate:(NSInteger) rate;
@end

@interface VCSimpleSession : NSObject
/** 推流的运行状态，readonly */
@property (nonatomic, readonly) VCSessionState rtmpSessionState;
/** 预览视图，readonly */
@property (nonatomic, strong, readonly) UIView* previewView;

/** 视频分辨率，在推流开始运行后不能修改 */
@property (nonatomic, assign) CGSize            videoSize;
/** 视频的比特率，在推流开始运行后不能修改 */
@property (nonatomic, assign) int               bitrate;
/** 视频的帧速率，在推流开始运行后不能修改 */
@property (nonatomic, assign) int               fps;
/** 视频的朝向是否以应用的竖直方向为视频的竖直方向，在推流开始运行后不能修改，readonly */
@property (nonatomic, assign, readonly) BOOL    useInterfaceOrientation;
/** 摄像头类型，默认是后置摄像头 VCCameraStateBack */
@property (nonatomic, assign) VCCameraState cameraState;
/** 摄像头的视频是否锁定方向，在推流开始运行后不能修改 */
@property (nonatomic, assign) BOOL          orientationLocked;
/** 是否开启闪光灯，默认不开启 NO */
@property (nonatomic, assign) BOOL          torch;
/** 视频的缩放比例，取值范围为(0, 1] */
@property (nonatomic, assign) float         videoZoomFactor;
/** 音频的声道，只能为1或者2，默认为2，在推流开始运行后不能修改 */
@property (nonatomic, assign) int           audioChannelCount;
/** 音频的采样频率，只能为44100或者22050，默认为44100.0，在推流开始运行后不能修改 */
@property (nonatomic, assign) float         audioSampleRate;
/** 麦克风的增益因子，取值范围为[0,1]，默认为 1 */
@property (nonatomic, assign) float         micGain;
/** 自动对焦时的对焦焦点，取值范围为[(0,0),(1,1)]，默认为视频中心，(0,0)为左上角，(1,1)为右下角 */
@property (nonatomic, assign) CGPoint       focusPointOfInterest;
/** 自动对焦时的测光中心点，取值范围为[(0,0),(1,1)]，默认为视频中心，(0,0)为左上角，(1,1)为右下角*/
@property (nonatomic, assign) CGPoint       exposurePointOfInterest;
/** 是否开启自动对焦，默认开启 YES */
@property (nonatomic, assign) BOOL          continuousAutofocus;
/** 是否开启动态测光，默认开启 YES */
@property (nonatomic, assign) BOOL          continuousExposure;
/** 是否开启编码过程中动态调整码率，默认关闭 NO，在推流开始运行后不能修改 */
@property (nonatomic, assign) BOOL          useAdaptiveBitrate;
/** 推流时的预判网络带宽 Bytes/Second, readonly */
@property (nonatomic, readonly) int         estimatedThroughput;
/** 视频画面的填充方式，默认为 VCAspectModeFit */
@property (nonatomic, assign) VCAspectMode  aspectMode;
/** 视频的滤镜效果，默认为正常模式 VCFilterNormal */
@property (nonatomic, assign) VCFilter      filter;

@property (nonatomic, assign) id<VCSessionDelegate> delegate;

// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
/**
 开始推送RTMP流
 @param rtmpUrl 上推地址，rtmp:// 协议
 @param streamKey 秘钥
 */
- (void) startRtmpSessionWithURL:(NSString*) rtmpUrl
                    andStreamKey:(NSString*) streamKey;
/** 结束RTMP流推送 */
- (void) endRtmpSession;
/** 获得原始视频画面 */
- (void) getCameraPreviewLayer: (AVCaptureVideoPreviewLayer**) previewLayer;

/** 
 添加水印
 @param image 水印图片
 @param rect 水印的位置和大小。rect.origin指定的是水印的中心点在视频画面中的位置，rect.size指定的是水印在视频画面中的大小
 */
- (void) addPixelBufferSource: (UIImage*) image
                     withRect: (CGRect) rect;

@end
