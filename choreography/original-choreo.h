/*
    demo.h: header file for demo code
*/
#ifndef ORIGINAL_CHOREO_H
#define ORIGINAL_CHOREO_H


#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <streambuf>
#include <assert.h>
#include <map>
#include <set>
#include <stdlib.h>

#include <functional>
#include <sys/ioctl.h> //For FIONREAD.
#include <termios.h>

#include <spot/spot.h>
#include <chrono>


#define WALK_BACKWARD 0
#define WALK_FORWARD 1
#define WALK_LEFT 2
#define WALK_RIGHT 3
#define ROTATE 4



enum {
    posX, 
    posY,
    posZ,
    pitch, // z axis
    roll, // x axis
    yaw, // y axis
    velX, // move forward (positive) / backward (negative) 
    velY, // move left / right
    rot, // turn clockwise(negative) and counterclockwise (positive)
    n_items
} movement;

#endif