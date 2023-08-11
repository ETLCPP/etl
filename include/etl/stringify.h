#ifndef ETL_ETL_STRINGIFY_INCLUDED
#define ETL_ETL_STRINGIFY_INCLUDED

#ifndef STRINGIFY
/** Helper macro for STRINGIFY. */
#define STRINGIFY_1(...) #__VA_ARGS__

/**
 * Indirect stringification.
 *
 * Doing two levels allows the parameter to be a macro itself.
 * For example, compile with `-DFOO=bar`, `STRINGIFY(FOO)` converts to "bar".
 *
 * \param x will be converted to one string, including the spaces and commas between arguments.
 *             For example `STRINGIFY( a , b )` will be expanded to "a , b".
 */
#define STRINGIFY(...) STRINGIFY_1(__VA_ARGS__)
#endif /* STRINGIFY */

#endif /* ETL_ETL_STRINGIFY_INCLUDED */
