#include <iostream>
#include <string>
#include <algorithm>

#include <boost/lexical_cast.hpp>

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
//#include "ppapi/gles2/gl2ext_ppapi.h"

//#include "ppapi/cpp/rect.h"
//#include "ppapi/cpp/size.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/input_event.h"

namespace wrp { 
  struct Labyrinthian_instance
    : public pp::Instance
  {
    explicit Labyrinthian_instance(PP_Instance a): pp::Instance(a){
      RequestInputEvents(
        PP_INPUTEVENT_CLASS_MOUSE |
        PP_INPUTEVENT_CLASS_WHEEL
      );
      RequestFilteringInputEvents(
        PP_INPUTEVENT_CLASS_KEYBOARD
      );
    }
    virtual ~Labyrinthian_instance(){}
    virtual void HandleMessage(const pp::Var& a){
      if(!a.is_string())
        return;
      auto v = a.AsString();
      std::reverse(v.begin(),v.end());
      std::cerr<<v<<"\n";
      PostMessage(pp::Var(v));
    }
    virtual bool HandleInputEvent(const pp::InputEvent& event){
      PostMessage(pp::Var("HandleInputEvent"));
      std::cerr << "HandleInputEvent\n";
      auto m = std::string(
          "Test: input / handle input event\n"
          "  type: "
          );
      m += boost::lexical_cast<std::string>(event.GetType()) + "\n"
        "  time: " + boost::lexical_cast<std::string>(event.GetTimeStamp());
      PostMessage(m);
      return pp::Instance::HandleInputEvent(event);
    }
    virtual void DidChangeView(const pp::View& view){
      PostMessage("ToDo: resize view / did change view(v)");
      pp::Instance::DidChangeView(view);
    }
    virtual void DidChangeView(const pp::Rect& position, const pp::Rect& clip){
      PostMessage("ToDo: resize view / did change view(position, clip)");
    }
    virtual void DidChangeFocus(bool focus){
      PostMessage("ToDo: auto pause / did change focus(focus)");
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

