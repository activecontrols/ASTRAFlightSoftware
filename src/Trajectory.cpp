#include <Arduino.h>
#include "Trajectory.hpp"
#include <cstring>

#define TRAJECTORY_MEM_SIZE_KB 256

EXTMEM uint8_t TRAJECTORY_MEM[TRAJECTORY_MEM_SIZE_KB * 1024];

Trajectory::Trajectory() {
    this->gainsCount = 0;
    this->pointsCount = 0;
}

void Trajectory::wipe() {
    for (int i = 0; i < TRAJECTORY_MEM_SIZE_KB * 1024; i++)
    TRAJECTORY_MEM[i] = 0;
}

int Trajectory::init(int p, int kDim1, int kDim2, int k) {
    TrajectoryHeader *header = (TrajectoryHeader*) TRAJECTORY_MEM;

    // Check bounds
    if (p * 384 > 3840) return TRAJECTORY_NOT_ENOUGH_GAINS_SPACE;
    if (k * 72 > (TRAJECTORY_MEM_SIZE_KB * 1024 - 4864))
        return TRAJECTORY_NOT_ENOUGH_SPACE;
    header->p = p;
    header->k = k;
    header->kDim1 = kDim1;
    header->kDim2 = kDim2;

    return TRAJECTORY_OK;
}

TrajectoryPoint* Trajectory::getTrajectoryPoint(int k) {
    TrajectoryPoint* ptr = (TrajectoryPoint*) (TRAJECTORY_MEM + 4864);
    ptr += k;
    return ptr;
}

void Trajectory::appendGainsMatrix(float *K) {
    this->storeGainsMatrix(K, this->gainsCount);
    this->gainsCount += 1;
}

void Trajectory::storeGainsMatrix(float *K, int p) {
    float* ptr = (float*) (TRAJECTORY_MEM + 1024);
    ptr += this->kDim1 * this->kDim2 * p;
    memcpy(ptr, K, sizeof(float) * this->kDim1 * this->kDim2);
}

void Trajectory::appendTrajectoryPoint(TrajectoryPoint *point) {
    this->storeTrajectoryPoint(point, this->pointsCount);
    this->pointsCount += 1;
}

void Trajectory::storeTrajectoryPoint(TrajectoryPoint *point, int k) {
    TrajectoryPoint *ptr = this->getTrajectoryPoint(k);
    // Serial.printf("Write to %p\n", ptr);
    memcpy(ptr->x, point->x, 12 * sizeof(float));
    memcpy(ptr->uRef, point->uRef, 4 * sizeof(float));
    ptr->time = point->time;
    // Serial.printf("Store point %d: x{", k);
    // for (int j = 0; j < 12; j++) Serial.printf("%d,", point->x[j]);
    // Serial.printf("}, uRef{");
    // for (int j = 0; j < 4; j++) Serial.printf("%d,", point->uRef[j]);
    // Serial.printf("}, time: %d\n", point->time);
}
