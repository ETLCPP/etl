/** \file */
#pragma once

/** Helper macro for STRINGIFY. */
#define STRINGIFY_1(x...) #x

/**
 * Indirect stringification.
 *
 * Doing two levels allows the parameter to be a macro itself.
 * For example, compile with `-DFOO=bar`, `STRINGIFY(FOO)` converts to "bar".
 *
 * \param x will be converted to one string, including the spaces and commas between arguments.
 *             For example `STRINGIFY( a , b )` will be expanded to "a , b".
 */
#define STRINGIFY(x...) STRINGIFY_1(x)
