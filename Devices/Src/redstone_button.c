#include "redstone_button.h"
#include "myTasks.h"

void rs_button_init(struct rs_button_stu *rsb){
    // 将所有引脚设为高电平
    for(uint8_t i = 0; i < RS_BUTTON_CH_NUM; i ++){
        rsb->io.pin_write(&(rsb->io.channels[i]), 1);
    }

    // 重置引脚输出数据脚本，预设引脚电平全设置为 1
    for(uint8_t i = 0; i < RS_BUTTON_CH_NUM; i ++){
        for(uint8_t j = 0; j < RS_CHANNEL_STEP_max; j ++){
            rsb->io.channels[i].pin_script[j] = 1;
        }
    }
    // 关闭 led
    rsb->led_pin_write(1);
}


uint8_t my_rs_io_pin_read(struct rs_channel_stu *ch){
    return HAL_GPIO_ReadPin(ch->pin.port, ch->pin.pin);
}

void my_rs_io_pin_write(struct rs_channel_stu *ch, uint8_t pin_state){
    HAL_GPIO_WritePin(ch->pin.port, ch->pin.pin, pin_state);
}

void my_rs_io_callback_communication_init(struct rs_io_stu *io){
    uint8_t button_state = my_rs_button.button_pin_read();
    uint8_t flag_output = 0;

    if(my_rs_button.button_last_state == 1){
        if(button_state == 0){ // 按键下降沿
            if(my_rs_button.button_cd > my_rs_button.button_delay){ // 按钮空闲，切换为输出
                my_rs_button.button_cd = my_rs_button.button_delay;
            }
        }
    }my_rs_button.button_last_state = button_state;

    if(my_rs_button.button_cd <= my_rs_button.button_delay){ // 按钮正在输出倒计时
        if(0 == my_rs_button.button_cd--){ // 倒计时完毕，关闭输出
            flag_output = 0;
        }else {
            flag_output = 1;
        }
    }

    // 设置引脚通信脚本
    if(flag_output == 0){ // 不输出信号，熄灭 led
        for(uint8_t i = 0; i < RS_BUTTON_CH_NUM; i ++){
            for(uint8_t j = RS_CHANNEL_STEP_3_set_r16; j < RS_CHANNEL_STEP_19_set_ack; j ++){
                my_rs_button.io.channels[i].pin_script[j] = 1;
            }
        }

        my_rs_button.led_pin_write(1);
    }else { // 输出强度为 16 的信号，点亮 led
        for(uint8_t i = 0; i < RS_BUTTON_CH_NUM; i ++){
            for(uint8_t j = RS_CHANNEL_STEP_3_set_r16; j < RS_CHANNEL_STEP_19_set_ack; j ++){
                my_rs_button.io.channels[i].pin_script[j] = 0;
            }
        }

        my_rs_button.led_pin_write(0);
    }

    // 确保 1ms 后再进入通信
    task_rs_io.topic.flag = 0;
}

void my_rs_io_callback_channel_change(struct rs_io_stu *io){
    // 没有事情需要做
}

void my_rs_io_callback_communication_over(struct rs_io_stu *io){
    // 没什么事情做
}


void my_rs_button_led_pin_write(uint8_t pin_state){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, pin_state);
}

uint8_t my_rs_button_button_pin_read(void){
    return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
}
