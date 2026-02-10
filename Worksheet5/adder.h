#ifndef MATHSLIBADDERH
#define MATHSLIBADDERH

/** @file
 * This file contains the declarations of all exported functions in the maths library.
 */



#if defined(_WIN32)
  #ifdef mathsSTATIC
    #define MATHSLIBAPI
  #else
    #ifdef mathsEXPORTS
      #define MATHSLIBAPI __declspec(dllexport)
    #else
      #define MATHSLIBAPI __declspec(dllimport)
    #endif
  #endif
#else
  #define MATHSLIBAPI
#endif

/** Adding function
 * This function adds two numbers.
 * @param a is the first number
 * @param b is the second number
 * @return sum of a and b
 */
MATHSLIBAPI int add(int a, int b);

#endif
