# IR-Remote
IR Remote

# Contents

# Design Criteria

# Development Process
== Getting IR codes due to broken controller
== Transmitting IR codes via Serial Input
== Building a GUI
== Processing input from GUI into Adafruit.io
== Receiving input into GUI from Adafruit.io

# Finished Product

# Testing
| Situation                   | Comment                                    							 |  Result  |
| --------------------------- |:--------------------------------------------------------------------:| --------:|
| Pressing Number Button      | Sends corresponding number to TV            						  | $1600    |
| Pressing Circle Button      | Sends corresponding function to TV         			  				 |   $12    |
| Pressing Square Button      | Sends corresponding function to TV         							 |    $1    |
| Pressing Change Page Button | Changes page on remote                 							     |          |
| Pressing Triangle Button    | Sends corresponding function to TV                                   |          |
| Pressing Power Button       | Initially wouldn't switch TV on, made the remote repeatedly send the signal for half a second to simulate holding; now turns TV on and off. | Success |	
| Holding Number Button       | Sends corresponding number to TV as long as held                     |          |
| Holding Circle Button       | Sends corresponding function to TV as long as held                   |          |
| Holding Square Button       | Sends corresponding function to TV as long as held                   |          |
| Holding Change Page Button  | Continuously changes pages back and forth                            |          |
| Holding Triangle Button     | Sends corresponding function to TV as long as held                   |          |
| Pressing button whilst      |                                                                      |          |
| connected to Adafruit.io    | Sends corresponding number or function to TV, but                    |          |
|  							  |	experiences latency due to internet connection                       |          |




# Conclusion
== Things to improve
== What was done well
== How it is useful in IoT