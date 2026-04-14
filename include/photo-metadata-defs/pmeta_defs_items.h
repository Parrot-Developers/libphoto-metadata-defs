/**
 * Copyright (c) 2026 Parrot Drones SAS
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * * Neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
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

#ifndef PMETA_DEFS_ITEMS_H
#define PMETA_DEFS_ITEMS_H


/* ================================================================= *
 * EXIF Namespace Prefixes (Single Source of Truth)
 * ================================================================= */
#define PMETA_EXIF_PREFIX_IFD_0 "Exif.Image"
#define PMETA_EXIF_PREFIX_IFD_EXIF "Exif.Photo"
#define PMETA_EXIF_PREFIX_IFD_GPS "Exif.GPSInfo"
#define PMETA_EXIF_PREFIX_IFD_1 "Exif.Image"
#define PMETA_EXIF_PREFIX_IFD_RAW ""


/* Virtual group mapping for RAW tags to IFD0 */
#define PMETA_DEFS_IFD_RAW PMETA_DEFS_IFD_0


/* ================================================================= *
 * Generic EXIF Helper (tid is now 1st arg)
 *
 * This macro bridges the raw tag list with the dictionary entry generator.
 * It prepends the correct namespace prefix based on the IFD group.
 * ================================================================= */
#define _EXIF(tid, _m, ifd, id, prop, type, count)                             \
	_m(tid,                                                                \
	   id,                                                                 \
	   PMETA_EXIF_PREFIX_##ifd,                                            \
	   prop,                                                               \
	   PMETA_DEFS_##ifd,                                                   \
	   type,                                                               \
	   count)


/* clang-format off */
/* codecheck_ignore[COMPLEX_MACRO] */
#define PMETA_DEFS_EXIF_TAG_LIST(_) \
	/* --- IFD 0: Main Image --- */ \
	_EXIF(0x010E, _, IFD_0, IMAGE_DESCRIPTION, \
		"ImageDescription", ASCII, 0) \
	_EXIF(0x010F, _, IFD_0, MAKE, \
		"Make", ASCII, 0) \
	_EXIF(0x0110, _, IFD_0, MODEL, \
		"Model", ASCII, 0) \
	_EXIF(0x0112, _, IFD_0, ORIENTATION, \
		"Orientation", SHORT, 1) \
	_EXIF(0x011A, _, IFD_0, X_RESOLUTION, \
		"XResolution", RATIONAL, 1) \
	_EXIF(0x011B, _, IFD_0, Y_RESOLUTION, \
		"YResolution", RATIONAL, 1) \
	_EXIF(0x0128, _, IFD_0, RESOLUTION_UNIT, \
		"ResolutionUnit", SHORT, 1) \
	_EXIF(0x0131, _, IFD_0, SOFTWARE, \
		"Software", ASCII, 0) \
	_EXIF(0x0132, _, IFD_0, DATETIME, \
		"DateTime", ASCII, 0) \
	_EXIF(0x013B, _, IFD_0, ARTIST, \
		"Artist", ASCII, 0) \
	_EXIF(0x0213, _, IFD_0, YCBCR_POSITIONING, \
		"YCbCrPositioning", SHORT, 1) \
	_EXIF(0x8298, _, IFD_0, COPYRIGHT, \
		"Copyright", ASCII, 0) \
	_EXIF(0x8769, _, IFD_0, EXIF_OFFSET, \
		"ExifOffset", LONG, 1) \
	_EXIF(0x8825, _, IFD_0, GPS_INFO_OFFSET, \
		"GPSInfoOffset", LONG, 1) \
	_EXIF(0xC614, _, IFD_RAW, UNIQUE_CAMERA_MODEL, \
		"UniqueCameraModel", ASCII, 0) \
	_EXIF(0xC61A, _, IFD_0, BLACK_LEVEL, \
		"BlackLevel", LONG, 1) \
	_EXIF(0xC61D, _, IFD_0, WHITE_LEVEL, \
		"WhiteLevel", LONG, 1) \
	_EXIF(0xC621, _, IFD_0, COLOR_MATRIX_1, \
		"ColorMatrix1", SRATIONAL, 9) \
	_EXIF(0xC628, _, IFD_RAW, AS_SHOT_NEUTRAL, \
		"AsShotNeutral", RATIONAL, 3) \
	_EXIF(0xC65A, _, IFD_0, CALIBRATION_ILLUMINANT_1, \
		"CalibrationIlluminant1", SHORT, 1) \
	\
	/* --- Exif Sub-IFD: Photography --- */ \
	_EXIF(0x829A, _, IFD_EXIF, EXPOSURE_TIME, \
		"ExposureTime", RATIONAL, 1) \
	_EXIF(0x829D, _, IFD_EXIF, FNUMBER, \
		"FNumber", RATIONAL, 1) \
	_EXIF(0x8822, _, IFD_EXIF, EXPOSURE_PROGRAM, \
		"ExposureProgram", SHORT, 1) \
	_EXIF(0x8827, _, IFD_EXIF, ISO_SPEED_RATINGS, \
		"ISOSpeedRatings", SHORT, 1) \
	_EXIF(0x8830, _, IFD_EXIF, SENSITIVITY_TYPE, \
		"SensitivityType", SHORT, 1) \
	_EXIF(0x8833, _, IFD_EXIF, ISO_SPEED, \
		"ISOSpeed", LONG, 1) \
	_EXIF(0x9000, _, IFD_EXIF, EXIF_VERSION, \
		"ExifVersion", UNDEFINED, 4) \
	_EXIF(0x9003, _, IFD_EXIF, DATETIME_ORIGINAL, \
		"DateTimeOriginal", ASCII, 0) \
	_EXIF(0x9004, _, IFD_EXIF, DATETIME_DIGITIZED, \
		"DateTimeDigitized", ASCII, 0) \
	_EXIF(0x9101, _, IFD_EXIF, COMPONENTS_CONFIG, \
		"ComponentsConfiguration", UNDEFINED, 4) \
	_EXIF(0x9201, _, IFD_EXIF, SHUTTER_SPEED_VALUE, \
		"ShutterSpeedValue", SRATIONAL, 1) \
	_EXIF(0x9202, _, IFD_EXIF, APERTURE_VALUE, \
		"ApertureValue", RATIONAL, 1) \
	_EXIF(0x9204, _, IFD_EXIF, EXPOSURE_BIAS, \
		"ExposureBiasValue", SRATIONAL, 1) \
	_EXIF(0x9010, _, IFD_EXIF, OFFSET_TIME, \
		"OffsetTime", ASCII, 0) \
	_EXIF(0x9011, _, IFD_EXIF, OFFSET_TIME_ORIGINAL, \
		"OffsetTimeOriginal", ASCII, 0) \
	_EXIF(0x9012, _, IFD_EXIF, OFFSET_TIME_DIGITIZED, \
		"OffsetTimeDigitized", ASCII, 0) \
	_EXIF(0x9207, _, IFD_EXIF, METERING_MODE, \
		"MeteringMode", SHORT, 1) \
	_EXIF(0x9208, _, IFD_EXIF, LIGHT_SOURCE, \
		"LightSource", SHORT, 1) \
	_EXIF(0x9209, _, IFD_EXIF, FLASH, \
		"Flash", SHORT, 1) \
	_EXIF(0x920A, _, IFD_EXIF, FOCAL_LENGTH, \
		"FocalLength", RATIONAL, 1) \
	_EXIF(0x9290, _, IFD_EXIF, SUBSEC_TIME, \
		"SubSecTime", ASCII, 0) \
	_EXIF(0x9291, _, IFD_EXIF, SUBSEC_TIME_ORIGINAL, \
		"SubSecTimeOriginal", ASCII, 0) \
	_EXIF(0x9292, _, IFD_EXIF, SUBSEC_TIME_DIGITIZED, \
		"SubSecTimeDigitized", ASCII, 0) \
	_EXIF(0xA000, _, IFD_EXIF, FLASHPIX_VERSION, \
		"FlashpixVersion", UNDEFINED, 4) \
	_EXIF(0xA001, _, IFD_EXIF, COLOR_SPACE, \
		"ColorSpace", SHORT, 1) \
	_EXIF(0xA002, _, IFD_EXIF, PIXEL_X_DIMENSION, \
		"PixelXDimension", LONG, 1) \
	_EXIF(0xA003, _, IFD_EXIF, PIXEL_Y_DIMENSION, \
		"PixelYDimension", LONG, 1) \
	_EXIF(0xA20E, _, IFD_EXIF, FOCAL_PLANE_X_RES, \
		"FocalPlaneXResolution", RATIONAL, 1) \
	_EXIF(0xA20F, _, IFD_EXIF, FOCAL_PLANE_Y_RES, \
		"FocalPlaneYResolution", RATIONAL, 1) \
	_EXIF(0xA210, _, IFD_EXIF, FOCAL_PLANE_RES_UNIT, \
		"FocalPlaneResolutionUnit", SHORT, 1) \
	_EXIF(0xA300, _, IFD_EXIF, FILE_SOURCE, \
		"FileSource", UNDEFINED, 1) \
	_EXIF(0xA301, _, IFD_EXIF, SCENE_TYPE, \
		"SceneType", UNDEFINED, 1) \
	_EXIF(0xA402, _, IFD_EXIF, EXPOSURE_MODE, \
		"ExposureMode", SHORT, 1) \
	_EXIF(0xA403, _, IFD_EXIF, WHITE_BALANCE, \
		"WhiteBalance", SHORT, 1) \
	_EXIF(0xA404, _, IFD_EXIF, DIGITAL_ZOOM_RATIO, \
		"DigitalZoomRatio", RATIONAL, 1) \
	_EXIF(0xA405, _, IFD_EXIF, FOCAL_LENGTH_35MM, \
		"FocalLengthIn35mmFilm", SHORT, 1) \
	_EXIF(0xA406, _, IFD_EXIF, SCENE_CAPTURE_TYPE, \
		"SceneCaptureType", SHORT, 1) \
	_EXIF(0xA407, _, IFD_EXIF, GAIN_CONTROL, \
		"GainControl", SHORT, 1) \
	_EXIF(0xA408, _, IFD_EXIF, CONTRAST, \
		"Contrast", SHORT, 1) \
	_EXIF(0xA409, _, IFD_EXIF, SATURATION, \
		"Saturation", SHORT, 1) \
	_EXIF(0xA40A, _, IFD_EXIF, SHARPNESS, \
		"Sharpness", SHORT, 1) \
	_EXIF(0xA431, _, IFD_EXIF, BODY_SERIAL_NUMBER, \
		"BodySerialNumber", ASCII, 0) \
	_EXIF(0xC62F, _, IFD_EXIF, CAMERA_SERIAL_NUMBER, \
		"CameraSerialNumber", ASCII, 0) \
	\
	/* --- GPS Sub-IFD --- */ \
	_EXIF(0x0000, _, IFD_GPS, GPS_VERSION_ID, \
		"GPSVersionID", BYTE, 4) \
	_EXIF(0x0001, _, IFD_GPS, GPS_LATITUDE_REF, \
		"GPSLatitudeRef", ASCII, 2) \
	_EXIF(0x0002, _, IFD_GPS, GPS_LATITUDE, \
		"GPSLatitude", RATIONAL, 3) \
	_EXIF(0x0003, _, IFD_GPS, GPS_LONGITUDE_REF, \
		"GPSLongitudeRef", ASCII, 2) \
	_EXIF(0x0004, _, IFD_GPS, GPS_LONGITUDE, \
		"GPSLongitude", RATIONAL, 3) \
	_EXIF(0x0005, _, IFD_GPS, GPS_ALTITUDE_REF, \
		"GPSAltitudeRef", BYTE, 1) \
	_EXIF(0x0006, _, IFD_GPS, GPS_ALTITUDE, \
		"GPSAltitude", RATIONAL, 1) \
	_EXIF(0x0008, _, IFD_GPS, GPS_SATELLITES, \
		"GPSSatellites", SHORT, 1) \
	_EXIF(0x0012, _, IFD_GPS, GPS_MAP_DATUM, \
		"GPSMapDatum", ASCII, 0) \
	\
	/* --- IFD 1 & Links --- */ \
	_EXIF(0x0103, _, IFD_1, COMPRESSION, \
		"Compression", SHORT, 1) \
	_EXIF(0x0201, _, IFD_1, THUMBNAIL_OFFSET, \
		"ThumbnailOffset", LONG, 1) \
	_EXIF(0x0202, _, IFD_1, THUMBNAIL_LENGTH, \
		"ThumbnailLength", LONG, 1)
/* clang-format on */


/* ================================================================= *
 * XMP Namespace Prefixes (Single Source of Truth)
 * ================================================================= */
#define PMETA_XMP_PREFIX_PARROT "drone-parrot"
#define PMETA_XMP_PREFIX_PARROT_STAT "parrot"
#define PMETA_XMP_PREFIX_CAMERA "Camera"
#define PMETA_XMP_PREFIX_TIFF "tiff"
#define PMETA_XMP_PREFIX_EXIF "exif"
#define PMETA_XMP_PREFIX_XMP "xmp"
#define PMETA_XMP_PREFIX_PHOTOSHOP "photoshop"
#define PMETA_XMP_PREFIX_DC "dc"


/* ================================================================= *
 * XMP Namespaces List
 * ================================================================= */
/* clang-format off */
/* codecheck_ignore[COMPLEX_MACRO] */
#define PMETA_DEFS_XMP_NS_LIST(_) \
	_(PARROT, PMETA_XMP_PREFIX_PARROT, \
		"http://www.parrot.com/drone-parrot/1.0/") \
	_(PARROT_STAT, PMETA_XMP_PREFIX_PARROT_STAT, \
		"http://www.parrot.com/status/1.0/") \
	_(CAMERA, PMETA_XMP_PREFIX_CAMERA, \
		"http://pix4d.com/camera/1.0/") \
	_(TIFF, PMETA_XMP_PREFIX_TIFF, \
		"http://ns.adobe.com/tiff/1.0/") \
	_(EXIF, PMETA_XMP_PREFIX_EXIF, \
		"http://ns.adobe.com/exif/1.0/") \
	_(XMP, PMETA_XMP_PREFIX_XMP, \
		"http://ns.adobe.com/xap/1.0/") \
	_(PHOTOSHOP, PMETA_XMP_PREFIX_PHOTOSHOP, \
		"http://ns.adobe.com/photoshop/1.0/") \
	_(DC, PMETA_XMP_PREFIX_DC, \
		"http://purl.org/dc/elements/1.1/")
/* clang-format on */


/* Generic XMP Helper
 *
 * This macro bridges the XMP key list with the dictionary entry generator.
 * It resolves the namespace prefix and ID from the shorthand group name.
 */
#define _NS(id, _m, ns, prop, type, count)                                     \
	_m(id, ns, PMETA_XMP_PREFIX_##ns, prop, type, count)


/* clang-format off */
/* codecheck_ignore[COMPLEX_MACRO] */
#define PMETA_DEFS_XMP_KEY_LIST(_) \
	/* --- tiff --- */ \
	_NS(TIFF_MAKE, _, TIFF, \
		"Make", STRING, 0) \
	_NS(TIFF_MODEL, _, TIFF, \
		"Model", STRING, 0) \
	_NS(TIFF_SOFTWARE, _, TIFF, \
		"Software", STRING, 0) \
	\
	/* --- xmp --- */ \
	_NS(CREATE_DATE, _, XMP, \
		"CreateDate", STRING, 0) \
	_NS(MODIFY_DATE, _, XMP, \
		"ModifyDate", STRING, 0) \
	\
	/* --- dc --- */ \
	_NS(DC_DATE, _, DC, \
		"date", STRING, 0) \
	\
	/* --- drone-parrot --- */ \
	_NS(MODEL_ID, _, PARROT, \
		"ModelId", STRING, 0) \
	_NS(SERIAL_NUMBER, _, PARROT, \
		"SerialNumber", STRING, 0) \
	_NS(SOFTWARE_VERSION, _, PARROT, \
		"SoftwareVersion", STRING, 0) \
	_NS(SOFTWARE_BUILD_ID, _, PARROT, \
		"SoftwareBuildId", STRING, 0) \
	_NS(BOOT_DATE, _, PARROT, \
		"BootDate", STRING, 0) \
	_NS(BOOT_ID, _, PARROT, \
		"BootId", STRING, 0) \
	_NS(FLIGHT_DATE, _, PARROT, \
		"FlightDate", STRING, 0) \
	_NS(FLIGHT_ID, _, PARROT, \
		"FlightId", STRING, 0) \
	_NS(CUSTOM_ID, _, PARROT, \
		"CustomId", STRING, 0) \
	_NS(CAMERA_SERIAL_NUMBER, _, PARROT, \
		"CameraSerialNumber", STRING, 0) \
	_NS(PHOTO_MODE, _, PARROT, \
		"PhotoMode", STRING, 0) \
	_NS(PANORAMA_TYPE, _, PARROT, \
		"PanoramaType", STRING, 0) \
	_NS(PHOTO_COUNT, _, PARROT, \
		"PhotoCount", STRING, 0) \
	_NS(SECURE_CN, _, PARROT, \
		"SecureCn", STRING, 0) \
	_NS(CAPTURE_TS_US, _, PARROT, \
		"CaptureTsUs", STRING, 0) \
	_NS(DRONE_LATITUDE, _, PARROT, \
		"DroneLatitude", STRING, 0) \
	_NS(DRONE_LONGITUDE, _, PARROT, \
		"DroneLongitude", STRING, 0) \
	_NS(DRONE_ALTITUDE, _, PARROT, \
		"DroneAltitudeEgm96Amsl", STRING, 0) \
	_NS(CAMERA_ROLL, _, PARROT, \
		"CameraRollDegree", STRING, 0) \
	_NS(CAMERA_PITCH, _, PARROT, \
		"CameraPitchDegree", STRING, 0) \
	_NS(CAMERA_YAW, _, PARROT, \
		"CameraYawDegree", STRING, 0) \
	_NS(UTC_TS_ACCURACY, _, PARROT, \
		"UtcTsAccuracy", STRING, 0) \
	_NS(SEQUENCE_NUMBER, _, PARROT, \
		"SequenceNumber", STRING, 0) \
	_NS(DRONE_ALTITUDE_WGS84, _, PARROT, \
		"DroneAltitudeWgs84Ellipsoid", STRING, 0) \
	_NS(DRONE_CAMERA_NED_START_QUAT, _, PARROT, \
		"CameraNedStartQuat", SEQ, 4) \
	_NS(THERMAL_SPOT_MIN, _, PARROT, \
		"ThermalSpotMin", SEQ, 3) \
	_NS(THERMAL_SPOT_MAX, _, PARROT, \
		"ThermalSpotMax", SEQ, 3) \
	_NS(THERMAL_MASK, _, PARROT, \
		"ThermalMask", SEQ, 4) \
	_NS(CAMERA_SPECTRUM, _, PARROT, \
		"CameraSpectrum", STRING, 0) \
	_NS(MEDIA_ID, _, PARROT, \
		"MediaId", STRING, 0) \
	_NS(RESOURCE_INDEX, _, PARROT, \
		"ResourceIndex", STRING, 0) \
	\
	/* --- Camera (Pix4D) --- */ \
	_NS(CAMERA_MODEL_TYPE, _, CAMERA, \
		"ModelType", STRING, 0) \
	_NS(CAMERA_FLIGHT_UUID, _, CAMERA, \
		"FlightUUID", STRING, 0) \
	_NS(PERSPECTIVE_FOCAL_LENGTH, _, CAMERA, \
		"PerspectiveFocalLength", STRING, 0) \
	_NS(PERSPECTIVE_FOCAL_LENGTH_UNITS, _, CAMERA, \
		"PerspectiveFocalLengthUnits", STRING, 0) \
	_NS(PERSPECTIVE_DISTORTION, _, CAMERA, \
		"PerspectiveDistortion", SEQ, 5) \
	_NS(FISHEYE_AFFINE_MATRIX, _, CAMERA, \
		"FisheyeAffineMatrix", SEQ, 4) \
	_NS(FISHEYE_AFFINE_SYMMETRIC, _, CAMERA, \
		"FisheyeAffineSymmetric", STRING, 0) \
	_NS(FISHEYE_POLYNOMIAL, _, CAMERA, \
		"FisheyePolynomial", SEQ, 5) \
	_NS(CAMERA_ABOVE_GROUND_ALTITUDE, _, CAMERA, \
		"AboveGroundAltitude", STRING, 0) \
	_NS(CAMERA_HORIZ_CS, _, CAMERA, \
		"HorizCS", STRING, 0) \
	_NS(CAMERA_VERT_CS, _, CAMERA, \
		"VertCS", STRING, 0) \
	_NS(PIX4D_CAMERA_ROLL, _, CAMERA, \
		"Roll", STRING, 0) \
	_NS(PIX4D_CAMERA_PITCH, _, CAMERA, \
		"Pitch", STRING, 0) \
	_NS(PIX4D_CAMERA_YAW, _, CAMERA, \
		"Yaw", STRING, 0) \
	_NS(PIX4D_GPS_XY_ACCURACY, _, CAMERA, \
		"GPSXYAccuracy", STRING, 0) \
	_NS(PIX4D_GPS_Z_ACCURACY, _, CAMERA, \
		"GPSZAccuracy", STRING, 0) \
	_NS(PRINCIPAL_POINT, _, CAMERA, \
		"PrincipalPoint", SEQ, 2) \
	_NS(COLOR_MATRIX, _, CAMERA, \
		"ColorMatrix", SEQ, 9)
/* clang-format on */


#endif /* PMETA_DEFS_ITEMS_H */
