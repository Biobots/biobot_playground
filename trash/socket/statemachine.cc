#include "statemachine.h"

void ItemCollection::collectionInit()
{
    for(ItemType i = KEY_1; i < KEY_END; i = (ItemType)(i++))
    {
        items.insert(i);
    }
}

void bindProperties()
{
    ItemProperties prop1 = {
        .count = 0;
        .func1 = testfunc;
        .func2 = testfunc;
        .func3 = testfunc;
    };
    lut[KEY_1] = prop1;
    ItemProperties prop2 = {
        .count = 0;
        .func1 = testfunc;
        .func2 = testfunc;
        .func3 = testfunc;
    };
    lut[KEY_2] = prop2;
    ItemProperties prop3 = {
        .count = 0;
        .func1 = testfunc;
        .func2 = testfunc;
        .func3 = testfunc;
    };
    lut[KEY_3] = prop3;
}

void testfunc()
{
    printf("testfunc");
}
void callfunc(ItemType cmd)
{
    for (vector<ItemType>::iterator iter = ic.items.begin(); iter != ic.items.end(); iter++)
    {
        if (*iter == cmd)
        {
            
        }
    }
}