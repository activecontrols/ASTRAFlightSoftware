/*
Error.h - Error Header/Constants File
Description: Error Constants for General Errors that can occur throughout Astra
Author: Vincent Palmerio
*/

#ifndef ERROR_H
#define ERROR_H

#define NO_ERROR_CODE (0)
#define GENERAL_ERROR_CODE (-1)
#define MEMORY_ALLOCATION_ERROR_CODE (-2)
#define VECTOR_INIT_ZERO_SIZE_ERROR (-3) // size of vector cannot be 0
#define MAX_ALLOWED_TIME_BETWEEN_INTEGRATION_EXCEEDED (-4)
#define VECTOR_SIZE_MISMATCH (-5)

#define INNER_GIMBAL_NOT_ATTACHED (-6)
#define OUTER_GIMBAL_NOT_ATTACHED (-7)
#define LEFT_TORQUE_VANE_NOT_ATTACHED (-8)
#define RIGHT_TORQUE_VANE_NOT_ATTACHED (-9)
#define ESC_NOT_ATTACHED (-14)

#define ROW_KGAIN_MISMATCH (-10)
#define COLUMN_KGAIN_MISMATCH (-11)
#define COLUMN_QSM_MISMATCH (-12)

#define CANNOT_REQUEST_MISSION_TIME_IF_MISSION_NOT_STARTED (-13)

#define INVALID_ESC_BUFFER_INIT (-15)
#define INVALID_THROTTLE_ESC (-16)

#define ENCODER_NOT_ATTACHED (-17)

#define FAILED_CALIBRATION_HELPER_INIT (-100)
#define FAILED_LOAD_CALIBRATION (-101)
#define FAILED_SENSOR_INIT (-102)

#endif