#include <Arduino.h>
#include "Trajectory.hpp"

Trajectory trajectory;

unsigned long timeToRead = 0;

void setup() {
    while (!Serial) ;
    trajectory.init(2, 24, 4, 10000);

    for (int i = 0; i < 2; i++) {
        float K[24][4];
        for (int j = 0; j < 24 * 4; j++) {
            ((float*)K)[j] = random(100) / 100.f;
        }
        trajectory.appendGainsMatrix((float*)K);
    }
    for (int i = 0; i < 10000; i++) {
        TrajectoryPoint pt;
        for (int j = 0; j < 12; j++) {
            pt.x[j] = random(100, 200) / 100.f;
            Serial.println(pt.x[j]);
        }
        for (int j = 0; j < 4; j++) {
            pt.uRef[j] = random(200, 300) / 100.f;
        }
        pt.time = i;
        trajectory.appendTrajectoryPoint(&pt);
    }

    Serial.printf("Gains written: %d, points: %d\n", trajectory.gainsCount, trajectory.pointsCount);

    for (int i = 0; i < 10000; i++) {
        elapsedMicros took = 0;
        TrajectoryPoint *pt = trajectory.getTrajectoryPoint(i);
        for (int j = 0; j < 12; j++) {
            Serial.print(pt->x[j]);
            Serial.print(",");
        }
        Serial.print("|");
        for (int j = 0; j < 4; j++) {
            Serial.print(pt->uRef[j]);
            Serial.print(",");
        }
        Serial.print("|");
        Serial.print(pt->time);
        Serial.println();

        // Serial.print("Took: ");
        // Serial.println(took);
        timeToRead = max((unsigned long)took, timeToRead);
    }

    Serial.println();
    Serial.printf("Max time to read point: %d\n", timeToRead);
}

void loop() {
}
