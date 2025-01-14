# MSP 104
     
Command that is sent to the drone. While the mapper, and dedicated serializer will determine what each member of
the request payload will affect. The following gives a general description.


## Members

### Roll: Rotation around the X-axis.

Visualize this as tilting the drone to the left or right. If you imagine a cylinder representing the drone, rolling would rotate it along its length.

### Pitch: Rotation around the Y-axis.

This is akin to nodding your head up and down. For a drone, pitching forward means the front of the drone tilts downwards.

### Yaw: Rotation around the Z-axis.

This represents turning left or right without changing altitude. Picture turning your head side to side.

### Throttle:

Throttle controls altitude and is not a rotational movement but rather affects the Z  position in the coordinate system. Increasing throttle raises the drone vertically, while  decreasing it lowers the drone.

For the purposes of a land drone, throttle is the accelration or voltage sent to the  hbridge, 

## AUX1, AUX2, AUX3, and AUX4

Reserved for future use.