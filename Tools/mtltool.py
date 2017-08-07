#!/usr/bin/env python3

import os
import glob
import argparse
import subprocess


if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='mtltool', description='Tool for compiling metal shaders '
                                                                 'and building libraries')
    parser.add_argument('--input', '-i', type=str, help='The directory holding the .metal files',
                        required=True)
    parser.add_argument('--out', '-o', type=str, help='The compiled library\'s output directory',
                        required=True)

    args = parser.parse_args()

    input_dir = os.path.realpath(os.path.join(os.getcwd(), args.input))
    output_dir = os.path.realpath(os.path.join(os.getcwd(), args.out))

    if not os.path.exists(output_dir):
        os.mkdir(output_dir)

    shader_files = glob.glob(os.path.join(input_dir, '*.metal'))

    air_out = os.path.join(output_dir, 'Lib.air')
    shader_out = os.path.join(output_dir, 'Lib.metallib')

    cmd = ['xcrun', '-sdk', 'macosx', 'metal', '-Wall'] + shader_files + ['-o', air_out]
    subprocess.call(cmd)

    cmd = ['xcrun', '-sdk', 'macosx', 'metallib', air_out, '-o', shader_out]
    subprocess.call(cmd)

    os.remove(air_out)
