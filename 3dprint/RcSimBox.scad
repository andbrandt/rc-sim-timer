$fn=25;

axisX = 1;
axisY = 2;
axisZ = 3;

innerFinger = 0;
outerFinger = 1;

// Reference system:
// Outer box bottom resides on z=0 plane
// Outer box front is aligned to x axis (y=0) and symmetrical to y axis

boxOuterWidth           = 430;  // x
boxOuterDepth           = 300;  // y
boxOuterHeight          = 80;   // z

materialThickness       = 6;    // Thickness of the board
fingerDepth             = materialThickness;    // Must always match
fingerTargetWidth       = fingerDepth*2;   // Actual width Must be calculated to match the actual edge length; Never to be less than this number
fingerCutCompensation   = -0.5;    // Makes cutting tool remove a little less (or add if negative) from all edges

airHoleDiameter         = 6;
airHoleSpacing          = 12;

//boxInnerWidth           = boxOuterWidth - materialThickness*2;
//boxInnerDepth           = boxOuterDepth - materialThickness;
//boxInnerHeight          = boxOuterHeight - materialThickness*2;

sideOffsetLeft          = (boxOuterWidth-materialThickness)/2;
sideOffsetRight         = sideOffsetLeft;
sideOffsetTop           = boxOuterHeight - materialThickness/2;
sideOffsetBottom        = + materialThickness/2;
sideOffsetFront         = + materialThickness/2;

timerFrameWidth         = 175;
timerFrameHeight        = 37;

// #################### BASIC BOX SHAPE ###########################

module Layer01_BoxSolid()
{    
    translate([0,boxOuterDepth/2,boxOuterHeight/2]) cube([boxOuterWidth,boxOuterDepth,boxOuterHeight], center=true);
}

module Layer03_BoxCutoutTimerFrame()
{        
    translate([0,boxOuterDepth/2,boxOuterHeight/2]) cube([timerFrameWidth,boxOuterDepth+0.01,timerFrameHeight], center=true);
}

module Layer04_BoxCutoutAirHoles()
{        
holeStepRangeY = abs((boxOuterDepth - airHoleSpacing*4)/(airHoleSpacing*2))*2;
holeStepRangeZ = abs((boxOuterHeight - airHoleSpacing*2)/(airHoleSpacing*2))*2;

    for (StepY = [-holeStepRangeY/2:holeStepRangeY/2])
    {
        for (StepZ = [-holeStepRangeZ/2:holeStepRangeZ/2])
        {
            translate([0,boxOuterDepth/2+StepY*airHoleSpacing,boxOuterHeight/2 + airHoleDiameter/2 + StepZ*airHoleSpacing]) rotate([0,90,0,]) cylinder(d=airHoleDiameter, h=boxOuterWidth+0.01, center=true);
        }    
    }
}

module Layer09_BoxWithCutouts()
{
    difference() {
        Layer01_BoxSolid();
        Layer03_BoxCutoutTimerFrame();
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

module Layer19_AllSides()   // Just for testing. No need for finger cutting
{    
    Layer11_SideLeft();
    Layer12_SideRight();
    Layer13_SideTop();
    Layer14_SideBottom();
    Layer15_SideFront();
}

// #################### FINGER MARKERS ###########################
function fingerNumber(edgeLength) = 2*floor(edgeLength/(fingerTargetWidth*2))+1; // Must be an odd number!

function fingerWidth(edgeLength) = edgeLength/fingerNumber(edgeLength);

module FingerMarkers(axis, edgeLength, innerOuterFinger)
{
    rotate(axis == axisZ ? [0,-90,0] : (axis == axisY ? [0,0,90] : [0,0,0])) {
        for (finger = [innerOuterFinger:2:fingerNumber(edgeLength)-1])
        {
            translate([(finger+0.5)*fingerWidth(edgeLength),0,0]) cube([fingerWidth(edgeLength)-fingerCutCompensation+0.01,fingerDepth-fingerCutCompensation+0.01,fingerDepth-fingerCutCompensation+0.01], center=true);
        }
    }
}

module Layer21_SideLeftWithFingers()
{
    difference() {
        Layer11_SideLeft();

        // Front edge
        translate([(-boxOuterWidth+fingerDepth)/2,sideOffsetFront,0]) FingerMarkers(axisZ, boxOuterHeight, innerFinger);

        // Side edges
        for (zPos=[+fingerDepth/2, boxOuterHeight-fingerDepth/2]) {
            translate([(-boxOuterWidth+fingerDepth)/2,fingerDepth-fingerCutCompensation,zPos]) FingerMarkers(axisY, boxOuterDepth-fingerDepth, outerFinger);
        }
    }
}

module Layer22_SideRightWithFingers()
{
     translate([boxOuterWidth-materialThickness,0,0]) Layer21_SideLeftWithFingers();    
}

module Layer23_SideTopWithFingers()
{        
    difference() {
        Layer13_SideTop();

        // Front edges
        translate([-boxOuterWidth/2,sideOffsetFront,boxOuterHeight-materialThickness/2]) FingerMarkers(axisX, boxOuterWidth, innerFinger);

        // Side edges
        for (edgeFactor=[-1:2:+1]) {
            translate([edgeFactor*(boxOuterWidth-fingerDepth)/2,fingerDepth-fingerCutCompensation,boxOuterHeight-fingerDepth/2]) FingerMarkers(axisY, boxOuterDepth-fingerDepth, innerFinger);
        }
    }
}

module Layer24_SideBottomWithFingers()
{     
     translate([0,0,-(boxOuterHeight-materialThickness)]) Layer23_SideTopWithFingers();
}

module Layer25_SideFrontWithFingers()
{        
    difference() {
        Layer15_SideFront();
        // Bottom and Top edges
        translate([-boxOuterWidth/2,sideOffsetFront,fingerDepth/2]) FingerMarkers(axisX, boxOuterWidth, outerFinger);

        translate([-boxOuterWidth/2,sideOffsetFront,boxOuterHeight-fingerDepth/2]) FingerMarkers(axisX, boxOuterWidth, outerFinger);
        
        // Left and Right edges
            for (edgeFactor=[-1:2:+1]) {
            translate([edgeFactor*(boxOuterWidth-fingerDepth)/2,sideOffsetFront,0]) FingerMarkers(axisZ, boxOuterHeight, outerFinger);
            }
    }
}


//Layer09_BoxWithCutouts();
//Layer19_AllSides();
//Layer15_SideFront();
Layer23_SideTopWithFingers();
Layer24_SideBottomWithFingers();
Layer25_SideFrontWithFingers();
Layer21_SideLeftWithFingers();
Layer22_SideRightWithFingers();