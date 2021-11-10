#include <demo.h>

using namespace std;
// velocity move has a max trajectory points of 6
// trajectory pose also has a maximum (not sure) so set body pose at the end of each method
// uses cubic interpolation for the trajectory

// struct for all the movement data parameters
double *point = new double[n_items];
Trajectory3D trajPose; // trajectory pose used for setting pos and pitch,roll,yaw
// start trajectory time
int ctr = 0;
// trajectory increment might have to check if there is a visible difference between 250, 500, and 1000 milliseconds
int incr = 1000;


// move the spot - should call each time after changing body movement (not position)
// should still call even if set movement to 0 (or no longer moving)
void issueMove(Spot &spot) {
	// stand or move based on velocity and angular rotation
	if (point[velY] == 0 && point[velX] == 0 && point[rot] == 0){
		spot.stand();
	}
	else {
		spot.velocityMove(point[velX], point[velY], point[rot], ctr+=incr, FLAT_BODY, false);
		
	}
}

// run body pose trajectory by setting the body pose and telling spot to stand
// trajectory seems to have some limit so should run trajectory at least twice 
void runTrajectory(Spot &spot) {
	spot.setBodyPose(trajPose, true);
	spot.stand();
}

// walk in a circle counterclock-wise
void walkInCircleCounter(Spot &spot, bool firstTime) {
	// speed forward (positive)
	point[velX] = 2.0;
	// direction of rotation (counterclockwise)
	point[rot] = 1.5;
	spot.velocityMove(point[velX], point[velY], point[rot], ctr+=incr, FLAT_BODY, firstTime);
}

// walk in circle clockwise
void walkInCircleClockwise(Spot &spot) {
	// speed forward (positive)
	point[velX] = 2.0;
	// direction of rotation (clockwise)
	point[rot] = -1.5;
	spot.velocityMove(point[velX], point[velY], point[rot], ctr+=incr, FLAT_BODY, false);
}

// sit with front facing upwards and bottom pointed downwards
void sit(Spot &spot) {
	// tilt torso backwards along z axis
	point[pitch] = -3.14/7;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
	runTrajectory(spot);
}

// wag tail by moving bottom to the left and right
void wagMotion(Spot &spot) {
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
void wagLeft(Spot &spot) {
	point[yaw] = 3.14/8;
	point[roll] = -3.14/16;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
}

// one wag to the right
void wagRight(Spot &spot) {
	point[yaw] = -3.14/8;
	point[roll] = 3.14/16;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
}


// point front of torso downwards and point bottom upwards
void playBow(Spot &spot) {
	// tilt torso forward along z axis
	point[pitch] = (3.14*3)/14;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
	runTrajectory(spot);
}


// spin counterclockwise in one place
void spinCounterClock(Spot &spot) {
	// set the rotation to 1
	// so turn the torso 1 (radian?) counterClockwise from current body frame  
	point[rot] = 1;
	spot.velocityMove(point[velX], point[velY], point[rot], ctr+=incr, FLAT_BODY, false);
}

// spin clockwise in one place
void spinClockwise(Spot &spot) {
	// set the rotation to 1
	// so turn the torso -1 clockwise from current body frame  
	point[rot] = -1;
	spot.velocityMove(point[velX], point[velY], point[rot], ctr+=incr, FLAT_BODY, false);
}

// reset all the positions of the body 
void reset(Spot &spot) {
	// this for some reason messes up the entire trajectory even though the time should be correct
	point[posX] = 0;
	point[posY] = 0;
	point[posZ] = 0;
	point[pitch] = 0;
	point[roll] = 0;
	point[yaw] = 0;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
	runTrajectory(spot);
	
}

// reset the pitch to 0
void resetPitch(Spot &spot) {
	// using this instead of general reset for wag demo 
	point[pitch] = 0;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch], point[yaw], ctr+=incr);
	runTrajectory(spot);
}

void printPosition(Spot &spot) {
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
void wagTail(Spot &spot) {
	int numWags = 6;
	// cubic interpolation
	trajPose.setPosInterp(true);
	for(int i = 0; i < numWags; i ++) {
		// pack wag tail motion left and right n times
		// might change so that changes point[yaw] and point[roll] and not placing number directly into the method
		trajPose.addPointRPY(point[posX], point[posY], point[posZ], -3.14/16, point[pitch], 3.14/8, ctr+=incr); // left
		trajPose.addPointRPY(point[posX], point[posY], point[posZ], 3.14/16, point[pitch], -3.14/8, ctr+=incr); // right
	}
	runTrajectory(spot);
}

// wags tail while in play bow mode
void playBowWagTail(Spot &spot) {
	// tilt face downwards
	point[pitch] = 3.14/7;
	trajPose.addPointRPY(point[posX], point[posY], point[posZ], point[roll], point[pitch],point[yaw], ctr+=incr);
	runTrajectory(spot);
	// wag tail
	wagTail(spot);
}

void wagDemo(Spot &spot) {
	// cubic interpolation (false is linear interpolation)
	trajPose.setPosInterp(true);
	// sit while wagging tail
	sit(spot);
	wagTail(spot);
	// normal position and wag tail
	resetPitch(spot);
	wagTail(spot);
	// wag tail while in play bow
	playBow(spot);
	wagTail(spot);
	// reset pitch so stands straight
	resetPitch(spot);		
}

void circleDemo(Spot &spot) {
	// walk in a circle in both directions 
	// 12 makes a complete circle, 6 is a half circle, etc.
	for(int i = 0; i < 12; i++) {
		walkInCircleCounter(spot, false);
	}
	// spin to the opposite direction
	for(int i = 0; i < 8; i++) {
		spinClockwise(spot);
	}
	// now walk in a circleclockwise
	for(int i = 0; i < 12; i++) {
		walkInCircleClockwise(spot);
	}
	// reset the velocity and rot to 0
	point[velX] = 0;
	point[rot] = 0;
	// some reason need an issue move here? or will skip directly to velx and rot being 0
	issueMove(spot);
}

// demo code to spin in both directions
void spinDemo(Spot &spot) {
	// spin both directions i times
	std::wcout << "ctr: " << ctr << std::endl;
	int LOOP = 8;
	// spin clockwise 8 times
	for(int i = 0; i < LOOP; i++) {
		spinClockwise(spot);
	}
	// spin counterClockwise 8 times
	for(int i = 0; i < LOOP; i++) {
		spinCounterClock(spot);
	}
	// reset rot to 0
	point[rot] = 0;
	spot.velocityMove(point[velX], point[velY], point[rot], ctr+=incr, FLAT_BODY, false);
}

// walk a number of steps forward
void walkForward(Spot &spot) {
	int steps = 6;
	point[velX] = 1.0;
	// MOVE 5 steps forward with speed of 1.0 
	for(int i = 0; i < steps; i++) {
		spot.velocityMove(point[velX], point[velY], point[rot], ctr+=incr, FLAT_BODY, false);
	}
}

// walk a number of steps backwards
void walkBackward(Spot &spot) {
	int steps = 6;
	// negative x velocity is backwards
	point[velX] = -1.0;
	// call velocity move the number of times spot should move
	for(int i = 0; i < steps; i++) {
		spot.velocityMove(point[velX], point[velY], point[rot], ctr+=incr, FLAT_BODY, false);
	}
}

// main function for running Spot clients
int main(int argc, char *argv[]) {
	assert(argc == 3);

	// get username and password
	const std::string username = argv[1];
	const std::string password = argv[2];

	// spotbase testing code
	Spot spot; // spot object
	spot.basicInit(username, password);
	
	// stand and wait (otherwise wobbles when stands to reach trajectory)
	// maybe if set initial trajectory to a larger number will not need usleep
	spot.stand();

	// std::shared_ptr<ClientLayer::RobotStateClient> robotStateClient = spot.getRobotStateClient();
    // RobotStateResponse robotStateResponse = robotStateClient->getRobotState();
	// RobotState robotState = robotStateResponse.robot_state();
	// KinematicState kinematicState = robotState.kinematic_state();
	usleep(1000000);
	std::shared_ptr<CoreLayer::SpotBase> _spotBase;
	_spotBase->getTimeSyncThread()->getEndpoint()->robotTimestampFromLocalTimestamp(TimeUtil::GetCurrentTime());
	//wagTail(spot);
	//walkInCircleCounter(spot);
	// 12 makes a complete circle, 6 is a half circle, etc.
	walkInCircleCounter(spot, true);
	 for(int i = 0; i < 24; i++) {
	 	walkInCircleCounter(spot, false);
	// 	std::wcout << i << std::endl;
	 }
	// usleep(1000000);
	// wagTail(spot);
	
	/*
	// main function to run
	bool keepRunning = true;

	// fill the point vector with body enums to 0
	fill_n(point, n_items, 0);

	// main code to run demo
	//note - why isn't walk running?? something wrong with plotting trajectory with velocity move
	walkForward();
	
	circleDemo();

	spinDemo();

	wagDemo();
	for(;;){
		circleDemo();
	}

	// for some reason will make the trajectory actually execute and not skip straight to returning
	// maybe if place code in a while loop will not need usleep
	usleep(500);
	*/
	return 0;
}

