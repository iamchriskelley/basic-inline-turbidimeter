$fn=30;

//translate([4,13,4]) rotate([0,-90,0]) 
light_shield();


module prism(hgt,sz){
rotate([0,0,0]) linear_extrude(height=hgt, center=true)
polygon(points = [ [0,0], [0,sz], [sz,sz]], paths = [ [0, 1, 2]]);
}

module light_shield(){
lightback();
translate([34.4,0,0]) rotate([0,0,180]) lightback();
translate([-4,10,0]) cube([42.4,6,1.2]);
translate([-4,-15,0]) cube([42.4,6,1.2]);
translate([-4,-15,0]) cube([42.4,6,1.2]);
translate([17.2,13.6,2.8]) rotate([0,90,0]) prism(42.4,2.4);
translate([17.2,-15,0.4]) rotate([90,0,90]) prism(42.4,2.4);
}

module lightback(){
    union(){
        difference(){
            //cylinder(r=4, h=2);
            translate([-4,-4,0]) cube([8,8,3]);
            //translate([-1,0,1.6]) rotate([0,15,0]) cylinder(r=10, h=3);
            translate([0,0,3]) rotate([0,15,0]) cube([18,18,3], true);
            translate([0,1.2,0])  cylinder(r=.8, h=3);
            translate([0,-1.2,0])  cylinder(r=.8, h=3);
        }
        translate([-4,4,0]) cube([8,6,1.2]);
        translate([-4,-10,0]) cube([8,6,1.2]);
        translate([4,-10,0]) cube([.8,20,.8]);
    }
}
