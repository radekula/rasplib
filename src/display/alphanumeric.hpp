#ifndev __RASPLIB_DISPLAY_ALPHANUMERIC_HPP__
#define __RASPLIB_DISPLAY_ALPHANUMERIC_HPP__


namespace rasplib {
namespace display {




class AlphanumericScreen
{
private:
    unsigned short columns;
    unsigned short lines;

public:
    AlphanumericScreen();
    AlphanumericScreen(unsigned short columns, unsigned short lines);
    virtual ~LCDScreen();

public:
    void Init();
    void Mode();

public:
    void Clear();
    void Home();
    void Print();
    void Delete();
    
}




}
}

#endif
