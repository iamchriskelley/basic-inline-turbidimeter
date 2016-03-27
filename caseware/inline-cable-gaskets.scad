$fn=30;
difference(){
    cube([34,28,7]);
    translate([2,2,3]) difference(){
        cube([30,11,4]);
        translate([15,5.5,0]) cylinder(r=2.5, h=4);
    }
    translate([2,15,3]) difference(){
        cube([30,11,4]);
        translate([15,5.5,0]) cylinder(r=2.5, h=4);
    }
}