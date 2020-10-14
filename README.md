# Presence-detector

This detection system uses probe request packets and ESP 32 cards. 
Once 3 or more cards have been arranged in a room, it is possible to triangulate the signals and thus estimate how many people are present in a room.
An interface has been created in order to display the devices on a map of the room in a given time slot, and with a settable granularity in seconds. From the interface it is also possible to view statistics and count the devices that have the MAC address hidden and therefore not usable. Finally, from a console (visible in the third photo) it is possible to run queries in order to view the status of the database.

![Alt text](Images/1.PNG?raw=true "Initial view")

![Alt text](Images/2.PNG?raw=true "Detection")

![Alt text](Images/3.PNG?raw=true "Final view")
