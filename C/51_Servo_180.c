#include <reg52.h>
#define PWM_T 200                     //产生中断的时间，因为是24MHz，200即100微妙(0.1毫秒)
int PWM_count0 = 0;                    //进入中断的次数
int PWM_count1 = 0;                    //与上面的类似，用于计数
int PWM_F = 0;                        //一个用于计时的标志，作用类似定时器的TF0、TF1sbit 
sbit servo = P1^4;                    //舵机信号线所连接的IO口
void PWM_Start()                      //PWM初始化函数，打开了定时器0
{  
    EA = 1;  
    ET0 = 1;  
    TMOD |= 0x01;  
    TR0 = 1;  
    TH0 = (65536-PWM_T)/256;  
    TL0 = (65536-PWM_T)%256;
}
int PWM(int PWM_value)                //控制PWM输出的子函数
{  
    if(PWM_count0 <= PWM_value)  
    {    
        return 1;  
    }  
    else  
    {    
        return 0;  
    }
}
void main()
{  
    PWM_Start();                        //PWM开始运行  
    while(1)  
    {    
        while(1)    
        {      
            servo = PWM(5);                  //旋转至0°      
            if(PWM_F == 1)                  //判断标志是否为1，是则代表着过了1秒      
            {       
                PWM_F = 0;                    //将标志归零        
                break;                        //过了1秒，退出循环      
            }    
        }    
        while(1)    
        {      
            servo = PWM(10);                //旋转至45°      
            if(PWM_F == 1)      
            {        
                PWM_F = 0;
                break;      
            }    
        }    
        while(1)    
        {      
            servo = PWM(15);                //旋转至90°      
            if(PWM_F == 1)      
            {        
                PWM_F = 0;        
                break;      
            }    
            }    
        while(1)    
        {      
                servo = PWM(20);                //旋转至135°      
                if(PWM_F == 1)      
                {        
                    PWM_F = 0;        
                    break;      
                }    
        }   
        while(1)    
        {      
            servo = PWM(25);                //旋转至180°      
            if(PWM_F == 1)      
            {       
                PWM_F = 0;        
                break;      
            }    
        } 
    }
}
void Timer0() interrupt 1
{  
    TH0 = (65536-PWM_T)/256;
    TL0 = (65536-PWM_T)%256;  
    PWM_count0++;  
    if(PWM_count0 == 200)                //过了20ms，完成了一个PWM周期  
    {    
        PWM_count0 = 0;                    //清零，重新开始计数    
        PWM_count1++;    
        if(PWM_count1 == 50)              //PWM_count0清零了50次，也就是说过了20ms*50=1000ms=1s    
        {      
            PWM_count1 = 0;                  //清零，重新开始计数      
            PWM_F = 1;                      //标志置1，说明已经过了2s    
        }  
    }
}