#include <stdio.h>

#define ESTIMATED_STATE_DIMENSION 6
#define MEASUREMENT_DIMENSION 9
#define U_ROW_LENGTH 4

typedef struct {
    float t;                               // time
    float battVoltage;                     // battery voltage
    float x[ESTIMATED_STATE_DIMENSION];    // state
    float y[MEASUREMENT_DIMENSION];        // measurement
    float u[U_ROW_LENGTH];                 // control
} Data;

int main() {
    // read in file:
    FILE *inFilePtr = fopen("./LOG.BIN", "rb");
    if (inFilePtr == NULL) {
        perror("Error opening file");
        return 1;
    }
    // write csv
    FILE *outFilePtr = fopen("log.csv", "w");

    if (inFilePtr == NULL) {
        fclose(inFilePtr);
        inFilePtr = NULL;
        return -1;
    }

    int count = 0;

    char* header = "t,battVoltage,x0,x1,x2,x3,x4,x5,y0,y1,y2,y3,y4,y5,y6,y7,y8,u0,u1,u2,u3\n"; // remember to change if data changes
    fprintf(outFilePtr, "%s", header);
    while (1) {
        Data d;
        int bytesRead = fread(&d, sizeof(Data), 1, inFilePtr);
        if (bytesRead != 1) {
            fclose(inFilePtr);
            fflush(outFilePtr);
            fclose(outFilePtr);
            printf("Done writing. Wrote %d data points.\n", count);
            break;
        }
        count++;
        fprintf(outFilePtr, "%f,%f," // ideally this would be generated according to the defined constants but we could really lose perf on that.
        "%f,%f,%f,%f,%f,%f,"
        "%f,%f,%f,%f,%f,%f,%f,%f,%f,"
        "%f,%f,%f,%f\n",
        d.t, d.battVoltage,
        d.x[0], d.x[1], d.x[2], d.x[3], d.x[4], d.x[5],
        d.y[0], d.y[1], d.y[2], d.y[3], d.y[4], d.y[5], d.y[6], d.y[7], d.y[8],
        d.u[0], d.u[1], d.u[2], d.u[3]);
    }
}

