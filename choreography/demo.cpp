/*#include <demo.h>

using namespace std;

// struct for all the movement data parameters
double *point = new double[n_items];
Trajectory3D trajPose; // trajectory pose used for setting pos and pitch,roll,yaw
Spot spot; // spot object
int ctr;
int incr = 1000;
//Call this at program start to setup for kbhit.
void initTerminalInput()
{
	//Disable internal buffering.
	std::wcout << std::unitbuf;

	//Turn off line buffering.
	struct termios term;
	tcgetattr(0, &term);
	term.c_lflag &= ~ICANON;
	tcsetattr(0, TCSANOW, &term);
	setbuf(stdin, NULL);
}

//Returns 0 if there's no input character to read.
int kbhit()
{
	static int nbbytes;
	ioctl(0, FIONREAD, &nbbytes);
	return nbbytes;
}

static wchar_t getWCharClean()
{
  static wchar_t inputWChar;
  do
  {
    //Wait until there's an input character.
    while (!kbhit())
    {
    }
    inputWChar = getwchar();
	//Erase the valid character.
	std::wcout << L"\b \b";
    break;
  } while (true);
  return inputWChar;
}

// move the spot - should call each time after changing body position
void issueMove() {
	// issue move
	if (point[velY] == 0 && point[velX] == 0 && point[rot] == 0){
		spot.stand();
		
	}
	else {
		spot.velocityMove(point[velX], point[velY], point[rot], ctr+=incr, FLAT_BODY);
		
	}
}

void runTrajectory() {
	spot.setBodyPose(trajPose, true);
	spot.stand();
	ctr = 0;
}

// walk in a circle counterclock-wise
void walkInCircleCounter() {
	// speed
	point[velX] = 2.0;
	// direction of rotation (counterclockwise)
	point[rot] = 1.5;
	issueMove();
}

// walk in circle clockwise
void walkInCircleClockwise() {
	// speed
	point[velX] = 2.0;
	// direction of rotation (clockwise)
	point[rot] = -1.5;
	issueMove();
}

// sit with front facing upwards and bottom pointed downwards
void sit() {
	point[pitch] = -3.14/7;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
}

// wag tail by moving bottom to the left once, right twice, and back to the left to center
void wagMotion() {
	// twist 2 right
	point[yaw] = -3.14/8;
	point[roll] = 3.14/16;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
	// twist left
	point[yaw] = 3.14/8;
	point[roll] = -3.14/16;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
}

// one wag to the left 
void wagLeft() {
	point[yaw] = 3.14/8;
	point[roll] = -3.14/16;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
}

// one wag to the right
void wagRight() {
	point[yaw] = -3.14/8;
	point[roll] = 3.14/16;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
}


// point front of torso downwards and point bottom upwards
void playBow() {
	point[pitch] = (3.14*3)/14;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
}


// spin counterclockwise in one place
void spinCounterClock() {
	point[rot] += 1;
	issueMove();
}

// spin clockwise in one place
void spinClockwise() {
	point[rot] -= 1;
	issueMove();
}

// reset all the positions of the body 
void reset() {
	point[posX] = 0;
	point[posY] = 0;
	point[posZ] = 0;
	point[pitch] = 0;
	point[roll] = 0;
	point[yaw] = 0;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
}

void printPosition() {
	//printing position changes
	std::wcout << "Positions" << std::endl;
	std::wcout << "velX: " << point[velX] << std::endl;
	std::wcout << "velY: " << point[velY] << std::endl;
	std::wcout << "rot: " << point[rot] << std::endl;
	std::wcout << "roll: " << point[roll] << std::endl;
	std::wcout << "pitch: " << point[pitch] << std::endl;
	std::wcout << "yaw: " << point[yaw] << std::endl;
	std::wcout << "---------------" << std::endl;
}
/*

    posY,
    posZ,
    pitch,
    roll,
    yaw,
    velX,
    velY,
    rot,

//closing comment here



// general wagging motion total
void wagTail() {
	trajPose.setPosInterp(true);
	for(int i = 0; i < 4; i ++) {
		trajPose.addPointRPY(point[posX], point[posY], point[posZ], -3.14/16, point[pitch], 3.14/8, ctr+=incr);
		trajPose.addPointRPY(point[posX], point[posY], point[posZ], 3.14/16, point[pitch], -3.14/8, ctr+=incr);
	}
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], 0, point[pitch], 0, ctr+=incr);
	point[yaw] = 0;
	point[pitch] = 0;
}

// wags tail while in play bow mode
void playBowWagTail() {
	point[pitch] = 3.14/7;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch],point[yaw], ctr+=incr);
	wagTail();
}

void wagDemo() {
	// sit while wagging tail
	sit();
	std::wcout << "sit" << std::endl;
	wagTail();
	std::wcout << "wag" << std::endl;
	// normal position and wag tail
	runTrajectory();
	ctr = 0;
	reset();
	std::wcout << "reset" << std::endl;
	wagTail();
	std::wcout << "wag" << std::endl;
	// wag tail while in play bow
	playBow();
	std::wcout << "playbow" << std::endl;
	wagTail();
	std::wcout << "wag" << std::endl;
	reset();		
}

void circleDemo() {
	// walk in a circle in both directions 
	// 8 makes a complete circle, 4 is a half circle, etc.
	for(int i = 0; i < 8; i++) {
		walkInCircleCounter();
	}
	for(int i = 0; i < 8; i++) {
		walkInCircleClockwise();
	}
}

void spinDemo() {
	for(int i = 0; i < 2; i++) {
		// spin both directions i times
		spinClockwise();
		spinCounterClock();
	}
}

void walk() {
	point[velX] = 1.0;
	// MOVE 5 steps forward with speed of 1.0 
	for(int i = 0; i < 5; i++) {
		issueMove();
	}
}

// main function for running Spot clients
int main(int argc, char *argv[]) {
	assert(argc == 3);

	// get username and password
	const std::string username = argv[1];
	const std::string password = argv[2];

	// spotbase testing code
	spot.basicInit(username, password);
	
	// stand and wait
	spot.stand();
	usleep(1000000);

	// main function to run
	bool keepRunning = true;

	double *point = new double[n_items];
	fill_n(point, n_items, 0);

	wagDemo();
	runTrajectory();
	/*
	while(keepRunning) {
		// walk to human then sit in front of them
		//walk();
		//sit();
		// do motions demos
		//circleDemo();
		//spinDemo();
		// reset then lie down
		//reset();
		//spot.sit();
		// break loop
		//keepRunning = false;
	}
	
	//closing comment should be here


	usleep(1000000);
	return 0;
}
*/


#include <spot_teleop.h>

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

#include <spot/robot_layer.h>
#include <spot/spot.h>

//#include <spot/clients/robot_state.h>
//using namespace std;

double posX = 0;
double posY = 0;
double posZ = 0;
double pitch = 0;
double roll = 0;
double yaw = 0;
double velX = 0;
double velY = 0;
double rot = 0;
Trajectory3D trajPose;
Spot spot;

//Call this at program start to setup for kbhit.
void initTerminalInput()
{
	//Disable internal buffering.
	std::wcout << std::unitbuf;

	//Turn off line buffering.
	struct termios term;
	tcgetattr(0, &term);
	term.c_lflag &= ~ICANON;
	tcsetattr(0, TCSANOW, &term);
	setbuf(stdin, NULL);
}

//Returns 0 if there's no input character to read.
int kbhit()
{
	static int nbbytes;
	ioctl(0, FIONREAD, &nbbytes);
	return nbbytes;
}

static wchar_t getWCharClean()
{
  static wchar_t inputWChar;
  do
  {
    //Wait until there's an input character.
    while (!kbhit())
    {
    }
    inputWChar = getwchar();
	//Erase the valid character.
	std::wcout << L"\b \b";
    break;
  } while (true);
  return inputWChar;
}

// move the spot - should call each time after changing body position
void issueMove() {
	// issue move
	if (velY == 0 && velX == 0 && rot == 0 ){
		spot.stand();
		
	}
	else {
		spot.velocityMove(velX, velY, rot, 500, FLAT_BODY);
		
	}
	usleep(500000);
}

// walk in a circle counterclock-wise
void walkInCircleCounter() {
	// speed
	velX = 2.0;
	// direction of rotation (counterclockwise)
	rot = 1.5;
	issueMove();
}

// walk in circle clockwise
void walkInCircleClockwise() {
	// speed
	velX = 2.0;
	// direction of rotation (clockwise)
	rot = -1.5;
	issueMove();
}

// sit with front facing upwards and bottom pointed downwards
void sit() {
	pitch = -3.14/7;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();
}

// wag tail by moving bottom to the left once, right twice, and back to the left to center
void wagTail() {
	// twist 2 right
	yaw = -3.14/8;
	roll = 3.14/16;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();
	// twist left
	yaw = 3.14/8;
	roll = -3.14/16;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();
}

// one wag to the left 
void wagLeft() {
	yaw = 3.14/8;
	roll = -3.14/16;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();
}

// one wag to the right
void wagRight() {
	yaw = -3.14/8;
	roll = 3.14/16;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();
}

// point front of torso downwards and point bottom upwards
void playBow() {
	pitch = (3.14*3)/14;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();
}

// wags tail while in play bow mode
void playBowWagTail() {
	pitch = 3.14/7;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();
	wagLeft();
	issueMove();
	wagTail();
	issueMove();
	wagRight();
	issueMove();
}

// testing if the yaw changes
void yawChange() {
	yaw -= 3.14/16;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();
}

// testing if the roll changes
void rollChange() {
	roll += 3.14/16;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();

}

// spin counterclockwise in one place
void spinCounterClock() {
	rot = 1;
	issueMove();
}

// spin clockwise in one place
void spinClockwise() {
	rot = -1;
	issueMove();
}

// reset all the positions of the body 
void reset() {
	posX = 0;
	posY = 0;
	posZ = 0;
	pitch = 0;
	roll = 0;
	yaw = 0;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();
}

void printPosition() {
	//printing position changes
	std::wcout << "Positions" << std::endl;
	std::wcout << "velX: " << velX << std::endl;
	std::wcout << "velY: " << velY << std::endl;
	std::wcout << "rot: " << rot << std::endl;
	std::wcout << "roll: " << roll << std::endl;
	std::wcout << "pitch: " << pitch << std::endl;
	std::wcout << "yaw: " << yaw << std::endl;
	std::wcout << "---------------" << std::endl;
}

void wagDemo() {
	// sit while wagging tail
	sit();
	wagLeft();
	for(int i = 0; i < 6; i++) {
		wagTail();
	}
	wagRight();
	usleep(500000);
	// normal position and wag tail
	reset();
	usleep(500000);
	wagLeft();
	for(int i = 0; i < 6; i++) {
		wagTail();
	}
	wagRight();
	// wag tail while in play bow
	playBow();
	wagLeft();
	for(int i = 0; i < 6; i++) {
		wagTail();
	}
	wagRight();
	usleep(500000);
	reset();		
	usleep(500000);
}

void circleDemo() {
	// walk in a circle in both directions 
	// 8 makes a complete circle, 4 is a half circle, etc.
	for(int i = 0; i < 16; i++) {
		walkInCircleCounter();
		// need a sleep otherwise will not go to next call properly
		usleep(200000);
	}
	usleep(500000);
	for(int i = 0; i < 16; i++) {
		walkInCircleClockwise();
		usleep(200000);
	}
	usleep(500000);
}

void spinDemo() {
	// spin both directions i times
	int LOOP = 30;
	for(int i = 0; i < LOOP; i++) {
		spinClockwise();
	}
	for(int i = 0; i < LOOP; i++) {
		// spin both directions i times
		spinCounterClock();
	}
	rot = 0;
	issueMove();
}

void walk() {
	velX = 1.0;
	// MOVE 5 steps forward with speed of 1.0 
	for(int i = 0; i < 10; i++) {	
		issueMove();
	}		
	velX = 0;
	issueMove();
}

// main function for running Spot clients
int main(int argc, char *argv[]) {
	assert(argc == 3);

	// get username and password
	const std::string username = argv[1];
	const std::string password = argv[2];

	// spotbase testing code
	spot.basicInit(username, password);
	
	// stand and wait
	spot.stand();
	usleep(700000);

	// main function to run
	bool keepRunning = true;

	while(keepRunning) {
		// walk to human then sit in front of then
		walk();
		//sit();
		// do motions demos
		wagDemo();
		circleDemo();
		spinDemo();
		// reset then lie down
		reset();
		spot.sit();
		// break loop
		keepRunning = false;
	}

	return 0;
}