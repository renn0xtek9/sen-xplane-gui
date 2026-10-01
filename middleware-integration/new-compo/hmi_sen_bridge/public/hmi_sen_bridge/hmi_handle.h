#include <sen/core/meta/var.h>
#include <sen/kernel/component_api.h>
#include <stl/hmi_sen_bridge/hmi_handle.stl.h>
// sen
#include "sen/core/base/compiler_macros.h"

namespace hmi_handle {
class SEN_EXPORT HmiHandleImpl : public hmi_sen_bridge::HmiHandleBase {

SEN_NOCOPY_NOMOVE(HmiHandleImpl)

public:
 HmiHandleImpl(std::string name,const sen::VarMap& args):hmi_sen_bridge::HmiHandleBase(name)
 {
    std::ignore=args;
    std::cout <<"Handle created"<<std::endl;
 }
 ~HmiHandleImpl()=default;
 protected:

void registered(sen::kernel::RegistrationApi& api) override;
void unregistered(sen::kernel::RegistrationApi& api) override;

void minimizeImpl() override;
void fullScreenImpl() override;
};

}
