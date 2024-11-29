# EVENT HANDLING

producer writes to queue
* writes at most $limit to outbound queue
* hands over lock and waits for at least $thread_wait_time

consumer recieves from queue
* consumes at most $limit from inbound queue
* hands over lock and waits for at least $thread_wait_time


# PRIMARY SERVICES

## USER_INTERFACE 

reads inbound port, authenticates and sends events to categorizer.
sends events back to UI, when recieved from catagorizer.


## CATEGORIZER 

modifies events and directs them to where they need to go, this can be controlled by modes sent to it from
     
## MICROCONTROLLER

sends commands to microcontroller, 
reads events from micro controller (such as ultra sonic) and sends them to catagorizer.

## FLIGHT_CONTROLLER

sends events to flight controller
reads events from flight controller