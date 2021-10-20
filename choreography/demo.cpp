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

void walkInCircle() {
	velX += 2.0;
	rot += 1.5;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
}

void sit() {
	pitch -= 3.14/7;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
}

void issueMove() {
	// issue move
	if (velY == 0 && velX == 0 && rot == 0 ){
		spot.stand();
		
	}
	else {
		spot.velocityMove(velX, velY, rot, 500, FLAT_BODY);
		
	}
	sleep(1);
}

void wagTail() {
	//m 
	yaw += 3.14/8;
	roll -= 3.14/16;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();
	sleep(1);
	// 2*n
	yaw -= 3.14/4;
	roll += 3.14/8;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();
	sleep(1);
	//m 
	yaw += 3.14/8;
	roll -= 3.14/16;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
}

void oneWag() {
	yaw += 3.14/8;
	roll -= 3.14/16;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
}

void otherWag() {
	yaw -= 3.14/4;
	roll += 3.14/8;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
}

void playBow() {
	pitch += (3.14*3)/14;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
}

void playBowWagTail() {
	pitch += 3.14/7;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
	issueMove();
	sleep(1);
	wagTail();
}

void yawChange() {
	yaw -= 3.14/16;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
}
void rollChange() {
	roll += 3.14/16;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
}
void reset() {
	pitch = 0;
	posX = 0;
	posY = 0;
	posZ = 0;
	pitch = 0;
	roll = 0;
	yaw = 0;
	trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
	spot.setBodyPose(trajPose, true);
}


// main function for running Spot clients
int main(int argc, char *argv[]) {
	assert(argc == 3);

	// get username and password
	const std::string username = argv[1];
	const std::string password = argv[2];

	// spotbase testing code
	spot.basicInit(username, password);
	//ClientLayer::RobotStateClient data(username, password); 

	// Trajectory2D trajTest;
	// trajTest.addPoint(0.5, 0, 0, 2);
	// //trajTest.addPoint(1, 0, 0, 2);
	// robot.trajectoryMove(trajTest, ODOM, 10);
	// sleep(10);
	spot.stand();
     sleep(1);
	// move
	//initTerminalInput();
	bool keepRunning = true;
	
	while(keepRunning) {
		walkInCircle();
		issueMove();
		sleep(1);
		walkInCircle();
		issueMove();
		sleep(1);
		walkInCircle();
		issueMove();
		sleep(1);
		 //printing position changes
		 std::wcout << "Positions" << std::endl;
		 std::wcout << "velX: " << velX << std::endl;
		 std::wcout << "velY: " << velY << std::endl;
		 std::wcout << "rot: " << rot << std::endl;
		 std::wcout << "roll: " << roll << std::endl;
		 std::wcout << "pitch: " << pitch << std::endl;
		 std::wcout << "yaw: " << yaw << std::endl;
		 std::wcout << "---------------" << std::endl;

	keepRunning = false;	
	}
	return 0;
}
