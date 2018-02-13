//
//  NSGLView.mm
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 7/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/Renderer/OpenGL/NSGLView.h"
#include "Skyrocket/Graphics/Renderer/OpenGL/GLGDI.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Graphics/Apple/MacViewport.h"

namespace sky {


void OpenGLGDI::set_clear_color(const Color& color)
{
    auto view = viewport_->get_native_handle()->view;

    [view setBackingColor:((CGFloat) color.r) / 255.0
                        g:((CGFloat) color.g) / 255.0
                        b:((CGFloat) color.b) / 255.0
                        a:((CGFloat) color.a) / 255.0];
}


}

@implementation NSGLView

-(id)initWithFrame:(NSRect)frameRect {
    if ( [super initWithFrame:frameRect] ) {

    }
    return self;
}

-(void)setBackingColor:(CGFloat)r
                     g:(CGFloat)g
                     b:(CGFloat)b
                     a:(CGFloat)a
{
    glClearColor((GLfloat)r, (GLfloat)g, (GLfloat)b, (GLfloat)a);
}

@end