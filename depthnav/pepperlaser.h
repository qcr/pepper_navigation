#ifndef PEPPERLASER
#define PEPPERLASER

#include <qi/anyobject.hpp>
#include <al/from_any_value.hpp>
#include <opencv2/opencv.hpp>
static const char* laserMemoryKeys[] = {
  // RIGHT LASER
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg01/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg01/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg02/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg02/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg03/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg03/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg04/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg04/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg05/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg05/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg06/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg06/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg07/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg07/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg08/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg08/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg09/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg09/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg10/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg10/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg11/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg11/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg12/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg12/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg13/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg13/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg14/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg14/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg15/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg15/Y/Sensor/Value",
  // FRONT LASER
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg01/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg01/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg02/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg02/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg03/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg03/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg04/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg04/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg05/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg05/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg06/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg06/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg07/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg07/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg08/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg08/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg09/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg09/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg10/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg10/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg11/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg11/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg12/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg12/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg13/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg13/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg14/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg14/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg15/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg15/Y/Sensor/Value",
  // LEFT LASER
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg01/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg01/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg02/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg02/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg03/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg03/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg04/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg04/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg05/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg05/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg06/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg06/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg07/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg07/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg08/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg08/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg09/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg09/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg10/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg10/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg11/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg11/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg12/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg12/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg13/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg13/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg14/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg14/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg15/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg15/Y/Sensor/Value",
};

static const char* laserMemoryKeysUCOD[] = {
  // RIGHT LASER
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg01/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg01/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg02/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg02/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg03/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg03/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg04/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg04/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg05/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg05/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg06/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg06/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg07/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg07/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg08/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg08/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg09/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg09/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg10/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg10/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg11/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg11/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg12/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg12/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg13/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg13/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg14/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg14/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg15/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/UncertainObstacle/Seg15/Y/Sensor/Value",
  // FRONT LASER
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg01/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg01/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg02/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg02/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg03/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg03/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg04/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg04/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg05/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg05/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg06/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg06/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg07/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg07/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg08/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg08/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg09/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg09/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg10/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg10/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg11/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg11/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg12/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg12/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg13/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg13/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg14/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg14/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg15/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/UncertainObstacle/Seg15/Y/Sensor/Value",
  // LEFT LASER
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg01/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg01/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg02/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg02/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg03/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg03/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg04/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg04/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg05/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg05/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg06/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg06/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg07/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg07/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg08/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg08/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg09/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg09/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg10/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg10/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg11/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg11/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg12/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg12/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg13/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg13/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg14/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg14/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg15/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/UncertainObstacle/Seg15/Y/Sensor/Value",
};

static const char* laserMemoryKeysARD[] = {
  // RIGHT LASER
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg01/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg01/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg02/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg02/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg03/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg03/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg04/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg04/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg05/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg05/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg06/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg06/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg07/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg07/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg08/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg08/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg09/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg09/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg10/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg10/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg11/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg11/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg12/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg12/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg13/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg13/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg14/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg14/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg15/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/AnnoyingReflection/Seg15/Y/Sensor/Value",
  // FRONT LASER
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg01/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg01/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg02/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg02/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg03/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg03/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg04/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg04/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg05/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg05/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg06/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg06/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg07/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg07/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg08/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg08/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg09/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg09/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg10/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg10/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg11/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg11/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg12/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg12/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg13/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg13/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg14/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg14/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg15/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/AnnoyingReflection/Seg15/Y/Sensor/Value",
  // LEFT LASER
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg01/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg01/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg02/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg02/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg03/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg03/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg04/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg04/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg05/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg05/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg06/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg06/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg07/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg07/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg08/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg08/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg09/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg09/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg10/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg10/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg11/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg11/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg12/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg12/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg13/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg13/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg14/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg14/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg15/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/AnnoyingReflection/Seg15/Y/Sensor/Value",
};

static const char* RightlaserMemoryKeys[] = {
  // RIGHT LASER
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg01/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg01/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg02/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg02/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg03/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg03/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg04/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg04/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg05/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg05/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg06/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg06/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg07/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg07/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg08/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg08/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg09/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg09/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg10/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg10/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg11/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg11/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg12/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg12/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg13/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg13/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg14/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg14/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg15/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Right/Horizontal/Seg15/Y/Sensor/Value",
};

static const char* FrontlaserMemoryKeys[] = {
  // FRONT LASER
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg01/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg01/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg02/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg02/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg03/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg03/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg04/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg04/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg05/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg05/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg06/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg06/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg07/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg07/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg08/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg08/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg09/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg09/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg10/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg10/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg11/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg11/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg12/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg12/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg13/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg13/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg14/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg14/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg15/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Front/Horizontal/Seg15/Y/Sensor/Value",
};
static const char* LeftlaserMemoryKeys[] = {
  // LEFT LASER
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg01/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg01/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg02/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg02/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg03/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg03/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg04/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg04/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg05/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg05/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg06/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg06/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg07/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg07/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg08/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg08/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg09/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg09/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg10/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg10/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg11/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg11/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg12/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg12/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg13/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg13/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg14/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg14/Y/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg15/X/Sensor/Value",
  "Device/SubDeviceList/Platform/LaserSensor/Left/Horizontal/Seg15/Y/Sensor/Value",
};

/*

void getLaserData(qi::AnyObject memeoryproxy,std::vector<double> &obsdist)
{

for(int i=0;i<61;i++)
    obsdist[i]=-1;

  static const std::vector<std::string> laser_keys_value(laserMemoryKeys, laserMemoryKeys+90);

  std::vector<float> result_value;

  try {
      qi::AnyValue anyvalues = memeoryproxy.call<qi::AnyValue>("getListData", laser_keys_value);
      naoqi::tools::fromAnyValueToFloatVector(anyvalues, result_value);
    } catch (const std::exception& e) {
    std::cerr << "Exception caught in LaserConverter: " << e.what() << std::endl;
    return;
  }


   int pos = 0;

   double theta = 1.757;
theta = 1.5708;
  // ranges between 0-29
  for( size_t i=0; i<30; i=i+2, ++pos)
  {
    const float lx = result_value[28-i]; // segments are internally flipped
    const float ly = result_value[28-i+1]; // segments are internally flipped
    float bx = lx*std::cos(-theta) - ly*std::sin(-theta) - 0.018;
    float by = lx*std::sin(-theta) + ly*std::cos(-theta) - 0.0899;
    float dist = std::sqrt( std::pow(bx,2) + std::pow(by,2) );
    //float dist = std::sqrt( std::pow(lx,2) + std::pow(ly,2) );
   // std::cout << "got a distance at "<< pos << " with "  << dist << std::endl;
    obsdist[pos]= dist;
  }

  pos = pos+8; // leave out 8 blanks ==> pos = 15+8

  // ranges between 30-59
  for( size_t i=0; i<30; i=i+2, ++pos)
  {
    const float lx = result_value[58-i];
    const float ly = result_value[58-i+1];
    float bx = lx + 0.0562 ;
    float by = ly;
    float dist = std::sqrt( std::pow(bx,2) + std::pow(by,2) );
    //float dist = std::sqrt( std::pow(lx,2) + std::pow(ly,2) );
    //std::cout << "got a distance at "<< pos << " with "  << dist << std::endl;
     obsdist[pos]= dist;
  }

  pos = pos+8; // leave out again 8 blanks ==> pos = 15+8+15+8

  for( size_t i=0; i<30; i=i+2, ++pos)
  {
    const float lx = result_value[88-i];
    const float ly = result_value[88-i+1];
    float bx = lx*std::cos(theta) - ly*std::sin(theta) - 0.018;
    float by = lx*std::sin(theta) + ly*std::cos(theta) + 0.0899;
    float dist = std::sqrt( std::pow(bx,2) + std::pow(by,2) );
    //float dist = std::sqrt( std::pow(lx,2) + std::pow(ly,2) );
    //std::cout << "got a distance at "<< pos << " with "  << dist << std::endl;
    obsdist[pos]= dist;
  }


}

void getLaserData(qi::AnyObject memeoryproxy,std::vector<double> &X,std::vector<double> &Y)
{

for(int i=0;i<61;i++){
    X[i]=0;
    Y[i]=0;
}

  static const std::vector<std::string> laser_keys_value(laserMemoryKeys, laserMemoryKeys+90);

  std::vector<float> result_value;

  try {
      qi::AnyValue anyvalues = memeoryproxy.call<qi::AnyValue>("getListData", laser_keys_value);
      naoqi::tools::fromAnyValueToFloatVector(anyvalues, result_value);
    } catch (const std::exception& e) {
    std::cerr << "Exception caught in LaserConverter: " << e.what() << std::endl;
    return;
  }


   int pos = 0;
double theta = 1.757;
theta = 1.5708;
  // ranges between 0-29
  for( size_t i=0; i<30; i=i+2, ++pos)
  {
    const float lx = result_value[28-i]; // segments are internally flipped
    const float ly = result_value[28-i+1]; // segments are internally flipped
    float bx = lx*std::cos(-theta) - ly*std::sin(-theta) - 0.018;
    float by = lx*std::sin(-theta) + ly*std::cos(-theta) - 0.0889;

   X[pos]= bx;
   Y[pos]= by;
  }

  pos = pos+8; // leave out 8 blanks ==> pos = 15+8

  // ranges between 30-59
  for( size_t i=0; i<30; i=i+2, ++pos)
  {
    const float lx = result_value[58-i];
    const float ly = result_value[58-i+1];
    float bx = lx + 0.0562 ;
    float by = ly;
    //float dist = std::sqrt( std::pow(bx,2) + std::pow(by,2) );
    //float dist = std::sqrt( std::pow(lx,2) + std::pow(ly,2) );
    //std::cout << "got a distance at "<< pos << " with "  << dist << std::endl;
    X[pos]= bx;
    Y[pos]= by;
  }

  pos = pos+8; // leave out again 8 blanks ==> pos = 15+8+15+8

  for( size_t i=0; i<30; i=i+2, ++pos)
  {
    const float lx = result_value[88-i];
    const float ly = result_value[88-i+1];
    float bx = lx*std::cos(theta) - ly*std::sin(theta) - 0.018;
    float by = lx*std::sin(theta) + ly*std::cos(theta) + 0.0889;
   // float dist = std::sqrt( std::pow(bx,2) + std::pow(by,2) );
    //float dist = std::sqrt( std::pow(lx,2) + std::pow(ly,2) );
    //std::cout << "got a distance at "<< pos << " with "  << dist << std::endl;
    X[pos]= bx;
    Y[pos]= by;
  }




}



void getLaserData2(qi::AnyObject memeoryproxy,std::vector<double> &obsdist)
{

for(int i=0;i<61;i++)
    obsdist[i]=-1;

  static const std::vector<std::string> laser_keys_value(laserMemoryKeys, laserMemoryKeys+90);

  std::vector<float> result_value;

  try {
      qi::AnyValue anyvalues = memeoryproxy.call<qi::AnyValue>("getListData", laser_keys_value);
      naoqi::tools::fromAnyValueToFloatVector(anyvalues, result_value);
    } catch (const std::exception& e) {
    std::cerr << "Exception caught in LaserConverter: " << e.what() << std::endl;
    return;
  }


   int pos = 0;


  // ranges between 0-29
  for( size_t i=0; i<30; i=i+2, ++pos)
  {
    const float lx = result_value[28-i]; // segments are internally flipped
    const float ly = result_value[28-i+1]; // segments are internally flipped
    float bx = lx;
    float by = ly;
    float dist = std::sqrt( std::pow(bx,2) + std::pow(by,2) );
    //float dist = std::sqrt( std::pow(lx,2) + std::pow(ly,2) );
   // std::cout << "got a distance at "<< pos << " with "  << dist << std::endl;
    obsdist[pos]= dist;
  }

  pos = pos+8; // leave out 8 blanks ==> pos = 15+8

  // ranges between 30-59
  for( size_t i=0; i<30; i=i+2, ++pos)
  {
    const float lx = result_value[58-i];
    const float ly = result_value[58-i+1];
    float bx = lx;
    float by = ly;
    float dist = std::sqrt( std::pow(bx,2) + std::pow(by,2) );
    //float dist = std::sqrt( std::pow(lx,2) + std::pow(ly,2) );
    //std::cout << "got a distance at "<< pos << " with "  << dist << std::endl;
     obsdist[pos]= dist;
  }

  pos = pos+8; // leave out again 8 blanks ==> pos = 15+8+15+8

  for( size_t i=0; i<30; i=i+2, ++pos)
  {
    const float lx = result_value[88-i];
    const float ly = result_value[88-i+1];
    float bx = lx;//*std::cos(1.757) - ly*std::sin(1.757) - 0.018;
    float by = ly;//lx*std::sin(1.757) + ly*std::cos(1.757) + 0.090;
    float dist = std::sqrt( std::pow(bx,2) + std::pow(by,2) );
    //float dist = std::sqrt( std::pow(lx,2) + std::pow(ly,2) );
    //std::cout << "got a distance at "<< pos << " with "  << dist << std::endl;
    obsdist[pos]= dist;
  }


}*/

#endif // PEPPERLASER

