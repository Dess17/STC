#include "tle.h"

#include <QMap>
#include <QStringList>

static int fullYear(int year)
{
    return year < 57 ? 2000 + year : 1900 + year;
}

static QString checkLine(const QString &line, char type)
{
    if (line.size() != 69 || line[0] != type || line[1] != ' ')
        return QString("ожидалась строка %1 формата TLE").arg(type);

    int sum = 0;
    for (int i = 0; i < 68; i++) {
        if (line[i].isDigit())
            sum += line[i].digitValue();
        else if (line[i] == '-')
            sum += 1;
    }
    if (line[68].digitValue() != sum % 10)
        return "неверная контрольная сумма";
    return QString();
}

QVector<Satellite> parseTle(const QString &text, QString &error)
{
    QStringList lines;
    QVector<int> numbers;
    QStringList all = text.split('\n');
    for (int i = 0; i < all.size(); i++) {
        QString line = all[i].trimmed();
        if (!line.isEmpty()) {
            lines << line;
            numbers << i + 1;
        }
    }
    if (lines.isEmpty()) {
        error = "нет данных";
        return {};
    }

    QVector<Satellite> satellites;
    int i = 0;
    while (i < lines.size()) {
        Satellite s;
        int start = numbers[i];
        if (!lines[i].startsWith("1 ")) {
            s.name = lines[i];
            i++;
        }
        if (i + 1 >= lines.size()) {
            error = QString("строка %1: неполная запись").arg(start);
            return {};
        }

        QString problem = checkLine(lines[i], '1');
        int bad = i;
        if (problem.isEmpty()) {
            problem = checkLine(lines[i + 1], '2');
            bad = i + 1;
        }
        if (!problem.isEmpty()) {
            error = QString("строка %1: %2").arg(numbers[bad]).arg(problem);
            return {};
        }

        const QString &line1 = lines[i];
        const QString &line2 = lines[i + 1];
        if (s.name.isEmpty())
            s.name = line1.mid(2, 5).trimmed();

        QString launch = line1.mid(9, 2).trimmed();
        if (!launch.isEmpty())
            s.launchYear = fullYear(launch.toInt());

        int epochYear = fullYear(line1.mid(18, 2).toInt());
        double epochDay = line1.mid(20, 12).toDouble();
        s.epoch = QDateTime(QDate(epochYear, 1, 1), QTime(0, 0), Qt::UTC).addMSecs(qRound64((epochDay - 1) * 86400000));
        s.inclination = line2.mid(8, 8).toDouble();

        satellites << s;
        i += 2;
    }
    return satellites;
}

QString statistics(const QVector<Satellite> &satellites)
{
    QDateTime oldest = satellites[0].epoch;
    QMap<int, int> years;
    QMap<int, int> inclinations;
    for (const Satellite &s : satellites) {
        if (s.epoch < oldest)
            oldest = s.epoch;
        if (s.launchYear > 0)
            years[s.launchYear]++;
        inclinations[qRound(s.inclination)]++;
    }

    QString text;
    text += QString("Всего спутников: %1\n").arg(satellites.size());
    text += "Самые старые данные: " + oldest.toString("dd.MM.yyyy hh:mm:ss") + " UTC\n";
    text += "\nЗапущено по годам:\n";
    for (auto it = years.begin(); it != years.end(); ++it)
        text += QString("%1: %2\n").arg(it.key()).arg(it.value());
    text += "\nНаклонение орбиты:\n";
    for (auto it = inclinations.begin(); it != inclinations.end(); ++it)
        text += QString("%1°: %2\n").arg(it.key()).arg(it.value());
    return text;
}
