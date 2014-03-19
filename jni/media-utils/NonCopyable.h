#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

#include <utility>

namespace laiwang {
namespace Utils {



struct NonCopyable {
    NonCopyable() = default;

    NonCopyable(NonCopyable const&) = delete;
    NonCopyable(NonCopyable&&)      = default;

    auto operator=(NonCopyable const&)  -> NonCopyable& = delete;
    auto operator=(NonCopyable&&)       -> NonCopyable& = default;
};

template<typename T>
class PointerWrapper {
protected:
    T*      _Value;

public:
    PointerWrapper(T* value = nullptr)
        : _Value(value) {}
    PointerWrapper(PointerWrapper&& src)
        : _Value(src._Value)
    {
        src._Value = nullptr;
    }

public:
    auto get()          -> T*&              { return _Value; }
    void set(T* value)                      { _Value = value; }
    auto operator ->()  -> T*               { return _Value; }
    auto operator ->() const -> T const*    { return _Value; }

    auto operator =(PointerWrapper&& src) -> PointerWrapper&
    {
        std::swap(src._Value, _Value);
        return *this;
    }

    auto operator ==(T* value) const -> bool    { return _Value == value; }
    auto operator !=(T* value) const -> bool    { return _Value != value; }


    operator T*()               { return _Value; }
};

}
} /* namespace duanqu */

#define arraysize(arr) \
    (sizeof(arr)/sizeof(arr[0]))

#endif /* __NONCOPYABLE_H__ */
