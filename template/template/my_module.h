#ifndef __SRC_MY_MODULE_H__
#define __SRC_MY_MODULE_H__

#include "src/jujimeizuo.h"

namespace name_space {

class MyModule {
public:
    typedef std::shared_ptr<MyModule> ptr;
    MyModule(int argc, char** argv);

    bool run();
};

}

#endif