#include "Wire.h"

#include "MPU6050.h"


MPU6050lib mpu;


float aRes, gRes;        //传感器的每个LSB的刻度分辨率 scale resolutions per LSB for the sensors

int16_t accelCount[3];      //存储16位符号加速度计传感器输出 Stores the 16-bit signed accelerometer sensor output

int16_t gyroCount[3];      //存储16位符号陀螺传感器输出 Stores the 16-bit signed gyro sensor output

float SelfTest[6];

float gyroBias[3] = {0, 0, 0};

float accelBias[3] = {0, 0, 0}; //陀螺仪和加速度计的偏差校正 Bias corrections for gyro and accelerometer

float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};// vector to hold quaternion

float pitch, yaw, roll;

//6自由度传感器融合计算参数 parameters for 6 DoF sensor fusion calculations

float GyroMeasError = PI * (40.0f / 180.0f);    // 陀螺仪测量误差以拉德/秒为单位(从60度/秒开始)，然后在~10秒后减小到3gyroscope measurement error in rads/s (start at 60 deg/s), then reduce after ~10 s to 3

float beta = sqrt(3.0f / 4.0f) * GyroMeasError;  // 计算β compute beta

float GyroMeasDrift = PI * (2.0f / 180.0f);    // 陀螺仪测量漂移(rad/s/s)(从0.0度/s/s开始)gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)

float zeta = sqrt(3.0f / 4.0f) * GyroMeasDrift;  // 计算zeta, Madgwick格式中的另一个自由参数通常设置为一个小值或零值compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value

float deltat = 0.0f;                // 两种滤波方案的积分区间integration interval for both filter schemes

uint32_t lastUpdate = 0, firstUpdate = 0;     // 用于计算积分区间used to calculate integration interval

uint32_t Now = 0;                 // 用于计算积分区间used to calculate integration interval

double ax,ay,az; //滤波器的输出

double gyrox,gyroy,gyroz; //滤波器的输出


void setup()

{

 Wire.begin();

 Serial.begin(115200);


 uint8_t c = mpu.readByte(MPU6050_ADDRESS, WHO_AM_I_MPU6050);  // Read WHO_AM_I register for MPU-6050

 Serial.print("I AM ");

 Serial.println(c, HEX);

 mpu.settings(AFS_8G, GFS_250DPS);

 if (c == 0x68) //WHO_AM_I应该总是0x68 WHO_AM_I should always be 0x68

 {

  Serial.println("MPU6050 is online...");

  //从执行自检和报告值开始 Start by performing self test and reporting values

  mpu.MPU6050SelfTest(SelfTest); 

  Serial.print("x-axis self test: acceleration trim within : "); Serial.print(SelfTest[0],1); Serial.println("% of factory value");

  Serial.print("y-axis self test: acceleration trim within : "); Serial.print(SelfTest[1],1); Serial.println("% of factory value");

  Serial.print("z-axis self test: acceleration trim within : "); Serial.print(SelfTest[2],1); Serial.println("% of factory value");

  Serial.print("x-axis self test: gyration trim within : "); Serial.print(SelfTest[3],1); Serial.println("% of factory value");

  Serial.print("y-axis self test: gyration trim within : "); Serial.print(SelfTest[4],1); Serial.println("% of factory value");

  Serial.print("z-axis self test: gyration trim within : "); Serial.print(SelfTest[5],1); Serial.println("% of factory value");

 

  if (SelfTest[0] < 1.0f && SelfTest[1] < 1.0f && SelfTest[2] < 1.0f && SelfTest[3] < 1.0f && SelfTest[4] < 1.0f && SelfTest[5] < 1.0f) {

   Serial.println("Pass Selftest!");

   //校准陀螺和加速度计，在偏置寄存器中加载偏置 Calibrate gyro and accelerometers, load biases in bias registers

   mpu.calibrateMPU6050(gyroBias, accelBias); 

   Serial.println("MPU6050 bias");

   Serial.println(" x\t  y\t  z  ");

   Serial.print((int)(1000 * accelBias[0])); Serial.print('\t');

   Serial.print((int)(1000 * accelBias[1])); Serial.print('\t');

   Serial.print((int)(1000 * accelBias[2]));

   Serial.println(" mg");

 

   Serial.print(gyroBias[0], 1); Serial.print('\t');

   Serial.print(gyroBias[1], 1); Serial.print('\t');

   Serial.print(gyroBias[2], 1);

   Serial.println(" o/s");

 

   mpu.settings(AFS_8G, GFS_2000DPS);

   mpu.initMPU6050(); 

   //初始化设备的主动模式读数 加速度计，陀螺仪，和温度 Initialize device for active mode read of acclerometer, gyroscope, and temperature

   Serial.println("MPU6050 initialized for active data mode...."); 

  }

 }

 else

 {

  Serial.print("Could not connect to MPU6050: 0x");

  Serial.println(c, HEX);

  while(1); //如果通信没有发生，则永远循环 Loop forever if communication doesn't happen

 }

 for(int i = 0; i < 300;i++){

  if (mpu.readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01) { //检查数据是否准备 check if data ready interrupt

   mpu.readAccelData(accelCount);   //读取x/y/z adc值 Read the x/y/z adc values

   mpu.readGyroData(gyroCount);    //读取x/y/z adc值 Read the x/y/z adc values

  }

 }

}

 

void loop()

{

 //如果设置了数据就绪位，则所有数据寄存器都有新数据 If data ready bit set, all data registers have new data

 if (mpu.readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01) {
    //检查数据是否准备 check if data ready interrupt

  mpu.readAccelData(accelCount);   //读取x/y/z adc值 Read the x/y/z adc values

  // Kalman_Filter(accelCount[0],accelCount[1],accelCount[2]);

  //现在我们将加速度值计算为实际的g Now we'll calculate the accleration value into actual g's

  aRes = mpu.getAres();//获取转换值

  ax = (float)accelCount[0] * aRes; //得到实际的g值，这取决于所设置的比例 get actual g value, this depends on scale being set

  ay = (float)accelCount[1] * aRes;

  az = (float)accelCount[2] * aRes;

 

  mpu.readGyroData(gyroCount);    //读取x/y/z adc值 Read the x/y/z adc values

  // Kalman_Filter(gyroCount[0],gyroCount[1],gyroCount[2]);

  gRes = mpu.getGres();       //获取转换值

  //计算陀螺值为实际度每秒 Calculate the gyro value into actual degrees per second

  gyrox = (float)gyroCount[0] * gRes;//获得实际的陀螺值，这取决于刻度被设置 get actual gyro value, this depends on scale being set

  gyroy = (float)gyroCount[1] * gRes;

  gyroz = (float)gyroCount[2] * gRes;


 }

 //获取系统当前微秒时间

 Now = micros();

 //按自上次过滤器更新以来经过的时间设置集成时间 set integration time by time elapsed since last filter update

 deltat = ((Now - lastUpdate) / 1000000.0f);

 lastUpdate = Now;

 if(lastUpdate - firstUpdate > 10000000uL) {

  beta = 0.041; //稳定后降低滤波器增益 decrease filter gain after stabilized

  zeta = 0.015; //稳定后增加陀螺偏压漂移增益 increase gyro bias drift gain after stabilized

 }

 // 陀螺仪数据转换为弧度

 gyrox = gyrox  * PI / 180.0f;

 gyroy = gyroy * PI / 180.0f;

 gyroz = gyroz * PI / 180.0f;

 // 四元素转换函数

 MadgwickQuaternionUpdate(ax, ay, az, gyrox, gyroy, gyroz);


 // yaw  = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]), q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);

 pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));

 roll  = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);


 pitch *= 180.0f / PI;

 yaw  *= 180.0f / PI;

 roll  *= 180.0f / PI;



 Serial.print("roll:");Serial.print(roll);

 Serial.print("            ");

 Serial.print("pitch:");Serial.println(pitch);


 delay(100);

}


// Implementation of Sebastian Madgwick's "...efficient orientation filter for... inertial/magnetic sensor arrays"

// which fuses acceleration and rotation rate to produce a quaternion-based estimate of relative

// device orientation -- which can be converted to yaw, pitch, and roll. Useful for stabilizing quadcopters, etc.

// The performance of the orientation filter is at least as good as conventional Kalman-based filtering algorithms

// but is much less computationally intensive---it can be performed on a 3.3 V Pro Mini operating at 8 MHz!

void MadgwickQuaternionUpdate(float ax, float ay, float az, float gyrox, float gyroy, float gyroz)

{

 float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];     //为便于阅读，简称局部变量 short name local variable for readability

 float norm;                        //向量范数 vector norm

 float f1, f2, f3;                     //目标功能元素 objetive funcyion elements

 float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33; //目标函数雅可比矩阵 objective function Jacobian elements

 float qDot1, qDot2, qDot3, qDot4;

 float hatDot1, hatDot2, hatDot3, hatDot4;

 float gerrx, gerry, gerrz, gbiasx, gbiasy, gbiasz;     //陀螺偏压误差 gyro bias error


 //辅助变量避免重复运算 Auxiliary variables to avoid repeated arithmetic

 float _halfq1 = 0.5f * q1;

 float _halfq2 = 0.5f * q2;

 float _halfq3 = 0.5f * q3;

 float _halfq4 = 0.5f * q4;

 float _2q1 = 2.0f * q1;

 float _2q2 = 2.0f * q2;

 float _2q3 = 2.0f * q3;

 float _2q4 = 2.0f * q4;

 float _2q1q3 = 2.0f * q1 * q3;

 float _2q3q4 = 2.0f * q3 * q4;


 //规范化加速度计测量 Normalise accelerometer measurement

 norm = sqrt(ax * ax + ay * ay + az * az);

 if (norm == 0.0f) return; // handle NaN

 norm = 1.0f/norm;

 ax *= norm;

 ay *= norm;

 az *= norm;


 //计算目标函数和雅可比矩阵 Compute the objective function and Jacobian

 f1 = _2q2 * q4 - _2q1 * q3 - ax;

 f2 = _2q1 * q2 + _2q3 * q4 - ay;

 f3 = 1.0f - _2q2 * q2 - _2q3 * q3 - az;

 J_11or24 = _2q3;

 J_12or23 = _2q4;

 J_13or22 = _2q1;

 J_14or21 = _2q2;

 J_32 = 2.0f * J_14or21;

 J_33 = 2.0f * J_11or24;

 
 //计算梯度(矩阵乘法) Compute the gradient (matrix multiplication)

 hatDot1 = J_14or21 * f2 - J_11or24 * f1;

 hatDot2 = J_12or23 * f1 + J_13or22 * f2 - J_32 * f3;

 hatDot3 = J_12or23 * f2 - J_33 *f3 - J_13or22 * f1;

 hatDot4 = J_14or21 * f1 + J_11or24 * f2;


 //将梯度归一化 Normalize the gradient

 norm = sqrt(hatDot1 * hatDot1 + hatDot2 * hatDot2 + hatDot3 * hatDot3 + hatDot4 * hatDot4);

 hatDot1 /= norm;

 hatDot2 /= norm;

 hatDot3 /= norm;

 hatDot4 /= norm;
 

 //计算估计陀螺仪偏差 Compute estimated gyroscope biases

 gerrx = _2q1 * hatDot2 - _2q2 * hatDot1 - _2q3 * hatDot4 + _2q4 * hatDot3;

 gerry = _2q1 * hatDot3 + _2q2 * hatDot4 - _2q3 * hatDot1 - _2q4 * hatDot2;

 gerrz = _2q1 * hatDot4 - _2q2 * hatDot3 + _2q3 * hatDot2 - _2q4 * hatDot1;


 //计算和消除陀螺仪偏差 Compute and remove gyroscope biases

 gbiasx += gerrx * deltat * zeta;

 gbiasy += gerry * deltat * zeta;

 gbiasz += gerrz * deltat * zeta;

 gyrox -= gbiasx;

 gyroy -= gbiasy;

 gyroz -= gbiasz;


 //计算四元数的导数 Compute the quaternion derivative

 qDot1 = -_halfq2 * gyrox - _halfq3 * gyroy - _halfq4 * gyroz;

 qDot2 =  _halfq1 * gyrox + _halfq3 * gyroz - _halfq4 * gyroy;

 qDot3 =  _halfq1 * gyroy - _halfq2 * gyroz + _halfq4 * gyrox;

 qDot4 =  _halfq1 * gyroz + _halfq2 * gyroy - _halfq3 * gyrox;


 //计算，然后积分估计四元数的导数 Compute then integrate estimated quaternion derivative

 q1 += (qDot1 -(beta * hatDot1)) * deltat;

 q2 += (qDot2 -(beta * hatDot2)) * deltat;

 q3 += (qDot3 -(beta * hatDot3)) * deltat;

 q4 += (qDot4 -(beta * hatDot4)) * deltat;


 //四元数归一化 Normalize the quaternion

 norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);   // 标准化四元数 normalise quaternion

 norm = 1.0f/norm;

 q[0] = q1 * norm;

 q[1] = q2 * norm;

 q[2] = q3 * norm;

 q[3] = q4 * norm;

}