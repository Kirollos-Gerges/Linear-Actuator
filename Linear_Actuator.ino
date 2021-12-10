
//Author: Kirollos Gerges

//Declare pin functions on Arduino
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define MS3 6
#define EN  7
#define homeButton 12

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Change those values for program modification//
int offDis = 100; //[mm] //offset distance desired from home
int delayT = 2; //[seconds] //delay time desired bewteen waty points
int dBetPoints = 30; //[mm] //distance desired between way points
int pDiam = 12;  //pitch diameter
int d2Home = 100; //[mm] // distance from current position to home
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

//Declare variables for functions
char user_input;
char wayPointDelayTime;

int x;
int y;
int state;

int homeIsSet = 0;

void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);


  pinMode(12, INPUT_PULLUP ); 

   
  resetBEDPins(); //Set step, direction, microstep and enable pins to default states
  Serial.begin(9600); //Open Serial connection for debugging
  Serial.println("Begin motor control");
  Serial.println();
  //attachInterrupt(digitalPinToInterrupt(setHomeButton), setHome, RISING);


  Serial.println("Homing");

  
  while(towardsHousing(200));
  
  
  
  //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. Go to initial offset");
  Serial.println("2. Execute 10 way points.");
  Serial.println("3. Execute 15 way points");
  Serial.println("4. Execute 20 way points");
  Serial.println("5. Go Home..");


}




void loop() {

  //get user input of what to do// 1 - to do the offset 
  while(Serial.available()){
      user_input = Serial.read(); //Read user input and trigger appropriate function
      if (user_input == '1') //to go to initial offest 
      {
         awayfromHousing(offDis);
         Serial.flush();
      }
      else if(user_input =='2') //Execute 10 way points
      {
        for(int i = 0; i < 10; i ++){
          awayfromHousing(dBetPoints);
          delay(delayT * 1000);
        }
      }
      else if(user_input =='3') //Execute 15 way points
      {
        for(int i = 0; i < 15; i ++){
          awayfromHousing(dBetPoints);
          delay(delayT * 1000);
        }
      }
      else if(user_input == '4') // execute 20 points
      {
        for(int i = 0; i < 20; i ++){
          awayfromHousing(dBetPoints);
          delay(delayT * 1000);
        }
      }
      else if(user_input == '5')
      {
        goHome(d2Home);
      }
      else{
        Serial.println("Invalid option entered.");
      }
      resetBEDPins();
  }


}

void goHome(int d2Home){

  towardsHousing(d2Home);
  
}

//Default microstep mode function
int towardsHousing(float distance)
{
  
  int x = distance/pDiam;
  int nSteps = 200*x; //200 steps for 1 complete revolution
  digitalWrite(EN, LOW); //Pull enable pin low to set FETs active and allow motor control


  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  for(x= 0; x < nSteps; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    if(digitalRead(12)){ //if high - high by default
      digitalWrite(stp,HIGH); //Trigger one step forward
      delay(1);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(1);
    }else{
      return 0;
    }
    

  }
  return 1;
    

}
//Reverse default microstep mode function

int awayfromHousing(float distance)
{
  int x = distance/pDiam;
  int nSteps = 200*x; //200 steps for 1 complete revolution
  
  digitalWrite(EN, LOW); //Pull enable pin low to set FETs active and allow motor control
  digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"

  for(x= 0; x < nSteps; x++)  //Loop the stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);

  }
}

//Reset Easy Driver pins to default states
void resetBEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}
