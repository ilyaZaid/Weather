#ifndef IBUTTONCLICK
#define IBUTTONCLICK

class iButtonClick
{
public:

    virtual ~iButtonClick() {}
    virtual void buttonBackClick() = 0;
    virtual void buttonNextClick() = 0;
};
#endif // IBUTTONCLICK

