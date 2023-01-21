#ifndef __SRC_NONCOPYABLE_H__
#define __SRC_NONCOPYABLE_H__

namespace jujimeizuo {

/**
 * @brief 对象无法拷贝赋值
*/
class Noncopyable {

    /**
     * @brief 默认构造函数
    */
    Noncopyable() = default;
    /**
     * @brief 默认析构函数
    */
    ~Noncopyable() = default;
    /**
     * @brief 拷贝构造函数（禁用）
    */
    Noncopyable(const Noncopyable&) = delete;
    /**
     * @brief 赋值函数（禁用）
    */
    Noncopyable& operator=(const Noncopyable&) = delete;
};

}

#endif