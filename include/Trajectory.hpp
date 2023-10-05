#ifndef TRAJECTORY_H
#define TRAJECTORY_H

class TrajectoryPoint {
public:
    float K[24][4]; // Gains matrix for trajectory error
    float x[12]; // State trajectory vector
    float u_ref[4]; // "Reference" inputs (what is nominally needed to get here from previous state)
};

class Trajectory {
public:
    TrajectoryPoint getTrajectoryPoint(int k); // Fetches trajectory point k
    void storeTrajectoryPoint(); // Store a trajectory point at end of trajectory block
    void storeTrajectoryPoint(int k); // Store a trajectory point at position k if it exists (overwrites existing)
    void deleteTrajectoryData(); // Delete all trajectory data
    void deleteTrajectoryPoint(int k); // Delete trajectory point k
private:
    TrajectoryPoint trajectoryBuffer[8]; // Hold next few trajectory points
};

#endif // TRAJECTORY_H
