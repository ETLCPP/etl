Vector
==================

A fixed capacity vector.
STL equivalent: `std::vector <https://en.cppreference.com/w/cpp/container/vector>`_.

There are two APIs available: :cpp:class:`etl::vector_ext` and :cpp:class:`etl::vector`.
Both inherit from :cpp:class:`etl::ivector`, which may be used as a size independency pointer
or reference type for any :cpp:class:`etl::vector` instance.

Has the ability to be copied by low level functions such as memcpy by use of a :cpp:method:`etl::vector::repair` function.
See the function reference for an example of use.

API
----

.. doxygenclass:: etl::vector_ext
	:members:

.. doxygenclass:: etl::vector
	:members:

.. doxygenclass:: etl::ivector
	:members:
