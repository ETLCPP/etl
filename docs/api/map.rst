Map and Multimap
==================

Map Container
--------------

A fixed capacity map.
Uses ``etl::less`` as the default key comparison method.
STL equivalent: :cpp:class:`std::map` / :cpp:class:`std::multimap`.

:cpp:class:`etl::map` inherits from :cpp:class:`etl::imap`.
:cpp:class:`etl::imap` may be used as a size independent pointer or reference type
for any :cpp:class:`etl::map` instance.

Examples
~~~~~~~~~~

Defines data as a map of ``int/int`` pairsm has a length of 4.

.. code:: cpp
	
	#include <etl/map.h>

	etl::map data{ etl::pair{0, 1}, etl::pair{2, 3}, etl::pair{4, 5}, etl::pair{6, 7} };

API
~~~~

.. doxygenclass:: etl::map
	:members:

.. doxygenclass:: etl::imap
	:members:

Multimap Container
-------------------

.. doxygenclass:: etl::multimap
	:members:

.. doxygenclass:: etl::imultimap
	:members:
