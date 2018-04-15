#pragma once


#ifndef DXRL_THROW_IF_FAILED

#if defined (DEBUG) || (_DEBUG)
#define DXRL_THROW_IF_FAILED(result) { assert(SUCCEEDED(result)); }
#else
#define DXRL_THROW_IF_FAILED(result) result;
#endif // DEBUG

#endif // DXRL_THROW_IF_FAILED

////////////////////////////////////////

#ifndef DXRL_DEFINE_COPYABLE_MOVABLE

#define DXRL_DEFINE_COPYABLE_MOVABLE(className)                                 \
                                                                                \
    className();                                                                \
    className(className const&);                                                \
    className& operator=(className const&);                                     \
                                                                                \
    className(className&&);                                                     \
    className& operator=(className&&);                                          \
                                                                                \

#endif // DXRL_DEFINE_COPYABLE_MOVABLE_DEFAULT

////////////////////////////////////////

#ifndef DXRL_DEFINE_UNCOPYABLE_MOVABLE_DEFAULT

#define DXRL_DEFINE_UNCOPYABLE_MOVABLE_DEFAULT(className)                       \
                                                                                \
    className() = default;                                                      \
    className(className const&) = delete;                                       \
    className& operator=(className const&) = delete;                            \
                                                                                \
    className(className&&) = default;                                           \
    className& operator=(className&&) = default;                                \
                                                                                \

#endif // DXRL_DEFINE_UNCOPYABLE_MOVABLE_DEFAULT

////////////////////////////////////////

#ifndef DXRL_DEFINE_UNCOPYABLE_MOVABLE

#define DXRL_DEFINE_UNCOPYABLE_MOVABLE(className)                               \
                                                                                \
    className();                                                                \
    className(className const&) = delete;                                       \
    className& operator=(className const&) = delete;                            \
                                                                                \
    className(className&&);                                                     \
    className& operator=(className&&);                                          \


#endif // DXRL_DEFINE_UNCOPYABLE_MOVABLE

////////////////////////////////////////

#ifndef DXRL_DEFINE_COPYABLE_MOVABLE_DEFAULT

#define DXRL_DEFINE_COPYABLE_MOVABLE_DEFAULT(className)                         \
                                                                                \
    className() = default;                                                      \
    className(className const&) = default;                                      \
    className& operator=(className const&) = default;                           \
                                                                                \
    className(className&&) = default;                                           \
    className& operator=(className&&) = default;                                \
                                                                                \

#endif // DXRL_DEFINE_COPYABLE_MOVABLE_DEFAULT

////////////////////////////////////////

