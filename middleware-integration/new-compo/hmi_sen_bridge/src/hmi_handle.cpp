#include <hmi_sen_bridge/hmi_handle.h>

namespace hmi_handle {

void HmiHandleImpl::registered(sen::kernel::RegistrationApi& api)
{
    std::ignore=api;
    std::cout<<"Handle registered "<<std::endl;
}

void HmiHandleImpl::unregistered(sen::kernel::RegistrationApi& api)
{
    std::ignore=api;
    std::cout<<"Handle unregistered "<<std::endl;
}

void HmiHandleImpl::minimizeImpl(){

}

void HmiHandleImpl::fullScreenImpl(){
    
}

SEN_EXPORT_CLASS(HmiHandleImpl)

}
