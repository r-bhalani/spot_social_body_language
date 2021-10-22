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

using namespace std;

//#include <spot/clients/robot_state.h>
//using namespace std;

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
	bool keepRunning = true;
	
	double posX = 0;
	double posY = 0;
	double posZ = 0;

	double pitch = 0;
	double roll = 0;
	double yaw = 0;

	double velX = 0;
	double velY = 0;
	double velZ = 0;

	double angVelX = 0;
	double angVelY = 0;
	double angVelZ = 0;
	
	initTerminalInput();

	bool keepGoing = true;
	while(keepGoing) {
		static wchar_t inputWChar;
		if(kbhit()) {
			inputWChar = getwchar();
			//cout << inputWChar << endl;

			switch(inputWChar) {
				case 'q':
					posX += 0.1;
					break;
				case 'a':
					posX -= 0.1;
					break;
				case 'w':
					posY += 0.1;
					break;
				case 's':
					posY -= 0.1;
					break;
				case 'e':
					posZ += 0.1;
					break;
				case 'd':
					posZ -= 0.1;
					break;
					
				case 'r':
					velX += 0.1;
					break;
				case 'f':
					velX -= 0.1;
					break;
			}
		}


		Trajectory3D trajPose;
		cout
			<< "posX:	" << posX
			<< "posY:	" << posY
			<< "posZ:	" << posZ

			<< "velX:	" << velX
			<< "posY:	" << posY
			<< "posZ:	" << posZ
			<< endl;
		trajPose.addPointRPYVel(posX, posY, posZ, roll, pitch, yaw, 1,
				velX, velY, velZ, angVelX, angVelY, angVelZ);
		spot.setBodyPose(trajPose, true);
		//sleep(1);
	} //really jank while loop ending
	return 0;
}

/*
	double rot = 0;

	//char commands[] = "sssswwb"; //testing the function
	char commandsFake[] = "cccccccccb"; //walk in a circle
	char commands[] = "kkmnnmmnnmmnnmmnnmmnnmmnnmmnnmmnnmmnnb"; //sit wag tail
	char commands2[] = "kkkb"; //sitting
	char commands3[] = "iiib"; //play bow
	char commands4[] = "iimnnmmnnmmnnmmnnmmnnmmnnmmnnmmnnmmnnb"; //play bow wag tail
	char commands5[] = "wkkmnnmmnnmmnnmmnnmmnniiiimnnmmnnmmnnmmnnmmnncb"; //interact with person

	for(int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
		//wchar_t wchar = getWCharClean();
		
		// initialize velocities and angular velocity (rot)

		// xy translation
		int flag = 0;

		if (commands[i] == 'w') {
			velX += 1.0;
			flag = 1;
		}
		if (commands[i] == 'a') {
			velY += 1.0;
			flag = 1;
		}
		if (commands[i] == 's') {
			velX -= 1.0;
			flag = 1;
		}
		if (commands[i] == 'd') {
			velY -= 1.0;
			flag = 1;
		}
		if (commands[i] == 'q') {
			rot += 0.5;
			flag = 1;
		}
		if (commands[i]== 'e') {
			rot -= 0.5;
			flag = 1;
		}

		// orientation (once we figure out)

		if (commands[i]== 'i') {
			pitch += 3.14/14;
			flag = 0;
		}
		if (commands[i]== 'j') {
			roll += 3.14/16;
			flag = 0;
		}
		if (commands[i] == 'k') {
			pitch -= 3.14/14;
			flag = 0;
		}
		if (commands[i] == 'l') {
			roll -= 3.14/16;
			flag = 0;
		}
		if (commands[i]== 'u') {
			yaw -= 3.14/16;
			flag = 0;
		}
		if (commands[i]== 'o') {
			yaw += 3.14/16;
			flag = 0;
		}

		// height / positions ?
		if (commands[i] == 'r') {
			posX += 0.2;
			flag = 0;
		}
		if (commands[i] == 'f') {
			posX -= 0.2;
			flag = 0;
		}
		if (commands[i] =='t') {
			posY += 0.2;
			flag = 0;
		}
		if (commands[i] =='g') {
			posY -= 0.2;
			flag = 0;
		}
		if (commands[i]== 'y') {
			posZ += 0.2;
			flag = 0;
		}
		if (commands[i]== 'h') {
			posZ -= 0.2;
			flag = 0;
		}
		
		
		// keys with multiple moves at once
		if (commands[i]== 'm') {
			yaw += 3.14/8;
			roll -= 3.14/16;
			velX+= 1.0;
			flag = 0;
			std::wcout << "combining O and L" << std::endl;
		}
		if (commands[i] == 'n') {
			yaw -= 3.14/8;
			roll += 3.14/16;
			velX -= 1.0;
			flag = 0;
			std::wcout << "combining U and J" << std::endl;
		}
		if (commands[i] == 'c') {
			velX += 2.0;
			flag = 1;
			rot += 1.5;
		}
		

		// exit
		if (commands[i] == 'b') {
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
		if (commands[i] == 'v') {
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
		std::wcout << "KEYPRESS: " << commands[i] << std::endl;
		 Trajectory3D trajPose;
		 //trajPose.addPointRPY(posX, posY, posZ, roll, pitch, yaw, 1);
		 trajPose.addPointRPYVel(posX, posY, posZ, roll, pitch, yaw, 1,
		    velX, velY, velZ, angVelX, angVelY, angVelZ);
		
		 spot.setBodyPose(trajPose, true);

		 //spot.setBodyPose(trajPose, true, velX, velY, rot, 500, FLAT_BODY);
		 
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
		if (velY == 0 && velX == 0 && rot == 0 ){
			spot.stand();
			
		}
		else {
			//spot.velocityMove(velX, velY, rot, 500, FLAT_BODY);
			
		}

		sleep(1);
	}
		*/