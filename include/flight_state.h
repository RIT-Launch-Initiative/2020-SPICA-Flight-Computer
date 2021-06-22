typedef enum {
    DISARMED,
    ARMED,
    BOOST,
    COAST,
    APOGEE,
    APOGEE_DELAY,
    DESCENT,
    LANDING
} flight_state_t;

// run this how often? schedule? or update as soon as measurements available
// have multiple updates for different measurements?
// sample idea for a function that takes accelerations, rotational velocities, barometric altitude, maybe GPS?
flight_state_t state = DISARMED; // assume this changes to armed somewhere else
// probably want to send state in telemetry packet
void update(ax, ay, az, rx, ry, rz, alt) {
    switch(state) {
        case DISARMED:
            // do nothing
            break;
        case ARMED:
            if(magnitude(ax,ay,az) > SOME_AMOUNT_OF_GS) { // get SOME_AMOUNT_OF_GS from flight configuration, magnitude could be a rolling average
                state++; // in boost now
            }
            break;
        case BOOST:
            if(magnitude_fromvert(ax,ay,az,rx,ry,rz) > SOME_VALUE_OF_1G) { // value of 1G depends on reading of sensor on ground, magnitude is from the ground (incorporates rotation) and may be a rolliing avg
                state++; // in coast now
            }
            break;
        case COAST:
            if(apogee_detected(ax,ay,az,rx,ry,rz,alt)) { // look for kalman filter Y velocity = 0, nose over, or altitude has gone down on average over multiple data points (rolling average negative)
                blow_drogues(); // either blow immediately or schedule to be blown later (e.g. with a delay)
                state++; // waiting for drogues to blow, apogee delay
                // blow_drogues will change state to descent when they are blown
            }
            break;
        case APOGEE_DELAY:
            // do nothing
            break;
        case DESCENT:
            if(landed(ax,ay,az,rx,ry,rz,alt) { // look for accleration = g, altitude doesnt change, GPS doesnt change, rotation doesnt change
                schedule_landing_task(); // probably shuts some things down, sends data at a lower rate (or only GPS), starts a buzzer that beeps
                state++; // change to landed
            }
            break;
        case LANDED:
            // do nothing
            break;
        default:
            // do nothing
            break;
    }
}
