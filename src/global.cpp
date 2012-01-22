/*
 * Copyright © 2012 Canonical Ltd.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of
 * Novell, Inc. not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior permission.
 * Novell, Inc. makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 * NOVELL, INC. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL NOVELL, INC. BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Thomas Voss <thomas.voss@canonical.com>
 */

#include <list>

#include "core/string.h"

class CompWindow;

char *programName;
char **programArgv;
int  programArgc;

char *backgroundImage = NULL;

bool shutDown = false;
bool restartSignal = false;

CompWindow *lastFoundWindow = 0;

bool replaceCurrentWm = false;
bool indirectRendering = false;
bool noDetection = false;
bool useDesktopHints = false;
bool debugOutput = false;
bool useCow = true;

std::list <CompString> initialPlugins;

unsigned int pluginClassHandlerIndex = 0;