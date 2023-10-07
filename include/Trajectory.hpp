#ifndef TRAJECTORY_H
#define TRAJECTORY_H
#include <cstdint>

// 4MB trajectory memory buffer
extern uint8_t TRAJECTORY_MEM[];

// Size: 1024B max;
typedef struct {
    int p; // # Trajectory segments = # Gain Matrices
    int kDim1; // First dimension of gain matrix
    int kDim2; // Second dimension of gain matrix
    int k; // # Trajectory points total
} TrajectoryHeader;

typedef struct {
    float *K; // K gains matrix (variable dims)
} Gains;

// Size: 96B;
typedef struct {
    float x[12]; // State trajectory vector
    float uRef[4]; // "Reference" inputs (what is nominally needed to get here from previous state)
    float time;
} TrajectoryPoint;

enum TrajectoryStatus {
    TRAJECTORY_OK = 0,
    TRAJECTORY_NOT_ENOUGH_SPACE = -1,
    TRAJECTORY_NOT_ENOUGH_GAINS_SPACE = -2,
};

class Trajectory {
public:
    Trajectory();
    void wipe(); // Wipes any existing trajectory
    int init(int p, int kDim1, int kDim2, int k); // Initializes trajectory header block with data
    int readFromSD(); // Read trajectory data from SD card. Requires SD initialized beforehand.
    TrajectoryPoint* getTrajectoryPoint(int k); // Fetches trajectory point k
    void appendGainsMatrix(float *K); // Store a gains matrix
    void storeGainsMatrix(float *K, int p); // Store a gains matrix
    void appendTrajectoryPoint(TrajectoryPoint *point); // Store a trajectory point at end of trajectory block
    void storeTrajectoryPoint(TrajectoryPoint *point, int k); // Store a trajectory point at position k if it exists
    int kDim1;
    int kDim2;
    int gainsCount;
    int pointsCount;
};

#endif // TRAJECTORY_H
