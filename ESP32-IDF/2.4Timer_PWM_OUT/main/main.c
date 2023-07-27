#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#define PWMPIN          22
#define GPIOHIGH()      (gpio_set_level(PWMPIN, 1))
#define GPIOLOW()       (gpio_set_level(PWMPIN, 0))

uint64_t getWhileTime = 0;

/* 定义占空比 = pwm_high / pwm_all*/
uint8_t pwm_all = 100;      //进入定时器中断的次数变量
uint8_t pwm_high = 88;      //高电平的次数，控制此变量，就可以控制pwm的占空比（PWM的值）
uint8_t pwm_cnt = 0;        //进入中断的次数，计数变量

static const char *TAG = "blink LED & 16bit timer";

/* 定时器1回调函数 */
void timer1_Callback(void *arg);

static esp_timer_handle_t timer1_handler;  /* 定时器1的句柄 */

/* 定时器1的参数 */
static esp_timer_create_args_t timer_Once_Obj = {
    .name = "Timer_NUM_1",          /* 定时器的名称 */
    .arg = NULL,                    /* 传递给回调函数的参数 */
    .callback = &timer1_Callback,   /* 回调函数 */
};

void app_main(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");

    /* 复位GPIO的状态 */
    gpio_reset_pin(22);

    /* 设置GPIO为输出模式 */
    gpio_set_direction(22,GPIO_MODE_OUTPUT);

    /* 初始化定时器 */
    esp_timer_init();

    /* 创建定时器1 */
    esp_timer_create(&timer_Once_Obj,&timer1_handler);

    /* 启动定时器1（周期性） */
    esp_timer_start_periodic(timer1_handler,10);
    
    while(1)
    {
        static uint8_t i = 0;
        getWhileTime = esp_timer_get_time();    /* 获取自启动以来的时间（单位us） */

        ESP_LOGI(TAG, "WHILE Time is : %lld", getWhileTime);  /* 通过log打印出来 */
        ESP_LOGI(TAG, "While %d ", i++);
        vTaskDelay(200 / portTICK_PERIOD_MS);   /* 延时ms */
        i > 100 ? i = 0 : i;
    }
}

/* 定时器1回调函数 */
void timer1_Callback(void *arg)
{
    pwm_cnt++;

    if (pwm_cnt < pwm_high){
        GPIOHIGH();     /* GPIO输出电平 */
    }
    else if (pwm_cnt < pwm_all){
        GPIOLOW();      /* GPIO输出电平 */
    }
    else {
        pwm_cnt = 0;
        GPIOHIGH();     /* GPIO输出电平 */
    }
}
