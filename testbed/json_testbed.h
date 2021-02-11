#pragma once

#include <qmt.h>

#include <QUuid>

struct Point
{
    double x = qQNaN();
    double y = qQNaN();

    Point() = default;
    Point(const double x, const double y) : x(x), y(y) {}
};

QMT_DECLARE_TYPE_INFO2(Point, x, y)

struct ShipInfo
{
    Point pos;
    QString name;
    int number = 1;
    QVariant value;
};

QMT_DECLARE_TYPE_INFO4(ShipInfo, pos, name, number, value)

struct Alarm
{
    QUuid uuid;
    ShipInfo ship;
};

QMT_DECLARE_TYPE_INFO2(Alarm, uuid, ship)

template <typename Type>
inline void testJsonCodecStl(const char* name, const Type& value)
{
    Q_UNUSED(name);
    const QJsonValue json = Qmt::toJsonValue(value);
    //qDebug() << "* Json Test: " << name;
    //qDebug() << "  serialize" << json;
    const auto readed = Qmt::fromJsonValue<Type>(json);
    Q_ASSERT(readed == value);
}

template <typename Type>
inline void testJsonCodec(const Type& value)
{
    const QJsonValue json = Qmt::toJsonValue(value);
    qDebug() << "* Json Test: " << QMetaType::typeName(qMetaTypeId<Type>());
    qDebug() << "  value " << value;
    qDebug() << "  serialize" << json;
    const auto readed = Qmt::fromJsonValue<Type>(json);
    qDebug() << "  readed" << readed;
    Q_ASSERT(readed == value);
}

void jsonTestbed()
{
    QMT_REGISTER(ShipInfo);
    QMT_REGISTER(Alarm);
    QMT_REGISTER(Point);

    ShipInfo ship;
    ship.name = "Ship1";
    ship.pos = Point{1, 3};
    ship.value = "123";

    qDebug() << "ship" << ship;
    qDebug() << "ship json" << Qmt::toJsonValue(ship);


    Alarm alarm;
    alarm.uuid = QUuid::createUuid();
    alarm.ship = ship;


    testJsonCodec(QVariantHash{{"ship", QVariant::fromValue(ship)}});

    auto json = Qmt::toJsonValue(alarm);
    //qDebug() << "alarm json" << json;
    //qDebug() << "alarm " << alarm;

    Alarm alarm2 = Qmt::fromJsonValue<Alarm>(json.toObject());
    //qDebug() << "alarm2" << alarm2;
    //qDebug() << "alarm cmp" << (alarm == alarm2);

    Q_ASSERT(json == Qmt::toJsonValue(alarm2));

    testJsonCodec(1u);
    testJsonCodecStl("std::list", std::list<Alarm>{alarm, alarm2});
    testJsonCodecStl("std::vector", std::vector<Alarm>{alarm, alarm2});
    testJsonCodecStl("std::map", std::map<std::string,Alarm>{{"a1", alarm}, {"a2", alarm2}});
    testJsonCodecStl("std::unordered_map", std::unordered_map<std::string,Alarm>{{"a1", alarm}, {"a2", alarm2}});
    testJsonCodec(QString{"Hello"});
    testJsonCodec(QList<int>{1, 3});
    testJsonCodec(QList<Alarm>{alarm, alarm2});

    testJsonCodec(QMap<QString,Alarm>{{"a1", alarm}, {"a2", alarm2}});
    testJsonCodec(QHash<QString,Alarm>{{"a1", alarm}, {"a2", alarm2}});
    testJsonCodec(QVariantHash{{"a1", QVariant::fromValue(alarm)},
                               {"a2", QVariant::fromValue(alarm2)}});
}
