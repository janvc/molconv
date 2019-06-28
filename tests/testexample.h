#ifndef TESTEXAMPLE_H
#define TESTEXAMPLE_H

#include <QTest>

#include "source/molecule.h"

class TestExample : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void testFoo();
};

#endif // TESTEXAMPLE_H
