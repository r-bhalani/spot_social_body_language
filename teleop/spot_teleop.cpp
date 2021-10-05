
#include <spot_teleop.h>

#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <streambuf>
#include <assert.h>
#include <map>
#include <set>

#include <functional>
#include <sys/ioctl.h> //For FIONREAD.
#include <termios.h>

//#include <spot/clients/robot_state.h>

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

// main function for running Spot clients
int main(int argc, char *argv[]) {
	assert(argc == 3);

	// get username and password
	const std::string username = argv[1];
	const std::string password = argv[2];

	// spotbase testing code
	Spot spot;
	spot.basicInit(username, password);
	//ClientLayer::RobotStateClient data(username, password); 

	// Trajectory2D trajTest;
	// trajTest.addPoint(0.5, 0, 0, 2);
	// //trajTest.addPoint(1, 0, 0, 2);
	// robot.trajectoryMove(trajTest, ODOM, 10);
	// sleep(10);
	spot.stand();

	// move
	initTerminalInput();
	bool keepRunning = true;
	
	double posX = 0;
	double posY = 0;
	double posZ = 0;
	double pitch = 0;
	double roll = 0;
	double yaw = 0;

	while(keepRunning){
		wchar_t wchar = getWCharClean();
		
		// initialize velocities and angular velocity (rot)
		double velX = 0;
		double velY = 0;
		double rot = 0;

		// xy translation
		int flag = 0;

		if (wchar == L'w') {
			velX += 1.0;
			flag = 1;
		}
		if (wchar == L'a') {
			velY += 1.0;
			flag = 1;
		}
		if (wchar == L's') {
			velX -= 1.0;
			flag = 1;
		}
		if (wchar == L'd') {
			velY -= 1.0;
			flag = 1;
		}
		if (wchar == L'q') {
			rot += 0.5;
			flag = 1;
		}
		if (wchar == L'e') {
			rot -= 0.5;
			flag = 1;
		}

		// orientation (once we figure out)
		if (wchar == L'i') {
			pitch += 3.14/16;
			flag = 0;
		}
		if (wchar == L'j') {
			roll += 3.14/16;
			flag = 0;
		}
		if (wchar == L'k') {
			pitch -= 3.14/16;
			flag = 0;
		}
		if (wchar == L'l') {
			roll -= 3.14/16;
			flag = 0;
		}
		if (wchar == L'u') {
			yaw -= 3.14/16;
			flag = 0;
		}
		if (wchar == L'o') {
			yaw += 3.14/16;
			flag = 0;
		}

		// height / positions ?
		if (wchar == L'r') {
			posX += 0.2;
			flag = 0;
		}
		if (wchar == L'f') {
			posX -= 0.2;
			flag = 0;
		}
		if (wchar == L't') {
			posY += 0.2;
			flag = 0;
		}
		if (wchar == L'g') {
			posY -= 0.2;
			flag = 0;
		}
		if (wchar == L'y') {
			posZ += 0.2;
			flag = 0;
		}
		if (wchar == L'h') {
			posZ -= 0.2;
			flag = 0;
		}
		
		
		// keys with multiple moves at once
		if (wchar == L'm') {
			yaw += 3.14/16;
			roll -= 3.14/16;
			//try roll +=
			flag = 0;
			std::wcout << "combining O and L" << std::endl;
		}
		if (wchar == L'n') {
			yaw -= 3.14/16;
			roll += 3.14/16;
			//try roll -=
			flag = 0;
			std::wcout << "combining U and J" << std::endl;
		}
		

		// exit
		if (wchar == L'b') {
			keepRunning = false;
			// robot.move(sit);
			spot.sit();
			std::wcout << "TURNING OFF. PLEASE STANDBY." << std::endl;
			roll = 0;
			pitch = 0;
			yaw = 0;
			posX = 0;
			posY = 0;
			posZ = 0;
			break;
		}
		if (wchar == L'v') {
			//keepRunning = false;
			// robot.move(sit);
			std::wcout << "RESETTING JOINT POSITIONS" << std::endl;
			roll = 0;
			pitch = 0;
			yaw = 0;
			posX = 0;
			posY = 0;
			posZ = 0;
			
		}
		
		 Trajectory3D trajPose;
		 trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
		 spot.setBodyPose(trajPose, true);
		 
		 //printing position changes
		 std::wcout << "Positions" << std::endl;
		 std::wcout << "velX: " << velX << std::endl;
		 std::wcout << "velY: " << velY << std::endl;
		 std::wcout << "rot: " << rot << std::endl;
		 std::wcout << "roll: " << roll << std::endl;
		 std::wcout << "pitch: " << pitch << std::endl;
		 std::wcout << "yaw: " << yaw << std::endl;
		 std::wcout << "---------------" << std::endl;

		// issue move
		if (velY == 0 && velX == 0 && rot == 0 /*&& flag*/){
			spot.stand();
			
		}
		else {
			spot.velocityMove(velX, velY, rot, 500, FLAT_BODY);
			
		}
	
	}
	return 0;
}

