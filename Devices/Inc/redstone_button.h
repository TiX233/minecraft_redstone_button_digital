#ifndef __RS_BUTTON_H__
#define __RS_BUTTON_H__

#include "redstone.h"

// 按键输出延时，单位：红石刻（100ms）
typedef enum {
    RS_BUTTON_CD_STONE = 10,                        // 石制按键
    RS_BUTTON_CD_WOOD = 15,                         // 木制按键
    RS_BUTTON_CD_FAST = 1,                          // 快速按键

    RS_BUTTON_CD_MAX = 0xFF,
} rs_button_delay_e;

typedef enum {
    RS_BUTTON_CH1_up = 0,
    RS_BUTTON_CH2_down,
    RS_BUTTON_CH3_left,
    RS_BUTTON_CH4_right,

    RS_BUTTON_CH_NUM,
} rs_button_chs_e;

struct rs_button_stu {
    void (*led_pin_write)(uint8_t pin_state);
    uint8_t (*button_pin_read)(void);

    uint8_t button_last_state;
    const rs_button_delay_e button_delay;
    uint8_t button_cd;

    struct rs_io_stu io;
};


void rs_button_init(struct rs_button_stu *rsl);

uint8_t my_rs_io_pin_read(struct rs_channel_stu *ch);
void my_rs_io_pin_write(struct rs_channel_stu *ch, uint8_t pin_state);

void my_rs_io_callback_channel_change(struct rs_io_stu *io);
void my_rs_io_callback_communication_init(struct rs_io_stu *io);
void my_rs_io_callback_communication_over(struct rs_io_stu *io);

void my_rs_button_led_pin_write(uint8_t pin_state);
uint8_t my_rs_button_button_pin_read(void);

#endif // __RS_BUTTON_H__
