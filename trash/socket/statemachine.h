#include <vector>
#include <map>

using namespace std;

typedef enum{
    KEY_1 = 0,
    KEY_2,
    KEY_3,
    KEY_END,
}ItemType;

typedef struct{
    int count;
    
    void(*func1)();
    void(*func2)();
    void(*func3)();
}ItemProperties;

class ItemCollection
{
    private:
        vector<ItemType> items;
        

    public:
        ItemCollection();
        virtual ~ItemCollection();

        void collectionInit();
        
}
static map<ItemType, ItemProperties> lut;
static ItemCollection ic;
void bindProperties();
void testfunc();
void callfunc(ItemType cmd);