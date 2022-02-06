#ifndef touch_interface_h
#define touch_interface_h

#define touch_XL 22 // Y+
#define touch_XR 27 // Y-
#define touch_YU 26 // X-
#define touch_YD 21 // X+

#define touch_XIN 0
#define touch_YIN 1

#define touch_sample_delay 10

//! \brief Return x value of touch on display. Returns 1000 if no touch detected
int touch_getX();

//! \brief Return y value of touch on display. Returns 1000 if no touch detected
int touch_getY();

//! \brief Initialize touch interface, setting up pins and ADC
void touch_init();

#endif