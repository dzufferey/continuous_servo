module support() {
    difference(){
        cube([50,63,6]);
        translate([2,2,1]) cube([50,59,5]);
        translate([1.2,1.2,3]) cube([50,60.6,2]);
    }
}

//rotate to print without overhang, but brim is advised
rotate([0,-90,0]) support();