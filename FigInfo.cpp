
#include "FigInfo.h"

int FigInfo::getPosIndex(int fig, int color, int number) {
    //first pos for:       K  H  W  S  G  Pc Pb 
    assert(fig >= 0 && fig < 7);
    int index_maping[] = { 0, 1, 2, 4, 6, 8, 8 };
    int max_number[] = { 0, 0, 1, 1, 1, 7, 7 };
    number = std::min(number, max_number[fig]);
    return index_maping[fig] + number + 16 * color;
}

int FigInfo::getFigNumber(int pos_index, int color) {
    if (color == -1)
        color = getColor(pos_index);
    int index_maping[] = { 0, 1, 2, 2, 3, 3, 4, 4 };
    pos_index -= color * 16;
    if (pos_index < 8)
        return index_maping[pos_index];
    else
        return 5 + color;
}

int FigInfo::getColor(int posIndex)
{
    return posIndex / 16;
}

int FigInfo::not(int color) {
    return (color ^ 1);
}
