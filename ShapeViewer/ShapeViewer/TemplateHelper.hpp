#include <type_traits>

namespace ShapeViewer
{
template <typename T, typename U>
bool Equal(const T& l, const U& r)
    requires(std::is_base_of_v<U, T>)
{
    auto obj = dynamic_cast<const T*>(&r);
    if (!obj)
    {
        return false;
    }

    if (&l == &r)
    {
        return true;
    }

    return l == *obj;
}

template <typename T, typename U>
bool Copy(T& target, const U& obj)
    requires(std::is_base_of_v<U, T>)
{
    auto object = dynamic_cast<const T*>(&obj);
    if (!object)
    {
        return false;
    }

    if (&target == object)
    {
        return true;
    }

    target = *object;

    return true;
}

template <typename T> T* Clone(const T& obj)
{
    return new T(obj);
}
} // namespace ShapeViewer
