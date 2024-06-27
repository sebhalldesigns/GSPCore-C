#ifndef GSVGTYPES_H
#define GSVGTYPES_H

// GSPCore - GSvgTypes.h
// This file defines C structures for the SVG Tiny 1.2 specification
// as described in https://www.w3.org/TR/SVGTiny12/types.html

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef enum {
    SVG_UNIT_NONE,
    SVG_UNIT_IN,
    SVG_UNIT_CM,
    SVG_UNIT_MM,
    SVG_UNIT_PT,
    SVG_UNIT_PC,
    SVG_UNIT_PX,
    SVG_UNIT_PERCENT
} GSvgUnit;

typedef bool GSvgBoolean;

typedef char GSvgChar;

typedef struct {
    float hours;
    float minutes;
    float seconds;
    float fractional;
} GSvgClockVal;

typedef struct {
    float red;
    float green;
    float blue;
} GSvgColor;


// by no means complete - could sepate category from type and extension
typedef enum {
    CONTENT_TYPE_NONE,

    CONTENT_TYPE_AUDIO_3GPP,
    CONTENT_TYPE_AUDIO_3GPP2, 
    CONTENT_TYPE_AUDIO_AAC,
    CONTENT_TYPE_AUDIO_OGG,
    CONTENT_TYPE_AUDIO_MPEG,
    CONTENT_TYPE_AUDIO_MIDI,
    CONTENT_TYPE_AUDIO_WAV,
    CONTENT_TYPE_AUDIO_WEBM,
    CONTENT_TYPE_AUDIO_X_MIDI,


    CONTENT_TYPE_APPLICATION_EPUB_ZIP,
    CONTENT_TYPE_APPLICATION_GZIP,
    CONTENT_TYPE_APPLICATION_JAVA_ARCHIVE,
    CONTENT_TYPE_APPLICATION_JSON,
    CONTENT_TYPE_APPLICATION_LD_JSON,
    CONTENT_TYPE_APPLICATION_MSWORD,
    CONTENT_TYPE_APPLICATION_OCTET_STREAM,
    CONTENT_TYPE_APPLICATION_OGG,
    CONTENT_TYPE_APPLICATION_PDF,
    CONTENT_TYPE_APPLICATION_RTF,
    CONTENT_TYPE_APPLICATION_VND_APPLE_INSTALLER_XML,
    CONTENT_TYPE_APPLICATION_VND_MS_EXCEL,
    CONTENT_TYPE_APPLICATION_VND_MS_FONTOBJECT,
    CONTENT_TYPE_APPLICATION_VND_MS_POWERPOINT,
    CONTENT_TYPE_APPLICATION_VND_MICROSOFT_ICON,
    CONTENT_TYPE_APPLICATION_VND_OASIS_OPENDOCUMENT_PRESENTATION,
    CONTENT_TYPE_APPLICATION_VND_OASIS_OPENDOCUMENT_SPREADSHEET,
    CONTENT_TYPE_APPLICATION_VND_OASIS_OPENDOCUMENT_TEXT,
    CONTENT_TYPE_APPLICATION_VND_OPENXMLFORMATS_OFFICEDOCUMENT_SPREADSHEETML_SHEET,
    CONTENT_TYPE_APPLICATION_VND_OPENXMLFORMATS_OFFICEDOCUMENT_WORDPROCESSINGML_DOCUMENT,
    CONTENT_TYPE_APPLICATION_VND_OPENXMLFORMATS_OFFICEDOCUMENT_PRESENTATIONML_PRESENTATION,
    CONTENT_TYPE_APPLICATION_VND_RAR,
    CONTENT_TYPE_APPLICATION_VND_VISIO,
    CONTENT_TYPE_APPLICATION_XML,
    CONTENT_TYPE_APPLICATION_XHTML_XML,
    CONTENT_TYPE_APPLICATION_X_SH,
    CONTENT_TYPE_APPLICATION_X_TAR,
    CONTENT_TYPE_APPLICATION_X_ZIP_COMPRESSED,
    CONTENT_TYPE_APPLICATION_X_7Z_COMPRESSED,
    CONTENT_TYPE_APPLICATION_ZIP,


    CONTENT_TYPE_FONT_OTF,
    CONTENT_TYPE_FONT_TTF,
    CONTENT_TYPE_FONT_WOFF,
    CONTENT_TYPE_FONT_WOFF2,

    CONTENT_TYPE_IMAGE_AVIF,
    CONTENT_TYPE_IMAGE_BMP,
    CONTENT_TYPE_IMAGE_GIF,
    CONTENT_TYPE_IMAGE_JPEG,
    CONTENT_TYPE_IMAGE_PNG,
    CONTENT_TYPE_IMAGE_SVG_XML,
    CONTENT_TYPE_IMAGE_TIFF,
    CONTENT_TYPE_VIDEO_WEBP,

    CONTENT_TYPE_TEXT_CALENDAR,
    CONTENT_TYPE_TEXT_CSS,
    CONTENT_TYPE_TEXT_CSV,
    CONTENT_TYPE_TEXT_HTML,
    CONTENT_TYPE_TEXT_JAVASCRIPT,
    CONTENT_TYPE_TEXT_PLAIN,
    CONTENT_TYPE_TEXT_XML,

    CONTENT_TYPE_VIDEO_3GPP,
    CONTENT_TYPE_VIDEO_3GPP2,
    CONTENT_TYPE_VIDEO_OGG,
    CONTENT_TYPE_VIDEO_MP2T,
    CONTENT_TYPE_VIDEO_MP4,
    CONTENT_TYPE_VIDEO_MPEG,
    CONTENT_TYPE_VIDEO_WEBM,
    CONTENT_TYPE_VIDEO_X_MSVIDEO,

} GSvgContentType;


typedef struct {
    float x;
    GSvgUnit xUnit;
    float y;
    GSvgUnit yUnit;
} GSvgCoordinate;

typedef enum {
    SVG_FOCUS_NAV_NEXT,
    SVG_FOCUS_NAV_PREV,
    SVG_FOCUS_NAV_UP,
    SVG_FOCUS_NAV_UP_RIGHT,
    SVG_FOCUS_NAV_RIGHT,
    SVG_FOCUS_NAV_DOWN_RIGHT,
    SVG_FOCUS_NAV_DOWN,
    SVG_FOCUS_NAV_DOWN_LEFT,
    SVG_FOCUS_NAV_LEFT,
    SVG_FOCUS_NAV_UP_LEFT,
} GSvgFocus;

typedef char* GSvgFontFamilyValue;

typedef char* GSvgFamilyName;

typedef char* GSvgFontFamilyValue;

typedef struct {
    float size;
    GSvgUnit unit;
} GSvgFontSizeValue;

typedef char* GSvgFuncIri;

typedef char* GSvgId;

typedef char* GSvgIdRef;

typedef int16_t GSvgInteger;

typedef char* GSvgIri;

typedef char* GSvgLanguageId;

typedef struct {
    float length;
    GSvgUnit unit;
} GSvgLength;

typedef struct {
    GSvgContentType* types;
    size_t count;
} GSvgListOfContentTypes;

typedef struct {
    GSvgFamilyName* types;
    size_t count;
} GSvgListOfFamilyNames;

typedef struct {
    GSvgLanguageId* languageIds;
    size_t count;
} GSvgListOfLanguageIds;

typedef struct {
    void* items;
    size_t count;
} GSvgListOfTs;

typedef int32_t GSvgLong;

typedef char* GSvgNcName;

typedef float GSvgNumber;

typedef enum {
    GSVGPAINTVARIANT_NONE,
    GSVGPAINTVARIANT_CURRENT_COLOR,
    GSVGPAINTVARIANT_COLOR,
    GSVGPAINTVARIANT_FUNC_IRI,
    GSVGPAINTVARIANT_SYSTEM_PAINT,
    GSVGPAINTVARIANT_INHERIT
} GSvgPaintVariant;

typedef struct {
    GSvgPaintVariant variant;
    
} GSvgPaint;

typedef char* GSvgString;

typedef struct {
    GSvgString* strings;
    size_t count;
} GSvgListOfStrings;

#endif // GSVGTYPES_H