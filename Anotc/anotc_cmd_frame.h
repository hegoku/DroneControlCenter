#ifndef ANOTC_CMD_FRAME_H
#define ANOTC_CMD_FRAME_H

#define ANOTC_CMD_CALIBRATE_GYRO 0x0
#define ANOTC_CMD_CALIBRATE_ACCEL 0x1

void anotc_send_cmd_calibrate_gyro();
void anotc_send_cmd_calibrate_accel(char direction);

#endif // ANOTC_CMD_FRAME_H
