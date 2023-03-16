//
// Created by Trym Hamer Gudvangen on 3/16/23.
//

#include <iostream>

class WSDataFrame {
    short header{};
    short extended_payload = -1;
    short payload{};
public:
    int mask[4];

    WSDataFrame() {
        set_fin_bit(true);
        set_opcode_bits(0);
    };

    short get_bit(short value);
    void set_fin_bit(bool status);
    short get_fin_bit();
    void set_opcode_bits(char opCode);
    short get_opcode_bits();
    void set_mask_bit(bool status);
    short get_mask_bit();
    void set_payload_length(int length);
    short get_payload_length();
    int *get_mask();
    void set_mask(int &frame_mask);

};

void WSDataFrame::set_fin_bit(bool status) {
    header |= status;
}
/**
 * Operation code is 4 bits long.
0 -> Continuation frame
1 -> Text frame
2 -> Binary frame
8 -> Connection close
9 -> Ping
A -> Pong
etc. -> Reserved
 * @param opCode
 */
void WSDataFrame::set_opcode_bits(char opCode) {
    header |= opCode & 0x0F;
}

void WSDataFrame::set_mask_bit(bool status) {
    header |= (int) status << 8;
}

void WSDataFrame::set_payload_length(int length) {
    if (length > 128); //TODO: extend payload length;
    header |= length << 9;
}

short WSDataFrame::get_fin_bit() {
    return get_bit(0);
}

short WSDataFrame::get_opcode_bits() {
    return get_bit(7) << 4
    | get_bit(6) << 3
    | get_bit(5) << 2
    | get_bit(4) << 1;
}

short WSDataFrame::get_mask_bit() {
    return get_bit(8);
}

short WSDataFrame::get_payload_length() {
    return (short) (header >> 9);
}

short WSDataFrame::get_bit(short value) {
    return (short) ((header >> value) & 1);
}
