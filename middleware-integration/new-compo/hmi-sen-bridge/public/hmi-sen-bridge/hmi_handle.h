#include <sen/core/meta/var.h>
#include <stl/hmi_sen_bridge/config.stl.h>
// sen
#include "sen/core/base/compiler_macros.h"

namespace hmi_sen_bridge{
class SEN_EXPORT HmiHandleImpl : public HmiHandleBase {

    SEN_NOCOPY_NOMOVE(HmiHandleImpl)

public:
 HmiHandleImpl(const std::string name):HmiHandleBase(name)
 {
    std::cout <<"Handle created"<<std::endl;
 }
  void minimizeImpl();
  void fullScreenImpl();
};

// SEN_EXPORT_CLASS(HmiHandleImpl)
// SEN_EXPORT(HmiHandleImpl)
}
