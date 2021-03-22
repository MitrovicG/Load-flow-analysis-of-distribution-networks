# Load-flow-analysis-of-distribution-networks
Load flow method for radial and weakly-meshed distribution networks.

## Description of method
This method is based on the backward/forward sweep algorithm.  
* In order to obtain the load flow solution of a weakly-meshed network, the network is first converted to a radial network with the slack bus as its root.
* Next step is construction of sensitivity matrix:
  *  Diagonal elements are self impedances of the breakpoints and the off-diagonal elements are mutual impedances of two breakpoints.
* The iterative procedure starts with an initial guess of the current injection.
* After iterative procedure, voltages at the two sides of the breakpoints should be equal.
