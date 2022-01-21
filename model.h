#ifndef MODEL_H
#define MODEL_H
#include "data.h"

class View;

class Model
{
private:
    View* view;
    Data* data;
public:
    Model();
    void setView(View* v);
};

#endif // MODEL_H
