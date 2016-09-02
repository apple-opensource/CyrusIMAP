/* convert_code.c: Convert IMAP_* error to exitcodes.h exit status
 * Copyright (c) 1998-2003 Carnegie Mellon University.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The name "Carnegie Mellon University" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For permission or any other legal
 *    details, please contact  
 *      Office of Technology Transfer
 *      Carnegie Mellon University
 *      5000 Forbes Avenue
 *      Pittsburgh, PA  15213-3890
 *      (412) 268-4387, fax: (412) 268-7395
 *      tech-transfer@andrew.cmu.edu
 *
 * 4. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by Computing Services
 *     at Carnegie Mellon University (http://www.cmu.edu/computing/)."
 *
 * CARNEGIE MELLON UNIVERSITY DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY BE LIABLE
 * FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *
 * $Id: convert_code.c,v 1.5 2005/03/05 00:36:45 dasenbro Exp $ */

/* Same old convert_code function moved into one place instead of being
 * redundant in every file.
 * tjs 23-jul-1998
 */

#include <config.h>

#include "imap_err.h"
#include "exitcodes.h"

int convert_code(int r)
{
    switch (r) {
    case 0:
        return 0;
        
    case IMAP_IOERROR:
        return EC_IOERR;

    case IMAP_PERMISSION_DENIED:
        return EC_NOPERM;

    case IMAP_QUOTA_EXCEEDED:
        return EC_TEMPFAIL;

    case IMAP_MAILBOX_NOTSUPPORTED:
        return EC_DATAERR;

    case IMAP_MAILBOX_NONEXISTENT:
        return EC_UNAVAILABLE;
    }
        
    /* Some error we're not expecting. */
    return EC_SOFTWARE;
}       
