#include <string>

#include <boost/lexical_cast.hpp>

#include "ppapi/cpp/instance.h"
//#include "ppapi/gles2/gl2ext_ppapi.h"
//#include "ppapi/cpp/rect.h"
//#include "ppapi/cpp/size.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/input_event.h"

#define WRP_PRODUCT_NAME "Labyrinthian"
#include <wrp/log.hxx>

namespace wrp {
namespace Labyrinthian {
namespace client {
namespace nacl {

  class instance
    : public pp::Instance
  {
  public:
    explicit instance(PP_Instance a): pp::Instance(a){
      log log_("instance", this);
      RequestInputEvents(
        PP_INPUTEVENT_CLASS_MOUSE |
        PP_INPUTEVENT_CLASS_WHEEL
      );
      RequestFilteringInputEvents(
        PP_INPUTEVENT_CLASS_KEYBOARD
      );
    }
    virtual ~instance(){}
    virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]){
      log l("Init", this);
      for(decltype(argc) n = 0; n < argc; ++n){
        l << "attribute#" << boost::lexical_cast<std::string>(n) << "\n"
          << "name: " << argn[n] <<"\n"
          << "value: " << argv[n] 
          ;
      }
      return true;
    }
    virtual void HandleMessage(const pp::Var& a){
      log l("HandleMessage", this);
      if(!a.is_string())
        return;
      auto v = a.AsString();
      l << "v: " << v;
    }
    virtual bool HandleInputEvent(const pp::InputEvent& event){
      log l("HandleInputEvent", this);
      l << "type: " << boost::lexical_cast<std::string>(event.GetType()) + "\n"
        << "time: " << boost::lexical_cast<std::string>(event.GetTimeStamp())
        ;
      return true;
    }
    virtual void DidChangeView(const pp::View& view){
      log l("DidChangeView(const pp::View&)", this);
      // ToDo: resize view
      pp::Instance::DidChangeView(view);
    }
    virtual void DidChangeView(const pp::Rect& position, const pp::Rect& clip){
      log l("DidChangeView(const pp::Rect&, const pp::Rect&)", this);
      // ToDo: resize view
    }
    virtual void DidChangeFocus(bool focus){
      log l("DidChangeFocus", this);
      l << "focus: " << focus;
      // ToDo: 
    }
  };

}}}}

