/**
 * Copyright (c) 2026 Parrot Drones SAS
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the copyright holders nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <futils/futils.h>
#include <photo-metadata-defs/pmeta_defs.h>
#include <string.h>

#define ULOG_TAG pmeta_defs
#include <ulog.h>

ULOG_DECLARE_TAG(ULOG_TAG);


/* ================================================================= *
 * EXIF Internal Definitions
 * ================================================================= */

/* codecheck_ignore[COMPLEX_MACRO] */
#define PMETA_EXIF_KEY_ENTRY(_tid, _id, _pref, _prop, _group, _type, _count)   \
	[PMETA_DEFS_EXIF_IDX_##_id] = {                                        \
		.tag_id = _tid,                                                \
		.tag_name = (_pref[0] == '\0') ? _prop : _pref "." _prop,      \
		.ifd_group = _group,                                           \
		.expected_type = PMETA_DEFS_EXIF_TYPE_##_type,                 \
		.default_count = _count},

static const struct pmeta_defs_exif_def s_exif_tag_dict[] = {
	PMETA_DEFS_EXIF_TAG_LIST(PMETA_EXIF_KEY_ENTRY)};
#undef PMETA_EXIF_KEY_ENTRY


static const size_t s_exif_tag_dict_size = SIZEOF_ARRAY(s_exif_tag_dict);


/* ================================================================= *
 * XMP Internal Definitions
 * ================================================================= */

static const struct pmeta_defs_xmp_ns s_xmp_namespace_dict[] = {
#define _(id, prefix_str, uri_str)                                             \
	{prefix_str, sizeof(prefix_str) - 1, uri_str},
	PMETA_DEFS_XMP_NS_LIST(_)
#undef _
};


static const size_t s_xmp_namespace_dict_size =
	SIZEOF_ARRAY(s_xmp_namespace_dict);


/* codecheck_ignore[COMPLEX_MACRO] */
#define PMETA_XMP_KEY_ENTRY(_id, _ns_id, _ns_str, _prop, _type, _count)        \
	[PMETA_DEFS_XMP_IDX_##_id] = {.ns_idx = PMETA_DEFS_XMP_NS_##_ns_id,    \
				      .prop_name = _prop,                      \
				      .full_key = "Xmp." _ns_str "." _prop,    \
				      .type = PMETA_DEFS_XMP_TYPE_##_type,     \
				      .count = _count},

static const struct pmeta_defs_xmp_def s_xmp_key_dict[] = {
	PMETA_DEFS_XMP_KEY_LIST(PMETA_XMP_KEY_ENTRY)};
#undef PMETA_XMP_KEY_ENTRY


static const size_t s_xmp_key_dict_size = SIZEOF_ARRAY(s_xmp_key_dict);


/* ================================================================= *
 * EXIF API
 * ================================================================= */

const struct pmeta_defs_exif_def *pmeta_defs_get_exif_tag(uint16_t tag_id)
{
	for (size_t i = 0; i < s_exif_tag_dict_size; i++) {
		if (s_exif_tag_dict[i].tag_id == tag_id)
			return &s_exif_tag_dict[i];
	}
	return NULL;
}


const struct pmeta_defs_exif_def *
pmeta_defs_get_exif_tag_by_idx(enum pmeta_defs_exif_idx idx)
{
	if (idx >= PMETA_DEFS_EXIF_IDX_COUNT)
		return NULL;
	return &s_exif_tag_dict[idx];
}


const struct pmeta_defs_exif_def *
pmeta_defs_get_exif_tag_by_name(const char *name)
{
	if (name == NULL)
		return NULL;

	for (size_t i = 0; i < s_exif_tag_dict_size; i++) {
		if (strcmp(s_exif_tag_dict[i].tag_name, name) == 0)
			return &s_exif_tag_dict[i];
	}
	return NULL;
}


const char *pmeta_defs_exif_type_to_str(enum pmeta_defs_exif_type type)
{
	switch (type) {
	case PMETA_DEFS_EXIF_TYPE_BYTE:
		return "BYTE";
	case PMETA_DEFS_EXIF_TYPE_ASCII:
		return "ASCII";
	case PMETA_DEFS_EXIF_TYPE_SHORT:
		return "SHORT";
	case PMETA_DEFS_EXIF_TYPE_LONG:
		return "LONG";
	case PMETA_DEFS_EXIF_TYPE_RATIONAL:
		return "RATIONAL";
	case PMETA_DEFS_EXIF_TYPE_UNDEFINED:
		return "UNDEFINED";
	case PMETA_DEFS_EXIF_TYPE_SRATIONAL:
		return "SRATIONAL";
	default:
		return "UNKNOWN";
	}
}


const char *pmeta_defs_ifd_group_to_str(enum pmeta_defs_ifd_group group)
{
	switch (group) {
	case PMETA_DEFS_IFD_0:
		return "IFD0";
	case PMETA_DEFS_IFD_EXIF:
		return "EXIF";
	case PMETA_DEFS_IFD_GPS:
		return "GPS";
	case PMETA_DEFS_IFD_1:
		return "IFD1";
	default:
		return "UNKNOWN";
	}
}


/* ================================================================= *
 * XMP API
 * ================================================================= */

const struct pmeta_defs_xmp_def *pmeta_defs_get_xmp_tag(const char *full_key)
{
	int res;
	enum pmeta_defs_xmp_ns_id ns_idx;
	const char *prop_name;

	res = pmeta_defs_resolve_xmp_key(full_key, &ns_idx, &prop_name);
	if (res < 0)
		return NULL;

	for (size_t i = 0; i < s_xmp_key_dict_size; i++) {
		if (s_xmp_key_dict[i].ns_idx == ns_idx &&
		    strcmp(s_xmp_key_dict[i].prop_name, prop_name) == 0)
			return &s_xmp_key_dict[i];
	}

	return NULL;
}


const struct pmeta_defs_xmp_def *
pmeta_defs_get_xmp_tag_by_idx(enum pmeta_defs_xmp_idx idx)
{
	if (idx >= PMETA_DEFS_XMP_IDX_COUNT)
		return NULL;
	return &s_xmp_key_dict[idx];
}


const struct pmeta_defs_xmp_ns *
pmeta_defs_get_xmp_ns(enum pmeta_defs_xmp_ns_id index)
{
	if (index >= (enum pmeta_defs_xmp_ns_id)s_xmp_namespace_dict_size)
		return NULL;
	return &s_xmp_namespace_dict[index];
}


int pmeta_defs_resolve_xmp_key(const char *full_key,
			       enum pmeta_defs_xmp_ns_id *out_ns_idx,
			       const char **out_prop_name)
{
	const char *cursor = full_key;

	ULOG_ERRNO_RETURN_ERR_IF(full_key == NULL, EINVAL);
	ULOG_ERRNO_RETURN_ERR_IF(out_ns_idx == NULL, EINVAL);
	ULOG_ERRNO_RETURN_ERR_IF(out_prop_name == NULL, EINVAL);

	/* Skip optional "Xmp." prefix */
	if (strncmp(cursor, "Xmp.", 4) == 0)
		cursor += 4;

	for (size_t i = 0; i < s_xmp_namespace_dict_size; i++) {
		size_t len = s_xmp_namespace_dict[i].prefix_len;

		/* Check if prefix matches and is followed by a dot or colon */
		if (strncmp(cursor, s_xmp_namespace_dict[i].prefix, len) == 0 &&
		    (cursor[len] == '.' || cursor[len] == ':')) {
			if (cursor[len + 1] == '\0')
				return -EINVAL;
			*out_ns_idx = (enum pmeta_defs_xmp_ns_id)i;
			*out_prop_name = cursor + len + 1;
			return 0;
		}
	}

	return -ENOENT;
}


const char *pmeta_defs_xmp_type_to_str(enum pmeta_defs_xmp_type type)
{
	switch (type) {
	case PMETA_DEFS_XMP_TYPE_STRING:
		return "STRING";
	case PMETA_DEFS_XMP_TYPE_SEQ:
		return "SEQ";
	default:
		return "UNKNOWN";
	}
}


int pmeta_defs_exif_type_size(enum pmeta_defs_exif_type type)
{
	switch (type) {
	case PMETA_DEFS_EXIF_TYPE_BYTE:
		return 1;
	case PMETA_DEFS_EXIF_TYPE_ASCII:
		return 1;
	case PMETA_DEFS_EXIF_TYPE_SHORT:
		return 2;
	case PMETA_DEFS_EXIF_TYPE_LONG:
		return 4;
	case PMETA_DEFS_EXIF_TYPE_RATIONAL:
		return 8;
	case PMETA_DEFS_EXIF_TYPE_UNDEFINED:
		return 1;
	case PMETA_DEFS_EXIF_TYPE_SLONG:
		return 4;
	case PMETA_DEFS_EXIF_TYPE_SRATIONAL:
		return 8;
	case PMETA_DEFS_EXIF_TYPE_IFD:
		return 4;
	default:
		return -EINVAL;
	}
}
