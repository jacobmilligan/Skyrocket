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
#include "Skyrocket/Platform/Platform.hpp"

#import "Skyrocket/Platform/macOS/macOS.h"

#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>

@implementation MetalView

-(id)initWithFrame:(NSRect)frameRect {
    if ( (self = [super initWithFrame:frameRect]) ) {
        [self initDevice];
    }
    
    return self;
}

-(void)initDevice {
    [self setWantsLayer:YES];
    
    _metalLayer = [CAMetalLayer layer];
    _device = MTLCreateSystemDefaultDevice();
    _metalLayer.device = _device;
    _metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    
    _commandQueue = [_device newCommandQueue];
    
    [self setLayer:_metalLayer];
}

-(void)refresh {
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    id<CAMetalDrawable> drawable = _metalLayer.nextDrawable;
    id<MTLTexture> texture = drawable.texture;
    
    if ( !drawable )
        return;
    
    MTLRenderPassDescriptor* passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    passDescriptor.colorAttachments[0].texture = texture;
    passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(1, 0, 0, 1);
    
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    
    id<MTLRenderCommandEncoder> commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:passDescriptor];
    [commandEncoder endEncoding];
    
    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
    
    [pool drain];
}

-(BOOL)acceptsFirstResponder {
    return YES;
}

-(BOOL)acceptsFirstMouse:(NSEvent *)event {
    return YES;
}

-(void)keyDown:(NSEvent *)event {
    auto keycode = [event keyCode];
    self.app->on_key(sky::Platform::get_vk(keycode));
}

@end
