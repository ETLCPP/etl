---
title: "Generators"
---

Some templates within the ETL are designed to support a variable number of template parameters. To accommodate this, these template classes have a number of specialisations to handle each variant. The ETL sets this number to a reasonable default, but there will almost certainly be circumstances where these are too small for the project at hand.

To this end, the ETL supplies 'generators' to enable the creation of customised variants of these template classes.

The code generation is handled by a Python library called COG. This allows Python code to be embedded within the C++. When COG is run on the generator header, it will produce a customised version of that header.

The naming convention used is
The generator file: xxxx_generator.h
The output file: xxxx.h
The script: generate_xxxx.bat

The script generate.bat will run all of the generate scripts.

e.g.
Running the COG script on the generator type_traits_generator.h will create  type_traits.h

The syntax for processing a generator file is
python -m cogapp -d -e -o<output> -D<parameter>=<N> <generator>

e.g.
python -m cogapp -d -e -otype_traits.h -DIsOneOf=16 type_traits_generator.h
The line above will create type_traits.h with the ability to handle up to 16 types in the etl::is_one_of structure.

All of the generator files may be processed by running the script generate.bat
There are generator scripts for each generator header.
This can be run as a command line batch file under Windows or as a script under Linux (if set to 'executable')

Defined generators

Functionality	Generator	Output	Parameters	Notes
Finite State Machine	fsm_generator.h	fsm.h	Handlers	Defines the maximum number of events that can be handled
Message Router	message_router_generator.h	message_router.h	Handlers	Defines the maximum number of messages that can be handled
Smallest	smallest_generator.h	smallest.h	NTypes	Defines the maximum number of types that can be declared
Largest	largest_generator.h	largest.h
NTypes	Defines the maximum number of types that can be declared
Type Traits	type_traits_generator.h	type_traits.h	IsOneOf	Defines the maximum number of types that can be declared in the class etl::is_one_of
Type Lookup	type_lookup_generator.h	type_lookup.h	NTypes	Defines the maximum number of types that can be declared in the classes etl::type_id_lookup and etl::type_type_lookup
Type Select	type_select_generator.h	type_select.h		Defines the maximum number of types that can be declared in the class etl::type_select
Variant Pool	variant_pool_generator.h	variant_pool.h	NTypes	Defines the maximum number of types that can be declared


