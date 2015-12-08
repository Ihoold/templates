#ifndef PIZZA_H
#define PIZZA_H
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <array>

//HELPERS

// Returns -1 if LookingFor is not found
template <typename LookingFor>
constexpr int NumberOfType(int pos)
{
  return -1;
}

template <typename LookingFor, typename Head, typename... Tail>
constexpr int NumberOfType(int pos)
{
  return (std::is_same<LookingFor,Head>::value) ? pos : NumberOfType<LookingFor,Tail...>(pos+1);
}

/*
template<bool B, size_t t, size_t f>
struct transfer() {static constexpr size_t impl() {return t;}};
 
template<size_t t, size_t f>
struct transfer<false, t, f>() {static constexpr size_t impl() {return t;}};

template<bool B, size_t t, size_t f>
constexpr size_t transform(){return transfer<B,t,f>::impl();}
*/


template <typename... List>
struct IsInjective;
//IsInjective<List...>::value is true iff there are no two same types in List

template <typename Head, typename... Tail>
struct IsInjective<Head, Tail...> {
  enum { value = ((NumberOfType<Head, Tail...>(0) !=-1) && IsInjective<Tail...>::value) };
	};

template <typename T>
struct IsInjective <T> {
	enum { value = true };
};

//\HELPERS

template<typename... Kinds> struct Pizzeria {
  //static_assert(IsInjective<Kinds...>::value,
  //		"No need to mention two pizza types twice!");
  template<size_t... slices> struct Pizza {
  public:
    static constexpr std::array<size_t, sizeof... (Kinds)> as_array() {
      const std::array<size_t, sizeof... (slices)> arr_of_types = {{slices...}};
      return arr_of_types;
    }

    template<typename PKind> static constexpr size_t count() {
      constexpr std::array<size_t, sizeof... (slices)> arr_of_types = {{slices...}};
      return arr_of_types[NumberOfType<PKind,Kinds...>(0)];
    }

    typedef Pizza<(2*slices)...> sliced_type;
  };

  template<typename Kind> struct make_pizza {
    static_assert(NumberOfType<Kind, Kinds...>(0) != -1);
  public:
    //  typedef Pizza<(transform<std::is_same<Kind,Kinds>::value,8,0>)...> type;
    typedef Pizza<8,0> type;
  };
};

#endif
