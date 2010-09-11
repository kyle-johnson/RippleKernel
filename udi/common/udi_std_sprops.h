
/*
 * File: env/common/udi_std_sprops.h
 *
 * UDI Standard Static Properties definitions.
 *
 * The std_sprops may be used by environments which obtain the static
 * properties as a set of strings and wish to parse and represent
 * those strings internally in a generic manner.
 */

/*
 * $Copyright udi_reference:
 * 
 * 
 *    Copyright (c) 1995-2001; Compaq Computer Corporation; Hewlett-Packard
 *    Company; Interphase Corporation; The Santa Cruz Operation, Inc;
 *    Software Technologies Group, Inc; and Sun Microsystems, Inc
 *    (collectively, the "Copyright Holders").  All rights reserved.
 * 
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the conditions are met:
 * 
 *            Redistributions of source code must retain the above
 *            copyright notice, this list of conditions and the following
 *            disclaimer.
 * 
 *            Redistributions in binary form must reproduce the above
 *            copyright notice, this list of conditions and the following
 *            disclaimers in the documentation and/or other materials
 *            provided with the distribution.
 * 
 *            Neither the name of Project UDI nor the names of its
 *            contributors may be used to endorse or promote products
 *            derived from this software without specific prior written
 *            permission.
 * 
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS," AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDERS OR ANY CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *    TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 *    USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 *    DAMAGE.
 * 
 *    THIS SOFTWARE IS BASED ON SOURCE CODE PROVIDED AS A SAMPLE REFERENCE
 *    IMPLEMENTATION FOR VERSION 1.01 OF THE UDI CORE SPECIFICATION AND/OR
 *    RELATED UDI SPECIFICATIONS. USE OF THIS SOFTWARE DOES NOT IN AND OF
 *    ITSELF CONSTITUTE CONFORMANCE WITH THIS OR ANY OTHER VERSION OF ANY
 *    UDI SPECIFICATION.
 * 
 * 
 * $
 */

#ifndef _UDI_STD_SPROPS_H
#define _UDI_STD_SPROPS_H

typedef struct _udi_std_mesgfile {
	struct _udi_std_mesgfile *nextmsgfile;	/* List of driver's message files */
	char mesgfile_name[513];	/* Name of this message file */
	char *mesg_text;		/* pointer to text of message file */
	udi_size_t mesg_len;		/* # bytes @ mesg_text */
} _udi_std_mesgfile_t;

_udi_std_mesgfile_t *_OSDEP_GET_STD_MSGFILE(struct _udi_driver *drv,
					    char *msgfile_name,
					    udi_ubit32_t num);

typedef struct _usp_line {
	udi_queue_t ul_link;		/* list ptrs for rapid access. */
	char *ul_line;			/* full text line in sprops. */
	char *ul_args;			/* first token into line. */
} _usp_line_t;

typedef struct _udi_std_sprops {
	struct _udi_std_sprops *nextprops;	/* link for these structs */
	struct _udi_std_sprops *ancillary;	/* link for related (e.g. msgfile) */
	char *props_text;		/* pointer to properties text memory */
	udi_size_t props_len;		/* # bytes pointed to by props_text */
	udi_boolean_t is_meta;		/* True if this is a meta library. */
	_udi_std_mesgfile_t *props_msgs;	/* List of message files */
	udi_size_t line_cnt;			/* Number of lines present. */
	_usp_line_t *usp_lines;		/* ptr to array of lines. */
	udi_size_t device_ln_count;
	udi_queue_t devline_list;
	udi_queue_t default_list;
} _udi_std_sprops_t;

const char * _udi_get_shortname_from_sprops(struct _udi_std_sprops *sprops);
const char * _udi_get_descr_from_sprops(struct _udi_std_sprops *sprops);
udi_boolean_t _udi_module_is_meta(struct _udi_std_sprops*, char **provides);


/* _udi_std_sprops_parse
 *
 * This routine is called to parse a group of property strings and
 * return one or more standard properties structures to describe the
 * parsed properties.  The propstrings pointer points to a group of
 * concatentated null-terminated strings whose total length (including
 * terminators) is specified by bytelength.  This routine returns one
 * more more newly allocated _udi_std_sprops_t structures to represent
 * those properties.
 * The drvname argument may be used to filter the properties returned to
 * only be those for the specified driver; otherwise drvname may be NULL
 * and a list of props structures will be returned.
 */

_udi_std_sprops_t *_udi_std_sprops_parse(char *propstrings,
					int bytelength,
					const char *drvname);

/* _udi_std_sprops_free
 *
 * This routine is called to deallocate the property structures that were
 * allocated and returned by the _udi_std_sprops_parse routine above.
 */

void _udi_std_sprops_free(_udi_std_sprops_t *propslist);

#endif /* _UDI_STD_SPROPS_H */
