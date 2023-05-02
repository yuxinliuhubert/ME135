#ifndef DATASTRUCT_H
#define DATASTRUCT_H

const size_t dataPoints = 2;
// COM Send struct
typedef struct com_send_struct {
    short int commanded_state;
} com_send_struct;

// COM receive struct
typedef struct com_receive_struct {
    short int daq_current_state;
    int data[dataPoints];

    // Add more fields as needed for the receiver
} com_receive_struct;

#endif
