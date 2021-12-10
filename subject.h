#ifndef SUBJECT_H
#define SUBJECT_H

#include <list>

using namespace std;

class Subject;

class Observer {
   public:
       virtual ~ Observer();
       virtual void Update(Subject* s) = 0;
   protected:
       Observer();
};

class Subject{
    public:
        virtual ~Subject();

        virtual void Attach(Observer *o) = 0;
        virtual void Detach(Observer *o) = 0;
        virtual void Notify() = 0;
    protected:
        Subject();
    private:
        list<Observer*> *observer_list;
};

#endif // SUBJECT_H
