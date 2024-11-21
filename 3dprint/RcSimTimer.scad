$fn=100;

frameFrontThickness = 2;
frameWallThickness  = 2;

usbFrontWidth        = 31;
usbFrontHeight       = 10;
usbFrontDepth        = 1.7;

usbBaseWidth            = 13;
usbBaseHeight           = 7;
usbBaseDepth            = 19;

usbFrontOffset      = 0;

UsbMountThingyWidth = 5;
UsbMountThingyHeight = UsbMountThingyWidth*1.5;

displayWidth        = 51;
displayHeight       = 20;
displayDepth        = 8;

displayBaseWidth    = 72;
displayBaseHeight   = 28;
displayBaseDepth    = usbBaseDepth;

displayFrontOffset  = frameFrontThickness-0.4;


buttonDiameterTop   = 20;
buttonDiameterBottom= 25;
buttonDepth         = 2;

buttonBaseDiameter  = 23;
buttonBaseDepth     = usbBaseDepth;

buttonFrontOffset   = frameFrontThickness;


frameWidth          = displayWidth*2+usbBaseWidth*2+buttonDiameterBottom*2;
frameHeight         = displayHeight*2;
frameDepth          = frameFrontThickness;

frameBaseWidth      = frameWidth-4;
frameBaseHeight     = frameHeight-4;
frameBaseDepth      = usbBaseDepth-frameFrontThickness-0.1;


//usbThingyWidth = usbFrontHeight*0.6;
//usbThingyBridgeHeight = usbBaseHeight*4;
//usbThingyContactDepth = frameBaseDepth/2 - usbFrontDepth;

usbXcenterPos = -displayBaseWidth+5;

module Layer01a_UsbMountThingy()
{    
    translate([0,0,-UsbMountThingyWidth/2]) cube([UsbMountThingyWidth,frameBaseHeight+0.01,UsbMountThingyWidth], center=true);

    translate([0,0,+UsbMountThingyWidth/2-UsbMountThingyHeight]) rotate([90,0,0]) cylinder(h=frameBaseHeight+0.01, d=UsbMountThingyWidth, center=true);
}

module Layer01b_UsbMountThingies(distance,scale) 
{
    for(xPos = [-distance/2, +distance/2]) {
        translate([xPos,0,usbFrontOffset-usbFrontDepth])scale([scale,1,scale])
Layer01a_UsbMountThingy();        
    }
}

module Layer01c_UsbMountThingiesFitSelection() 
{
    for(count = [3,4,5,6,7]) {
        translate([UsbMountThingyWidth*(count-4)*2.5,0,usbFrontOffset])Layer01b_UsbMountThingies(0, (UsbMountThingyHeight-count*0.1)/UsbMountThingyHeight);
    }
}

module Layer01_UsbCutout()
{
    color("green") {    
    
        // Frame cutout
        translate([0,0,+usbBaseDepth/2-0.01]) cube([usbBaseWidth+2, usbBaseHeight+2, usbBaseDepth], center=true);
    
        // Device base
        translate([0,0,-usbBaseDepth/2]) cube([usbFrontWidth+0.25, usbFrontHeight+0.25, usbBaseDepth], center=true);

    translate([0,0,usbFrontOffset])Layer01b_UsbMountThingies(usbFrontWidth-UsbMountThingyWidth, 1);
    }    
}

module Layer02_DisplayCutout()
{
    color("red") {
//        // Frame cutout
//        translate([0,0,+displayDepth/2-0.01]) cube([displayWidth-2, displayHeight-2, displayDepth], center=true);
        
        // Actual display
        translate([0,0,-displayDepth/2]) cube([displayWidth, displayHeight, displayDepth], center=true);

        // Display base
        translate([0,0,-displayDepth-displayBaseDepth/2+0.01]) cube([displayBaseWidth, displayBaseHeight, displayBaseDepth], center=true);
    }
}

module Layer03_PushbuttonCutout()
{
    color("blue") {    
        // Actual button
        translate([0,0,+buttonDepth/2]) cylinder(h=buttonDepth, d1=buttonDiameterBottom, d2=buttonDiameterTop, center=true);

        // Button base
        translate([0,0,-buttonBaseDepth/2+0.01]) cylinder(h=buttonBaseDepth, d=buttonBaseDiameter, center=true);
        
        // Button base mount space
        translate([0,0,-frameFrontThickness-buttonBaseDepth/2+0.01]) cylinder(h=buttonBaseDepth, d=frameBaseHeight-frameWallThickness*2, center=true);
        
    }
}
    
module Layer09_FullUiCutout()
{
    translate([+displayBaseWidth-5,0,buttonFrontOffset+0.01]) Layer03_PushbuttonCutout();
    translate([0,0,displayFrontOffset+0.01]) Layer02_DisplayCutout();
    translate([usbXcenterPos,0,usbFrontOffset]) Layer01_UsbCutout();    
}


module Layer10_FrameSolid(adder)
{
    scale([(frameBaseWidth+adder)/frameBaseWidth, (frameBaseHeight+adder)/frameBaseHeight, (frameBaseDepth+adder)/frameBaseDepth]) {
        // Actual Frame
        translate([0,0,+frameDepth/2]) cube([frameWidth, frameHeight, frameDepth], center=true);

        // Frame base
        translate([0,0,-frameBaseDepth/2]) cube([frameBaseWidth, frameBaseHeight, frameBaseDepth], center=true);
    }    
}

module Layer11_BackCoverStripHoleCutout(depth)
{
    for(xPos = [-displayBaseWidth-10, +displayBaseWidth+10]) {
        hull() {
            translate([xPos/2,0,-UsbMountThingyWidth/2]) rotate([90,0,0]) cylinder(d=UsbMountThingyWidth, h=frameBaseHeight+10, center=true);
            translate([xPos/2,0,-UsbMountThingyWidth/2-depth]) rotate([90,0,0]) cylinder(d=UsbMountThingyWidth, h=frameBaseHeight+10, center=true);
        }
    }
}

module Layer12_BackCoverCableHoleCutout(depth, length)
{
    hull() {
        translate([0,0,0]) rotate([0,90,0]) cylinder(d=UsbMountThingyWidth, h=length, center=true);

    translate([0,0,0-depth]) rotate([0,90,0]) cylinder(d=UsbMountThingyWidth, h=length, center=true);
    }
}


module Layer19_FinalFrame()
{
    difference() {
        Layer10_FrameSolid(0);
        Layer09_FullUiCutout();
        translate([0,0,-UsbMountThingyWidth*1.0])Layer11_BackCoverStripHoleCutout(0);
        color("chocolate")         translate([+frameBaseWidth*0.5,frameBaseHeight*0.37,-UsbMountThingyWidth/2-3]) Layer12_BackCoverCableHoleCutout(12,frameBaseWidth/5);

    color("aqua")         translate([+frameBaseWidth*0.25,0,-UsbMountThingyWidth*2.5]) Layer12_BackCoverCableHoleCutout(7,frameBaseWidth/5);

    color("lime")         translate([-frameBaseWidth*0.25,frameBaseHeight*0.07,-UsbMountThingyWidth*2.5]) Layer12_BackCoverCableHoleCutout(7,frameBaseWidth/5);
    }
}

module Layer20_BackCover()
{
    difference() {
        translate([0,0,-(frameBaseDepth+frameWallThickness)/2]) cube([frameBaseWidth+frameWallThickness*2, frameBaseHeight+frameWallThickness*2, frameBaseDepth+frameWallThickness], center=true);
        translate([0,0,+0.01]) Layer10_FrameSolid(0.5);
        translate([0,0,-UsbMountThingyWidth*0.6]) Layer11_BackCoverStripHoleCutout(8);
        color("chocolate")         translate([+frameBaseWidth*0.5,frameBaseHeight*0.37,-UsbMountThingyWidth/2+2+4]) Layer12_BackCoverCableHoleCutout(10,frameBaseWidth/5);

    }
}

Layer19_FinalFrame();
//Layer20_BackCover();
//Layer01c_UsbMountThingiesFitSelection();