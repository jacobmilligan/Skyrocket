#!/usr/bin/env python3

if __name__ == '__main__':
    left = 0.0
    right = 800.0
    bottom = 0.0
    top = 600.0
    near = 0.00999999977
    far = 1000.0

    sky_out = [0.00249999994, 0, 0, 0,
               0, 0.00333333341, 0, 0,
               0, 0, -0.00200002012, 0,
               -1.0, -1.0, -1.00002003, 1.0]
    real_out = [2.0 / (right - left), 0, 0, 0,
                0, 2.0 / (top - bottom), 0, 0,
                0, 0, -2.0 / (far - near), 0,
                -((right + left) / (right - left)), -((top + bottom) / (top - bottom)), -((far + near) / (far - near)), 1.0]
    for e in sky_out:
        print('{}'.format(e))

    print('')

    for e in real_out:
        print('{}'.format(round(e, 11)))
