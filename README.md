KeyDetector
===========

Arduino library for detecting state change on analog and digital pins. Listens for specified values on the analog input and sets KeyDetector object state accordingly. Detects digital pin state change as well.

Can be used to detect key press events that were assigned distinctive levels of the single analog signal ("multiplexed" to analog signal), i.e. by usig DAC to "encode" multiple digital signals to a single analog line.