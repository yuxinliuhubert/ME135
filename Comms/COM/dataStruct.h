#ifndef DATASTRUCT_H
#define DATASTRUCT_H

const size_t dataPoints = 2;
#define BYTEFORNUM 4
const size_t byteSize = dataPoints*BYTEFORNUM;
// COM Send struct
typedef struct com_send_struct {
    short int commanded_state;
} com_send_struct;

// COM receive struct
typedef struct com_receive_struct {
    short int daq_current_state;
    // String dataString;
    uint8_t daq_send_data[byteSize];

    // Add more fields as needed for the receiver
} com_receive_struct;

#endif
