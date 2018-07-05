Implementing support for new boards
===================================

In this tutorial we will implement support for a new, not really existing board.
The board will have two functionalities:

* It will send a heartbeat every :cpp:func:`ThetaCloud::tick()`,
* It will also respond with ``pong`` to every ``ping`` received on specific topic.

.. toctree::
   :maxdepth: 1
   :caption: Tutorial parts:

   newboards-tutorial/01-registering-read-handler.rst
   newboards-tutorial/02-registering-write-handler.rst
