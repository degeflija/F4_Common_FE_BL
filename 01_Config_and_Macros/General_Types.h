/**
  * ###########################################################################
  * @file     General_Types.h
  * @brief    New Vario
  * @author   Horst Rupp
  * @brief    Collection of general types usable in any project
  * ###########################################################################
  */

  #include  <stddef.h>
  #include  <inttypes.h>
  #include  <stdint.h>
  #include  <limits.h>

#ifndef __General_Types_H
  #define  __General_Types_H

// ***************************************************************************
  //
  // Date Time structs
  //
  typedef struct {
    uint16_t  Year;
    uint8_t   Month;
    uint8_t   Day;
  } Date_t;

  typedef struct  {
    uint8_t   Hours;
    uint8_t   Minutes;
    uint8_t   Seconds;
  } Time_t;

  //
  // ***************************************************************************
  // Kartesische Koordinaten int32
  //
  typedef struct
  {
    int32_t  x, y;

  } point_i32_t;

  // ***************************************************************************
  // Kartesische Koordinaten int16
  //
  typedef struct
  {
    int16_t  x, y;

  } point_i16_t;

  // ***************************************************************************
  // Kartesische Koordinaten int8
  //
  typedef struct
  {
    int8_t  x, y;

  } point_i8_t;

  // ***************************************************************************
  // Kartesische Koordinaten float
  //
  typedef struct
  {
    float  x;
    float  y;

  } CartCoord_t;

  // ***************************************************************************
  // Kurs in Grad und Radian
  //
  typedef float deg_course_t;
  typedef float rad_course_t;

  // ***************************************************************************
  // Polar-Koordinaten in Degrees
  //
  typedef struct
  {
    float         Radius;
    deg_course_t  DegAngle;

  } PolDegCoord_t;

  // ***************************************************************************
  // Polar-Koordinaten in Radian
  //
  typedef struct
  {
    float         Radius;
    rad_course_t  RadAngle;

  } PolRadCoord_t;

  // ***************************************************************************
  // Geographische Koordinaten RAD float
  //
  typedef struct
  {
    float  Lat;
    float  Lon;

  } GeoCoord_t;

  // ***************************************************************************
  // Degree Minutes Seconds (DMS-Coordinates)
  //
  typedef struct
  {
    uint16_t    Degrees;
    uint8_t     Minutes;
    uint8_t     Seconds;

  } DMS_Coord_t;

  // ***************************************************************************
  // General structs
  //
  typedef union
  {
    uint32_t    u32;
    int32_t     i32;
    float       f;
    uint16_t    u16[2];
    int16_t     i16[2];
    uint8_t     u8[4];
    struct {
      uint8_t   b0;
      uint8_t   b1;
      uint8_t   b2;
      uint8_t   b3;
    };
    struct {
      uint16_t  u0;
      uint16_t  u1;
    };

  } FourBytes_t;

  // ***************************************************************************

  typedef union
  {
    uint16_t    u16;
    int16_t     i16;
    struct {
      uint8_t   LSB;
      uint8_t   MSB;
    };

  } TwoBytes_t;

#endif  // __General_Types_H
//
// *****************************************************************************
// End of File
// *****************************************************************************
