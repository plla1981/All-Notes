#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

/* 定义获取任务时间变量 */

u_int64_t getLEDTime = 0;
u_int64_t getWhileTime = 0; 

/* 定时器1回调函数 */
void timer1_Callback(void *arg);

static uint8_t s_led_state = 0;
static const char *TAG = "blink LED & 16bit timer";
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
    esp_timer_start_periodic(timer1_handler,200 * 1000);

    while(1)
    {
        static uint8_t i = 0;
        getWhileTime = esp_timer_get_time();    /* 获取自启动以来的时间（单位us） */

        ESP_LOGI(TAG, "WHILE Time is : %lld", getWhileTime);  /* 通过log打印出来 */
        ESP_LOGI(TAG, "While %d ", i++);
        vTaskDelay(200 / portTICK_PERIOD_MS);   /* 延时300ms */
        i > 100 ? i = 0 : i;
    }

}

/* 定时器1回调函数 */
void timer1_Callback(void *arg)
{
    getLEDTime = esp_timer_get_time();    /* 获取自启动以来的时间（单位us） */
    ESP_LOGI(TAG, "LED Time is : %lld", getLEDTime);  /* 通过log打印出来 */

    ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
    gpio_set_level(22,s_led_state);         /* GPIO26输出电平 */

    s_led_state = !s_led_state;             /* 取反LED的状态 */
}
