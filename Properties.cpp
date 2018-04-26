#include "Properties.h"
#include <array>
#include "Definitions.h"

using namespace std;

array<int, 2> Properies::pawnsPosFact   { 2, 2 };
array<int, 2> Properies::knightsPosFact { 1, 1 };
array<int, 2> Properies::bishopsPosFact { 1, 1 };
array<int, 2> Properies::rocksPosFact   { 1, 1 };
array<int, 2> Properies::queenPosFact   { 1, 1 };
array<int, 2> Properies::kingsPosFact   { 1, 1 };

string Properies::getFactorsAsString()
{
    stringstream stream;
    
    addToStream(stream, "pawnsPosFact", pawnsPosFact);
    addToStream(stream, "knightsPosFact", knightsPosFact);
    addToStream(stream, "bishopsPosFact", bishopsPosFact);
    addToStream(stream, "rocksPosFact", rocksPosFact);
    addToStream(stream, "queenPosFact", queenPosFact);
    addToStream(stream, "kingsPosFact", kingsPosFact);
    return stream.str();
}

void Properies::addToStream(std::stringstream &stream, std::string name, const std::array<int, 2>& val)
{
    stream.width(15);
    stream << left << name; 
    stream.width(15);
    stream << left << "WHITE " + to_string(val[WHITE]);
    stream << "BLACK " << val[BLACK] << endl;
}
