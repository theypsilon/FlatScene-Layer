# FlatScene

**FlatScene** is a high abstraction layer designed to create *2D interactive applications* in a rapid and easy way. This library uses **OpenGL** to render 2D graphics supporting *hardware acceleration*, and **SDL** to manipulate I/O events. 

Actually, the library is ready for use in **C++**. It benefits from the *portability* inherited from **SDL** and **OpenGL**, thus allowing to the user the managing of *multiplattform projects* without so much concerns.

# DemoArk

This is a **test application** created to show the features of **FlatScene**.

In this demo, input keys are :

* **ESC** : Simple option *menu*. You got here an option for leaving the demo.

* **F1** : Swap between *tests*.
* **F2** : *Restart* current test.
* **F3** : Swap *fullscreen mode* - *window mode*.

* **SPACE** : *Pause*.

* **Arrow keys** : *Walking*.

* Into first test, press **u** to stress *change map* test.

* Into double cam test, press **a** to kill the grey enemies.

# Compilation

First, install gcc, cmake, and the dev libraries for opengl, sdl1.2 and boost (maybe boost is not really needed).

Then type on console: 
```bash
./run.sh
```