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
#include "Skyrocket/Graphics/Renderer/OpenGL/GLConfig.hpp"

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