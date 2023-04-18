#ifndef __SRC_LIBRARY_H__
#define __SRC_LIBRARY_H__

#include <memory>
#include "module.h"

namespace jujimeizuo {

class Library {
public:
    static Module::ptr GetModule(const std::string& path);
};

}

#endif