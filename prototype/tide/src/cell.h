#ifndef CELL_H
#define CELL_H

class Cell
{
private:
    bool isExist;
public:
    float xPos;
    float yPos;
    bool isExistCopy;

    Cell();
    ~Cell();

    bool getExistence();
    int setExistence(bool value);
};

#endif