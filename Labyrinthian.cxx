#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
//#include "ppapi/gles2/gl2ext_ppapi.h"

//#include "ppapi/cpp/rect.h"
//#include "ppapi/cpp/size.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/input_event.h"

#define WRP_PRODUCT_NAME "Labyrinthian"

namespace wrp { 

#if !defined(WRP_PRODUCT_NAME)
  #define WRP_PRODUCT_NAME "UNKNOWN PRODUCT"
#endif

  class log{
  private:
    decltype(std::chrono::system_clock::now()) start;
    std::string object_name;
    void* object_address;
    static int nesting_counter;
  public:
    typedef std::chrono::duration<double> unit;
    std::stringstream buffer;
    explicit log(std::string object_name_ = "", void* object_address_ = 0)
      : start(std::chrono::system_clock::now())
      , object_name(object_name_)
      , object_address(object_address_)
    {
      ++nesting_counter;
    }
    ~log(){
      using std::chrono::duration_cast;
      auto end = std::chrono::system_clock::now();
      std::string indent;
      for(auto n = nesting_counter; n; --n)
        indent += "  ";
      std::cout
        << indent << "[" WRP_PRODUCT_NAME "] "
                  << object_name << " " << object_address << "\n"
        << indent << "start: "
                  << duration_cast<unit>(start.time_since_epoch()).count() << "\n"
        << indent << "end  : "
                  << duration_cast<unit>(end.time_since_epoch()).count() << "\n"
        << indent << "dt   : "
                  << duration_cast<unit>((end - start)).count() << "\n"
        ;
      std::string b;
      while(std::getline(buffer, b))
        std::cout << indent << b << "\n";
      std::cout << std::flush;
      --nesting_counter;
    }
    auto operator<<(const bool v) -> decltype(buffer<<v) { return buffer << v; }
    auto operator<<(const char* v) -> decltype(buffer<<v) { return buffer << v; }
    auto operator<<(std::string& v) -> decltype(buffer<<v) { return buffer << v; }
    auto operator<<(int& v) -> decltype(buffer<<v){ return buffer << v; }
  };
  int log::nesting_counter = -1;
  
  class Labyrinthian_instance
    : public pp::Instance
  {
  public:
    explicit Labyrinthian_instance(PP_Instance a): pp::Instance(a){
      log log_("Labyrinthian_instance", this);
      RequestInputEvents(
        PP_INPUTEVENT_CLASS_MOUSE |
        PP_INPUTEVENT_CLASS_WHEEL
      );
      RequestFilteringInputEvents(
        PP_INPUTEVENT_CLASS_KEYBOARD
      );
    }
    virtual ~Labyrinthian_instance(){}
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

