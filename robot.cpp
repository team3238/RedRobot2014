#include <WPILib.h>
#include "robot.h"
#include "portnums.h"
#include "arduinoI2C.h"

robot::robot(void)
{
	I2C = new ArduinoI2C();
	joystick = new Joystick(joystickPort);
	theChassis = new chassis(leftFrontTalonPort, leftRearTalonPort, rightFrontTalonPort, rightRearTalonPort, leftUltrasonicPort, rightUltrasonicPort);
	theCatapult = new catapult(catapultTalonOnePort, catapultTalonTwoPort, catapultEncoderPortA, catapultEncoderPortB, catapultLimitSwitchPort);
	theCollector = new collector(liftingTalonPort, rollerTalonPort, I2C, upperLimitSensorPort, lowerLimitSensorPort);
	AutonomousTimer = new Timer();
	UnfoldingTimer = new Timer();
	AutonomousServo = new Servo(autonomousServoPort);
}

void robot::RobotInit() 
{
	theCatapult->SetMotorPower(1.0);
//	insight_ballDistance.setHeader("Diff:");
//	insight.registerData(insight_ballDistance, 1);
//	insight.startDisplay();
	SmartDashboard::init();
}
	
void robot::DisabledInit() 
{
	theCollector->ReInit();
	theCatapult->ReInit();
	AutonomousTimer->Stop();
}

void robot::AutonomousInit() 
{
	SmartDashboard::PutNumber("Potential Targets:", 0);
	SmartDashboard::PutNumber("Target Range:", 0);
	auto_fired = false;
	AutonomousTimer->Start();
	AutonomousTimer->Reset();
	unfoldingState = collectorLower;
	movementState = waiting;
	correct_range_loops = 0;
	theCatapult->ResetEncoder();
	theCatapult->SetMotorPower(1.0);
	theCollector->ReInit();
	theCollector->Run();
//	total_error = 0;
}

void robot::TeleopInit() 
{
	AutonomousTimer->Stop();
}

void robot::TestInit()
{
	
}
	
void robot::DisabledPeriodic() 
{
//	float leftVoltage = theUltrasonics->GetLeftUltrasonicVoltage();
//	float rightVoltage = theUltrasonics->GetRightUltrasonicVoltage();
//	insight_ballDistance.setData(0);
//	theUltrasonics->Idle();
//	SmartDashboard::PutNumber("leftVoltage", leftVoltage);
//	SmartDashboard::PutNumber("rightVoltage", rightVoltage);
	int left = 0, right = 0, ball = 0;
	static int loops = 0;
	loops++;
	
	if(loops % 250)
	{
		I2C->GetValues(left, right,ball);
	}
	I2C->Idle();
	theCatapult->SetStoppingPoint(151);
}

void robot::AutonomousPeriodic() {
I2C->Idle();
	
//  int   num_targets = static_cast<int>(SmartDashboard::GetNumber("Potential Targets:"));
//  float target_range = SmartDashboard::GetNumber("Target Range:");
//  float optimal_range = 120;
//  float range_tolerance = 30; //inches
 // total_error += (target_range - optimal_range);
  
  switch(unfoldingState){
  	  case collectorLower:
  		  if(AutonomousTimer->Get() < 1.5){
  			  
  		  }
  		  else{
  			  theCatapult->ResetLoweringTimer();
  			  unfoldingState = shooterLower;
  		  }
  	  break;
  	  
  	  case shooterLower:
  		  theCatapult->AutonomousLower();
  		  unfoldingState = done;
  	  break;
  	  
  	  case done:
  		  if(AutonomousTimer->Get() < 3){
  			  
  		  }
  		  else{
  			  theCollector->Disable();
  		  }
  	  break;
  }

if(I2C->GetRight() > 85){
	error = I2C->GetRight() - 85;
	cummulative_error += error;
	float x = -.11;
	float y = ((error)*(0.006))+(cummulative_error*0.000025);
	float twist = 0;
	theChassis->SetJoystickData(x, y, twist);
}
else if(!auto_fired){
     float x = 0;
     float y = 0;
     float twist = 0;
     theChassis->SetJoystickData(x, y, twist);
     theCatapult->SetMotorPower(1.0);
     theCatapult->SetStoppingPoint(165);
     theCatapult->Fire();
     auto_fired = true;
}
else{
 	float x = 0;
  	float y = 0;
  	float twist = 0;
  	theChassis->SetJoystickData(x, y, twist);
}
// Timing Only Autonomous
//  switch(movementState){
//	  case waiting:
//		  if(AutonomousTimer->Get() > 1){
//			  movementState = moving;
//		  }
//	  break;
//	  
//	  case moving:
//	  	  if(AutonomousTimer->Get() > 4.3){
//	  		  movementState = stopping;
//	  	  }
//	  	  else{
//	  		  theChassis->SetJoystickData(0, 0.6, 0);
//	  		  
//	  	  }
//	  break;
//	  
//	  case stopping:
//		  if(AutonomousTimer->Get() > 6.3){
//			  movementState = shooting;
//		  }
//		  else{
//			  theChassis->SetJoystickData(0, 0, 0);
//		  }
//	  break;
//	  
//	  case shooting:
//		  theCatapult->Fire();
//		  movementState = autonomousDone;
//	  break;
//	  
//	  case autonomousDone:
//		  
//	  break;
//  }
  
// Vision Only Autonomous
//  if (target_range - optimal_range > range_tolerance && !(auto_fired) && AutonomousTimer->Get() < 9) {
//    float x = 0;
//    float y = 0.45;
////    float y = 0.004347826*(target_range - optimal_range)+total_error*0.00000001;
//    float twist = 0;
//    theChassis->SetJoystickData(x, y, twist);
//  }
//  else if ((AutonomousTimer->Get() > 9 || 1 < num_targets) && !(auto_fir`ed)) {
//    //stop
//    float x = 0;
//    float y = 0;
//    float twist = 0;
//    theChassis->SetJoystickData(x, y, twist);
//    //shoot!
//		theCatapult->SetMotorPower(1.0);
//    	theCatapult->SetStoppingPoint(165);
//		theCatapult->Fire();
//	auto_fired = true;
//  }
//  else{
//  	float x = 0;
//  	float y = 0;
//  	float twist = 0;
//  	theChassis->SetJoystickData(x, y, twist);
//  }
  	theChassis->Idle();
  	theCatapult->Idle(); 
	theCollector->Idle();
}
	
void robot::TeleopPeriodic() {
	I2C->Idle();
	
	float x = joystick->GetRawAxis(3);
	float y = -(joystick->GetRawAxis(2));
	float twist = joystick->GetRawAxis(1);
	theChassis->SetJoystickData(x, y, twist);
	if(joystick->GetRawButton(1)){
		theCatapult->Fire();
	}
	if(joystick->GetRawButton(2)){
		theCollector->Run();
	}
	if(joystick->GetRawButton(3)){
		theCollector->Disable();
	}
	if(joystick->GetRawButton(4)){
		theCollector->SetManualRollerMode();
	}
	if(joystick->GetRawButton(5)){
		theCatapult->SetStoppingPoint(151);
	}
	if(joystick->GetRawButton(6)){
		theCatapult->ResetEncoder();
	}
	if(joystick->GetRawButton(7)){
		theCatapult->SetStoppingPoint(100);
	}
	if(joystick->GetRawButton(8)){
		theCatapult->SetStoppingPoint(80);
	}
	if(joystick->GetRawButton(9)){
		theCollector->ManualRaise();
	}
	if(joystick->GetRawButton(10)){
		theCollector->ReInit();
	}
	if(joystick->GetRawButton(11)){
		theCollector->EnableProtectedMode();
	}
	if(joystick->GetRawButton(12)){
		theCollector->DisableProtectedMode();
	}
	theCollector->ManualRoller(joystick->GetRawAxis(6));
	theCatapult->Idle();
	theChassis->Idle();
	theCollector->Idle();
}

void robot::TestPeriodic(){
	theCollector->AssistedManualRaise();
}

START_ROBOT_CLASS(robot);
