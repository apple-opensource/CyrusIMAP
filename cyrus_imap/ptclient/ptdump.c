/*
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
 */
#include <config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "auth_pts.h"
#include "cyrusdb.h"
#include "global.h"
#include "libconfig.h"

static char rcsid[] = "$Id: ptdump.c,v 1.13 2006/11/30 17:11:24 murch Exp $";

int config_need_data = 0;

static int dump_cb(void *rockp __attribute__((unused)),
		     const char *key, int keylen,
		     const char *data,
		     int datalen __attribute__((unused))) 
{
    struct auth_state *authstate = (struct auth_state *)data;
    int i;
    
    printf("user: ");
    fwrite(key, keylen, 1, stdout);
    printf(" time: %d groups: %d\n",
	   (unsigned)authstate->mark, (unsigned)authstate->ngroups);

    for (i=0; i < authstate->ngroups; i++)
	printf("  %s\n",authstate->groups[i].id);
    
    return 0;
}

int main(int argc, char *argv[])
{
    struct db *ptdb;
    char fnamebuf[1024];
    extern char *optarg;
    int opt;
    int r;
    char *alt_config = NULL;

    while ((opt = getopt(argc, argv, "C:")) != EOF) {
	switch (opt) {
	case 'C': /* alt config file */
	    alt_config = optarg;
	    break;
	default:
	    fprintf(stderr,"usage: [-C filename]"
		    "\n\t-C <filename>\tAlternate Config File"
		    "\n");
	    exit(-1);
	    break;
	    /* just pass through */
	}
    }

    cyrus_init(alt_config, "ptdump", 0);

    /* open database */
    strcpy(fnamebuf, config_dir);
    strcat(fnamebuf, PTS_DBFIL);
    r = config_ptscache_db->open(fnamebuf, CYRUSDB_CREATE, &ptdb);
    if(r != CYRUSDB_OK) {
	fprintf(stderr,"error opening %s (%s)", fnamebuf,
	       cyrusdb_strerror(r));
	exit(1);
    }

    /* iterate through db, wiping expired entries */
    config_ptscache_db->foreach(ptdb, "", 0, NULL, dump_cb, ptdb, NULL);

    config_ptscache_db->close(ptdb);

    cyrus_done();

    return 0;
}
