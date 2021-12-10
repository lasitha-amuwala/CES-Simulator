#include "subject.h"


using namespace std;

Subject::Subject(){}
Subject::~Subject(){}

void Subject::Attach(Observer *o){
    observer_list->push_back(o);
}

void Subject::Detach(Observer *o){
    observer_list->remove(o);
}

void Subject::Notify() {
    std::list<Observer *>::iterator iter = observer_list->begin();

        while (iter != observer_list->end()) {
          (*iter)->Update(this);
          ++iter;
    }
}
