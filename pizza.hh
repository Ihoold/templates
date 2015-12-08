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

template <typename... List>
struct IsInjective;
//IsInjective<List...>::value is true iff there are no two same types in List

template <typename Head, typename... Tail>
struct IsInjective<Head, Tail...> {
  enum { value = ((NumberOfType<Head, Tail...>(0) ==-1) && IsInjective<Tail...>::value) };
	};

template <typename T>
struct IsInjective <T> {
	enum { value = true };
};

// Returns number of pieces to take
// takes: max and curr are part of recursive call
// amount is maximum of pieces we can take of given type
template <typename Kind>
constexpr int most_yummy (int max, size_t curr, size_t amount)
{
  return (curr == amount+1) ? max :
    ((Kind::yumminess(curr) >= max) ? 
     most_yummy<Kind>(Kind::yumminess(curr), curr+1,amount):
     most_yummy<Kind>(max, curr+1, amount));
}


//\HELPERS
template<typename Pizza1, typename Pizza2> 
struct best_mix
{
  static_assert(std::is_same<typename Pizza1::type, typename Pizza2::type>::value,
		"We can't mix pizzas from diffrent pizzerias!");
  //  typedef Pizza<Pizza1::type,> type;
};

template<typename... Kinds> struct Pizzeria {
  static_assert(IsInjective<Kinds...>::value,
  		"No need to mention one pizza type twice!");
  template<typename Pizza_type, size_t... slices> 
  struct Pizza {
  public:
    static constexpr std::array<size_t, sizeof... (Kinds)> as_array() {
      const std::array<size_t, sizeof... (slices)> arr_of_types = {{slices...}};
      return arr_of_types;
    }

    template<typename PKind> static constexpr size_t count() {
      constexpr std::array<size_t, sizeof... (slices)> arr_of_types = {{slices...}};
      return arr_of_types[NumberOfType<PKind,Kinds...>(0)];
    }
  private:
    typedef Pizzeria<Kinds...> type;
  public:
    typedef Pizza<type,(2*slices)...> sliced_type;

  };

  template<typename Kind> struct make_pizza {
    static_assert(NumberOfType<Kind, Kinds...>(0) != -1,
		  "We don't have this one in menu!");
  public:
    typedef Pizza<Pizzeria, (std::conditional<
		   std::is_same<Kinds,Kind>::value,
		   std::integral_constant<size_t,8>,
		   std::integral_constant<size_t,0> 
		   >::type::value) ...> type;
  };
};

#endif
