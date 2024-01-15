#ifndef FRAME_DATA_H
#define FRAME_DATA_H
#include <iostream>
#include <vector>
enum TYPE_FRAME
{
    U16 = 0,
    U_8 = 1
};

struct Frame_settings
{
    uint16_t m_type_Frame;
    std::vector<std::vector<uint16_t>> m_frame_buffer;
};
#endif // FRAME_DATA_H
