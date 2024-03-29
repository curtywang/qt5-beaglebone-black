/****************************************************************************
**
** Copyright (C) 2017 Ford Motor Company
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtRemoteObjects module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "rep_MyInterface_replica.h"

#include <QCoreApplication>
#include <QtRemoteObjects/qremoteobjectnode.h>
#include <QtTest/QtTest>

class tst_Client_Process : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
        m_repNode.connectToNode(QUrl(QStringLiteral("tcp://127.0.0.1:65213")));
        m_rep.reset(m_repNode.acquire<MyInterfaceReplica>());
        QVERIFY(m_rep->waitForSource());
    }

    void testRun()
    {

        auto reply = m_rep->start();
        QVERIFY(reply.waitForFinished());

        // BEGIN: Testing
        QSignalSpy advanceSpy(m_rep.data(), SIGNAL(advance()));

        QSignalSpy spy(m_rep.data(), SIGNAL(enum1Changed(MyInterfaceReplica::Enum1)));
        QVERIFY(advanceSpy.wait());

        QCOMPARE(spy.count(), 2);
        // END: Testing

        reply = m_rep->stop();
        QVERIFY(reply.waitForFinished());
    }

    void testEnumDetails()
    {
        QHash<QByteArray, int> kvs = {{"First", 0}, {"Second", 1}, {"Third", 2}};
        QScopedPointer<QRemoteObjectDynamicReplica> rep(m_repNode.acquireDynamic("MyInterface"));
        QVERIFY(rep->waitForSource());

        auto mo = rep->metaObject();
        int enumIdx = mo->indexOfEnumerator("Enum1");
        QVERIFY(enumIdx != -1);
        auto enumerator = mo->enumerator(enumIdx);
        QCOMPARE(enumerator.name(), "Enum1");
        QCOMPARE(enumerator.keyCount(), 3);
        for (int i = 0; i < 3; ++i) {
            auto key = enumerator.key(i);
            auto val = enumerator.value(i);
            auto it = kvs.find(key);
            QVERIFY(it != kvs.end());
            QCOMPARE(*it, val);
            kvs.erase(it);
        }

        int propIdx = mo->indexOfProperty("enum1");
        QVERIFY(propIdx != -1);
        auto property = mo->property(propIdx);
        property.write(rep.data(), 1);
        QTRY_COMPARE(property.read(rep.data()).toInt(), 1);
    }

    void testMethodSignalParamDetails()
    {
        QScopedPointer<QRemoteObjectDynamicReplica> rep(m_repNode.acquireDynamic("MyInterface"));
        QVERIFY(rep->waitForSource());

        auto mo = rep->metaObject();
        int signalIdx = mo->indexOfSignal("testEnumParamsInSignals(Enum1,bool,QString)");
        QVERIFY(signalIdx != -1);
        auto simm = mo->method(signalIdx);
        {
            QCOMPARE(simm.parameterCount(), 3);
            auto paramNames = simm.parameterNames();
            QCOMPARE(paramNames.size(), 3);
            QCOMPARE(paramNames.at(0), "enumSignalParam");
            QCOMPARE(paramNames.at(1), "signalParam2");
            QCOMPARE(paramNames.at(2), "__repc_variable_1");
        }

        int slotIdx = mo->indexOfSlot("testEnumParamsInSlots(Enum1,bool,int)");
        QVERIFY(slotIdx != -1);
        auto slmm = mo->method(slotIdx);
        {
            QCOMPARE(slmm .parameterCount(), 3);
            auto paramNames = slmm .parameterNames();
            QCOMPARE(paramNames.size(), 3);
            QCOMPARE(paramNames.at(0), "enumSlotParam");
            QCOMPARE(paramNames.at(1), "slotParam2");
            QCOMPARE(paramNames.at(2), "__repc_variable_1");
        }

        int enumVal = 0;
        mo->invokeMethod(rep.data(), "testEnumParamsInSlots",
                                    QGenericArgument("Enum1", &enumVal),
                                    Q_ARG(bool, true), Q_ARG(int, 1234));

        int enumIdx = mo->indexOfProperty("enum1");
        QVERIFY(enumIdx != -1);
        QTRY_COMPARE(mo->property(enumIdx).read(rep.data()).toInt(), 0);

        int startedIdx = mo->indexOfProperty("started");
        QVERIFY(startedIdx != -1);
        QTRY_COMPARE(mo->property(startedIdx).read(rep.data()).toBool(), true);
    }

    void cleanupTestCase()
    {
        auto reply = m_rep->quit();
        QVERIFY(reply.waitForFinished());
    }

private:
    QRemoteObjectNode m_repNode;
    QScopedPointer<MyInterfaceReplica> m_rep;
};

QTEST_MAIN(tst_Client_Process)

#include "main.moc"
