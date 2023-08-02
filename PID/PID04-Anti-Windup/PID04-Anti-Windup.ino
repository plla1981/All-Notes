/*
  PID算法：抗积分饱和
    所谓的积分饱和现象是指如果系统存在一个方向的偏差，
  PID控制器的输出由于积分作用的不断累加而加大，
  从而导致执行机构达到极限位置，若控制器输出U(k)继续增大，
  执行器开度不可能再增大，此时计算机输出控制量超出了正常运行范围而进入饱和区。
  一旦系统出现反向偏差，u(k)逐渐从饱和区退出。进入饱和区越深则退出饱和区时间越长。
  在这段时间里，执行机构仍然停留在极限位置而不随偏差反向而立即做出相应的改变，
  这时系统就像失控一样，造成控制性能恶化，这种现象称为积分饱和现象或积分失控现象。

    防止积分饱和的方法之一就是抗积分饱和法，该方法的思路是在计算u(k)时，
  首先判断上一时刻的控制量u(k-1)是否已经超出了极限范围： 
  如果u(k-1)>umax，则只累加负偏差; 如果u(k-1)<umin，则只累加正偏差。
  从而避免控制量长时间停留在饱和区。
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
  pid.uMax = 600;
  pid.uMin = -300;
}

/* step 3 控制算法 */
float pidRealize(float speed){
  int index;
  pid.setSpeed = speed;
  pid.err = pid.setSpeed - pid.actualSpeed;
  /* start */
  if (pid.actualSpeed > pid.uMax){
    if(abs(pid.err) > 300){
      index = 0;
    }else {
      pid.integral += pid.err;
    }
  }else if (pid.actualSpeed < pid.uMin){
    if (abs(pid.err) > 300){//积分分离过程
      index = 0;
    }else {
      index = 1;
      if(pid.err > 0){
        pid.integral += pid.err;
      }
    }
  }else {
    if(abs(pid.err > 300)){//积分分离过程
      index = 0;
    }else {
      index = 1;
      pid.integral += pid.err;
    }
  }
  pid.voltegral = (pid.kP * pid.err) + (index * pid.kI * pid.integral) + (pid.kD * (pid.err - pid.errLast));
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
  float setValue = 300.0;
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
