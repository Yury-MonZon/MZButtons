# MZButtons / MikeZuluButtons
DIY USB HID controller for aviation simulators.  
![Image of MZButtons](https://github.com/Yury-MonZon/MZButtons/blob/master/Photos/IMG_20180505_092451.jpg)
![Image of MZButtons](https://github.com/Yury-MonZon/MZButtons/blob/master/Photos/IMG_20180510_130318.jpg)
Has a standard USB joystick interface with up to 128 buttons, 2 encoders, 4 switches.  
All done with inexpensive parts, easily obtainable at Chinese online shops.  
Based on Pro Micro, with the 3D printable expandable modular case.  

# Max number of buttons
The device can provide up to 128 buttons. Each rocker/toggle switch counts as 2 buttons, each encoder counts as 3 button. So the formula is:  
```
#if ((KEYPADS_NO*16)+(ENCODERS_NO*3)+(SWITCHES_NO*2) > 128)  
#error MAX allowed joystick buttons is 128!  
#endif  
```

# Suggestions wanted
Yup, I need someone to help me find the best layout for the decals and to add color-code grouping to them.  

# Status
The project is almost finished, check what's left below. The code and 3d printed case parts were printed/tested/debugged.  
**TODO:**
Install encoders and debug the corresponding code. I'm still waiting for the parcel with encoders to arrive.

# Files and Folders
In **Decals** folder you can find **Print_me.html** file in which you can easily create and customize your own labels for the buttons. Just print it out on any laser printer and cut along the lines.
**STL files** are here: https://www.thingiverse.com/thing:2836442  

# Bill of Materials
- Button matrix module 1-8 pcs   
($5 per piece with shipping)  
(manufacturer https://robotdyn.com/button-keypad-4x4-module.html)  
https://www.aliexpress.com/item/Button-Keypad-module-4x4-Analog-pin-connection/32635207928.html  

- Arduino Pro Micro 1pcs  
BEWARE Old versions are flashed with the faulty bootloader - you'll have to flash it with ISP programmer. If you don't have one - just buy the new version, it flashes fine via USB cable.  
($4.32 per piece with shipping)  
https://www.aliexpress.com/item/Free-Shipping-New-Pro-Micro-for-arduino-ATmega32U4-5V-16MHz-Module-with-2-row-pin-header/2040881593.html  

- Encoders 2pcs  
($0.72 per piece)  
https://www.aliexpress.com/item/Integrated-Circuits-rotary-encoder-ky-040-Brick-Sensor-Development-for-arduino-KY-040/32818097809.html  

- Toggle Switches 4pcs  
Sold as 5pcs pack.  
($1 per 5pcs pack)  
https://www.aliexpress.com/item/5pcs-Mini-MTS-102-3-Pin-SPDT-ON-ON-6A-125VAC-Miniature-Toggle-Switches/32813717258.html  

or

- Rocker Switches 4pcs  
Sold as 10pcs pack.  
($1 per 10pcs pack)  
https://www.aliexpress.com/item/10pcs-lot-15-10-mm-2PIN-Kcd1-Boat-Rocker-Switch-SPST-Snap-in-ON-OFF-Position/32811132757.html  

- Rubber feet ?pcs If you're going to use it flat on the table.  
Sold as 48pcs pack.  
($1 per 48pcs pack)  
https://www.aliexpress.com/item/Wholesale-2-48Pcs-Non-slip-Protectors-Furniture-Leg-Feet-Rubber-Pads-Felt-Pads-Self-Adhesive/32843489564.html  

# Schematics
There is no schematics as this project is very simple to assemble even if your soldering kung fu is not strong.  

- 16 buttons matrices  
are connected to A0, A1, A2, A3, 4, 6, 8, 9 pins on Arduino Pro Micro. Also, they use VCC and GND pins which could be chained from one module to another.  
- Rocker/Toggle switches  
should be connected to pins 10, 14, 15, 16 with the other wire connected to the GND pin.  
- Encoders  
The encoder's support has not been tested as I am still waiting for the parcel to arrive.  



# I like this thing! What can I do?
If you like this project and want to support me - drop me a message in Google Hangouts or e-mail at yurymonzon(w0of-wo0f)gmail (.) com :smiley_cat: