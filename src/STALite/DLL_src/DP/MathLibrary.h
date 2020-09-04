#pragma once

# define MATHLIBRARY_API __declspec(dllexport)

extern "C" MATHLIBRARY_API double add_c(int a, int b);
extern "C" MATHLIBRARY_API double network_assignment(int a, int b);

