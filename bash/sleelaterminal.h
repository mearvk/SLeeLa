#ifndef SLEELATERMINAL_H
#define SLEELATERMINAL_H

/*
 * SleelaTerminal(TM) -- product identity for the SLeeLa terminal.
 *
 * SleelaTerminal(TM) is a SLeeLa product BUILT ON GNU Bash (free software,
 * GPL, (C) Free Software Foundation). This header carries the identity of the
 * SLeeLa-authored PRODUCT LAYER only. It sits ALONGSIDE Bash's own version --
 * it does NOT replace, alter, or reattribute the vendored Bash version string,
 * startup banner, copyright, or AUTHORS, which stay intact per the GPL.
 *
 * Use these constants to brand SleelaTerminal(TM) product surfaces (GUI title,
 * About dialog, installer/package name, our own banner line). The underlying
 * shell engine is still GNU Bash and must be acknowledged as such -- see the
 * SLEELATERMINAL_ENGINE_* macros and bash/NOTICE.
 *
 * This file is a SLeeLa-authored addition; the vendored GNU Bash sources keep
 * their own headers and copyright.
 */

/* --- SleelaTerminal(TM) product identity (our layer) --- */
#define SLEELATERMINAL_NAME        "SleelaTerminal"
#define SLEELATERMINAL_TRADEMARK   "SleelaTerminal(TM)"
#define SLEELATERMINAL_VERSION     "1.0.0"
#define SLEELATERMINAL_VERSION_MAJOR 1
#define SLEELATERMINAL_VERSION_MINOR 0
#define SLEELATERMINAL_VERSION_PATCH 0

/* Package / installer identity for the SLeeLa product. */
#define SLEELATERMINAL_PACKAGE     "sleelaterminal"
#define SLEELATERMINAL_GUI_TITLE   "SleelaTerminal(TM)"

/*
 * The incorporated engine. SleelaTerminal(TM) runs on GNU Bash; these macros
 * exist so any About/banner text credits the engine honestly. The actual Bash
 * version comes from Bash's own version.h / --version output (unchanged).
 */
#define SLEELATERMINAL_ENGINE_NAME    "GNU Bash"
#define SLEELATERMINAL_ENGINE_LICENSE "GNU General Public License (GPL)"
#define SLEELATERMINAL_ENGINE_COPYRIGHT "(C) Free Software Foundation, Inc."

/*
 * A one-line identity string suitable for a banner printed ALONGSIDE Bash's
 * own startup banner (not instead of it).
 */
#define SLEELATERMINAL_IDENTITY \
    SLEELATERMINAL_TRADEMARK " " SLEELATERMINAL_VERSION \
    " -- built on " SLEELATERMINAL_ENGINE_NAME \
    " (" SLEELATERMINAL_ENGINE_LICENSE ", " SLEELATERMINAL_ENGINE_COPYRIGHT ")"

#endif /* SLEELATERMINAL_H */
