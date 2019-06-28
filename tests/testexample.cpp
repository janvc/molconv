#include "testexample.h"

//TestExample::TestExample(QObject *parent) : QObject(parent)
//{

//}

void TestExample::init()
{
}

void TestExample::testFoo()
{
    molconv::Molecule mol;

    QVERIFY(mol.size() == 0);
}

QTEST_APPLESS_MAIN(TestExample)
