$fn=60;

//to fit motor mounting bracket: https://www.pololu.com/file/download/2676-bracket-dimensions.pdf?file_id=0J825

tolerance = 0.1;
offsetX1 = 9.5;
offsetX2 = offsetX1 + 6.35 * 5 ;
offsetY1 = (25-21.7)/2;
offsetY2 = 25 - offsetY1;
d=3.3;
r=3.3/2 - tolerance;

module neg() {
    difference() {
        translate([0,-tolerance])square([52,25+2*tolerance]);
        translate([offsetX1,offsetY1-r]) square(d, true);
        translate([offsetX1,offsetY1]) circle(r);
        translate([offsetX2,offsetY1-r]) square(d, true);
        translate([offsetX2,offsetY1]) circle(r);
        translate([offsetX1,offsetY2+r]) square(d, true);
        translate([offsetX1,offsetY2]) circle(r);
        translate([offsetX2,offsetY2+r]) square(d, true);
        translate([offsetX2,offsetY2]) circle(r);
    }
}

difference() {
    minkowski() {
        translate([3,3,0]) cube([46,44,4]);
        cylinder(r=3,h=1);
    }
    translate([0,12.5,2.5]) linear_extrude(h=3) neg();
}
