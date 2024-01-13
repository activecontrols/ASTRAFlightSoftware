/*
Error.h - Error Header/Constants File
Description: Error Constants for General Errors that can occur throughout Astra
Author: Vincent Palmerio
*/



#ifndef ERROR_H
#define ERROR_H

#define NO_ERROR_CODE 0
#define GENERAL_ERROR_CODE -1
#define MEMORY_ALLOCATION_ERROR_CODE -2
#define VECTOR_INIT_ZERO_SIZE_ERROR -3 // size of vector cannot be 0
#define MAX_ALLOWED_TIME_BETWEEN_INTEGRATION_EXCEEDED -4
#define VECTOR_SIZE_MISMATCH -5

#define INNER_GIMBAL_NOT_ATTACHED -6
#define OUTER_GIMBAL_NOT_ATTACHED -7
#define LEFT_TORQUE_VANE_NOT_ATTACHED -8
#define RIGHT_TORQUE_VANE_NOT_ATTACHED -9

#endif