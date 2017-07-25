python -m cogapp -d -e -ofsm.h -DHandlers=16 fsm_generator.h 
python -m cogapp -d -e -omessage_router.h -DHandlers=16 message_router_generator.h 
python -m cogapp -d -e -olargest.h -DNTypes=16 largest_generator.h
python -m cogapp -d -e -osmallest.h -DNTypes=16 smallest_generator.h
python -m cogapp -d -e -otype_traits.h -DIsOneOf=16 type_traits_generator.h