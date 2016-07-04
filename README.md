# Thread-Pool-Mandelbrot-Set-Generator
**Thread Pool Mandelbrot Generator**
Joshua Tanner
Media Design School 2016

This program generates the mandelbrot set using a pool of threads.
The user is able to specify the number of threads and the zoom/offset factor in an INI file.
By left clicking, the user can change which thread's backbuffer is "active" and is able to see which parts of the 
image each thread was responsible for. 
The program shows the time taken for calculation and the time improvements are visible as more threads are created.

Areas to improve: 
Limitations in INI Parser; can be no blank spaces at end of INI & negative numbers cannot be used in the INI file
Colours & aspect ratio





