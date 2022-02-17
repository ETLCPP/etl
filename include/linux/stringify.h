/** \file */
#pragma once

/** Helper macro for __stringify. */
#define __stringify_1(x...) #x

/**
 * Indirect stringification.
 *
 * Doing two levels allows the parameter to be a macro itself.
 * For example, compile with `-DFOO=bar, __stringify(FOO)` converts to "bar".
 *
 * \param x will be converted to one string, including the spaces and commas between arguments.
 *             For example `__stringify( a , b )` will be expanded to "a , b".
 */
#define __stringify(x...) __stringify_1(x)
