#include <demo.h>

using namespace std;
// velocity move has a max trajectory points of 6
// trajectory pose also has a maximum (not sure) so set body pose at the end of each method
// uses cubic interpolation for the trajectory

// struct for all the movement data parameters
double *point = new double[n_items];
Trajectory3D trajPose; // trajectory pose used for setting pos and pitch,roll,yaw
Spot spot; // spot object
int ctr = 0;
// might have to check if there is a visible difference between 250, 500, and 1000
int incr = 500;

/*
// terminal input code, not necessary for demo
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
} */


// move the spot - should call each time after changing body movement (not position)
// should still call even if set movement to 0 (or no longer moving)
void issueMove() {
	// stand or move based on velocity and angular rotation
	if (point[velY] == 0 && point[velX] == 0 && point[rot] == 0){
		spot.stand();
	}
	else {
		spot.velocityMove(point[velX], point[velY], point[rot], ctr+=incr, FLAT_BODY);
		
	}
}

// run body pose trajectory by setting the body pose and telling spot to stand
// trajectory seems to have some limit so should run trajectory at least twice 
void runTrajectory() {
	spot.setBodyPose(trajPose, true);
	spot.stand();
}

// walk in a circle counterclock-wise
void walkInCircleCounter() {
	// speed forward (positive)
	point[velX] = 2.0;
	// direction of rotation (counterclockwise)
	point[rot] = 1.5;
	issueMove();
}

// walk in circle clockwise
void walkInCircleClockwise() {
	// speed forward (positive)
	point[velX] = 2.0;
	// direction of rotation (clockwise)
	point[rot] = -1.5;
	issueMove();
}

// sit with front facing upwards and bottom pointed downwards
void sit() {
	// tilt torso backwards along z axis
	point[pitch] = -3.14/7;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
	runTrajectory();
}

// wag tail by moving bottom to the left once, right twice, and back to the left to center
void wagMotion() {
	// twist to the right
	point[yaw] = -3.14/8;
	point[roll] = 3.14/16;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
	// twist to the left
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
	// tilt torso forward along z axis
	point[pitch] = (3.14*3)/14;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
	runTrajectory();
}


// spin counterclockwise in one place
void spinCounterClock() {
	// set the rotation to 1
	// so turn the torso 1 (radian?) counterClockwise from current body frame  
	point[rot] = 1;
	issueMove();
}

// spin clockwise in one place
void spinClockwise() {
	// set the rotation to 1
	// so turn the torso -1 clockwise from current body frame  
	point[rot] = -1;
	issueMove();
}

// reset all the positions of the body 
void reset() {
	// this for some reason messes up the entire trajectory even though the time should be correct
	point[posX] = 0;
	point[posY] = 0;
	point[posZ] = 0;
	point[pitch] = 0;
	point[roll] = 0;
	point[yaw] = 0;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
	runTrajectory();
	
}

// reset the pitch to 0
void resetPitch() {
	// using this instead of general reset for wag demo 
	point[pitch] = 0;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
	runTrajectory();
}

void printPosition() {
	// printing position changes to check if correct
	std::wcout << "Positions" << std::endl;
	std::wcout << "velX: " << point[velX] << std::endl;
	std::wcout << "velY: " << point[velY] << std::endl;
	std::wcout << "rot: " << point[rot] << std::endl;
	std::wcout << "roll: " << point[roll] << std::endl;
	std::wcout << "pitch: " << point[pitch] << std::endl;
	std::wcout << "yaw: " << point[yaw] << std::endl;
	std::wcout << "---------------" << std::endl;
}

// general wagging motion total
void wagTail() {
	int numWags = 6;
	// cubic interpolation
	trajPose.setPosInterp(true);
	for(int i = 0; i < numWags; i ++) {
		// pack wag tail motion left and right n times
		// might change so that changes point[yaw] and point[roll] and not placing number directly into the method
		trajPose.addPointRPY(point[posX], point[posY], point[posZ], -3.14/16, point[pitch], 3.14/8, ctr+=incr);
		trajPose.addPointRPY(point[posX], point[posY], point[posZ], 3.14/16, point[pitch], -3.14/8, ctr+=incr);
	}
	// reset the body pose back to center of y and z axis
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], 0, point[pitch], 0, ctr+=incr);
	runTrajectory();
}

// wags tail while in play bow mode
void playBowWagTail() {
	// tilt face downwards
	point[pitch] = 3.14/7;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch],point[yaw], ctr+=incr);
	runTrajectory();
	// wag tail
	wagTail();
}

void wagDemo() {
	// cubic interpolation (false is linear interpolation)
	trajPose.setPosInterp(true);
	// sit while wagging tail
	sit();
	wagTail();
	// normal position and wag tail
	resetPitch();
	wagTail();
	// wag tail while in play bow
	playBow();
	wagTail();
	// reset pitch so stands straight
	resetPitch();		
}

void circleDemo() {
	// walk in a circle in both directions 
	// 12 makes a complete circle, 6 is a half circle, etc.
	for(int i = 0; i < 12; i++) {
		walkInCircleCounter();
	}
	// spin to the opposite direction
	for(int i = 0; i < 8; i++) {
		spinClockwise();
	}
	// now walk in a circleclockwise
	for(int i = 0; i < 12; i++) {
		walkInCircleClockwise();
	}
	// reset the velocity and rot to 0
	point[velX] = 0;
	point[rot] = 0;
	// some reason need an issue move here? or will skip directly to velx and rot being 0
	issueMove();
}

// demo code to spin in both directions
void spinDemo() {
	// spin both directions i times
	std::wcout << "ctr: " << ctr << std::endl;
	int LOOP = 8;
	// spin clockwise 8 times
	for(int i = 0; i < LOOP; i++) {
		spinClockwise();
	}
	// spin counterClockwise 8 times
	for(int i = 0; i < LOOP; i++) {
		spinCounterClock();
	}
	// reset rot to 0
	point[rot] = 0;
	issueMove();
}

// walk a number of steps forward
void walkForward() {
	int steps = 6;
	point[velX] = 1.0;
	// MOVE 5 steps forward with speed of 1.0 
	for(int i = 0; i < steps; i++) {
		issueMove();
	}
}

// walk a number of steps backwards
void walkBackward() {
	int steps = 6;
	// negative x velocity is backwards
	point[velX] = -1.0;
	// call velocity move the number of times spot should move
	for(int i = 0; i < steps; i++) {
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
	
	// stand and wait (otherwise wobbles when stands to reach trajectory)
	// maybe if set initial trajectory to a larger number will not need usleep
	spot.stand();
	usleep(1000000);

	// main function to run
	bool keepRunning = true;

	// fill the point vector with body enums to 0
	fill_n(point, n_items, 0);

	// main code to run demo
	//note - why isn't walk running?? something wrong with velocity move trajectory
	walkForward();
	
	//only runs once and then infinite loops while spot is just standing there
	// does not work due to problems wil trajectory on velocity move
	for(;;){
		circleDemo();
	}

	// for some reason will make the trajectory actually execute and not skip straight to returning
	// maybe if place code in a while loop will not need usleep
	usleep(500);
	return 0;
}


/*
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
	for(int i = 0; i < 8; i++) {
		spinClockwise();
	}
	rot = 0;
	usleep(500000);
	for(int i = 0; i < 16; i++) {
		walkInCircleClockwise();
		usleep(200000);
	}
	velX = 0;
	rot = 0;
	usleep(500000);
}

void spinDemo() {
	// spin both directions i times
	int LOOP = 16;
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
		//issueMove();
		spot.velocityMove(velX, velY, rot, 1000, FLAT_BODY);
		
	
	usleep(500000);
	}		
	velX = 0;
	issueMove();
	velX = -1.0;
	// MOVE 5 steps forward with speed of 1.0 
	for(int i = 0; i < 10; i++) {	
		//issueMove();
		spot.velocityMove(velX, velY, rot, 1000, FLAT_BODY);
		
	
	usleep(500000);
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
		/*walk();
		velX = 0;
		issueMove();*/
		/*for(int i = 0; i < ; i++) {
			spinClockwise();
		}
		usleep(50000);
		rot = 0;
		issueMove();*/
		/*for(int i = 0; i < 10; i++) {	
		//issueMove();
		spot.velocityMove(velX, velY, rot, 1000, FLAT_BODY);
		
	
		usleep(500000);
		}		
		sit();
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
}*/