#include "therapy.h"

Therapy::Therapy(QString w, float f,int c,int p,QDateTime s) :
   waveform(w),
   frequency(f),
   countdownCycle(c),
   current(p),
   startTime(s)
{}

Therapy::~Therapy(){}

QString Therapy::getWaveform() { return waveform ; }
float Therapy::getFrequency() { return frequency; }
QDateTime Therapy::getStart() { return startTime; }
int Therapy::getCurrent() {return current; }
int Therapy::getTime() { return countdownCycle; }

void Therapy::setTime(int newt){ countdownCycle = newt; }
void Therapy::setCurrent(int newp){ current = newp; }
void Therapy::setStart(QDateTime news){ startTime = news; }
void Therapy::setFrequency(float newf){ frequency = newf ;}
void Therapy::setWaveform(QString neww){ waveform = neww; }
