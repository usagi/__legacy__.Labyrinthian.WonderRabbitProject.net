#include "ppapi/cpp/module.h"

namespace wrp {
namespace Labyrinthian {
namespace client {
namespace nacl {
  
  struct module: public pp::Module{
    module (): pp::Module(){}
    virtual ~module(){}
    virtual pp::Instance* CreateInstance(PP_Instance a)
    { return new instance(a); }
  };

}}}}

