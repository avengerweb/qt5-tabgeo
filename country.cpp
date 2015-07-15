#include "country.h"
namespace TabGeo {
    Country::Country(QString file)
    {
        QFile geo(file);

        if (!geo.open(QIODevice::ReadOnly))
            qWarning() << "Couldn`t open IP database file";
        else {
            m_ipDatabase = geo.readAll();
            geo.close();
        }
    }

    Country::~Country()
    {}

    QString Country::getCountryCode(quint32 id) const
    {
        return (quint32)m_countries.length() > id ? m_countries.at(id) : "Unknown";
    }

    QList<QByteArray> Country::slice(QByteArray arr, int count)
    {
        QList<QByteArray> array;
        int maxLength = arr.length();
        int length = 0;

        while(true)
        {
            if (length >= maxLength)
                return array;

            array.append(arr.mid(length, count));
            length += count;
        }

        return array;
    }

    Country::StreamedData Country::parsePacket(QList<QByteArray> data, QString ipPart, bool step)
    {
        int start = 0;
        int end = data.length() - 1;

        StreamedData packetPrev, packet;

        while(true) {
            packet = StreamedData();
            int mid = qFloor((start + end) / 2);
            if (step)
            {
                QByteArray tempArray = data[mid];
                QDataStream ds(tempArray.prepend(char(0x00)));
                ds >> packet.offset;
                ds >> packet.ip;
                ds >> packet.cId;
            }
            else
            {
                QDataStream ds(data[mid]);
                ds >> packet.ip;
                ds >> packet.cId;
            }

            if (packet.ip == ipPart.toInt())
                return packet;

            if (end - start  <   0)
                return ipPart.toInt() > packet.ip ? packet : packetPrev;

            if (ipPart.toInt() < packet.ip)
                end = mid - 1;
            else
                start = mid + 1;

            packetPrev = packet;
        }

        return packet;
    }

    QString Country::get(QString ip)
    {
        if (QAbstractSocket::IPv4Protocol != QHostAddress(ip).protocol() || m_ipDatabase.isEmpty())
            return "Unknown";

        QStringList splitedIp = ip.split(".");

        QByteArray arr = m_ipDatabase.mid((splitedIp[0].toInt() * 256 + splitedIp[1].toInt()) * 4, 4);

        arr.prepend(char(0x00));
        QDataStream ds(arr);

        StreamedData packet;
        ds >> packet.offset;
        ds >> packet.length;


        if(packet.offset == 16777215)
            return getCountryCode(packet.length);

        arr = m_ipDatabase.mid(packet.offset * 5 + 262144, (packet.length + 1) * 5);

        packet = parsePacket(slice(arr, 5), splitedIp[2], true);

        if (packet.offset == 16777215)
            return getCountryCode(packet.cId);

        if (splitedIp[2].toInt() > packet.ip)
            splitedIp[3] = 255;

        arr = m_ipDatabase.mid(m_ipDatabase.length() - ((packet.offset + 1 + packet.cId) * 2), (packet.cId + 1) * 2);

        packet = parsePacket(slice(arr, 2), splitedIp[3], false);

        return getCountryCode(packet.cId);
    }
}
