# IR-Remote
For this project, an IoT-styled TV Controller has been designed and implemented. The focus throughout development has been to create a useful, fun and internet connected device that stands out from your usual TV Remote to offer fun extra features that may even be useful. In particular, we aim to make use and demonstrate our knowledge of the IoT by producing a device that best utilises the hardware available, providing as much network connectivity as possible whilst maintaining security. The main motivation for selecting this project is that I have a TV without a working remote, and would like to be able to retune and use this TV again!

# Design Criteria
The TV remote design can be split into two distinct parts. Firstly, the remote itself should be fully functional and do everything you expect a regular remote to do. Secondly, it should be as IoT driven as possible - i.e. not just a TV remote, but a remote that clearly demonstrates IoT-styled functionality. In particular, we want the remote to be highly network connected. Ideally, the solution would allow for input via a platform such as Adafruit or IFTTT such that a user could control the device from a distance. As a result, the remote could be placed permanently facing the TV so that if left on, you could just log in to the platform and turn it off. Also, it would be nice to use a platform to record and analyse usage - for example, which channel and I visiting most frequently? Such a design could be extended to provide more interesting analytics.

In terms of the code, an effective solution would clearly be object-oriented. A TV controller can easily be divided into a series of objects, behaviours and relationships; thus, making sense to structure the code in the same fashion. We aim to use good C++ practice, making use of header files etc. and object-oriented features to get as much power out of the solution as possible.


# Development Process
== Getting IR codes due to broken controller
== Transmitting IR codes via Serial Input
== Building a GUI
== Processing input from GUI into Adafruit.io
== Receiving input into GUI from Adafruit.io

# Finished Product

# Testing
| Situation                   | Comment                                    							 |  Result  |
| --------------------------- |:--------------------------------------------------------------------:|:--------:|
| Pressing Number Button      | Sends corresponding number to TV            						 | Success  |
| Pressing Circle Button      | Sends corresponding function to TV         			  				 | Success  |
| Pressing Square Button      | Sends corresponding function to TV         							 | Success  |
| Pressing Change Page Button | Changes page on remote                 							     | Success  |
| Pressing Triangle Button    | Sends corresponding function to TV                                   | Success  |
| Pressing Power Button       | Initially wouldn't switch TV on, made the remote repeatedly send the signal for half a second to simulate holding; now turns TV on and off. | Success  |	
| Holding Number Button       | Sends corresponding number to TV as long as held                     | Success  |
| Holding Circle Button       | Sends corresponding function to TV as long as held                   | Success  |
| Holding Square Button       | Sends corresponding function to TV as long as held                   | Success  |
| Holding Change Page Button  | Continuously changes pages back and forth                            | Success  |
| Holding Triangle Button     | Sends corresponding function to TV as long as held                   | Success  |
| Pressing button whilst connected to Adafruit.io | Sends corresponding number or function to TV, but experiences latency due to internet connection. | Needs Improving |
| Controlling controller from Adafruit.io | Can successfully use the numerical buttons from Adafruit | Success  |
| Updating frequently used number on Adafruit.io | When a button is pressed, it will successfully update a counter on Adafruit.io | Success  |




# Conclusion
== Things to improve
== What was done well
== How it is useful in IoT