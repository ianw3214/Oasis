#include "oasis.h"

class Sandbox : public Oasis::IState
{
public:
    virtual void Init() override {}
    virtual void Close() override {}

    virtual void Update()  override;
private:
};