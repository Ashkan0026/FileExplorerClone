#ifndef CONVERTER_H
#define CONVERTER_H
#include <QString>

namespace size_converter
{
    qint64 ExtractGigabytes(qint64 size);

    qint64 ExtractMegabytes(qint64 size);

    qint64 ExtractKilobytes(qint64 size);
}

#endif // CONVERTER_H
