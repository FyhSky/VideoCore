//
//  ViewController.h
//  SampleBroadcaster
//
//  Created by liuley on 16/02/26.
//  Copyright © 2016 LEY. All rights reserved.
//

#import "ViewController.h"
#import "VCSimpleSession.h"
#import <GLKit/GLKit.h>
@interface ViewController ()<VCSessionDelegate>

@property (weak, nonatomic) IBOutlet UIButton *connectButton;
@property (weak, nonatomic) IBOutlet UIView *previewView;
@property (nonatomic, strong) VCSimpleSession *session;
@property (nonatomic, strong) NSString *pushUrl;
@property (weak, nonatomic) IBOutlet UILabel *videosizeLabel;
@end

@implementation ViewController

- (IBAction)pressedConnect:(id)sender {
    switch (self.session.rtmpSessionState) {
        case VCSessionStateNone:
        case VCSessionStatePreviewStarted:
        case VCSessionStateEnded:
        case VCSessionStateError:
            [self.session startRtmpSessionWithURL:self.pushUrl];
            break;
        default:
            [self.session endRtmpSession];
            break;
    }
}
- (IBAction)pressedFilter:(id)sender {
    switch (_session.filter) {
        case VCFilterNormal:
            [_session setFilter:VCFilterGray];
            break;
        case VCFilterGray:
            [_session setFilter:VCFilterInvertColors];
            break;
        case VCFilterInvertColors:
            [_session setFilter:VCFilterSepia];
            break;
        case VCFilterSepia:
            [_session setFilter:VCFilterFisheye];
            break;
        case VCFilterFisheye:
            [_session setFilter:VCFilterGlow];
            break;
        case VCFilterGlow:
            [_session setFilter:VCFilterNormal];
            break;
        default:
            break;
    }
}
- (IBAction)changeCameraState:(id)sender {
    switch (_session.cameraState) {
        case VCCameraStateBack:
            _session.cameraState = VCCameraStateFront;
            break;
        case VCCameraStateFront:
            _session.cameraState = VCCameraStateBack;
            break;
        default:
            break;
    }
}
- (void)viewDidLoad {
    [super viewDidLoad];
    self.pushUrl = @"rtmp://xxxxx";
}

-(void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    if (!self.session) {
        self.session = [[VCSimpleSession alloc] initWithCurrentStatus];
        self.session.delegate = self;
        self.session.previewView.frame = CGRectMake(0, 0, self.session.videoSize.width, self.session.videoSize.height);
        self.videosizeLabel.text = [NSString stringWithFormat:@"%.2f x %.2f", self.session.videoSize.width, self.session.videoSize.height];
        [self.previewView addSubview:self.session.previewView];
    }
}

-(void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

-(void)viewDidLayoutSubviews
{
    CGFloat y = self.session.videoSize.height * self.previewView.bounds.size.width / self.session.videoSize.width;
    if (y < self.previewView.bounds.size.height) {
        self.session.previewView.frame = CGRectMake(0, (self.previewView.bounds.size.height - y)*0.5, self.previewView.bounds.size.width, y);
    } else {
        CGFloat x = self.session.videoSize.width * self.previewView.bounds.size.height / self.session.videoSize.height;
        self.session.previewView.frame = CGRectMake((self.previewView.bounds.size.width - x)*0.5, 0, x, self.previewView.bounds.size.height);
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - 
- (void) connectionStatusChanged: (VCSessionState) sessionState
{
    dispatch_async(dispatch_get_main_queue(), ^{
        switch(sessionState) {
            case VCSessionStateStarting:
                [self.connectButton setTitle:@"正在连接" forState:UIControlStateNormal];
                break;
            case VCSessionStateStarted:
                [self.connectButton setTitle:@"断开连接" forState:UIControlStateNormal];
                break;
            default:
                [self.connectButton setTitle:@"连接" forState:UIControlStateNormal];
                break;
        }
    });
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    return UIInterfaceOrientationLandscapeLeft;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskLandscapeLeft | UIInterfaceOrientationMaskLandscapeRight;
}
@end
