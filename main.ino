// seven segment
#define COMMON_ANODE_DIGIT_1 2
#define COMMON_ANODE_DIGIT_2 4
#define CATHODE_D 5
#define COMMON_ANODE_L1_L2_L3 13
#define CATHODE_E 12
#define COMMON_ANODE_DIGIT_3 14
#define CATHODE_DP 18
#define COMMON_ANODE_DIGIT_4 19
#define NOPIN 21
#define CATHODE_F 22
#define CATHODE_C_L3 23
#define CATHODE_A_L1 27
#define CATHODE_G 26
#define CATHODE_B_L2 25
#define LED_PIN 33
// ultrasonic motion sensor
#define echoPin 21
#define trigPin 15

TaskHandle_t Task1;
TaskHandle_t Task2; // multicore!

const int DELAY = 1; // in ms
bool separator_visible = true;
bool display_time_remaining = true; // true: display time remaining; false: display time class ends
int time_absolute_time_remaining = 5000; // remaining time to show absolute time
const int TIME_TO_SHOW_CHANGED_STATE = 5000;
int time_class_ends = 1058; // 12-hour time. 0100 to 1259

int distance = 100;

void setup() {
  // seven segment pins
  pinMode(COMMON_ANODE_DIGIT_1, OUTPUT);
  pinMode(COMMON_ANODE_DIGIT_2, OUTPUT);
  pinMode(CATHODE_D, OUTPUT);
  pinMode(COMMON_ANODE_L1_L2_L3, OUTPUT);
  pinMode(CATHODE_E, OUTPUT);
  pinMode(COMMON_ANODE_DIGIT_3, OUTPUT);
  pinMode(CATHODE_DP, OUTPUT);
  pinMode(COMMON_ANODE_DIGIT_4, OUTPUT);
  pinMode(CATHODE_F, OUTPUT);
  pinMode(CATHODE_C_L3, OUTPUT);
  pinMode(CATHODE_A_L1, OUTPUT);
  pinMode(CATHODE_G, OUTPUT);
  pinMode(CATHODE_B_L2, OUTPUT);
  // ultrasonic motion sensor pins
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed

  xTaskCreatePinnedToCore(
                    loopy_loop,   /* Task function. */
                    "loopy_loop",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(100); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    ultrasonic_update,   /* Task function. */
                    "ultrasonic_update",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(100); 
  
}

void display_time_left(int time_left) { // converts ms
  off();
  int first_digit = (time_left/1000) / 60 % 100 / 10;
  number_displayed(first_digit);
  digit(1);
  delay(DELAY);
    
  off();
  int second_digit = (time_left/1000) / 60 % 10;
  number_displayed(second_digit);
  digit(2);
  delay(DELAY);

  off();
  separator();
  delay(DELAY);
     
  off();
  int third_digit = (time_left/1000) % 60 / 10;
  number_displayed(third_digit);
  digit(3);
  delay(DELAY);
    
  off();
  int fourth_digit = (time_left/1000) % 60 % 10;
  number_displayed(fourth_digit);
  digit(4);
  delay(DELAY);
}

void display_class_time() { // converts 0100 to 1259 format
  off();
  int first_digit = (time_class_ends/1000);
  number_displayed(first_digit);
  digit(1);
  delay(DELAY);
    
  off();
  int second_digit = (time_class_ends%1000/100);
  number_displayed(second_digit);
  digit(2);
  delay(DELAY);

  off();
  separator();
  delay(DELAY);
     
  off();
  int third_digit = (time_class_ends%100/10);
  number_displayed(third_digit);
  digit(3);
  delay(DELAY);
    
  off();
  int fourth_digit = (time_class_ends%10);
  number_displayed(fourth_digit);
  digit(4);
  delay(DELAY);
  
}

void motion_sensor_check() {

    
  if (distance < 10) {
    display_time_absolute();
  }
  
  

  check_time_state();
  
  //Serial.print("Distance: ");
  //Serial.print(distance);
  //Serial.println(" cm");
}

void display_time_absolute() { // displays time this çlass ends
  display_time_remaining = false;
  time_absolute_time_remaining = TIME_TO_SHOW_CHANGED_STATE;
}

void check_time_state() {
  if (display_time_remaining == false) {
    time_absolute_time_remaining -= (5 * DELAY);
    if (time_absolute_time_remaining <= 0) {
      display_time_remaining = true;
    }
  }else{
    //time_absolute_time_remaining = TIME_TO_SHOW_CHANGED_STATE;
  }
  //Serial.println(time_absolute_time_remaining);
}

void off() {
  digitalWrite(COMMON_ANODE_DIGIT_1, LOW);
  digitalWrite(COMMON_ANODE_DIGIT_2, LOW);
  digitalWrite(CATHODE_D, HIGH);
  digitalWrite(COMMON_ANODE_L1_L2_L3, LOW);
  digitalWrite(CATHODE_E, HIGH);
  digitalWrite(COMMON_ANODE_DIGIT_3, LOW);
  digitalWrite(CATHODE_DP, HIGH);
  digitalWrite(COMMON_ANODE_DIGIT_4, LOW);
  digitalWrite(CATHODE_F, HIGH);
  digitalWrite(CATHODE_C_L3, HIGH);
  digitalWrite(CATHODE_A_L1, HIGH);
  digitalWrite(CATHODE_G, HIGH);
  digitalWrite(CATHODE_B_L2, HIGH);
}

void zero() {
  digitalWrite(CATHODE_D, LOW);
  digitalWrite(CATHODE_E, LOW);
  digitalWrite(CATHODE_DP, HIGH);
  digitalWrite(CATHODE_F, LOW);
  digitalWrite(CATHODE_C_L3, LOW);
  digitalWrite(CATHODE_A_L1, LOW);
  digitalWrite(CATHODE_G, HIGH);
  digitalWrite(CATHODE_B_L2, LOW);
}

void one() {
  digitalWrite(CATHODE_D, HIGH);
  digitalWrite(CATHODE_E, HIGH);
  digitalWrite(CATHODE_DP, HIGH);
  digitalWrite(CATHODE_F, HIGH);
  digitalWrite(CATHODE_C_L3, LOW);
  digitalWrite(CATHODE_A_L1, HIGH);
  digitalWrite(CATHODE_G, HIGH);
  digitalWrite(CATHODE_B_L2, LOW);
}

void two() {
  digitalWrite(CATHODE_D, LOW);
  digitalWrite(CATHODE_E, LOW);
  digitalWrite(CATHODE_DP, HIGH);
  digitalWrite(CATHODE_F, HIGH);
  digitalWrite(CATHODE_C_L3, HIGH);
  digitalWrite(CATHODE_A_L1, LOW);
  digitalWrite(CATHODE_G, LOW);
  digitalWrite(CATHODE_B_L2, LOW);
}

void three() {
  digitalWrite(CATHODE_D, LOW);
  digitalWrite(CATHODE_E, HIGH);
  digitalWrite(CATHODE_DP, HIGH);
  digitalWrite(CATHODE_F, HIGH);
  digitalWrite(CATHODE_C_L3, LOW);
  digitalWrite(CATHODE_A_L1, LOW);
  digitalWrite(CATHODE_G, LOW);
  digitalWrite(CATHODE_B_L2, LOW);
}

void four() {
  digitalWrite(CATHODE_D, HIGH);
  digitalWrite(CATHODE_E, HIGH);
  digitalWrite(CATHODE_DP, HIGH);
  digitalWrite(CATHODE_F, LOW);
  digitalWrite(CATHODE_C_L3, LOW);
  digitalWrite(CATHODE_A_L1, HIGH);
  digitalWrite(CATHODE_G, LOW);
  digitalWrite(CATHODE_B_L2, LOW);
}

void five() {
  digitalWrite(CATHODE_D, LOW);
  digitalWrite(CATHODE_E, HIGH);
  digitalWrite(CATHODE_DP, HIGH);
  digitalWrite(CATHODE_F, LOW);
  digitalWrite(CATHODE_C_L3, LOW);
  digitalWrite(CATHODE_A_L1, LOW);
  digitalWrite(CATHODE_G, LOW);
  digitalWrite(CATHODE_B_L2, HIGH);
}

void six() {
  digitalWrite(CATHODE_D, LOW);
  digitalWrite(CATHODE_E, LOW);
  digitalWrite(CATHODE_DP, HIGH);
  digitalWrite(CATHODE_F, LOW);
  digitalWrite(CATHODE_C_L3, LOW);
  digitalWrite(CATHODE_A_L1, LOW);
  digitalWrite(CATHODE_G, LOW);
  digitalWrite(CATHODE_B_L2, HIGH);
}

void seven() {
  digitalWrite(CATHODE_D, HIGH);
  digitalWrite(CATHODE_E, HIGH);
  digitalWrite(CATHODE_DP, HIGH);
  digitalWrite(CATHODE_F, HIGH);
  digitalWrite(CATHODE_C_L3, LOW);
  digitalWrite(CATHODE_A_L1, LOW);
  digitalWrite(CATHODE_G, HIGH);
  digitalWrite(CATHODE_B_L2, LOW);
}

void eight() {
  digitalWrite(CATHODE_D, LOW);
  digitalWrite(CATHODE_E, LOW);
  digitalWrite(CATHODE_DP, HIGH);
  digitalWrite(CATHODE_F, LOW);
  digitalWrite(CATHODE_C_L3, LOW);
  digitalWrite(CATHODE_A_L1, LOW);
  digitalWrite(CATHODE_G, LOW);
  digitalWrite(CATHODE_B_L2, LOW);
}

void nine() {
  digitalWrite(CATHODE_D, HIGH);
  digitalWrite(CATHODE_E, HIGH);
  digitalWrite(CATHODE_DP, HIGH);
  digitalWrite(CATHODE_F, LOW);
  digitalWrite(CATHODE_C_L3, LOW);
  digitalWrite(CATHODE_A_L1, LOW);
  digitalWrite(CATHODE_G, LOW);
  digitalWrite(CATHODE_B_L2, LOW);
}

void separator() {
  if ((separator_visible == true) || !display_time_remaining) {
    digitalWrite(CATHODE_D, HIGH);
    digitalWrite(COMMON_ANODE_L1_L2_L3, HIGH);
    digitalWrite(CATHODE_E, HIGH);
    digitalWrite(CATHODE_DP, HIGH);
    digitalWrite(CATHODE_F, HIGH);
    digitalWrite(CATHODE_C_L3, HIGH);
    digitalWrite(CATHODE_A_L1, LOW);
    digitalWrite(CATHODE_G, HIGH);
    digitalWrite(CATHODE_B_L2, LOW);
  }else{
    digitalWrite(CATHODE_D, HIGH);
    digitalWrite(COMMON_ANODE_L1_L2_L3, HIGH);
    digitalWrite(CATHODE_E, HIGH);
    digitalWrite(CATHODE_DP, HIGH);
    digitalWrite(CATHODE_F, HIGH);
    digitalWrite(CATHODE_C_L3, HIGH);
    digitalWrite(CATHODE_A_L1, HIGH);
    digitalWrite(CATHODE_G, HIGH);
    digitalWrite(CATHODE_B_L2, HIGH);
  }
}

void digit(int i) {
  switch(i) {
    case 1:
      digitalWrite(COMMON_ANODE_DIGIT_1, HIGH);
      break;
    case 2:
      digitalWrite(COMMON_ANODE_DIGIT_2, HIGH);
      break;
    case 3:
      digitalWrite(COMMON_ANODE_DIGIT_3, HIGH);
      break;
    case 4:
      digitalWrite(COMMON_ANODE_DIGIT_4, HIGH);
      break;
    default:
      digitalWrite(COMMON_ANODE_DIGIT_1, HIGH);
      digitalWrite(COMMON_ANODE_DIGIT_2, HIGH);
      digitalWrite(COMMON_ANODE_DIGIT_3, HIGH);
      digitalWrite(COMMON_ANODE_DIGIT_4, HIGH);
  }
}

void number_displayed(int i) {
  switch(i) {
    case 0:
      zero();
      break;
    case 1:
      one();
      break;
    case 2:
      two();
      break;
    case 3:
      three();
      break;
    case 4:
      four();
      break;
    case 5:
      five();
      break;
    case 6:
      six();
      break;
    case 7:
      seven();
      break;
    case 8:
      eight();
      break;
    case 9:
      nine();
      break;
    default:
      off();
      break;
  }
}

void turnLED(bool isOn) {
  if (isOn) {
    digitalWrite(LED_PIN, HIGH);
  }else{
    digitalWrite(LED_PIN, LOW);
  }
}

int time_left = 600000; // in ms

void loop() {
}

void loopy_loop( void * pvParameters ){
  while (true) {
    motion_sensor_check();
    
    separator_visible = (time_left % 2000 < 1000);
    if (time_left >= 3600000) {
      if (display_time_remaining) { display_time_left(time_left/60); }else{ display_class_time(); }
      //display_time_left(time_left/60);
      time_left -= DELAY*5;
    }
    else if (time_left >= 0) {
      if (display_time_remaining) { display_time_left(time_left); }else{ display_class_time(); }
      //display_time_left(time_left);
      time_left -= DELAY*5;
    }else{
      off();
    }
  }
}

void ultrasonic_update( void * pvParameters ){
  while (true) {
      // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    int duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.0343 / 2; // Speed of sound wave divided by 2 (go and back) 
    Serial.println(distance);
    delay(100);
  }
}
