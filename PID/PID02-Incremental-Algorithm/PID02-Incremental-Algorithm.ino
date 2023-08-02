/*
  PID：增量型
*/

/* step 1 定义PID变量结构体 */
struct _pid{
  float setSpeed;     //定义设定值
  float actualSpeed;  //定义实际值
  float err;          //定义偏差值
  float errNext;      //定义上一个偏差值
  float errLast;      //定义上上一个偏差值
  float kP,kI,kD;     //定义比例、积分、微分系数
}pid;

/* step 2 初始化变量函数 */
/* 统一初始化变量，尤其是Kp,Ki,Kd三个参数，调试过程当中，对于要求的控制效果，可以通过调节这三个量直接进行调节。 */
void pidInit(){
  pid.setSpeed = 0.0;
  pid.actualSpeed = 0.0;
  pid.err = 0.0;
  pid.errLast = 0.0;
  pid.errNext = 0.0;
  pid.kP = 0.2;
  pid.kI = 0.015;
  pid.kD = 0.2;
}

/* step 3 控制算法 */
float pidRealize(float speed){
  pid.setSpeed = speed;
  pid.err = pid.setSpeed - pid.actualSpeed;
  float incrementSpeed = (pid.kP * (pid.err - pid.errNext)) + (pid.kI * pid.err) + (pid.kD * (pid.err- 2 * pid.errNext + pid.errLast));
  pid.actualSpeed += incrementSpeed;
  pid.errLast = pid.errNext;
  pid.errNext = pid.err;

  return pid.actualSpeed;
}

void setup() {
  Serial.begin(9600);
  pidInit();
}

void loop() {
  int count = 0;
  int setValue = 200.0;
  while(count < 1000){
    float speed = pidRealize(setValue);
    Serial.print("c:");
    Serial.print(count);Serial.print("   ");
    Serial.print("v:");
    Serial.print(setValue);Serial.print("   ");
    Serial.print("s:");
    Serial.println(speed, 4);
    count++;
  }
  Serial.println("PID end!");
  delay(4000);
}
