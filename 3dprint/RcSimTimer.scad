$fn=100;

frameFrontThickness = 2;
frameWallThickness  = 2;

usbBaseWidth        = 32;
usbBaseHeight       = 11;
usbBaseDepth        = 2;

usbWidth            = 13;
usbHeight           = 7;
usbDepth            = 17;

usbFrontOffset      = 0;



displayWidth        = 51;
displayHeight       = 20;
displayDepth        = 8;

displayBaseWidth    = 72;
displayBaseHeight   = 28;
displayBaseDepth    = 9;

displayFrontOffset  = 0;


buttonDiameterTop   = 20;
buttonDiameterBottom= 25;
buttonDepth         = 2;

buttonBaseDiameter  = 23;
buttonBaseDepth     = 14;

buttonFrontOffset   = frameFrontThickness;


frameWidth          = displayWidth*2+usbWidth*2+buttonDiameterBottom*2;
frameHeight         = displayHeight*2;
frameDepth          = frameFrontThickness;

frameBaseWidth      = frameWidth-4;
frameBaseHeight     = frameHeight-4;
//frameBaseDepth      = displayBaseDepth;
frameBaseDepth      = buttonBaseDepth-frameFrontThickness-0.1;


usbThingyWidth = usbBaseHeight*0.6;
usbThingyBridgeHeight = usbHeight*4;
usbThingyContactDepth = frameBaseDepth/2 - usbBaseDepth;

module Layer01a_UsbMountThingy()
{
    // Contact part
    translate([0,0,-usbThingyContactDepth/2- usbBaseDepth - 0.01]) cube([usbThingyWidth, usbBaseHeight-1, usbThingyContactDepth], center=true);
    
    // Bridge part
    translate([0,0,-frameBaseDepth*0.75-0.01]) cube([usbThingyWidth, usbThingyBridgeHeight, frameBaseDepth/2], center=true);


}

module Layer01b_UsbMountThingies()
{
    color("cyan") {    
        translate([(-usbBaseWidth/2+usbThingyWidth/2),0,0]) Layer01a_UsbMountThingy();
        translate([-(-usbBaseWidth/2+usbThingyWidth/2),0,0]) Layer01a_UsbMountThingy();
    }    
}


module Layer01_UsbCutout()
{
    color("red") {    
    
        // Frame cutout
        translate([0,0,+usbDepth/2-0.01]) cube([usbWidth+2, usbHeight+2, usbDepth], center=true);
    
        // Device base
        translate([0,0,-usbDepth/2]) cube([usbBaseWidth, usbBaseHeight, usbDepth], center=true);

        // Mount thingy cutout
        minkowski() {
            Layer01b_UsbMountThingies();
            cube(size=1, center=true);
        }
    }    
}

module Layer02_DisplayCutout()
{
    color("red") {
        // Frame cutout
        translate([0,0,+displayDepth/2-0.01]) cube([displayWidth-2, displayHeight-2, displayDepth], center=true);
        
        // Actual display
        translate([0,0,-displayDepth/2]) cube([displayWidth, displayHeight, displayDepth], center=true);

        // Display base
        translate([0,0,-displayDepth-displayBaseDepth/2+0.01]) cube([displayBaseWidth, displayBaseHeight, displayBaseDepth], center=true);
    }
}

module Layer03_PushbuttonCutout()
{
    color("red") {    
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
    translate([0,0,displayFrontOffset]) Layer02_DisplayCutout();
    translate([-displayBaseWidth+5,0,usbFrontOffset]) Layer01_UsbCutout();    
}


module Layer10_FrameSolid()
{
    // Actual Frame
    translate([0,0,+frameDepth/2]) cube([frameWidth, frameHeight, frameDepth], center=true);

    // Frame base
    translate([0,0,-frameBaseDepth/2]) cube([frameBaseWidth, frameBaseHeight, frameBaseDepth], center=true);
    
}

module Layer19_FinalFrame()
{
    difference() {
        Layer10_FrameSolid();
        Layer09_FullUiCutout();
    }
}

//Layer19_FinalFrame();
rotate([180,0,0]) Layer19_FinalFrame();
   
//translate([-displayBaseWidth,0,usbFrontOffset]) Layer01b_UsbMountThingies();
