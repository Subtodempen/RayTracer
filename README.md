## A Minimal Ray Tracing 
Using diffuse lambertian shading on spheres and triangles, written in C and using ppm imaging to display the shapes. In Furure could add phong shading and other materials. Uses an abstractable world struct to make extending to different shapes easier. 

## Why C and not CPP
Although most of my previous projects are in c++ and i am definetly more comptferble in it, I chose to qwrite this raytracer in C. Even though many tutorials and other projects had chosen to do it in c++, due to the easibilty of abstracting light rays into classes, The use of polymorphism in the vector functionailty And because of the move away from lower memory managment and use of smart pointerds would mean that I could focus on the maths rather then low level design. 

Although, I did end up choosing C for this project due to the simplicity of its functional approach. Most of the preojects before this lacked subastance and I would focus too much on abstracting my code with classes that i would end up not writing that much actual logic code.

So in future C++ and C projects i have learnt to actually plan the structure and .ogic of my code before grouping themi nto objects. It is much easier to see what code needs to go where from  a higher level.
For example it is abanduntly clear thatt he vector functionailty should be in its own class with operater overloading but tis only becomes so obvious once all the code has been written.
