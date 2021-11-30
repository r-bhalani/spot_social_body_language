/*
    demo.h: header file for demo code
*/
#ifndef DEMO_H
#define DEMO_H


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

#define WAG 0
#define WALK_CLOCKWISE 1
#define SPIN_CLOCKWISE 2


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