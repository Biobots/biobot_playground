#include "cell.h"

Cell::Cell()
{

}

Cell::~Cell()
{

}

bool Cell::getExistence()
{
    return isExist;
}

int Cell::setExistence(bool value)
{
    isExist = value;
    return 0;
}