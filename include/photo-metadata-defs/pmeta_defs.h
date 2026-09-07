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

#ifndef PMETA_DEFS_H
#define PMETA_DEFS_H

#include "pmeta_defs_items.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* To be used for all public API */
#ifdef PMETA_DEFS_API_EXPORTS
#	ifdef _WIN32
#		define PMETA_DEFS_API __declspec(dllexport)
#	else /* !_WIN32 */
#		define PMETA_DEFS_API __attribute__((visibility("default")))
#	endif /* !_WIN32 */
#else /* !PMETA_DEFS_API_EXPORTS */
#	define PMETA_DEFS_API
#endif /* !PMETA_DEFS_API_EXPORTS */


/* ================================================================= *
 * Common Definitions
 * ================================================================= */

/**
 * @brief Metadata destination types.
 *
 * Defines the logical target compartment for a metadata entry.
 */
enum pmeta_defs_dest {
	/** EXIF metadata compartment */
	PMETA_DEFS_DEST_EXIF = 0,
	/** XMP metadata compartment */
	PMETA_DEFS_DEST_XMP,
	/** Total count of destination types (for range validation) */
	PMETA_DEFS_DEST_COUNT
};


/* ================================================================= *
 * EXIF Definitions
 * ================================================================= */

/**
 * @brief Standard EXIF/TIFF 6.0 data types.
 */
enum pmeta_defs_exif_type {
	/** 8-bit unsigned integer */
	PMETA_DEFS_EXIF_TYPE_BYTE = 1,
	/** 8-bit byte containing 7-bit ASCII */
	PMETA_DEFS_EXIF_TYPE_ASCII = 2,
	/** 16-bit unsigned integer */
	PMETA_DEFS_EXIF_TYPE_SHORT = 3,
	/** 32-bit unsigned integer */
	PMETA_DEFS_EXIF_TYPE_LONG = 4,
	/** Two 32-bit unsigned integers (numerator/denominator) */
	PMETA_DEFS_EXIF_TYPE_RATIONAL = 5,
	/** 8-bit byte that can take any value depending on the field */
	PMETA_DEFS_EXIF_TYPE_UNDEFINED = 7,
	/** 32-bit signed integer */
	PMETA_DEFS_EXIF_TYPE_SLONG = 9,
	/** Two 32-bit signed integers (numerator/denominator) */
	PMETA_DEFS_EXIF_TYPE_SRATIONAL = 10,
	/** 32-bit offset */
	PMETA_DEFS_EXIF_TYPE_IFD = 13,
};


/**
 * @brief EXIF IFD (Image File Directory) groups.
 */
enum pmeta_defs_ifd_group {
	/** Main Image IFD */
	PMETA_DEFS_IFD_0 = 0,
	/** Photography Sub-IFD */
	PMETA_DEFS_IFD_EXIF,
	/** Geolocation Sub-IFD */
	PMETA_DEFS_IFD_GPS,
	/** Thumbnail Image IFD */
	PMETA_DEFS_IFD_1,
	/** Total count of IFD groups */
	PMETA_DEFS_IFD_COUNT
};


/**
 * @brief Generated enumeration mapping to actual EXIF hex tag IDs.
 */
enum pmeta_defs_exif_tag {
/* codecheck_ignore[COMPLEX_MACRO] */
#define _(tid, id, pref, prop, group, type, count)                             \
	PMETA_DEFS_EXIF_TAG_##id = tid,
	PMETA_DEFS_EXIF_TAG_LIST(_)
#undef _
};


/**
 * @brief Generated enumeration representing continuous 0-based indices for EXIF
 * tags. Used for dictionary lookups.
 */
enum pmeta_defs_exif_idx {
#define _(tid, id, pref, prop, group, type, count) PMETA_DEFS_EXIF_IDX_##id,
	PMETA_DEFS_EXIF_TAG_LIST(_)
#undef _
		PMETA_DEFS_EXIF_IDX_COUNT
};


/**
 * @brief Single source of truth definition for an EXIF tag.
 */
struct pmeta_defs_exif_def {
	/** Hexadecimal tag ID (e.g., 0x0110) */
	uint16_t tag_id;
	/** Full property name (e.g., "Exif.Image.Model") */
	const char *tag_name;
	/** Target IFD group (enum pmeta_defs_ifd_group) */
	enum pmeta_defs_ifd_group ifd_group;
	/** Expected data type (enum pmeta_defs_exif_type) */
	enum pmeta_defs_exif_type expected_type;
	/** Number of items, or 0 for variable length */
	uint32_t default_count;
};


/* ================================================================= *
 * XMP Definitions
 * ================================================================= */

/**
 * @brief XMP property types mapping.
 */
enum pmeta_defs_xmp_type {
	/** Standard string property */
	PMETA_DEFS_XMP_TYPE_STRING = 0,
	/** Ordered array/sequence of items (<rdf:Seq>) */
	PMETA_DEFS_XMP_TYPE_SEQ = 1,
};


/**
 * @brief Generated enumeration of internal XMP namespace IDs.
 */
enum pmeta_defs_xmp_ns_id {
#define _(id, prefix_str, uri_str) PMETA_DEFS_XMP_NS_##id,
	PMETA_DEFS_XMP_NS_LIST(_)
#undef _
		PMETA_DEFS_XMP_NS_COUNT
};


/**
 * @brief Definition of an XMP namespace.
 */
struct pmeta_defs_xmp_ns {
	/** Short prefix (e.g., "drone-parrot") */
	const char *prefix;
	/** Length of the prefix string */
	size_t prefix_len;
	/** Full URI (e.g., "http://www.parrot.com/...") */
	const char *uri;
};


/**
 * @brief Generated enumeration representing continuous 0-based indices for XMP
 * keys. Used for dictionary lookups.
 */
enum pmeta_defs_xmp_idx {
#define _(id, ns, ns_str, prop, type, count) PMETA_DEFS_XMP_IDX_##id,
	PMETA_DEFS_XMP_KEY_LIST(_)
#undef _
		PMETA_DEFS_XMP_IDX_COUNT
};


/**
 * @brief Single source of truth definition for an XMP tag.
 */
struct pmeta_defs_xmp_def {
	/** Target namespace index (enum pmeta_defs_xmp_ns_id) */
	enum pmeta_defs_xmp_ns_id ns_idx;
	/** Raw property name (e.g., "ModelId") */
	const char *prop_name;
	/** Absolute identifier for fallback (e.g., "drone-parrot:ModelId") */
	const char *full_key;
	/** Expected XMP node format (String or Seq) */
	enum pmeta_defs_xmp_type type;
	/** Number of expected items if type is SEQ (0 for STRING) */
	int count;
};


/* ================================================================= *
 * API Functions - EXIF
 * ================================================================= */

/**
 * @brief Look up an EXIF definition by its hexadecimal tag ID.
 *
 * @param tag_id The hex ID of the tag (e.g., 0x0110 for Model).
 * @return Pointer to the EXIF definition, or NULL if not found.
 */
PMETA_DEFS_API const struct pmeta_defs_exif_def *
pmeta_defs_get_exif_tag(uint16_t tag_id);


/**
 * @brief Look up an EXIF definition by its internal enum index.
 *
 * @param idx The continuous zero-based index of the tag.
 * @return Pointer to the EXIF definition, or NULL if out of bounds.
 */
PMETA_DEFS_API const struct pmeta_defs_exif_def *
pmeta_defs_get_exif_tag_by_idx(enum pmeta_defs_exif_idx idx);


/**
 * @brief Look up an EXIF definition by its full name string.
 *
 * @param name The full property name (e.g., "Exif.Image.Model" or
 * "UniqueCameraModel").
 * @return Pointer to the EXIF definition, or NULL if not found.
 */
PMETA_DEFS_API const struct pmeta_defs_exif_def *
pmeta_defs_get_exif_tag_by_name(const char *name);


/**
 * @brief Convert an EXIF data type enum to its string representation.
 *
 * @param type The EXIF data type (enum pmeta_defs_exif_type).
 * @return String name of the type, or "UNKNOWN".
 */
PMETA_DEFS_API const char *
pmeta_defs_exif_type_to_str(enum pmeta_defs_exif_type type);


/**
 * @brief Get the size in bytes of an EXIF data type element.
 *
 * @param type The EXIF data type (enum pmeta_defs_exif_type).
 * @return Size in bytes of a single element of the given type on success,
 * negative errno value
 */
PMETA_DEFS_API int pmeta_defs_exif_type_size(enum pmeta_defs_exif_type type);


/**
 * @brief Convert an IFD group enum to its string representation.
 *
 * @param group The IFD group (enum pmeta_defs_ifd_group).
 * @return String name of the group, or "UNKNOWN".
 */
PMETA_DEFS_API const char *
pmeta_defs_ifd_group_to_str(enum pmeta_defs_ifd_group group);


/* ================================================================= *
 * API Functions - XMP
 * ================================================================= */

/**
 * @brief Look up an XMP definition by its full key string.
 *
 * Automatically resolves the key into its namespace and property name
 * before matching.
 *
 * @param full_key The raw string key (e.g., "Xmp.drone-parrot.ModelId" or
 * "drone-parrot:ModelId").
 * @return Pointer to the XMP definition, or NULL if not found.
 */
PMETA_DEFS_API const struct pmeta_defs_xmp_def *
pmeta_defs_get_xmp_tag(const char *full_key);


/**
 * @brief Look up an XMP definition by its internal enum index.
 *
 * @param idx The continuous zero-based index of the tag.
 * @return Pointer to the XMP definition, or NULL if out of bounds.
 */
PMETA_DEFS_API const struct pmeta_defs_xmp_def *
pmeta_defs_get_xmp_tag_by_idx(enum pmeta_defs_xmp_idx idx);


/**
 * @brief Retrieve an XMP namespace definition by its index.
 *
 * @param index The namespace enum index (enum pmeta_defs_xmp_ns_id).
 * @return Pointer to the namespace structure, or NULL if out of bounds.
 */
PMETA_DEFS_API const struct pmeta_defs_xmp_ns *
pmeta_defs_get_xmp_ns(enum pmeta_defs_xmp_ns_id index);


/**
 * @brief Resolve a custom XMP key into a namespace index and property name.
 *
 * Parses a string formatted as "prefix:property" or "prefix.property" and
 * attempts to match the prefix against the known namespace list.
 *
 * @param full_key The raw string key to resolve (e.g.,
 * "drone-parrot:PhotoMode" or "Xmp.Camera.Pitch").
 * @param out_ns_idx Pointer to store the resolved namespace index.
 * @param out_prop_name Pointer to store the resolved property name pointer
 * (points inside full_key).
 * @return 0 on success, negative errno value (-EINVAL) if parsing fails or
 * namespace is unknown.
 */
PMETA_DEFS_API int
pmeta_defs_resolve_xmp_key(const char *full_key,
			   enum pmeta_defs_xmp_ns_id *out_ns_idx,
			   const char **out_prop_name);


/**
 * @brief Convert an XMP property type enum to its string representation.
 *
 * @param type The XMP property type (enum pmeta_defs_xmp_type).
 * @return String name of the type ("STRING", "SEQ"), or "UNKNOWN".
 */
PMETA_DEFS_API const char *
pmeta_defs_xmp_type_to_str(enum pmeta_defs_xmp_type type);

#ifdef __cplusplus
}
#endif

#endif /* PMETA_DEFS_H */
