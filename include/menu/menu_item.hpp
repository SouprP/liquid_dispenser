#ifndef MENU_ITEM_HPP_
#define MENU_ITEM_HPP_

enum MenuType {
    MENU_TYPE_DEFAULT,
    MENU_TYPE_VALUE,
    MENU_TYPE_NUMBER_INPUT,
    MENU_TYPE_CALLBACK,
};

class MenuItem{
    private:
        const char* display_name;
        
};

#endif
