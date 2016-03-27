#Basic Inline Turbidimeter
##Purpose  
The Basic Inline Turbidimeter is designed to measure turbidity in a body of water, automatically, for the lowest possible price.  

##Function  
_How does the device measure a parameter?_  
This device is a back-scatter [nephelometrer](https://en.wikipedia.org/wiki/Nephelometer). A pair of infrared light-emitting diodes (IR LED) are mounted on either side of a light-to-frequency sensor. Some portion of this emitted light is scattered by suspended sediment as it passes through a sample of water. This scattered light is detected by an IR-sensitive [photodiode](https://en.wikipedia.org/wiki/Photodiode) mounted at a right angle to the center of the IR LED's light beam. The intensity of light detected by the photodiode is referenced to a calibration curve stored in the device's memory to interpolate the corresponding turbidity value, measured in [nephelometric turbidity units (NTU)](http://or.water.usgs.gov/grapher/fnu.html).  

_How does the device record data?_  
This device contains an SD card slot for storing data.   

_How does the device display data?_  
This device is intended for standalone data logging and does not include a display.  

_What units does the device report in?_  
Like all AWQUA turbidimeters, this device reports readings in  [nephelometric turbidity units (NTU)](http://or.water.usgs.gov/grapher/fnu.html).  

_How does the device communicate data electronically?_  
This device does not possess data telemetry capabilities, however they may be added for additional cost. See the [Communications](https://github.com/AWQUA/Communication) repo for options. 

##Use Case  
_Where can the device be safely used?_  
This device can handle temperatures up to 70C, and is intended for field conditions. It is water proof to at least one foot.

##Power  
_How is the device powered?_  
This device uses 3.3V DC power, and requires less than 100mA of current, and uses an internal 3.7V lithium polymer battery.

_How much power does it use?_  
In automated testing, this device was capable of taking a measurement every 15 minutes for 29.5 days.

##Standard and Testing  
_How was the device tested?_  
This device has so far only been tested in the Open Source Water lab. The testing consisted of:
(1) immersing a prototype device in a water bath which contains a spin plate for bath mixing  
(2) adding small doses of a colloidal suspension to the water bath while relaying readings from the prototype via Bluetooth  every 10 seconds
(3) on every fourth dose of colloidal suspension, sampling the water bath with a commercial handheld turbidimeter  
(4) analyzing the fit between the commercial and prototype open-source turbidimeter

_How did it perform?_  
Test results may be found in the `testing` subdirectory in the file `basic-inline-turbidimeter_analysis.20151021.xlsx`. These data will be updated as further testing is conducted.

_What performance standards does the device meet?_  
We have not evaluated this device against existing standards for backscatter turbidimeters yet, nor have we yet developed a new standard.

##Construction  

###Bill of Materials
| Quantity  | Part Description | Example Price (each) |
| :-------------: | ------------- | :-------------: |
| 1 | ATMega328P-PU microprocessor  | [$2.90](http://www.electrodragon.com/product/atmega328-with-arduino-optiboot-uno/)|
| 1 | LMT86 temperature sensor  | [$1.00](http://www.mouser.com/ProductDetail/Texas-Instruments/LMT86LP/?qs=sGAEpiMZZMvfFCidbTccA1jyVRrxk5nB%252bTEjPRynSGM%3d)|
| 1 | TSL230BRD  | [$5.03](http://www.mouser.com/ProductDetail/ams/TSL230BRD-TR/?qs=sGAEpiMZZMu97qiQi8P%252buifvOPr%252b402N67DKHn09PFo%3d)|
| 1 | LP2985 3.3V Voltage regulator  | [$1.04](http://www.mouser.com/ProductDetail/Texas-Instruments/LP2985-33DBVT/?qs=paYhMW8qfivCywxvLCGbYQ%3D%3D)|
| 2 | 10uF aluminum electrolytic capacitors  | [$0.24](http://www.mouser.com/ProductDetail/Nichicon/ULD1J100MDD1TD/?qs=sGAEpiMZZMvwFf0viD3Y3c1N%2fF%2fpDmX12XlNW9X9Y3vZmYyMo5GpOw%3d%3d)|
| 2 | 22pF ceramic capacitors  | [$0.03](http://www.mouser.com/ProductDetail/Vishay-BC-Components/K220J10C0GF5UH5/?qs=sGAEpiMZZMsh%252b1woXyUXj9nJp%252b8gphztXIKBUu3gv%252bs%3d)|
| 5 | 100nF ceramic capacitors  | [$0.06](http://www.mouser.com/ProductDetail/Vishay-BC-Components/K104Z15Y5VE5TL2/?qs=sGAEpiMZZMsh%252b1woXyUXj5VNmf0jXPWIrWjnp1zAXac%3d)|
| 4 | 100 ohm resistors  | [$0.10](http://www.mouser.com/ProductDetail/KOA-Speer/MOS1-2CT52R101J/?qs=sGAEpiMZZMtlubZbdhIBICYVxaT5CDYBpEIIN%2f0BoAk%3d)|
| 1 | 220 ohm resistors  | [$0.10](http://www.mouser.com/ProductDetail/KOA-Speer/MOS1-2CT52R221J/?qs=sGAEpiMZZMtlubZbdhIBICYVxaT5CDYBStqm8FrIljg%3d)|
| 1 | 1K ohm resistors 1% tolerance | [$0.10](http://www.mouser.com/ProductDetail/Vishay-Beyschlag/MBB02070C1001FCT00/?qs=sGAEpiMZZMtlubZbdhIBIAA6NYEPHyksoxlYyK2s%252btM%3d)|
| 4 | 10K ohm resistors  | [$0.10](http://www.mouser.com/ProductDetail/Vishay-Beyschlag/MBA02040C1002FCT00/?qs=sGAEpiMZZMtlubZbdhIBIP7908E9uONJr6Tgqqo7oLA%3d)|
| 1 | IR LED  | [$0.47](http://www.mouser.com/ProductDetail/Kingbright/WP7113SF4C/?qs=sGAEpiMZZMvAL21a%2fDhxMro1KOf4QOs%252bCw5JRgjZK8o%3d)|
| 1 | Momentary contact button  | [$1.50](http://www.mouser.com/)|
| 1 | SPST power switch  | [$1.50](http://www.mouser.com/)|
| x | Header pins (0.1" spacing)  | [$1.50](http://www.mouser.com/)|
|||
||Total Cost:|$xx.xx|
###Circuit Board
####Schematic
![picture alt](./hardware/basic-inline-turbidimeter.schematic.jpg "Schematic of Basic Handheld Turbidimeter circuit board")
####Layout
![picture alt](./hardware/basic-inline-turbidimeter.board.jpg "Layout of Basic Handheld Turbidimeter circuit board")
###Assembly
[forthcoming]  
##FAQ and Troubleshooting  
###Online Resources
