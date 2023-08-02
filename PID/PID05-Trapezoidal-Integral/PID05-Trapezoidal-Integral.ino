/*
  PID算法：梯形积分
  作为PID控制律的积分项，其作用是消除余差，为了尽量减小余差，应提高积分项运算精度，为此可以将矩形积分改为梯形积分
*/


/* step 1 定义PID变量结构体 */
struct _pid{
  float setSpeed;   //设定值
  float actualSpeed;//实际值
  float err;        //当前偏差值
  float errLast;    //上一个偏差值
  float kP,kI,kD;   //比例P，积分I，微分系数D
  float voltegral;  //电压值（控制执行器电机的变量）
  float integral;   //积分值
  float uMax;
  float uMin;
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
  pid.kI = 0.1; //加大了积分环节的值
  pid.kD = 0.2;
  pid.uMax = 400;
  pid.uMin = -200;
}

/* step 3 控制算法 */
float pidRealize(float speed){
  int index;
  pid.setSpeed = speed;
  pid.err = pid.setSpeed - pid.actualSpeed;
  /* start */
  if (pid.actualSpeed > pid.uMax){
    if(abs(pid.err) > 200){
      index = 0;
    }else {
      pid.integral += pid.err;
    }
  }else if (pid.actualSpeed < pid.uMin){
    if (abs(pid.err) > 200){//积分分离过程
      index = 0;
    }else {
      index = 1;
      if(pid.err > 0){
        pid.integral += pid.err;
      }
    }
  }else {
    if(abs(pid.err > 200)){//积分分离过程
      index = 0;
    }else {
      index = 1;
      pid.integral += pid.err;
    }
  }
  //梯形积分
  pid.voltegral = (pid.kP * pid.err) + (index * pid.kI * (pid.integral / 2)) + (pid.kD * (pid.err - pid.errLast));
  /* end */
  pid.errLast = pid.err;
  pid.actualSpeed = pid.voltegral * 1.0;

  return pid.actualSpeed;
}

void setup() {

  Serial.begin(9600);

  pidInit();
  Serial.println("pid init!");
}

void loop() {
  long oldTime = millis();
  int count = 0;
  float setValue = 200.0;
  Serial.println("pid Realize!");
  while (count < 1000){
    
    float speed = pidRealize(setValue);
    Serial.print("c:");
    Serial.print(count);Serial.print("   ");
    Serial.print("V:");
    Serial.print(setValue);Serial.print("   ");
    Serial.print("s:");
    Serial.println(speed, 6);
    count++;
  }
  Serial.println("pid end!");
  delay(5000);
}
