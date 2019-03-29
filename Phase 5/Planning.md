
Challenge 3:

Speedometer
Safety System 
Gear Indicator (Drive, Reverse, Park, Neutral)
Honk on boot

Tasks:
Soldering
Assembly (Laser cutting)
Testing

Go beyond:

Bluetooth, App
# Bot Dimensions

Approximately 4x8 Inch layout
Potentially stack protoboards.




# Profile View of bot layout

______ = a layer of support material (wood, foam, etc...)


                                                          
                                                              "Comms"
                                                  _________________________________
                                                              "H-Bridge"
            "Collision Detection"     Arduino     2x 9V batteries (for +9V to -9V)
    Bumper ________________________________________________________________________ Rear bumper
            "Sensors"


## Legend

| Grouping             | List                                                                                |
| -------------------- | ----------------------------------------------------------------------------------- |
| Sensors              | Color detection, Hall effect proximity headlights horn                              |
| Collision Dectection | collision detection 7 seg, status, speedometer, maybe turn signals                  |
| h bridge             | h bridge circuit, break/reverse lights, turn signal, voltage regulator, rear bumper |
| Comms                | receiver transmitter                                                                |


Wire Convention


Black - Ground
White - Intermediate
Brown - Digital In brown
Grey  - Interrupt
Yellow - -V
Red    - +V
Orange - PWM
Blue   - Digital Out 
Green  - Analog IN
