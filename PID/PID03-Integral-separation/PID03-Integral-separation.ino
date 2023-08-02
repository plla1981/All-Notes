/*
  PID:积分分离
*/

/* step 1 定义PID变量结构体 */
struct _pid{
  float setSpeed;     //定义设定值
  float actualSpeed;  //定义实际值
  float err;          //定义偏差值
  float errLast;      //定义上一个偏差值
  float kP,kI,kD;     //比例P、积分I、微分系数D
  float voltegral;    //电压值（控制执行器电机的变量）
  float integral;     //积分值
  float index;
}pid;

/* step 2 初始化变量 */
/* 统一初始化变量，尤其是Kp,Ki,Kd三个参数，调试过程当中，对于要求的控制效果，可以通过调节这三个量直接进行调节。 */
void pidInit(){
  pid.setSpeed = 0.0;
  pid.actualSpeed = 0.0;
  pid.err = 0.0;
  pid.errLast = 0.0;
  pid.voltegral = 0.0;
  pid.integral = 0.0;
  pid.kP = 0.2;
  pid.kI = 0.04;
  pid.kD = 0.2;
}

/* step 3 控制算法 */
float pidRealize(float speed){
  pid.setSpeed = speed;
  pid.err = pid.setSpeed - pid.actualSpeed;
  /* start */
  if (abs(pid.err) > 200){
    pid.index = 0;
  }
  else {
    pid.index = 1;
    pid.integral += pid.err;
  }
  pid.voltegral = (pid.kP * pid.err) + (pid.index * pid.kI * pid.integral) + (pid.kD * (pid.err - pid.errLast));
  /* end */
  pid.errLast = pid.err;
  pid.actualSpeed = pid.voltegral * 1.0;

  return pid.actualSpeed;
}

void setup() {
  Serial.begin(9600);
  pidInit();
}

void loop() {
  int count = 0;
  float setValue = 200.0;
  while (count < 1000){
    
    float speed = pidRealize(setValue);
    Serial.print("c:");
    Serial.print(count);Serial.print("   ");
    Serial.print("v:");
    Serial.print(setValue);Serial.print("   ");
    Serial.print("s:");
    Serial.println(speed, 6);
    count++;
  }
  Serial.println("pid end!");
  delay(5000);
}
