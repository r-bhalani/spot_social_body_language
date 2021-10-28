#include <demo.h>

using namespace std;

// struct for all the movement data parameters
double *point = new double[n_items];
Trajectory3D trajPose; // trajectory pose used for setting pos and pitch,roll,yaw
Spot spot; // spot object
int ctr = 0;
int incr = 500;
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
	runTrajectory();
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
	runTrajectory();
}


// spin counterclockwise in one place
void spinCounterClock() {
	point[rot] = 1;
	issueMove();
}

// spin clockwise in one place
void spinClockwise() {
	point[rot] = -1;
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
	trajPose.addPointRPY(0, 0, 0, 0, 0, 0, ctr+=10*incr);
	runTrajectory();
	
}


void resetPitch() {
	point[pitch] = 0;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
	runTrajectory();
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

//closing comment here*/



// general wagging motion total
void wagTail() {
	trajPose.setPosInterp(true);
	for(int i = 0; i < 6; i ++) {
		trajPose.addPointRPY(point[posX], point[posY], point[posZ], -3.14/16, point[pitch], 3.14/8, ctr+=incr);
		trajPose.addPointRPY(point[posX], point[posY], point[posZ], 3.14/16, point[pitch], -3.14/8, ctr+=incr);
	}
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], 0, point[pitch], 0, ctr+=incr);
	runTrajectory();
}

// wags tail while in play bow mode
void playBowWagTail() {
	point[pitch] = 3.14/7;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch],point[yaw], ctr+=incr);
	runTrajectory();
	wagTail();
}

void wagDemo() {
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
	resetPitch();		
}

void circleDemo() {
	// walk in a circle in both directions 
	// 12 makes a complete circle, 6 is a half circle, etc.
	for(int i = 0; i < 12; i++) {
		walkInCircleCounter();
	}
	/*for(int i = 0; i < 8; i++) {
		spinClockwise();
	}*/
	//usleep(750000);
	
	//point[velX] = 0;
	//point[rot] = 0;
	//usleep(500000);
}

void spinDemo() {
	// spin both directions i times
	int LOOP = 8;
	for(int i = 0; i < LOOP; i++) {
		spinClockwise();
	}
	for(int i = 0; i < LOOP; i++) {
		// spin counter clockwise i times
		spinCounterClock();
	}
	//point[rot] = 0;
	//issueMove();
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
	//wagDemo();
	//circleDemo();
	
	circleDemo();
	for(int i = 0; i < 12; i++) {
		std::wcout << "ctr: " << ctr << std::endl;
		walkInCircleClockwise();
	}
	//point[rot] = 0;
	//issueMove();
	//spinDemo();
//	spot.sit();
	//wagTail();
//	while(keepRunning) {
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
	//}
	
	//closing comment should be here


	usleep(5000000);
	return 0;
}
