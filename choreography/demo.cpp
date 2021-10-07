
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

//#include <spot/clients/robot_state.h>
using namespace std;

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

static string getWCharClean()
{
   for (string line; getline(cin, line);) {
        cout << line << endl;
		return line;
    }

	return NULL;
}

bool run_motions(char[] commands) {
	bool keepRunning = true;
	while(1) {
		for(int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
			
			// initialize velocities and angular velocity (rot)
			double velX = 0;
			double velY = 0;
			double rot = 0;

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
				flag = 0;
				std::wcout << "combining O and L" << std::endl;
			}
			if (commands[i] == 'n') {
				yaw -= 3.14/8;
				roll += 3.14/16;
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
				return false;
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

			sleep(1);
		}
	}
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
	spot.stand();
     
	// move
	initTerminalInput();
	
	
	double posX = 0;
	double posY = 0;
	double posZ = 0;
	double pitch = 0;
	double roll = 0;
	double yaw = 0;
	bool run = true;
	while(run) {
		string input = getWCharClean();
		if(input == "circles")
			char commands[] = "ccccccccc";
		//more if statements to determine what the user wants to run
		run = run_command(commands);
		
	}
	
	

	//put this into an infinite while loop
	
	
	return 0;
}

