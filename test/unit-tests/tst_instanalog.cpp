#include <QString>
#include <QtTest>

#include "instruments/instanalog.h"

class TestInstAnalog : public QObject
{
    Q_OBJECT

public:
    TestInstAnalog();

private Q_SLOTS:
    void testCase1();
};

TestInstAnalog::TestInstAnalog()
{
}

void TestInstAnalog::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(InstAnalog)

#include "tst_instanalog.moc"
