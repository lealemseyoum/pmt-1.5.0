/* 
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 *  
 * The Original Code is the Picture Metadata Toolkit,
 * released November 13, 2000.
 *
 * The Initial Developer of the Original Code is Eastman 
 * Kodak Company. Portions created by Kodak are 
 * Copyright (C) 1999-2003 Eastman Kodak Company. All 
 * Rights Reserved. 
 * 
*/

/***************************************************
 * This is a generated file.  DO NOT EDIT!         *
 ***************************************************/

#ifndef PMT_DEFAULT_PREPARSED_SCHEMA
#define PMT_DEFAULT_PREPARSED_SCHEMA

static const char * PmtDefaultPreparsedSchema[] = {
"PmtRootElement","PmtRootElementT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"CaptureConditions","CaptureConditionsT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"CaptureConditionsT defines the different elements related to picture-taking conditions at the time of image capture.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Aperture","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The unit of lens aperture is the APEX  value.",
"The unit of lens aperture is the APEX (Additive System of Photographic Exposure) value.",
#endif
"BatteryLevel","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"",
"",
#endif
"Brightness","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The unit of brightness is the APEX value. ",
"The unit of brightness is the APEX value. Ordinarily it is given in the range of -99.99 to99.99.",
#endif
"Contrast","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the direction of contrast processing applied by the camera when the image was shot.",
"This metadata object indicates the direction of contrast processing applied by the camera when the image was shot. The metadata object has the following enumerations:                 0 = Normal  1 = Soft  2 = Hard  Other = reserved  ",
#endif
"CustomRendered","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the use of special processing on image data, such as rendering geared to output.",
"This metadata object indicates the use of special processing on image data, such as rendering geared to output. When special processing is performed, the reader is expected to disable or minimize any further processing. The metadata object has the following enumerations:                 0 = Normal process  1 = Custom process  Other = reserved  ",
#endif
"DeviceSettingDescription","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates information on the picture-taking conditions of a particular camera model.",
"This metadata object indicates information on the picture-taking conditions of a particular camera model. The tag is used only to indicate the picture-taking conditions in the reader. The information is recorded in the format shown in Figure 17. The data is recorded in Unicode using SHORT type for the number of display rows and columns and UNDEFINED type for the camera settings. The Unicode (UCS-2) string including Signature is NULL terminated. The specifics of the Unicode string are as given in ISO/IEC 10646-1.",
#endif
"DigitalZoomRatio","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the digital zoom ratio when the image was shot.",
"This metadata object indicates the digital zoom ratio when the image was shot. If the numerator of the recorded value is 0, this indicates that digital zoom was not used.",
#endif
"ExposureBias","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The unit of exposure Bias is the APEX value.",
"The unit of exposure Bias is the APEX value. Ordinarily it is given in the range of -99.99 to 99.99.",
#endif
"ExposureIndex","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The exposure index selected on the camera or input device at the time the image is captured.",
"The exposure index selected on the camera or input device at the time the image is captured.",
#endif
"ExposureMode","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the exposure mode set when the image was shot.",
"This metadata object indicates the exposure mode set when the image was shot. In auto-bracketing mode, the camera shoots a series of frames of the same scene at different exposure settings.",
#endif
"ExposureProgram","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Defines the class of the program used by the camera to set exposure when the picture is taken.",
"Defines the class of the program used by the camera to set exposure when the picture is taken. The values are defined as follows:                 0 = Not defined  1 = Manual  2 = Normal program  3 = Aperture priority  4 = Shutter priority  5 = Creative program (biased toward depth of field)  6 = Action program (biased toward fast shutter speed)  7 = Portrait mode (for closeup photos with the background out of focus)  8 = Landscape mode (for landscape photos with the background in focus)  9 to 255 = reserved  ",
#endif
"ExposureTime","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Exposure time, given in seconds.",
"Exposure time, given in seconds.",
#endif
"Flash","FlashT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"FlashT defines the different elements that describe the status of the flash system. The FlashT type is comprised of the following elements: Flash, Fired, Return, and Energy.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Flash","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This tag indicates the status of flash when the image was shot.",
"This tag indicates the status of flash when the image was shot. Bit 0 indicates the flash firing status, bits 1 and 2 indicate the flash return status, bits 3 and 4 indicate the flash mode, bit 5 indicates whether the flash function is present, and bit 6 indicates \"red eye\" mode. (The other descriptions about Flash Tag definition which were mentioned in Exif Version 2.2 are invalid.)                 Values for bit 0 indicating whether the flash fired.  0b = Flash did not fire.  1b = Flash fired.  Values for bits 1 and 2 indicating the status of returned light.  00b = No strobe return detection function  01b = reserved  10b = Strobe return light not detected.  11b = Strobe return light detected.  Values for bits 3 and 4 indicating the camera's flash mode.  00b = unknown  01b = Compulsory flash firing  10b = Compulsory flash suppression  11b = Auto mode  Values for bit 5 indicating the presence of a flash function.  0b = Flash function present  1b = No flash function  Values for bit 6 indicating the camera's red-eye mode.  0b = No red-eye reduction mode or unknown  1b = Red-eye reduction supported  ",
#endif
"Fired","boolean",(char *)2,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates that the camera activated the flash system.",
"Indicates that the camera activated the flash system.",
#endif
"Return","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates that the camera detected reflected light from the flash.",
"Indicates that the camera detected reflected light from the flash.",
#endif
"Energy","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates the strobe energy at the time the image is captured.",
"Indicates the strobe energy at the time the image is captured, as measured in Beam Candle Power Seconds (BCPS).",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Flash",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Fnumber","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The F number. ",
"The F number. ",
#endif
"FocalLength","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The actual focal length of the lens, in mm. ",
"The actual focal length of the lens, in mm. Conversion is not made to the focal length of a 35 mm film camera.",
#endif
"FocalLengthIn35mmFilm","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the equivalent focal length assuming a 35mm film camera, in mm.",
"This metadata object indicates the equivalent focal length assuming a 35mm film camera, in mm. A value of 0 means the focal length is unknown. Note that this tag differs from the FocalLength tag.",
#endif
"GainControl","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the degree of overall image gain adjustment.",
"This metadata object indicates the degree of overall image gain adjustment. The metadata object has the following enumerations:                 0 = None  1 = Low gain up  2 = High gain up  3 = Low gain down  4 = High gain down  Other = reserved  ",
#endif
"ImageNumber","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object encodes the number assigned to an image.",
"This metadata object encodes the number assigned to an image. This tag is useful when storing a burst of images which are \"chained\" together within the same TIFF/EP file.",
#endif
"LightSource","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The light source",
"The light source, defined as follows:                 0 = unknown  1 = Daylight  2 = Fluorescent  3 = Tungsten (incandescent light)  4 = Flash  9 = Fine weather  10 = Cloudy weather  11 = Shade          17 = Standard light A  18 = Standard light B  19 = Standard light C  20 = D55  21 = D65  22 = D75  23 = D50  24 = ISO studio tungsten  255 = other light source  Other = reserved  ",
#endif
"Magnification","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Magnification is the ratio of the focal length divided by the subject distance",
"Magnification is the ratio of the focal length divided by the subject distance",
#endif
"MaxAperture","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The smallest F number of the lens is the APEX value.",
"The smallest F number of the lens is the APEX value.",
#endif
"MeteringMode","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The metering mode",
"The metering mode, defined as follows:                 0 = unknown  1 = Average  2 = CenterWeightedAverage  3 = Spot  4 = MultiSpot  5 = Pattern  6 = Partial  7 to 254 = reserved  255 = other  ",
#endif
"PAR","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"PAR, PrintAspectRatio, defines the desired print format of the current image.",
"PAR, PrintAspectRatio, defines the desired print format of the current image. The PAR Flag has the following enumeration:                 0 = the complete image area is displayed  1 = APS Print, H print (~16:9)  2 = APS Print, P print (~3:1)  3 = APS Print, C print (~3:2)  ",
#endif
"PrintQuantity","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field stores the number of prints requested for this image.",
"This field stores the number of prints requested for this image.",
#endif
"Saturation","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the direction of saturation processing applied by the camera when the image was shot. ",
"This metadata object indicates the direction of saturation processing applied by the camera when the image was shot. The metadata object has the following enumerations:                 0 = Normal  1 = Low saturation  2 = High saturation  Other = reserved  ",
#endif
"SceneCaptureType","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the type of scene that was shot.",
"This metadata object indicates the type of scene that was shot. It can also be used to record the mode in which the image was shot. Note that this differs from the scene type (SceneType) tag.  The metadata object has the following enumerations:                 0 = Standard  1 = Landscape  2 = Portrait  3 = Night scene  Other = reserved  ",
#endif
"SecurityClassification","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object encodes the level of security classification assigned to the image.",
"This metadata object encodes the level of security classification assigned to the image. The tag value can either be a single ASCII character or an ASCII string.",
#endif
"SelfTimerMode","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"",
"",
#endif
"Sharpness","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the direction of sharpness processing applied by the camera when the image was shot.",
"This metadata object indicates the direction of sharpness processing applied by the camera when the image was shot. The metadata object has the following enumerations:                 0 = Normal  1 = Soft  2 = Hard  Other = reserved  ",
#endif
"ShutterSpeed","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The unit of shutter speed.",
"The unit of shutter speed  is the APEX (Additive System of Photographic Exposure) setting.",
#endif
"SubjectArea","vuint16",(char *)11,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the location and area of the main subject in the overall scene.",
"This metadata object indicates the location and area of the main subject in the overall scene.",
#endif
"SubjectDistance","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The distance to the subject in meters.",
"The distance to the subject in meters.",
#endif
"SubjectDistanceRange","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the distance to the subject. ",
"This metadata object indicates the distance to the subject. The metadata object has the following enumerations:                 0 = unknown  1 = Macro  2 = Close view  3 = Distant view  Other = reserved  ",
#endif
"WhiteBalance","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the white balance mode set when the image was shot.",
"This metadata object indicates the white balance mode set when the image was shot. The metadata object has the following enumerations:                 0 = Auto white balance  1 = Manual white balance  Other = reserved  ",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"CaptureConditions",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"CaptureDevice","CaptureDeviceT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"CaptureDeviceT contains different elements that define the device that generated the image, such as a camera, scanner, video digitizer or other equipment. ",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"CFAPattern","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"CFAPattern indicates the color filter array (CFA) geometric pattern of the image sensor when a one-chip color area sensor is used. ",
"CFAPattern indicates the color filter array (CFA) geometric pattern of the image sensor when a one-chip color area sensor is used. ",
#endif
"CFARepeatPatternDim","vuint16",(char *)11,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object encodes the number of pixels horizontally and vertically that are needed to uniquely define the repeat pattern of the color filter array (CFA) pattern used in the color image sensor.",
"This metadata object encodes the number of pixels horizontally and vertically that are needed to uniquely define the repeat pattern of the color filter array (CFA) pattern used in the color image sensor. It is mandatory when PhotometricInterpretation = 32803, and there are no defaults allowed. It is optional when PhotometricInterpretation = 2 or 6 and SensingMethod = 2, where it can be used to indicate the original sensor sampling positions.",
#endif
"CameraOwner","CameraOwnerT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"CameraOwnerT defines the different elements that identify the name of the camera owner. ",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"CameraOwner","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A character string that identifies the name of the camera owner. ",
"A character string that identifies the name of the camera owner. Photographer or image creator.  ",
#endif
"EK","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A character string that identifies the name of the camera owner. ",
"A character string that identifies the name of the camera owner. Photographer or image creator.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"CameraOwner",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"FID","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field contains a string which records information from the photo finisher that is used to identify an order.",
"This field contains a string which records information from the photo finisher that is used to identify an order. In the case of Advanced Photo System film, the FID will be written; for other film, the lab will use its own discretion.",
#endif
"FilmCategory","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"FilmCategory defines the category of the film used.",
"FilmCategory defines the category of the film used. FilmCategory vales are defined as follows:                 0 = Unidentified  1 = Negative BandW  2 = Negative Color  3 = Negative BandW  4 = Reversal Color  5 - 255 = Reserved  ",
#endif
"FilmGencode","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"FilmGencode defines the film gencode.",
"FilmGencode defines the film gencode.",
#endif
"FilmProductCode","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"FilmProductCode defines the film product code.",
"FilmProductCode defines the film product code.",
#endif
"FilmSize","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"FilmSize defines the size of the film.",
"FilmSize defines the size of the film. Valid values:                 0 = 135 (35mm)  1 = 124(APS)  2 - 254 = Reserved  255 = Unidentified  ",
#endif
"FocalPlaneResolutionUnit","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates the unit for measuring FocalPlaneXResolution and FocalPlaneYResolution.",
"Indicates the unit for measuring FocalPlaneXResolution and FocalPlaneYResolution. This value is the same as the ResolutionUnit. The values are defined as follows:                 2 = inches  3 = centimeters  Others = reserved  ",
#endif
"FocalPlaneXResolution","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates the number of pixels in the image width (X) direction per FocalPlaneResolutionUnit on the image device focal plane.",
"Indicates the number of pixels in the image width (X) direction per FocalPlaneResolutionUnit on the image device focal plane.",
#endif
"FocalPlaneYResolution","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates the number of pixels in the image height (Y) direction per FocalPlaneResolutionUnit on the image device focal plane.",
"Indicates the number of pixels in the image height (Y) direction per FocalPlaneResolutionUnit on the image device focal plane.",
#endif
"HostComputer","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The computer and/or operating system in use at the time of image creation.",
"The computer and/or operating system in use at the time of image creation.",
#endif
"ISOSpeedRating","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates the ISO Speed and ISO Latitude of the camera or input device as specified in ISO 12232xiii.",
"Indicates the ISO Speed and ISO Latitude of the camera or input device as specified in ISO 12232xiii.",
#endif
"Interlace","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the field number of multi-field images. ",
"This metadata object indicates the field number of multi-field images. The tag allows for both simple vertical interlace of between 2 and 255 interlace fields per frame, and for 2-dimensional interlace of up to 127 vertical x 127 horizontal spatial offset locations per frame. Another application of this interlace mechanism is to store images for progressive transmission. For 2:1 vertical interlace, as employed in many video systems, the first field (lines 1, 3, 5 from the frame image) is indicated with a tag value = 1, and the second field is indicated with a tag value = 2. For 2- dimensional interlace, the 7 least significant bits of the value indicate the vertical field number, and the next 7 bits indicate the horizontal field number. To store multiple interlace fields in one TIFF/EP file, the different fields shall use the IFD chaining mechanism.",
#endif
"Make","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The manufacturer of the recording equipment.",
"The manufacturer of the recording equipment. This is the manufacturer of the DSC, scanner, video digitizer or other equipment that generated the image. When the field is left blank, it is treated as unknown.",
#endif
"MakerNotes","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A character string that identifies from a manufacturers perspective, any desired information about the image writer or capture device.",
"A character string that identifies from a manufacturers perspective, any desired information about the image writer or capture device.",
#endif
"Model","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The model name or model number of the equipment.",
"The model name or model number of the equipment. This is the model name of number of the DSC, scanner, video digitizer or other equipment that generated the image. When the field is left blank, it is treated as unknown.",
#endif
"NativePhysicalResolutionUnit","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the unit for measuring NativePhysicalXResolution and NativePhysicalYResolution.",
"This metadata object indicates the unit for measuring NativePhysicalXResolution and NativePhysicalYResolution.",
#endif
"NativePhysicalXResolution","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the number of pixel sensors in the X direction per the NativePhysicalResolutionUnit on the image sensor focal plane.",
"This metadata object indicates the number of pixel sensors in the X direction per the NativePhysicalResolutionUnit on the image sensor focal plane.",
#endif
"NativePhysicalYResolution","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the number of pixel sensors in the Y direction per the NativePhysicalResolutionUnit on the image sensor focal plane.",
"This metadata object indicates the number of pixel sensors in the Y direction per the NativePhysicalResolutionUnit on the image sensor focal plane.",
#endif
"Noise","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object encodes camera noise measurement values.",
"This metadata object encodes camera noise measurement values.",
#endif
"OECF","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"OECF the \"Opto-Electronic Conversion Function\" is the relationship between the optical input and the image file code value output of an electronic camera.",
"OECF the \"Opto-Electronic Conversion Function\" is the relationship between the optical input and the image file code value output of an electronic camera.  It is expressed as a vector of floats.  The first element m and subsequent elements m+4 is the Log Exposure (where m is equal to or greater then1).  The second element n and subsequent elements n+4 is the RED output level (where n is equal to or greater than 2).  The third element o and subsequent elements 0+4 is the GREEN output level (where o is equal to or greater than 3).  The fourth element is p and subsequent elements p+4 is the BLUE output level (where p is equal to or greater than 4).",
#endif
"Scanner","ScannerT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"ScannerT defines those elements associated with the scanner firmware.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"ModelAndVersion","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A character string that identifies the name and version of the scanner firmware.",
"A character string that identifies the name and version of the scanner firmware that created the digital image file or raw sensor image data.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Scanner",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"SensingMethod","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The image sensor type on the camera or input image device.",
"The image sensor type on the camera or input image device. The defined values are as follows:                 1 = Not defined  2 = One-chip color area sensor  3 = Two-chip color area sensor  4 = Three-chip color area sensor  5 = Color sequential area sensor  7 = Trilinear sensor  8 = Color sequential linear sensor  Other = reserved  ",
#endif
"SerialNumber","SerialNumberT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"SerialNumberT defines the element that identifies the serial number of the camera.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Camera","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A character string that identifies the serial number of the camera.",
"A character string that identifies the serial number of the camera.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"SerialNumber",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Software","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object records the name and version of the software or firmware of the camera or image input device used to generate the image. ",
"This metadata object records the name and version of the software or firmware of the camera or image input device used to generate the image. The detailed format is not specified, but it is recommended that the example shown below be followed. When the field is left blank, it is treated as unknown. ",
#endif
"SpatialFrequencyResponse","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The spatial frequency response (SFR) of a camera or image capture device.",
"The spatial frequency response (SFR) of a camera or image capture device as defined in ISO/TC42/WG18 work item [188] Working Draft 6.0, \"Photography -Electronic Still Picture Cameras - Resolution Measurement\" is represented as a vector of floats. The first element m and subsequent elements m+3 is the Spatial Frequency of the line widths per picture height(where m is greater than or equal to 1). The second element n and subsequent elements n+3 is the Horizontal SFR(where n is equal to or greater then 2).  The third element o and subsequent elements o+3 is the Vertical SFR(where o is equal to or greater then 3).  A non value entry will be identified by a -1.0.",
#endif
"SpectralSensitivity","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates the spectral sensitivity of each channel of the camera used.",
"Indicates the spectral sensitivity of each channel of the camera used. The tag value is an ASCII string compatible with the standard developed by the ASTM Technical committee.",
#endif
"Type","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates the type of capture device used.",
"Indicates the type of capture device used.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"CaptureDevice",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"DigitalProcess","DigitalProcessT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"DigitalProcessT defines the elements that describe the digital processing applied to the image",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"CellLength","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The length of the dithering or halftoning matrix used to create a dithered or halftoned bilevel file.",
"The length of the dithering or halftoning matrix used to create a dithered or halftoned bilevel file.",
#endif
"CellWidth","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The width of the dithering or halftoning matrix used to create a dithered or halftoned bilevel file.",
"The width of the dithering or halftoning matrix used to create a dithered or halftoned bilevel file. Tag = 264 (108.H)",
#endif
"EnhancementPathUtilized","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The enhancement path taken.",
"The enhancement path taken.                 0 = No processing was applied  1 = Ansel Locked Beam for Digital and Color Negative Paths  2 = Ansel Enhanced for Digital and Color Negative Paths  3 = Ansel Balance Digital Path and Optical Color Negative Path  4 = Ansel Premium for Digital and Color Negative Paths  Others = reserved.  ",
#endif
"CompletedImageProcessing","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This",
"This field",
#endif
"EnhancementPathVersionNumber","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The version of enhancement used.",
"The version of enhancement used.",
#endif
"HalftoneHints","vuint16",(char *)11,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The purpose of the HalftoneHints field is to convey to the halftone function the range of gray levels within a colorimetrically-specified image that should retain tonal detail. ",
"The purpose of the HalftoneHints field is to convey to the halftone function the range of gray levels within a colorimetrically-specified image that should retain tonal detail. The field contains two values of sixteen bits each and, therefore, is contained wholly within the field itself; no offset is required. The first word specifies the highlight gray level which should be halftoned at the lightest printable tint of the final output device. The second word specifies the shadow gray level which should be halftoned at the darkest printable tint of the final output device. Portions of the image which are whiter than the highlight gray level will quickly, if not immediately, fade to specular highlights. There is no default value specified, since the highlight and shadow gray levels are a function of the subject matter of a particular image. Appropriate values may be derived algorithmically or may be specified by the user, either directly or indirectly. The HalftoneHints field, as defined here, defines an achromatic function. It can be used just as effectively with color images as with monochrome images. When used with opponent color spaces such as CIE L*a*b* or YCbCr, it refers to the achromatic component only; L* in the case of CIELab, and Y in the case of YCbCr. When used with tri-stimulus spaces such as RGB, it suggests to retain tonal detail for all colors with an NTSC gray component within the bounds of the R=G=B=Highlight to R=G=B=Shadow range.",
#endif
"History","HistoryT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"HistoryT defines the various elements pertaining to the history of image edits. They include SBA operations and the EditTagArray.  EditTagArray contains a summary of the edit operations applied to the image",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"EditTagArray","vuint16",(char *)11,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"EditTagArray contains a summary of the edit operations and therefore only one occurrence of each defined edit functions will be stored in the array. ",
"From the Picture CD Appendix C specification: The Edit Tags are designed to help manage the maintenance issues associated with metadata as different operations that modify or edit an image are applied. The array contains a summary of the edit operations and therefore only one occurrence of each defined edit functions will be stored in the array. The values of the array are enumerated in the following manner:                 1. Digital image created  2. Image cropped  3. Image rotated  4. Global tone/ color adjustment applied. e. g., color balance, contrast adjust, density adjust, etc.  5. Global spatial adjustment applied e. g., sharpening, extreme compression , substantial subsampling, blur, noise suppression, etc.  6. Image retouched e. g., redeye, scratch removal  7. Pixels edited extensively, significantly changing the captured scene content. e. g., object removal, image warping or morphing operations  8. Image composited with another image or background, or graphics and/ or text added  9. Regional tone/ color adjustment applied. e. g., digital \"dodge and burn\" operation, selective color adjustment, etc.  10 - 65535 Reserved for future edit functional definitions.  ",
#endif
"SBAExposureRecord","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"SBAExposureRecord field contains the complete exposure record from the SBA algorithm.",
"SBAExposureRecord field contains the complete exposure record from the SBA algorithm.  The purpose of this field is for use by the creation system in diagnostics mode.  The count is the number of bytes in the data stream.If this information is available to the creation system this field shall be written, otherwise this field shall be omitted. ",
#endif
"SBAInputImageBitDepth","vuint16",(char *)11,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"SBAInputImageBitDepth field indicates the bit depth of each channel of the RGB input image to the SBA algorithm.",
"SBAInputImageBitDepth field indicates the bit depth of each channel of the RGB input image to the SBA algorithm. The first value is the bit depth of the red channel, the second value is the bit depth of the green channel, and the third value is the bit depth of the blue channel.  If the creation system applied SBA adjustments to the primary image this field shall be written, otherwise this field shall be omitted. ",
#endif
"SBAInputImageColorspace","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"SBAInputImageColorspace defines the colorspace used as an input to the SBA.",
"SBAInputImageColorspace defines the colorspace used as an input to the SBA.  Defined vales are:                 0 = Uncalibrated  1 = RIM  2 = ROM  3 = RPD  4 = RLSE  Others = reserved.  ",
#endif
"SBARGBShifts","vint32",(char *)13,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"SBARGBShifts contains the scene balance shifts that were calculated by the SBA algorithm.",
"SBARGBShifts contains the scene balance shifts that were calculated by the SBA algorithm. The actual definition of the recorded shifts is dependent upon the color space of the input image to the SBA algorithm.  It is assumed that the input image to the SBA is three channel and it's composition is an RGB representation.  The first value of this field is the SBA shift for the Red channel, the second value is the SBA shift for the Green channel, and the third value if for the Green channel.  If the input image color space is reference printing densities (RPD), then the SBA _RGB_Shifts field is defined as reference printing densities * 1000.  ",
#endif
"UserAdjSBARGBShifts","vint32",(char *)13,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"UserAdjSBARGBShifts field contains the SBA shifts that were determined via user feedback while viewing (i.e. operator at the scan service) the image.",
"UserAdjSBARGBShifts field contains the SBA shifts that were determined via user feedback while viewing (i.e. operator at the scan service) the image. It will be assumed that the image was viewed in a \"SRGB\" colorspace. The shifts are represented in pixel code values. The first value in the field is the shift for the red channel, the second value is the shift for the green channel, and the third value is the shift for the blue channel.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"History",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"ICCProfile","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object encodes an embedded InterColor device profile relating to the camera color characterization.",
"This metadata object encodes an embedded InterColor device profile relating to the camera color characterization.",
#endif
"ImageFileChangeDateTime","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The date and time of the last change that has been made to the image file. ",
"The date and time of the last change that has been made to the image file. The format is \"YYYY:MM:DD HH:MM:SS.TH\" with time shown in 24-hour format including fractions of a second, and the date and time separated by one blank character [20.H]. When the date and time are unknown, all the character spaces except colons (\":\") may be filled with blank characters, or else the entire field may be filled with blank characters. The character string length is 20 bytes including NULL for termination. When the field is left blank, it is treated as unknown. Note, at file creation this value is identical to ImageFileCreationDateTime. ",
#endif
"ImageFileCreationDateTime","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The date and time when the image was stored as a digital file.",
"The date and time when the image was stored as a digital file. The format is \"YYYY:MM:DD HH:MM:SS.TH\" with time shown in 24-hour format including fractions of a second, and the date and time separated by one blank character [20.H]. When the date and time are unknown, all the character spaces except colons (\":\") may be filled with blank characters, or else the entire field may be filled with blank characters. The character string length is 20 bytes including NULL for termination. When the field is left blank, it is treated as unknown. ",
#endif
"ImageFileCreationSubSecond","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The recorded fractions of seconds for the date and time when the image was stored as a digital file.",
"The recorded fractions of seconds for the date and time when the image was stored as a digital file.",
#endif
"ImagePrintStatus","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"ImagePrintStatus field indicates the print status of the image as detected by the system that created the image.",
"ImageRotationStatus field indicates the print status of the image as detected by the system that created the image. Defined values:                 0 = Image may be printed, no detected problems  1 = Image not suitable for printing, REASON  2 = Image not suitable for printing, REASON  3 = Image not suitable for printing, REASON  4 = Image not suitable for printing, REASON  5 - 255 =  Reserved  ",
#endif
"ImageRotationStatus","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"ImageRotationStatus field indicates what triggered the system to rotate the image from the orientation of the original scan.",
"ImageRotationStatus field indicates what triggered the system to rotate the image from the orientation of the original scan. Defined values:                 0 = image not rotated  1 = rotated based off of  user input (i. e. Scan operator)  2 = rotated based off of customer direction  3 =  rotated based off of algorithm input  4 - 254 =  Reserved  255 = Unknown reason  ",
#endif
"ImageSource","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The image source indicating the source of the image.",
"The image source indicating the source of the image. If a DSC recorded the image, this tag value will always be set to 3, indicating that the image was recorded on a DSC.                 0 = others  1 = scanner of transparent type  2 = scanner of reflex type  3 = DSC  Other = reserved  ",
#endif
"ImageSourceEK","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The image source indicating the device source of the image.",
"The image source indicating the device source of the image. The values defined are as follows:                 0 = unidentified  1 = film scanner  2 = reflection print scanner  3 = digital still camera  4 = still from video  5 = computer graphics  Other = reserved  ",
#endif
"ImageState","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field indicates the state of image processing that has been conducted on the image data.",
"This field indicates the state of image processing that has been conducted on the image data.The values defined as follows:                 0 = R0, is the initial state designated as image data enters the Rendered Digital Input Path   1 = R1, is the state assigned when image data has completed all the Rendered Input Normalization operations  2 = R2, is the state assigned when image data has completed all the Rendered Image Correction / Enhancement operations.  3 = R3, is the state assigned when the image data has completed all the Rendered Product Creation operations.  4 = R4, is the state assigned when the image data has completed all the Normal Output Conversion operations.  5 = R5, is the state assigned when the image data has completed all the Unit Specific Output Conversion operations.  Other = reserved  ",
#endif
"Jpeg","JpegT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"JpegT defines the various elements that describe the different components of the compressed image, such as AlphaQTable, Chroma1QTable, Chroma2QTable, and LuminQTable.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"AlphaQTable","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"JPEG compression quantization table applied to the alpha channel of the image.",
"JPEG compression quantization table applied to the alpha channel of the image.",
#endif
"Chroma1QTable","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"JPEG compression quantization table applied to the first chromanance channel of the image.",
"JPEG compression quantization table applied to the first chromanance channel of the image.",
#endif
"Chroma2QTable","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"JPEG compression quantization table applied to the second chromanance channel of the image.",
"JPEG compression quantization table applied to the second chromanance channel of the image.",
#endif
"LuminQTable","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"JPEG compression quantization table applied to the luminance channel of the image.",
"JPEG compression quantization table applied to the luminance channel of the image.",
#endif
"SOF","SOFT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"SOFT defines the different elements associated with the JPEF SOF marker",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"ChrominanceSampling","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This is the chrominance sampling of the compressed image.",
"This is the chrominance sampling of the compressed image. It is read from the SOF marker.",
#endif
"Components","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This is the number of channels in the compressed image.",
"This is the number of channels in the compressed image. It is read from the SOF marker.",
#endif
"Height","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This is the height of the compressed image.",
"This is the height of the compressed image. It is read from the SOF marker.",
#endif
"HorizontalSampling","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This is a vector with the Horizontal Sampling factors.",
"This is a vector with the Horizontal Sampling factors for each component in the compressed image. It is read from the SOF marker.",
#endif
"Precision","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This is the data precision in bits of each channel in the compressed image.",
"This is the data precision in bits of each channel in the compressed image. It is read from the SOF marker.",
#endif
"VerticalSampling","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This is a vector with the Vertical Sampling factors.",
"This is a vector with the Vertical Sampling factors for each component in the compressed image. It is read from the SOF marker.",
#endif
"Width","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This is the width of the compressed image.",
"This is the width of the compressed image. It is read from the SOF marker.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"SOF",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Jpeg",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"PrinterDock","PrinterDockT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"color preference metadata",
"ImageLink metadata supports storing color preference metadata that ImageLink camerapartners will stoinre in ther digital camera image files. The printer dock willinteroperatewith this data and apply the color preference stored in the metadata         ",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"ImageLinkVersion","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"ImageLinkVersion indicates the version of the metadata stored within the PrinterDock IFD",
"ImageLinkVersion indicates the version of the metadata stored within the PrinterDock IFD.Version 1.0 is indicated as '0100' or in hexadecimal as 0x30, 0x31, 0x30, 0x30.             ",
#endif
"UseOfImageLinkPartnerColorPreferenceData","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"UseOfImageLinkPartnerColorPreferenceData indicates if the ImageLink Partner Color Preference Data should be used.",
"UseOfImageLinkPartnerColorPreferenceData indicates if the ImageLink Partner Color Preference Data should be used.If the tag value is set to 0xFF then the data shall be used. If the camera has performed special color enhancementsand therefore does not what to use the ImageLink Partner Color Preference Data then the tag value is set to 0x00. Thedefault value is 0xFF.             ",
#endif
"ImageLinkPartnerColorPreferenceDataType","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"ImageLinkPartnerColorPreferenceDataType indicates the data composition of the ImageLink Partner Color Preference Data.",
"ImageLinkPartnerColorPreferenceDataType indicates the data composition of the ImageLink Partner Color Preference Data.The values of this tag has the following enumerated values:                 1 = 1D LUT for 3 color channels comprised of 3x256=768 bytes of data  2 = encrypted 1D LUT for 3 color channels using the printer public key. This keywill be provided to camera partner manufacturers during camera development.                   3 = a 3D LUT comprised of 17-cubed elements for each color channel that represents17-cubed x 3 = 14739 bytes of data                   4 - encrypted 3D LUT comprised of 17-cubed elements for each color channel using theprinter public key. This key will be provided to camera partner manufacturersduring camera development.                   5 - a 3D LUT comprised of 33-cubed elements for each color channel that represents33-cubed x 3 = 107811 bytes of data                   6 - encrypted 3D LUT comprised of 33-cubed elements for each color channel using theprinter public key. This key will be provided to camera partner manufacturersduring camera development.                   0,7-65535 are reserved for future definitions   NOTE 1: If values 5 or 6 are used, the 3D LUT data will be stored in uniquely labeled APP4s thatwill leverage the method used in Kodak Professional digital camersa to store compressed extendedrange image data into multiple Application Segments. This method extends Exif in a non-destructivemanner while providing the means to store more than 64 Kbytes of data.                 NOTE 2:the actual encryption mechanism is TBD. ",
#endif
"ImageLinkPartnerColorPreferenceData","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"ImageLinkColorPreferenceData is the color preference data the camera systemswishes the printer dock system to use.             ",
"mageLinkColorPreferenceData is the color preference data the camera systemswishes the printer dock system to use. The format of the data is described inImageLinkColorPreferenceDataType             ",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"PrinterDock",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"RollGuid","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"RollGuid is a unique identifier for a roll of film that was assigned during the digitization process.",
"RollGuid is a unique identifier for a roll of film that was assigned during the digitization process. Further definition is TBD",
#endif
"ScanFrameSequenceNumber","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field contains the sequence number of the frame as it was scanned.",
"This field contains the sequence number of the frame as it was scanned. For APS scans, this field will contain the frame number that is decoded from the latent image barcode.",
#endif
"Thresholding","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"For black and white TIFF files that represent shades of gray, the technique used to convert from gray to black and white pixels.",
"For black and white TIFF files that represent shades of gray, the technique used to convert from gray to black and white pixels.                 1 = No dithering or halftoning has been applied to the image data.  2 = An ordered dither or halftone technique has been applied to the image data.  3 = A randomized process such as error diffusion has been applied to the image data.  Default is Threshholding = 1. See also CellWidth, CellLength.  ",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"DigitalProcess",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"ImageContainer","ImageContainerT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"ImageContainerT defines the different elements that describe how the image is stored in a compressed stream.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"ActualImageHeight","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"(Deprecated) The compressed image height applies to compressed images and represents the valid height of the meaningful image.",
"(Deprecated) The compressed image height applies to compressed images and represents the valid height of the meaningful image. An uncompressed image will have no value for this field.",
#endif
"ActualImageWidth","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"(Deprecated) The compressed image width applies to compressed images and represents the valid width of the meaningful image. ",
"(Deprecated) The compressed image width applies to compressed images and represents the valid width of the meaningful image.  An uncompressed image will have no value for this field.",
#endif
"BitsPerSample","vuint16",(char *)11,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of bits per image component (pixel).",
"The number of bits per image component (pixel).",
#endif
"ColorSpaceInformation","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The color space information records the color space specified.  ",
"The color space information records the color space specified.  ",
#endif
"ComponentsConfiguration","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The compressed image data channels of each component are arranged in order from the 1st component to the 4th.",
"The compressed image data channels of each component are arranged in order from the 1st component to the 4th. For uncompressed data the data arrangement is given in the PhotometricInterpretation.",
#endif
"CompressedBitsPerPixel","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The compressed image compression mode indicated in unit bits per pixel.",
"The compressed image compression mode indicated in unit bits per pixel.",
#endif
"Compression","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The compression scheme used for the image data.",
"The compression scheme used for the image data.",
#endif
"DisplayResolutionUnit","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The unit for measuring Xresolution and Yresolution.",
"The unit for measuring Xresolution and Yresolution. The values are defined as follows:                 2 = inches  3 = centimeters  Others = reserved  ",
#endif
"DisplayXResolution","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of pixels per ResolutionUnit in the Width direction.",
"The number of pixels per ResolutionUnit in the Width direction.",
#endif
"DisplayYResolution","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of pixels per ResolutionUnit in the Height direction. ",
"The number of pixels per ResolutionUnit in the Height direction. ",
#endif
"ExifVersion","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The version of Exif Standard.",
"The version of Exif Standard.",
#endif
"FlashpixVersion","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The version of the Flashpix Specification.",
"The version of the Flashpix Specification.",
#endif
"Gamma","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object indicates the value of coefficient gamma.",
"This metadata object indicates the value of coefficient gamma. The formula of transfer function used for image reproduction is expressed as follows: (Reproduced value) = (Input value)^gamma  Both reproduced value and input value indicate normalized value, whose minimum value is 0 and maximum value is 1.",
#endif
"Height","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of rows of image data.",
"The number of rows of image data.",
#endif
"IPTC_NAA","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This element encodes relevent information concerning the image that is useful for newspaper photographs.",
"This element encodes relevent information concerning the image that is useful for newspaper photographs. The information is defined in Application Record No. 2 of the IPTC-NAA (International Press Telecommunications Council-Newspaper Association of America) Information Interchange Model and Digital NewsphotoParameter Record, Version 2, April 14 1993. (Note: the record contains capture condition, intellectual property, scene content, etc. information)  (See TIFF/EP spec)",
#endif
"ImageUniqueID","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field indicates an identifier assigned uniquely to each image.",
"This field indicates an identifier assigned uniquely to each image. It is recorded as an ASCII string equivalent to hexadecimal notation and 128-bit fixed length.",
#endif
"Interoperability","InteroperabilityT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"InteroperabilityT defines the different elements regarding interoperability.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Index","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates the identification of the Interoperability rule.",
"Indicates the identification of the Interoperability rule. The following rules are defined. Four bytes used including the termination code (NULL). \"R98\" = Indicates a file conforming to R98 file specification of Recommended Exif Interoperability Rules (ExifR98) or to DCF basic file stipulated by Design Rule for Camera File System. \"THM\" = Indicates a file conforming to DCF thumbnail file stipulated by Design rule for Camera File System. \"R03\" = Indicates a file conforming to DCF Option File (Adobe RGB color space) stipulated by Design rule for Camera File System.",
#endif
"Version","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object records the Interoperability version.",
"",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Interoperability",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"MetadataNumber","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The version of the metadata in the APP3 segment.",
"The version of the metadata in the APP3 segment.",
#endif
"Movie","MovieT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"MovieT defines the different elements regarding moving images.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Duration","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates the longest track duration in seconds.",
"Indicates the longest track duration in seconds.",
#endif
"FrameRate","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object records the number of frames per second.",
"This metadata object records the number of frames per second.",
#endif
"FrameCount","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object records the number of frames.",
"This metadata object records the number of frames.",
#endif
"SoundSampleRate","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object records the number of sound samples per second.",
"This metadata object records the number of sound samples per second.",
#endif
"SpecialHardwareAndSoftwareRequirements","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object records any specialized hardware and software requirements for playback.",
"This metadata object records any specialized hardware and software requirements for playback.",
#endif
"TimeScale","int",(char *)12,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object records the number of time units per second.",
"This metadata object records the number of time units per second.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Movie",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Orientation","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The image orientation viewed in terms of rows and columns.",
"The image orientation viewed in terms of rows and columns.",
#endif
"PhotometricInterpretation","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The pixel composition.(SRGB, YcbCr, etc.)",
"The pixel composition.(SRGB, YcbCr, etc.)",
#endif
"PlanarConfiguration","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates whether pixel components are recorded in chunky or planar format.",
"Indicates whether pixel components are recorded in chunky or planar format.",
#endif
"PrimaryChromaticities","vfloat",(char *)17,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The chromaticity of the three primary colors of the image.  ",
"The chromaticity of the three primary colors of the image.  ",
#endif
"ReferenceBlackWhite","vfloat",(char *)17,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The reference black point value and reference white point value.",
"The reference black point value and reference white point value.",
#endif
"RowsPerStrip","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of rows per strip when an image is divided into strips.",
"The number of rows per strip when an image is divided into strips.",
#endif
"SamplesPerPixel","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of components per pixel.",
"The number of components per pixel.",
#endif
"SoundType","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"0 = WAV",
"0 = WAV",
#endif
"SourceImageDirectory","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field contains a Unicode string which records the Sources Image Directory.",
"This field contains a Unicode string which records the Sources Image Directory.",
#endif
"SourceImageFileName","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field contains a Unicode string which records the Sources Image File Name.",
"This field contains a Unicode string which records the Sources Image File Name.",
#endif
"SourceImageVolumeName","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field contains a Unicode string which records the Sources Image Volume Name.",
"This field contains a Unicode string which records the Sources Image Volume Name.",
#endif
"StripByteCounts","vuint32",(char *)15,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The total number of bytes in each strip.",
"The total number of bytes in each strip.",
#endif
"StripOffsets","vuint32",(char *)15,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"For each strip, the byte offset of that strip.",
"For each strip, the byte offset of that strip.",
#endif
"SubIfdPointers","vuint32",(char *)15,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"",
"",
#endif
"Thumbnail","ThumbnailT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"ThumbnailT defines the different elements that describe JPEG compressed thumbnail data.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"BitsPerSample","vuint16",(char *)11,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of bits per image component (pixel).",
"The number of bits per image component (pixel).",
#endif
"BytesOfJPEGData","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field contains the number of bytes of JPEG compressed thumbnail data.",
"This field contains the number of bytes of JPEG compressed thumbnail data.",
#endif
"Compression","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The compression scheme used for the image data.",
"The compression scheme used for the image data.",
#endif
"DisplayResolutionUnit","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The unit for measuring Xresolution and Yresolution.",
"The unit for measuring Xresolution and Yresolution. The values are defined as follows:                 2 = inches  3 = centimeters  Others = reserved  ",
#endif
"DisplayXResolution","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of pixels per ResolutionUnit in the Width direction.",
"The number of pixels per ResolutionUnit in the Width direction.",
#endif
"DisplayYResolution","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of pixels per ResolutionUnit in the Height direction. ",
"The number of pixels per ResolutionUnit in the Height direction. ",
#endif
"Height","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of rows of image data.",
"The number of rows of image data.",
#endif
"OffsetToJPEGSOI","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field contains the offset to the start byte (SOI) of JPEG compressed thumbnail data.",
"This field contains the offset to the start byte (SOI) of JPEG compressed thumbnail data.",
#endif
"Orientation","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The image orientation viewed in terms of rows and columns.",
"The image orientation viewed in terms of rows and columns.",
#endif
"PhotometricInterpretation","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The pixel composition.(SRGB, YcbCr, etc.)",
"The pixel composition.(SRGB, YcbCr, etc.)",
#endif
"PlanarConfiguration","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates whether pixel components are recorded in chunky or planar format.",
"Indicates whether pixel components are recorded in chunky or planar format.",
#endif
"RowsPerStrip","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of rows per strip when an image is divided into strips.",
"The number of rows per strip when an image is divided into strips.",
#endif
"SamplesPerPixel","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of components per pixel.",
"The number of components per pixel.",
#endif
"StripByteCounts","vuint32",(char *)15,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The total number of bytes in each strip.",
"The total number of bytes in each strip.",
#endif
"StripOffsets","vuint32",(char *)15,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"For each strip, the byte offset of that strip.",
"For each strip, the byte offset of that strip.",
#endif
"Width","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of columns of image data, equal to the number of pixels per row. ",
"The number of columns of image data, equal to the number of pixels per row. ",
#endif
"YCbCrCoefficients","vfloat",(char *)17,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The matrix coefficients for transformation from RGB to YCbCr image data.",
"The matrix coefficients for transformation from RGB to YCbCr image data.",
#endif
"YCbCrPositioning","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The position of chrominance components in relation to the luminance component.",
"The position of chrominance components in relation to the luminance component.",
#endif
"YCbCrSubSampling","vuint16",(char *)11,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The sampling ratio of chrominance components in relation to the luminance component.",
"The sampling ratio of chrominance components in relation to the luminance component.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Thumbnail",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"TiffepVersion","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object encodes the version of this TIFF/EP file as a four tier revision number, for example 1.0.0.0.",
"This metadata object encodes the version of this TIFF/EP file as a four tier revision number, for example 1.0.0.0. This revision number has the form of w.x.y.z where w=0-255, x=0-255, y=0-255, and z=0-255. The purpose of this tag is to allow a TIFF/EP compliant file to identify itself to a TIFF/EP aware reader.",
#endif
"TransferFunction","vuint16",(char *)11,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A tabular style transfer function for the image.",
"A tabular style transfer function for the image.",
#endif
"TransferRange","vuint16",(char *)11,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Expands the range of the TransferFunction.",
"Expands the range of the TransferFunction. The first value within a pair is associated with TransferBlack and the second isassociated with TransferWhite. The ordering ofpairs is the same as for pixel components of the PhotometricInterpretation type.By default, theTransferFunction is defined over a range from a minimum intensity, 0 or nominal black, to a maximum intensity,(BitsPerSample) - 1 or nominal white. Kodak PhotoYCC uses an extended range TransferFunction in order to describe highlights,saturated colors and shadow detail beyond this range. The TransferRange expands the TransferFunction to support these values.It is defined only for RGB and YCbCr PhotometricInterpretations. For more details, see TIFF 6.0 Specification.             ",
#endif
"WhitePoint","vfloat",(char *)17,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The chromaticity of the white point of the image.",
"The chromaticity of the white point of the image.",
#endif
"Width","unsignedInt",(char *)14,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The number of columns of image data, equal to the number of pixels per row. ",
"The number of columns of image data, equal to the number of pixels per row. ",
#endif
"YCbCrCoefficients","vfloat",(char *)17,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The matrix coefficients for transformation from RGB to YCbCr image data.",
"The matrix coefficients for transformation from RGB to YCbCr image data.",
#endif
"YCbCrPositioning","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The position of chrominance components in relation to the luminance component.",
"The position of chrominance components in relation to the luminance component.",
#endif
"YCbCrSubSampling","vuint16",(char *)11,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The sampling ratio of chrominance components in relation to the luminance component.",
"The sampling ratio of chrominance components in relation to the luminance component.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"ImageContainer",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"IntellectualProperty","IntellectualPropertyT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"IntellectualPropertyT defines the different elements that identify the photographer and the editor copyright information. ",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Copyright","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A character string that identifies both the photographer and editor Copyright information.",
"A character string that identifies both the photographer and editor Copyright information.",
#endif
"ImageCreator","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A character string that identifies both the photographer and editor Copyright information.",
"A character string that identifies both the photographer and editor Copyright information.",
#endif
"Photographer","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A character string that identifies the name of the photographer.",
"A character string that identifies the name of the photographer.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"IntellectualProperty",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"OutputOrder","OutputOrderT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"OutputOrderT defines the different elements pertaining to the Output Order information.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Information","OutputOrderInformationT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"OutputOrderInformationT defines the different elements pertaining to Dealer ID Number, Envelope Number, and Vendor Order Number",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"DealerIDNumber","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The Dealer ID Number data item contents a Dealer ID Number assigned by the industry.",
"The Dealer ID Number data item contents a Dealer ID Number assigned by the industry.",
#endif
"EnvelopeNumber","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The Envelope Number data item contains the 6 least significant digits of the number printed on the order envelope.",
"The Envelope Number data item contains the 6 least significant digits of the number printed on the order envelope.",
#endif
"VendorOrderNumber","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Identification number of an order and unique to a vendor.",
"Identification number of an order and unique to a vendor.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Information",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"SimpleRenderInst","OutputOrderSimpleRenderInstT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"OutputOrderSimpleRenderInstT defines the element Frame Number. ",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"FrameNumber","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The Frame Number data item contains the number of the current frame.",
"The Frame Number data item contains the number of the current frame. Valid numbers are 01 through 40.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"SimpleRenderInst",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"OutputOrder",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"SceneContent","SceneContentT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"SceneContentT defines the different elements associated with the Scene Content. Included is GPS information, scene information, and any audio information related to the image.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Audio","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This is the entry to special audio API of the EXIF toolkit.",
"This is the entry to special audio API of the EXIF toolkit. Needs to change.",
#endif
"GPS","GPST",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"GPST defines the different elements pertaining to the Global Positioning System.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Altitude","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Altitude indicates the altitude based on the reference in AltitudeRef.",
"Altitude indicates the altitude based on the reference in AltitudeRef. Altitude is expressed as one RATIONAL value. The reference unit is meters. ",
#endif
"AltitudeRef","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"AltitudeRef indicates the altitude used as the reference altitude.",
"AltitudeRef indicates the altitude used as the reference altitude. The reference unit is meters. The defined values are:                 0 = Sea level  Others = reserved  ",
#endif
"AreaInformation","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field contains a character string recording the name of the GPS area. ",
"This field contains a character string recording the name of the GPS area. ",
#endif
"DateStamp","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field contains a character string recording date and time information relative to UTC (Coordinated Universal Time).",
"This field contains a character string recording date and time information relative to UTC (Coordinated Universal Time). The format is \"YYYY:MM:DD.\"",
#endif
"DOP","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"DOP indicates the GPS DOP (data degree of precision).",
"DOP indicates the GPS DOP (data degree of precision). An HDOP value is written during two-dimensional measurement and PDOP during three-dimensional measurement.",
#endif
"DestBearing","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"DestBearing indicates the bearing to the destination point.",
"DestBearing indicates the bearing to the destination point. The range of values is from 0.00 to 359.99.",
#endif
"DestBearingRef","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"DestBearingRef indicates the reference used for giving the bearing to the destination point.",
"DestBearingRef indicates the reference used for giving the bearing to the destination point. The defined values are:                 'T' = True direction  'M' = Magnetic direction  Others = reserved  ",
#endif
"DestDistance","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"DestDistance indicates the distance to the destination point.",
"DestDistance indicates the distance to the destination point.",
#endif
"DestDistanceRef","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"DestDistanceRef indicates the unit used to express the distance to the destination point.",
"DestDistanceRef indicates the unit used to express the distance to the destination point. The defined values are:                 'K' = Kilometers  'M' = Miles  'N' = Knots  Others = reserved.  ",
#endif
"DestLatitude","vfloat",(char *)17,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"DestLatitude indicates the latitude of the destination point.",
"DestLatitude indicates the latitude of the destination point. The latitude is expressed as three RATIONAL values giving the degrees, minutes, and seconds, respectively. When degrees, minutes and seconds are expressed, the format is dd/1,mm/1,ss/1. When degrees and minutes are used and, for example, fractions of minutes are given up to two decimal places, the format is dd/1,mmmm/100,0/1.",
#endif
"DestLatitudeRef","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"DestLatitudeRef indicates whether the latitude of the destination point is north or south latitude.",
"DestLatitudeRef indicates whether the latitude of the destination point is north or south latitude. The defined values are:                 'N' = North latitude  'S' = Sourth latitude  Others = reserved.  ",
#endif
"DestLongitude","vfloat",(char *)17,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"DestLongitude indicates the longitude of the destination point.",
"DestLongitude indicates the longitude of the destination point. The longitude is expressed as three RATIONAL values giving the degrees, minutes, and seconds, respectively. When degrees, minutes and seconds are expressed, the format is ddd/1,mm/1,ss/1. When degrees and minutes are used and, for example, fractions of minutes are given up to two decimal places, the format is ddd/1,mmmm/100,0/1.",
#endif
"DestLongitudeRef","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"DestLongitudeRef Indicates whether the longitude of the destination point is east or west longitude.",
"DestLongitudeRef Indicates whether the longitude of the destination point is east or west longitude. The defined values are:                 'E' = East longitude  'W' = West longitude  Others = reserved  ",
#endif
"Differential","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field Indicates whether differential correction is applied to the GPS receiver.",
"This field Indicates whether differential correction is applied to the GPS receiver.                 0 = Measurement without differential correction  1 = Differential correction applied  ",
#endif
"ImgDirection","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"ImgDirection indicates the direction of the image when it was captured.",
"ImgDirection indicates the direction of the image when it was captured. The range of values is from 0.00 to 359.99. ",
#endif
"ImgDirectionRef","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"ImgDirectionRef indicates the reference for giving the direction of the image when it is captured.",
"ImgDirectionRef indicates the reference for giving the direction of the image when it is captured. The defined values are:                 'T' = True direction  'M' = Magnetic direction  Others = reserved  ",
#endif
"Latitude","vfloat",(char *)17,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Latitude indicates the latitude. The latitude is expressed as three RATIONAL values giving the degrees, minutes, and seconds, respectively.",
"Latitude indicates the latitude. The latitude is expressed as three RATIONAL values giving the degrees, minutes, and seconds, respectively. When degrees, minutes and seconds are expressed, the format is dd/1,mm/1,ss/1. When degrees and minutes are used and, for example, fractions of minutes are given up to two decimal places, the format is dd/1,mmmm/100,0/1.",
#endif
"LatitudeRef","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"LatitudeRef indicates whether the latitude is north or south latitude.",
"LatitudeRef indicates whether the latitude is north or south latitude. The defined values are:                 'N' = North latitude  'S' = South latitude  Others = reserved  ",
#endif
"Longitude","vfloat",(char *)17,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Longitude indicates the longitude.",
"Longitude indicates the longitude. The longitude is expressed as three RATIONAL values giving the degrees, minutes, and seconds, respectively. When degrees, minutes and seconds are expressed, the format is dd/1,mm/1,ss/1. When degrees and minutes are used and, for example, fractions of minutes are given up to two decimal places, the format is dd/1,mmmm/100,0/1.  ",
#endif
"LongitudeRef","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"LongitudeRef indicates whether the longitude is east or west longitude.",
"LongitudeRef indicates whether the longitude is east or west longitude. The defined values are:                 'E' = East longitude  'W' = West longitude  Others = reserved  ",
#endif
"MapDatum","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"MapDatum indicates the geodetic survey data used by the GPS receiver.",
"MapDatum indicates the geodetic survey data used by the GPS receiver. If the survey data is restricted to Japan, the value of this tag is 'TOKYO' or 'WGS-84'. If a GPS Info is recorded, it is strongly recommended that MapDatum be recorded.",
#endif
"MeasureMode","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"MeasureMode Indicates the GPS measurement mode.",
"MeasureMode Indicates the GPS measurement mode. The defined values are:                 '2' = two-dimensional measurement  '3' = three-dimensional measurement is in progress.  Others = reserved.  ",
#endif
"ProcessingMethod","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This field contains a character string recording the name of the method used for location finding.",
"This field contains a character string recording the name of the method used for location finding. ",
#endif
"Satellites","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Satellites indicates the GPS satellites used for measurements.",
"Satellites indicates the GPS satellites used for measurements. This metadata object can be used to describe the number of satellites, their ID number, angle of elevation, azimuth, SNR and other information in ASCII notation.",
#endif
"Speed","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Speed indicates the speed of GPS receiver movement.",
"Speed indicates the speed of GPS receiver movement.",
#endif
"SpeedRef","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"SpeedRef indicates the unit used to express the GPS receiver speed of movement.",
"SpeedRef indicates the unit used to express the GPS receiver speed of movement. The defined values are:                 'K' = Kilometers per hour  'M' = Miles per hour  'N' = Knots  Others = reserved  ",
#endif
"Status","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Status indicates the status of the GPS receiver when the image is recorded.",
"Status indicates the status of the GPS receiver when the image is recorded. The defined values are:                 'A' = measurement is in progress  'V' = the measurement is interrupted.   Others = reserved.  ",
#endif
"TimeStamp","vfloat",(char *)17,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"TimeStamp indicates the time as UTC (Coordinated Universal Time).",
"TimeStamp indicates the time as UTC (Coordinated Universal Time). TimeStamp is expressed as three RATIONAL values giving the hour, minute, and second.",
#endif
"Track","float",(char *)16,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates the direction of GPS receiver movement.",
"Indicates the direction of GPS receiver movement. The range of values is from 0.00 to 359.99.",
#endif
"TrackRef","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"TrackRef indicates the reference for giving the direction of GPS receiver movement. ",
"TrackRef indicates the reference for giving the direction of GPS receiver movement. The defined values are:                 'T' = True direction  'M' = Magnetic direction  Others = reserved  ",
#endif
"VersionID","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"VersionID indicates the version of InfoIFD.",
"VersionID indicates the version of InfoIFD. The version is given as a string.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"GPS",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"GroupCaption","GroupCaptionT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"GroupCaptionT contains various elements which define different titles for a group of images.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"GroupCaption","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This is a user selectable title for a group of images.",
"This is a user selectable title for a group of images.",
#endif
"UserInputGroupTitle","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A title assigned to a group or collection of images. The group title has been selected from a predefined collection.",
"A title assigned to a group or collection of images. The group title has been selected from a predefined collection.",
#endif
"UserSelectGroupTitle","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This is a user selectable title for a group of images.",
"This is a user selectable title for a group of images.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"GroupCaption",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"ImageCaptureDateTime","ImageCaptureDateTimeT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"ImageCaptureDateTimeT defines the elements that describe the date, time, and time zone of the original image capture.",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"ImageCaptureDateTime","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The date and time of the original Image Scene.",
"The date and time of the original Image Scene. The format is \"YYYY:MM:DD HH:MM:SS.TH\" with time shown in 24-hour format including fractions of a second, and the date and time separated by one blank character [20.H]. When the date and time are unknown, all the character spaces except colons (\":\") may be filled with blank characters, or else the entire field may be filled with blank characters. The character string length is 20 bytes including NULL for termination. When the field is left blank, it is treated as unknown.",
#endif
"ImageCaptureSubSecondTime","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The recorded fractions of seconds for the date and time when the image was captured.",
"The recorded fractions of seconds for the date and time when the image was captured.",
#endif
"TimeZoneOffset","vint16",(char *)9,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This metadata object encodes the time zone of the camera clock (relative to Greenwich Mean Time) used to create the DataTimeOriginal tag-value when the picture was taken.",
"This metadata object encodes the time zone of the camera clock (relative to Greenwich Mean Time) used to create the DataTimeOriginal tag-value when the picture was taken. It may also contain the time zone offset of the clock used to create the DateTime tag-value when the image was modified.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"ImageCaptureDateTime",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"ImageDescription","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A character string giving the title of the image.",
"A character string giving the title of the image.",
#endif
"RelatedSoundFile","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A character string that identifies an audio file that is related to the image data.",
"A character string that identifies an audio file that is related to the image data. The only relational information recorded here is the Exif audio file name and extension (an ASCII string consisting of 8 characters plus '.' plus 3 characters). The path is not recorded.",
#endif
"Faces","SceneObjectsT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The Scene Object -- a type, an ID, and a list of features",
" The Scene Object -- a type (such as 'face'), an ID (usually a GUID), a reference (usually another object's ID) and a list of features.         ",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"DetectionAlgorithmID","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The detection algorithm ID",
"A UUID (the four - that separate the five different fields are ignored) or GUID of the detection algorithm used to detect the objects. The unique ID that identifies the algorithm name and version is a 128-bit value where each nibble (4 bits) is represented by an ASCII character.             ",
#endif
"RecognitionAlgorithmID","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The recognition algorithm ID",
"A UUID (the four - that separate the five different fields are ignored) or GUID of the recognition algorithm used to recognise the objects. The unique ID that identifies the algorithm name and version is a 128-bit value where each nibble (4 bits) is represented by an ASCII character.             ",
#endif
"CartesianZeroPoint","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Indicates the zero, zero point of the Cartesian coordinate in the image pixel plan.",
"Indicates the zero, zero point of the Cartesian coordinate in the image pixel plan.The expected values are enumerated as follows:                   1 Upper Left-hand corner  2 Lower Left-hand corner  3 Upper right-hand corner  4 Center of the image  Values 0 and 5 through 65535 are reserved for future definitions.             ",
#endif
"Object","SceneObjectT",(char *)2147483649,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The Scene Object -- a type, an ID, and a list of features",
" The Scene Object -- a type (such as 'face'), an ID (usually a GUID), a reference (usually another object's ID) and a list of features.         ",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Type","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"",
"",
#endif
"ID","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"",
"",
#endif
"Name","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"",
"",
#endif
"Reference","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"",
"",
#endif
"Feature","SceneObjectFeatureT",(char *)2147483649,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The data defining a feature",
"The points and information defining a feature. The name describes the feature (mouth, nose, etc.)  A string value may be used when required by the feature type.  Possible values for the list type are:             'Outline' = A connected outline of the feature  'Path' = A connected outline of the feature where the beginning and end points are not connected  'List' = A list (1 or more) of points describing the feature  ",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Type","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"",
"",
#endif
"Data","vfloat",(char *)17,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"",
"",
#endif
"Value","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"",
"",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Feature[1]",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Object[1]",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Faces",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"SceneType","unsignedByte",(char *)6,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The SceneType indicates the type of scene.",
"The SceneType indicates the type of scene. The values defined are as follows:                 1 = A directly photographed image  Others = reserved  ",
#endif
"SoundCaptureDateTime","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Same as the value of DateTime Original Tag. The date and time of the original Image Scene.",
"Same as the value of DateTime Original Tag. The date and time of the original Image Scene. The format is \"YYYY:MM:DD HH:MM:SS.TH\" with time shown in 24-hour format including fractions of a second, and the date and time separated by one blank character [20.H]. When the date and time are unknown, all the character spaces except colons (\":\") may be filled with blank characters, or else the entire field may be filled with blank characters. The character string length is 20 bytes including NULL for termination. When the field is left blank, it is treated as unknown.",
#endif
"SubjectLocation","vuint16",(char *)11,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Subject Location indicates the location of the main subject in the scene.",
"Subject Location indicates the location of the main subject in the scene. The value this item represents the pixel at the center of the main subject relative to the left edge, prior to rotation processing as per the Rotation tag. The first value is the X column number and second indicates the Y row number.",
#endif
"UserComment","UserCommentT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"UserCommentT defines the different elements that describe the title and information about an image. ",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"MS","MSCommentT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"MSCommentT defines the different elements that MS added to EXIF files. ",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Author","vuint8",(char *)2147483655,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The tag value indicates the person or organization primarily responsible for creating the image resource.",
"The tag value indicates the person or organization primarily responsible for creating the image resource.  The data for this tag is encoded using Unicode.",
#endif
"Comments","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The tag value indicates composed annotations of the image resource.",
"The tag value indicates composed annotations of the image resource. The data for this tag is encoded using Unicode.",
#endif
"Keywords","vuint8",(char *)2147483655,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The tag value indicates a significant or descriptive word(s) or key phrases or classification codes that describe the topic of the image resource.",
"The tag value indicates a significant or descriptive word(s) or key phrases or classification codes that describe the topic of the image resource.  Preferably, a semicolon separates keywords.  A comma may be used but is less preferable.  The data for this tag is encoded using Unicode.",
#endif
"PhotoRating","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The tag value indicates the rating of the image.",
"The tag value indicates yet another rating of the image. The rating can vary from 0 to 100.",
#endif
"Rating","unsignedShort",(char *)10,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The tag value indicates the rating of the image.",
"The tag value indicates the rating of the image. The rating can vary from 0 stars unassigned to 5 stars the highest rating.",
#endif
"Subject","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The tag value indicates the topic of the image resource.",
"The tag value indicates the topic of the image resource.  The data for this tag is encoded using Unicode.",
#endif
"Title","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"The tag indicates the name by which the image resource is formally known.",
"The tag indicates the name by which the image resource is formally known. This name will be used to search for this resource. Therefore the name must be as meaningful and descriptive as possible.  The data for this tag is encoded using Unicode.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"MS",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"UserComment","vuint8",(char *)7,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"A character string that identifies from a user perspective, any desired information about the image.",
"A character string that identifies from a user perspective, any desired information about the image. This is in addition to any comments identified in ImageDescription metadata. The first 8 bytes identify a character code ID field.  Current registered Character Code Ids are ASCI, JIS and Unicode.",
#endif
"UserInputFrameTitle","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This is a user input title for an image.",
"This is a user input title for an image.",
#endif
"UserSelectFrameTitle","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"This is a user selectable title for an image.",
"This is a user selectable title for an image.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"UserComment",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"SceneContent",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"XMP","XMPT",(char *)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"XMPT defines the different elements associated with XMP",
"",
#endif
"xsd:sequence",NULL,(char*)1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"Document","string",(char *)20,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
"Any embedded XMP information in RDF/XML format.",
"A string containing the entire XMP document embedded in the file.",
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"XMP",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"xsd:sequence",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
"PmtRootElement",NULL,(char*)-1,
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
NULL,NULL,
#endif
};

#endif

