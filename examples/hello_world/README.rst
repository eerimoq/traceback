Build and run
=============

.. code-block:: text

   $ make
   gcc -I ../../include main.c ../../src/traceback.c -O0 -g -o main
   ./main
   Traceback (most recent call last):
   _start at ??:?
   ?? ??:0
   main at /home/erik/workspace/traceback/examples/hello_world/main.c:50
   fie at /home/erik/workspace/traceback/examples/hello_world/main.c:44
   bar at /home/erik/workspace/traceback/examples/hello_world/main.c:39
   foo at /home/erik/workspace/traceback/examples/hello_world/main.c:34
   traceback_print at /home/erik/workspace/traceback/examples/hello_world/../../src/traceback.c:47
