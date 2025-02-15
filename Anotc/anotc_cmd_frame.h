#ifndef ANOTC_CMD_FRAME_H
#define ANOTC_CMD_FRAME_H

#define ANOTC_CMD_CALIBRATE_GYRO 0x0
#define ANOTC_CMD_CALIBRATE_ACCEL 0x1
#define ANOTC_CMD_REBOOT 0x3
#define ANOTC_CMD_TOGGLE_MOTOR_TEST_STATUS 0x4
#define ANOTC_CMD_MOTOR_TEST_THROLLE 0x5

void anotc_send_cmd_calibrate_gyro();
void anotc_send_cmd_calibrate_accel(char direction);
void anotc_send_cmd_reboot();
void anotc_send_cmd_motor_test_status(unsigned char enable);
void anotc_send_cmd_motor_test_throttle(unsigned short *value, int count);

#endif // ANOTC_CMD_FRAME_H
