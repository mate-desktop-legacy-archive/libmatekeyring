/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/* test-session.c: Test reading a secret

   Copyright (C) 2011 Collabora Ltd.

   The Mate Keyring Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Mate Keyring Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Mate Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
   Boston, MA 02110-1301, USA.

   Author: Stef Walter <stefw@collabora.co.uk>
*/

#include "config.h"

#include "mate-keyring.h"
#include <glib.h>

static void
test_create_find_read_password (void)
{
	MateKeyringResult res;
	MateKeyringAttributeList* attrs;
	MateKeyringFound* f;
	guint id;
	GList *found;
	guint num;

	/* Unique for every run */
	num  = time (NULL);

	attrs = mate_keyring_attribute_list_new ();
	mate_keyring_attribute_list_append_string (attrs, "dog", "woof");
	mate_keyring_attribute_list_append_string (attrs, "bird", "cheep");
	mate_keyring_attribute_list_append_string (attrs, "iguana", "");
	mate_keyring_attribute_list_append_uint32 (attrs, "num", num);

	/* Create the item */
	res = mate_keyring_item_create_sync ("session", MATE_KEYRING_ITEM_GENERIC_SECRET,
	                                      "Barnyard", attrs, "the very secret", TRUE, &id);
	g_assert_cmpint (MATE_KEYRING_RESULT_OK, ==, res);

	/* Now try to find it */
	res = mate_keyring_find_items_sync (MATE_KEYRING_ITEM_GENERIC_SECRET, attrs, &found);
	g_assert_cmpint (MATE_KEYRING_RESULT_OK, ==, res);

	g_assert_cmpint (g_list_length (found), ==, 1);
	f = (MateKeyringFound*)found->data;

	g_assert_cmpstr (f->keyring, ==, "session");
	g_assert_cmpstr (f->secret, ==, "the very secret");

	mate_keyring_found_list_free (found);
}

int
main (int argc, char **argv)
{
	g_test_init (&argc, &argv, NULL);

	if (!mate_keyring_is_available ()) {
		g_printerr ("skipping any-daemon tests, no daemon is running");
		return 0;
	}

	g_test_add_func ("/any-daemon/create-find-ready-password", test_create_find_read_password);
	return g_test_run ();
}
