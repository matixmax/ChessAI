#include "FigInfo.h"

int8 FigInfo::getPosIndex(int8 fig, int8 color, int8 number) {
    //first pos for:       K  H  W  S  G  Pc Pb 
    assert(fig >= 0 && fig < 7);
    int8 index_maping[] = { 0, 1, 2, 4, 6, 8, 8 };
    int8 max_number[] = { 0, 0, 1, 1, 1, 7, 7 };
    number = std::min(number, max_number[fig]);
    return index_maping[fig] + number + 16 * color;
}

int8 FigInfo::getFigNumber(int8 pos_index, int8 color) {
    int8 index_maping[] = { 0, 1, 2, 2, 3, 3, 4, 4 };
    pos_index -= color * 16;
    if (pos_index < 8)
        return index_maping[pos_index];
    return 5 + color;
}

int8 FigInfo::not(int8 color) {
    return (color ^ 1);
}
