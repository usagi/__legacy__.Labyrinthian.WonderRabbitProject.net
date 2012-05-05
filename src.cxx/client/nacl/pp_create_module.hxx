#include "ppapi/cpp/module.h"

namespace pp {
  Module* CreateModule()
  { return new wrp::Labyrinthian::client::nacl::module(); }
}

