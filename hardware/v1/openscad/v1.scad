include <YAPPgenerator.scad>

pcbLength = 56;
pcbWidth = 43;
pcbThickness = 2.0;
padding = 1.0;

wallThickness = 2.0;
basePlaneThickness = 1.5;
lidPlaneThickness = 2;

baseWallHeight = 25;
lidWallHeight = 8;

ridgeHeight = 4.0; 

standoffHeight = 4.0;
standoffPinDiameter = 1.8;
standoffDiameter = 2.5;

snapJoins = [
  [10, 5, yappLeft],
  [pcbLength-10, 5, yappLeft],
  [10, 5, yappRight],
  [pcbLength-10, 5, yappRight]
];

cutoutsBack = [
[11.5, 5.5, 10, 6, 2, yappRoundedRect, yappCoordBoxInside]
];
cutoutsLid = [
  [13.5, 8.5, 29, 26, 1, yappRoundedRect]
];

pcbStands = [
  [3, 3, yappBoth, yappPin, yappAllCorners],
  
  [11, 10, 7, 24, 4, 1.8, yappDefault, yappDefault, yappLidOnly, yappHole, yappSelfThreading, yappCoordPCB, yappNoFillet],
  [45, 10, 7, 24, 4, 1.8, yappDefault, yappDefault, yappLidOnly, yappHole, yappSelfThreading, yappCoordPCB, yappNoFillet],
  [11, 33, 7, 24, 4, 1.8, yappDefault, yappDefault, yappLidOnly, yappHole, yappSelfThreading, yappCoordPCB, yappNoFillet],
  [45, 33, 7, 24, 4, 1.8, yappDefault, yappDefault, yappLidOnly, yappHole, yappSelfThreading, yappCoordPCB, yappNoFillet]
];

labelsPlane = [
  [50, 33, 270, 1, yappLid, "Liberation Mono:style=bold", 7, "DVH" ]
];

//printBaseShell = false;
//printLidShell = false;

YAPPgenerate();