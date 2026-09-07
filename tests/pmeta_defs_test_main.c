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

#include "pmeta_defs_test.h"

#include <photo-metadata-defs/pmeta_defs.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>


static void test_pmeta_exif_lookups(void)
{
	const struct pmeta_defs_exif_def *def;

	/* 1. Lookup by Hexadecimal Tag ID (e.g., 0x010F = Make) */
	def = pmeta_defs_get_exif_tag(0x010F);
	CU_ASSERT_PTR_NOT_NULL_FATAL(def);
	CU_ASSERT_EQUAL(def->tag_id, 0x010F);
	CU_ASSERT_STRING_EQUAL(def->tag_name, "Exif.Image.Make");
	CU_ASSERT_EQUAL(def->ifd_group, PMETA_DEFS_IFD_0);
	CU_ASSERT_EQUAL(def->expected_type, PMETA_DEFS_EXIF_TYPE_ASCII);
	CU_ASSERT_EQUAL(def->default_count, 0);

	/* 2. Lookup by internal Enum Index */
	def = pmeta_defs_get_exif_tag_by_idx(PMETA_DEFS_EXIF_IDX_FNUMBER);
	CU_ASSERT_PTR_NOT_NULL_FATAL(def);
	CU_ASSERT_EQUAL(def->tag_id, 0x829D);
	CU_ASSERT_STRING_EQUAL(def->tag_name, "Exif.Photo.FNumber");
	CU_ASSERT_EQUAL(def->ifd_group, PMETA_DEFS_IFD_EXIF);
	CU_ASSERT_EQUAL(def->expected_type, PMETA_DEFS_EXIF_TYPE_RATIONAL);
	CU_ASSERT_EQUAL(def->default_count, 1);

	/* 3. Test virtual group mapping (RAW mapped to IFD_0 with empty prefix)
	 */
	def = pmeta_defs_get_exif_tag(0xC614); /* UniqueCameraModel */
	CU_ASSERT_PTR_NOT_NULL_FATAL(def);
	/* The macro should omit the 'Exif.Image.' prefix for RAW tags */
	CU_ASSERT_STRING_EQUAL(def->tag_name, "UniqueCameraModel");
	/* But it must correctly map to the technical IFD_0 group */
	CU_ASSERT_EQUAL(def->ifd_group, PMETA_DEFS_IFD_0);

	/* 4. Boundaries and Invalid Inputs */
	/* Unknown hex ID */
	def = pmeta_defs_get_exif_tag(0xFFFF);
	CU_ASSERT_PTR_NULL(def);
	/* Out of bounds enum index */
	def = pmeta_defs_get_exif_tag_by_idx(PMETA_DEFS_EXIF_IDX_COUNT);
	CU_ASSERT_PTR_NULL(def);
}


static void test_pmeta_exif_groups(void)
{
	const struct pmeta_defs_exif_def *def;

	/* Verify GPS Sub-IFD mapping and prefix */
	def = pmeta_defs_get_exif_tag_by_idx(PMETA_DEFS_EXIF_IDX_GPS_LATITUDE);
	CU_ASSERT_PTR_NOT_NULL_FATAL(def);
	CU_ASSERT_EQUAL(def->ifd_group, PMETA_DEFS_IFD_GPS);
	CU_ASSERT_STRING_EQUAL(def->tag_name, "Exif.GPSInfo.GPSLatitude");

	/* Verify IFD 1 (Thumbnail) mapping and prefix */
	def = pmeta_defs_get_exif_tag_by_idx(
		PMETA_DEFS_EXIF_IDX_THUMBNAIL_OFFSET);
	CU_ASSERT_PTR_NOT_NULL_FATAL(def);
	CU_ASSERT_EQUAL(def->ifd_group, PMETA_DEFS_IFD_1);
	CU_ASSERT_STRING_EQUAL(def->tag_name, "Exif.Image.ThumbnailOffset");
}


static void test_pmeta_exif_iteration(void)
{
	/* Ensure the macro expansion generated a contiguous, valid array */
	for (int i = 0; i < PMETA_DEFS_EXIF_IDX_COUNT; i++) {
		const struct pmeta_defs_exif_def *def =
			pmeta_defs_get_exif_tag_by_idx(
				(enum pmeta_defs_exif_idx)i);

		CU_ASSERT_PTR_NOT_NULL_FATAL(def);
		CU_ASSERT_PTR_NOT_NULL(def->tag_name);
		CU_ASSERT_TRUE(def->ifd_group < PMETA_DEFS_IFD_COUNT);

		/* Ensure reverse lookup by hex ID also works for this tag */
		const struct pmeta_defs_exif_def *def_by_id =
			pmeta_defs_get_exif_tag(def->tag_id);
		CU_ASSERT_PTR_NOT_NULL(def_by_id);
	}
}


static void test_pmeta_xmp_lookups(void)
{
	const struct pmeta_defs_xmp_def *def;

	/* 1. Lookup a standard XMP String property */
	def = pmeta_defs_get_xmp_tag_by_idx(PMETA_DEFS_XMP_IDX_MODEL_ID);
	CU_ASSERT_PTR_NOT_NULL_FATAL(def);
	CU_ASSERT_STRING_EQUAL(def->prop_name, "ModelId");
	CU_ASSERT_EQUAL(def->ns_idx, PMETA_DEFS_XMP_NS_PARROT);
	CU_ASSERT_EQUAL(def->type, PMETA_DEFS_XMP_TYPE_STRING);
	CU_ASSERT_EQUAL(def->count, 0);

	/* 2. Lookup an XMP Sequence property (Array) */
	def = pmeta_defs_get_xmp_tag_by_idx(
		PMETA_DEFS_XMP_IDX_FISHEYE_POLYNOMIAL);
	CU_ASSERT_PTR_NOT_NULL_FATAL(def);
	CU_ASSERT_STRING_EQUAL(def->prop_name, "FisheyePolynomial");
	CU_ASSERT_EQUAL(def->ns_idx, PMETA_DEFS_XMP_NS_CAMERA);
	CU_ASSERT_EQUAL(def->type, PMETA_DEFS_XMP_TYPE_SEQ);
	CU_ASSERT_EQUAL(def->count, 5);

	/* 3. Boundaries and Invalid Inputs */
	def = pmeta_defs_get_xmp_tag_by_idx(PMETA_DEFS_XMP_IDX_COUNT);
	CU_ASSERT_PTR_NULL(def);
}


static void test_pmeta_xmp_iteration(void)
{
	/* Ensure the macro expansion generated a contiguous, valid array */
	for (int i = 0; i < PMETA_DEFS_XMP_IDX_COUNT; i++) {
		const struct pmeta_defs_xmp_def *def =
			pmeta_defs_get_xmp_tag_by_idx(
				(enum pmeta_defs_xmp_idx)i);

		CU_ASSERT_PTR_NOT_NULL_FATAL(def);
		CU_ASSERT_PTR_NOT_NULL(def->prop_name);
		CU_ASSERT_PTR_NOT_NULL(def->full_key);

		/* Ensure the namespace index points to a valid registered
		 * namespace */
		CU_ASSERT_TRUE(def->ns_idx >= 0);
		CU_ASSERT_TRUE(def->ns_idx < PMETA_DEFS_XMP_NS_COUNT);
	}
}


static void test_pmeta_xmp_namespaces(void)
{
	const struct pmeta_defs_xmp_ns *ns;

	/* 1. Lookup known namespace */
	ns = pmeta_defs_get_xmp_ns(PMETA_DEFS_XMP_NS_CAMERA);
	CU_ASSERT_PTR_NOT_NULL_FATAL(ns);
	CU_ASSERT_STRING_EQUAL(ns->prefix, "Camera");
	CU_ASSERT_STRING_EQUAL(ns->uri, "http://pix4d.com/camera/1.0/");

	/* 2. Lookup another known namespace */
	ns = pmeta_defs_get_xmp_ns(PMETA_DEFS_XMP_NS_PARROT);
	CU_ASSERT_PTR_NOT_NULL_FATAL(ns);
	CU_ASSERT_STRING_EQUAL(ns->prefix, "drone-parrot");
	CU_ASSERT_STRING_EQUAL(ns->uri,
			       "http://www.parrot.com/drone-parrot/1.0/");

	/* 3. Boundaries and Invalid Inputs */
	ns = pmeta_defs_get_xmp_ns(PMETA_DEFS_XMP_NS_COUNT);
	CU_ASSERT_PTR_NULL(ns);
}


static void test_pmeta_xmp_resolver(void)
{
	int res;
	enum pmeta_defs_xmp_ns_id ns_idx = PMETA_DEFS_XMP_NS_COUNT;
	const char *prop = NULL;

	/* 1. Valid resolution (Standard syntax with '.') */
	res = pmeta_defs_resolve_xmp_key(
		"drone-parrot.ModelId", &ns_idx, &prop);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(ns_idx, PMETA_DEFS_XMP_NS_PARROT);
	CU_ASSERT_STRING_EQUAL(prop, "ModelId");

	/* 2. Valid resolution (Standard syntax with ':') */
	res = pmeta_defs_resolve_xmp_key(
		"drone-parrot:ModelId", &ns_idx, &prop);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(ns_idx, PMETA_DEFS_XMP_NS_PARROT);
	CU_ASSERT_STRING_EQUAL(prop, "ModelId");

	/* 3. Valid resolution (GStreamer 'Xmp.' prefix syntax) */
	res = pmeta_defs_resolve_xmp_key("Xmp.Camera.Pitch", &ns_idx, &prop);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(ns_idx, PMETA_DEFS_XMP_NS_CAMERA);
	CU_ASSERT_STRING_EQUAL(prop, "Pitch");

	/* 4. Fail-Fast: Unknown namespace */
	res = pmeta_defs_resolve_xmp_key(
		"unknown-namespace.FlightId", &ns_idx, &prop);
	CU_ASSERT_EQUAL(res, -ENOENT);

	/* 5. Fail-Fast: Bad separator (expected '.' or ':') */
	res = pmeta_defs_resolve_xmp_key(
		"drone-parrot-ModelId", &ns_idx, &prop);
	CU_ASSERT_EQUAL(res, -ENOENT);

	/* 6. Robustness against NULL pointers */
	res = pmeta_defs_resolve_xmp_key(NULL, &ns_idx, &prop);
	CU_ASSERT_EQUAL(res, -EINVAL);
}


static void test_pmeta_xmp_resolver_edge_cases(void)
{
	int res;
	enum pmeta_defs_xmp_ns_id ns_idx = PMETA_DEFS_XMP_NS_COUNT;
	const char *prop = NULL;

	/* 1. Empty property name after the dot */
	res = pmeta_defs_resolve_xmp_key("drone-parrot.", &ns_idx, &prop);
	CU_ASSERT_TRUE(res < 0); /* Should fail validation */

	/* 2. Multiple dots: should isolate prefix correctly and keep the rest
	 */
	res = pmeta_defs_resolve_xmp_key(
		"Camera.My.Custom.Tag", &ns_idx, &prop);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(ns_idx, PMETA_DEFS_XMP_NS_CAMERA);
	CU_ASSERT_STRING_EQUAL(prop, "My.Custom.Tag");

	/* 3. Empty string */
	res = pmeta_defs_resolve_xmp_key("", &ns_idx, &prop);
	CU_ASSERT_TRUE(res < 0);
}


static void test_pmeta_xmp_resolver_null_args(void)
{
	int res;
	enum pmeta_defs_xmp_ns_id ns_idx;
	const char *prop;

	/* Valid key, but the output pointer ns_idx is NULL */
	res = pmeta_defs_resolve_xmp_key("Camera.Pitch", NULL, &prop);
	CU_ASSERT_EQUAL(res, -EINVAL);

	/* Valid key, but the output pointer prop is NULL */
	res = pmeta_defs_resolve_xmp_key("Camera.Pitch", &ns_idx, NULL);
	CU_ASSERT_EQUAL(res, -EINVAL);

	/* Everything is NULL */
	res = pmeta_defs_resolve_xmp_key(NULL, NULL, NULL);
	CU_ASSERT_EQUAL(res, -EINVAL);
}


static void test_pmeta_exif_helpers(void)
{
	const struct pmeta_defs_exif_def *def;
	const char *str;
	int ret;

	/* 1. Lookup by name */
	def = pmeta_defs_get_exif_tag_by_name("Exif.Image.Make");
	CU_ASSERT_PTR_NOT_NULL_FATAL(def);
	CU_ASSERT_EQUAL(def->tag_id, 0x010F);

	def = pmeta_defs_get_exif_tag_by_name("UniqueCameraModel");
	CU_ASSERT_PTR_NOT_NULL_FATAL(def);
	CU_ASSERT_EQUAL(def->tag_id, 0xC614);

	def = pmeta_defs_get_exif_tag_by_name("Unknown.Tag");
	CU_ASSERT_PTR_NULL(def);
	def = pmeta_defs_get_exif_tag_by_name(NULL);
	CU_ASSERT_PTR_NULL(def);

	/* 2. Type to string */
	str = pmeta_defs_exif_type_to_str(PMETA_DEFS_EXIF_TYPE_ASCII);
	CU_ASSERT_STRING_EQUAL(str, "ASCII");
	str = pmeta_defs_exif_type_to_str(0xFFFF);
	CU_ASSERT_STRING_EQUAL(str, "UNKNOWN");

	/* 3. IFD group to string */
	str = pmeta_defs_ifd_group_to_str(PMETA_DEFS_IFD_EXIF);
	CU_ASSERT_STRING_EQUAL(str, "EXIF");
	str = pmeta_defs_ifd_group_to_str(0xFF);
	CU_ASSERT_STRING_EQUAL(str, "UNKNOWN");

	/* 4. Type to size */
	ret = pmeta_defs_exif_type_size(PMETA_DEFS_EXIF_TYPE_BYTE);
	CU_ASSERT_EQUAL(ret, 1);
	ret = pmeta_defs_exif_type_size(PMETA_DEFS_EXIF_TYPE_LONG);
	CU_ASSERT_EQUAL(ret, 4);
	ret = pmeta_defs_exif_type_size(PMETA_DEFS_EXIF_TYPE_RATIONAL);
	CU_ASSERT_EQUAL(ret, 8);
	ret = pmeta_defs_exif_type_size(PMETA_DEFS_EXIF_TYPE_IFD + 1);
	CU_ASSERT_EQUAL(ret, -EINVAL);
}


static void test_pmeta_xmp_helpers(void)
{
	const struct pmeta_defs_xmp_def *def;
	const char *str;

	/* 1. Lookup by full key (dot syntax) */
	def = pmeta_defs_get_xmp_tag("drone-parrot.ModelId");
	CU_ASSERT_PTR_NOT_NULL_FATAL(def);
	CU_ASSERT_STRING_EQUAL(def->prop_name, "ModelId");

	/* 2. Lookup by full key (colon syntax) */
	def = pmeta_defs_get_xmp_tag("drone-parrot:ModelId");
	CU_ASSERT_PTR_NOT_NULL_FATAL(def);
	CU_ASSERT_STRING_EQUAL(def->prop_name, "ModelId");

	/* 3. Lookup with Xmp prefix */
	def = pmeta_defs_get_xmp_tag("Xmp.Camera.Pitch");
	CU_ASSERT_PTR_NOT_NULL_FATAL(def);
	CU_ASSERT_STRING_EQUAL(def->prop_name, "Pitch");

	/* 4. Boundaries and invalid inputs */
	def = pmeta_defs_get_xmp_tag("unknown.key");
	CU_ASSERT_PTR_NULL(def);
	def = pmeta_defs_get_xmp_tag(NULL);
	CU_ASSERT_PTR_NULL(def);

	/* 5. Type to string */
	str = pmeta_defs_xmp_type_to_str(PMETA_DEFS_XMP_TYPE_SEQ);
	CU_ASSERT_STRING_EQUAL(str, "SEQ");
	str = pmeta_defs_xmp_type_to_str(0xFFFF);
	CU_ASSERT_STRING_EQUAL(str, "UNKNOWN");
}


static void test_pmeta_integrity(void)
{
	/* Verify no duplicate IDs or names in EXIF dictionary */
	for (int i = 0; i < PMETA_DEFS_EXIF_IDX_COUNT; i++) {
		const struct pmeta_defs_exif_def *def1 =
			pmeta_defs_get_exif_tag_by_idx(
				(enum pmeta_defs_exif_idx)i);

		for (int j = i + 1; j < PMETA_DEFS_EXIF_IDX_COUNT; j++) {
			const struct pmeta_defs_exif_def *def2 =
				pmeta_defs_get_exif_tag_by_idx(
					(enum pmeta_defs_exif_idx)j);

			/* Duplicate IDs are only allowed if they are in
			 * different IFDs (rare but possible in EXIF, though
			 * here we should be unique) */
			if (def1->tag_id == def2->tag_id &&
			    def1->ifd_group == def2->ifd_group) {
				printf("\nDuplicate EXIF Tag ID 0x%04X "
				       "in group %d\n",
				       def1->tag_id,
				       def1->ifd_group);
				CU_FAIL("Duplicate EXIF Tag ID");
			}

			if (strcmp(def1->tag_name, def2->tag_name) == 0) {
				printf("\nDuplicate EXIF Tag Name '%s'\n",
				       def1->tag_name);
				CU_FAIL("Duplicate EXIF Tag Name");
			}
		}
	}

	/* Verify no duplicate names in XMP dictionary within the same
	 * namespace */
	for (int i = 0; i < PMETA_DEFS_XMP_IDX_COUNT; i++) {
		const struct pmeta_defs_xmp_def *def1 =
			pmeta_defs_get_xmp_tag_by_idx(
				(enum pmeta_defs_xmp_idx)i);

		for (int j = i + 1; j < PMETA_DEFS_XMP_IDX_COUNT; j++) {
			const struct pmeta_defs_xmp_def *def2 =
				pmeta_defs_get_xmp_tag_by_idx(
					(enum pmeta_defs_xmp_idx)j);

			if (def1->ns_idx == def2->ns_idx &&
			    strcmp(def1->prop_name, def2->prop_name) == 0) {
				printf("\nDuplicate XMP Property Name '%s' "
				       "in namespace %d\n",
				       def1->prop_name,
				       def1->ns_idx);
				CU_FAIL("Duplicate XMP Property Name");
			}
		}
	}
}


CU_TestInfo g_pmeta_defs_test_main[] = {
	{FN("exif_lookups"), test_pmeta_exif_lookups},
	{FN("exif_groups"), test_pmeta_exif_groups},
	{FN("exif_iteration"), test_pmeta_exif_iteration},
	{FN("exif_helpers"), test_pmeta_exif_helpers},
	{FN("xmp_lookups"), test_pmeta_xmp_lookups},
	{FN("xmp_iteration"), test_pmeta_xmp_iteration},
	{FN("xmp_namespaces"), test_pmeta_xmp_namespaces},
	{FN("xmp_resolver"), test_pmeta_xmp_resolver},
	{FN("xmp_resolver_edge_cases"), test_pmeta_xmp_resolver_edge_cases},
	{FN("xmp_resolver_null_args"), test_pmeta_xmp_resolver_null_args},
	{FN("xmp_helpers"), test_pmeta_xmp_helpers},
	{FN("integrity"), test_pmeta_integrity},

	CU_TEST_INFO_NULL,
};
