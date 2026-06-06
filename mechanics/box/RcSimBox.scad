$fn=50;

// Tricks to cut objects in LightBurn larger than physical laser bed:
// 1: https://www.youtube.com/watch?v=n__saOKVupA
// 2: https://www.youtube.com/watch?v=U259xw-fxOI
//
// Alternatives including making alignment holes and insert dowels after cutting first part

// Tricks to make STL to PNG: (Alternative: Make SCAD Projection cut and export to DXF for another lightburn layer)
// A: https://fenrus75.github.io/FenrusCNCtools/javascript/stl2png.html
//

axisX = 1;
axisY = 2;
axisZ = 3;

innerFinger = 0;
outerFinger = 1;

// Reference system:
// Outer box bottom resides on z=0 plane
// Outer box front is aligned to x axis (y=0) and symmetrical to y axis

timerFrameWidth         = 175;
timerFrameHeight        = 37;

materialThickness       = 6;    // Thickness of the board

boxOuterWidth           = 430;  // x
boxOuterDepth           = 310;  // y
boxOuterHeight          = max(50, timerFrameHeight+materialThickness*4);   // z
labelFontSize           = 8;

//boxOuterWidth           = 80;  // x
//boxOuterDepth           = 60;  // y
//boxOuterHeight          = 30;   // z
//labelFontSize           = 5.5;

fingerDepth             = materialThickness;    // Must always match
fingerTargetWidth       = fingerDepth*2;   // Actual width Must be calculated to match the actual edge length; Never to be less than this number
fingerCutCompensation   = 0; // -0.8;    // Makes cutting tool remove a little less (or add if negative) from all edges

airHoleDiameter         = 8;
airHoleSpacing          = 12;

//boxInnerWidth           = boxOuterWidth - materialThickness*2;
//boxInnerDepth           = boxOuterDepth - materialThickness;
//boxInnerHeight          = boxOuterHeight - materialThickness*2;

sideOffsetLeft          = (boxOuterWidth-materialThickness)/2;
sideOffsetRight         = -sideOffsetLeft;
sideOffsetTop           = (boxOuterHeight - materialThickness)/2;
sideOffsetBottom        = -sideOffsetTop;
sideOffsetFront         = (-boxOuterDepth + materialThickness)/2;

// #################### BASIC BOX SHAPE ###########################

module Layer01_BoxSolid()
{    
    cube([boxOuterWidth,boxOuterDepth,boxOuterHeight], center=true);
}

module Layer03_BoxCutoutTimerFrame()
{        
    cube([timerFrameWidth,boxOuterDepth+0.01,timerFrameHeight], center=true);
}

module Layer03b_BoxCutoutTimerFrameMountHole()
{        
    translate([0,(-boxOuterDepth/2+timerFrameHeight)+(boxOuterHeight-timerFrameHeight),-timerFrameHeight]) rotate([90,0,0]) cube([timerFrameWidth,timerFrameHeight+0.01,timerFrameHeight*2], center=true);
}


module Layer04_BoxCutoutAirHoles()
{        
holeStepRangeY = floor((boxOuterDepth - airHoleSpacing*4)/(airHoleSpacing*2))*2;
holeStepRangeZ = floor((boxOuterHeight - airHoleSpacing*2)/(airHoleSpacing*2))*2;

    for (StepY = [-holeStepRangeY/2:holeStepRangeY/2])
    {
        for (StepZ = [-holeStepRangeZ/2:holeStepRangeZ/2])
        { 
            translate([0,StepY*airHoleSpacing,StepZ*airHoleSpacing]) rotate([0,90,0,]) cylinder(d=airHoleDiameter, h=boxOuterWidth+0.01, center=true);
        }    
    }
}

module Layer09_BoxWithCutouts()
{
    difference() {
        Layer01_BoxSolid();
        Layer03_BoxCutoutTimerFrame();
        Layer03b_BoxCutoutTimerFrameMountHole();        
        Layer04_BoxCutoutAirHoles();
    }
}

// #################### ISOLATED BOX SIDES###########################

module Layer11_SideLeft()
{    
    intersection() {
        Layer09_BoxWithCutouts();
        translate([-sideOffsetLeft,0,0]) cube([materialThickness,boxOuterDepth*2,boxOuterHeight*2], center=true);
    }
}

module Layer12_SideRight()
{    
    intersection() {
        Layer09_BoxWithCutouts();
        translate([+sideOffsetLeft,0,0]) cube([materialThickness,boxOuterDepth*2,boxOuterHeight*2], center=true);
    }
}

module Layer13_SideTop()
{    
    intersection() {
        Layer09_BoxWithCutouts();
        translate([0,0,sideOffsetTop]) cube([boxOuterWidth*2,boxOuterDepth*2,materialThickness], center=true);
    }
}

module Layer14_SideBottom()
{    
    intersection() {
        Layer09_BoxWithCutouts();
        translate([0,0,sideOffsetBottom]) cube([boxOuterWidth*2,boxOuterDepth*2,materialThickness], center=true);
    }
}

module Layer15_SideFront()
{    
    difference() {
        intersection() {
            Layer09_BoxWithCutouts();
            translate([0,sideOffsetFront,0]) cube([boxOuterWidth*2,materialThickness,boxOuterHeight*2], center=true);
        }
        
    }
}

//module Layer19_AllSides()   // Just for testing. No need for finger cutting
//{    
//    Layer11_SideLeft();
//    Layer12_SideRight();
//    Layer13_SideTop();
//    Layer14_SideBottom();
//    Layer15_SideFront();
//}

// #################### FINGER MARKERS ###########################
function fingerNumber(edgeLength) = 2*floor(edgeLength/(fingerTargetWidth*2))+1; // Must be an odd number!

function fingerWidth(edgeLength) = edgeLength/fingerNumber(edgeLength);

module FingerMarkers(axis, edgeLength, innerOuterFinger)
{
    rotate(axis == axisZ ? [0,-90,0] : (axis == axisY ? [0,0,90] : [0,0,0])) {
        for (finger = [innerOuterFinger:2:fingerNumber(edgeLength)-1])
        {
            translate([(finger+0.5)*fingerWidth(edgeLength)-edgeLength/2,0,0]) cube([fingerWidth(edgeLength)-fingerCutCompensation+0.01,fingerDepth-fingerCutCompensation+0.01,fingerDepth-fingerCutCompensation+0.01], center=true);
        }
    }
}


module Layer21_SideLeftWithFingers(explodePosScale=[1,1,1])
{
    translate([(-boxOuterWidth/2)*(explodePosScale.x-1),(explodePosScale.y-1),(explodePosScale.z-1)]) {
        difference() {
            Layer11_SideLeft();

            // Front edge
            translate([(-boxOuterWidth+fingerDepth)/2,sideOffsetFront,0]) FingerMarkers(axisZ, boxOuterHeight, innerFinger);

            // Side edges
            for (zPos=[-sideOffsetTop, +sideOffsetTop]) {
                translate([(-boxOuterWidth+fingerDepth)/2,fingerDepth/2,zPos]) FingerMarkers(axisY, boxOuterDepth-fingerDepth, outerFinger);
            }
        }
    }
}

module Layer22_SideRightWithFingers(explodePosScale=[1,1,1])
{
    translate([(+boxOuterWidth/2)*(explodePosScale.x-1),(explodePosScale.y-1),(explodePosScale.z-1)]) {
        translate([boxOuterWidth-materialThickness,0,0]) Layer21_SideLeftWithFingers();
    }
}

module Layer23_SideTopWithFingers(explodePosScale=[1,1,1])
{        
    translate([(explodePosScale.x-1),(explodePosScale.y-1),(+boxOuterHeight/2)*(explodePosScale.z-1)]) {
        difference() {
            Layer13_SideTop();

            // Front edge
            translate([0,sideOffsetFront,(boxOuterHeight-materialThickness)/2]) FingerMarkers(axisX, boxOuterWidth, innerFinger);

            // Side edges
            for (edgeFactor=[-1:2:+1]) {
                translate([edgeFactor*(boxOuterWidth-fingerDepth)/2,fingerDepth/2,(boxOuterHeight-fingerDepth)/2]) FingerMarkers(axisY, boxOuterDepth-fingerDepth, innerFinger);
            }
        }
    }
}

module Layer24_SideBottomWithFingers(explodePosScale=[1,1,1])
{     
    translate([(explodePosScale.x-1),(explodePosScale.y-1),(-boxOuterHeight/2)*(explodePosScale.z-1)]) {
        difference() {
            Layer14_SideBottom();

            // Front edge
            translate([0,sideOffsetFront,-(boxOuterHeight-materialThickness)/2]) FingerMarkers(axisX, boxOuterWidth, innerFinger);

            // Side edges
            for (edgeFactor=[-1:2:+1]) {
                translate([edgeFactor*(boxOuterWidth-fingerDepth)/2,fingerDepth/2,(-(boxOuterHeight-fingerDepth)/2)]) FingerMarkers(axisY, boxOuterDepth-fingerDepth, innerFinger);
            }
        }
    }
}

module Layer25_SideFrontWithFingers(explodePosScale=[1,1,1])
{        
    translate([(explodePosScale.x-1),(-boxOuterDepth/2)*(explodePosScale.y-1),(explodePosScale.z-1)]) {
        difference() {
            Layer15_SideFront();
            
            // Bottom and Top edges
            for (edgeFactor=[-1,+1]) {
                translate([0,sideOffsetFront,edgeFactor*(boxOuterHeight-fingerDepth)/2]) FingerMarkers(axisX, boxOuterWidth, 
                    outerFinger);
            }        
            
            // Left and Right edges
            for (edgeFactor=[-1,+1]) {
                translate([edgeFactor*(boxOuterWidth-fingerDepth)/2,sideOffsetFront,0]) FingerMarkers(axisZ, boxOuterHeight, outerFinger);
            }
        }
    }
}

// ###############################################
module SideName(sideName) {
    color("DimGray") translate([-16,0,-boxOuterWidth*0.75]) linear_extrude(height = boxOuterWidth) text(sideName, font = "Liberation Mono", size = labelFontSize, valign = "center");
}

module Layer31_SideLeftFlattened()
{     
    projection(cut = false)
    difference() {
        rotate([0,-90,0]) Layer21_SideLeftWithFingers();
    translate([-5,-boxOuterDepth/2+14,0]) rotate([0,0,180]) SideName("Left");
    }
}

module Layer32_SideRightFlattened()
{     
    projection(cut = false)
    difference() {
        rotate([0,90,0]) Layer22_SideRightWithFingers();
    translate([-5,-boxOuterDepth/2+14,0]) rotate([0,0,180]) SideName("Right");
    }    
}

module Layer33_SideTopFlattened()
{     
    projection(cut = false)
    difference() {
        rotate([0,180,0]) Layer23_SideTopWithFingers();
    translate([-5,-boxOuterDepth/2+14,0]) rotate([0,0,180]) SideName("Top");
    }    
}

module Layer34_SideBottomFlattened()
{     
    projection(cut = false)
    difference() {
        Layer24_SideBottomWithFingers();
    translate([-5,-boxOuterDepth/2+14,0]) rotate([0,0,180]) SideName("Bottom");
    }    
}

module Layer35_SideFrontFlattened()
{     
    projection(cut = false)
    difference() {
        rotate([90,0,0]) Layer25_SideFrontWithFingers();
    translate([boxOuterWidth*0.25,-boxOuterHeight/2+14,0]) rotate([0,0,180]) SideName("Front");
    }    
}

sideToFlatten = "None";
echo(sideToFlatten);

//explodedDrawingFactor=2.5*[0.5,0.8,1];
explodedDrawingFactor=1.005*[1,1,1];

if      (sideToFlatten=="None") {
    Layer23_SideTopWithFingers(explodedDrawingFactor);
    Layer24_SideBottomWithFingers(explodedDrawingFactor);
    Layer25_SideFrontWithFingers(explodedDrawingFactor);
    Layer21_SideLeftWithFingers(explodedDrawingFactor);
    Layer22_SideRightWithFingers(explodedDrawingFactor);
}
else if      (sideToFlatten=="Left")
    Layer31_SideLeftFlattened();
else if (sideToFlatten=="Right")
    Layer32_SideRightFlattened();
else if (sideToFlatten=="Top")
    Layer33_SideTopFlattened();
else if (sideToFlatten=="Bottom")
    Layer34_SideBottomFlattened();
else if (sideToFlatten=="Front")
    Layer35_SideFrontFlattened();
// endif

