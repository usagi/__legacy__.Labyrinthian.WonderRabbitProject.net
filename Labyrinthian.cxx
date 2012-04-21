#include <string>
#include <algorithm>

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
//#include "ppapi/gles2/gl2ext_ppapi.h"

//#include "ppapi/cpp/rect.h"
//#include "ppapi/cpp/size.h"
#include "ppapi/cpp/var.h"

namespace wrp { 
  struct Labyrinthian_instance
    : public pp::Instance
  {
    explicit Labyrinthian_instance(PP_Instance a): pp::Instance(a){}
    virtual ~Labyrinthian_instance(){}
    virtual void HandleMessage(const pp::Var& a){
      if(!a.is_string())
        return;
      auto v = a.AsString();
      std::reverse(v.begin(),v.end());
      PostMessage(pp::Var(v));
    }
  };
  
  struct Labyrinthian_module
    : public pp::Module
  {
    Labyrinthian_module (): pp::Module(){}
    virtual ~Labyrinthian_module(){}
    virtual pp::Instance* CreateInstance(PP_Instance a)
    { return new wrp::Labyrinthian_instance(a); }
  };
}

namespace pp {
  Module* CreateModule()
  { return new wrp::Labyrinthian_module(); }
}

