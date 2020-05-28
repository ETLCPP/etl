python -m cogapp -d -e -o../fsm.h -DHandlers=16 fsm_generator.h 
python -m cogapp -d -e -o../message_router.h -DHandlers=16 message_router_generator.h 
python -m cogapp -d -e -o../largest.h -DNTypes=16 largest_generator.h
python -m cogapp -d -e -o../smallest.h -DNTypes=16 smallest_generator.h
python -m cogapp -d -e -o../type_traits.h -DIsOneOf=16 type_traits_generator.h
python -m cogapp -d -e -o../type_lookup.h -DNTypes=16 type_lookup_generator.h
python -m cogapp -d -e -o../type_select.h -DNTypes=16 type_select_generator.h
python -m cogapp -d -e -o../message_packet.h -DHandlers=16 message_packet_generator.h 