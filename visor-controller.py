### WIP pseudocode for visor control
### REQUIREMENTS: - PocketSphinx (or alternative speech rec. engine)
### - Stepper motor control (either with Rasp or Arduino)
### TODO: Import pocket sphinx (or other) and integrate 

def visor(signal):
    # signal should be in format outputted by a speech rec system
    
    response = ""
    
    if "UP" in signal.upper():
        response = "UP"
    elif "DOWN" in signal.lower():
        response = "DOWN"
    else
        pass

    ## TODO: response must be processed by some control function
    ## and result in motor movement
    ## currently it is not clear how to do this
    return response

