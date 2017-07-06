//
//  MetalView.mm
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 4/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Framework/Application.hpp"
#include "Skyrocket/Platform/macOS/macOS.h"

#import <AppKit/AppKit.h>

@implementation MetalView

-(id)initWithFrame:(NSRect)frameRect {
    if ( (self = [super initWithFrame:frameRect]) ) {
        [self setWantsLayer:YES];
        
        CGFloat bgColor[] = {0.0, 0.0, 0.0, 1.0};
        _metalLayer = [CAMetalLayer layer];
        
        _metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        _metalLayer.backgroundColor = CGColorCreate(CGColorSpaceCreateDeviceRGB(), bgColor);
        
        [self setLayer:_metalLayer];
    }
    
    return self;
}

-(void)setBackingColor:(CGFloat)r g:(CGFloat)g b:(CGFloat)b a:(CGFloat)a {
    CGFloat bgColor[] = {r, g, b, a};
    _metalLayer.backgroundColor = CGColorCreate(CGColorSpaceCreateDeviceRGB(), bgColor);
}

-(void)setDevice:(nonnull id<MTLDevice>)device {
    
    _metalLayer.device = device;
}

-(void)refresh {
//    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
//    
//    id<CAMetalDrawable> drawable = _metalLayer.nextDrawable;
//    id<MTLTexture> texture = drawable.texture;
//    
//    if ( !drawable )
//        return;
//    
//    MTLRenderPassDescriptor* passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
//    passDescriptor.colorAttachments[0].texture = texture;
//    passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
//    passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
//    passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(1, 0, 0, 1);
//    
//    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
//    
//    id<MTLRenderCommandEncoder> commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:passDescriptor];
//    [commandEncoder endEncoding];
//    
//    [commandBuffer presentDrawable:drawable];
//    [commandBuffer commit];
//    
//    [pool drain];
}

-(BOOL)acceptsFirstResponder {
    return YES;
}

-(BOOL)acceptsFirstMouse:(NSEvent *)event {
    return YES;
}

@end
