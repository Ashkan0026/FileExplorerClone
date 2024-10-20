#include "converter.h"
#include <cmath>

qint64 size_converter::ExtractGigabytes(qint64 size)
{
    qint64 gigabyte = size / (static_cast<qint64>(std::pow(10, 9)));
    return gigabyte;
}

qint64 size_converter::ExtractMegabytes(qint64 size)
{
    qint64 megabyte = size / (static_cast<qint64>(std::pow(10, 6)));
    return megabyte;
}

qint64 size_converter::ExtractKilobytes(qint64 size)
{
    qint64 kilobyte = size / (static_cast<qint64>(std::pow(10, 3)));
    return kilobyte;
}
