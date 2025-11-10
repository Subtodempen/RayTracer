## A Minimal Ray Tracing 
Using diffuse lambertian shading on spheres and triangles, written in C and using ppm imaging to display the shapes. In Furure could add phong shading and other materials. Uses an abstractable world struct to make extending to different shapes easier. 

## Why C and not CPP
Although most of my previous projects are in C++ and I am definitely more comfortable in it, I chose to write this raytracer in C. Even though many tutorials and other projects had chosen to do it in C++, due to the ease of abstracting light rays into classes, the use of polymorphism in the vector functionality, and because of the move away from lower memory management and use of smart pointers would mean that I could focus on the maths rather than low-level design.

Although, I did end up choosing C for this project due to the simplicity of its functional approach. Most of the projects before this lacked substance, and I would focus too much on abstracting my code with classes that I would end up not writing that much actual logic code.

So, in future C++ and C projects I have learnt to actually plan the structure and logic of my code before grouping them into objects. It is much easier to see what code needs to go where from a higher level. For example, it is abundantly clear that the vector functionality should be in its own class with operator overloading, but this only becomes so obvious once all the code has been written.
