#ifndef THERAPY_H
#define THERAPY_H

#include "QString"
#include "QStringList"
#include <QVector>
#include <QDateTime>


class Therapy
{
public:
    Therapy(QString="Alpha", float=0.5, int=20, int=1, QDateTime = QDateTime::currentDateTime());
    ~Therapy();

    QString getWaveform();
    float getFrequency();
    QDateTime getStart();
    int getCurrent();
    int getTime();

    void setTime(int);
    void setCurrent(int);
    void setStart(QDateTime);
    void setFrequency(float);
    void setWaveform(QString);

private:
    QString waveform;
    float frequency;
    int countdownCycle;
    int current;
    QDateTime startTime;
};

#endif // MENU_H
