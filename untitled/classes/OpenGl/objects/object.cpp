#include "object.h"

Object::Object()
{

}

void Object::add_instance(Object_instance instance)
{
    instances.append(instance);
}
